//=================================================
#include "ZSampleTask.h"
#include "ZGeneral.h"
#include "ZCalibration.h"
#include "ZXMLCalibrationIOHandler.h"
#include "ZMeasuringController.h"
#include "ZSample.h"
#include "ZSpeSpectrum.h"

#include <QSqlError>
#include <QSqlQuery>
//=================================================
bool ZSampleTask::zp_instanceSampleTaskObject(int sampleTaskId,
                                              ZSampleTask*& task,
                                              QString& msg,
                                              ZMeasuringController* measuringController,
                                              QObject* parent)
{
    msg.clear();
    if(sampleTaskId < 0 || parent == 0)
    {
        task = 0;
        msg = "Invalid sample task id.";
        return false;
    }

    // Get sampleTask data from database
    // name
    QSqlQuery query;
    QString queryString = QString("SELECT * FROM sample_tasks "
                                  "WHERE id=%1").arg(QString::number(sampleTaskId));

    if(!query.prepare(queryString))
    {
        msg = query.lastError().text();
        return false;
    }

    if(!query.exec())
    {
        msg = query.lastError().text();
        return false;
    }

    if(!query.next())
    {
        msg = tr("There is no sample task that has the id = %1.").arg(QString::number(sampleTaskId));
        return false;
    }

    QVariant vData = query.value(1);
    if(!vData.isValid() || !vData.canConvert<QString>())
    {
        msg = tr("Cannot get the name of the task id=%1 from sql query.").arg(QString::number(sampleTaskId));
        return false;
    }

    QString name = vData.toString();
    task = new ZSampleTask(sampleTaskId, name, measuringController, parent);

    return true;
}
//=================================================
ZSampleTask::ZSampleTask(int id,
                         const QString &name,
                         ZMeasuringController* measuringController,
                         QObject *parent)
    : QObject(parent)
{
    zv_sampleTaskId = id;
    zv_name = name;
    zv_measuringController = measuringController;
    zv_currentSample = 0;
    zh_initMeasuringTasks();
}
//=================================================
void ZSampleTask::zh_initMeasuringTasks()
{
    QSqlQuery query;
//    QString queryString = QString("SELECT * FROM conditions_has_sample_tasks "
//                                  "WHERE sample_tasks_id=%1").arg(QString::number(zv_id));

    QString queryString = QString("SELECT id "
//                                  "measuring_conditions.gain_factor, "
//                                  "measuring_conditions.exposition "
                                  "FROM conditions_has_sample_tasks "
//                                  "JOIN measuring_conditions "
                                  // "ON conditions_has_sample_tasks.measuring_conditions_id=conditions_has_sample_tasks.measuring_conditions_id "
//                                  "ON conditions_has_sample_tasks.measuring_conditions_id=measuring_conditions.id "
                                  "WHERE sample_tasks_id=%1").arg(QString::number(zv_sampleTaskId));

    if(!query.prepare(queryString))
    {
        return;
    }

    if(!query.exec())
    {
        return;
    }

    int measuringConditionsHasSampleTaskId;
    QVariant vData;

    while(query.next())
    {
        // measuring id
        vData = query.value(0);
        if(!vData.isValid() || !vData.canConvert<int>())
        {
            continue;;
        }

        measuringConditionsHasSampleTaskId = vData.toInt();

        ZMeasuringTask* measuringTask = new ZMeasuringTask(measuringConditionsHasSampleTaskId, this);

        zv_measuringTaskList.append(measuringTask);

    }
}
//=================================================
ZSampleTask::~ZSampleTask()
{

}
//=================================================
int ZSampleTask::zp_id() const
{
    return zv_sampleTaskId;
}
//=================================================
QString ZSampleTask::zp_name() const
{
    return zv_name;
}
//=================================================
void ZSampleTask::zp_appendClient(QObject *client)
{
    if(zv_clientList.contains(client))
    {
        return;
    }

    zv_clientList.append(client);
}
//=================================================
void ZSampleTask::zp_removeClient(QObject* client )
{

    if(!zv_clientList.contains(client))
    {
        return;
    }

    if(zv_currentSample == client)
    {
        // zp_stopMeasuring(zv_currentSample);
        emit zg_invokeToStopMeasurenent();
    }

    zv_clientList.removeAt(zv_clientList.indexOf(client));

    if(zv_clientList.count() < 1)
    {
        emit zg_inquiryToDelete();
    }
}
//=================================================
QStringList ZSampleTask::zp_chemicalStringList() const
{
    QStringList chemicalList;
    for(int i = 0; i < zv_measuringTaskList.count(); i++)
    {
        chemicalList.append(zv_measuringTaskList.at(i)->zp_chemicalList());
    }

    return chemicalList;
}
//=================================================
QStringList ZSampleTask::zp_measuringConditionsStringlist() const
{
    QStringList measuringConditionsList;
    for(int i = 0; i < zv_measuringTaskList.count(); i++)
    {
        measuringConditionsList.append(zv_measuringTaskList.at(i)->zp_measuringConditionsString());
    }

    return measuringConditionsList;
}
//=================================================
QList<QPair<quint8, int> > ZSampleTask::zp_measuringConditionsList() const
{
    QList<QPair<quint8,int> > measuringConditionsList;
    for(int i = 0; i < zv_measuringTaskList.count(); i++)
    {
        measuringConditionsList.append(zv_measuringTaskList.at(i)->zp_measuringConditions());
    }

    return measuringConditionsList;
}
//=================================================
QMap<int, QPair<quint8,int> > ZSampleTask::zp_measuringConditionsMap() const
{
    QMap<int, QPair<quint8,int> > measuringConditionsMap;
    foreach(ZMeasuringTask* task, zv_measuringTaskList)
    {
        measuringConditionsMap.insert(task->zp_measuringConditionsId(), task->zp_measuringConditions());
    }

    return measuringConditionsMap;
}
//=================================================
int ZSampleTask::zp_totalMeasuringDuration() const
{
    int measuringDuration = 0;
    for(int i = 0; i < zv_measuringTaskList.count(); i++)
    {
        measuringDuration += (zv_measuringTaskList.at(i)->zp_exposition());
    }

    return measuringDuration;
}
//=================================================
bool ZSampleTask::zp_startMeasuring(ZSample* sample)
{
    if(!zv_measuringController)
    {
        return false;
    }

    if(!zv_measuringController->zp_measureSample(zp_measuringConditionsList(), this))
    {
        return false;
    }

    zv_currentSample = sample;
    return true;
}
//=================================================
bool ZSampleTask::zp_stopMeasuring(ZSample* sample)
{
    if(!zv_measuringController)
    {
        return false;
    }

    zv_measuringController->zp_stopMeasuring();
    zv_currentSample = nullptr;
    return false;
}
//=================================================
void ZSampleTask::zp_measuringFinished()
{
    if(zv_currentSample)
    {
        zv_currentSample->zp_measuringFinished();
    }
}
//=================================================
void ZSampleTask::zp_handleSpectrumData(QList<quint32> speDataList,
                                        quint8 gainFactor,
                                        int exposition,
                                        quint32 time,
                                        quint32 deadTime,
                                        bool finished)
{
    if(zv_currentSample)
    {
        zv_currentSample->zp_setSpectrumData(speDataList, gainFactor, exposition, time,
                                             deadTime, finished);
    }
}
//=================================================
void ZSampleTask::zp_calcConcentrations(quint8 gainFactor, int exposition,
                                        const ZSpeSpectrum* spectrum,
                                        QList<ZChemicalConcentration>&  concentrationList) const
{
    for(int mt = 0; mt < zv_measuringTaskList.count(); mt++)
    {
        QPair<quint8, int> measuringConditions = zv_measuringTaskList.at(mt)->zp_measuringConditions();
        if(measuringConditions.first != gainFactor && measuringConditions.second != exposition)
        {
            continue;
        }

        zv_measuringTaskList.at(mt)->zp_calcConcentrations(spectrum, concentrationList);

    }
}
//=================================================
qint64 ZSampleTask::zp_spectrumIdForConditions(quint8 gainFactor,
                         int exposition) const
{
    if(!zv_currentSample)
    {
        return -1;
    }

    ZSpeSpectrum* spectrum =  zv_currentSample->zp_spectrumForMeasuringConditions(gainFactor, exposition);
    if(!spectrum)
    {
        return -1;
    }

    return spectrum->zp_spectrumId();
}
//=================================================
QList<double> ZSampleTask::zp_spectrumEnergyCalibrationForConditions(quint8 gainFactor, int exposition) const
{
    QList<double> energyCalibration;
    if(zv_currentSample)
    {
        ZSpeSpectrum* spectrum =  zv_currentSample->zp_spectrumForMeasuringConditions(gainFactor, exposition);
        if(spectrum)
        {
            energyCalibration = spectrum->zp_energyCalibration();
        }
    }

    return energyCalibration;
}
//=================================================
QList<double> ZSampleTask::zp_energyCalibrationForGainFactor(quint8 gainFactor) const
{
    for(auto& measuringTask : zv_measuringTaskList)
    {
        if(measuringTask->zp_measuringConditions().first == gainFactor)
        {
            return measuringTask->zp_energyCalibrationFactorList();
        }
    }

    return QList<double>();
}
//=================================================
void ZSampleTask::zp_setEnergyCalibration(quint8 gainFactor,
                                          const QList<double>& energyCalibrationFactorList)
{
    for(auto& measuringTask : zv_measuringTaskList)
    {
        if(measuringTask->zp_measuringConditions().first == gainFactor)
        {
            measuringTask->zp_setEnergyCalibrationFactorList(energyCalibrationFactorList);
        }
    }
}
//=================================================
// MEASURING TASK
//=================================================
ZMeasuringTask::ZMeasuringTask(int measurementConditionsHasSampleTaskId, QObject* parent)
    : QObject(parent)
{
    QSqlQuery query;
    QString queryString = QString("SELECT measuring_conditions.id, "
                                  "measuring_conditions.gain_factor,"
                                  "measuring_conditions.exposition "
                                  "FROM conditions_has_sample_tasks "
                                  "JOIN measuring_conditions "
                                  "ON conditions_has_sample_tasks.measuring_conditions_id=measuring_conditions.id "
                                  "WHERE conditions_has_sample_tasks.id=%1").arg(QString::number(measurementConditionsHasSampleTaskId));


    if(!query.prepare(queryString))
    {
        return;
    }

    if(!query.exec())
    {
        return;
    }

    if(!query.next())
    {
        return;
    }

    // id
    QVariant vData = query.value(0);
    if(!vData.isValid() || !vData.canConvert<int>())
    {
        return;
    }

    zv_id = vData.toInt();

    // gain factor
    vData = query.value(1);
    if(!vData.isValid() || !vData.canConvert<int>())
    {
        return;
    }

    zv_gainFactor = static_cast<quint8>(vData.toInt());

    vData = query.value(2);
    if(!vData.isValid() || !vData.canConvert<int>())
    {
        return;
    }

    zv_exposition = vData.toInt();

    zh_initEnergyCalibration();
    zh_initCalculationTasks(measurementConditionsHasSampleTaskId);
}
//=================================================
void ZMeasuringTask::zh_initEnergyCalibration()
{
    QSqlQuery query;
    QString queryString = QString("SELECT energyFactorK0, energyFactorK1, energyFactorK2 "
                                  "FROM gain_factors "
                                  "WHERE gain_factor=%1").arg(QString::number(zv_gainFactor));

    if(!query.prepare(queryString))
    {
        return;
    }

    if(!query.exec())
    {
        return;
    }

    QVariant vData;
    double energyFactor;
    bool ok;
    if(query.next())
    {
        for(int i = 0; i < 3; i++)
        {
            energyFactor = 0.0;

            vData = query.value(i);
            if(vData.isValid() && vData.canConvert<double>())
            {
                energyFactor = vData.toDouble(&ok);
                if(!ok)
                {
                    energyFactor = 0.0;
                }
            }

            zv_energyCalibrationFactorList.append(energyFactor);
        }
    }
}
//=================================================
void ZMeasuringTask::zh_initCalculationTasks(int measurementConditionsHasSampleTaskId)
{
    QSqlQuery query;
    QString queryString = QString("SELECT calibration_stacks_id "
                                  "FROM calibration_stacks_has_conditions_has_sample_tasks "
                                  "WHERE conditions_has_sample_tasks_id=%1").arg(QString::number(measurementConditionsHasSampleTaskId));


    if(!query.prepare(queryString))
    {
        return;
    }

    if(!query.exec())
    {
        return;
    }

    QVariant vData;
    int chemicalTaskId;
    while(query.next())
    {
        vData = query.value(0);
        if(!vData.isValid() || !vData.canConvert<int>())
        {
            continue;
        }

        chemicalTaskId = vData.toInt();

        QString msg;
        ZChemicalTask* calculationTask;
        ZChemicalTask::zp_instanceChemicalTaskObject(chemicalTaskId,
                                                     calculationTask,
                                                     msg,
                                                     this);

        zv_chemicalTaskList.append(calculationTask);
    }
}
//=================================================
QStringList ZMeasuringTask::zp_chemicalList() const
{
    QStringList chemicalList;

    for(int i = 0; i < zv_chemicalTaskList.count(); i++)
    {
        chemicalList.append(zv_chemicalTaskList.at(i)->zp_chemical());
    }

    return chemicalList;
}
//=================================================
QString ZMeasuringTask::zp_measuringConditionsString() const
{
    return tr("G.F. - %1, Exp. - %2").arg(QString::number(zv_gainFactor),
                                          QString::number(zv_exposition));

}
//=================================================
QPair<quint8, int> ZMeasuringTask::zp_measuringConditions() const
{
    QPair<quint8,int> measuringConditions;
    measuringConditions.first = zv_gainFactor;
    measuringConditions.second = zv_exposition;
    return measuringConditions;
}
//=================================================
int ZMeasuringTask::zp_measuringConditionsId() const
{
    return zv_id;
}
//=================================================
int ZMeasuringTask::zp_exposition() const
{
    return zv_exposition;
}
//=================================================
void ZMeasuringTask::zp_calcConcentrations(const ZSpeSpectrum* spectrum,
                                           QList<ZChemicalConcentration>& chemicalConcentrationList)
{
    for(int ct = 0; ct < zv_chemicalTaskList.count(); ct++)
    {
        ZChemicalConcentration chemicalConcentration;
        if(zv_chemicalTaskList.at(ct)->zp_calculateConcentration(spectrum, chemicalConcentration))
        {
            chemicalConcentrationList.append(chemicalConcentration);
        }
    }
}
//=================================================
QList<double> ZMeasuringTask::zp_energyCalibrationFactorList() const
{
    return zv_energyCalibrationFactorList;
}
//=================================================
void ZMeasuringTask::zp_setEnergyCalibrationFactorList(const QList<double>& energyCalibrationFactorList)
{
    zv_energyCalibrationFactorList = energyCalibrationFactorList;
}
//=================================================
// CALCULATION TASK
//=================================================
bool ZChemicalTask::zp_instanceChemicalTaskObject(int chemicalTaskId,
                                                  ZChemicalTask*& task,
                                                  QString& msg,
                                                  QObject *parent)
{
    QSqlQuery query;
    QString queryString = QString("SELECT * FROM calibration_stacks "
                                  "WHERE id=%1").arg(QString::number(chemicalTaskId));

    if(!query.prepare(queryString))
    {
        msg = query.lastError().text();
        return false;
    }

    if(!query.exec())
    {
        msg = query.lastError().text();
        return false;
    }

    if(!query.next())
    {
        msg = tr("There is no chemical task that has id=%1.").arg(QString::number(chemicalTaskId));
        return false;
    }

    QVariant vData = query.value(0);

    if(!vData.isValid() || !vData.canConvert<int>())
    {
        msg = tr("Cannot convert id value for id=%1 from QVariant to integer.").arg(QString::number(chemicalTaskId));
        return false;
    }

    if(vData.toInt() != chemicalTaskId)
    {
        msg = tr("There is no chemical task that has id=%1.").arg(QString::number(chemicalTaskId));
        return false;
    }



    task = new ZChemicalTask(msg, chemicalTaskId, query, parent);
    return true;
}
//=================================================
bool ZChemicalTask::zp_calculateConcentration(const ZSpeSpectrum* spectrum,
                                              ZChemicalConcentration& chemicalConcentration)
{
    if(zv_calibrationList.isEmpty())
    {
        return false;
    }
    qreal concentration = -1;
    for(int c = 0; c < zv_calibrationList.count(); c++ )
    {
        zv_calibrationList.at(c)->zp_calcConcentration(spectrum, concentration);

        chemicalConcentration.zv_concentration = concentration;
        chemicalConcentration.zv_chemical = zv_chemical;
        chemicalConcentration.zv_calibrationId = zv_calibrationList.at(c)->zp_databaseCalibrationId();

        if(zv_calibrationList.at(c)->zp_isConcentrationInBounds(concentration))
        {
             break;
        }
    }

    return true;
}
//=================================================
ZChemicalTask::ZChemicalTask(QString& msg, int chemicalTaskId, QSqlQuery& query, QObject* parent)
    : QObject(parent)
{
    zv_chemicalTaskId = chemicalTaskId;

    // task name
    QVariant vData = query.value(1);

    if(!vData.isValid() || !vData.canConvert<QString>())
    {
        zv_taskName = tr("Not defined");
    }
    else
    {
        zv_taskName = vData.toString();
    }

    // task description
    vData = query.value(2);

    if(!vData.isValid() || !vData.canConvert<QString>())
    {
        zv_taskDescription = QString();
    }
    else
    {
        zv_taskDescription = vData.toString();
    }

    // chemical id
    vData = query.value(3);
    if(vData.isValid() && vData.canConvert<int>())
    {
        zh_getChemical(vData.toInt());
    }

    // calibrations
    query.clear();
    QString queryString = QString("SELECT * FROM calibrations "
                                  "WHERE id "
                                  "IN (SELECT calibrations_id FROM "
                                  "calibrations_has_calibration_stacks "
                                  "WHERE calibration_stacks_id=%1)").arg(QString::number(chemicalTaskId));

    if(!query.prepare(queryString))
    {
        msg = query.lastError().text();
        return;
    }

    if(!query.exec())
    {
        msg = query.lastError().text();
        return;
    }

    QString calibrationString;
    QString calibrationName;
    ZZonedCalibration* calibration;
    ZXMLCalibrationIOHandler ioHandler;
    int calibrationId;
    while(query.next())
    {
        //calibration Id
        vData = query.value(0);
        if(!vData.isValid() || !vData.canConvert<int>())
        {
            // clear task and quit
            msg = tr("Cannot get calibration id for chemical task id=%1.")
                    .arg(QString::number(chemicalTaskId));
            qDeleteAll(zv_calibrationList);
            return;
        }
        else
        {
            calibrationId =vData.toInt();
        }

        // calibration name
        vData = query.value(1);
        if(!vData.isValid() || !vData.canConvert<QString>())
        {
            calibrationName = QString("Not defined");
        }
        else
        {
            calibrationName = vData.toString();
        }

        // calibration string
        vData = query.value(3);
        if(!vData.isValid() || !vData.canConvert<QString>())
        {
            continue;
        }

        calibrationString = vData.toString();
        calibration = new ZZonedCalibration(calibrationName, calibrationId, this);

        if(!ioHandler.zp_getCalibrationFromString(calibrationString, calibration))
        {
            delete calibration;
        }
        else
        {
            // limits
            QSqlQuery limitQuery;
            double minimumConcentration = 0.0;
            double maximumConcentration = 0.0;
            QString limitQueryString = QString("SELECT calibration_min_limit, calibration_max_limit "
                                               "FROM calibrations_has_calibration_stacks "
                                               "WHERE calibration_stacks_id=%1 "
                                               "AND calibrations_id=%2").arg(QString::number(chemicalTaskId),
                                                                             QString::number(calibrationId));

            if(!limitQuery.prepare(limitQueryString))
            {
                msg = query.lastError().text();
                qDeleteAll(zv_calibrationList);
                return;
            }

            if(!limitQuery.exec())
            {
                msg = query.lastError().text();
                qDeleteAll(zv_calibrationList);
                return;
            }

            if(!limitQuery.next())
            {
                msg = tr("Cannot get calibration limits for chemical task id=%1.")
                        .arg(QString::number(chemicalTaskId));
                qDeleteAll(zv_calibrationList);
                return;
            }

            // min
            vData = limitQuery.value(0);
            if(!vData.isValid() || !vData.canConvert<double>())
            {
                msg = tr("Cannot cast limits from QVariant for chemical task id=%1.")
                        .arg(QString::number(chemicalTaskId));
                qDeleteAll(zv_calibrationList);
                return;
            }

            minimumConcentration = vData.toDouble();

            // max
            vData = limitQuery.value(1);
            if(!vData.isValid() || !vData.canConvert<double>())
            {
                msg = tr("Cannot cast limits from QVariant for chemical task id=%1.")
                        .arg(QString::number(chemicalTaskId));
                qDeleteAll(zv_calibrationList);
                return;

            }

            maximumConcentration = vData.toDouble();
            calibration->zp_setMinMaConcentrations(minimumConcentration, maximumConcentration);

            zv_calibrationList.append(calibration);
        }
    }
}
//=================================================
QString ZChemicalTask::zp_chemical() const
{
    return zv_chemical;
}
//=================================================
void ZChemicalTask::zh_getChemical(int chemicalId)
{
    QSqlQuery query;
    QString queryString = QString("SELECT * FROM chemicals "
                                  "WHERE id=%1").arg(QString::number(chemicalId));

    if(!query.prepare(queryString))
    {
        return;
    }

    if(!query.exec())
    {
        return;
    }

    if(!query.next())
    {
        return;
    }

    QVariant vData = query.value(1);

    if(!vData.isValid() || !vData.canConvert<QString>())
    {
        return;
    }

    zv_chemical = vData.toString();
}
//=================================================
