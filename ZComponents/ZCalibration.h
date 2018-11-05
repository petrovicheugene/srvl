//=========================================================
#ifndef QABSTRACTCALIBRATION_H
#define QABSTRACTCALIBRATION_H
//=========================================================
#include <QObject>
#include <QMap>
#include <QDateTime>
#include <QColor>
#include "ZCalibrationWindow.h"
#include "ZAbstractTerm.h"
#include "ZCalibrationQualityData.h"
#include "ZRawTerm.h"
#include "ZRawWindow.h"
//=========================================================
class ZAbstractSpectrum;
class ZSimpleTerm;
class ZQuadraticTerm;
class ZMixedTerm;
class ZCustomTerm;
class ZTermNormalizer;
//=========================================================

//=========================================================
class ZCalibration : public QObject
{
    Q_OBJECT
    friend class ZAbstractTerm;
    friend class ZTermNormalizer;
    friend class ZTermCorrelationTableManager;

public:

    explicit ZCalibration(const QString& name, QObject *parent = 0);
    explicit ZCalibration(const ZCalibration* calibration,
                          const QString &name, QObject *parent = 0);
    virtual ~ZCalibration();

    // VARS
    enum WindowOperationType {WOT_BRGIN_INSERT_WINDOWS,
                              WOT_END_INSERT_WINDOWS,
                              WOT_BEGIN_REMOVE_WINDOWS,
                              WOT_END_REMOVE_WINDOWS,
                              WOT_WINDOW_CHANGED
                             };


    enum TremOperationType {TOT_BEGIN_INSERT_TERM,
                            TOT_END_INSERT_TERM,
                            TOT_BEGIN_REMOVE_TERM,
                            TOT_END_REMOVE_TERM,
                            TOT_TERM_NAME_CHANGED,
                            TOT_TERM_STATE_CHANGED,
                            TOT_TERM_WINDOW_MARGIN_CHANGED,
                            TOT_TERM_FACTOR_CHANGED
                           };

    enum EquationType {ET_NOT_DEFINED,
                       ET_POLYNOMIAL,
                       ET_FRACTIONAL
                      };
    // FUNCS
    QDateTime zp_dateTime() const;
    QString zp_baseName() const;
    void zp_setBaseName(const QString&);
    QString zp_fileName() const;

    QString zp_suffix() const;
    void zp_setName(const QString &);

    QString zp_path() const;
    void zp_setPath(const QString&);

    QString zp_chemElement() const;
    bool zp_setChemElement(const QString&);

//    void zp_setEnergyCalibration(qreal K0, qreal K1, qreal K2, const QString& energyUnit);
//    void zp_setEnergyCalibrationK0(qreal K0);
//    void zp_setEnergyCalibrationK1(qreal K1);
//    void zp_setEnergyCalibrationK2(qreal K2);
//    void zp_setEnergyUnit(const QString& energyUnit);
    void zp_setGainFactor(int gainFactor);
    void zp_setExposition(int exposition);

//    qreal zp_energyCalibrationK0() const;
//    qreal zp_energyCalibrationK1() const;
//    qreal zp_energyCalibrationK2() const;
//    QString zp_energyUnit() const;
    int zp_gainFactor() const;
    int zp_exposition() const;

    bool zp_isVisible() const;
    void zp_setVisible(bool);

    bool zp_isDirty() const;
    void zp_setDirty(bool);
    QColor zp_color() const;
    qint64 zp_calibrationId() const;
    bool zp_calcConcentration(const ZAbstractSpectrum* const, qreal& concentration);

    // windows
    int zp_createNewCalibrationWindow(int firstChannel = 0,
                                       int lastChannel = 0,
                                       ZCalibrationWindow::WindowType windowType = ZCalibrationWindow::WT_NOT_DEFINED);
    int zp_createNewCalibrationWindow(const ZRawWindow& rawWindow);
    bool zp_isCalibrationWindowVisible(int windowIndex) const;
    bool zp_setCalibrationWindowVisible(int windowIndex, bool visibility);
    int zp_calibrationWindowCount() const;
    QString zp_calibrationWindowName(int windowIndex) const;
    const ZCalibrationWindow* zp_calibrationWindow(int windowIndex) const;
    QString zp_calibrationWindowNameForId(qint64 windowId) const;

    bool zp_setCalibrationWindowName(int windowIndex, const QString&);
    ZCalibrationWindow::WindowType zp_calibrationWindowType(int windowIndex) const;
    bool zp_setCalibrationWindowType(int windowIndex, ZCalibrationWindow::WindowType type);

    int zp_calibrationWindowFirstChannel(int windowIndex) const;
    bool zp_setCalibrationWindowFirstChannel(int windowIndex, int channel);
    int zp_calibrationWindowLastChannel(int windowIndex) const;
    bool zp_setCalibrationWindowLastChannel(int windowIndex, int channel);

    qint64 zp_calibrationWindowId(int windowIndex) const;
    int zp_windowIndexForName(const QString&windowName) const;
    bool zp_removeCalibrationWindow(int);
    void zp_removeCalibrationWindows();

    // terms
    int zp_termCount() const;
    QString zp_termDisplayName(int termIndex) const;
    QString zp_termName(int termIndex) const;

    ZAbstractTerm::TermType zp_termType(int termIndex) const;
    bool zp_mixedTermExists() const;
    qint64 zp_termId(int termIndex) const;
    bool zp_setTermState(int termIndex, ZAbstractTerm::TermState zp_termStateForDisplay);
    ZAbstractTerm::TermState zp_termStateForDisplay(int termIndex) const;
    ZAbstractTerm::TermState zp_termState(int termIndex) const;

    void zp_setNextUsersTermState(int);
    bool zp_termFactor(int termIndex, qreal &factor) const;

    QList<int> zp_termWindowIndexList(int termIndex) const;
    QStringList zp_termWindowsNameList(int termIndex) const;

    QString zp_termFactorString(int termIndex) const;
    bool zp_termFactorString(int termIndex, QString &factorString) const;
    bool zp_setTermFactorString(int termIndex, const QString& factorString);
    bool zp_termVariablePart(int termIndex, const ZAbstractSpectrum* spectrum,  qreal &value) const;
    bool zp_calcBaseTermValue(const ZAbstractSpectrum* spectrum, qreal& value) const;
    bool zp_createMixedTerms(int termIndex);
    bool zp_removeMixedTerms();

    int zp_createTerm(QList<int>& windowIndexList,
                      ZAbstractTerm::TermType termType,
                      ZAbstractTerm::TermState termState,
                      const QString& termFactor);

    int zp_createTerm(const ZRawTerm& rawTerm);

    // normalizer
    ZTermNormalizer::NormaType zp_normaType() const;
    bool zp_setNormaType(ZTermNormalizer::NormaType type);
    bool zp_setNormaCustomString(const QString& customString);
    QString zp_normaCustomString() const;

    ZTermNormalizer::NormaType zp_baseTermNormaType() const;
    bool zp_setBaseTermNormaType(ZTermNormalizer::NormaType type);
    bool zp_setBaseTermNormaCustomString(const QString& customString);
    QString zp_baseTermNormaCustomString() const;
    bool zp_setBaseTermNormalizerParameters(ZTermNormalizer::NormaType type,
                                  const QString& customString);


    EquationType zp_equationType() const;
    bool zp_setEquationType(EquationType type);

    qreal zp_equationIntercept() const;
    bool zp_setEquationIntercept(qreal);

    QString zp_equationInterceptString() const;
    bool zp_setEquationInterceptString(const QString&);

    QString zp_baseTermDisplayString() const;
    QString zp_baseTermString() const;

    qint64 zp_baseTermId() const;
    bool zp_setBaseTermId(qint64 id);
    int zp_baseTermIndex() const;
    bool zp_setBaseTermFromName(const QString& baseTermName);

    void zp_createEquationDataForEquationRecalc(QMap<int, qreal *> &factorMap,
                                                qreal *&freeTermPtr);

    void zh_notifyCalibrationRecalc();
    void zp_resetEquationTerms();
    ZCalibrationQualityData zp_calibrationQualityData() const;
    void zp_setCalibrationQualityData(ZCalibrationQualityData qualityData);

    void zp_setDetermination(const QString&);
    void zp_setAdjustedDetermination(const QString&);
    void zp_setStandardDeviation(const QString&);

    void zp_setCurrentDateTime();
    void zp_setDateTime(QDateTime);

    // STATIC
    // VARS
    static const QString simplePolynomEquationString;
    static const QString fractionalEquationString;
    // FUNCS
    static QString zp_equationTypeString(ZCalibration::EquationType type);
    static ZCalibration::EquationType zp_equationTypeFromString(const QString&);

signals:

    void zg_message(QString) const;
    void zg_windowOperation(ZCalibration::WindowOperationType, int first, int last) const;
    void zg_visibilityChanged(bool) const;
    void zg_dirtyChanged(bool) const;
    void zg_termOperation(ZCalibration::TremOperationType, int first, int last)const;
    void zg_normalizerChanged() const;
    void zg_interceptChanged() const;

public slots:


private slots:

    void zh_onTermNameChange();
    void zh_onTermWindowMarginChange();
    void zh_removeTerm(ZAbstractTerm*);
    void zh_onNormalizerChange();

private:

    // VARS
    QString zv_baseName;
    QString zv_suffix;
    QString zv_path;
    QString zv_chemElement;
    int zv_exposition;
    qreal zv_energyCalibrationFactorK0;
    qreal zv_energyCalibrationFactorK1;
    qreal zv_energyCalibrationFactorK2;
    int zv_gainFactor;
    QString zv_energyUnit;

    QColor zv_color;
    qint64 zv_calibrationId;
    ZCalibrationQualityData zv_calibrationQualityData;
    bool zv_visibility;
    bool zv_dirty;

    QList<ZCalibrationWindow*> zv_windowList;
    QList<ZAbstractTerm*> zv_termList;
    EquationType zv_equationType;
    ZTermNormalizer* zv_termNormalizer;
    QString zv_interceptString;
    qreal zv_intercept;
    qint64 zv_baseTermId;
    // qint64 zv_baseTermNormaId;
    ZTermNormalizer* zv_baseTermNormalizer;
    bool zv_useBaseTermInFractionalEquation;
    QDateTime zv_dateTime;

    // FUNCS
    bool zh_isWindowExist(const QString&);
    void zh_createTermsForWindow(const ZCalibrationWindow*);
    bool zh_windowHasTerms(const ZCalibrationWindow*,
                           ZAbstractTerm::TermType) const;
    int zh_termIndex(const ZAbstractTerm*) const;
    void zh_chopTailZeroesFromInterceptString();
    qreal* zh_termFactorPointer(int termIndex) const;
    int zh_findNextTermIndex(ZAbstractTerm::TermType) const;

    // STATIC
    // VARS
    static QList<QColor> zv_colorList;
    static qint64 zv_lastCalibrationId;
    static int zv_lastColorIndex;
    static int zv_precision;
    static const QString zv_defaultWindowName;

    static QMap<ZCalibration::EquationType, QString> zv_eqationTypeStringMap;
    static bool zv_useBaseTermInFractionalEquationByDefault;
    //  FUNCS
    static QList<QColor> zp_createColorList();
    static bool checkColor(QColor color);
    static QMap<ZCalibration::EquationType, QString> zh_initEquationTypeStringMap();
    static QString zh_initPlynomialEquationString();
    static QString zh_initFractionalEquationString();
    static QString zh_initFractionalEquationString1();

};
//=========================================================
#endif // QABSTRACTCALIBRATION_H
