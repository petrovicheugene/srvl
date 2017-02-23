//======================================================
#include "ZMeasuringManager.h"
#include "ZGeneral.h"

#include "ZAddSampleDialog.h"
#include "ZControlAction.h"
#include "ZSample.h"
#include "ZSampleTask.h"
#include "ZSeriesTaskListDialog.h"
#include "ZSeriesTaskListManager.h"

//======================================================
ZMeasuringManager::ZMeasuringManager(QObject *parent)
    : QObject(parent)
{
    zv_currentSampleIndex = -1;
    zv_currentSeriesTaskDirty = false;

    zh_createActions();
    zh_createComponents();
    zh_createConnections();

    zh_manageControlEnable();
}
//======================================================
void ZMeasuringManager::zh_createActions()
{
    zv_saveSeriesAction  = new ZControlAction(this);
    zv_saveSeriesAction->setText(tr("Save series"));

    zv_loadSeriesAction  = new ZControlAction(this);
    zv_loadSeriesAction->setText(tr("Load series"));

    zv_addSamplesToSeriesAction = new ZControlAction(this);
    zv_addSamplesToSeriesAction->setText(tr("Add samples"));

    zv_removeSamplesFromSeriesAction = new ZControlAction(this);
    zv_removeSamplesFromSeriesAction->setText(tr("Remove samples"));
}
//======================================================
void ZMeasuringManager::zh_createComponents()
{

}
//======================================================
void ZMeasuringManager::zh_createConnections()
{
    connect(zv_saveSeriesAction, &ZControlAction::triggered,
            this, &ZMeasuringManager::zh_onSaveSeriesAction);
    connect(zv_loadSeriesAction, &ZControlAction::triggered,
            this, &ZMeasuringManager::zh_onLoadSeriesAction);

    connect(zv_addSamplesToSeriesAction, &ZControlAction::triggered,
            this, &ZMeasuringManager::zh_onAddSamplesToSeriesAction);
    connect(zv_removeSamplesFromSeriesAction, &ZControlAction::triggered,
            this, &ZMeasuringManager::zh_onRemoveSamplesFromSeriesAction);

}
//======================================================
QList<ZControlAction*> ZMeasuringManager::zp_sampleActions() const
{
    QList<ZControlAction*> actionList;
    actionList.append(zv_saveSeriesAction);
    actionList.append(zv_loadSeriesAction);

    actionList.append(zv_addSamplesToSeriesAction);
    actionList.append(zv_removeSamplesFromSeriesAction);
    return actionList;
}
//======================================================
QList<ZControlAction*> ZMeasuringManager::zp_sampleContextActions() const
{
    QList<ZControlAction*> actionList;
    actionList.append(zv_saveSeriesAction);
    actionList.append(zv_loadSeriesAction);

    actionList.append(zv_addSamplesToSeriesAction);
    actionList.append(zv_removeSamplesFromSeriesAction);
    return actionList;
}
//======================================================
int ZMeasuringManager::zp_sampleCount() const
{
    return zv_sampleList.count();
}
//======================================================
QString ZMeasuringManager::zp_sampleName(int sampleIndex) const
{
    if(sampleIndex < 0 || sampleIndex >= zv_sampleList.count())
    {
        return QString();
    }

    return zv_sampleList.at(sampleIndex)->zp_sampleName();
}
//======================================================
bool ZMeasuringManager::zp_setSampleName(int sampleIndex, const QString& name)
{
    if(sampleIndex < 0 || sampleIndex >= zv_sampleList.count())
    {
        return false;
    }

    return zv_sampleList.at(sampleIndex)->zp_setSampleName(name);
}
//======================================================
QString ZMeasuringManager::zp_sampleTaskName(int sampleIndex) const
{
    if(sampleIndex < 0 || sampleIndex >= zv_sampleList.count())
    {
        return QString();
    }

    return zv_sampleList.at(sampleIndex)->zp_sampleTaskName();
}
//======================================================
QStringList ZMeasuringManager::zp_chemicalListForSample(int sampleIndex) const
{
    if(sampleIndex < 0 || sampleIndex >= zv_sampleList.count())
    {
        return QStringList();
    }

    return zv_sampleList.at(sampleIndex)->zp_chemicalList();
}
//======================================================
QStringList ZMeasuringManager::zp_measuringConditionsListForSample(int sampleIndex) const
{
    if(sampleIndex < 0 || sampleIndex >= zv_sampleList.count())
    {
        return QStringList();
    }

    return zv_sampleList.at(sampleIndex)->zp_measuringConditionsList();
}
//======================================================
QString ZMeasuringManager::zp_seriesTaskName() const
{
    return zv_currentSeriesTaskName;
}
//======================================================
void ZMeasuringManager::zg_setCurrentSampleIndex(int currentSampleIndex)
{
    if(currentSampleIndex >= zv_sampleList.count())
    {
        zv_currentSampleIndex = currentSampleIndex;
    }
}
//======================================================
void ZMeasuringManager::zh_onSaveSeriesAction()
{
    ZSeriesTaskListManager seriesManager;
    ZSeriesTaskListDialog dialog(false);
    dialog.zp_connectToManager(&seriesManager);
    connect(&seriesManager, &ZSeriesTaskListManager::zg_requestSampleTaskIdList,
            this, &ZMeasuringManager::zh_sampleTaskIdList);

    if(!dialog.exec())
    {
        return;
    }

    zv_currentSeriesTaskDirty = false;
    emit zg_seriesTaskNameDirtyChanged(zv_currentSeriesTaskDirty);
}
//======================================================
void ZMeasuringManager::zh_onLoadSeriesAction()
{
    if(zv_currentSeriesTaskDirty)
    {
        QString questionString = tr("Current series task has been changed. "
                                    "Do you want to save it?");
        if(QMessageBox::question(0, tr("Series task loading"), questionString,
                                 QMessageBox::Yes, QMessageBox::No) == QMessageBox::Yes)
        {
            zh_onSaveSeriesAction();
        }
    }

    ZSeriesTaskListManager seriesManager;
    ZSeriesTaskListDialog dialog(true);
    dialog.zp_connectToManager(&seriesManager);

    if(!dialog.exec())
    {
        return;
    }

    int seriesTaskId = seriesManager.zp_currentSeriesTaskId();
    zh_clearSeriesTask();
    zh_loadSeriesTask(seriesTaskId);

    zv_currentSeriesTaskDirty = false;
    emit zg_seriesTaskNameDirtyChanged(zv_currentSeriesTaskDirty);
    zh_manageControlEnable();
}
//======================================================
void ZMeasuringManager::zh_clearSeriesTask()
{
    for(int sampleIndex = zv_sampleList.count() - 1; sampleIndex >= 0; sampleIndex--)
    {
        emit zg_sampleOperation(SOT_SAMPLE_ABOUT_TO_BE_REMOVED, sampleIndex, sampleIndex);
        ZSample* sample = zv_sampleList.takeAt(sampleIndex);
        delete sample;
        emit zg_sampleOperation(SOT_SAMPLE_REMOVED, sampleIndex, sampleIndex);
    }
}
//======================================================
bool ZMeasuringManager::zh_loadSeriesTask(int seriesTaskId)
{
    QSqlQuery query;
    QString queryString = QString("SELECT * FROM series_tasks "
                                  "WHERE id=%1").arg(QString::number(seriesTaskId));

    if(!query.prepare(queryString))
    {
        return false;
    }

    if(!query.exec())
    {
        return false;
    }

    if(!query.next())
    {
        return false;
    }

    // series task name
    QVariant vData = query.value(1);
    if(!vData.isValid() || !vData.canConvert<QString>())
    {
        return false;
    }

    QString seriesTaskName = vData.toString();
    // emit zg_seriesTaskNameChanged(zv_currentSeriesTaskName);

    // description
    vData = query.value(2);
    if(!vData.isValid() || !vData.canConvert<QString>())
    {
        return false;
    }

    // samples
    query.clear();
    queryString = QString("SELECT sample_tasks_id FROM sample_tasks_has_series_tasks "
                                      "WHERE series_tasks_id=%1").arg(QString::number(seriesTaskId));

    if(!query.prepare(queryString))
    {
        return false;
    }

    if(!query.exec())
    {
        return false;
    }

    int sampleTaskId;
    while(query.next())
    {
        vData = query.value(0);
        if(!vData.isValid() || !vData.canConvert<int>())
        {
            continue;
        }

        sampleTaskId = vData.toInt();

        zh_addSamplesToSeries(sampleTaskId, 1, QString());
    }

    zv_currentSeriesTaskName = seriesTaskName;
    emit zg_seriesTaskNameChanged(zv_currentSeriesTaskName);

    return true;
}
//======================================================
void ZMeasuringManager::zh_onAddSamplesToSeriesAction()
{
    ZAddSampleDialog dialog;
    if(!dialog.exec())
    {
        return;
    }

    // get data from dialog
    int sampleTaskId = dialog.zp_sampleTaskId();
    QString defaultSampleName = dialog.zp_sampleName();
    int sampleQuantity = dialog.zp_sampleQuantity();

    zh_addSamplesToSeries(sampleTaskId, sampleQuantity, defaultSampleName);

    zh_manageControlEnable();
    zv_currentSeriesTaskDirty = true;
    emit zg_seriesTaskNameDirtyChanged(zv_currentSeriesTaskDirty);

}
//======================================================
bool ZMeasuringManager::zh_addSamplesToSeries(int sampleTaskId,
                                              int sampleQuantity,
                                              QString defaultSampleName)
{
    if(defaultSampleName.isEmpty())
    {
        QSqlQuery query;
        QString queryString = QString("SELECT sample_name_template FROM sample_tasks "
                                      "WHERE id=%1").arg(QString::number(sampleTaskId));
        if(!query.prepare(queryString))
        {
            return false;
        }

        if(!query.exec())
        {
            return false;
        }

        if(!query.next())
        {
            return false;
        }

        QVariant vData = query.value(0);
        if(!vData.isValid() || !vData.canConvert<QString>())
        {
            return false;
        }

        defaultSampleName = vData.toString();
    }

    // check data
    if(sampleTaskId < 0 || sampleQuantity < 1)
    {
        return false;
    }

    // get sample task
    ZSampleTask* task = zh_instanceSampleTask(sampleTaskId);

    if(task == 0)
    {
        return false;
    }

    // create samples
    int lastSampleSerialNumber = zh_findLastSampleSerialNumber(defaultSampleName);
    QString newSampleName;
    QString serialNumberString;
    // serial index correction
    if(lastSampleSerialNumber < 0 && sampleQuantity > 1)
    {
        lastSampleSerialNumber = 0;
    }

    for(int i = 0; i < sampleQuantity; i++)
    {
        for(int c = 0; c < 999999999; c++)
        {
            if(++lastSampleSerialNumber == 0)
            {
                serialNumberString = QString();
            }
            else
            {
                serialNumberString = "-" + QString::number(lastSampleSerialNumber);
            }
            newSampleName = defaultSampleName + serialNumberString;
            if(zh_checkSampleName(newSampleName))
            {
                break;
            }
        }

        zh_createSample(newSampleName, task);
    }

    return true;
}
//======================================================
void ZMeasuringManager::zh_onRemoveSamplesFromSeriesAction()
{
    QList<int> selectedSampleList;
    emit zg_requestSelectedSampleList(selectedSampleList);

    qSort(selectedSampleList);

    int sampleIndex;
    for(int i = selectedSampleList.count() - 1; i >= 0; i--)
    {
        sampleIndex = selectedSampleList.at(i);
        emit zg_sampleOperation(SOT_SAMPLE_ABOUT_TO_BE_REMOVED, sampleIndex, sampleIndex);
        ZSample* sample = zv_sampleList.takeAt(sampleIndex);
        delete sample;
        emit zg_sampleOperation(SOT_SAMPLE_REMOVED, sampleIndex, sampleIndex);
    }

    if(zv_sampleList.isEmpty())
    {
        zv_currentSeriesTaskName.clear();
        emit zg_seriesTaskNameChanged(zv_currentSeriesTaskName);
    }

    zv_currentSeriesTaskDirty = !zv_sampleList.isEmpty();
    emit zg_seriesTaskNameDirtyChanged(zv_currentSeriesTaskDirty);

    zh_manageControlEnable();
}
//======================================================
void ZMeasuringManager::zh_deleteSampleTask()
{
    ZSampleTask* sampleTask = qobject_cast<ZSampleTask*>(sender());
    if(!zv_sampleTaskList.contains(sampleTask))
    {
        return;
    }

    delete zv_sampleTaskList.takeAt(zv_sampleTaskList.indexOf(sampleTask));

    zv_currentSeriesTaskDirty = true && !zv_sampleList.isEmpty();
    emit zg_seriesTaskNameDirtyChanged(zv_currentSeriesTaskDirty);
    zh_manageControlEnable();
}
//======================================================
void ZMeasuringManager::zh_sampleTaskIdList(QList<int>& idList) const
{
    for(int i = 0; i < zv_sampleList.count(); i++)
    {
        idList.append(zv_sampleList.at(i)->zp_sampleTaskId());
    }
}
//======================================================
ZSampleTask* ZMeasuringManager::zh_instanceSampleTask(int sampleTaskId)
{
    if(sampleTaskId < 0)
    {
        return 0;
    }

    // search in already created
    foreach(ZSampleTask* task, zv_sampleTaskList)
    {
        if(task->zp_id() == sampleTaskId)
        {
            return task;
        }
    }

    // create new sampleTask
    ZSampleTask* task;
    QString msg;
    if(ZSampleTask::zp_instanceSampleTaskObject(sampleTaskId, task, msg, this))
    {
        zv_sampleTaskList.append(task);
        connect(task, &ZSampleTask::zg_requestToDelete,
                this, &ZMeasuringManager::zh_deleteSampleTask);

        return task;
    }

    //
    return 0;
}
//======================================================
void ZMeasuringManager::zh_createSample(const QString& sampleName, ZSampleTask* sampleTask)
{
    ZSample* sample = new ZSample(sampleName, sampleTask, this);
    int sampleIndex = zv_sampleList.count();
    emit zg_sampleOperation(SOT_SAMPLE_ABOUT_TO_BE_INSERTED, sampleIndex, sampleIndex);
    zv_sampleList.append(sample);
    emit zg_sampleOperation(SOT_SAMPLE_INSERTED, sampleIndex, sampleIndex);
}
//======================================================
int ZMeasuringManager::zh_findLastSampleSerialNumber(const QString &sampleName) const
{
    // return value : -1 - no smple with tne name exists, 0 - sample w/o serial number , other - serial number
    QString currentSampleName;
    int lastSerialNumber = -1;
    QString extraPart;
    bool ok;
    int currentSerialNumber;
    int postFixLength;

    foreach(ZSample* sample, zv_sampleList)
    {
        currentSampleName = sample->zp_sampleName();
        if(!currentSampleName.startsWith(sampleName))
        {
            continue;
        }

        // try to convert right part to Int
        postFixLength = currentSampleName.length() - sampleName.length();
        if(postFixLength == 0)
        {
            if(lastSerialNumber < 0)
            {
                lastSerialNumber = 0;
            }
            continue;
        }

        currentSerialNumber = currentSampleName.right(postFixLength).toInt(&ok);
        if(!ok)
        {
            continue;
        }

        if(lastSerialNumber < currentSerialNumber)
        {
            lastSerialNumber = currentSerialNumber;
        }
    }

    return lastSerialNumber;
}
//======================================================
bool ZMeasuringManager::zh_checkSampleName(const QString& sampleName) const
{
    foreach(ZSample* item, zv_sampleList)
    {
        if(item->zp_sampleName() == sampleName)
        {
            return false;
        }
    }
    return true;
}
//======================================================
void ZMeasuringManager::zh_manageControlEnable()
{
    zv_saveSeriesAction->setDisabled(zv_sampleList.isEmpty() && zv_sampleTaskList.isEmpty());
}
//======================================================
