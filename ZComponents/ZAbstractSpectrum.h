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
/*!
 \brief

*/
class ZAbstractSpectrum : public QObject
{
    Q_OBJECT
public:
    /*!
     \brief

     \param intensityList
     \param path
     \param color
     \param parent
    */
    explicit ZAbstractSpectrum(const QList<int> &intensityList, const QString& path, QColor color, QObject* parent);
    /*!
     \brief

    */
    virtual ~ZAbstractSpectrum();

    // VARS
    /*!
     \brief

    */
    enum SpectrumType {ST_NA, ST_SPE};

    // FUNCS
    /*!
     \brief

     \return QString
    */
    QString zp_name() const;
    /*!
     \brief

     \return QString
    */
    QString zp_path() const ;
    /*!
     \brief

     \return SpectrumType
    */
    SpectrumType zp_type() const;
    /*!
     \brief

     \return bool
    */
    bool zp_isSpectrumVisible() const;
    /*!
     \brief

     \param bool
    */
    void zp_setSpectrumVisible(bool);

    /*!
     \brief

     \return bool
    */
    bool zp_isSpectrumChecked() const;
    /*!
     \brief

     \param bool
     \return bool
    */
    bool zp_setSpectrumChecked(bool);

    /*!
     \brief

     \param chemElementId
     \param concentration
    */
    void zp_insertConcentration(qint64 chemElementId, const QString& concentration);
    /*!
     \brief

     \param chemElementId
     \return bool
    */
    bool zp_removeConcentration(qint64 chemElementId);
    /*!
     \brief

     \param chemElementId
     \return QString
    */
    QString zp_concentrationString(qint64 chemElementId) const;
    /*!
     \brief

     \param chemElementId
     \return qreal
    */
    qreal zp_concentrationValue(qint64 chemElementId) const;
    /*!
     \brief

     \param chemElementId
     \param concentration
     \return bool
    */
    bool zp_setConcentration(qint64 chemElementId, const QString& concentration);
    /*!
     \brief

     \return QList<int>
    */
    QList<int> zp_spectrumData() const;
    /*!
     \brief

     \return const ZAbstractSpectrumAuxData
    */
    virtual const ZAbstractSpectrumAuxData* zp_auxData() const = 0;
    /*!
     \brief

     \param startChannel
     \param lastChannel
     \param intensity
     \return bool
    */
    bool zp_intensityInWindow(int startChannel, int lastChannel, qreal& intensity) const;
    /*!
     \brief

     \param
     \param intensity
     \return bool
    */
    bool zp_intensityInWindow(const ZCalibrationWindow*, qreal& intensity) const;

    /*!
     \brief

     \return int
    */
    int zp_channelCount() const;
    /*!
     \brief

     \return int
    */
    int zp_maxIntensity() const;
    /*!
     \brief

     \return QColor
    */
    QColor zp_color() const;
    /*!
     \brief

     \param QColor
    */
    void zp_setColor(QColor);
    /*!
     \brief

     \return qint64
    */
    qint64 zp_spectrumId() const;
    /*!
     \brief

     \return QPainterPath
    */
    QPainterPath zp_spectrumPainterPath() const;

protected:

    // VARS
    SpectrumType zv_type; /*!< TODO: describe */
    QString zv_name; /*!< TODO: describe */
    QString zv_path; /*!< TODO: describe */
    QList<int> zv_spectrumData; /*!< TODO: describe */

    QPainterPath zv_spectrumPaintPath; /*!< TODO: describe */

    QMap<qint64, QString> zv_concentrationMap; /*!< TODO: describe */
    bool zv_visible; /*!< TODO: describe */
    bool zv_checked; /*!< TODO: describe */
    QColor zv_color; /*!< TODO: describe */
    qint64 zv_spectrumId; /*!< TODO: describe */

    int zv_channelCount; /*!< TODO: describe */
    int zv_maxIntensity; /*!< TODO: describe */

    // FUNCS
    /*!
     \brief

    */
    void zh_recalcSpectrumPainterPath();

    // STATIC
    // VARS
    static qint64 zv_lastSpectrumId; /*!< TODO: describe */

};
//==========================================================
#endif // ZABSTRACTSPECTRUM_H
