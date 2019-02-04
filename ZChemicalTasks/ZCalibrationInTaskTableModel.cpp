//=======================================================
#include "ZCalibrationInTaskTableModel.h"
#include "ZGeneral.h"

#include "ZControlAction.h"
//=======================================================

//=======================================================
ZCalibrationInTaskTableModel::ZCalibrationInTaskTableModel(QObject *parent) : QAbstractTableModel(parent)
{

}
//=======================================================
Qt::ItemFlags	ZCalibrationInTaskTableModel::flags(const QModelIndex & index) const
{
    Qt::ItemFlags flags;
    return flags;
}
//=======================================================
QModelIndex ZCalibrationInTaskTableModel::index(int row, int column, const QModelIndex &parent) const
{
    return QAbstractTableModel::index(row, column, parent);
}
//=======================================================
int	ZCalibrationInTaskTableModel::columnCount(const QModelIndex & parent) const
{
    return 3;
}
//=======================================================
int ZCalibrationInTaskTableModel::rowCount(const QModelIndex & parent) const
{
    return 0;
}
//=======================================================
QVariant ZCalibrationInTaskTableModel::data(const QModelIndex & index, int role) const
{
    return QVariant();
}
//=======================================================
bool ZCalibrationInTaskTableModel::setData(const QModelIndex & index, const QVariant & value, int role)
{
    return false;
}
//=======================================================
QVariant ZCalibrationInTaskTableModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    return QVariant();
}
//=======================================================
