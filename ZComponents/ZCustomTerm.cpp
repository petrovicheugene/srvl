//===================================================================
#include "ZCustomTerm.h"
//===================================================================
ZCustomTerm::ZCustomTerm(ZCalibration *parent) : ZAbstractTerm(parent)
{
   zv_type = TT_CUSTOM;
}
//===================================================================
bool ZCustomTerm::zp_calcValue(const ZAbstractSpectrum * spectrum, qreal& value)
{
    value = 0;
    return true;
}
//===================================================================
bool ZCustomTerm::zp_calcTermVariablePart(const ZAbstractSpectrum* spectrum, qreal &value)  // w/o factor
{
    return false;
}
//===================================================================
bool ZCustomTerm::zp_termBelongsToWindow(const ZCalibrationWindow* window) const
{
    return false;
}
//===================================================================
QList<qint64> ZCustomTerm::zp_termWindowIdList() const
{
    QList<qint64> windowIdList;
    return windowIdList;
}
//===================================================================
