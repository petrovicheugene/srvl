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
class ZTermNormalizer : public QObject
{
    Q_OBJECT
public:

    enum NormaType {NT_NONE,
                    NT_COHERENT,
                    NT_INCOHERENT,
                    NT_COHERENT_INCOHERENT,
                    NT_INCOHERENT_COHERENT,
                    NT_CUSTOM
                   };

    explicit ZTermNormalizer(ZCalibration *parent);

    // Static
    static QStringList zp_normaTypeList();
    static QString zp_normaTypeString(NormaType);
    static ZTermNormalizer::NormaType zp_normaTypeForString(const QString&);
    // End Static

    void zp_connectToWindow(ZCalibrationWindow* window);

    bool zp_normalizeValue(const ZAbstractSpectrum *spectrum, qreal &value) const;
    bool zp_normalizeValue(qreal &value) const;
    bool zp_calcAndSetNormaValue(const ZAbstractSpectrum *spectrum);
    void zp_resetNormaValue();

    ZTermNormalizer::NormaType zp_normaType() const;
    bool zp_setNormaType(NormaType);

    bool zp_setCustomNormaString(const QString &customString);
    QString zp_normaCustomString() const;
    bool zp_setNormalizerParameters(ZTermNormalizer::NormaType type,
                                  const QString& customString);
signals:

    void zg_normalizerChanged() const;
    void zg_inquiryCoherentIntensity(qint64&) const;
    void zg_inquiryIncoherentIntensity(qint64&) const;

public slots:



private slots:

    void zh_onWindowTypeChange(ZCalibrationWindow::WindowType previousType,
                               ZCalibrationWindow::WindowType currentType) const;
    void zh_onWindowDestroy(QObject* obj) const;
    void zh_onWindowMarginsChange() const;

private:

    // VARS
    NormaType zv_normaType;
    ZCalibration* zv_calibration;
    QString zv_customNormaString;
    // buffer
    qint64 zv_spectrumIdBuffer;
    qreal zv_normaValueBuffer;

    // FUNCS
    void zh_createConnections();
    bool zh_getWindowsValue(ZCalibrationWindow::WindowType type, const ZAbstractSpectrum* spectrum, qint64& value) const;
    bool zh_calcNormaValue(const ZAbstractSpectrum *spectrum, qreal& normaValue) const;


    // STATIC
    // VARS
    static QMap<ZTermNormalizer::NormaType, QString> zv_normaTypeStringMap;

    // FUNCS
    static QMap<ZTermNormalizer::NormaType, QString> zh_initNormaTypeStringMap();

};
//======================================================================
#endif // ZTERMNORMALIZATOR_H
