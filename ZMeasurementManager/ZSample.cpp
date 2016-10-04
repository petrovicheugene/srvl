//=====================================================
#include "ZSample.h"
//=====================================================
ZSample::ZSample(const QString &sampleName,
                 QObject *parent)
    : QObject(parent)
{
    zv_sampleName = sampleName;
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
