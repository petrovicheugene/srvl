//========================================================
#include "ZCalibrationFilterProxyModel.h"
//========================================================
ZCalibrationFilterProxyModel::ZCalibrationFilterProxyModel(QObject *parent) : QSortFilterProxyModel(parent)
{
    zv_id = -1;
}
//========================================================
void ZCalibrationFilterProxyModel::zp_setMeasuringConditionsFilter(int id)
{
    zv_id = id;
    //    zv_gainFactorFilter = gainFactor;
    //    zv_expositionFilter = exposition;
    invalidateFilter();
}
//========================================================
bool ZCalibrationFilterProxyModel::filterAcceptsRow(int sourceRow, const QModelIndex &sourceParent) const
{
    if(zv_id < 0)
    {
        return false;
    }

    QModelIndex index;
    QVariant vData;
    bool ok;

    // id
    index = sourceModel()->index(sourceRow, 5);
    if(!index.isValid())
    {
        return false;
    }

    vData = index.data(Qt::DisplayRole);
    if(!vData.isValid() || !vData.canConvert<int>())
    {
        return false;
    }

    int id = vData.toInt(&ok);
    if(!ok)
    {
        return false;
    }

    return id == zv_id;
}
//========================================================
