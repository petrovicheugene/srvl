//======================================================
#include "ZMeasurementManager.h"

// components
#include "ZControlAction.h"
#include "ZSample.h"

// dialogs
#include "ZAddSampleDialog.h"


//======================================================
ZMeasurementManager::ZMeasurementManager(QObject *parent)
    : QObject(parent)
{
    zv_currentSampleIndex = -1;

    zh_createActions();
    zh_createComponents();
    zh_createConnections();

}
//======================================================
void ZMeasurementManager::zh_createActions()
{
    zv_addSamplesToSeriesAction = new ZControlAction(this);
    zv_addSamplesToSeriesAction->setText(tr("Add samples"));

    zv_removeSamplesFronSeriesAction = new ZControlAction(this);
    zv_removeSamplesFronSeriesAction->setText(tr("Add samples"));
}
//======================================================
void ZMeasurementManager::zh_createComponents()
{

}
//======================================================
void ZMeasurementManager::zh_createConnections()
{
    connect(zv_addSamplesToSeriesAction, &ZControlAction::triggered,
            this, &ZMeasurementManager::zh_onAddSamplesToSeriesAction);
    connect(zv_removeSamplesFronSeriesAction, &ZControlAction::triggered,
            this, &ZMeasurementManager::zh_onRemoveSamplesFromSeriesAction);

}
//======================================================
QList<ZControlAction*> ZMeasurementManager::zp_sampleActions() const
{
    QList<ZControlAction*> actionList;
    actionList.append(zv_addSamplesToSeriesAction);
    actionList.append(zv_removeSamplesFronSeriesAction);
    return actionList;

}
//======================================================
QList<ZControlAction*> ZMeasurementManager::zp_sampleContextActions() const
{
    QList<ZControlAction*> actionList;
    actionList.append(zv_addSamplesToSeriesAction);
    actionList.append(zv_removeSamplesFronSeriesAction);
    return actionList;
}
//======================================================
int ZMeasurementManager::zp_sampleCount() const
{
    return zv_sampleList.count();
}
//======================================================
QString ZMeasurementManager::zp_sampleName(int sampleIndex) const
{
    if(sampleIndex < 0 || sampleIndex >= zv_sampleList.count())
    {
        return QString();
    }

    return zv_sampleList.at(sampleIndex)->zp_sampleName();
}
//======================================================
bool ZMeasurementManager::zp_setSampleName(int sampleIndex, const QString& name)
{
    if(sampleIndex < 0 || sampleIndex >= zv_sampleList.count())
    {
        return false;
    }

    return zv_sampleList.at(sampleIndex)->zp_setSampleName(name);
}
//======================================================
void ZMeasurementManager::zg_setCurrentSampleIndex(int currentSampleIndex)
{
    if(currentSampleIndex >= zv_sampleList.count())
    {
        zv_currentSampleIndex = currentSampleIndex;
    }
}
//======================================================
void ZMeasurementManager::zh_onAddSamplesToSeriesAction()
{
    ZAddSampleDialog dialog;
    if(!dialog.exec())
    {
        return;
    }

    QString defaultSampleName = dialog.zp_sampleName();
    int sampleQuantity = dialog.zp_sampleQuantity();
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
                serialNumberString = QString::number(lastSampleSerialNumber);
            }
            newSampleName = defaultSampleName + serialNumberString;
            if(zh_checkSampleName(newSampleName))
            {
               break;
            }
        }

        zh_appendSampleToList(newSampleName);
    }
}
//======================================================
void ZMeasurementManager::zh_onRemoveSamplesFromSeriesAction()
{

}
//======================================================
void ZMeasurementManager::zh_appendSampleToList(const QString& sampleName)
{
    ZSample* sample = new ZSample(sampleName, this);
    zv_sampleList.append(sample);
}
//======================================================
int ZMeasurementManager::zh_findLastSampleSerialNumber(const QString &sampleName) const
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
bool ZMeasurementManager::zh_checkSampleName(const QString& sampleName) const
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
