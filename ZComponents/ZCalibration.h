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
/*!
 \brief

*/
class ZCalibration : public QObject
{
    Q_OBJECT
    friend class ZAbstractTerm;
    friend class ZTermNormalizer;
    friend class ZTermCorrelationTableManager;

public:

    /*!
     \brief

     \param name
     \param parent
    */
    explicit ZCalibration(const QString& name, QObject *parent = 0);
    /*!
     \brief

     \param calibration
     \param name
     \param parent
    */
    explicit ZCalibration(const ZCalibration* calibration,
                          const QString &name, QObject *parent = 0);
    /*!
     \brief

    */
    virtual ~ZCalibration();

    // VARS
    /*!
     \brief

    */
    enum WindowOperationType {WOT_BRGIN_INSERT_WINDOWS,
                              WOT_END_INSERT_WINDOWS,
                              WOT_BEGIN_REMOVE_WINDOWS,
                              WOT_END_REMOVE_WINDOWS,
                              WOT_WINDOW_CHANGED
                             };


    /*!
     \brief

    */
    enum TremOperationType {TOT_BEGIN_INSERT_TERM,
                            TOT_END_INSERT_TERM,
                            TOT_BEGIN_REMOVE_TERM,
                            TOT_END_REMOVE_TERM,
                            TOT_TERM_NAME_CHANGED,
                            TOT_TERM_STATE_CHANGED,
                            TOT_TERM_WINDOW_MARGIN_CHANGED,
                            TOT_TERM_FACTOR_CHANGED
                           };

    /*!
     \brief

    */
    enum EquationType {ET_NOT_DEFINED,
                       ET_POLYNOMIAL,
                       ET_FRACTIONAL
                      };
    // FUNCS
    /*!
     \brief

     \return QDateTime
    */
    QDateTime zp_dateTime() const;
    /*!
     \brief

     \return QString
    */
    QString zp_baseName() const;
    /*!
     \brief

     \param
    */
    void zp_setBaseName(const QString&);
    /*!
     \brief

     \return QString
    */
    QString zp_fileName() const;

    /*!
     \brief

     \return QString
    */
    QString zp_suffix() const;
    /*!
     \brief

     \param
    */
    void zp_setName(const QString &);

    /*!
     \brief

     \return QString
    */
    QString zp_path() const;
    /*!
     \brief

     \param
    */
    void zp_setPath(const QString&);

    /*!
     \brief

     \return QString
    */
    QString zp_chemElement() const;
    /*!
     \brief

     \param
     \return bool
    */
    bool zp_setChemElement(const QString&);

    /*!
     \brief

     \return bool
    */
    bool zp_isVisible() const;
    /*!
     \brief

     \param bool
    */
    void zp_setVisible(bool);

    /*!
     \brief

     \return bool
    */
    bool zp_isDirty() const;
    /*!
     \brief

     \param bool
    */
    void zp_setDirty(bool);
    /*!
     \brief

     \return QColor
    */
    QColor zp_color() const;
    /*!
     \brief

     \return qint64
    */
    qint64 zp_calibrationId() const;
    /*!
     \brief

     \param const
     \param concentration
     \return bool
    */
    bool zp_calcConcentration(const ZAbstractSpectrum* const, qreal& concentration);

    // windows
    /*!
     \brief

     \param firstChannel
     \param lastChannel
     \param windowType
     \return int
    */
    int zp_createNewCalibrationWindow(int firstChannel = 0,
                                       int lastChannel = 0,
                                       ZCalibrationWindow::WindowType windowType = ZCalibrationWindow::WT_NOT_DEFINED);
    /*!
     \brief

     \param rawWindow
     \return int
    */
    int zp_createNewCalibrationWindow(const ZRawWindow& rawWindow);
    /*!
     \brief

     \param windowIndex
     \return bool
    */
    bool zp_isCalibrationWindowVisible(int windowIndex) const;
    /*!
     \brief

     \param windowIndex
     \param visibility
     \return bool
    */
    bool zp_setCalibrationWindowVisible(int windowIndex, bool visibility);
    /*!
     \brief

     \return int
    */
    int zp_calibrationWindowCount() const;
    /*!
     \brief

     \param windowIndex
     \return QString
    */
    QString zp_calibrationWindowName(int windowIndex) const;
    /*!
     \brief

     \param windowIndex
     \return const ZCalibrationWindow
    */
    const ZCalibrationWindow* zp_calibrationWindow(int windowIndex) const;
    /*!
     \brief

     \param windowId
     \return QString
    */
    QString zp_calibrationWindowNameForId(qint64 windowId) const;

    /*!
     \brief

     \param windowIndex
     \param
     \return bool
    */
    bool zp_setCalibrationWindowName(int windowIndex, const QString&);
    /*!
     \brief

     \param windowIndex
     \return ZCalibrationWindow::WindowType
    */
    ZCalibrationWindow::WindowType zp_calibrationWindowType(int windowIndex) const;
    /*!
     \brief

     \param windowIndex
     \param type
     \return bool
    */
    bool zp_setCalibrationWindowType(int windowIndex, ZCalibrationWindow::WindowType type);

    /*!
     \brief

     \param windowIndex
     \return int
    */
    int zp_calibrationWindowFirstChannel(int windowIndex) const;
    /*!
     \brief

     \param windowIndex
     \param channel
     \return bool
    */
    bool zp_setCalibrationWindowFirstChannel(int windowIndex, int channel);
    /*!
     \brief

     \param windowIndex
     \return int
    */
    int zp_calibrationWindowLastChannel(int windowIndex) const;
    /*!
     \brief

     \param windowIndex
     \param channel
     \return bool
    */
    bool zp_setCalibrationWindowLastChannel(int windowIndex, int channel);

    /*!
     \brief

     \param windowIndex
     \return qint64
    */
    qint64 zp_calibrationWindowId(int windowIndex) const;
    /*!
     \brief

     \param windowName
     \return int
    */
    int zp_windowIndexForName(const QString&windowName) const;
    /*!
     \brief

     \param int
     \return bool
    */
    bool zp_removeCalibrationWindow(int);
    /*!
     \brief

    */
    void zp_removeCalibrationWindows();

    // terms
    /*!
     \brief

     \return int
    */
    int zp_termCount() const;
    /*!
     \brief

     \param termIndex
     \return QString
    */
    QString zp_termDisplayName(int termIndex) const;
    /*!
     \brief

     \param termIndex
     \return QString
    */
    QString zp_termName(int termIndex) const;

    /*!
     \brief

     \param termIndex
     \return ZAbstractTerm::TermType
    */
    ZAbstractTerm::TermType zp_termType(int termIndex) const;
    /*!
     \brief

     \return bool
    */
    bool zp_mixedTermExists() const;
    /*!
     \brief

     \param termIndex
     \return qint64
    */
    qint64 zp_termId(int termIndex) const;
    /*!
     \brief

     \param termIndex
     \param zp_termStateForDisplay
     \return bool
    */
    bool zp_setTermState(int termIndex, ZAbstractTerm::TermState zp_termStateForDisplay);
    /*!
     \brief

     \param termIndex
     \return ZAbstractTerm::TermState
    */
    ZAbstractTerm::TermState zp_termStateForDisplay(int termIndex) const;
    /*!
     \brief

     \param termIndex
     \return ZAbstractTerm::TermState
    */
    ZAbstractTerm::TermState zp_termState(int termIndex) const;

    /*!
     \brief

     \param int
    */
    void zp_setNextUsersTermState(int);
    /*!
     \brief

     \param termIndex
     \param factor
     \return bool
    */
    bool zp_termFactor(int termIndex, qreal &factor) const;

    /*!
     \brief

     \param termIndex
     \return QList<int>
    */
    QList<int> zp_termWindowIndexList(int termIndex) const;
    /*!
     \brief

     \param termIndex
     \return QStringList
    */
    QStringList zp_termWindowsNameList(int termIndex) const;

    /*!
     \brief

     \param termIndex
     \return QString
    */
    QString zp_termFactorString(int termIndex) const;
    /*!
     \brief

     \param termIndex
     \param factorString
     \return bool
    */
    bool zp_termFactorString(int termIndex, QString &factorString) const;
    /*!
     \brief

     \param termIndex
     \param factorString
     \return bool
    */
    bool zp_setTermFactorString(int termIndex, const QString& factorString);
    /*!
     \brief

     \param termIndex
     \param spectrum
     \param value
     \return bool
    */
    bool zp_termVariablePart(int termIndex, const ZAbstractSpectrum* spectrum,  qreal &value) const;
    /*!
     \brief

     \param spectrum
     \param value
     \return bool
    */
    bool zp_calcBaseTermValue(const ZAbstractSpectrum* spectrum, qreal& value) const;
    /*!
     \brief

     \param termIndex
     \return bool
    */
    bool zp_createMixedTerms(int termIndex);
    /*!
     \brief

     \return bool
    */
    bool zp_removeMixedTerms();

    /*!
     \brief

     \param windowIndexList
     \param termType
     \param termState
     \param termFactor
     \return int
    */
    int zp_createTerm(QList<int>& windowIndexList,
                      ZAbstractTerm::TermType termType,
                      ZAbstractTerm::TermState termState,
                      const QString& termFactor);

    /*!
     \brief

     \param rawTerm
     \return int
    */
    int zp_createTerm(const ZRawTerm& rawTerm);

    // normalizer
    /*!
     \brief

     \return ZTermNormalizer::NormaType
    */
    ZTermNormalizer::NormaType zp_normaType() const;
    /*!
     \brief

     \param type
     \return bool
    */
    bool zp_setNormaType(ZTermNormalizer::NormaType type);
    /*!
     \brief

     \param customString
     \return bool
    */
    bool zp_setNormaCustomString(const QString& customString);
    /*!
     \brief

     \return QString
    */
    QString zp_normaCustomString() const;

    /*!
     \brief

     \return ZTermNormalizer::NormaType
    */
    ZTermNormalizer::NormaType zp_baseTermNormaType() const;
    /*!
     \brief

     \param type
     \return bool
    */
    bool zp_setBaseTermNormaType(ZTermNormalizer::NormaType type);
    /*!
     \brief

     \param customString
     \return bool
    */
    bool zp_setBaseTermNormaCustomString(const QString& customString);
    /*!
     \brief

     \return QString
    */
    QString zp_baseTermNormaCustomString() const;
    /*!
     \brief

     \param type
     \param customString
     \return bool
    */
    bool zp_setBaseTermNormalizerParameters(ZTermNormalizer::NormaType type,
                                  const QString& customString);


    /*!
     \brief

     \return EquationType
    */
    EquationType zp_equationType() const;
    /*!
     \brief

     \param type
     \return bool
    */
    bool zp_setEquationType(EquationType type);

    /*!
     \brief

     \return qreal
    */
    qreal zp_equationIntercept() const;
    /*!
     \brief

     \param qreal
     \return bool
    */
    bool zp_setEquationIntercept(qreal);

    /*!
     \brief

     \return QString
    */
    QString zp_equationInterceptString() const;
    /*!
     \brief

     \param
     \return bool
    */
    bool zp_setEquationInterceptString(const QString&);

    /*!
     \brief

     \return QString
    */
    QString zp_baseTermDisplayString() const;
    /*!
     \brief

     \return QString
    */
    QString zp_baseTermString() const;

    /*!
     \brief

     \return qint64
    */
    qint64 zp_baseTermId() const;
    /*!
     \brief

     \param id
     \return bool
    */
    bool zp_setBaseTermId(qint64 id);
    /*!
     \brief

     \return int
    */
    int zp_baseTermIndex() const;
    /*!
     \brief

     \param baseTermName
     \return bool
    */
    bool zp_setBaseTermFromName(const QString& baseTermName);

    /*!
     \brief

     \param QMap<int
     \param factorMap
     \param freeTermPtr
    */
    void zp_createEquationDataForEquationRecalc(QMap<int, qreal *> &factorMap,
                                                qreal *&freeTermPtr);

    /*!
     \brief

    */
    void zh_notifyCalibrationRecalc();
    /*!
     \brief

    */
    void zp_resetEquationTerms();
    /*!
     \brief

     \return ZCalibrationQualityData
    */
    ZCalibrationQualityData zp_calibrationQualityData() const;
    /*!
     \brief

     \param qualityData
    */
    void zp_setCalibrationQualityData(ZCalibrationQualityData qualityData);

    /*!
     \brief

     \param
    */
    void zp_setDetermination(const QString&);
    /*!
     \brief

     \param
    */
    void zp_setAdjustedDetermination(const QString&);
    /*!
     \brief

     \param
    */
    void zp_setStandardDeviation(const QString&);

    /*!
     \brief

    */
    void zp_setCurrentDateTime();
    /*!
     \brief

     \param QDateTime
    */
    void zp_setDateTime(QDateTime);

    // STATIC
    // VARS
    static const QString simplePolynomEquationString; /*!< TODO: describe */
    static const QString fractionalEquationString; /*!< TODO: describe */
    // FUNCS
    /*!
     \brief

     \param type
     \return QString
    */
    static QString zp_equationTypeString(ZCalibration::EquationType type);
    /*!
     \brief

     \param
     \return ZCalibration::EquationType
    */
    static ZCalibration::EquationType zp_equationTypeFromString(const QString&);
signals:

    /*!
     \brief

     \param QString
    */
    void zg_message(QString) const;
    /*!
     \brief

     \param ZCalibration::WindowOperationType
     \param first
     \param last
    */
    void zg_windowOperation(ZCalibration::WindowOperationType, int first, int last) const;
    /*!
     \brief

     \param bool
    */
    void zg_visibilityChanged(bool) const;
    /*!
     \brief

     \param bool
    */
    void zg_dirtyChanged(bool) const;
    /*!
     \brief

     \param ZCalibration::TremOperationType
     \param first
     \param last
    */
    void zg_termOperation(ZCalibration::TremOperationType, int first, int last)const;
    /*!
     \brief

    */
    void zg_normalizerChanged() const;
    /*!
     \brief

    */
    void zg_interceptChanged() const;

public slots:


private slots:

    /*!
     \brief

    */
    void zh_onTermNameChange();
    /*!
     \brief

    */
    void zh_onTermWindowMarginChange();
    /*!
     \brief

     \param
    */
    void zh_removeTerm(ZAbstractTerm*);
    /*!
     \brief

    */
    void zh_onNormalizerChange();

private:

    // VARS
    QString zv_baseName; /*!< TODO: describe */
    QString zv_suffix; /*!< TODO: describe */
    QString zv_path; /*!< TODO: describe */
    QString zv_chemElement; /*!< TODO: describe */
    QColor zv_color; /*!< TODO: describe */
    qint64 zv_calibrationId; /*!< TODO: describe */
    ZCalibrationQualityData zv_calibrationQualityData; /*!< TODO: describe */
    bool zv_visibility; /*!< TODO: describe */
    bool zv_dirty; /*!< TODO: describe */

    QList<ZCalibrationWindow*> zv_windowList; /*!< TODO: describe */
    QList<ZAbstractTerm*> zv_termList; /*!< TODO: describe */
    EquationType zv_equationType; /*!< TODO: describe */
    ZTermNormalizer* zv_termNormalizer; /*!< TODO: describe */
    QString zv_interceptString; /*!< TODO: describe */
    qreal zv_intercept; /*!< TODO: describe */
    qint64 zv_baseTermId; /*!< TODO: describe */
    // qint64 zv_baseTermNormaId;
    ZTermNormalizer* zv_baseTermNormalizer; /*!< TODO: describe */
    bool zv_useBaseTermInFractionalEquation; /*!< TODO: describe */
    QDateTime zv_dateTime; /*!< TODO: describe */

    // FUNCS
    /*!
     \brief

     \param
     \return bool
    */
    bool zh_isWindowExist(const QString&);
    /*!
     \brief

     \param
    */
    void zh_createTermsForWindow(const ZCalibrationWindow*);
    /*!
     \brief

     \param
     \param ZAbstractTerm::TermType
     \return bool
    */
    bool zh_windowHasTerms(const ZCalibrationWindow*,
                           ZAbstractTerm::TermType) const;
    /*!
     \brief

     \param
     \return int
    */
    int zh_termIndex(const ZAbstractTerm*) const;
    /*!
     \brief

    */
    void zh_chopTailZeroesFromInterceptString();
    /*!
     \brief

     \param termIndex
     \return qreal
    */
    qreal* zh_termFactorPointer(int termIndex) const;
    /*!
     \brief

     \param ZAbstractTerm::TermType
     \return int
    */
    int zh_findNextTermIndex(ZAbstractTerm::TermType) const;

    // STATIC
    // VARS
    static QList<QColor> zv_colorList; /*!< TODO: describe */
    static qint64 zv_lastCalibrationId; /*!< TODO: describe */
    static int zv_lastColorIndex; /*!< TODO: describe */
    static int zv_precision; /*!< TODO: describe */
    static const QString zv_defaultWindowName; /*!< TODO: describe */

    static QMap<ZCalibration::EquationType, QString> zv_eqationTypeStringMap; /*!< TODO: describe */
    static bool zv_useBaseTermInFractionalEquationByDefault; /*!< TODO: describe */
    //  FUNCS
    /*!
     \brief

     \return QList<QColor>
    */
    static QList<QColor> zp_createColorList();
    /*!
     \brief

     \param color
     \return bool
    */
    static bool checkColor(QColor color);
    /*!
     \brief

     \return QMap<ZCalibration::EquationType, QString>
    */
    static QMap<ZCalibration::EquationType, QString> zh_initEquationTypeStringMap();
    /*!
     \brief

     \return QString
    */
    static QString zh_initPlynomialEquationString();
    /*!
     \brief

     \return QString
    */
    static QString zh_initFractionalEquationString();
    /*!
     \brief

     \return QString
    */
    static QString zh_initFractionalEquationString1();

};
//=========================================================
#endif // QABSTRACTCALIBRATION_H
