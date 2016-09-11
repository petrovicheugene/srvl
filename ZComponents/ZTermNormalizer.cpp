//======================================================================
#include "ZTermNormalizer.h"
#include "ZAbstractSpectrum.h"
#include "ZCalibration.h"
//======================================================================
// STATIC
//======================================================================
QMap<ZTermNormalizer::NormaType, QString> ZTermNormalizer::zv_normaTypeStringMap =
        ZTermNormalizer::zh_initNormaTypeStringMap();
//======================================================================
QStringList ZTermNormalizer::zp_normaTypeList()
{
    return zv_normaTypeStringMap.values();
}
//======================================================================
QString ZTermNormalizer::zp_normaTypeString(ZTermNormalizer::NormaType type)
{
    return zv_normaTypeStringMap.value(type);
}
//======================================================================
ZTermNormalizer::NormaType ZTermNormalizer::zp_normaTypeForString(const QString& string)
{
    if(!zv_normaTypeStringMap.values().contains(string))
    {
        return NT_NONE;
    }
    return zv_normaTypeStringMap.key(string);
}
//======================================================================
QMap<ZTermNormalizer::NormaType, QString> ZTermNormalizer::zh_initNormaTypeStringMap()
{
    QMap<ZTermNormalizer::NormaType, QString> map;
    map.insert(NT_NONE, tr("None"));
    map.insert(NT_COHERENT, tr("Coherent"));
    map.insert(NT_INCOHERENT, tr("Incoherent"));
    map.insert(NT_COHERENT_INCOHERENT, tr("Coherent / Incoherent"));
    map.insert(NT_INCOHERENT_COHERENT, tr("Incoherent / Coherent"));
    map.insert(NT_CUSTOM, tr("Custom"));
    return map;
}
// END STATIC
//======================================================================
//======================================================================
ZTermNormalizer::ZTermNormalizer(ZCalibration *parent) : QObject(parent)
{
    zv_calibration = parent;
    zv_normaType = NT_NONE;
    zv_customNormaString = QString();
    zh_createConnections();

}
//======================================================================
void ZTermNormalizer::zp_connectToWindow(ZCalibrationWindow* window)
{
    connect(window, &ZCalibrationWindow::zg_windowTypeChanged,
            this, &ZTermNormalizer::zh_onWindowTypeChange);
    connect(window, &ZCalibrationWindow::destroyed,
            this, &ZTermNormalizer::zh_onWindowDestroy);
    connect(window, &ZCalibrationWindow::zg_windowMarginsChanged,
            this, &ZTermNormalizer::zh_onWindowMarginsChange);

}
//======================================================================
//bool ZTermNormalizer::zp_isValid() const
//{
//    return false;
//}
////======================================================================
//qreal ZTermNormalizer::zp_value() const
//{
//    return 0;
//}
//======================================================================
ZTermNormalizer::NormaType ZTermNormalizer::zp_normaType() const
{
    return zv_normaType;
}
//======================================================================
bool ZTermNormalizer::zp_setNormaType(ZTermNormalizer::NormaType type)
{
    if(type == zv_normaType)
    {
        return false;
    }

    zv_normaType = type;
    emit zg_normalizerChanged();
    return true;
}
//======================================================================
bool ZTermNormalizer::zp_setCustomNormaString(const QString& customString)
{
    zv_customNormaString = customString;
    emit zg_normalizerChanged();
    return true;
}
//======================================================================
QString ZTermNormalizer::zp_normaCustomString() const
{
    return zv_customNormaString;
}
//======================================================================
bool ZTermNormalizer::zp_setNormalizerParameters(ZTermNormalizer::NormaType type,
                              const QString& customString)
{
    bool res = false;
    if(type != zv_normaType)
    {
        zv_normaType = type;
        res = true;
    }

    if(zv_customNormaString != customString)
    {
        zv_customNormaString = customString;
        res = true;
    }

    if(res)
    {
        emit zg_normalizerChanged();
    }

    return res;
}
//======================================================================
bool ZTermNormalizer::zp_normalizeValue(const ZAbstractSpectrum* spectrum, qreal& value ) const
{
    if(zv_normaType == NT_NONE)
    {
        return true;
    }

    qreal normaValue;
    if(!zh_calcNormaValue(spectrum, normaValue))
    {
        return false;
    }

    if(normaValue == 0)
    {
        return false;
    }

    value /= normaValue;
    return true;
}
//======================================================================
bool ZTermNormalizer::zp_normalizeValue(qreal &value) const
{
    if(zv_normaType == NT_NONE)
    {
        return true;
    }

    if(zv_normaValueBuffer == 0)
    {
        return false;
    }

    value /= zv_normaValueBuffer;
    return true;
}
//======================================================================
bool ZTermNormalizer::zp_calcAndSetNormaValue(const ZAbstractSpectrum *spectrum)
{
    zp_resetNormaValue();
    qreal normaValue;
    if(!zh_calcNormaValue(spectrum, normaValue))
    {
        return false;
    }

    zv_spectrumIdBuffer = spectrum->zp_spectrumId();
    zv_normaValueBuffer = normaValue;

    return true;
}
//======================================================================
bool ZTermNormalizer::zh_calcNormaValue(const ZAbstractSpectrum *spectrum, qreal& normaValue) const
{
    normaValue = 0;
    if(!spectrum)
    {
        return false;
    }

    if(zv_normaType == NT_NONE)
    {
        return true;
    }

    if(zv_normaType == NT_COHERENT)
    {
        qint64 windowIntensity = 0;
        if(!zh_getWindowsValue(ZCalibrationWindow::WT_COHERENT, spectrum, windowIntensity))
        {
            return false;
        }
        normaValue = static_cast<qreal>(windowIntensity);
    }
    else  if(zv_normaType == NT_INCOHERENT)
    {
        qint64 windowIntensity = 0;
        if(!zh_getWindowsValue(ZCalibrationWindow::WT_INCOHERENT, spectrum, windowIntensity))
        {
            return false;
        }
        normaValue = static_cast<qreal>(windowIntensity);
    }
    else if(zv_normaType == NT_COHERENT_INCOHERENT)
    {
        qint64 coherentValue = 0;
        qint64 incoherentValue = 0;
        if(!zh_getWindowsValue(ZCalibrationWindow::WT_INCOHERENT, spectrum, incoherentValue))
        {
            return false;
        }

        if(!zh_getWindowsValue(ZCalibrationWindow::WT_COHERENT, spectrum, coherentValue))
        {
            return false;
        }

        if(incoherentValue == 0)
        {
            return false;
        }

        normaValue = static_cast<qreal>(coherentValue) / static_cast<qreal>(incoherentValue);
    }
    else if(zv_normaType == NT_INCOHERENT_COHERENT)
    {
        qint64 coherentValue = 0;
        qint64 incoherentValue = 0;
        if(!zh_getWindowsValue(ZCalibrationWindow::WT_INCOHERENT, spectrum, incoherentValue))
        {
            return false;
        }

        if(!zh_getWindowsValue(ZCalibrationWindow::WT_COHERENT, spectrum, coherentValue))
        {
            return false;
        }

        if(coherentValue == 0)
        {
            return false;
        }

        normaValue = static_cast<qreal>(incoherentValue) / static_cast<qreal>(coherentValue);
    }
    else if(zv_normaType == NT_CUSTOM)
    {
        // TODO custom normalization
        normaValue = 1;
    }

    return true;
}
//======================================================================
void ZTermNormalizer::zp_resetNormaValue()
{
    zv_spectrumIdBuffer = -1;
    zv_normaValueBuffer = 0;
}
//======================================================================
void ZTermNormalizer::zh_createConnections()
{
    connect(this, &ZTermNormalizer::zg_normalizerChanged,
            zv_calibration, &ZCalibration::zh_onNormalizerChange);
}
//======================================================================
bool ZTermNormalizer::zh_getWindowsValue(ZCalibrationWindow::WindowType type,
                                         const ZAbstractSpectrum* spectrum, qint64& value) const
{
    qreal currentValue;
    bool ok;
    value = 0;
    for(int w = 0; w < zv_calibration->zv_windowList.count(); w++)
    {
        if(zv_calibration->zv_windowList.at(w)->zp_windowType() != type)
        {
            continue;
        }

        zv_calibration->zv_windowList.at(w)->zp_calcWindowIntensity(spectrum, currentValue, true, &ok);
        if(!ok)
        {
            return false;
        }
        value += currentValue;
    }
    return true;
}
//======================================================================
void ZTermNormalizer::zh_onWindowTypeChange(ZCalibrationWindow::WindowType previousType,
                                            ZCalibrationWindow::WindowType currentType) const
{
    if(zv_normaType == NT_NONE)
    {
        return;
    }
    else if(zv_normaType == NT_CUSTOM)
    {
        // TODO check out is destroyed window parted in custom norma
        emit zg_normalizerChanged();
        return;
    }

    if((previousType == ZCalibrationWindow::WT_COHERENT || currentType == ZCalibrationWindow::WT_COHERENT) &&
            (zv_normaType == NT_COHERENT ||  zv_normaType == NT_COHERENT_INCOHERENT
             || zv_normaType == NT_INCOHERENT_COHERENT))
    {
        emit zg_normalizerChanged();
    }
    else if((previousType == ZCalibrationWindow::WT_INCOHERENT || currentType == ZCalibrationWindow::WT_INCOHERENT) &&
            (zv_normaType == NT_INCOHERENT ||  zv_normaType == NT_COHERENT_INCOHERENT
             || zv_normaType == NT_INCOHERENT_COHERENT))
    {
        emit zg_normalizerChanged();
    }
}
//======================================================================
void ZTermNormalizer::zh_onWindowDestroy(QObject* obj) const
{
    if(zv_normaType == NT_NONE)
    {
        return;
    }
    else if(zv_normaType == NT_CUSTOM)
    {
        // TODO check out is destroyed window parted in custom norma
        emit zg_normalizerChanged();
        return;
    }

    ZCalibrationWindow* window = qobject_cast<ZCalibrationWindow*>(obj);
    if(!window)
    {
        emit zg_normalizerChanged();
        return;
    }

    ZCalibrationWindow::WindowType windowType = window->zp_windowType();
    if(windowType == ZCalibrationWindow::WT_COHERENT &&
            (zv_normaType == NT_COHERENT ||  zv_normaType == NT_COHERENT_INCOHERENT
             || zv_normaType == NT_INCOHERENT_COHERENT))
    {
        emit zg_normalizerChanged();
    }
    else if(windowType == ZCalibrationWindow::WT_INCOHERENT &&
            (zv_normaType == NT_INCOHERENT ||  zv_normaType == NT_COHERENT_INCOHERENT
             || zv_normaType == NT_INCOHERENT_COHERENT))
    {
        emit zg_normalizerChanged();
    }
}
//======================================================================
void ZTermNormalizer::zh_onWindowMarginsChange() const
{
    ZCalibrationWindow* window = qobject_cast<ZCalibrationWindow*>(sender());

    if(window->zp_windowType() == ZCalibrationWindow::WT_INCOHERENT &&
            (zv_normaType == NT_INCOHERENT ||  zv_normaType == NT_COHERENT_INCOHERENT
             || zv_normaType == NT_INCOHERENT_COHERENT))
    {
        emit zg_normalizerChanged();
    }
    else if(window->zp_windowType() == ZCalibrationWindow::WT_COHERENT &&
            (zv_normaType == NT_COHERENT ||  zv_normaType == NT_COHERENT_INCOHERENT
             || zv_normaType == NT_INCOHERENT_COHERENT))
    {
        emit zg_normalizerChanged();
    }
}
//======================================================================
