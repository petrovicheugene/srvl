//===================================================
#include "ZDashboardSettings.h"
#include <ZGLConstantsAndDefines.h>
#include <QObject>
#include <QComboBox>
//===================================================
// STATIC

const QMap<ZDashboardSettings::DashboardStyle, QString> ZDashboardSettings::zv_dashboardStyleNameMap
= zh_initDashboardStyleNameMap();
const QMap<ZDashboardSettings::DashboardLocation, QString> ZDashboardSettings::zv_dashboardLocationNameMap
= zh_initDashboardLocationNameMap();

const int ZDashboardSettings::zv_progressBarMinimumSize = 0;
const int ZDashboardSettings::zv_progressBarMaximumSize = 100;
//===================================================
void ZDashboardSettings::zp_initDashboardStyleComboBox(QComboBox& comboBox)
{
    comboBox.clear();
    QMap<ZDashboardSettings::DashboardStyle, QString>::const_iterator it;
    for(it = zv_dashboardStyleNameMap.begin(); it != zv_dashboardStyleNameMap.end(); it++)
    {
        comboBox.addItem(it.value(), QVariant(it.key()));
    }
}
//===================================================
void ZDashboardSettings::zp_initDashboardLocationComboBox(QComboBox& comboBox)
{
    comboBox.clear();
    QMap<ZDashboardSettings::DashboardLocation, QString>::const_iterator it;
    for(it = zv_dashboardLocationNameMap.begin(); it != zv_dashboardLocationNameMap.end(); it++)
    {
        comboBox.addItem(it.value(), QVariant(it.key()));
    }
}
//===================================================
QMap<ZDashboardSettings::DashboardStyle, QString> ZDashboardSettings::zh_initDashboardStyleNameMap()
{
    QMap<ZDashboardSettings::DashboardStyle, QString> map;
    map.insert(DBS_CLASSIC, QObject::tr("Classic"));
    map.insert(DBS_CLASSIC_SLIM, QObject::tr("Classic slim"));
    map.insert(DBS_DONUT, QObject::tr("Donut"));
    map.insert(DBS_DONUT_SLIM, QObject::tr("Donut slim"));
    // map.insert(DBS_ROUND_LINE, QObject::tr("Round line"));
    return map;
}
//===================================================
QMap<ZDashboardSettings::DashboardLocation, QString> ZDashboardSettings::zh_initDashboardLocationNameMap()
{
    QMap<ZDashboardSettings::DashboardLocation, QString> map;
    map.insert(DBL_BOTTOM, QObject::tr("Bottom"));
    map.insert(DBL_TOP, QObject::tr("Top"));
    map.insert(DBL_LEFT, QObject::tr("Left"));
    map.insert(DBL_RIGHT, QObject::tr("Right"));

    return map;
}

//===================================================
// END STATIC
//===================================================
ZDashboardSettings::ZDashboardSettings()
{
    zv_dashboardStyle = DBS_CLASSIC;
    zv_progressBarSize = 50;
    zv_dashboardLocation = DBL_BOTTOM;
    zv_progressBarColor = QColor(Qt::green);
    zv_textColor = QColor(Qt::darkGray);
}
//===================================================
QDataStream& operator << (QDataStream& out, const ZDashboardSettings& appSettings)
{
    // out << valeur.qsName << valeur.qsDefinition;
    out << static_cast<int>(appSettings.zv_dashboardStyle);
    out << appSettings.zv_progressBarSize;
    out << static_cast<int>(appSettings.zv_dashboardLocation);
    out << appSettings.zv_progressBarColor;
    out << appSettings.zv_textColor;

    return out;
}
//===================================================
QDataStream& operator >> (QDataStream& in, ZDashboardSettings& appSettings)
{
    // in >> valeur.qsName >> valeur.qsDefinition;
    int dashBoardStyle;
    int dashBoardLocation;
    in >> dashBoardStyle;
    in >> appSettings.zv_progressBarSize;
    in >> dashBoardLocation;
    in >> appSettings.zv_progressBarColor;
    in >> appSettings.zv_textColor;

    appSettings.zv_dashboardStyle = static_cast<ZDashboardSettings::DashboardStyle>(dashBoardStyle);
    appSettings.zv_dashboardLocation = static_cast<ZDashboardSettings::DashboardLocation>(dashBoardLocation);

    return in;
}
//===================================================
