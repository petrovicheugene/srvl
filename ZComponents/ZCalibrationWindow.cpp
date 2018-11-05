//====================================================
#include "ZCalibrationWindow.h"
#include "ZAbstractTerm.h"
#include "ZAbstractSpectrum.h"
#include <QPointer>

//====================================================
qint64 ZCalibrationWindow::zv_lastWindowId = 0;
QMap<ZCalibrationWindow::WindowType, QPair<QString, QString> > ZCalibrationWindow::zv_typeNameMap = ZCalibrationWindow::zh_intTypeNameMap();
//====================================================
QMap<ZCalibrationWindow::WindowType, QPair<QString, QString> > ZCalibrationWindow::zh_intTypeNameMap()
{
    QMap<ZCalibrationWindow::WindowType, QPair<QString, QString> > map;
    QPair<QString, QString> pair;
    pair.first = "Not defined";
    pair.second =  QObject::tr("Not defined");
    map.insert(WT_NOT_DEFINED, pair);

    pair.first = "Peak";
    pair.second =  QObject::tr("Peak");
    map.insert(WT_PEAK, pair);

    pair.first = "Coherent";
    pair.second =  QObject::tr("Coherent");
    map.insert(WT_COHERENT, pair);

    pair.first = "Incoherent";
    pair.second =  QObject::tr("Incoherent");
    map.insert(WT_INCOHERENT, pair);

    return map;
}
//====================================================
ZCalibrationWindow::ZCalibrationWindow(QObject* parent) : QObject(parent)
{
    zv_windowName = QString();
    zv_firstChannel = 0;
    zv_lastChannel = 0;
    zv_spectrumId = -1;
    zv_windowIntensityValue = 0;

    zv_type = WT_NOT_DEFINED;
    zv_visible = true;
    zv_windowId = zv_lastWindowId++;
}
//====================================================
//ZCalibrationWindow::ZCalibrationWindow(const QString& name,  WindowType type)
//{
//    zv_windowName = name;
//    zv_type = type;

//    zv_firstChannel = 0;
//    zv_lastChannel = 0;
//    zv_visible = true;
//    zv_windowId = zv_lastWindowId++;
//}
//====================================================
ZCalibrationWindow::ZCalibrationWindow(const QString& name,
                                       WindowType type,
                                       int firstChannel,
                                       int lastChannel,
                                       QObject *parent) : QObject(parent)
{
    zv_windowName = name;
    zv_type = type;

    if(firstChannel > lastChannel)
    {
        qSwap(firstChannel, lastChannel);
    }
    zv_firstChannel = firstChannel;
    zv_lastChannel = lastChannel;
    zv_visible = true;
    zv_windowId = zv_lastWindowId++;
}
//====================================================
bool ZCalibrationWindow::zp_isWindowVisible() const
{
    return zv_visible;
}
//====================================================
bool ZCalibrationWindow::zp_setWindowVisible(bool visible)
{
    if(zv_visible == visible)
    {
        return false;
    }

    zv_visible = visible;
    return true;
}
//====================================================
QString ZCalibrationWindow::zp_windowName() const
{
    return zv_windowName;
}
//====================================================
bool ZCalibrationWindow::zp_setWindowName(const QString& name)
{
    if(name.isEmpty() || name == zv_windowName)
    {
        return false;
    }
    zv_windowName = name;
    emit zg_windowNameChanged();
    return true;
}
//====================================================
int ZCalibrationWindow::zp_firstChannel() const
{
    return zv_firstChannel;
}
//====================================================
int ZCalibrationWindow::zp_lastChannel() const
{
    return zv_lastChannel;
}
//====================================================
bool ZCalibrationWindow::zp_setWindowFirstChannel(int channel)
{
    if(zv_firstChannel == channel || channel < 0 || channel > zv_lastChannel)
    {
        return false;
    }
    zv_firstChannel = channel;
    emit zg_windowMarginsChanged();
    return true;
}
//====================================================
bool ZCalibrationWindow::zp_setWindowLastChannel(int channel)
{
    if(zv_lastChannel == channel || channel < 0 || channel < zv_firstChannel)
    {
        return false;
    }
    zv_lastChannel = channel;
    emit zg_windowMarginsChanged();
    return true;
}
//====================================================
bool ZCalibrationWindow::zp_setWindowMarginChannels(int first, int last)
{
    if(first == zv_firstChannel && last == zv_lastChannel)
    {
        return false;
    }

    if(first > last)
    {
        qSwap(first, last);
    }
    zv_firstChannel = first;
    zv_lastChannel = last;
    emit zg_windowMarginsChanged();
    return true;
}
//====================================================
ZCalibrationWindow::WindowType ZCalibrationWindow::zp_windowType() const
{
    return zv_type;
}
//====================================================
bool ZCalibrationWindow::zp_setWindowType(ZCalibrationWindow::WindowType type)
{
    if(zv_type == type)
    {
        return false;
    }

    ZCalibrationWindow::WindowType previousType = zv_type;
    zv_type = type;
    emit zg_windowTypeChanged(previousType, zv_type);
    return true;
}
//====================================================
qint64 ZCalibrationWindow::zp_windowId() const
{
    return zv_windowId;
}
//====================================================
QString ZCalibrationWindow::zp_displayTypeName(WindowType type)
{
    if(!zv_typeNameMap.keys().contains(type))
    {
        return QString();
    }
    return zv_typeNameMap.value(type).second;
}
//====================================================
QString ZCalibrationWindow::zp_typeName(WindowType type)
{
    if(!zv_typeNameMap.keys().contains(type))
    {
        return QString();
    }
    return zv_typeNameMap.value(type).first;
}
//====================================================
ZCalibrationWindow::WindowType ZCalibrationWindow::zp_typeFromString(const QString& typeName)
{
    QMap<ZCalibrationWindow::WindowType, QPair<QString, QString> >::const_iterator it;
    for(it = zv_typeNameMap.begin(); it != zv_typeNameMap.end(); it++)
    {
        if(it.value().first == typeName || it.value().second == typeName )
        {
            return it.key();
        }
    }

    return ZCalibrationWindow::WT_NOT_DEFINED;
}
//====================================================
void ZCalibrationWindow::zp_calcWindowIntensity(const QObject *spectrumObject, qreal& intensityValue, bool useBuffer, bool* ok)
{
    const ZAbstractSpectrum* spectrum = qobject_cast<const ZAbstractSpectrum*>(spectrumObject);

    if(!spectrum)
    {
        intensityValue = 0.0;
        if(ok)
        {
            *ok = false;
        }
        return;
    }

    if(zv_spectrumId >= 0 && zv_spectrumId == spectrum->zp_spectrumId() && useBuffer)
    {
        intensityValue = zv_windowIntensityValue;
        if(ok)
        {
            *ok = true;
        }
        return;
    }

    bool res = spectrum->zp_intensityInWindow(zv_firstChannel, zv_lastChannel, intensityValue);
    if(res && useBuffer)
    {
        zv_windowIntensityValue = intensityValue;
        zv_spectrumId = spectrum->zp_spectrumId();
    }
    else
    {
        zv_windowIntensityValue = 0;
        zv_spectrumId = -1;
    }

    if(ok)
    {
        *ok = res;
    }
}
//====================================================
