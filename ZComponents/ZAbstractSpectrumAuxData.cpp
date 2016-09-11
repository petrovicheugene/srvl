//================================================
#include "ZAbstractSpectrumAuxData.h"
//================================================
ZAbstractSpectrumAuxData::ZAbstractSpectrumAuxData(ZAbstractSpectrumAuxData::DataType type)
{
    zv_type = type;
}
//================================================
ZAbstractSpectrumAuxData::DataType ZAbstractSpectrumAuxData::zp_type() const
{
    return zv_type;
}
//================================================
