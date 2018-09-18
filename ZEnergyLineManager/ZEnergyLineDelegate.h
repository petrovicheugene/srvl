//======================================================================
#ifndef ZENERGYLINEDELEGATE_H
#define ZENERGYLINEDELEGATE_H
//======================================================================
#include <QStyledItemDelegate>
//======================================================================

//======================================================================
class ZEnergyLineDelegate : public QStyledItemDelegate
{
    Q_OBJECT
public:
    explicit ZEnergyLineDelegate(QObject *parent = nullptr);

    virtual void paint ( QPainter * painter, const QStyleOptionViewItem & option, const QModelIndex & index ) const;
    virtual QSize	sizeHint ( const QStyleOptionViewItem & option, const QModelIndex & index ) const;

signals:


public slots:


};
//======================================================================
#endif // ZENERGYLINEDELEGATE_H
