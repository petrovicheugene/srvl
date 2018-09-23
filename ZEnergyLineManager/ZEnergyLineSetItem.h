//======================================================
#ifndef ZENERGYLINESETITEM_H
#define ZENERGYLINESETITEM_H
//======================================================
#include <QObject>

#include <QColor>
#include <QList>
#include <QMap>
#include <QPair>
#include <QSettings>
#include <QString>
//======================================================

//======================================================
class ZEnergyLineProperties
{
public:

    ZEnergyLineProperties();
    ZEnergyLineProperties(const QString& energyLineValue);
    ZEnergyLineProperties(const QString& energyLineValue, QColor color);

    ZEnergyLineProperties(const ZEnergyLineProperties &);

    QString zp_energyLineValue() const;
    bool zp_isVisible() const;
    void zp_setVisible(bool visible);
    QColor zp_color() const;
    void zp_setColor(QColor color);

private:

    QString zv_energyLineValue;
    bool zv_visibility;
    QColor zv_color;

};
//======================================================
typedef QList<QPair<QString, QString> > PropertyList;
typedef int EnergyLineOperation;
//======================================================
class ZEnergyLineSetItem : public QObject
{
    Q_OBJECT
public:
    enum EnergyLineOperation{EL_REMOVED = 0,
                             EL_INSERTED = 1,
                             EL_VISIBILITY_CHANGED = 2,
                             EL_COLOR_CHANGED = 3};

    explicit ZEnergyLineSetItem(int ZNumber, const QString& symbol,
                                QObject* parent = nullptr);
    ~ZEnergyLineSetItem();

    int zp_ZNumber() const;
    QString zp_symbol() const;

    bool zp_energyLineValue(const QString& lineName, QString &value) const;
    bool zp_isEnergyLineVisible(const QString& lineName, bool &visibility) const;
    bool zp_energyLineColor(const QString& lineName, QColor &color) const;
    bool zp_setEnergyLineVisible(const QString& lineName, bool visibility);
    int zp_energyLineCount() const;
    QStringList zp_energyLineNameStringList() const;

    void zp_createEnergyLines(const PropertyList& propertyList);

signals:

    void zg_energyLineOperation(const QString& lineSymbol,
                                const QString &name,
                                EnergyLineOperation operationType);

private slots:

    void zh_restoreVisibility();
private:

    // VARS
    int zv_ZNumber;
    QString zv_symbol;
    QMap<QString, ZEnergyLineProperties> zv_energyLinePropertyMap;
    static const QString zv_settingsSectionName;

    // FUNCS
    void zh_saveVisibility();
    void zh_openSettingsGroup(QSettings& settings);
    void zh_closeSettingsGroup(QSettings&  settings);

};
Q_DECLARE_METATYPE(PropertyList)
//======================================================
#endif // ZENERGYLINESETITEM_H
