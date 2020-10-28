//=================================================
#include "ZSpeAuxData.h"
//=================================================
ZSpeAuxData::ZSpeAuxData()
    : ZAbstractSpectrumAuxData(DT_SPE)
{
    zv_date = QDate();
    zv_time = QTime();
    zv_exposition = -1;
    zv_aliveTime = -1;
    zv_energyUnit = QString("kEv");
    zv_energyK0 = 0;
    zv_energyK1 = 0;
    zv_energyK2 = 0;

    zv_peakWidth = 0;
    zv_peakWidthK0 = 0;
    zv_peakWidthK1 = 0;
    zv_peakWidthK2 = 0;
    zv_srvN = "0";
}
//=================================================
void ZSpeAuxData::zp_setSrvN(const QString& srvN)
{
    if (srvN.isEmpty())
    {
        zv_srvN = "0";
    }
    else
    {
        zv_srvN = srvN;
    }
}
//=================================================
bool ZSpeAuxData::zp_setDate(const QString& string)
{
    if(string.isEmpty())
    {
        return false;
    }

    zv_date = QDate::fromString(string, "dd.MM.yyyy");
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
void ZSpeAuxData::zp_setDate(const QDate& date)
{
    zv_date = date;
}
//=================================================
void ZSpeAuxData::zp_setTime(const QTime& time)
{
    zv_time = time;
}
//=================================================
void ZSpeAuxData::zp_setDateTime(const QDateTime& dateTime)
{
    zv_date = dateTime.date();
    zv_time = dateTime.time();
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
void ZSpeAuxData::zp_setExposition(int exposition)
{
    zv_exposition = exposition;
}
//=================================================
bool ZSpeAuxData::zp_setAliveTime(const QString& string)
{
    if(string.isEmpty())
    {
        return false;
    }
    bool ok;
    zv_aliveTime = static_cast<quint32>(string.toInt(&ok));
    return ok;
}
//=================================================
void ZSpeAuxData::zp_setAliveTime(quint32 aliveTime)
{
    zv_aliveTime = aliveTime;
}
//=================================================
bool ZSpeAuxData::zp_setGainFactor(const QString& gainFactor)
{
    if(gainFactor.isEmpty())
    {
        return false;
    }
    bool ok;
    zv_gainFactor = static_cast<quint8>(gainFactor.toUInt(&ok));
    return ok;
}
//=================================================
void ZSpeAuxData::zp_setGainFactor(quint8 gainFactor)
{
    zv_gainFactor = gainFactor;
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
void ZSpeAuxData::zp_setEnergyK0(double K0)
{
    zv_energyK0 = K0;
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
void ZSpeAuxData::zp_setEnergyK1(double K1)
{
    zv_energyK1 = K1;
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
void ZSpeAuxData::zp_setEnergyK2(double K2)
{
    zv_energyK2 = K2;
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
void ZSpeAuxData::zp_setComment(int lineNumber, const QString& comment)
{
    zv_commentMap[lineNumber] = comment;
}
//=================================================
QString ZSpeAuxData::zp_comment(int lineNumber) const
{
    return zv_commentMap.value(lineNumber, QString("0"));
}
//=================================================
QString ZSpeAuxData::zp_srvN() const
{
    return zv_srvN;
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
quint8 ZSpeAuxData::zp_gainFactor() const
{
    return zv_gainFactor;
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
void ZSpeAuxData::zp_setEnergyCalibrationFactors(QList<double> energyCalibrationFactors )
{
    zv_energyK0 = energyCalibrationFactors.value(0, 0.0);
    zv_energyK1 = energyCalibrationFactors.value(1, 0.0);
    zv_energyK2 = energyCalibrationFactors.value(2, 0.0);

}
//=================================================
