//============================================================
#ifndef ZABSTRACTTERM_H
#define ZABSTRACTTERM_H
//============================================================
#include <QObject>
#include <QMap>
#include "ZTermNormalizer.h"
//============================================================
class ZAbstractSpectrum;
class ZCalibrationWindow;
class ZTermNormalizer;
class ZCalibration;
class ZSpectrumArray;
//============================================================
/*!
 \brief

*/
class ZAbstractTerm : public QObject
{
    Q_OBJECT
    friend  class ZCalibration;
public:
    /*!
     \brief

     \param
    */
    explicit ZAbstractTerm(ZCalibration*);

    // VARS
    /*!
     \brief

    */
    enum TermType {TT_NOT_DEFINED,
                   TT_SIMPLE,
                   TT_QUADRATIC,
                   TT_MIXED,
                   TT_CUSTOM};

    /*!
     \brief

    */
    enum TermState {TS_NOT_DEFINED,
                    TS_CONST_INCLUDED,
                    TS_EXAM_WAITING,
                    TS_INCLUDED,
                    TS_EXCEPTED,
                    TS_CONST_EXCLUDED,
                    TS_BASE};


    // FUNCS
    /*!
     \brief

     \param
     \param
     \return bool
    */
    virtual bool zp_calcValue(const ZAbstractSpectrum*, qreal&) = 0;
    /*!
     \brief

     \param
     \param value
     \return bool
    */
    virtual bool zp_calcTermVariablePart(const ZAbstractSpectrum*, qreal &value) = 0; // w/o factor
    /*!
     \brief

     \return QString
    */
    QString zp_termName() const;
    /*!
     \brief

     \return ZAbstractTerm::TermType
    */
    ZAbstractTerm::TermType zp_termType() const;
    /*!
     \brief

     \param
     \return bool
    */
    virtual bool zp_termBelongsToWindow(const ZCalibrationWindow*) const = 0;
    /*!
     \brief

     \return QList<qint64>
    */
    virtual QList<qint64> zp_termWindowIdList() const = 0;

    /*!
     \brief

     \return qint64
    */
    qint64 zp_termId();
    /*!
     \brief

     \return qreal
    */
    qreal zp_termFactor() const;
    /*!
     \brief

     \return QString
    */
    QString zp_termFactorString() const;

    /*!
     \brief

     \return ZAbstractTerm::TermState
    */
    ZAbstractTerm::TermState zp_termState() const;
    /*!
     \brief

     \param ZAbstractTerm::TermState
     \return bool
    */
    bool zp_setTermState(ZAbstractTerm::TermState);

    // STATIC
    /*!
     \brief

     \param int
     \return bool
    */
    static bool zp_setPrecision(int);
    /*!
     \brief

     \param ZAbstractTerm::TermType
     \return QString
    */
    static QString zp_termTypeName(ZAbstractTerm::TermType);
    /*!
     \brief

     \param ZAbstractTerm::TermState
     \return QString
    */
    static QString zp_termStateName(ZAbstractTerm::TermState);
    /*!
     \brief

     \param
     \return ZAbstractTerm::TermType
    */
    static ZAbstractTerm::TermType zp_termTypeFromString(const QString&);
    /*!
     \brief

     \param
     \return ZAbstractTerm::TermState
    */
    static ZAbstractTerm::TermState zp_termStateFromString(const QString&);


signals:

 //   void zg_requestNormalizerValue(qreal&);
    /*!
     \brief

    */
    void zg_termWindowMarginChanged();
    /*!
     \brief

    */
    void zg_termNameChanged() const;
    /*!
     \brief

     \param
    */
    void zg_requestForDelete(ZAbstractTerm*);
    /*!
     \brief

     \param ZAbstractTerm::TermState
    */
    void zg_termStateChanged(ZAbstractTerm::TermState) const;
   // void zg_termValuesChanged() const;

public slots:


protected slots:

    /*!
     \brief

    */
    void zh_normalizerChanged();
    /*!
     \brief

    */
    void zh_onWindowDestroying();

protected slots:

    /*!
     \brief

    */
    virtual void zh_updateTermNameForWindowName() = 0;
    /*!
     \brief

     \param previousType
     \param currentType
    */
    virtual void zh_onWindowTypeChange(ZCalibrationWindow::WindowType previousType, ZCalibrationWindow::WindowType currentType) = 0;

protected:

    // VARS
    qint64 zv_termId; /*!< TODO: describe */
    TermType zv_type; /*!< TODO: describe */
    QString zv_termFactorString; /*!< TODO: describe */
    qreal zv_termFactor; /*!< TODO: describe */
    TermState zv_termState; /*!< TODO: describe */
    QString zv_name; /*!< TODO: describe */
    //    QList<qint64> zv_unnormalizedValueList;
    //    qreal zv_averageValue;
    // FUNCS
    //bool zh_setTermFactor(qreal factor);
    /*!
     \brief

     \param
    */
    void zh_connectToNormalizer(ZTermNormalizer *);
    /*!
     \brief

     \param calibration
    */
    void zh_connectToCalibration(ZCalibration* calibration);
    /*!
     \brief

    */
    void zh_chopTailZeroesFromTermFactorString();
    /*!
     \brief

     \return qreal
    */
    qreal* zh_termFactorPointer();

    /*!
     \brief

     \param
     \return bool
    */
    bool zh_setTermFactor(const QString&);
    /*!
     \brief

     \param qreal
     \return bool
    */
    bool zh_setTermFactor(qreal);
    /*!
     \brief

    */
    void zh_conformStringWithValue();

    // STATIC
    // VARS
    static qint64 zv_lastTermId; /*!< TODO: describe */
    static int zv_precision; /*!< TODO: describe */

    static QMap<ZAbstractTerm::TermType, QString> zv_typeNameMap; /*!< TODO: describe */
    static QMap<ZAbstractTerm::TermState, QString> zv_stateNameMap; /*!< TODO: describe */

    // FUNCS
    /*!
     \brief

     \return QMap<ZAbstractTerm::TermType, QString>
    */
    static QMap<ZAbstractTerm::TermType, QString> zh_intTypeNameMap();
    /*!
     \brief

     \return QMap<ZAbstractTerm::TermState, QString>
    */
    static QMap<ZAbstractTerm::TermState, QString>zh_intStateNameMap();


};
//============================================================
#endif // ZABSTRACTTERM_H
