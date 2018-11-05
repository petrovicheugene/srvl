//=========================================================
#ifndef ZAPPSETTINGS_H
#define ZAPPSETTINGS_H
//=========================================================
#include "ZDashboardSettings.h"
#include "ZDeviceSettings.h"
#include <QDataStream>
//=========================================================
class ZAppSettings
{
    friend QDataStream& operator << (QDataStream& out, const ZAppSettings& appSettings);
    friend QDataStream& operator >> (QDataStream& in, ZAppSettings& appSettings);

public:

    ZAppSettings();
    ZDashboardSettings zv_dashboardSettings;
    ZDeviceSettings zv_deviceSettings;

    // Common appsettings
    bool zv_showMeasuringParameters;

};
//=========================================================
Q_DECLARE_METATYPE(ZAppSettings)
QDataStream& operator << (QDataStream& out, const ZAppSettings& appSettings);
QDataStream& operator >> (QDataStream& in, ZAppSettings& appSettings);
//=========================================================
#endif // ZAPPSETTINGS_H
