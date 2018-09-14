//======================================================
#include "ZAbstractChemicalElementPropertyRepository.h"
//======================================================
ZAbstractChemicalElementPropertyRepository::ZAbstractChemicalElementPropertyRepository()
{
    zv_chemicalElementCount = 118;
}
//======================================================
ZAbstractChemicalElementPropertyRepository::~ZAbstractChemicalElementPropertyRepository()
{

}
//======================================================
int ZAbstractChemicalElementPropertyRepository::zp_chemicalElementCount() const
{
    return zv_chemicalElementCount;
}
//======================================================
QString ZAbstractChemicalElementPropertyRepository::zp_lastError() const
{
    return zv_errorMsg;
}
//======================================================
