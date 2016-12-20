//==========================================
#ifndef ZNORMASETTINGSDATA_H
#define ZNORMASETTINGSDATA_H
//==========================================
#include "ZCalibration.h"
//==========================================
class ZNormaSettingsData
{
public:
    ZNormaSettingsData();
    explicit ZNormaSettingsData(const ZCalibration* calibration);

    // VARS
    ZTermNormalizer::NormaType normaType;
    QString customNormaString;
    QList<QPair<qint64, QString> > termList;


};
//==========================================
Q_DECLARE_METATYPE(ZNormaSettingsData)
#endif // ZNORMASETTINGSDATA_H
