//======================================================================
#include "ZEnergyLineDelegate.h"
//======================================================================
ZEnergyLineDelegate::ZEnergyLineDelegate(QObject *parent) : QStyledItemDelegate(parent)
{

}
//======================================================================
void ZEnergyLineDelegate::paint(QPainter* painter, const QStyleOptionViewItem & option, const QModelIndex & index ) const
{
    QStyledItemDelegate::paint(painter, option, index);
}
//======================================================================
QSize ZEnergyLineDelegate::sizeHint(const QStyleOptionViewItem & option, const QModelIndex & index ) const
{
    return QStyledItemDelegate::sizeHint(option, index);
}
//======================================================================
