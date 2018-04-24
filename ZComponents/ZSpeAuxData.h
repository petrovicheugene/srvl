//=================================================
#ifndef ZSPEAUXDATA_H
#define ZSPEAUXDATA_H
//=================================================
#include "ZAbstractSpectrumAuxData.h"
#include <QDateTime>
#include <QString>
//=================================================
class ZSpeAuxData : public ZAbstractSpectrumAuxData
{
public:
    ZSpeAuxData();

    bool zp_setDate(const QString&);
    bool zp_setTime(const QString&);
    void zp_setDate(const QDate&);
    void zp_setTime(const QTime&);
    void zp_setDateTime(const QDateTime&);

    bool zp_setExposition(const QString&);
    void zp_setExposition(int exposition);

    bool zp_setAliveTime(const QString&);
    void zp_setAliveTime(quint32 aliveTime);

    bool zp_setGainFactor(const QString&);
    void zp_setGainFactor(quint8 gainFactor);

    bool zp_setEnergyUnit(const QString&);

    bool zp_setEnergyK0(const QString&);
    void zp_setEnergyK0(double K0);

    bool zp_setEnergyK1(const QString&);
    void zp_setEnergyK1(double K1);

    bool zp_setEnergyK2(const QString&);
    void zp_setEnergyK2(double K2);

    bool zp_setPeakWidth(const QString&);
    bool zp_setPeakWidthK0(const QString&);
    bool zp_setPeakWidthK1(const QString&);
    bool zp_setPeakWidthK2(const QString&);

    QDateTime zp_dateTime() const;
    QDate zp_date() const;
    QTime zp_time() const;
    int zp_exposition() const;
    quint8 zp_gainFactor() const;
    int zp_aliveTime() const;
    QString zp_energyUnit() const;
    qreal zp_energyK0() const;
    qreal zp_energyK1() const;
    qreal zp_energyK2() const;

    int zp_peakWidth() const;
    qreal zp_peakWidthK0() const;
    qreal zp_peakWidthK1() const;
    qreal zp_peakWidthK2() const;

private:

    // VARS
    QDate zv_date;
    QTime zv_time;
    int zv_exposition;
    quint32 zv_aliveTime;
    quint8 zv_gainFactor;

    QString zv_energyUnit;
    qreal zv_energyK0;
    qreal zv_energyK1;
    qreal zv_energyK2;

    int zv_peakWidth;
    qreal zv_peakWidthK0;
    qreal zv_peakWidthK1;
    qreal zv_peakWidthK2;


};
//=================================================
#endif // ZSPEAUXDATA_H
