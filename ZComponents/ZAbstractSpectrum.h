//==========================================================
// BASE CLASS FOR ALL SPECTRUM CLASS
#ifndef ZABSTRACTSPECTRUM_H
#define ZABSTRACTSPECTRUM_H
//==========================================================
#include "ZAbstractSpectrumAuxData.h"

#include <QObject>
#include <QMetaType>
#include <QString>
#include <QStringList>
#include <QDate>
#include <QTime>
#include <QList>
#include <QMap>
#include <QColor>
#include <QPainterPath>
//==========================================================
class ZCalibrationWindow;
//==========================================================
// Inheritage: ADD  SpectrumType !!!
//==========================================================
class ZAbstractSpectrum : public QObject
{
    Q_OBJECT
public:

    //explicit ZAbstractSpectrum( QObject* parent = nullptr);
    explicit ZAbstractSpectrum(const QList<quint32> &intensityList,
                               const QString& path,
                               QColor color,
                               bool completed,
                               QObject* parent = nullptr);
    virtual ~ZAbstractSpectrum();

    // VARS
    enum SpectrumType {ST_NA, ST_SPE};

    // FUNCS
    QString zp_name() const;
    void zp_setSpectrumName(const QString& name);
    QString zp_path() const ;
    SpectrumType zp_type() const;
    bool zp_isSpectrumVisible() const;
    void zp_setSpectrumVisible(bool);

    bool zp_isSpectrumChecked() const;
    bool zp_setSpectrumChecked(bool);

    void zp_insertConcentration(qint64 chemElementId, const QString& concentration);
    bool zp_removeConcentration(qint64 chemElementId);
    QString zp_concentrationString(qint64 chemElementId) const;
    qreal zp_concentrationValue(qint64 chemElementId) const;
    bool zp_setConcentration(qint64 chemElementId, const QString& concentration);
    QList<quint32> zp_spectrumData() const;
    void zp_setSpectrumData(QList<quint32> data);

    virtual const ZAbstractSpectrumAuxData* zp_auxData() const = 0;
    bool zp_intensityInWindow(int startChannel, int lastChannel, qreal& intensity) const;
    bool zp_intensityInWindow(const ZCalibrationWindow*, qreal& intensity) const;

    int zp_channelCount() const;
    int zp_maxIntensity() const;
    QColor zp_color() const;
    void zp_setColor(QColor);
    qint64 zp_spectrumId() const;
    QPainterPath zp_spectrumPainterPath() const;

    void zp_setCompleted(bool completed);
    bool zp_isCompleted() const;

protected:

    // VARS
    SpectrumType zv_type;
    QString zv_name;
    QString zv_path;
    QList<quint32> zv_spectrumData;

    QPainterPath zv_spectrumPaintPath;

    QMap<qint64, QString> zv_concentrationMap;
    bool zv_visible;
    bool zv_checked;
    QColor zv_color;
    qint64 zv_spectrumId;

    int zv_channelCount;
    quint64 zv_maxIntensity;
    bool zv_completed;

    // FUNCS
    void zh_recalcSpectrumPainterPath();

    // STATIC
    // VARS
    static qint64 zv_lastSpectrumId;

};
//==========================================================
#endif // ZABSTRACTSPECTRUM_H
