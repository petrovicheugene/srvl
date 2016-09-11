//======================================================================
#ifndef ZTERMNORMALIZATOR_H
#define ZTERMNORMALIZATOR_H
//======================================================================
#include <QObject>
#include "ZCalibrationWindow.h"
//======================================================================
class ZAbstractSpectrum;
class ZCalibration;
//======================================================================
/*!
 \brief

*/
class ZTermNormalizer : public QObject
{
    Q_OBJECT
public:

    /*!
     \brief

    */
    enum NormaType {NT_NONE,
                    NT_COHERENT,
                    NT_INCOHERENT,
                    NT_COHERENT_INCOHERENT,
                    NT_INCOHERENT_COHERENT,
                    NT_CUSTOM
                   };

    /*!
     \brief

     \param parent
    */
    explicit ZTermNormalizer(ZCalibration *parent);

    // Static
    /*!
     \brief

     \return QStringList
    */
    static QStringList zp_normaTypeList();
    /*!
     \brief

     \param NormaType
     \return QString
    */
    static QString zp_normaTypeString(NormaType);
    /*!
     \brief

     \param
     \return ZTermNormalizer::NormaType
    */
    static ZTermNormalizer::NormaType zp_normaTypeForString(const QString&);
    // End Static

    /*!
     \brief

     \param window
    */
    void zp_connectToWindow(ZCalibrationWindow* window);

    /*!
     \brief

     \param spectrum
     \param value
     \return bool
    */
    bool zp_normalizeValue(const ZAbstractSpectrum *spectrum, qreal &value) const;
    /*!
     \brief

     \param value
     \return bool
    */
    bool zp_normalizeValue(qreal &value) const;
    /*!
     \brief

     \param spectrum
     \return bool
    */
    bool zp_calcAndSetNormaValue(const ZAbstractSpectrum *spectrum);
    /*!
     \brief

    */
    void zp_resetNormaValue();

    /*!
     \brief

     \return ZTermNormalizer::NormaType
    */
    ZTermNormalizer::NormaType zp_normaType() const;
    /*!
     \brief

     \param NormaType
     \return bool
    */
    bool zp_setNormaType(NormaType);

    /*!
     \brief

     \param customString
     \return bool
    */
    bool zp_setCustomNormaString(const QString &customString);
    /*!
     \brief

     \return QString
    */
    QString zp_normaCustomString() const;
    /*!
     \brief

     \param type
     \param customString
     \return bool
    */
    bool zp_setNormalizerParameters(ZTermNormalizer::NormaType type,
                                  const QString& customString);
signals:

    /*!
     \brief

    */
    void zg_normalizerChanged() const;
    /*!
     \brief

     \param
    */
    void zg_requestCoherentIntensity(qint64&) const;
    /*!
     \brief

     \param
    */
    void zg_requestIncoherentIntensity(qint64&) const;

public slots:



private slots:

    /*!
     \brief

     \param previousType
     \param currentType
    */
    void zh_onWindowTypeChange(ZCalibrationWindow::WindowType previousType,
                               ZCalibrationWindow::WindowType currentType) const;
    /*!
     \brief

     \param obj
    */
    void zh_onWindowDestroy(QObject* obj) const;
    /*!
     \brief

    */
    void zh_onWindowMarginsChange() const;

private:

    // VARS
    NormaType zv_normaType; /*!< TODO: describe */
    ZCalibration* zv_calibration; /*!< TODO: describe */
    QString zv_customNormaString; /*!< TODO: describe */
    // buffer
    qint64 zv_spectrumIdBuffer; /*!< TODO: describe */
    qreal zv_normaValueBuffer; /*!< TODO: describe */

    // FUNCS
    /*!
     \brief

    */
    void zh_createConnections();
    /*!
     \brief

     \param type
     \param spectrum
     \param value
     \return bool
    */
    bool zh_getWindowsValue(ZCalibrationWindow::WindowType type, const ZAbstractSpectrum* spectrum, qint64& value) const;
    /*!
     \brief

     \param spectrum
     \param normaValue
     \return bool
    */
    bool zh_calcNormaValue(const ZAbstractSpectrum *spectrum, qreal& normaValue) const;


    // STATIC
    // VARS
    static QMap<ZTermNormalizer::NormaType, QString> zv_normaTypeStringMap; /*!< TODO: describe */

    // FUNCS
    /*!
     \brief

     \return QMap<ZTermNormalizer::NormaType, QString>
    */
    static QMap<ZTermNormalizer::NormaType, QString> zh_initNormaTypeStringMap();

};
//======================================================================
#endif // ZTERMNORMALIZATOR_H
