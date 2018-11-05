//======================================================
#ifndef ZREADONLYSTYLEDITEMDELEGATE_H
#define ZREADONLYSTYLEDITEMDELEGATE_H
//======================================================
#include <QStyledItemDelegate>
//======================================================

//======================================================
class ZReadOnlyStyledItemDelegate : public QStyledItemDelegate
{
    Q_OBJECT
public:
    explicit ZReadOnlyStyledItemDelegate(QObject *parent = 0);

    virtual QWidget* createEditor(QWidget* parent, const QStyleOptionViewItem& option, const QModelIndex& index) const;

signals:

public slots:
};
//======================================================
#endif // ZREADONLYSTYLEDITEMDELEGATE_H
