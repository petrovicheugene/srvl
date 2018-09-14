//======================================================
#include "ZEnergyCalibrationLine.h"
//======================================================
ZEnergyCalibrationLine::ZEnergyCalibrationLine(QObject *parent)
    : QObject(parent)
{
    zv_channelNumber = 0.0;
    zv_energyValue = 0.0;
    zv_lineName = QString();
    zv_lineColor = QColor();
}
//======================================================
int ZEnergyCalibrationLine::zp_channelNumber() const
{
    return zv_channelNumber;
}
//======================================================
void ZEnergyCalibrationLine::zp_setChannelNumber(int channelNumber)
{
    if(zv_channelNumber == channelNumber)
    {
        return;
    }
    zv_channelNumber = channelNumber;
    emit zg_channelNumber(zv_channelNumber);
}
//======================================================
double ZEnergyCalibrationLine::zp_energyValue() const
{
    return zv_energyValue;
}
//======================================================
void ZEnergyCalibrationLine::zp_setEnergyValue(double energyValue)
{
    if(zv_energyValue - energyValue == 0.0)
    {
        return;
    }

    zv_energyValue = energyValue;
    emit zg_energyValue(zv_energyValue);
}
//======================================================
void ZEnergyCalibrationLine::zp_setEnergyValueString(const QString& value)
{
    bool ok;
    double dValue = value.toDouble(&ok);
    if(!ok)
    {
        return;
    }

    zp_setEnergyValue(dValue);
}
//======================================================
QString ZEnergyCalibrationLine::zp_lineName() const
{
    return zv_lineName;
}
//======================================================
void ZEnergyCalibrationLine::zp_setLineName(const QString& lineName)
{
    if(zv_lineName == lineName)
    {
        return;
    }

    zv_lineName = lineName;
    emit zg_lineName(zv_lineName);
}
//======================================================
QColor ZEnergyCalibrationLine::zp_lineColor() const
{
    return zv_lineColor;
}
//======================================================
void ZEnergyCalibrationLine::zp_setLineColor(QColor color)
{
    if(zv_lineColor == color)
    {
        return;
    }

    zv_lineColor = color;
    emit zg_lineColor(zv_lineColor);
}
//======================================================
bool ZEnergyCalibrationLine::zp_isEnabled() const
{
    return zv_enabled;
}
//======================================================
void ZEnergyCalibrationLine::zp_setEnabled(bool enabled)
{
    zv_enabled = enabled;
}
//======================================================
