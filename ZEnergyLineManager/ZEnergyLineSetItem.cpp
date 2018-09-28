//======================================================
#include "ZEnergyLineSetItem.h"
#include <QApplication>
//======================================================
const QString ZEnergyLineSetItem::zv_settingsSectionName("EnergyLineVisibility");
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
    zv_color = src.zv_color;
}
//======================================================
ZEnergyLineProperties::ZEnergyLineProperties(const QString& energyLineValue, QColor color)
{
    zv_energyLineValue = energyLineValue;
    zv_color = color;
    zv_visibility = false;
}
//======================================================
ZEnergyLineProperties::ZEnergyLineProperties(const QString& energyLineValue,
                      const QString& energyLineRelativeIntensity,
                      QColor color)
{
    zv_energyLineValue = energyLineValue;
    zv_energyLineRelativeIntensity = energyLineRelativeIntensity;
    zv_color = color;
    zv_visibility = false;
}
//======================================================
QString ZEnergyLineProperties::zp_energyLineValue() const
{
    return zv_energyLineValue;
}
//======================================================
QString ZEnergyLineProperties::zp_energyLineRelativeIntensity() const
{
    return zv_energyLineRelativeIntensity;
}
//======================================================
void ZEnergyLineProperties::zp_setEnergyLineRelativeIntensity(const QString relativeIntensity)
{
    zv_energyLineRelativeIntensity = relativeIntensity;
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
QColor ZEnergyLineProperties::zp_color() const
{
    return zv_color;
}
//======================================================
void ZEnergyLineProperties::zp_setColor(QColor color)
{
    zv_color = color;
}
//======================================================
// END ZEnergyLineProperties
//======================================================
ZEnergyLineSetItem::ZEnergyLineSetItem(int ZNumber,
                                       const QString &symbol,
                                       QObject *parent)
    : QObject(parent)
{
    qRegisterMetaType<PropertyList>("PropertyList");

    zv_ZNumber = ZNumber;
    zv_symbol = symbol;
}
//======================================================
ZEnergyLineSetItem::~ZEnergyLineSetItem()
{
    zh_saveVisibility();

    for(auto& key : zv_energyLinePropertyMap.keys())
    {
        emit zg_energyLineOperation(zv_symbol, key, EL_REMOVED);
    }
}
//======================================================
void ZEnergyLineSetItem::zp_createEnergyLines(const PropertyList &propertyList)
{
    QColor color;
    for(auto& property : propertyList)
    {
        if(property.first.contains(QChar('K'), Qt::CaseSensitive))
        {
            color = QColor(Qt::blue);
        }
        else if(property.first.contains(QChar('L'), Qt::CaseSensitive))
        {
            color = QColor(Qt::green);
        }
        else if(property.first.contains(QChar('M'), Qt::CaseSensitive))
        {
            color = QColor(Qt::red);
        }
        else
        {
            color = QColor(Qt::magenta);
        }

        zv_energyLinePropertyMap.insert(property.first, ZEnergyLineProperties(property.second, color));

        emit zg_energyLineOperation(zv_symbol, property.first, EL_INSERTED);
    }

    zh_restoreVisibility();
}
//======================================================
void ZEnergyLineSetItem::zp_loadRelativeIntensity(const PropertyList &propertyList)
{
    for(auto& property : propertyList)
    {
        if(!zv_energyLinePropertyMap.keys().contains(property.first))
        {
            continue;
        }
        zv_energyLinePropertyMap[property.first].zp_setEnergyLineRelativeIntensity(property.second);
        emit zg_energyLineOperation(zv_symbol, property.first, EL_PROPERTY_CHANGED);
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
                                            QString& value) const
{
    if(!zv_energyLinePropertyMap.keys().contains(lineName))
    {
        return false;
    }

    value = zv_energyLinePropertyMap[lineName].zp_energyLineValue();
    return true;
}
//======================================================
bool ZEnergyLineSetItem::zp_energyLineRelativeIntensity(const QString& lineName, QString &value) const
{
    if(!zv_energyLinePropertyMap.keys().contains(lineName))
    {
        return false;
    }

    value = zv_energyLinePropertyMap[lineName].zp_energyLineRelativeIntensity();
    return true;
}
//======================================================
bool ZEnergyLineSetItem::zp_isEnergyLineVisible(const QString& lineName, bool& visibility) const
{
    if(!zv_energyLinePropertyMap.keys().contains(lineName))
    {
        return false;
    }

    visibility = zv_energyLinePropertyMap[lineName].zp_isVisible();
    return true;
}
//======================================================
bool ZEnergyLineSetItem::zp_energyLineColor(const QString& lineName, QColor &color) const
{
    if(!zv_energyLinePropertyMap.keys().contains(lineName))
    {
        return false;
    }

    color = zv_energyLinePropertyMap[lineName].zp_color();
    return true;
}
//======================================================
bool ZEnergyLineSetItem::zp_setEnergyLineVisible(const QString& lineName, bool visibility)
{
    if(!zv_energyLinePropertyMap.keys().contains(lineName))
    {
        return false;
    }

    zv_energyLinePropertyMap[lineName].zp_setVisible(visibility);

    emit zg_energyLineOperation(zv_symbol, lineName, EL_VISIBILITY_CHANGED);

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
void ZEnergyLineSetItem::zh_restoreVisibility()
{
    QSettings settings;
    zh_openSettingsGroup(settings);
    QStringList keys = settings.childKeys();
    QVariant vData;
    zv_energyLinePropertyMap.keys();
    foreach(QString key, keys)
    {
        vData = settings.value(key);
        if(!zv_energyLinePropertyMap.keys().contains(key) || !vData.isValid() || !vData.canConvert<bool>())
        {
            continue;
        }

        zv_energyLinePropertyMap[key].zp_setVisible(vData.toBool());
        emit zg_energyLineOperation(zv_symbol, key, EL_VISIBILITY_CHANGED);

    }
    zh_closeSettingsGroup(settings);
}
//======================================================
void ZEnergyLineSetItem::zh_saveVisibility()
{
    QSettings settings;
    zh_openSettingsGroup(settings);

    QMap<QString, ZEnergyLineProperties>::const_iterator it;
    for(it = zv_energyLinePropertyMap.begin(); it != zv_energyLinePropertyMap.end(); it++)
    {
        settings.setValue(it.key(), QVariant(it.value().zp_isVisible()));
    }

    zh_closeSettingsGroup(settings);
}
//======================================================
void ZEnergyLineSetItem::zh_openSettingsGroup(QSettings& settings)
{
    settings.beginGroup(qApp->property("glAppVersion").toString());
    settings.beginGroup(zv_settingsSectionName);
    settings.beginGroup(QString::number(zv_ZNumber));
}
//======================================================
void ZEnergyLineSetItem::zh_closeSettingsGroup(QSettings& settings)
{
    while(settings.group().isEmpty())
    {
        settings.endGroup();
    }
}
//======================================================
