//===================================================
#ifndef ZDASHBOARDSETTINGS_H
#define ZDASHBOARDSETTINGS_H
//===================================================
#include <QDataStream>
#include <QColor>
#include <QMap>
//===================================================
class QComboBox;
//===================================================
class ZDashboardSettings
{
    friend QDataStream& operator << (QDataStream& out, const ZDashboardSettings& appSettings);
    friend QDataStream& operator >> (QDataStream& in, ZDashboardSettings& appSettings);

public:

    ZDashboardSettings();
    // VARS
    enum DashboardStyle {DBS_CLASSIC, DBS_CLASSIC_SLIM, DBS_DONUT, DBS_DONUT_SLIM};
    enum DashboardLocation {DBL_TOP, DBL_BOTTOM, DBL_LEFT, DBL_RIGHT};

    DashboardStyle zv_dashboardStyle;
    int zv_progressBarSize;
    DashboardLocation zv_dashboardLocation;
    QColor zv_progressBarColor;
    QColor zv_textColor;

    // STATIC
    // VARS
    static const QMap<DashboardStyle, QString> zv_dashboardStyleNameMap;
    static const QMap<DashboardLocation, QString> zv_dashboardLocationNameMap;
    static const int zv_progressBarMinimumSize;
    static const int zv_progressBarMaximumSize;

    // FUNCS
    static void zp_initDashboardStyleComboBox(QComboBox& comboBox);
    static void zp_initDashboardLocationComboBox(QComboBox& comboBox);

private:

    // STATIC
    // FUNCS
    static QMap<DashboardStyle, QString> zh_initDashboardStyleNameMap();
    static QMap<DashboardLocation, QString> zh_initDashboardLocationNameMap();

};
//===================================================
Q_DECLARE_METATYPE(ZDashboardSettings)
Q_DECLARE_METATYPE(ZDashboardSettings::DashboardStyle)
Q_DECLARE_METATYPE(ZDashboardSettings::DashboardLocation)

QDataStream& operator << (QDataStream& out, const ZDashboardSettings& appSettings);
QDataStream& operator >> (QDataStream& in, ZDashboardSettings& appSettings);
//===================================================
#endif // ZDASHBOARDSETTINGS_H
