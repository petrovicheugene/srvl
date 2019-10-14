//=================================================
#include "ZMeasuringController.h"
#include "ZGeneral.h"
#include "ZUralAdcDeviceConnector.h"
#include "ZSampleTask.h"

#include <QDebug>
#include <QTimerEvent>

//=================================================
ZMeasuringController::ZMeasuringController(QObject *parent) : QObject(parent)
{
    zv_connector = nullptr;
    zv_deviceButtonInquiryTimer = 0;
    zv_measuringTimer = 0;
    zv_startUnixTime = 0;
    zv_controllerState = CS_DISCONNECTED;
    zv_currentSampleTask = nullptr;
    zv_bufferChannelSize = 16384 / sizeof(CHANNEL); // doc for ComDV.dll
    zv_currentMeasuringIndex = -1;

    zv_Load = 0;
    zv_DeadTime = 0;
    zv_CountEvent = 0;
    zv_Time = 0;
    zv_ticDuration = 100392;

    zv_buffer = nullptr;
    zh_createDataBuffer();
}
//=================================================
void ZMeasuringController::timerEvent(QTimerEvent *event)
{
    if(event->timerId() == zv_deviceButtonInquiryTimer)
    {
        zh_inquiryDeviceButton();
    }
    else if(event->timerId() == zv_measuringTimer)
    {
        zh_processMeasurementResults();
    }
}
//=================================================
void ZMeasuringController::zp_setConnector(ZUralAdcDeviceConnector* connector)
{
    zv_connector = connector;

    if(zv_deviceButtonInquiryTimer)
    {
        killTimer(zv_deviceButtonInquiryTimer);
        zv_deviceButtonInquiryTimer = 0;
    }

    if(!zv_connector)
    {
        return;
    }

    // start connection checking
    zv_deviceButtonInquiryTimer = startTimer(zv_inquiryPeriod);
}
//=================================================
bool ZMeasuringController::zp_setSampleShiftImpulseDuration(int duration)
{
    if(duration < 0)
    {
        return false;
    }

    zv_sampleShiftImpulseDuration = duration;
    return true;
}
//=================================================
void ZMeasuringController::zp_setSpectrumChannelCount(int channelCount)
{
    //    if(channelCount > 0 && zv_bufferByteSize / 2 != channelCount && zv_controllerState != CS_RUNNING)
    //    {
    //        zv_bufferByteSize = channelCount * 2;
    //        zh_createDataBuffer();
    //    }
}
//=================================================
void ZMeasuringController::zh_createDataBuffer()
{
    if(zv_buffer)
    {
        delete[] zv_buffer;
    }

    zv_buffer = new CHANNEL[zv_bufferChannelSize];

}
//=================================================
bool ZMeasuringController::zp_measureSample(QList<QPair<quint8, int> > measuringConditions,
                                            ZSampleTask* currentSampleTask)
{
    if(zv_connector == nullptr)
    {
        if(zv_deviceButtonInquiryTimer)
        {
            killTimer(zv_deviceButtonInquiryTimer);
            zv_deviceButtonInquiryTimer = 0;
        }
        zv_controllerState = CS_DISCONNECTED;

        // end of measuring
        if(zv_measuringTimer)
        {
            killTimer(zv_measuringTimer);
            zv_measuringTimer = 0;
        }
        zv_deviceButtonInquiryTimer = startTimer(zv_inquiryPeriod);

        return false;
    }

    QString msg;
    if(!zv_connector)
    {
        msg = tr("Device connector is absent.\n"
                 "Check device library and connection to device");
        emit zg_message(msg, QMessageBox::Critical);
        return false;
    }
    // check measuring conditions and connector
    if(measuringConditions.isEmpty())
    {
        return false;
    }

    if(zv_controllerState == CS_RUNNING)
    {
        // measuring is in progress
        return false;
    }

    // check connection
    ZUralAdcDeviceConnector::SlotResult res;
    zv_connector->zp_isDeviceConnected(res);
    if(res == ZUralAdcDeviceConnector::SR_FUNCTION_UNRESOLVED)
    {
        msg = tr("Device connector is not initilized prperly.");
        emit zg_message(msg, QMessageBox::Critical);
    }
    else if(res == ZUralAdcDeviceConnector::SR_ERROR)
    {
        msg = tr("Unknown error while cheking connection to device.");
        emit zg_message(msg, QMessageBox::Critical);
    }
    else if(res == ZUralAdcDeviceConnector::SR_FALSE)
    {
        msg = tr("Connection to device is lost.");
        emit zg_message(msg, QMessageBox::Warning);
    }

    if(!msg.isEmpty())
    {
        return false;
    }

    // connection present
    // stop inquiry Device Button
    if(zv_deviceButtonInquiryTimer)
    {
        killTimer(zv_deviceButtonInquiryTimer);
        zv_deviceButtonInquiryTimer = 0;
    }

    // save measuringConditions
    zv_currentMeasuringConditions.clear();
    zv_currentMeasuringConditions = measuringConditions;
    zv_currentSampleTask = currentSampleTask;

    // reset current Measuring Index
    zv_currentMeasuringIndex = 0;
    zv_passedExpositionsSummMS = 0;
    return zh_startSingleMeasuring();
}
//=================================================
bool ZMeasuringController::zp_stopMeasuring()
{
    if(zv_connector == nullptr)
    {
        if(zv_deviceButtonInquiryTimer)
        {
            killTimer(zv_deviceButtonInquiryTimer);
            zv_deviceButtonInquiryTimer = 0;
        }
        zv_controllerState = CS_DISCONNECTED;

        // end of measuring
        if(zv_measuringTimer)
        {
            killTimer(zv_measuringTimer);
            zv_measuringTimer = 0;
        }

        zv_deviceButtonInquiryTimer = startTimer(zv_inquiryPeriod);
        return false;
    }

    if(zv_measuringTimer != 0)
    {
        killTimer(zv_measuringTimer);
        zv_measuringTimer = 0;
    }

    ZUralAdcDeviceConnector::SlotResult res;

    for(int i = 0; i < 10; i++)
    {
        zv_connector->zp_stopExposition(res, 0);
        if(res != ZUralAdcDeviceConnector::SR_TRUE)
        {
            continue;
        }
    }

    if(zv_deviceButtonInquiryTimer == 0)
    {
        zv_deviceButtonInquiryTimer = startTimer(zv_inquiryPeriod);
    }

    zv_passedExpositionsSummMS = 0;
    return true;
}
//=================================================
void ZMeasuringController::zp_finishCurrentMeasuring()
{
    if(zv_measuringTimer != 0)
    {
        killTimer(zv_measuringTimer);
        zv_measuringTimer = 0;
    }

    ZUralAdcDeviceConnector::SlotResult res;

    for(int i = 0; i < 10; i++)
    {
        zv_connector->zp_stopExposition(res, 0);
        if(res != ZUralAdcDeviceConnector::SR_TRUE)
        {
            continue;
        }
    }

    zv_currentMeasuringIndex = -1;
    zv_currentSampleTask->zp_measuringFinished();
}
//=================================================
int ZMeasuringController::zp_currentSampleTaskId() const
{
    if(!zv_currentSampleTask)
    {
        return -1;
    }

    return zv_currentSampleTask->zp_id();
}
//=================================================
QMap<int, QPair<quint8,int> > ZMeasuringController::zp_currentSampleTaskMeasuringConditions() const
{
    return zv_currentSampleTask->zp_measuringConditionsMap();
}
//=================================================
bool ZMeasuringController::zh_startSingleMeasuring()
{
    if(zv_connector == nullptr)
    {
        if(zv_deviceButtonInquiryTimer)
        {
            killTimer(zv_deviceButtonInquiryTimer);
            zv_deviceButtonInquiryTimer = 0;
        }
        zv_controllerState = CS_DISCONNECTED;

        // end of measuring
        if(zv_measuringTimer)
        {
            killTimer(zv_measuringTimer);
            zv_measuringTimer = 0;
        }
        zv_deviceButtonInquiryTimer = startTimer(zv_inquiryPeriod);

        return false;
    }

    // current measuring index sould be set
    // check measuring index
    if(zv_currentMeasuringIndex < 0
            || zv_currentMeasuringIndex >= zv_currentMeasuringConditions.count())
    {
        // end of measuring
        if(zv_measuringTimer)
        {
            killTimer(zv_measuringTimer);
        }
        zv_measuringTimer = 0;
        zv_deviceButtonInquiryTimer = startTimer(zv_inquiryPeriod);
        return false;
    }

    // set gain factor to device
    quint8 gainFactor = zv_currentMeasuringConditions.at(zv_currentMeasuringIndex).first;

    ZUralAdcDeviceConnector::SlotResult res = ZUralAdcDeviceConnector::SR_FALSE;
    // try to write up to 10 times
    for(int i = 0; i < 10; i++)
    {
        zv_connector->zp_writeGainFactor(gainFactor, res);

        if(res == ZUralAdcDeviceConnector::SR_TRUE
                || res == ZUralAdcDeviceConnector::SR_FUNCTION_UNRESOLVED)
        {
            break;
        }
    }

    QString errorMsg;
    if(res == ZUralAdcDeviceConnector::SR_FUNCTION_UNRESOLVED)
    {
        errorMsg = tr("The function for write gain factor has not been resolved.");
        // no function resolved
    }
    else if(res == ZUralAdcDeviceConnector::SR_ERROR)
    {
        errorMsg = tr("Unknown error by calling gain factor write function.");
        // unknown error
    }
    else if(res == ZUralAdcDeviceConnector::SR_FALSE)
    {
        errorMsg = tr("Error write gain factor to device.");
        // didn't write
    }

    if(!errorMsg.isEmpty())
    {
        // send message
        emit zg_message(errorMsg, QMessageBox::Critical);
        zv_deviceButtonInquiryTimer = startTimer(zv_inquiryPeriod);
        return false;
    }

    // gain factor write - ok
    // Launch expo
    zv_currentExpositionMS = static_cast<quint32>(zv_currentMeasuringConditions.at(zv_currentMeasuringIndex).second) * 1000;
    // start expo
    zv_connector->zp_startExposition(res);

    if(res == ZUralAdcDeviceConnector::SR_FUNCTION_UNRESOLVED)
    {
        errorMsg = tr("The function for start exposition has not been resolved.");
        // no function resolved
    }
    else if(res == ZUralAdcDeviceConnector::SR_ERROR)
    {
        errorMsg = tr("Unknown error by calling start exposition function.");
        // unknown error
    }
    else if(res == ZUralAdcDeviceConnector::SR_FALSE)
    {
        // didn't start
        errorMsg = tr("Error start exposition.");
    }

    if(!errorMsg.isEmpty())
    {
        // send message
        emit zg_message(errorMsg, QMessageBox::Critical);
        zv_deviceButtonInquiryTimer = startTimer(zv_inquiryPeriod);
        return false;
    }

    // expo started
    // create buffer


    // clear buffer and auxillary spe data
    // quint32 bufferSize = static_cast<quint32>(sizeof(quint32) * zv_bufferChannelSize );
    quint32 bufferSize = 0;

    for(int i = 0; i < 10; i++)
    {
        zv_connector->zp_clearResultBuffer(0, &bufferSize, res);
        if(res == ZUralAdcDeviceConnector::SR_TRUE
                || res == ZUralAdcDeviceConnector::SR_FUNCTION_UNRESOLVED)
        {
            break;
        }
    }

    for(int i = 0; i < zv_bufferChannelSize; i++)
    {
        *(zv_buffer + i) = 0x00000000;
    }

    zv_Load = 0;
    zv_DeadTime = 0;
    zv_CountEvent = 0;
    zv_Time = 0;

    if(res == ZUralAdcDeviceConnector::SR_FUNCTION_UNRESOLVED)
    {
        errorMsg = tr("The function for clear buffer has not been resolved.");
        // no function resolved
    }
    else if(res == ZUralAdcDeviceConnector::SR_ERROR)
    {
        errorMsg = tr("Unknown error by calling clear buffer function.");
        // unknown error
    }
    else if(res == ZUralAdcDeviceConnector::SR_FALSE)
    {
        // didn't start
        errorMsg = tr("Error clear buffer.");
    }

    if(!errorMsg.isEmpty())
    {
        // send message
        emit zg_message(errorMsg, QMessageBox::Critical);
        zv_deviceButtonInquiryTimer = startTimer(zv_inquiryPeriod);
        return false;
    }

    // reset passed exposition time
    zv_currentExpositionStartUnixTime = QDateTime::currentMSecsSinceEpoch();
    zv_currentExpositionSecondPassed = 0;
    zv_currentTime = 0;
    // buffer is clean
    // launch timer
    zv_measuringTimer = startTimer(zv_measuringInquiryPeriod, Qt::PreciseTimer);
    emit zg_expositionPassed(zv_passedExpositionsSummMS);

    //    qint64 measuredSpectrumId = zv_currentSampleTask->zp_spectrumIdForConditions(gainFactor,
    //                                                                                 zv_currentMeasuringConditions.at(zv_currentMeasuringIndex).second);
    ////    qDebug() << "SP ID" << measuredSpectrumId;
    //    emit zg_spectrumIsMeasuring(measuredSpectrumId);


    return true;
}
//=================================================
void ZMeasuringController::zh_processMeasurementResults()
{
    if(zv_connector == nullptr)
    {
        if(zv_deviceButtonInquiryTimer)
        {
            qDebug() << "KILL 1";
            killTimer(zv_deviceButtonInquiryTimer);
            zv_deviceButtonInquiryTimer = 0;
        }
        zv_controllerState = CS_DISCONNECTED;

        // end of measuring
        if(zv_measuringTimer)
        {
            qDebug() << "KILL 2";
            killTimer(zv_measuringTimer);
            zv_measuringTimer = 0;
        }
        zv_deviceButtonInquiryTimer = startTimer(zv_inquiryPeriod);

        return;
    }

    // check buffer
    if(zv_buffer == nullptr)
    {
        if(zv_measuringTimer)
        {
            qDebug() << "KILL 3";

            killTimer(zv_measuringTimer);
            zv_measuringTimer = 0;
        }
        QString errorMsg = tr("The buffer has not been created.");
        emit zg_message(errorMsg, QMessageBox::Critical);
        zh_createDataBuffer();
        return;
    }

    // read result from device
    ZUralAdcDeviceConnector::SlotResult res;
    zv_connector->zp_readResultBuffer(zv_buffer,
                                      &zv_Load,
                                      &zv_DeadTime,
                                      &zv_CountEvent,
                                      &zv_Time,
                                      res);

    if(res == ZUralAdcDeviceConnector::SR_FUNCTION_UNRESOLVED)
    {
        if(zv_measuringTimer)
        {
            qDebug() << "KILL 4";

            killTimer(zv_measuringTimer);
            zv_measuringTimer = 0;
        }
        QString errorMsg = tr("The function for clear buffer has not been resolved.");
        emit zg_message(errorMsg, QMessageBox::Critical);
        return;
    }

    // result from device - ok
    // calc current Exposition passed and if it has been changed send to dashboard
    quint64 currentExpositionMsPassed = static_cast<quint64>(QDateTime::currentMSecsSinceEpoch() - zv_currentExpositionStartUnixTime);
    if(currentExpositionMsPassed > zv_currentExpositionMS)
    {
        currentExpositionMsPassed = zv_currentExpositionMS;
    }
    quint32 currentExpositionSecondPassed = static_cast<quint32>(currentExpositionMsPassed / 1000);

    if(zv_currentExpositionSecondPassed != static_cast<quint32>(currentExpositionSecondPassed)
            && /*currentExpositionMsPassed*/zv_Time < zv_currentExpositionMS)
    {
        zv_currentExpositionSecondPassed = static_cast<quint32>(currentExpositionSecondPassed);
        emit zg_expositionPassed(zv_passedExpositionsSummMS + static_cast<qint64>(currentExpositionMsPassed));
    }

    // update statusbar message that connection is ok
    QString msg = tr("Connection to ADC - OK.");
    emit zg_connectionState(msg, QMessageBox::Information);

    // check income data for change
    if(zv_currentTime == zv_Time)
    {
        // data has not been chaqnged
        return;
    }
    zv_currentTime = zv_Time;

    // calc deadTime in MS and spectrumIntensityIntegral and send measuring parameters
    quint32 deadTime = static_cast<quint32>((static_cast<double>(zv_DeadTime) * (static_cast<double>(zv_ticDuration) / 1000.0)));
    CHANNEL integral = 0;
    for(int i = 0; i < zv_bufferChannelSize; i++)
    {
        integral += *(zv_buffer + i);
    }
    emit zg_currentMeasuringParameters(zv_Time, deadTime, integral);

    // if exposition is in pogress get spe data and send it to the further handling
    QList<CHANNEL> speDataList;
    if(zv_Time < zv_currentExpositionMS)
    {
        for(int i = 0; i < zv_bufferChannelSize; i+=2)
        {
            speDataList.append(*(zv_buffer + i));
        }

        QList<double> currentSpectrumEnergyCalibration = zv_currentSampleTask->zp_spectrumEnergyCalibrationForConditions(zv_currentMeasuringConditions.at(zv_currentMeasuringIndex).first,
                                                                                                                         zv_currentMeasuringConditions.at(zv_currentMeasuringIndex).second);


        zv_currentSampleTask->zp_handleSpectrumData(speDataList,
                                                    currentSpectrumEnergyCalibration,
                                                    zv_currentMeasuringConditions.at(zv_currentMeasuringIndex).first,
                                                    zv_currentMeasuringConditions.at(zv_currentMeasuringIndex).second,
                                                    zv_Time,
                                                    deadTime,
                                                    false);

        qint64 measuredSpectrumId = zv_currentSampleTask->zp_spectrumIdForConditions(zv_currentMeasuringConditions.at(zv_currentMeasuringIndex).first,
                                                                                     zv_currentMeasuringConditions.at(zv_currentMeasuringIndex).second);
        emit zg_currentSpectrumId(measuredSpectrumId);

        emit zg_currentEnergyCalibration(currentSpectrumEnergyCalibration);

        // continue measurement
        return;
    }

    // exposition done
    // stop exposition to device and kill measuring timer
    bool sampleMeasuringDone = zv_currentMeasuringIndex + 1 >= zv_currentMeasuringConditions.count();
    quint32 x = sampleMeasuringDone ? static_cast<quint32>(zv_sampleShiftImpulseDuration) : 0x00000000;
    zv_connector->zp_stopExposition(res, x);

    if(zv_measuringTimer)
    {

        killTimer(zv_measuringTimer);
        zv_measuringTimer = 0;
    }

    // calc sample common measuring duration
    zv_passedExpositionsSummMS += zv_currentExpositionMS;
    emit zg_expositionPassed(zv_passedExpositionsSummMS);

    // get last piece of  result
    zv_connector->zp_readResultBuffer(zv_buffer,
                                      &zv_Load,
                                      &zv_DeadTime,
                                      &zv_CountEvent,
                                      &zv_Time,
                                      res);

    // get spe data and send it to the further handling
    speDataList.clear();

    QString dbg;
    switch(res)
    {
    case ZUralAdcDeviceConnector::SR_FUNCTION_UNRESOLVED:
        dbg = "SR_FUNCTION_UNRESOLVED";
        break;
    case ZUralAdcDeviceConnector::SR_ERROR:
        dbg = "SR_ERROR";
        break;
    case ZUralAdcDeviceConnector::SR_FALSE:
        dbg = "SR_FALSE";
        break;
    case ZUralAdcDeviceConnector::SR_TRUE:

        for(int i = 0; i < zv_bufferChannelSize; i+=2)
        {
            speDataList.append(*(zv_buffer + i));
        }

        QList<double> currentSpectrumEnergyCalibration = zv_currentSampleTask->zp_spectrumEnergyCalibrationForConditions(zv_currentMeasuringConditions.at(zv_currentMeasuringIndex).first,
                                                                                                                         zv_currentMeasuringConditions.at(zv_currentMeasuringIndex).second);

        zv_currentSampleTask->zp_handleSpectrumData(speDataList,
                                                    currentSpectrumEnergyCalibration,
                                                    zv_currentMeasuringConditions.at(zv_currentMeasuringIndex).first,
                                                    zv_currentMeasuringConditions.at(zv_currentMeasuringIndex).second,
                                                    zv_Time,
                                                    deadTime,
                                                    true);
        qint64 measuredSpectrumId = zv_currentSampleTask->zp_spectrumIdForConditions(zv_currentMeasuringConditions.at(zv_currentMeasuringIndex).first,
                                                                                     zv_currentMeasuringConditions.at(zv_currentMeasuringIndex).second);
        emit zg_currentSpectrumId(measuredSpectrumId);

        emit zg_currentEnergyCalibration(currentSpectrumEnergyCalibration);

        break;
    }

    // check is measuring done or it's still have to take more spectra
    if(sampleMeasuringDone)
    {
        // measuring finished
        if(zv_measuringTimer)
        {
            killTimer(zv_measuringTimer);
            zv_measuringTimer = 0;
        }

        zv_currentMeasuringIndex = -1;
        zv_currentSampleTask->zp_measuringFinished();
        // emit zg_spectrumIsMeasuring(-1);

        // zv_deviceButtonInquiryTimer = startTimer(zv_inquiryPeriod);
        return;
    }

    // take one more spectrum
    ++zv_currentMeasuringIndex;
    zh_startSingleMeasuring();
}
//=================================================
void ZMeasuringController::zh_inquiryDeviceButton()
{
    if(zv_connector == nullptr)
    {
        killTimer(zv_deviceButtonInquiryTimer);
        zv_deviceButtonInquiryTimer = 0;
        zv_controllerState = CS_DISCONNECTED;

        // end of measuring
        killTimer(zv_measuringTimer);
        zv_measuringTimer = 0;
        zv_deviceButtonInquiryTimer = startTimer(zv_inquiryPeriod);

        return;
    }


    if(zv_connector == nullptr)
    {
        killTimer(zv_deviceButtonInquiryTimer);
        zv_deviceButtonInquiryTimer = 0;
        zv_controllerState = CS_DISCONNECTED;
        return;
    }

    ZUralAdcDeviceConnector::SlotResult res;
    zv_connector->zp_isDeviceConnected(res);
    if(res != ZUralAdcDeviceConnector::SR_TRUE)
    {
        QString msg = tr("Connection to ADC device is lost. No handle.");
        emit zg_connectionState(msg, QMessageBox::Critical);
        //        killTimer(zv_deviceButtonInquiryTimer);
        //        zv_deviceButtonInquiryTimer = 0;
        zv_controllerState = CS_DISCONNECTED;
        return;
    }

    for(int i = 0; i < 10; i++)
    {
        zv_connector->zp_isHardwareButtonDown(res);
        if(res != ZUralAdcDeviceConnector::SR_TRUE
                && res != ZUralAdcDeviceConnector::SR_FALSE)
        {
            continue;
        }
    }
    zv_connector->zp_isHardwareButtonDown(res);
    if(res != ZUralAdcDeviceConnector::SR_TRUE
            && res != ZUralAdcDeviceConnector::SR_FALSE)
    {
        QString msg = tr("Connection to ADC device is lost.");
        emit zg_connectionState(msg, QMessageBox::Critical);
        //        killTimer(zv_deviceButtonInquiryTimer);
        //        zv_deviceButtonInquiryTimer = 0;
        zv_controllerState = CS_DISCONNECTED;
        return;
    }

    // connection ok
    QString msg = tr("Connection to ADC -OK.");
    zv_controllerState = CS_SUSPENDED;
    emit zg_connectionState(msg, QMessageBox::Information);
}
//=================================================
