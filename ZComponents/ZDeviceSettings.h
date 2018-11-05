//=========================================================
#ifndef ZDEVICESETTINGS_H
#define ZDEVICESETTINGS_H
//=========================================================
#include <QDataStream>
#include <QString>
//=========================================================
class ZDeviceSettings
{
    friend QDataStream& operator << (QDataStream& out, const ZDeviceSettings& appSettings);
    friend QDataStream& operator >> (QDataStream& in, ZDeviceSettings& appSettings);

public:
    ZDeviceSettings();

    QString zv_deviceSerial;
    int zv_deviceSampleQuantity;
    int zv_deviceChannelQuantity;
    QString zv_libraryFileName;
    QString zv_defaultConnectionADC;
    int zv_expositionStarter; // 0-app; 1-device button; 2-app or bevice button
    int zv_sampleShiftImpulseDuration;
    int zv_expositionDelayTime;



//    // STATIC
//    static const QString zv_manualConnectionString;
//    static const QString zv_applicationString;
//    static const QString zv_deviceButtonString;
//    static const QString zv_applicationOrDeviceButtonString;

};
//=========================================================
Q_DECLARE_METATYPE(ZDeviceSettings)

QDataStream& operator << (QDataStream& out, const ZDeviceSettings& settings);
QDataStream& operator >> (QDataStream& in, ZDeviceSettings& settings);

//=========================================================
#endif // ZDEVICESETTINGS_H
