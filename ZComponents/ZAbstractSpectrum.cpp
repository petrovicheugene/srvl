//==========================================================
#include "ZAbstractSpectrum.h"
#include <QFileInfo>
#include "ZCalibrationWindow.h"
//==========================================================
qint64 ZAbstractSpectrum::zv_lastSpectrumId = 0;
//==========================================================
ZAbstractSpectrum::ZAbstractSpectrum(const QList<quint32> &intensityList,
                                     const QString& path, QColor color, bool completed, QObject *parent)
    : QObject(parent)
{
    zv_visible = true;
    zv_path = path;
    zv_color = color;
    zv_spectrumId = zv_lastSpectrumId++;
    zv_checked = true;
    zv_completed = completed;

    QFileInfo fileInfo(path);
    QString suffix = fileInfo.suffix();
    if(suffix == "spe")
    {
        zv_type = ST_SPE;
    }
    else
    {
        zv_type = ST_NA;
    }

    zv_name = fileInfo.fileName();
    zv_spectrumData = intensityList;
    zv_channelCount = intensityList.count();
    zv_maxIntensity = 0;
    for(int i = 0; i < zv_channelCount; i++)
    {
        if(zv_maxIntensity < intensityList.value(i))
        {
            zv_maxIntensity = intensityList.value(i);
        }
    }

    // zh_recalcSpectrumPainterPath();
}
//==========================================================
ZAbstractSpectrum::~ZAbstractSpectrum()
{

}
//==========================================================
ZAbstractSpectrum::SpectrumType ZAbstractSpectrum::zp_type() const
{
    return zv_type;
}
//==========================================================
bool ZAbstractSpectrum::zp_isSpectrumVisible() const
{
    return zv_visible;
}
//==========================================================
void ZAbstractSpectrum::zp_setSpectrumVisible(bool visible)
{
    zv_visible = visible;
}
//==========================================================
bool ZAbstractSpectrum::zp_isSpectrumChecked() const
{
    return zv_checked;
}
//==========================================================
bool ZAbstractSpectrum::zp_setSpectrumChecked(bool checked)
{
    if(zv_checked == checked)
    {
        return false;
    }

    zv_checked = checked;
    return true;
}
//==========================================================
void ZAbstractSpectrum::zp_insertConcentration(qint64 chemElementId, const QString& concentration)
{
    zv_concentrationMap.insert(chemElementId, concentration);
}
//==========================================================
bool ZAbstractSpectrum::zp_removeConcentration(qint64 chemElementId)
{
    if(zv_concentrationMap.remove(chemElementId) > 0)
    {
        return true;
    }

    return false;
}
//==========================================================
QString ZAbstractSpectrum::zp_concentrationString(qint64 chemElementId) const
{
    if(!zv_concentrationMap.keys().contains(chemElementId))
    {
        return QString();
    }

    return zv_concentrationMap.value(chemElementId);
}
//==========================================================
qreal ZAbstractSpectrum::zp_concentrationValue(qint64 chemElementId) const
{
    QString concentrationString =  zp_concentrationString(chemElementId);
    qreal concentration;
    bool ok;
    if(concentrationString.isEmpty())
    {
        concentration = 0;
    }
    else
    {
        concentration = concentrationString.toDouble(&ok);
        if(!ok)
        {
            concentration = 0;
        }
    }
    return concentration;
}
//==========================================================
bool ZAbstractSpectrum::zp_setConcentration(qint64 chemElementId,
                                            const QString& concentration)
{
    if(chemElementId < 0)
    {
        return false;
    }

    if(zv_concentrationMap.contains(chemElementId)
            && ( zv_concentrationMap.value(chemElementId) == concentration))
    {
        return false;
    }

    zv_concentrationMap.insert(chemElementId, concentration);
    return true;
}
//==========================================================
QList<quint32> ZAbstractSpectrum::zp_spectrumData() const
{
    return zv_spectrumData;
}
//==========================================================
void ZAbstractSpectrum::zp_setSpectrumData(QList<quint32> data)
{
    zv_spectrumData = data;
    zv_channelCount = data.count();
    zv_maxIntensity = 0;
    for(int i = 0; i < zv_channelCount; i++)
    {
        if(zv_maxIntensity < data.value(i))
        {
            zv_maxIntensity = data.value(i);
        }
    }

    zh_recalcSpectrumPainterPath();
}
//==========================================================
QString ZAbstractSpectrum::zp_name() const
{
    return zv_name;
}
//==========================================================
void ZAbstractSpectrum::zp_setSpectrumName(const QString& name)
{
    zv_name = name;
}
//==========================================================
QString ZAbstractSpectrum::zp_path() const
{
    return zv_path;
}
//==========================================================
bool ZAbstractSpectrum::zp_intensityInWindow(int startChannel, int lastChannel, qreal& intensity) const
{
    if(startChannel < 0)
    {
        startChannel = 0;
    }
    else if(startChannel > zv_spectrumData.count())
    {
        startChannel = zv_spectrumData.count() - 1;
    }

    if(lastChannel < 0)
    {
        lastChannel = 0;
    }
    else if(lastChannel > zv_spectrumData.count())
    {
        lastChannel = zv_spectrumData.count() - 1;
    }

//    if(startChannel == lastChannel)
//    {
//        return false;
//    }

    if(startChannel > lastChannel)
    {
        qSwap(startChannel, lastChannel);
    }

    intensity = 0.0;
    for(int c = startChannel; c <= lastChannel; c++)
    {
        intensity += static_cast<qreal>(zv_spectrumData.value(c));
    }

    return true;
}
//==========================================================
bool ZAbstractSpectrum::zp_intensityInWindow(const ZCalibrationWindow* window, qreal &intensity) const
{
    if(!window)
    {
        return false;
    }

    return zp_intensityInWindow(window->zp_firstChannel(), window->zp_lastChannel(),  intensity);
}
//==========================================================
int ZAbstractSpectrum::zp_channelCount() const
{
    return zv_channelCount;
}
//==========================================================
int ZAbstractSpectrum::zp_maxIntensity() const
{
    return zv_maxIntensity;
}
//==========================================================
QColor ZAbstractSpectrum::zp_color() const
{
    return zv_color;
}
//==========================================================
void ZAbstractSpectrum::zp_setColor(QColor color)
{
    zv_color = color;
}
//==========================================================
qint64 ZAbstractSpectrum::zp_spectrumId() const
{
    return zv_spectrumId;
}
//==========================================================
QPainterPath ZAbstractSpectrum::zp_spectrumPainterPath() const
{
    return zv_spectrumPaintPath;
}
//==========================================================
void ZAbstractSpectrum::zp_setCompleted(bool completed)
{
    zv_completed = completed;
}
//==========================================================
bool ZAbstractSpectrum::zp_isCompleted() const
{
    return zv_completed;
}
//==========================================================
void ZAbstractSpectrum::zh_recalcSpectrumPainterPath()
{
    QPointF point(0.0, 0.0);
    QPainterPath newShape;
    newShape.moveTo(point);

    qreal scValue = 0.0;
    for(int i = 0; i < zv_spectrumData.count(); i++)
    {
        scValue = (qreal)(zv_spectrumData.value(i));
        point = QPointF((qreal)i,  scValue*-1);
        newShape.lineTo(point);
        point = QPointF((qreal)(i+1),  scValue*-1);
        newShape.lineTo(point);
    }

    // last point
    point = QPointF((qreal)(zv_spectrumData.count() - 1),  0);
    newShape.lineTo(point);
    zv_spectrumPaintPath = newShape;
}
//==========================================================
