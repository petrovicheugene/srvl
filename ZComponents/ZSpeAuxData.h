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
    bool zp_setExposition(const QString&);
    bool zp_setAliveTime(const QString&);
    bool zp_setEnergyUnit(const QString&);
    bool zp_setEnergyK0(const QString&);
    bool zp_setEnergyK1(const QString&);
    bool zp_setEnergyK2(const QString&);

    bool zp_setPeakWidth(const QString&);
    bool zp_setPeakWidthK0(const QString&);
    bool zp_setPeakWidthK1(const QString&);
    bool zp_setPeakWidthK2(const QString&);

    QDateTime zp_dateTime() const;
    QDate zp_date() const;
    QTime zp_time() const;
    int zp_exposition() const;
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
    QDateTime zv_dateTime;
    QDate zv_date;
    QTime zv_time;
    int zv_exposition;
    int zv_aliveTime;

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
