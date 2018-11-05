//========================================================
#ifndef ZEQUATIONSETTINGSDATA_H
#define ZEQUATIONSETTINGSDATA_H
//========================================================
#include "ZCalibration.h"
#include "ZNormaSettingsData.h"
//========================================================
class ZEquationSettingsData
{
public:
    ZEquationSettingsData();
    explicit ZEquationSettingsData(const ZCalibration* calibration);

    // VARS

    ZCalibration::EquationType equationType;
    qint64 baseTermId;
    QList<QPair<qint64, QString> > termList;
    ZNormaSettingsData fractionalBaseNormaSettingsData;
};
Q_DECLARE_METATYPE(ZEquationSettingsData)
//========================================================
#endif // ZEQUATIONSETTINGSDATA_H
