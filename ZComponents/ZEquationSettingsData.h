//========================================================
#ifndef ZEQUATIONSETTINGSDATA_H
#define ZEQUATIONSETTINGSDATA_H
//========================================================
#include "ZCalibration.h"
#include "ZNormaSettingsData.h"
//========================================================
/*!
 \brief

*/
class ZEquationSettingsData
{
public:
    /*!
     \brief

    */
    ZEquationSettingsData();
    /*!
     \brief

     \param calibration
    */
    explicit ZEquationSettingsData(const ZCalibration* calibration);

    // VARS

    ZCalibration::EquationType equationType; /*!< TODO: describe */
    qint64 baseTermId; /*!< TODO: describe */
    QList<QPair<qint64, QString> > termList; /*!< TODO: describe */
    ZNormaSettingsData fractionalBaseNormaSettingsData; /*!< TODO: describe */
};
Q_DECLARE_METATYPE(ZEquationSettingsData)
//========================================================
#endif // ZEQUATIONSETTINGSDATA_H
