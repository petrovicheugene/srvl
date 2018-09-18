//======================================================
#include "ZEnergyLineSetItem.h"
//======================================================
ZEnergyLineProperties::ZEnergyLineProperties()
{
    zv_energyLineValue = QString();
    zv_visibility = false;
}
//======================================================
ZEnergyLineProperties::ZEnergyLineProperties(const QString& energyLineValue)
{
    zv_energyLineValue = energyLineValue;
    zv_visibility = false;
}
//======================================================
ZEnergyLineProperties::ZEnergyLineProperties(const ZEnergyLineProperties& src)
{
    zv_energyLineValue = src.zv_energyLineValue;
    zv_visibility = src.zv_visibility;
}
//======================================================
QString ZEnergyLineProperties::zp_energyLineValue() const
{
    return zv_energyLineValue;
}
//======================================================
bool ZEnergyLineProperties::zp_isVisible() const
{
    return zv_visibility;
}
//======================================================
void ZEnergyLineProperties::zp_setVisible(bool visible)
{
    zv_visibility = visible;
}
//======================================================
// END ZEnergyLineProperties
//======================================================
ZEnergyLineSetItem::ZEnergyLineSetItem(int ZNumber,
                                       const QString &symbol,
                                       PropertyList& propertyList)
{
    zv_ZNumber = ZNumber;
    zv_symbol = symbol;

    for(auto& property : propertyList)
    {
        zv_energyLinePropertyMap.insert(property.first, ZEnergyLineProperties (property.second));
    }
}
//======================================================
int ZEnergyLineSetItem::zp_ZNumber() const
{
    return zv_ZNumber;
}
//======================================================
QString ZEnergyLineSetItem::zp_symbol() const
{
    return zv_symbol;
}
//======================================================
bool ZEnergyLineSetItem::zp_energyLineValue(const QString& lineName,
                                            QString& value)
{
    if(!zv_energyLinePropertyMap.keys().contains(lineName))
    {
        return false;
    }

    value = zv_energyLinePropertyMap[lineName].zp_energyLineValue();
    return true;
}
//======================================================
int ZEnergyLineSetItem::zp_energyLineCount() const
{
    return zv_energyLinePropertyMap.count();
}
//======================================================
QStringList ZEnergyLineSetItem::zp_energyLineNameStringList() const
{
    QStringList energyLineNameStringList = zv_energyLinePropertyMap.keys();
    return energyLineNameStringList;
}
//======================================================
