//============================================
#ifndef ZCALIBRATIONQUALITYDATA_H
#define ZCALIBRATIONQUALITYDATA_H
//============================================
#include <QString>
//============================================
class ZCalibrationQualityData
{
public:
    ZCalibrationQualityData();

    QString determination;
    QString adj_determination;
    QString squareSigma;
    QString standardDeviation;

};
//============================================
#endif // ZCALIBRATIONQUALITYDATA_H
