//===================================================
#include "ZSpeSpectrum.h"
//===================================================
//ZSpeSpectrum::ZSpeSpectrum(QObject *parent) : ZAbstractSpectrum(parent)
//{
//    zv_type = ST_SPE;
//}
//===================================================
ZSpeSpectrum::ZSpeSpectrum(const QList<quint32>& intensityList, const ZSpeAuxData& speAuxData,
                           const QString& name, QColor color, bool completed, QObject *parent)
    : ZAbstractSpectrum(intensityList, name, color, completed, parent)
{
    zv_auxData = speAuxData;
}
//===================================================
ZSpeSpectrum::~ZSpeSpectrum()
{

}
//===================================================
const ZAbstractSpectrumAuxData* ZSpeSpectrum::zp_auxData() const
{
    return &zv_auxData;
}
//===================================================
const ZSpeAuxData* ZSpeSpectrum::zp_speAuxData() const
{
    return &zv_auxData;
}
//===================================================
QStringList ZSpeSpectrum::zp_isEnergyCalibrationAndExpositionSuitable(const QString& energyUnit,
                                                                      qreal K0, qreal K1, qreal K2, int exposition)
{
    QStringList inconsistenciesList;

    if(zv_auxData.zp_energyUnit() != energyUnit)
    {
        inconsistenciesList << tr("energy unit");
    }

    if(zv_auxData.zp_energyK0() - K0 == 0.0)
    {
        inconsistenciesList << tr("energy factor K0");
    }

    if(zv_auxData.zp_energyK1() - K1 != 0.0)
    {
        inconsistenciesList << tr("energy factor K1");
    }

    if(zv_auxData.zp_energyK2() - K2 != 0.0)
    {
        inconsistenciesList << tr("energy factor K2");
    }

    if(zv_auxData.zp_exposition() != exposition)
    {
        inconsistenciesList << tr("exposition");
    }

    return inconsistenciesList;
}
//===================================================
void ZSpeSpectrum::zp_setSpectrumDateTime(QDateTime datetime)
{
    zv_auxData.zp_setDateTime(datetime);
}
//===================================================
void ZSpeSpectrum::zp_setExposition(int exposition)
{
    zv_auxData.zp_setExposition(exposition);
}
//===================================================
void ZSpeSpectrum::zp_setGainFactor(quint8 gainFactor)
{
    zv_auxData.zp_setGainFactor(gainFactor);
    emit zg_gainFactorChanged();
}
//===================================================
quint8 ZSpeSpectrum::zp_gainFactor() const
{
    return zv_auxData.zp_gainFactor();
}
//===================================================
void ZSpeSpectrum::zp_setAliveTime(quint32 aliveTime)
{
    zv_auxData.zp_setAliveTime(aliveTime);
}
//===================================================
void ZSpeSpectrum::zp_setEnergyUnit(const QString& energyUnit)
{
    zv_auxData.zp_setEnergyUnit(energyUnit);
}
//===================================================
void ZSpeSpectrum::zp_setEnergyCalibration(const QList<double>& energyCalibrationFactorList)
{
    zv_auxData.zp_setEnergyK0(energyCalibrationFactorList.value(0, 0.0));
    zv_auxData.zp_setEnergyK1(energyCalibrationFactorList.value(1, 0.0));
    zv_auxData.zp_setEnergyK2(energyCalibrationFactorList.value(2, 0.0));
    emit zg_energyCalibrationChanged();
}
//===================================================
QList<double> ZSpeSpectrum::zp_energyCalibration() const
{
    QList<double> energyCalibrationFactorList;
    energyCalibrationFactorList.append(zv_auxData.zp_energyK0());
    energyCalibrationFactorList.append(zv_auxData.zp_energyK1());
    energyCalibrationFactorList.append(zv_auxData.zp_energyK2());
    return energyCalibrationFactorList;
}
//===================================================
