//=========================================================
#include "ZDeviceSettings.h"
//=========================================================
//const QString ZDeviceSettings::zv_manualConnectionString = QObject::tr("Manual connection");
//const QString ZDeviceSettings::zv_applicationString = QObject::tr("Application only");
//const QString ZDeviceSettings::zv_deviceButtonString = QObject::tr("Device button only");
//const QString ZDeviceSettings::zv_applicationOrDeviceButtonString = QObject::tr("Application or device button");
//=========================================================
ZDeviceSettings::ZDeviceSettings()
{
    zv_deviceSerial = QString();
    zv_deviceSampleQuantity = 1;
    zv_deviceChannelQuantity = 1;
    zv_libraryFileName = QString();
    zv_defaultConnectionADC = QString();
    zv_expositionStarter = 0;
    zv_sampleShiftImpulseDuration = 0;
    zv_expositionDelayTime = 0;
}
//=========================================================
QDataStream& operator << (QDataStream& out, const ZDeviceSettings& settings)
{
    out << settings.zv_deviceSerial;
    out << settings.zv_deviceSampleQuantity;
    out << settings.zv_deviceChannelQuantity;
    out << settings.zv_libraryFileName;
    out << settings.zv_defaultConnectionADC;
    out << settings.zv_expositionStarter;
    out << settings.zv_sampleShiftImpulseDuration;
    out << settings.zv_expositionDelayTime;

    return out;
}
//=========================================================
QDataStream& operator >> (QDataStream& in, ZDeviceSettings& settings)
{
    in >>  settings.zv_deviceSerial;
    in >>  settings.zv_deviceSampleQuantity;
    in >>  settings.zv_deviceChannelQuantity;
    in >>  settings.zv_libraryFileName;
    in >>  settings.zv_defaultConnectionADC;
    in >>  settings.zv_expositionStarter;
    in >>  settings.zv_sampleShiftImpulseDuration;
    in >>  settings.zv_expositionDelayTime;

    return in;
}
//=========================================================

