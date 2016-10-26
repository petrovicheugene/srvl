//======================================================
#include "ZReadOnlyStyledItemDelegate.h"
//======================================================
ZReadOnlyStyledItemDelegate::ZReadOnlyStyledItemDelegate(QObject *parent) : QStyledItemDelegate(parent)
{

}
//======================================================
QWidget* ZReadOnlyStyledItemDelegate::createEditor(QWidget* parent, const QStyleOptionViewItem& option, const QModelIndex& index) const
{
    return 0;
}
//======================================================
