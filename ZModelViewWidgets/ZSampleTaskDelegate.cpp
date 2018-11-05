//=================================================================
#include "ZSampleTaskDelegate.h"
#include "ZSampleTaskDialog2.h"
#include <QDebug>
//=================================================================
ZSampleTaskDelegate::ZSampleTaskDelegate(QObject *parent)
    : QStyledItemDelegate(parent)
{

}
//=================================================================
QWidget* ZSampleTaskDelegate::createEditor ( QWidget * parent, const QStyleOptionViewItem & option, const QModelIndex & index ) const
{
    ZSampleTaskDialog2* editor = new ZSampleTaskDialog2;
    editor->setModal(true);
    return editor;
}
//=================================================================
void ZSampleTaskDelegate::setEditorData(QWidget* editor, const QModelIndex & index ) const
{
    if(!index.isValid())
    {
        return;
    }

    bool ok;
    int sampleTaskId = index.data(Qt::EditRole).toInt(&ok);
    if(!ok)
    {
        return;
    }

    ZSampleTaskDialog2* sampleTaskDialog2 = qobject_cast<ZSampleTaskDialog2*>(editor);
    if(!sampleTaskDialog2)
    {
        return;
    }

    sampleTaskDialog2->zp_loadSampleTask(sampleTaskId);
}
//=================================================================
void ZSampleTaskDelegate::updateEditorGeometry ( QWidget * editor,
                                                  const QStyleOptionViewItem & option,
                                                  const QModelIndex & index ) const
{

}
//=================================================================

