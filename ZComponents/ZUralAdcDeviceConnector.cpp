//===========================================================
#include "ZUralAdcDeviceConnector.h"

#include <windows.h>
#include <QApplication>
#include <QDir>
#include <QFile>
#include <QFileInfo>
#include <QMetaObject>
#include <QSettings>
#include <QStringList>
//===========================================================
// STATIC
//===========================================================
QMap<QString, QPair<ZUralAdcDeviceConnector::PID, ZUralAdcDeviceConnector::VID>>
    ZUralAdcDeviceConnector::zv_devicePidVidMap = zh_createDevicePidVidMap();
//===========================================================
QMap<QString, QPair<ZUralAdcDeviceConnector::PID, ZUralAdcDeviceConnector::VID>>
ZUralAdcDeviceConnector::zh_createDevicePidVidMap()
{
    QMap<QString,
         QPair<ZUralAdcDeviceConnector::PID, ZUralAdcDeviceConnector::VID>>
        devicePidVidMap;
    // SU
    devicePidVidMap.insert(QString("SU-1"),
                           QPair<ZUralAdcDeviceConnector::PID,
                                 ZUralAdcDeviceConnector::VID>(
                               ZUralAdcDeviceConnector::PID_SU1,
                               ZUralAdcDeviceConnector::VID_CYPRESS));
    devicePidVidMap.insert(QString("SU-2"),
                           QPair<ZUralAdcDeviceConnector::PID,
                                 ZUralAdcDeviceConnector::VID>(
                               ZUralAdcDeviceConnector::PID_SU2,
                               ZUralAdcDeviceConnector::VID_CYPRESS));
    devicePidVidMap.insert(QString("SU-6"),
                           QPair<ZUralAdcDeviceConnector::PID,
                                 ZUralAdcDeviceConnector::VID>(
                               ZUralAdcDeviceConnector::PID_SU6,
                               ZUralAdcDeviceConnector::VID_CYPRESS));
    // PU
    devicePidVidMap.insert(QString("PU-2"),
                           QPair<ZUralAdcDeviceConnector::PID,
                                 ZUralAdcDeviceConnector::VID>(
                               ZUralAdcDeviceConnector::PID_PU2,
                               ZUralAdcDeviceConnector::VID_CYPRESS));
    devicePidVidMap.insert(QString("PU-3"),
                           QPair<ZUralAdcDeviceConnector::PID,
                                 ZUralAdcDeviceConnector::VID>(
                               ZUralAdcDeviceConnector::PID_PU3,
                               ZUralAdcDeviceConnector::VID_CYPRESS));
    devicePidVidMap.insert(QString("PU-3F"),
                           QPair<ZUralAdcDeviceConnector::PID,
                                 ZUralAdcDeviceConnector::VID>(
                               ZUralAdcDeviceConnector::PID_PU3F,
                               ZUralAdcDeviceConnector::VID_CYPRESS));
    devicePidVidMap.insert(QString("PU-4"),
                           QPair<ZUralAdcDeviceConnector::PID,
                                 ZUralAdcDeviceConnector::VID>(
                               ZUralAdcDeviceConnector::PID_PU4,
                               ZUralAdcDeviceConnector::VID_CYPRESS));
    devicePidVidMap.insert(QString("PU-5"),
                           QPair<ZUralAdcDeviceConnector::PID,
                                 ZUralAdcDeviceConnector::VID>(
                               ZUralAdcDeviceConnector::PID_PU5,
                               ZUralAdcDeviceConnector::VID_CYPRESS));
    devicePidVidMap.insert(QString("PW-3"),
                           QPair<ZUralAdcDeviceConnector::PID,
                                 ZUralAdcDeviceConnector::VID>(
                               ZUralAdcDeviceConnector::PID_PW3,
                               ZUralAdcDeviceConnector::VID_CYPRESS));

    return devicePidVidMap;
}
//===========================================================
QStringList ZUralAdcDeviceConnector::zp_deviceNameList()
{
    return zv_devicePidVidMap.keys();
}
//===========================================================
void ZUralAdcDeviceConnector::zp_pidVidForDeviceName(
    const QString& deviceName,
    ZUralAdcDeviceConnector::PID& pid,
    ZUralAdcDeviceConnector::VID& vid)
{
    if (!zv_devicePidVidMap.keys().contains(deviceName))
    {
        pid = ZUralAdcDeviceConnector::PID_UNDEFINED;
        vid = ZUralAdcDeviceConnector::VID_UNDEFINED;
        return;
    }

    pid = zv_devicePidVidMap.value(deviceName).first;
    vid = zv_devicePidVidMap.value(deviceName).second;
}
//===========================================================
// END STATIC
//===========================================================
ZUralAdcDeviceConnector::ZUralAdcDeviceConnector(const QString& libraryFileName,
                                                 bool& ok,
                                                 QString& errorMsg,
                                                 QObject* parent)
    : QObject(parent)
{
    zv_handle = nullptr;
    zh_resetConnectionProperties();
    // library func pointers
    // service
    zh_HOpenUSBDriver = nullptr;
    zh_CloseUSBDriver = nullptr;
    zh_ReadDeviceUIN = nullptr;
    zh_CheckQConnect = nullptr;
    // spectrometry
    zh_ButtonInquiry = nullptr;
    zh_WriteSerialRegister = nullptr;
    zh_StartExposition = nullptr;
    zh_StopIsoStream = nullptr;
    zh_ClearBufferUSB = nullptr;
    zh_ReadIsoBuffer = nullptr;
    // extra funcs
    zh_ConfExtPort1 = nullptr;
    zh_WriteExtPort1 = nullptr;
    zh_ReadExtPort1 = nullptr;
    zh_SetVoltageOut = nullptr;
    zh_SetCurrentOut = nullptr;
    zh_StartXRay = nullptr;
    zh_StopXRay = nullptr;
    zh_ReadStatus = nullptr;
    zh_ReadVoltageOut = nullptr;
    zh_ReadCurrentOut = nullptr;
    zh_ReadVoltageIn = nullptr;
    zh_ReadCurrentIn = nullptr;
    zh_ReadOperationTimes = nullptr;

    // load library
    ok = zh_initializeLibrary(libraryFileName, errorMsg);

    zh_createComponents();
    zh_createActions();
    zh_createConnections();
    zh_restoreSettings();
}
//===========================================================
ZUralAdcDeviceConnector::~ZUralAdcDeviceConnector()
{
    zv_library.unload();
    zh_saveSettings();
}
//===========================================================
void ZUralAdcDeviceConnector::zh_createComponents() {}
//===========================================================
void ZUralAdcDeviceConnector::zh_createActions() {}
//===========================================================
void ZUralAdcDeviceConnector::zh_createConnections() {}
//===========================================================
void ZUralAdcDeviceConnector::zh_restoreSettings()
{
    QSettings settings;
    // open version group
    QString verString = qApp->applicationVersion();
    if (!verString.isEmpty())
    {
        settings.beginGroup(verString);
    }

    // close version group
    if (!verString.isEmpty())
    {
        settings.endGroup();
    }
}
//===========================================================
void ZUralAdcDeviceConnector::zh_saveSettings()
{
    QSettings settings;
    // open version group
    QString verString = qApp->applicationVersion();
    if (!verString.isEmpty())
    {
        settings.beginGroup(verString);
    }

    // close version group
    if (!verString.isEmpty())
    {
        settings.endGroup();
    }
}
//===========================================================
bool ZUralAdcDeviceConnector::zh_initializeLibrary(
    const QString& libraryFileName, QString& errorMsg)
{
    zv_library.setFileName(libraryFileName);
    zv_library.setLoadHints(QLibrary::PreventUnloadHint);

    try
    {
        zv_library.load();
    }
    catch (...)
    {
        errorMsg = tr("The application cannot load library from file \"%1\". "
                      "Error: \"%2\"")
                       .arg(zv_library.fileName(), zv_library.errorString());

        return false;
    }

    // library loaded
    // initialize func pointers
    //    zh_HOpenUSBDriver = (HOpenUSBDriver)zv_library.resolve("hOpenUSBDriver");
    //    if(!zh_HOpenUSBDriver) zv_unresolvedFunctionList.append("hOpenUSBDriver");

    //    zh_CloseUSBDriver = (CloseUSBDriver)zv_library.resolve("CloseUSBDriver");
    //    if(!zh_CloseUSBDriver) zv_unresolvedFunctionList.append("CloseUSBDriver");

    //    zh_ReadDeviceUIN = (ReadDeviceUIN)zv_library.resolve("ReadDeviceUIN");
    //    if(!zh_ReadDeviceUIN) zv_unresolvedFunctionList.append("ReadDeviceUIN");

    //    zh_CheckQConnect = (CheckQConnect)zv_library.resolve("CheckQConnect");
    //    if(!zh_CheckQConnect) zv_unresolvedFunctionList.append("CheckQConnect");

    //    // spectrometry
    //    zh_ButtonInquiry = (ButtonInquiry)zv_library.resolve("ButtonInquiry");
    //    if(!zh_ButtonInquiry) zv_unresolvedFunctionList.append("ButtonInquiry");

    //    zh_WriteSerialRegister = (WriteSerialRegister)zv_library.resolve("WriteSerialRegister");
    //    if(!zh_WriteSerialRegister) zv_unresolvedFunctionList.append("WriteSerialRegister");

    //    zh_StartExposition = (StartExposition)zv_library.resolve("StartExposition");
    //    if(!zh_StartExposition) zv_unresolvedFunctionList.append("StartExposition");

    //    zh_StopIsoStream = (StopIsoStream)zv_library.resolve("StopIsoStream");
    //    if(!zh_StopIsoStream) zv_unresolvedFunctionList.append("StopIsoStream");

    //    zh_ClearBufferUSB = (ClearBufferUSB)zv_library.resolve("ClearBufferUSB");
    //    if(!zh_ClearBufferUSB) zv_unresolvedFunctionList.append("ClearBufferUSB");

    //    zh_ReadIsoBuffer = (ReadIsoBuffer)zv_library.resolve("ReadIsoBuffer");
    //    if(!zh_ReadIsoBuffer) zv_unresolvedFunctionList.append("ReadIsoBuffer");

    //    // extra funcs
    //    zh_ConfExtPort1 = (ConfExtPort1)zv_library.resolve("ConfExtPort1");
    //    if(!zh_ConfExtPort1) zv_unresolvedFunctionList.append("ConfExtPort1");

    //    zh_WriteExtPort1 = (WriteExtPort1)zv_library.resolve("WriteExtPort1");
    //    if(!zh_WriteExtPort1) zv_unresolvedFunctionList.append("WriteExtPort1");

    //    zh_ReadExtPort1 = (ReadExtPort1)zv_library.resolve("ReadExtPort1");
    //    if(!zh_ReadExtPort1) zv_unresolvedFunctionList.append("ReadExtPort1");

    //    zh_SetVoltageOut = (SetVoltageOut)zv_library.resolve("SetVoltageOut");
    //    if(!zh_SetVoltageOut) zv_unresolvedFunctionList.append("SetVoltageOut");

    //    zh_SetCurrentOut = (SetCurrentOut)zv_library.resolve("SetCurrentOut");
    //    if(!zh_SetCurrentOut) zv_unresolvedFunctionList.append("SetCurrentOut");

    //    zh_StartXRay = (StartXRay)zv_library.resolve("StartXRay");
    //    if(!zh_StartXRay) zv_unresolvedFunctionList.append("StartXRay");

    //    zh_StopXRay = (StopXRay)zv_library.resolve("StopXRay");
    //    if(!zh_StopXRay) zv_unresolvedFunctionList.append("StopXRay");

    //    zh_ReadStatus = (ReadStatus)zv_library.resolve("ReadStatus");
    //    if(!zh_ReadStatus) zv_unresolvedFunctionList.append("ReadStatus");

    //    zh_ReadVoltageOut = (ReadVoltageOut)zv_library.resolve("ReadVoltageOut");
    //    if(!zh_ReadVoltageOut) zv_unresolvedFunctionList.append("ReadVoltageOut");

    //    zh_ReadCurrentOut = (ReadCurrentOut)zv_library.resolve("ReadCurrentOut");
    //    if(!zh_ReadCurrentOut) zv_unresolvedFunctionList.append("ReadCurrentOut");

    //    zh_ReadVoltageIn = (ReadVoltageIn)zv_library.resolve("ReadVoltageIn");
    //    if(!zh_ReadVoltageIn) zv_unresolvedFunctionList.append("ReadVoltageIn");

    //    zh_ReadCurrentIn = (ReadCurrentIn)zv_library.resolve("ReadCurrentIn");
    //    if(!zh_ReadCurrentIn) zv_unresolvedFunctionList.append("ReadCurrentIn");

    //    zh_ReadOperationTimes = (ReadOperationTimes)zv_library.resolve("ReadOperationTimes");
    //    if(!zh_ReadOperationTimes) zv_unresolvedFunctionList.append("ReadOperationTimes");

    return true;
}

//===========================================================
QStringList ZUralAdcDeviceConnector::zp_unresolvedFunctionList() const
{
    return zv_unresolvedFunctionList;
}
//===========================================================
void ZUralAdcDeviceConnector::zh_resetConnectionProperties()
{
    zv_deviceUIN = zv_undefinedDeviceUIN;
    zv_devicePid = zv_undefinedDevicePidVid;
    zv_deviceVid = zv_undefinedDevicePidVid;
}
//===========================================================
void ZUralAdcDeviceConnector::zp_connectToDevice(SlotResult& res,
                                                 quint16 pid,
                                                 quint16 vid)
{
    //    int pointer = reinterpret_cast<int>((void*)zh_HOpenUSBDriver);
    //    QString m = QString::number(pointer);

    //    if(!zh_HOpenUSBDriver) zv_unresolvedFunctionList.append("hOpenUSBDriver");
    //    res = SR_TRUE;
    //    return;
    //    HOpenUSBDriver h_HOpenUSBDriver = (HOpenUSBDriver)QLibrary::resolve(zv_library.fileName(), "hOpenUSBDriver");
    //    if(!h_HOpenUSBDriver)
    //    {
    //        res = SR_FUNCTION_UNRESOLVED;
    //        return;

    //    }

    //    zv_handle = zh_HOpenUSBDriver(pid, vid);
    //    return;

    //if(!zh_HOpenUSBDriver) zv_unresolvedFunctionList.append("hOpenUSBDriver");

    //    QLibrary library(zv_library.fileName());
    //    library.setLoadHints(QLibrary::PreventUnloadHint);

    //    if(!library.load())
    //    {
    //        res = SR_FALSE;
    //        return;
    //    }

    //    HOpenUSBDriver h_HOpenUSBDriver = (HOpenUSBDriver)QLibrary::resolve(zv_library.fileName(), "hOpenUSBDriver");
    zh_HOpenUSBDriver = nullptr;
    zh_HOpenUSBDriver = (HOpenUSBDriver) zv_library.resolve("hOpenUSBDriver");
    if (!zh_HOpenUSBDriver)
    {
        zv_unresolvedFunctionList.append("hOpenUSBDriver");
    }

    if (zh_HOpenUSBDriver == nullptr)
    {
        res = SR_FUNCTION_UNRESOLVED;
        return;
    }

    if (zv_handle != nullptr)
    {
        if (zv_devicePid == pid && zv_deviceVid == vid)
        {
            // connection is already exists
            res = SR_TRUE;
        }
        else
        {
            // connection exists but pid-vid are not appropriate
            res = SR_FALSE;
        }

        return;
    }

    zv_handle = zh_HOpenUSBDriver(pid, vid);

    if (!zv_handle)
    {
        // connection has not been established
        res = SR_FALSE;
        zh_resetConnectionProperties();
        return;
    }

    zh_ReadDeviceUIN = nullptr;
    zh_ReadDeviceUIN = (ReadDeviceUIN) zv_library.resolve("ReadDeviceUIN");
    if (!zh_ReadDeviceUIN)
        zv_unresolvedFunctionList.append("ReadDeviceUIN");

    zv_devicePid = pid;
    zv_deviceVid = vid;

    if (!zh_ReadDeviceUIN(zv_handle, &zv_deviceUIN))
    {
        zv_deviceUIN = zv_undefinedDeviceUIN;
    }

    res = SR_TRUE;
}
//===========================================================
void ZUralAdcDeviceConnector::zp_isDeviceConnected(SlotResult& res)
{
    if (zv_handle == nullptr)
    {
        res = SR_FALSE;
    }
    else
    {
        res = SR_TRUE;
    }
}
//===========================================================
void ZUralAdcDeviceConnector::zp_disconnectFromDevice(SlotResult& res)
{
    zh_CloseUSBDriver = nullptr;
    zh_CloseUSBDriver = (CloseUSBDriver) zv_library.resolve("CloseUSBDriver");
    if (!zh_CloseUSBDriver)
        zv_unresolvedFunctionList.append("CloseUSBDriver");

    if (!zh_CloseUSBDriver)
    {
        res = SR_FUNCTION_UNRESOLVED;
        return;
    }

    if (!zv_handle)
    {
        res = SR_TRUE;
        return;
    }

    if (!zh_CloseUSBDriver(zv_handle))
    {
        res = SR_FALSE;
        return;
    }

    zv_handle = nullptr;

    res = SR_TRUE;
    zh_resetConnectionProperties();
}
//===========================================================
void ZUralAdcDeviceConnector::zp_checkConnectionQuality(SlotResult& res,
                                                        quint16* qualityRes)
{
    zh_CheckQConnect = nullptr;
    zh_CheckQConnect = (CheckQConnect) zv_library.resolve("CheckQConnect");
    if (!zh_CheckQConnect)
        zv_unresolvedFunctionList.append("CheckQConnect");

    if (!zh_CheckQConnect)
    {
        res = SR_FUNCTION_UNRESOLVED;
        return;
    }

    if (zv_handle == nullptr)
    {
        res = SR_FALSE;
        return;
    }

    if (zh_CheckQConnect(zv_handle, qualityRes))
    {
        res = SR_TRUE;
    }
    else
    {
        res = SR_FALSE;
    }
}
//===========================================================
void ZUralAdcDeviceConnector::zp_isDeviceUINValid(SlotResult& res)
{
    if (zv_deviceUIN == zv_undefinedDeviceUIN)
    {
        res = SR_FALSE;
    }
    else
    {
        res = SR_TRUE;
    }
}
//===========================================================
void ZUralAdcDeviceConnector::zp_isHardwareButtonDown(SlotResult& res)
{
    zh_ButtonInquiry = nullptr;
    zh_ButtonInquiry = (ButtonInquiry) zv_library.resolve("ButtonInquiry");
    if (!zh_ButtonInquiry)
        zv_unresolvedFunctionList.append("ButtonInquiry");

    //    res = SR_TRUE;
    //    return;

    if (!zh_ButtonInquiry)
    {
        res = SR_FUNCTION_UNRESOLVED;
        return;
    }

    if (zv_handle == nullptr)
    {
        res = SR_ERROR;
        return;
    }

    quint8 funcRes = zh_ButtonInquiry(zv_handle);
    if (funcRes == 0)
    {
        res = SR_ERROR;
    }
    else if (funcRes == 1)
    {
        res = SR_TRUE;
    }
    else if (funcRes == 2)
    {
        res = SR_FALSE;
    }
    else
    {
        res = SR_FALSE;
    }
}
//===========================================================
void ZUralAdcDeviceConnector::zp_writeGainFactor(quint8 gainFactor,
                                                 SlotResult& res)
{
    zh_WriteSerialRegister = nullptr;
    zh_WriteSerialRegister = (WriteSerialRegister) zv_library.resolve(
        "WriteSerialRegister");
    if (!zh_WriteSerialRegister)
        zv_unresolvedFunctionList.append("WriteSerialRegister");

    if (!zh_WriteSerialRegister)
    {
        res = SR_FUNCTION_UNRESOLVED;
        return;
    }

    if (zv_handle == nullptr)
    {
        res = SR_ERROR;
        return;
    }

    if (zh_WriteSerialRegister(zv_handle, gainFactor))
    {
        res = SR_TRUE;
    }
    else
    {
        res = SR_FALSE;
    }
}
//===========================================================
void ZUralAdcDeviceConnector::zp_startExposition(SlotResult& res)
{
    zh_StartExposition = nullptr;
    zh_StartExposition = (StartExposition) zv_library.resolve(
        "StartExposition");
    if (!zh_StartExposition)
        zv_unresolvedFunctionList.append("StartExposition");

    if (!zh_StartExposition)
    {
        res = SR_FUNCTION_UNRESOLVED;
        return;
    }

    if (zv_handle == nullptr)
    {
        res = SR_ERROR;
        return;
    }

    if (zh_StartExposition(zv_handle))
    {
        res = SR_TRUE;
    }
    else
    {
        res = SR_FALSE;
    }
}
//===========================================================
void ZUralAdcDeviceConnector::zp_stopExposition(SlotResult& res, quint32 x)
{
    zh_StopIsoStream = nullptr;
    zh_StopIsoStream = (StopIsoStream) zv_library.resolve("StopIsoStream");
    if (!zh_StopIsoStream)
        zv_unresolvedFunctionList.append("StopIsoStream");

    if (!zh_StopIsoStream)
    {
        res = SR_FUNCTION_UNRESOLVED;
        return;
    }

    if (zv_handle == nullptr)
    {
        res = SR_ERROR;
        return;
    }

    if (zh_StopIsoStream(zv_handle, x))
    {
        res = SR_TRUE;
    }
    else
    {
        res = SR_FALSE;
    }
}
//===========================================================
void ZUralAdcDeviceConnector::zp_readResultBuffer(
    quint32* pBuffer,     // buffer
    quint32* pLoad,       // average load
    quint32* pDeadTime,   // dead time tics increament (1tic = 100392 mcs)
    quint32* pCountEvent, // new event increment
    quint32* pTime,       // new event time increment
    SlotResult& res)
{
    zh_ReadIsoBuffer = nullptr;
    zh_ReadIsoBuffer = (ReadIsoBuffer) zv_library.resolve("ReadIsoBuffer");
    if (!zh_ReadIsoBuffer)
        zv_unresolvedFunctionList.append("ReadIsoBuffer");

    if (!zh_ReadIsoBuffer)
    {
        res = SR_FUNCTION_UNRESOLVED;
        return;
    }

    if (zv_handle == nullptr)
    {
        res = SR_ERROR;
        return;
    }

    quint8 funcRes = zh_ReadIsoBuffer(zv_handle,
                                      pBuffer,
                                      pLoad,
                                      pDeadTime,
                                      pCountEvent,
                                      pTime);
    if (funcRes != 0xFF)
    {
        res = SR_TRUE;
    }
    else
    {
        res = SR_FALSE;
    }
}
//===========================================================
void ZUralAdcDeviceConnector::zp_clearResultBuffer(quint32* pBuffer,
                                                   quint32* bufferSize,
                                                   SlotResult& res)
{
    zh_ClearBufferUSB = nullptr;
    zh_ClearBufferUSB = (ClearBufferUSB) zv_library.resolve("ClearBufferUSB");
    if (!zh_ClearBufferUSB)
        zv_unresolvedFunctionList.append("ClearBufferUSB");

    if (!zh_ClearBufferUSB)
    {
        res = SR_FUNCTION_UNRESOLVED;
        return;
    }

    if (zv_handle == nullptr)
    {
        res = SR_ERROR;
        return;
    }

    if (zh_ClearBufferUSB(zv_handle, pBuffer, bufferSize))
    {
        res = SR_TRUE;
    }
    else
    {
        res = SR_FALSE;
    }
}
//===========================================================
