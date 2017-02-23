//=====================================================
#include "ZSample.h"
#include "ZSampleTask.h"
//=====================================================
ZSample::ZSample(const QString &sampleName,
                 ZSampleTask* sampleTask,
                 QObject *parent)
    : QObject(parent)
{
    zv_sampleName = sampleName;
    zv_sampleTask = sampleTask;
    zv_sampleTask->zp_appendClient(this);
}
//=====================================================
ZSample::~ZSample()
{
    if(zv_sampleTask != 0)
    {
        zv_sampleTask->zp_removeClient(this);
    }
}
//=====================================================
QString ZSample::zp_sampleName() const
{
    return zv_sampleName;
}
//=====================================================
bool ZSample::zp_setSampleName(const QString& sampleName)
{
    if(sampleName.isEmpty() || zv_sampleName == sampleName)
    {
        return false;
    }

    zv_sampleName = sampleName;
    return true;
}
//=====================================================
QString ZSample::zp_sampleTaskName() const
{
    if(zv_sampleTask == 0)
    {
        return QString();
    }

    return zv_sampleTask->zp_name();
}
//=====================================================
QStringList ZSample::zp_chemicalList() const
{
    if(zv_sampleTask == 0)
    {
        return QStringList();
    }

    return zv_sampleTask->zp_chemicalList();
}
//=====================================================
QStringList ZSample::zp_measuringConditionsList() const
{
    if(zv_sampleTask == 0)
    {
        return QStringList();
    }

    return zv_sampleTask->zp_measuringConditionsList();
}
//=====================================================
int ZSample::zp_sampleTaskId() const
{
    if(zv_sampleTask == 0)
    {
        return -1;
    }

    return zv_sampleTask->zp_id();
}
//=====================================================
