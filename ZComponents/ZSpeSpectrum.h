//===================================================
#ifndef ZSPESPECTRUM_H
#define ZSPESPECTRUM_H
//===================================================
#include <QStringList>
#include "ZAbstractSpectrum.h"
#include "ZSpeAuxData.h"
//===================================================
class ZSpeSpectrum : public ZAbstractSpectrum
{
    Q_OBJECT
public:

    // explicit ZSpeSpectrum(QObject* parent);
    explicit ZSpeSpectrum(const QList<quint32> &intensityList, const ZSpeAuxData& speAuxdata,
                          const QString& path, QColor color, bool completed, QObject* parent);

    virtual ~ZSpeSpectrum();

    const ZAbstractSpectrumAuxData *zp_auxData() const;
    const ZSpeAuxData* zp_speAuxData() const;

    QStringList zp_isEnergyCalibrationAndExpositionSuitable(const QString&,
                                        qreal K0, qreal K1, qreal K2, int exposition);


    void zp_setSpectrumDateTime(QDateTime datetime);
    void zp_setExposition(int exposition);
    void zp_setGainFactor(quint8 gainFactor);
    quint8 zp_gainFactor() const;
    void zp_setAliveTime(quint32 aliveTime);
    void zp_setEnergyUnit(const QString& energyUnit);
    void zp_setEnergyCalibration(const QList<double> &energyCalibrationFactorList);
    QList<double> zp_energyCalibration() const;

signals:

    void zg_energyCalibrationChanged() const;
    void zg_gainFactorChanged() const;

private:

    // VARS
    ZSpeAuxData zv_auxData;

    // int zv_deviceNumber;
    // QDate zv_date;
    // QTime zv_time;
    // int zv_exposition;
    // int zv_aliveTime;
    // QStringList zv_comments;
    // QString zv_energyUnit;
    // QList<double> zv_energyFactorList;
    // int zv_peakWidht;

};
//===================================================
#endif // ZSPESPECTRUM_H
