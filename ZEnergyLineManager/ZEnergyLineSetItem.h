//======================================================
#ifndef ZENERGYLINESETITEM_H
#define ZENERGYLINESETITEM_H
//======================================================
#include <QList>
#include <QMap>
#include <QPair>
#include <QString>
//======================================================
class ZEnergyLineProperties
{
public:

    ZEnergyLineProperties();
    ZEnergyLineProperties(const QString& energyLineValue);
    ZEnergyLineProperties(const ZEnergyLineProperties &);

    QString zp_energyLineValue() const;
    bool zp_isVisible() const;
    void zp_setVisible(bool visible);

private:

    QString zv_energyLineValue;
    bool zv_visibility;

};
//======================================================
typedef QList<QPair<QString, QString> > PropertyList;
//======================================================
class ZEnergyLineSetItem
{
public:
    ZEnergyLineSetItem(int ZNumber, const QString& symbol,
                       PropertyList& propertyList);

    int zp_ZNumber() const;
    QString zp_symbol() const;

    bool zp_energyLineValue(const QString& lineName, QString &value);
    int zp_energyLineCount() const;
    QStringList zp_energyLineNameStringList() const;


private:

    // VARS
    int zv_ZNumber;
    QString zv_symbol;
    QMap<QString, ZEnergyLineProperties> zv_energyLinePropertyMap;

    // FUNCS


};
//======================================================
#endif // ZENERGYLINESETITEM_H
