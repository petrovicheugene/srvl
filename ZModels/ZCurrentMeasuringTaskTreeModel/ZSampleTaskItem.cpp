//=================================================================
#include "ZSampleTaskItem.h"
//=================================================================
ZSampleTaskItem::ZSampleTaskItem(const QString& name,
                                 const QString& sampleTaskName,
                                 ZSeriesTaskTreeModel *parent) : ZItem(IT_SAMPLE, parent)
{
    zv_sampleName = name;
    zv_sampleTaskName = sampleTaskName;
}
//=================================================================
QString ZSampleTaskItem::zp_displayString() const
{
    return zv_sampleTaskName;
}
//=================================================================
QString ZSampleTaskItem::zp_sampleName() const
{
    return zv_sampleName;
}
//=================================================================
bool ZSampleTaskItem::zp_setSampleName(const QString& sampleName)
{
    if(sampleName.isEmpty() || sampleName == zv_sampleName)
    {
        return false;
    }

    zv_sampleName = sampleName;
    return true;
}
//=================================================================
bool ZSampleTaskItem::zp_setSampleTaskName(const QString& sampleTaskName)
{
    if(sampleTaskName.isEmpty() || sampleTaskName == zv_sampleTaskName)
    {
        return false;
    }

    zv_sampleTaskName = sampleTaskName;
    return true;
}
//=================================================================
QString ZSampleTaskItem::zp_sampleTaskName()
{
    return zv_sampleTaskName;
}
//=================================================================
