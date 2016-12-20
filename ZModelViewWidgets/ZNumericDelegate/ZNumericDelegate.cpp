//=================================================================
#include "ZNumericDelegate.h"
#include "ZNumericEditor.h"
#include <QDoubleSpinBox>
#include <QLineEdit>
#include <QTextEdit>
#include <limits>
#include <QMessageBox>
#include <QDebug>
#include <QEvent>
#include <QKeyEvent>
//=================================================================
ZNumericDelegate::ZNumericDelegate(QWidget *parent) :
    QStyledItemDelegate(parent)
{
    zv_editorMin = (double)std::numeric_limits<long>::min();
    zv_editorMax = (double)std::numeric_limits<long>::max();
    zv_isEditorAlive = false;
    zv_inheritFontBoldFlag = true;
    zv_fontBoldFlag = false;
    zv_immediatellyValueChangeFlag = true;

}
//=================================================================
QWidget* ZNumericDelegate::createEditor ( QWidget * parent, const QStyleOptionViewItem & option, const QModelIndex & index ) const
{
    ZNumericEditor* editor = new ZNumericEditor(parent);
    connect(editor, SIGNAL(enterClicked()), this, SLOT(zh_onEditorEnterClick()));

    ZNumericDelegate* pThis = const_cast<ZNumericDelegate*>(this);
    emit zg_requestEditorMinMax(pThis, index);
    editor->setMinMax(zv_editorMin, zv_editorMax);

    QModelIndex* pIndex = const_cast<QModelIndex*>(&zv_editedIndex);
    *pIndex = index;

    if(zv_immediatellyValueChangeFlag)
    {
        ZNumericDelegate* pThis = const_cast<ZNumericDelegate*>(this);
        connect(editor, SIGNAL(valueChanged(double)), pThis, SLOT(zh_editorValueChanged(double)));
    }
    return editor;
}
//=================================================================
void	ZNumericDelegate::paint ( QPainter * painter, const QStyleOptionViewItem & option, const QModelIndex & index ) const
{
    QStyleOptionViewItem newOption(option);

    if(!zv_inheritFontBoldFlag)
    {
        newOption.font.setBold(zv_fontBoldFlag);
    }

    newOption.state =  newOption.state | QStyle::State_Active;
    QStyledItemDelegate::paint(painter, newOption, index);
}
//=================================================================
void	ZNumericDelegate::setEditorData ( QWidget * editor, const QModelIndex & index ) const
{
    QString val = index.model()->data(index, Qt::DisplayRole).toString();
    ZNumericEditor* numericEditor = qobject_cast<ZNumericEditor*>(editor);
    numericEditor->setText(val);
}
//=================================================================
void	ZNumericDelegate::setModelData ( QWidget * editor, QAbstractItemModel * model, const QModelIndex & index ) const
{
    ZNumericEditor* numericEditor = qobject_cast<ZNumericEditor*>(editor);
    QString valString  = numericEditor->text();

    bool ok;
    double val = valString.toDouble(&ok);
    bool removeLastTree = false;
    MessageType messageType = MT_NO_MESSAGE;

    if(val < zv_editorMin )
    {
        // QMessageBox::warning(0, tr("Limit exceed"),  tr("The value exceeds minimum limit."), QMessageBox::Ok) ;
        messageType = MT_MIN_LIMIT;
        valString = QString::number(zv_editorMin, 'g', 18);
        removeLastTree = true;
    }
    else if(val > zv_editorMax)
    {
        // QMessageBox::warning(0, tr("Limit exceed"),  tr("The value exceeds maximum limit."), QMessageBox::Ok) ;
        messageType = MT_MAX_LIMIT;
        valString = QString::number(zv_editorMax, 'g', 18);
        removeLastTree = true;
    }

    valString.replace(QRegExp(","), ".");
    valString.replace(QRegExp("e"), "E");
    bool powerTypeRecord = valString.contains(QRegExp("[eE]"));

    QStringList parts = valString.split(QRegExp("[,.Ee]"),QString::KeepEmptyParts);

    if(removeLastTree && parts.count() > 2)
    {
        QString fractionalPart = parts.value(1);
        int digitsToRemoveNumber = fractionalPart.count() - 15;
        if(digitsToRemoveNumber > 0)
        {
            fractionalPart.remove(fractionalPart.count() - digitsToRemoveNumber , digitsToRemoveNumber);
            parts[1] = fractionalPart;
        }
    }

    for(int i  = 0; i < parts.count(); i++)
    {
        if(parts.value(i) == "")
            parts[i] = "0";
    }

    QString newNumericString;
    for(int i = 0; i < parts.count(); i++)
    {

        newNumericString = newNumericString + parts.value(i);
        if((i == 0 && powerTypeRecord == false && parts.count() == 2) ||
                (i == 0 && powerTypeRecord == true && parts.count() == 3))
        {
            newNumericString = newNumericString + ".";
        }

        if( (i == 0 &&  powerTypeRecord == true && parts.count() == 2) ||
                (i == 1 && powerTypeRecord == true && parts.count() == 3) )
        {
            newNumericString = newNumericString + "E";
        }
    }

    numericEditor->setText(newNumericString);
    model->setData(index, QVariant(newNumericString));

    bool* pIsEditorAlive = const_cast<bool*>(&zv_isEditorAlive);

    if(messageType == MT_MAX_LIMIT)
    {
        QMessageBox::warning(0, tr("Limit exceed"),  tr("The value exceeds maximum limit."), QMessageBox::Ok) ;
        *pIsEditorAlive = false;
    }
    else if(messageType == MT_MIN_LIMIT)
    {
        QMessageBox::warning(0, tr("Limit exceed"),  tr("The value exceeds minimum limit."), QMessageBox::Ok) ;
        *pIsEditorAlive = false;
    }
    else
    {
        *pIsEditorAlive = true;
    }
}
//=================================================================
QSize ZNumericDelegate::sizeHint ( const QStyleOptionViewItem & option, const QModelIndex & index ) const
{
    return QStyledItemDelegate::sizeHint (option, index);
}
//=================================================================
void	ZNumericDelegate::updateEditorGeometry ( QWidget * editor, const QStyleOptionViewItem & option, const QModelIndex & index ) const
{
    QStyledItemDelegate::updateEditorGeometry (editor, option, index);
}
//=================================================================
void ZNumericDelegate::zp_setImmediatellyValueChange(bool flag)
{
    zv_immediatellyValueChangeFlag = flag;
}
//=================================================================
void ZNumericDelegate::zp_setEditorMinMax(double min, double max)
{
    if(min < max || min < std::numeric_limits<long>::min() || max > std::numeric_limits<long>::max())
    {
        zv_editorMin = min;
        zv_editorMax = max;
    }
}
//=================================================================
void ZNumericDelegate::zp_editorMinMax(double& min, double& max) const
{
    min = zv_editorMin;
    max = zv_editorMax;
}
//=================================================================
void ZNumericDelegate::zp_setFontBold(bool boldFlag)
{
    zv_inheritFontBoldFlag = false;
    zv_fontBoldFlag = boldFlag;
}
//=================================================================
void ZNumericDelegate::zh_onEditorEnterClick()
{
    ZNumericEditor* editor = qobject_cast<ZNumericEditor*>(sender());
    if(editor->text().isEmpty())
    {
        editor->setValue(0);
    }
    emit commitData(editor);
    if(zv_isEditorAlive)
    {
        emit closeEditor(editor);
    }

    emit zg_editNext(zv_editedIndex);
}
//=================================================================
void ZNumericDelegate::zh_editorValueChanged(double value)
{
    ZNumericEditor* editor = qobject_cast<ZNumericEditor*>(sender());
    if(!editor)
    {
        return;
    }

    QAbstractItemModel* model = const_cast<QAbstractItemModel*>(zv_editedIndex.model());
    setModelData (editor, model, zv_editedIndex);
}
//=================================================================
