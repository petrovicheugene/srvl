//==========================================
#ifndef ZNORMASETTINGSDATA_H
#define ZNORMASETTINGSDATA_H
//==========================================
#include "ZCalibration.h"
//==========================================
/*!
 \brief

*/
class ZNormaSettingsData
{
public:
    /*!
     \brief

    */
    ZNormaSettingsData();
    /*!
     \brief

     \param calibration
    */
    explicit ZNormaSettingsData(const ZCalibration* calibration);

    // VARS
    ZTermNormalizer::NormaType normaType; /*!< TODO: describe */
    QString customNormaString; /*!< TODO: describe */
    QList<QPair<qint64, QString> > termList; /*!< TODO: describe */


};
//==========================================
Q_DECLARE_METATYPE(ZNormaSettingsData)
#endif // ZNORMASETTINGSDATA_H
