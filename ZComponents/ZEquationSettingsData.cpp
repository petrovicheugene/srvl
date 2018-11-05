//========================================================
#include "ZEquationSettingsData.h"
//========================================================
ZEquationSettingsData::ZEquationSettingsData()
{
    equationType = ZCalibration::ET_NOT_DEFINED;
    baseTermId = -1;
}
//========================================================
ZEquationSettingsData::ZEquationSettingsData(const ZCalibration *calibration)
{
    equationType = calibration->zp_equationType();
    baseTermId = calibration->zp_baseTermId();
    fractionalBaseNormaSettingsData.normaType = calibration->zp_baseTermNormaType();
    fractionalBaseNormaSettingsData.customNormaString = calibration->zp_baseTermNormaCustomString();

    QPair<qint64, QString> termData;
    for(int t = 0; t < calibration->zp_termCount(); t++)
    {
        termData.first = calibration->zp_termId(t);
        termData.second = calibration->zp_termName(t);
        termList.append(termData);
    }
}
//========================================================
