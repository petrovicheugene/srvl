//=========================================================
#include "ZAppSettings.h"
//=========================================================
ZAppSettings::ZAppSettings()
{

}
//=========================================================
QDataStream& operator << (QDataStream& out, const ZAppSettings& appSettings)
{
    out << appSettings.zv_dashboardSettings;
    out << appSettings.zv_deviceSettings;
    out << appSettings.zv_showMeasuringParameters;

    return out;
}
//=========================================================
QDataStream& operator >> (QDataStream& in, ZAppSettings& appSettings)
{
    in >> appSettings.zv_dashboardSettings;
    in >> appSettings.zv_deviceSettings;
    in >> appSettings.zv_showMeasuringParameters;

    return in;
}
//=========================================================
