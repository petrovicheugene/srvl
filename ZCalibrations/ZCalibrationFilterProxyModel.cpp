//========================================================
#include "ZCalibrationFilterProxyModel.h"
//========================================================
ZCalibrationFilterProxyModel::ZCalibrationFilterProxyModel(QObject *parent) : QSortFilterProxyModel(parent)
{
    zv_gainFactorFilter = -1;
    zv_expositionFilter = -1;
}
//========================================================
void ZCalibrationFilterProxyModel::zp_setMeasuringConditionsFilter(int gainFactor, int exposition)
{
    zv_gainFactorFilter = gainFactor;
    zv_expositionFilter = exposition;
    invalidateFilter();
}
//========================================================
bool ZCalibrationFilterProxyModel::filterAcceptsRow(int sourceRow, const QModelIndex &sourceParent) const
{
    if(zv_gainFactorFilter < 0 && zv_expositionFilter < 0)
    {
        return false;
    }

    QModelIndex index;
    QVariant vData;
    bool ok;

    // gainFactor
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

    int gainFactor = vData.toInt(&ok);
    if(!ok)
    {
        return false;
    }

    // exposition
    index = sourceModel()->index(sourceRow, 6);
    if(!index.isValid())
    {
        return false;
    }

    vData = index.data(Qt::DisplayRole);
    if(!vData.isValid() || !vData.canConvert<int>())
    {
        return false;
    }

    int exposition = vData.toInt(&ok);
    if(!ok)
    {
        return false;
    }

    return exposition == zv_expositionFilter && gainFactor == zv_gainFactorFilter;
}
//========================================================
