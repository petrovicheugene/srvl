//=========================================------------------------------------------
#ifndef ZCONCENTRATIONDELEGATE_H
#define ZCONCENTRATIONDELEGATE_H
//=========================================------------------------------------------
#include <QStyledItemDelegate>
//=========================================------------------------------------------
class QDoubleSpinBox;
class ZNumericEditor;
//=========================================------------------------------------------
class ZNumericDelegate : public QStyledItemDelegate
{
    Q_OBJECT
public:
    explicit ZNumericDelegate(QWidget *parent = 0);

    virtual QWidget *	createEditor ( QWidget * parent, const QStyleOptionViewItem & option, const QModelIndex & index ) const;
    virtual void	paint ( QPainter * painter, const QStyleOptionViewItem & option, const QModelIndex & index ) const;
    virtual void	setEditorData ( QWidget * editor, const QModelIndex & index ) const;
    virtual void	setModelData ( QWidget * editor, QAbstractItemModel * model, const QModelIndex & index ) const;
    virtual QSize	sizeHint ( const QStyleOptionViewItem & option, const QModelIndex & index ) const;
    virtual void	updateEditorGeometry ( QWidget * editor, const QStyleOptionViewItem & option, const QModelIndex & index ) const;

    void zp_setImmediatellyValueChange(bool);
    //bool editorEvent(QEvent *event, QAbstractItemModel *model, const QStyleOptionViewItem &option, const QModelIndex &index);
    void zp_setEditorMinMax(double min, double max) ;
    void zp_editorMinMax(double& min, double& max) const;
    void zp_setFontBold(bool);

protected:

    // VARS

    enum MessageType {MT_NO_MESSAGE, MT_MAX_LIMIT, MT_MIN_LIMIT};
    //ZNumericEditor* m_numericEditor;
    bool zv_fontBoldFlag;
    bool zv_inheritFontBoldFlag;
    double zv_editorMin;
    double zv_editorMax;

    QModelIndex zv_editedIndex;
    bool zv_isEditorAlive;
    bool zv_immediatellyValueChangeFlag;

    // FUNCS

signals:
    
    //void dataStored(QModelIndex) const;
    void zg_editNext(QModelIndex);
    void zg_requestEditorMinMax(ZNumericDelegate*, QModelIndex) const;

public slots:
    
protected slots:

    void zh_onEditorEnterClick();
    void zh_editorValueChanged(double value);

};
//=========================================------------------------------------------
#endif // ZCONCENTRATIONDELEGATE_H
