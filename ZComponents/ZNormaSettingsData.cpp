//==========================================
#include "ZNormaSettingsData.h"
//==========================================
ZNormaSettingsData::ZNormaSettingsData()
{
    normaType = ZTermNormalizer::NT_NONE;
}
//==========================================
ZNormaSettingsData::ZNormaSettingsData(const ZCalibration *calibration)
{
    normaType = calibration->zp_normaType();
    customNormaString = calibration->zp_normaCustomString();

    QPair<qint64, QString> termData;
    for(int t = 0; t < calibration->zp_termCount(); t++)
    {
        termData.first = calibration->zp_termId(t);
        termData.second = calibration->zp_termName(t);
        termList.append(termData);
    }
}
//========================================================
