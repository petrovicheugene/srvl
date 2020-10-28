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
class ZAbstractTerm : public QObject
{
    Q_OBJECT
    friend  class ZCalibration;
public:
    explicit ZAbstractTerm(ZCalibration* calibration);

    // VARS
    enum TermType {TT_NOT_DEFINED,
                   TT_SIMPLE,
                   TT_QUADRATIC,
                   TT_MIXED,
                   TT_CUSTOM};

    enum TermState {TS_NOT_DEFINED,
                    TS_CONST_INCLUDED,
                    TS_EXAM_WAITING,
                    TS_INCLUDED,
                    TS_EXCEPTED,
                    TS_CONST_EXCLUDED,
                    TS_BASE};


    // FUNCS
    virtual bool zp_calcValue(const ZAbstractSpectrum*, qreal&) = 0;
    virtual bool zp_calcTermVariablePart(const ZAbstractSpectrum*, qreal &value) = 0; // w/o factor
    QString zp_termName() const;
    ZAbstractTerm::TermType zp_termType() const;
    virtual bool zp_termBelongsToWindow(const ZCalibrationWindow*) const = 0;
    virtual QList<qint64> zp_termWindowIdList() const = 0;

    qint64 zp_termId();
    qreal zp_termFactor() const;
    QString zp_termFactorString() const;

    ZAbstractTerm::TermState zp_termState() const;
    bool zp_setTermState(ZAbstractTerm::TermState);
    void zp_setWindowMap(const QMap<QString, int>& windowMap);

    // STATIC
    static bool zp_setPrecision(int);
    static QString zp_termTypeName(ZAbstractTerm::TermType);
    static QString zp_termStateName(ZAbstractTerm::TermState);
    static ZAbstractTerm::TermType zp_termTypeFromString(const QString&);
    static ZAbstractTerm::TermState zp_termStateFromString(const QString&);

signals:

 //   void zg_requestNormalizerValue(qreal&);
    void zg_termWindowMarginChanged();
    void zg_termNameChanged() const;
    void zg_requestForDelete(ZAbstractTerm*);
    void zg_termStateChanged(ZAbstractTerm::TermState) const;
   // void zg_termValuesChanged() const;

public slots:


protected slots:

    void zh_normalizerChanged();
    void zh_onWindowDestroying();

protected slots:

    virtual void zh_updateTermNameForWindowName() = 0;
    virtual void zh_onWindowTypeChange(ZCalibrationWindow::WindowType previousType, ZCalibrationWindow::WindowType currentType) = 0;

protected:

    // VARS
    qint64 zv_termId;
    TermType zv_type;
    QString zv_termFactorString;
    qreal zv_termFactor;
    TermState zv_termState;
    QString zv_name;

    //    QList<qint64> zv_unnormalizedValueList;
    //    qreal zv_averageValue;
    // FUNCS
    //bool zh_setTermFactor(qreal factor);
    void zh_connectToNormalizer(ZTermNormalizer *);
    void zh_connectToCalibration(ZCalibration* calibration);
    void zh_chopTailZeroesFromTermFactorString();
    qreal* zh_termFactorPointer();

    bool zh_setTermFactor(const QString&);
    bool zh_setTermFactor(qreal);
    void zh_conformStringWithValue();

    // STATIC
    // VARS
    static qint64 zv_lastTermId;
    static int zv_precision;

    static QMap<ZAbstractTerm::TermType, QString> zv_typeNameMap;
    static QMap<ZAbstractTerm::TermState, QString> zv_stateNameMap;

    // FUNCS
    static QMap<ZAbstractTerm::TermType, QString> zh_intTypeNameMap();
    static QMap<ZAbstractTerm::TermState, QString>zh_intStateNameMap();


};
//============================================================
#endif // ZABSTRACTTERM_H
