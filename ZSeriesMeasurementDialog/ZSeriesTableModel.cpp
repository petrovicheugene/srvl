//======================================================
#include "ZSeriesTableModel.h"
#include <QDebug>
//======================================================
ZSeriesTableModel::ZSeriesTableModel(QObject *parent)
    : ZDependentModel(parent)
{

}
//======================================================
int ZSeriesTableModel::columnCount(const QModelIndex & parent) const
{
    return 1;
}
//======================================================
int	 ZSeriesTableModel::rowCount(const QModelIndex & parent) const
{
    return 1;
}
//======================================================
QVariant	ZSeriesTableModel::data(const QModelIndex & index, int role) const
{
    return QVariant();
}
//======================================================
void ZSeriesTableModel::zp_reload(qint64 id)
{
    qDebug() << "RELOAD DATA" << id;
}
//======================================================

