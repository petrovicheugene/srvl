//=================================================
#ifndef ZMEASURINGCONTROLLER_H
#define ZMEASURINGCONTROLLER_H
//=================================================
#include <QDateTime>
#include <QObject>
#include <QMessageBox>
#include <QList>
#include <QPair>
//=================================================
class ZUralAdcDeviceConnector;
class ZSampleTask;
//=================================================
class ZMeasuringController : public QObject
{
    Q_OBJECT
public:

    enum ControllerState {CS_DISCONNECTED,
                          CS_SUSPENDED,
                          CS_RUNNING,
                         };

    explicit ZMeasuringController(QObject *parent = nullptr);
    void zp_setConnector(ZUralAdcDeviceConnector* connector);
    bool zp_setSampleShiftImpulseDuration(int duration);
    void zp_setSpectrumChannelCount(int channelCount);
    bool zp_measureSample(QList<QPair<quint8, int> > measuringConditions,
                          ZSampleTask* sampleTask);
    bool zp_stopMeasuring();
    void zp_finishCurrentMeasuring();

    int zp_currentSampleTaskId() const;
    QMap<int, QPair<quint8,int> >zp_currentSampleTaskMeasuringConditions() const;

signals:

    void zg_connectionState(QString msg, QMessageBox::Icon) const;
    void zg_message(QString msg, QMessageBox::Icon) const;
    void zg_expositionPassed(qint64 ms);
    void zg_currentMeasuringParameters(quint32 expoPassedMs,
                                       quint32 deadTimeMs,
                                       quint32 spectrumIntensityIntegral);

    void zg_currentSpectrumId(qint64 currentMeasuredSpectrumId) const;
    void zg_currentEnergyCalibration(QList<double> energyCalibrationFactorList) const;

public slots:


protected:

    virtual void timerEvent(QTimerEvent *event);

private slots:

private:

    typedef quint32 CHANNEL;
    // VARS
    ZUralAdcDeviceConnector* zv_connector;
    int zv_deviceButtonInquiryTimer;
    int zv_measuringTimer;
    quint32 zv_currentExpositionMS;
    const int zv_measuringInquiryPeriod = 200;
    const int zv_inquiryPeriod = 1000;
    int zv_sampleShiftImpulseDuration;

    qint64 zv_currentExpositionStartUnixTime;
    qint64 zv_passedExpositionsSummMS;
    ControllerState zv_controllerState;
    int zv_bufferChannelSize;
    CHANNEL* zv_buffer;
    quint32 zv_Load;
    quint32 zv_DeadTime;
    quint32 zv_CountEvent;
    quint32 zv_Time;
    quint32 zv_currentTime;
    quint32 zv_ticDuration;
    quint32 zv_currentExpositionSecondPassed;

    QList<QPair<quint8, int> > zv_currentMeasuringConditions;
    ZSampleTask* zv_currentSampleTask;
    int zv_currentMeasuringIndex;
    int zv_startUnixTime;

    // FUNCS
    void zh_inquiryDeviceButton();
    void zh_processMeasurementResults();
    bool zh_startSingleMeasuring();
    void zh_createDataBuffer();

};
//=================================================
#endif // ZMEASURINGCONTROLLER_H
