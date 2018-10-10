//=================================================================
#ifndef ZSAMPLETASKDELEGATE_H
#define ZSAMPLETASKDELEGATE_H
//=================================================================
#include <QStyledItemDelegate>
//=================================================================
class ZSampleTaskDelegate : public QStyledItemDelegate
{
    Q_OBJECT
public:
    explicit ZSampleTaskDelegate(QObject *parent = nullptr);

    virtual QWidget* createEditor(QWidget * parent,
                                  const QStyleOptionViewItem & option,
                                  const QModelIndex & index ) const override;

    virtual void setEditorData(QWidget* editor, const QModelIndex& index) const override;
    virtual void updateEditorGeometry ( QWidget * editor,
                                        const QStyleOptionViewItem & option,
                                        const QModelIndex & index ) const override;

signals:

public slots:

};
//=================================================================
#endif // ZSAMPLETASKDELEGATE_H
