//=================================================
#include "ZSampleTask.h"
#include "ZGeneral.h"
#include "ZCalibration.h"
#include "ZXMLCalibrationIOHandler.h"

#include <QSqlError>
#include <QSqlQuery>

//=================================================
bool ZSampleTask::zp_instanceSampleTaskObject(int sampleTaskId,
                                              ZSampleTask*& task,
                                              QString& msg,
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
    task = new ZSampleTask(sampleTaskId, name, parent);

    return true;
}
//=================================================
ZSampleTask::ZSampleTask(int id, const QString &name, QObject *parent)
    : QObject(parent)
{
    zv_id = id;
    zv_name = name;
    zh_initMeasuringTasks();
}
//=================================================
void ZSampleTask::zh_initMeasuringTasks()
{
    QSqlQuery query;
    QString queryString = QString("SELECT * FROM conditions_has_sample_tasks "
                                  "WHERE sample_tasks_id=%1").arg(QString::number(zv_id));

    if(!query.prepare(queryString))
    {
        return;
    }

    if(!query.exec())
    {
        return;
    }

    int measuringConditionsId;
    int gainFactor;
    int exposition;
    QVariant vData;
    while(query.next())
    {
        // measuring id
        vData = query.value(0);
        if(!vData.isValid() || !vData.canConvert<int>())
        {
            continue;;
        }

        measuringConditionsId = vData.toInt();

        //gain factor
        vData = query.value(1);
        if(!vData.isValid() || !vData.canConvert<int>())
        {
            continue;;
        }

        gainFactor = vData.toInt();

        // exposition
        vData = query.value(2);
        if(!vData.isValid() || !vData.canConvert<int>())
        {
            continue;;
        }

        exposition = vData.toInt();

        ZMeasuringTask* measuringTask = new ZMeasuringTask(measuringConditionsId,
                                                           gainFactor, exposition, this);

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
    return zv_id;
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
    qDebug() << QString("Appended client to %1: ").arg(zv_name);

}
//=================================================
void ZSampleTask::zp_removeClient(QObject* client )
{
    if(!zv_clientList.contains(client))
    {
        return;
    }

    zv_clientList.removeAt(zv_clientList.indexOf(client));
    qDebug() << QString("Removed client from %1: ").arg(zv_name);

    if(zv_clientList.count() < 1)
    {
        emit zg_requestToDelete();
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
QList<QPair<int,int> > ZSampleTask::zp_measuringConditionsList() const
{
    QList<QPair<int,int> > measuringConditionsList;
    for(int i = 0; i < zv_measuringTaskList.count(); i++)
    {
        measuringConditionsList.append(zv_measuringTaskList.at(i)->zp_measuringConditions());
    }

    return measuringConditionsList;
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
// MEASURING TASK
//=================================================
ZMeasuringTask::ZMeasuringTask(int id, int gainFactor, int exposition, QObject* parent)
    : QObject(parent)
{
    zv_id = id;
    zv_gainFactor = gainFactor;
    zv_exposition = exposition;

    zh_initCalculationTasks();

}
//=================================================
void ZMeasuringTask::zh_initCalculationTasks()
{
    QSqlQuery query;
    QString queryString = QString("SELECT calibration_stacks_id FROM calibration_stacks_has_conditions_has_sample_tasks "
                                  "WHERE conditions_has_sample_tasks_id=%1").arg(QString::number(zv_id));


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
QPair<int,int> ZMeasuringTask::zp_measuringConditions() const
{
    QPair<int,int> measuringConditions;
    measuringConditions.first = zv_gainFactor;
    measuringConditions.second = zv_exposition;
    return measuringConditions;
}
//=================================================
int ZMeasuringTask::zp_exposition() const
{
    return zv_exposition;
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
        calibration = new ZZonedCalibration(calibrationName, this);

        if(!ioHandler.zp_getCalibrationFromString(calibrationString, calibration))
        {
            delete calibration;
        }
        else
        {
            // limits
            QSqlQuery limitQuery;
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

            calibration->zv_minConcentration = vData.toDouble();

            // max
            vData = limitQuery.value(1);
            if(!vData.isValid() || !vData.canConvert<double>())
            {
                msg = tr("Cannot cast limits from QVariant for chemical task id=%1.")
                        .arg(QString::number(chemicalTaskId));
                qDeleteAll(zv_calibrationList);
                return;

            }

            calibration->zv_maxConcentration = vData.toDouble();
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
