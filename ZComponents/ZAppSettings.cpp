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
    return out;
}
//=========================================================
QDataStream& operator >> (QDataStream& in, ZAppSettings& appSettings)
{
    in >> appSettings.zv_dashboardSettings;
    return in;
}
//=========================================================
