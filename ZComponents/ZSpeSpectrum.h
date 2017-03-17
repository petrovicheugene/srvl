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
    explicit ZSpeSpectrum(const QList<int> &intensityList, const ZSpeAuxData& speAuxdata,
                          const QString& path, QColor color, QObject* parent);
    virtual ~ZSpeSpectrum();

    const ZAbstractSpectrumAuxData *zp_auxData() const;
    const ZSpeAuxData* zp_speAuxData() const;

    QStringList zp_isEnergyCalibrationAndExpositionSuitable(const QString&,
                                        qreal K0, qreal K1, qreal K2, int exposition);


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
