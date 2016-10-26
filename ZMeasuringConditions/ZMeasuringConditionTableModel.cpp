//======================================================
#include "ZMeasuringConditionTableModel.h"
//======================================================
ZMeasuringConditionTableModel::ZMeasuringConditionTableModel(QObject *parent)
    : QAbstractTableModel(parent)
{

}
//======================================================
// override FUNCS
Qt::ItemFlags	ZMeasuringConditionTableModel::flags(const QModelIndex & index) const
{
    Qt::ItemFlags flags;
    return flags;
}
//======================================================
QModelIndex ZMeasuringConditionTableModel::index(int row, int column, const QModelIndex &parent) const
{
    return QAbstractTableModel::index(row, column, parent);
}
//======================================================
int	 ZMeasuringConditionTableModel::columnCount(const QModelIndex & parent) const
{
    return 5;
}
//======================================================
int	 ZMeasuringConditionTableModel::rowCount(const QModelIndex & parent) const
{
    return 2;
}
//======================================================
QVariant	ZMeasuringConditionTableModel::data(const QModelIndex & index, int role) const
{
    return QVariant();
}
//======================================================
bool	ZMeasuringConditionTableModel::setData(const QModelIndex & index, const QVariant & value, int role)
{
    return false;
}
//======================================================
QVariant	ZMeasuringConditionTableModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    return QVariant();
}
//======================================================
