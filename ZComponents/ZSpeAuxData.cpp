//=================================================
#include "ZSpeAuxData.h"
//=================================================
ZSpeAuxData::ZSpeAuxData()
    : ZAbstractSpectrumAuxData(DT_SPE)
{
    zv_dateTime = QDateTime();
    zv_date = QDate();
    zv_time = QTime();
    zv_exposition = -1;
    zv_aliveTime = -1;
    zv_energyUnit = QString();
    zv_energyK0 = 0;
    zv_energyK1 = 0;
    zv_energyK2 = 0;

    zv_peakWidth = 0;
    zv_peakWidthK0 = 0;
    zv_peakWidthK1 = 0;
    zv_peakWidthK2 = 0;

}
//=================================================
bool ZSpeAuxData::zp_setDate(const QString& string)
{
    if(string.isEmpty())
    {
        return false;
    }

    zv_date = QDate::fromString(string);
    return zv_date.isValid();
}
//=================================================
bool ZSpeAuxData::zp_setTime(const QString& string)
{
    if(string.isEmpty())
    {
        return false;
    }

    zv_time = QTime::fromString(string);
    return zv_time.isValid();
}
//=================================================
bool ZSpeAuxData::zp_setExposition(const QString& string)
{
    if(string.isEmpty())
    {
        return false;
    }
    bool ok;
    zv_exposition = string.toInt(&ok);
    return ok;
}
//=================================================
bool ZSpeAuxData::zp_setAliveTime(const QString& string)
{
    if(string.isEmpty())
    {
        return false;
    }
    bool ok;
    zv_aliveTime = string.toInt(&ok);
    return ok;
}
//=================================================
bool ZSpeAuxData::zp_setEnergyUnit(const QString& string)
{
    if(string.isEmpty())
    {
        return false;
    }
    if(string == "1")
    {
        zv_energyUnit = "kEv";
        return true;
    }
    else if(string == "2")
    {
        zv_energyUnit = "MEv";
        return true;
    }
    else
    {
        zv_energyUnit = QString();
        return false;
    }
}
//=================================================
bool ZSpeAuxData::zp_setEnergyK0(const QString& string)
{
    if(string.isEmpty())
    {
        return false;
    }
    bool ok;
    zv_energyK0 = string.toDouble(&ok);
    return ok;
}
//=================================================
bool ZSpeAuxData::zp_setEnergyK1(const QString& string)
{
    if(string.isEmpty())
    {
        return false;
    }
    bool ok;
    zv_energyK1 = string.toDouble(&ok);
    return ok;
}
//=================================================
bool ZSpeAuxData::zp_setEnergyK2(const QString& string)
{
    if(string.isEmpty())
    {
        return false;
    }
    bool ok;
    zv_energyK2 = string.toDouble(&ok);
    return ok;
}
//=================================================
bool ZSpeAuxData::zp_setPeakWidth(const QString& string)
{
    if(string.isEmpty())
    {
        return false;
    }
    bool ok;
    zv_peakWidth = string.toInt(&ok);
    return ok;
}
//=================================================
bool ZSpeAuxData::zp_setPeakWidthK0(const QString& string)
{
    if(string.isEmpty())
    {
        return false;
    }
    bool ok;
    zv_peakWidthK0 = string.toDouble(&ok);
    return ok;
}
//=================================================
bool ZSpeAuxData::zp_setPeakWidthK1(const QString& string)
{
    if(string.isEmpty())
    {
        return false;
    }
    bool ok;
    zv_peakWidthK1 = string.toDouble(&ok);
    return ok;
}
//=================================================
bool ZSpeAuxData::zp_setPeakWidthK2(const QString& string)
{
    if(string.isEmpty())
    {
        return false;
    }
    bool ok;
    zv_peakWidthK2 = string.toDouble(&ok);
    return ok;
}
//=================================================
QDateTime ZSpeAuxData::zp_dateTime() const
{
    QDateTime dateTime = QDateTime(zv_date, zv_time);
    return dateTime;
}
//=================================================
QDate ZSpeAuxData::zp_date() const
{
    return zv_date;
}
//=================================================
QTime ZSpeAuxData::zp_time() const
{
    return zv_time;
}
//=================================================
int ZSpeAuxData::zp_exposition() const
{
    return zv_exposition;
}
//=================================================
int ZSpeAuxData::zp_aliveTime() const
{
    return zv_aliveTime;
}
//=================================================
QString ZSpeAuxData::zp_energyUnit() const
{
    return zv_energyUnit;
}
//=================================================
qreal ZSpeAuxData::zp_energyK0() const
{
    return zv_energyK0;
}
//=================================================
qreal ZSpeAuxData::zp_energyK1() const
{
    return zv_energyK1;
}
//=================================================
qreal ZSpeAuxData::zp_energyK2() const
{
    return zv_energyK2;
}
//=================================================
int ZSpeAuxData::zp_peakWidth() const
{
    return zv_peakWidth;
}
//=================================================
qreal ZSpeAuxData::zp_peakWidthK0() const
{
    return zv_peakWidthK0;
}
//=================================================
qreal ZSpeAuxData::zp_peakWidthK1() const
{
    return zv_peakWidthK1;
}
//=================================================
qreal ZSpeAuxData::zp_peakWidthK2() const
{
    return zv_peakWidthK2;
}
//=================================================