//===========================================================
#ifndef ZURALADCDEVICECONNECTOR_H
#define ZURALADCDEVICECONNECTOR_H
//===========================================================
/* The class provides communication with ADC cards via "ComDV.dll" library.
 *
 *
*/
//===========================================================
#include <QObject>
#include <QLibrary>
#include <QMap>
#include <QMessageBox>
#include <QStringList>
//===========================================================
class ZControlAction;
//===========================================================
class ZUralAdcDeviceConnector : public QObject
{
    Q_OBJECT
public:

    enum SlotResult {
        SR_FUNCTION_UNRESOLVED=0,
        SR_TRUE=1,
        SR_FALSE=2,
        SR_ERROR=3
    };
    enum PID {
        PID_UNDEFINED=0x0000,
        PID_SU1=0x0001,
        PID_SU2=0x0009,
        PID_SU6=0x002D,

        PID_PU2=0x1002,
        PID_PU3=0x0011,
        PID_PU3F=0x0019,
        PID_PU4=0x001B,
        PID_PU5=0x002B,

        PID_PW3=0x8001
    };
    enum VID {
        VID_UNDEFINED=0x0000,
        VID_CYPRESS=0x0547
    };

    // STATIC FUNCS
    static QStringList zp_deviceNameList();
    static void zp_pidVidForDeviceName(const QString& deviceName, PID& pid, VID& vid);

    // FUNCS
    explicit ZUralAdcDeviceConnector(const QString& libraryFileName,
                                     bool& ok,
                                     QString& errorMsg,
                                     QObject *parent = 0);

    ~ZUralAdcDeviceConnector();

    QStringList zp_unresolvedFunctionList() const;


signals:

    void zg_message(QMessageBox::Icon, QString sender, QString msg) const;

public slots:

    void zp_connectToDevice(SlotResult& res, quint16 pid , quint16 vid = VID::VID_CYPRESS);
    void zp_isDeviceConnected(SlotResult& res) const;
    void zp_disconnectFromDevice(SlotResult& res);
    void zp_checkConnectionQuality(SlotResult& res, quint16 *qualityRes);
    void zp_isDeviceUINValid(SlotResult &res) const;

    // spectrometry
    void zp_isHardwareButtonDown(SlotResult &res) const;
    void zp_writeGainFactor(quint8 gainFactor, SlotResult &res);
    void zp_startExposition(SlotResult &res);
    void zp_stopExposition(SlotResult &res, quint32 x);
    void zp_readResultBuffer(quint32* pBuffer, // buffer
                             quint32* pLoad, // average load
                             quint32* pDeadTime, // dead time tics increament (1tic = 100392 mcs)
                             quint32* pCountEvent, // new event increment
                             quint32* pTime, // new event time increment
                             SlotResult &res) const;

    void zp_clearResultBuffer(quint32* pBuffer,
                              quint32 *bufferSize,
                              SlotResult &res) const;

    // extra port

private:

    // TYPEDEFS of library func pointers
    // service
    typedef Qt::HANDLE (*HOpenUSBDriver)(quint16, quint16); // (WORD ucPID, WORD ucVID)
    typedef bool (*CloseUSBDriver)(Qt::HANDLE);
    typedef bool (*ReadDeviceUIN)(Qt::HANDLE, quint16*); // (HANDLE hDevObj, WORD* pdata)
    typedef bool (*CheckQConnect)(Qt::HANDLE, quint16*); //(HANDLE hDevObj, WORD* pdata)
    // spectrometry
    typedef quint8 (*ButtonInquiry)(Qt::HANDLE); // (HANDLE hDevice)
    typedef bool (*WriteSerialRegister)(Qt::HANDLE, quint8); // (HANDLE hDevice, BYTE data)
    typedef bool (*StartExposition)(Qt::HANDLE); // (HANDLE hDevice)
    typedef bool (*StopIsoStream)(Qt::HANDLE, quint32); // (HANDLE hDevice, DWORD x)
    typedef bool (*ClearBufferUSB)(Qt::HANDLE, quint32*, quint32*); // (HANDLE hDevice, DWORD * appbuffer, DWORD * amountcodes)
    typedef quint8 (*ReadIsoBuffer)(Qt::HANDLE, quint32*, quint32*, // (HANDLE hDevice, DWORD * pAppbuffer, DWORD * pLoad,
                                    quint32*, quint32*, quint32*);                    // DWORD * pDeadTime, DWORD * pCountEvent, DWORD * pTime)

    // extra port funcs
    typedef bool (*ConfExtPort1)(Qt::HANDLE, qint16); // (HANDLE hDevice, WORD data)
    typedef bool (*WriteExtPort1)(Qt::HANDLE, quint8*, quint8); // (HANDLE hDevice, BYTE *pOutBuffer, BYTE ByteCount)
    typedef bool (*ReadExtPort1)(Qt::HANDLE, quint8*, quint8*); //(HANDLE hDevice, BYTE *pInBuffer, BYTE *pByteCount)
    typedef bool (*SetVoltageOut)(Qt::HANDLE, quint16); // (HANDLE hDevice, WORD data)
    typedef bool (*SetCurrentOut)(Qt::HANDLE, quint16); // (HANDLE hDevice, WORD data)
    typedef bool (*StartXRay)(Qt::HANDLE); // (HANDLE hDevice)
    typedef bool (*StopXRay)(Qt::HANDLE); // (HANDLE hDevice)
    typedef bool (*ReadStatus)(Qt::HANDLE, quint16*); // (HANDLE hDevice, WORD* pdata)
    typedef bool (*ReadVoltageOut)(Qt::HANDLE, quint16*); // (HANDLE hDevice, WORD* pdata)
    typedef bool (*ReadCurrentOut)(Qt::HANDLE, quint16*); // (HANDLE hDevice, WORD* pdata)
    typedef bool (*ReadVoltageIn)(Qt::HANDLE, quint16*); // (HANDLE hDevice, WORD* pdata)
    typedef bool (*ReadCurrentIn)(Qt::HANDLE, quint16*); // (HANDLE hDevice, WORD* pdata)
    typedef bool (*ReadOperationTimes)(Qt::HANDLE, quint32*); // (HANDLE hDevice, DWORD* pdata)


    // FUNC POINTERS
    // service
    HOpenUSBDriver zh_HOpenUSBDriver;
    CloseUSBDriver zh_CloseUSBDriver;
    ReadDeviceUIN zh_ReadDeviceUIN;
    CheckQConnect zh_CheckQConnect;
    // spectrometry
    ButtonInquiry zh_ButtonInquiry;
    WriteSerialRegister zh_WriteSerialRegister;
    StartExposition zh_StartExposition;
    StopIsoStream zh_StopIsoStream;
    ClearBufferUSB zh_ClearBufferUSB;
    ReadIsoBuffer zh_ReadIsoBuffer;
    // extra funcs
    ConfExtPort1 zh_ConfExtPort1;
    WriteExtPort1 zh_WriteExtPort1;
    ReadExtPort1 zh_ReadExtPort1;
    SetVoltageOut zh_SetVoltageOut;
    SetCurrentOut zh_SetCurrentOut;
    StartXRay zh_StartXRay;
    StopXRay zh_StopXRay;
    ReadStatus zh_ReadStatus;
    ReadVoltageOut zh_ReadVoltageOut;
    ReadCurrentOut zh_ReadCurrentOut;
    ReadVoltageIn zh_ReadVoltageIn;
    ReadCurrentIn zh_ReadCurrentIn;
    ReadOperationTimes zh_ReadOperationTimes;

    // STATIC
    static QMap<QString, QPair<PID, VID> > zv_devicePidVidMap;
    static QMap<QString, QPair<PID, VID> >zh_createDevicePidVidMap();

    // VARS
    QLibrary zv_library;
    Qt::HANDLE zv_handle;
    quint16 zv_deviceUIN;
    const quint16 zv_undefinedDeviceUIN = 0xFFFF;
    QStringList zv_unresolvedFunctionList;

    const quint16 zv_undefinedDevicePidVid = 0x0000;
    quint16 zv_devicePid;
    quint16 zv_deviceVid;

    //    QString zv_libraryFileName;// "ComDV.dll"

    // FUNCS
    void zh_createComponents();
    void zh_createActions();
    void zh_createConnections();
    void zh_restoreSettings();
    void zh_saveSettings();

    //    bool zh_checkDll(const QString& libraryFileAbsName, QString &errorMsg);
    //    bool zh_createLibraryFile(const QString& libraryFileAbsName, QString &errorMsg);
    bool zh_initializeLibrary(const QString& libraryFileName, QString &errorMsg);
    void zh_resetConnectionProperties();


};
//===========================================================
#endif // ZURALADCDEVICECONNECTOR_H
