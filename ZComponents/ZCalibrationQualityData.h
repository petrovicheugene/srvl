//============================================
#ifndef ZCALIBRATIONQUALITYDATA_H
#define ZCALIBRATIONQUALITYDATA_H
//============================================
#include <QString>
//============================================
/*!
 \brief

*/
class ZCalibrationQualityData
{
public:
    /*!
     \brief

    */
    ZCalibrationQualityData();

    QString determination; /*!< TODO: describe */
    QString adj_determination; /*!< TODO: describe */
    QString squareSigma; /*!< TODO: describe */
    QString standardDeviation; /*!< TODO: describe */

};
//============================================
#endif // ZCALIBRATIONQUALITYDATA_H
