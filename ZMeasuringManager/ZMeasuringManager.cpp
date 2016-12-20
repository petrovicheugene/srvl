//======================================================
#include "ZMeasuringManager.h"
#include "ZGLConstantsAndDefines.h"
#include "ZControlAction.h"
#include "ZSample.h"
#include "ZSampleTask.h"
#include "ZAddSampleDialog.h"
//======================================================
ZMeasuringManager::ZMeasuringManager(QObject *parent)
    : QObject(parent)
{
    zv_currentSampleIndex = -1;

    zh_createActions();
    zh_createComponents();
    zh_createConnections();

}
//======================================================
void ZMeasuringManager::zh_createActions()
{
    zv_addSamplesToSeriesAction = new ZControlAction(this);
    zv_addSamplesToSeriesAction->setText(tr("Add samples"));

    zv_removeSamplesFronSeriesAction = new ZControlAction(this);
    zv_removeSamplesFronSeriesAction->setText(tr("Remove samples"));
}
//======================================================
void ZMeasuringManager::zh_createComponents()
{

}
//======================================================
void ZMeasuringManager::zh_createConnections()
{
    connect(zv_addSamplesToSeriesAction, &ZControlAction::triggered,
            this, &ZMeasuringManager::zh_onAddSamplesToSeriesAction);
    connect(zv_removeSamplesFronSeriesAction, &ZControlAction::triggered,
            this, &ZMeasuringManager::zh_onRemoveSamplesFromSeriesAction);

}
//======================================================
QList<ZControlAction*> ZMeasuringManager::zp_sampleActions() const
{
    QList<ZControlAction*> actionList;
    actionList.append(zv_addSamplesToSeriesAction);
    actionList.append(zv_removeSamplesFronSeriesAction);
    return actionList;

}
//======================================================
QList<ZControlAction*> ZMeasuringManager::zp_sampleContextActions() const
{
    QList<ZControlAction*> actionList;
    actionList.append(zv_addSamplesToSeriesAction);
    actionList.append(zv_removeSamplesFronSeriesAction);
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
QString ZMeasuringManager::zp_taskName(int sampleIndex) const
{
    if(sampleIndex < 0 || sampleIndex >= zv_sampleList.count())
    {
        return QString();
    }

    return zv_sampleList.at(sampleIndex)->zp_sampleTaskName();
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

    // check data
    if(sampleTaskId < 0 || defaultSampleName.isEmpty() || sampleQuantity < 1)
    {
        return;
    }

    // get sample task
    ZSampleTask* task = zh_instanceSampleTask(sampleTaskId);

    if(task == 0)
    {
        return;
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
}
//======================================================
void ZMeasuringManager::zh_onRemoveSamplesFromSeriesAction()
{
    QList<int> selectedSampleList;
    emit zg_requestSelectedSampleList(selectedSampleList);

    qSort(selectedSampleList);

    int sampleIndex;
    for(int i =  selectedSampleList.count() - 1; i >= 0; i--)
    {
        sampleIndex = selectedSampleList.at(i);
        emit zg_sampleOperation(SOT_SAMPLE_ABOUT_TO_BE_REMOVED, sampleIndex, sampleIndex);
        ZSample* sample = zv_sampleList.takeAt(sampleIndex);
        delete sample;
        emit zg_sampleOperation(SOT_SAMPLE_REMOVED, sampleIndex, sampleIndex);
    }
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
