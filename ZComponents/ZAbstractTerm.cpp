//============================================================
#include "ZAbstractTerm.h"
#include "ZGLConstantsAndDefines.h"
#include "ZCalibrationWindow.h"
#include "ZCalibration.h"
#include <QPointer>
//============================================================
// STATIC
qint64 ZAbstractTerm::zv_lastTermId = 0;
int ZAbstractTerm::zv_precision = 15;
QMap<ZAbstractTerm::TermType, QString> ZAbstractTerm::zv_typeNameMap = ZAbstractTerm::zh_intTypeNameMap();
QMap<ZAbstractTerm::TermState, QString> ZAbstractTerm::zv_stateNameMap = ZAbstractTerm::zh_intStateNameMap();
//============================================================
QMap<ZAbstractTerm::TermType, QString> ZAbstractTerm::zh_intTypeNameMap()
{
    QMap<ZAbstractTerm::TermType, QString> map;
    map.insert(TT_NOT_DEFINED, "Not defined");
    map.insert(TT_SIMPLE, "Simple");
    map.insert(TT_QUADRATIC, "Quadratic");
    map.insert(TT_MIXED, "Mixed");
    map.insert(TT_CUSTOM, "Custom");

    return map;
}
//============================================================
QMap<ZAbstractTerm::TermState, QString> ZAbstractTerm::zh_intStateNameMap()
{
    QMap<ZAbstractTerm::TermState, QString> map;
    map.insert(TS_NOT_DEFINED, "Not defined");
    map.insert(TS_CONST_INCLUDED, "Constantly Included");
    map.insert(TS_CONST_EXCLUDED, "Constantly Excluded");
    map.insert(TS_EXAM_WAITING, "Exam Waiting");
    map.insert(TS_EXCEPTED, "Excepted");
    map.insert(TS_INCLUDED, "Included");
    map.insert(TS_BASE, "Base");

    return map;
}
//============================================================
QString ZAbstractTerm::zp_termTypeName(ZAbstractTerm::TermType termType)
{
    if(!zv_typeNameMap.keys().contains(termType))
    {
        return QString();
    }

    return zv_typeNameMap.value(termType);
}
//============================================================
QString ZAbstractTerm::zp_termStateName(ZAbstractTerm::TermState termState)
{
    if(!zv_stateNameMap.keys().contains(termState))
    {
        return QString();
    }

    return zv_stateNameMap.value(termState);
}
//============================================================
ZAbstractTerm::TermType ZAbstractTerm::zp_termTypeFromString(const QString& typeString)
{
    if(!zv_typeNameMap.values().contains(typeString))
    {
        return ZAbstractTerm::TT_NOT_DEFINED;
    }

    return zv_typeNameMap.key(typeString);
}
//============================================================
ZAbstractTerm::TermState ZAbstractTerm::zp_termStateFromString(const QString& stateString)
{
    if(!zv_stateNameMap.values().contains(stateString))
    {
        return ZAbstractTerm::TS_NOT_DEFINED;
    }

    return zv_stateNameMap.key(stateString);
}
//============================================================
// END STATIC
//============================================================
ZAbstractTerm::ZAbstractTerm(ZCalibration *parent) : QObject(parent)
{
    zh_connectToCalibration(parent);
    zv_type = TT_NOT_DEFINED;
    zv_termState = TS_CONST_EXCLUDED;
    // zp_setTermFactor(0.0); // zv_termFactorString = 0;
    zh_setTermFactor("0.0");
    zv_termId = zv_lastTermId++;
}
//============================================================
qint64 ZAbstractTerm::zp_termId()
{
    return zv_termId;
}
//============================================================
ZAbstractTerm::TermType ZAbstractTerm::zp_termType() const
{
    return zv_type;
}
//============================================================
ZAbstractTerm::TermState ZAbstractTerm::zp_termState() const
{
    return zv_termState;
}
//============================================================
bool ZAbstractTerm::zp_setTermState(ZAbstractTerm::TermState state)
{
    if(zv_termState == state)
    {
        return false;
    }

    zv_termState = state;
    emit zg_termStateChanged(zv_termState);
    return true;
}
//============================================================
void ZAbstractTerm::zh_conformStringWithValue()
{
    if(qAbs(zv_termFactor) < 1)
    {
        bool started = false;
        zv_termFactorString = QString::number(zv_termFactor, 'f', zv_precision);
        // first zero count after point
        int firstZeroCount = 0;
        for(int i = 0; i < zv_termFactorString.count(); i++)
        {
            if(!started && (zv_termFactorString.at(i) == '.' || zv_termFactorString.at(i) == ','))
            {
                started = true;
                continue;
            }

            if(zv_termFactorString.at(i) == '0')
            {
                firstZeroCount++;
                continue;
            }

            break;
        }

        if(firstZeroCount > 9)
        {
            zv_termFactorString = QString::number(zv_termFactor, 'g', zv_precision);
        }
        else
        {
            // chop tail zero if they exist
            for(int i = zv_termFactorString.count()-1; i > 0; i--)
            {
                if(zv_termFactorString.at(i) == '0' && (zv_termFactorString.at(i - 1) != '.' || zv_termFactorString.at(i - 1) != ','))
                {
                    zv_termFactorString.remove(i, 1);
                    continue;
                }
                break;
            }
        }
    }
    else
    {
        zv_termFactorString = QString::number(zv_termFactor, 'g', zv_precision);
    }
}
//============================================================
bool ZAbstractTerm::zp_setPrecision(int precision)
{
    if(precision < 0 )
    {
        return false;
    }
    zv_precision = precision;
    return true;
}
//============================================================
QString ZAbstractTerm::zp_termName() const
{
    return zv_name;
}
//============================================================
qreal ZAbstractTerm::zp_termFactor() const
{
    return zv_termFactor;
}
//============================================================
qreal* ZAbstractTerm::zh_termFactorPointer()
{
    return &zv_termFactor;
}
//============================================================
QString ZAbstractTerm::zp_termFactorString() const
{
    return zv_termFactorString;
}
//============================================================
bool ZAbstractTerm::zh_setTermFactor(qreal factor)
{
    if(zv_termFactor - factor == 0.0)
    {
        return false;
    }

    zv_termFactor = factor;
    zh_conformStringWithValue();
//    zv_termFactorString = QString::number(factor, 'f', 15);
//    zh_chopTailZeroesFromTermFactorString();
    return true;
}
//============================================================
bool ZAbstractTerm::zh_setTermFactor(const QString& termFactorString)
{
    bool ok;
    qreal termFactor = termFactorString.toDouble(&ok);
    if(!ok)
    {
        return false;
    }

    zv_termFactorString = termFactorString;
    zv_termFactor = termFactor;
    return true;
}
//============================================================
void ZAbstractTerm::zh_chopTailZeroesFromTermFactorString()
{
    for(int i = zv_termFactorString.count() - 1; i > 0; i--)
    {
        if(zv_termFactorString.at(i) != '0' || (zv_termFactorString.at(i - 1) == '.' || zv_termFactorString.at(i - 1) == ',' ))
        {
            break;
        }
        zv_termFactorString.chop(1);
    }
}
//============================================================
void ZAbstractTerm::zh_connectToNormalizer(ZTermNormalizer* normalizer)
{
    connect(normalizer, &ZTermNormalizer::zg_normalizerChanged,
            this, &ZAbstractTerm::zh_normalizerChanged);
    //    connect(this, &ZAbstractTerm::zg_requestIsNormalizerValid,
    //            normalizer, &ZTermNormalizer::zp_isValid);
    //    connect(this, &ZAbstractTerm::zg_requestNormalizerValue,
    //            normalizer, &ZTermNormalizer::zp_value);

}
//============================================================
void ZAbstractTerm::zh_connectToCalibration(ZCalibration* calibration)
{
    connect(this, &ZAbstractTerm::zg_requestForDelete,
            calibration, &ZCalibration::zh_removeTerm);

    connect(calibration, &ZCalibration::zg_normalizerChanged,
            this, &ZAbstractTerm::zh_normalizerChanged);
    //    connect(this, &ZAbstractTerm::zg_requestNormalizerValue,
    //            calibration, &ZCalibration::zh_normalizerValue);
    connect(this, &ZAbstractTerm::zg_termNameChanged,
            calibration, &ZCalibration::zh_onTermNameChange);

    connect(this, &ZAbstractTerm::zg_termWindowMarginChanged,
            calibration, &ZCalibration::zh_onTermWindowMarginChange);
    //    connect(this, &ZAbstractTerm::zg_termValuesChanged,
    //            calibration, &ZCalibration::zp_onTermAverageValueChange);

}
//============================================================
void ZAbstractTerm::zh_normalizerChanged()
{
    emit zg_termNameChanged();
}
//============================================================
void ZAbstractTerm::zh_onWindowDestroying()
{
    emit zg_requestForDelete(this);
}
//============================================================
