//=========================================================
#include "ZCalibration.h"
#include "ZAbstractSpectrum.h"
#include "ZSimpleTerm.h"
#include "ZQuadraticTerm.h"
#include "ZMixedTerm.h"
#include "ZCustomTerm.h"
#include "ZEquationSettingsData.h"
#include "ZTermNormalizer.h"
#include "ZGeneral.h"
#include "ZQrealToStringConverter.h"

#include <QFileInfo>
#include <QDir>
#include <QPointer>
//=========================================================
// STATIC
//=========================================================
const QString ZCalibration::simplePolynomEquationString(ZCalibration::zh_initPlynomialEquationString());
const QString ZCalibration::fractionalEquationString(ZCalibration::zh_initFractionalEquationString1());
const QString ZCalibration::zv_defaultWindowName(QObject::tr("Peak #"));

bool ZCalibration::zv_useBaseTermInFractionalEquationByDefault = false;
QList<QColor> ZCalibration::zv_colorList = ZCalibration::zp_createColorList();
qint64 ZCalibration::zv_lastCalibrationId = 0;
int ZCalibration::zv_lastColorIndex = 0;
int ZCalibration::zv_precision = 15;
QMap<ZCalibration::EquationType, QString> ZCalibration::zv_eqationTypeStringMap =
        ZCalibration::zh_initEquationTypeStringMap();
//=========================================================
QMap<ZCalibration::EquationType, QString> ZCalibration::zh_initEquationTypeStringMap()
{
    QMap<ZCalibration::EquationType, QString> map;
    map.insert(ET_NOT_DEFINED, tr("Not defined"));
    map.insert(ET_POLYNOMIAL, tr("Polynomial"));
    map.insert(ET_FRACTIONAL, tr("Fractional"));
    return map;
}
//=========================================================
QString ZCalibration::zh_initPlynomialEquationString()
{
    QString simplePolynomEquationString = "<table border=0 cellspacing=0 cellpadding=0>"
                                          "<tr>"
                                          "<td></td>"
                                          "<td align=center><sub><i>n</i></sub></td>"
                                          "<td></td>"
                                          "</tr>"

                                          "<tr>"
                                          "<td valign=middle>C=</td>"
                                          "<td valign=middle align=center>\u2211</td>"
                                          "<td valign=middle>(a<sub><i>i</i></sub>*X<sub><i>i</i></sub>) + a<sub>0</sub></td>"
                                          "</tr>"

                                          "<tr>"
                                          "<td></td>"
                                          "<td align=center><sup><i>i</i> = 1</sup></td>"
                                          "<td></td>"
                                          "</tr>"
                                          "</table>";
    return simplePolynomEquationString;
}
//=========================================================
QString ZCalibration::zh_initFractionalEquationString()
{
    QString fractionalEquationString = "<table border=0 cellspacing=0 cellpadding=0>"
                                       "<tr>"
                                       "<td></td>"
                                       "<td></td>"
                                       "<td>a<sub><i>base</i></sub>*X<sub><i>base</i></sub></td>"
                                       "</tr>"

                                       "<tr>"
                                       "<td>C= </td>"
                                       "<td><hr></td>"
                                       "<td><hr></td>"
                                       "</tr>"

                                       "<tr>"
                                       "<td></td>"
                                       "<td align=center><sub><i>n</i></sub></td>"
                                       "<td></td>"
                                       "<td></td>"

                                       "<tr>"
                                       "<td></td>"
                                       "<td valign=middle align=center>\u2211</td>"
                                       "<td valign=middle>(a<sub><i>i</i></sub>*X<sub><i>i</i></sub>) + a<sub>0</sub></td>"
                                       "</tr>"

                                       "<tr>"
                                       "<td></td>"
                                       "<td align=center><sup><i>i</i> = 1</sup></td>"
                                       "<td></td>"
                                       "</tr>"
                                       "</table>";
    return fractionalEquationString;
}
//=========================================================
QString ZCalibration::zh_initFractionalEquationString1()
{
    QString fractionalEquationString = "<table border=0 cellspacing=0 cellpadding=0>"
                                       "<tr>"
                                       "<td></td>"
                                       "<td align=center><sub><i>n</i></sub></td>"
                                       "<td></td>"
                                       "</tr>"

                                       "<tr>"
                                       "<td valign=middle>C= a<sub><i>base</i></sub>*X<sub><i>base</i></sub> / (</td>"
                                       "<td valign=middle align=center>\u2211</td>"
                                       "<td valign=middle>(a<sub><i>i</i></sub>*X<sub><i>i</i></sub>) + a<sub>0</sub>)</td>"
                                       "</tr>"

                                       "<tr>"
                                       "<td></td>"
                                       "<td align=center><sup><i>i</i> = 1</sup></td>"
                                       "<td></td>"
                                       "</tr>"
                                       "</table>";
    return fractionalEquationString;
}
//=========================================================
QString ZCalibration::zp_equationTypeString(ZCalibration::EquationType type)
{
    return zv_eqationTypeStringMap.value(type);
}
//=========================================================
ZCalibration::EquationType ZCalibration::zp_equationTypeFromString(const QString& typestring)
{
    if(!zv_eqationTypeStringMap.values().contains(typestring))
    {
        return ZCalibration::ET_NOT_DEFINED;
    }

    return zv_eqationTypeStringMap.key(typestring);
}
// END STATIC
//=========================================================
//=========================================================
ZCalibration::ZCalibration(const QString& name, QObject *parent) : QObject(parent)
{
    zv_visibility = false;
    zv_useBaseTermInFractionalEquation = zv_useBaseTermInFractionalEquationByDefault;

    QFileInfo fileInfo(name);
    if(fileInfo.exists())
    {

        zv_baseName = fileInfo.baseName();
        zv_suffix = fileInfo.completeSuffix();
        zv_path = fileInfo.absolutePath();
        zv_dirty = false;
    }
    else
    {
        zv_path = QString();
        zv_suffix = "clbx";
        zv_baseName = name;
        zv_dirty = true;
    }

    zv_color = zv_colorList.value(zv_lastColorIndex++);
    if(zv_lastColorIndex >= zv_colorList.count())
    {
        zv_lastColorIndex = 0;
    }

    zv_exposition = 0;
//    zv_energyCalibrationFactorK0 = 0;
//    zv_energyCalibrationFactorK1 = 0;
//    zv_energyCalibrationFactorK2 = 0;
    zv_gainFactor = 0;
//    zv_energyUnit = "not defined";

    zv_calibrationId = zv_lastCalibrationId++;
    zv_chemElement = glDefaultChemElementString;
    zv_termNormalizer  = new ZTermNormalizer(this);
    zv_baseTermNormalizer = new ZTermNormalizer(this);


    zv_equationType = ET_POLYNOMIAL;
    zp_setEquationIntercept(0.0);
    zv_baseTermId = -1;
    zv_dateTime = QDateTime::currentDateTime();
}
//=========================================================
ZCalibration::ZCalibration(const ZCalibration *calibration, const QString& name, QObject *parent)  : QObject(parent)
{
    zv_visibility = false;
    zv_useBaseTermInFractionalEquation = calibration->zv_useBaseTermInFractionalEquationByDefault;

    zv_path = calibration->zv_path;
    zv_suffix = calibration->zv_suffix;
    zv_baseName = name;
    zv_dirty = true;

    zv_color = zv_colorList.value(zv_lastColorIndex++);
    if(zv_lastColorIndex >= zv_colorList.count())
    {
        zv_lastColorIndex = 0;
    }

    zv_exposition = calibration->zv_exposition;
//    zv_energyCalibrationFactorK0 =  calibration->zv_energyCalibrationFactorK0;
//    zv_energyCalibrationFactorK1 = calibration->zv_energyCalibrationFactorK1;
//    zv_energyCalibrationFactorK2 = calibration->zv_energyCalibrationFactorK2;
    zv_gainFactor = calibration->zv_gainFactor;
//    zv_energyUnit = calibration->zv_energyUnit;

    zv_calibrationId = zv_lastCalibrationId++;
    zv_chemElement = calibration->zv_chemElement;

    zv_termNormalizer  = new ZTermNormalizer(this);
    zv_baseTermNormalizer = new ZTermNormalizer(this);

    zv_equationType = calibration->zv_equationType;

    zp_setEquationIntercept(0.0);
    zv_baseTermId = -1;
    zv_dateTime = QDateTime::currentDateTime();
}
//=========================================================
ZCalibration::~ZCalibration()
{

}
//=========================================================
QDateTime ZCalibration::zp_dateTime() const
{
    return zv_dateTime;
}
//=========================================================
QString ZCalibration::zp_baseName() const
{
    return zv_baseName;
}
//=========================================================
void ZCalibration::zp_setBaseName(const QString& name)
{
    if(name == zv_baseName || name.isEmpty())
    {
        return;
    }

    zv_baseName = name;
    zv_dirty = true;
    zv_dateTime = QDateTime::currentDateTime();
    emit zg_dirtyChanged(zv_dirty);
}
//=========================================================
QString ZCalibration::zp_fileName() const
{
    return zv_baseName + "." + zv_suffix;
}
//=========================================================
QString ZCalibration::zp_suffix() const
{
    return zv_suffix;
}
//=========================================================
void ZCalibration::zp_setName(const QString& name)
{
    QFileInfo fileInfo(QDir(zv_path), name);
    zv_suffix = fileInfo.completeSuffix();
    zp_setBaseName(fileInfo.baseName());
    zv_dirty = true;
    zv_dateTime = QDateTime::currentDateTime();
    emit zg_dirtyChanged(zv_dirty);
}
//=========================================================
QString ZCalibration::zp_path() const
{
    return zv_path;
}
//=========================================================
void ZCalibration::zp_setPath(const QString& path)
{
    if(path == zv_path || path.isEmpty())
    {
        return;
    }
    QFileInfo fileInfo(path);
    if(fileInfo.absoluteFilePath().isEmpty())
    {
        return;
    }

    zv_path = fileInfo.absolutePath();

    if(fileInfo.isFile())
    {
        zv_baseName = fileInfo.baseName();
        zv_suffix = fileInfo.completeSuffix();
    }

    zv_dirty = true;
    zv_dateTime = QDateTime::currentDateTime();
    emit zg_dirtyChanged(zv_dirty);
}
//=========================================================
QString ZCalibration::zp_chemElement() const
{
    return zv_chemElement;
}
//=========================================================
bool ZCalibration::zp_setChemElement(const QString& chemElement)
{
    if(chemElement.isEmpty())
    {
        return false;
    }

    zv_chemElement = chemElement;
    zv_dirty = true;
    zv_dateTime = QDateTime::currentDateTime();
    emit zg_dirtyChanged(zv_dirty);

    return true;
}
//=========================================================
//void ZCalibration::zp_setEnergyCalibration(qreal K0, qreal K1, qreal K2, const QString& energyUnit)
//{
//    zv_energyCalibrationFactorK0 = K0;
//    zv_energyCalibrationFactorK1 = K1;
//    zv_energyCalibrationFactorK2 = K2;
//    zv_energyUnit = energyUnit;

//}
//=========================================================
//void ZCalibration::zp_setEnergyCalibrationK0(qreal K0)
//{
//    zv_energyCalibrationFactorK0 = K0;
//}
////=========================================================
//void ZCalibration::zp_setEnergyCalibrationK1(qreal K1)
//{
//    zv_energyCalibrationFactorK1 = K1;
//}
////=========================================================
//void ZCalibration::zp_setEnergyCalibrationK2(qreal K2)
//{
//    zv_energyCalibrationFactorK2 = K2;
//}
////=========================================================
//void ZCalibration::zp_setEnergyUnit(const QString& energyUnit)
//{
//    zv_energyUnit = energyUnit;
//}
//=========================================================
void ZCalibration::zp_setGainFactor(int gainFactor)
{
    zv_gainFactor = gainFactor;
}
//=========================================================
void ZCalibration::zp_setExposition(int exposition)
{
    zv_exposition = exposition;
}
//=========================================================
//qreal ZCalibration::zp_energyCalibrationK0() const
//{
//    return zv_energyCalibrationFactorK0;
//}
////=========================================================
//qreal ZCalibration::zp_energyCalibrationK1() const
//{
//    return zv_energyCalibrationFactorK1;
//}
////=========================================================
//qreal ZCalibration::zp_energyCalibrationK2() const
//{
//    return zv_energyCalibrationFactorK2;
//}
////=========================================================
//QString ZCalibration::zp_energyUnit() const
//{
//    return zv_energyUnit;
//}
//=========================================================
int ZCalibration::zp_gainFactor() const
{
    return zv_gainFactor;
}
//=========================================================
int ZCalibration::zp_exposition() const
{
    return zv_exposition;
}
//=========================================================
bool ZCalibration::zp_isVisible() const
{
    return zv_visibility;
}
//=========================================================
void ZCalibration::zp_setVisible(bool visibility)
{
    if(visibility != zv_visibility)
    {
        zv_visibility = visibility;
        emit zg_visibilityChanged(zv_visibility);
    }
}
//=========================================================
bool ZCalibration::zp_isDirty() const
{
    return zv_dirty;
}
//=========================================================
void ZCalibration::zp_setDirty(bool dirty)
{
    zv_dirty = dirty;
    emit zg_dirtyChanged(zv_dirty);
}
//=========================================================
QColor ZCalibration::zp_color() const
{
    return zv_color;
}
//=========================================================
qint64 ZCalibration::zp_calibrationId() const
{
    return zv_calibrationId;
}
//=========================================================
bool ZCalibration::zp_calcConcentration(const ZAbstractSpectrum* const spectrum, qreal& concentration)
{
    concentration = 0;
    if(spectrum == nullptr || zv_equationType == ET_NOT_DEFINED)
    {
        return false;
    }

    // calc norma
    if(!zv_termNormalizer->zp_calcAndSetNormaValue(spectrum))
    {
        return false;
    }

    // calc polynom
    qreal value;
    for(int t = 0; t < zv_termList.count(); t++)
    {
        // exclude base term if !zv_useBaseTermInFractionalEquation
        if(zv_equationType == ET_FRACTIONAL
                && !zv_useBaseTermInFractionalEquation
                && zv_termList.at(t)->zp_termId() == zv_baseTermId)
        {
            continue;
        }

        // calc all terms
        if(!zv_termList.at(t)->zp_calcValue(spectrum, value))
        {
            return false;
        }

        // normalize terms (zv_termNormalizer defines nesessary)
        if(!zv_termNormalizer->zp_normalizeValue(value))
        {
            return false;
        }
        concentration += value;
    }

    concentration += zv_intercept;

    if(zv_equationType == ET_FRACTIONAL)
    {
        // concentration goes to denominator. check out it
        if(concentration == 0.0)
        {
            return false;
        }

        // calc nominator
        if(!zp_calcBaseTermValue(spectrum, value))
        {
            return false;
        }

        concentration = value / concentration;
    } // end ET_FRACTIONAL

    return true;
}
//=========================================================
int ZCalibration::zp_createNewCalibrationWindow(int firstChannel, int lastChannel,
                                                ZCalibrationWindow::WindowType windowType)
{
    int windowNewIndex = -1;
    int maxWindowNumber = 1;
    for(int i = 0; i < zv_windowList.count(); i++)
    {
        QString name = zv_windowList.at(i)->zp_windowName();

        QString digitalPart = name.mid(zv_defaultWindowName.count());
        bool ok;
        int number = digitalPart.toInt(&ok);
        if(!ok)
        {
            continue;
        }
        if(number >= maxWindowNumber)
        {
            maxWindowNumber = ++number;
        }
    }
    QString nextName = zv_defaultWindowName + QString::number(maxWindowNumber);

    windowNewIndex = zv_windowList.count();
    ZCalibrationWindow* calibrationWindow = new ZCalibrationWindow(nextName,
                                                                   windowType,
                                                                   firstChannel,
                                                                   lastChannel,
                                                                   this);

    zv_termNormalizer->zp_connectToWindow(calibrationWindow);
    zv_baseTermNormalizer->zp_connectToWindow(calibrationWindow);

    emit zg_windowOperation(WOT_BRGIN_INSERT_WINDOWS, windowNewIndex, windowNewIndex);
    zv_windowList.append(calibrationWindow);
    emit zg_windowOperation(WOT_END_INSERT_WINDOWS, windowNewIndex, windowNewIndex);

    if(calibrationWindow->zp_windowType() == ZCalibrationWindow::WT_PEAK)
    {
        zh_createTermsForWindow(calibrationWindow);
    }
    zv_dirty = true;
    zv_dateTime = QDateTime::currentDateTime();
    emit zg_dirtyChanged(zv_dirty);

    return windowNewIndex;
    // zh_createTermsForWindow(calibrationWindow);
}
//=========================================================
int ZCalibration::zp_createNewCalibrationWindow(const ZRawWindow& rawWindow)
{
    int windowNewIndex = zv_windowList.count();
    bool ok;
    int firstChannel = rawWindow.firstChannel.toInt(&ok);
    if(!ok)
    {
        firstChannel = 0;
    }
    int lastChannel = rawWindow.lastChannel.toInt(&ok);
    if(!ok)
    {
        lastChannel = 0;
    }


    ZCalibrationWindow* calibrationWindow = new ZCalibrationWindow(rawWindow.name,
                                                                   rawWindow.windowType,
                                                                   firstChannel,
                                                                   lastChannel,
                                                                   this);

    zv_termNormalizer->zp_connectToWindow(calibrationWindow);
    zv_baseTermNormalizer->zp_connectToWindow(calibrationWindow);

    emit zg_windowOperation(WOT_BRGIN_INSERT_WINDOWS, windowNewIndex, windowNewIndex);
    zv_windowList.append(calibrationWindow);
    emit zg_windowOperation(WOT_END_INSERT_WINDOWS, windowNewIndex, windowNewIndex);

    if(calibrationWindow->zp_windowType() == ZCalibrationWindow::WT_PEAK)
    {
        zh_createTermsForWindow(calibrationWindow);
    }
    zv_dirty = false;
    emit zg_dirtyChanged(zv_dirty);
    return windowNewIndex;
}
//=========================================================
void ZCalibration::zh_createTermsForWindow(const ZCalibrationWindow* window)
{
    // creation simple
    int windowIndex = zv_windowList.indexOf(const_cast<ZCalibrationWindow*>( window));

    if(!zh_windowHasTerms(window, ZAbstractTerm::TT_SIMPLE))
    {
        // define newTermIndex;
        int newTermIndex = -1;
        const ZCalibrationWindow* currentWindow;

        for(int t = 0; t < zv_termList.count(); t++)
        {
            if(zv_termList.at(t)->zp_termType() != ZAbstractTerm::TT_SIMPLE)
            {
                continue;
            }

            const ZSimpleTerm* simpleTerm = qobject_cast<const ZSimpleTerm*>(zv_termList.at(t));
            if(!simpleTerm)
            {
                continue;
            }

            currentWindow = simpleTerm->zp_window();
            if(zv_windowList.indexOf(const_cast<ZCalibrationWindow*>(currentWindow)) > windowIndex)
            {
                break;
            }
            newTermIndex = t;
        }

        newTermIndex++;

        // creation and insertion
        ZSimpleTerm* simpleTerm = new ZSimpleTerm(window, this);
        emit zg_termOperation(TOT_BEGIN_INSERT_TERM, newTermIndex, newTermIndex);
        zv_termList.insert(newTermIndex, simpleTerm);
        emit zg_termOperation(TOT_END_INSERT_TERM, newTermIndex, newTermIndex);
        zv_dirty = true;
        emit zg_dirtyChanged(zv_dirty);
    }

    // quadratic
    if(!zh_windowHasTerms(window, ZAbstractTerm::TT_QUADRATIC))
    {
        // define newTermIndex;
        int newTermIndex = -1;
        int lastSimpleTermIndex = -1;
        const ZCalibrationWindow* currentWindow;

        for(int t = 0; t < zv_termList.count(); t++)
        {
            if(zv_termList.at(t)->zp_termType() == ZAbstractTerm::TT_SIMPLE)
            {
                lastSimpleTermIndex = t;
                continue;
            }

            if(zv_termList.at(t)->zp_termType() != ZAbstractTerm::TT_QUADRATIC)
            {
                continue;
            }

            const ZQuadraticTerm* simpleTerm = qobject_cast<const ZQuadraticTerm*>(zv_termList.at(t));
            if(!simpleTerm)
            {
                continue;
            }

            currentWindow = simpleTerm->zp_window();
            if(zv_windowList.indexOf(const_cast<ZCalibrationWindow*>(currentWindow)) > windowIndex)
            {
                break;
            }

            newTermIndex = t;
        }

        if(newTermIndex < 0)
        {
            newTermIndex = lastSimpleTermIndex + 1;
        }
        else
        {
            newTermIndex++;
        }

        // creation and insertion
        ZQuadraticTerm* quadraticTerm = new ZQuadraticTerm(window, this);
        // insertion
        emit zg_termOperation(TOT_BEGIN_INSERT_TERM, newTermIndex, newTermIndex);
        zv_termList.insert(newTermIndex, quadraticTerm );
        emit zg_termOperation(TOT_END_INSERT_TERM, newTermIndex, newTermIndex);
        zv_dirty = true;
        zv_dateTime = QDateTime::currentDateTime();
        emit zg_dirtyChanged(zv_dirty);
    }
}
//=========================================================
bool ZCalibration::zh_windowHasTerms(const ZCalibrationWindow* window, ZAbstractTerm::TermType type) const
{
    for(int t = 0; t < zv_termList.count(); t++)
    {
        if(zv_termList.at(t)->zp_termType() != type)
        {
            continue;
        }

        if(zv_termList.at(t)->zp_termBelongsToWindow(window))
        {
            return true;
        }
    }

    return false;
}
//=========================================================
int ZCalibration::zh_termIndex(const ZAbstractTerm* term) const
{
    if(!term)
    {
        return -1;
    }

    for(int i = 0; i < zv_termList.count(); i++)
    {
        if(zv_termList.at(i) == term)
        {
            return i;
        }
    }

    return -1;
}
//=========================================================
void ZCalibration::zh_chopTailZeroesFromInterceptString()
{
    for(int i = zv_interceptString.count() - 1; i > 0; i--)
    {
        if(zv_interceptString.at(i) != '0' || (zv_interceptString.at(i - 1) == '.' || zv_interceptString.at(i - 1) == ',' ))
        {
            break;
        }
        zv_interceptString.chop(1);
    }
}
//=========================================================
qreal* ZCalibration::zh_termFactorPointer(int termIndex) const
{
    if(termIndex < 0 || termIndex >= zv_termList.count() )
    {
        return nullptr;
    }

    return zv_termList.value(termIndex)->zh_termFactorPointer();
}
//=========================================================
int ZCalibration::zh_findNextTermIndex(ZAbstractTerm::TermType termType) const
{
    if(termType == ZAbstractTerm::TT_SIMPLE)
    {
        for(int t = 0; t < zv_termList.count(); t++)
        {
            if(zv_termList.at(t)->zp_termType() != ZAbstractTerm::TT_SIMPLE)
            {
                return t;
            }
        }
    }
    else if(termType == ZAbstractTerm::TT_QUADRATIC)
    {
        for(int t = 0; t < zv_termList.count(); t++)
        {
            if(zv_termList.at(t)->zp_termType() == ZAbstractTerm::TT_SIMPLE
                    || zv_termList.at(t)->zp_termType() == ZAbstractTerm::TT_QUADRATIC)
            {
                continue;
            }

            return t;
        }
    }
    else if(termType == ZAbstractTerm::TT_MIXED)
    {
        for(int t = 0; t < zv_termList.count(); t++)
        {
            if(zv_termList.at(t)->zp_termType() == ZAbstractTerm::TT_SIMPLE
                    || zv_termList.at(t)->zp_termType() == ZAbstractTerm::TT_QUADRATIC
                    || zv_termList.at(t)->zp_termType() == ZAbstractTerm::TT_MIXED)
            {
                continue;
            }

            return t;
        }
    }
    else if(termType == ZAbstractTerm::TT_CUSTOM)
    {
        for(int t = 0; t < zv_termList.count(); t++)
        {
            if(zv_termList.at(t)->zp_termType() == ZAbstractTerm::TT_SIMPLE
                    || zv_termList.at(t)->zp_termType() == ZAbstractTerm::TT_QUADRATIC
                    || zv_termList.at(t)->zp_termType() == ZAbstractTerm::TT_MIXED
                    || zv_termList.at(t)->zp_termType() == ZAbstractTerm::TT_CUSTOM)
            {
                continue;
            }

            return t;
        }
    }

    return zv_termList.count();
}
//=========================================================
bool ZCalibration::zp_termFactor(int termIndex, qreal& factor) const
{
    if(termIndex < 0 || termIndex >= zv_termList.count() )
    {
        factor = 0;
        return false;
    }

    factor =  zv_termList.at(termIndex)->zp_termFactor();
    return true;
}
//=========================================================
QList<int> ZCalibration::zp_termWindowIndexList(int termIndex) const
{
    QList<int> indexList;
    if(termIndex < 0 || termIndex >= zv_termList.count())
    {
        return indexList;
    }

    QList<qint64> windowsIdList = zv_termList.at(termIndex)->zp_termWindowIdList();

    for(int i = 0; i < windowsIdList.count(); i++)
    {
        for(int w = 0; w < zv_windowList.count(); w++)
        {
            if(zv_windowList.at(w)->zp_windowId() == windowsIdList.at(i))
            {
                indexList.append(w);
                break;
            }
        }
    }

    return indexList;
}
//=========================================================
QStringList ZCalibration::zp_termWindowsNameList(int termIndex) const
{
    QList<int> termIndexList = zp_termWindowIndexList(termIndex);
    if(termIndexList.isEmpty())
    {
        return QStringList();
    }

    QStringList termWindowsNameList;
    for(int w = 0; w < termIndexList.count(); w++)
    {
        termWindowsNameList.append(zv_windowList.at(termIndexList.at(w))->zp_windowName());
    }

    return termWindowsNameList;
}
//=========================================================
QString ZCalibration::zp_termFactorString(int termIndex) const
{
    if(termIndex < 0 || termIndex >= zv_termList.count() )
    {
        return "0.0";
    }

    return zv_termList.at(termIndex)->zp_termFactorString();
}
//=========================================================
bool ZCalibration::zp_termFactorString(int termIndex, QString& factorString) const
{
    if(termIndex < 0 || termIndex >= zv_termList.count() )
    {
        factorString = "0.0";
        return false;
    }

    factorString = zv_termList.at(termIndex)->zp_termFactorString();
    return true;
}
//=========================================================
bool ZCalibration::zp_setTermFactorString(int termIndex, const QString& factorString)
{
    if(termIndex < 0 || termIndex >= zv_termList.count() )
    {
        return false;
    }

    if(zv_termList.at(termIndex)->zh_setTermFactor(factorString))
    {
        emit zg_termOperation(TOT_TERM_FACTOR_CHANGED, termIndex, termIndex);
        zv_dateTime = QDateTime::currentDateTime();
        return true;
    }

    return false;
}
//=========================================================
bool ZCalibration::zp_termVariablePart(int termIndex, const ZAbstractSpectrum* spectrum,  qreal& value) const
{
    if(termIndex < 0 || termIndex >= zv_termList.count() || !spectrum)
    {
        return false;
    }

    value = 0.0;
    bool res = zv_termList.at(termIndex)->zp_calcTermVariablePart(spectrum, value);
    if(res)
    {
        res = zv_termNormalizer->zp_normalizeValue(spectrum, value);
    }

    return res;
}
//=========================================================
bool ZCalibration::zp_calcBaseTermValue(const ZAbstractSpectrum* spectrum, qreal& value) const
{
    value = 0.0;
    if(zv_baseTermId < 0)
    {
        return false;
    }

    //qreal baseTermValue = std::numeric_limits<double>::quiet_NaN();
    //qreal baseNormaValue = std::numeric_limits<double>::quiet_NaN();

    for(int t = 0; t < zv_termList.count(); t++)
    {
        if(zv_termList.at(t)->zp_termId() == zv_baseTermId)
        {
            if(!zv_termList.at(t)->zp_calcTermVariablePart(spectrum, value))
            {
                return false;
            }
            // baseTerm - ok

            if(!zv_baseTermNormalizer->zp_normalizeValue(spectrum, value))
            {
                return false;
            }

            break;
        }
    }

    return true;
}
//=========================================================
bool ZCalibration::zp_createMixedTerms(int termIndex)
{
    if(termIndex < 0 || termIndex >= zv_termList.count())
    {
        return false;
    }

    if(zv_termList.at(termIndex)->zp_termType() != ZAbstractTerm::TT_SIMPLE)
    {
        return false;
    }

    //  get window from simple term
    ZSimpleTerm* baseTerm = qobject_cast<ZSimpleTerm*>(zv_termList.at(termIndex));
    if(!baseTerm)
    {
        return false;
    }

    const ZCalibrationWindow* baseWindow = baseTerm->zp_window();
    ZMixedTerm* mixedTerm;
    QList<ZAbstractTerm*> mixedTermList;
    for(int w = 0; w < zv_windowList.count(); w++)
    {
        if(zv_windowList.at(w) == baseWindow || zv_windowList.at(w)->zp_windowType() != ZCalibrationWindow::WT_PEAK)
        {
            continue;
        }

        // check existing mixed terms
        bool termExists = false;
        for(int t = 0; t < zv_termList.count(); t++)
        {
            if(zv_termList.at(t)->zp_termType() != ZAbstractTerm::TT_MIXED)
            {
                continue;
            }

            if(zv_termList.at(t)->zp_termBelongsToWindow(baseWindow) && zv_termList.at(t)->zp_termBelongsToWindow(zv_windowList.at(w)))
            {
                termExists = true;
                break;
            }
        }

        if(termExists)
        {
            continue;
        }

        // create mixed term
        mixedTerm = new ZMixedTerm(baseWindow, zv_windowList.at(w), this);
        mixedTermList.append(mixedTerm);
    }

    if(mixedTermList.isEmpty())
    {
        return false;
    }

    // define first custom term
    int t = 0;
    for(; t < zv_termList.count(); t++)
    {
        if(zv_termList.at(t)->zp_termType() == ZAbstractTerm::TT_CUSTOM)
        {
            break;
        }
    }

    // insert new term
    if(mixedTermList.isEmpty())
    {
        return false;
    }

    emit zg_termOperation(TOT_BEGIN_INSERT_TERM, t, t + mixedTermList.count() - 1);
    for(int i = 0; i < mixedTermList.count(); i++)
    {
        zv_termList.insert(t+i , mixedTermList.at(i));
    }
    emit zg_termOperation(TOT_END_INSERT_TERM, t, t + mixedTermList.count() - 1);
    zv_dirty = true;
    zv_dateTime = QDateTime::currentDateTime();
    emit zg_dirtyChanged(zv_dirty);

    return true;
}
//=========================================================
bool ZCalibration::zp_removeMixedTerms()
{
    for(int t = zv_termList.count() - 1; t >= 0; t--)
    {
        if(zv_termList.at(t)->zp_termType() != ZAbstractTerm::TT_MIXED)
        {
            continue;
        }

        emit zg_termOperation(TOT_BEGIN_REMOVE_TERM, t, t);
        delete zv_termList.takeAt(t);
        emit zg_termOperation(TOT_END_REMOVE_TERM, t, t);
        zv_dirty = true;
        zv_dateTime = QDateTime::currentDateTime();
        emit zg_dirtyChanged(zv_dirty);
    }

    return true;
}
//=========================================================
int ZCalibration::zp_createTerm(QList<int>& windowIndexList,
                                ZAbstractTerm::TermType termType,
                                ZAbstractTerm::TermState termState,
                                const QString &termFactor)
{

    // define next term index
    int termIndex = zh_findNextTermIndex(termType);
    ZAbstractTerm* term = nullptr;
    if(termType == ZAbstractTerm::TT_SIMPLE)
    {
        if(windowIndexList.isEmpty())
        {
            return -1;
        }
        ZCalibrationWindow* window = zv_windowList.value(windowIndexList.at(0));
        if(window->zp_windowType() != ZCalibrationWindow::WT_PEAK)
        {
            return -1;
        }

        term = new ZSimpleTerm(window, this);
        term->zp_setTermState(termState);
        term->zh_setTermFactor(termFactor);
        //        emit zg_termOperation(TOT_BEGIN_INSERT_TERM, termIndex, termIndex);
        //        zv_termList.insert(termIndex, simpleTerm);
        //        emit zg_termOperation(TOT_END_INSERT_TERM, termIndex, termIndex);

    }
    else if(termType == ZAbstractTerm::TT_QUADRATIC)
    {
        if(windowIndexList.isEmpty())
        {
            return -1;
        }
        ZCalibrationWindow* window = zv_windowList.value(windowIndexList.at(0));
        if(window->zp_windowType() != ZCalibrationWindow::WT_PEAK)
        {
            return -1;
        }

        term = new ZQuadraticTerm(window, this);
        term->zp_setTermState(termState);
        term->zh_setTermFactor(termFactor);
    }
    else if(termType == ZAbstractTerm::TT_MIXED)
    {
        if(windowIndexList.count() != 2)
        {
            return -1;
        }

        ZCalibrationWindow* window1 = zv_windowList.value(windowIndexList.at(0));
        if(window1->zp_windowType() != ZCalibrationWindow::WT_PEAK)
        {
            return -1;
        }
        ZCalibrationWindow* window2 = zv_windowList.value(windowIndexList.at(1));
        if(window2->zp_windowType() != ZCalibrationWindow::WT_PEAK)
        {
            return -1;
        }
        term = new ZMixedTerm(window1, window2, this);
        term->zp_setTermState(termState);
        term->zh_setTermFactor(termFactor);
    }
    else if(termType == ZAbstractTerm::TT_CUSTOM)
    {
        return -1;
        // term = new ZCustomTerm(this);
    }
    else if(termType == ZAbstractTerm::TT_NOT_DEFINED)
    {
        return -1;
    }

    emit zg_termOperation(TOT_BEGIN_INSERT_TERM, termIndex, termIndex);
    zv_termList.insert(termIndex, term);
    emit zg_termOperation(TOT_END_INSERT_TERM, termIndex, termIndex);
    zv_dirty = true;
    zv_dateTime = QDateTime::currentDateTime();
    emit zg_dirtyChanged(zv_dirty);

    return termIndex;
}
//=========================================================
int ZCalibration::zp_createTerm(const ZRawTerm& rawTerm)
{
    // check out is the term already exist
    for(int t = 0; t < zv_termList.count(); t++)
    {
        if(rawTerm.name == zv_termList.at(t)->zp_termName()
                && rawTerm.termType == zv_termList.at(t)->zp_termType())
        {
            QList<qint64> termWindowIdList =  zv_termList.at(t)->zp_termWindowIdList();
            if(termWindowIdList.count() == rawTerm.windowList.count())
            {
                bool res = true;
                foreach(qint64 id, termWindowIdList)
                {
                    if(!rawTerm.windowList.contains(zp_calibrationWindowNameForId(id)))
                    {
                        res = false;
                        break;
                    }
                }

                // term exists. set parameters to it
                if(res)
                {
                    zv_termList.at(t)->zp_setTermState(rawTerm.termState);
                    zv_termList.at(t)->zh_setTermFactor(rawTerm.factor) ;
                    return t;
                }
            }
        }
    }

    // create windowIndexList
    QList<int> windowIndexList;
    int index;
    foreach(QString windowName, rawTerm.windowList)
    {
        index = zp_windowIndexForName(windowName);
        if(index < 0 || index >= zv_windowList.count())
        {
            // TODO error msg
            return -1;
        }
        windowIndexList.append(index);
    }

    return zp_createTerm(windowIndexList,
                         rawTerm.termType,
                         rawTerm.termState,
                         rawTerm.factor);
}
//=========================================================
ZTermNormalizer::NormaType ZCalibration::zp_normaType() const
{
    return zv_termNormalizer->zp_normaType();
}
//=========================================================
bool ZCalibration::zp_setNormaType(ZTermNormalizer::NormaType type)
{
    bool res = zv_termNormalizer->zp_setNormaType(type);
    if(res)
    {
        zv_dirty = true;
        zv_dateTime = QDateTime::currentDateTime();
        emit zg_dirtyChanged(zv_dirty);
    }
    return res;
}
//=========================================================
bool ZCalibration::zp_setNormaCustomString(const QString& customString)
{
    bool res = zv_termNormalizer->zp_setCustomNormaString(customString);
    if(res)
    {
        zv_dirty = true;
        zv_dateTime = QDateTime::currentDateTime();
        emit zg_dirtyChanged(zv_dirty);
    }
    return res;
}
//=========================================================
QString ZCalibration::zp_normaCustomString() const
{
    return zv_termNormalizer->zp_normaCustomString();
}
//=========================================================
ZTermNormalizer::NormaType ZCalibration::zp_baseTermNormaType() const
{
    return zv_baseTermNormalizer->zp_normaType();
}
//=========================================================
bool ZCalibration::zp_setBaseTermNormaType(ZTermNormalizer::NormaType type)
{
    bool res = zv_baseTermNormalizer->zp_setNormaType(type);
    if(res)
    {
        zv_dirty = true;
        zv_dateTime = QDateTime::currentDateTime();
        emit zg_dirtyChanged(zv_dirty);
    }
    return res;
}
//=========================================================
bool ZCalibration::zp_setBaseTermNormaCustomString(const QString& customString)
{
    bool res = zv_baseTermNormalizer->zp_setCustomNormaString(customString);
    if(res)
    {
        zv_dirty = true;
        zv_dateTime = QDateTime::currentDateTime();
        emit zg_dirtyChanged(zv_dirty);
    }
    return res;
}
//=========================================================
QString ZCalibration::zp_baseTermNormaCustomString() const
{
    return zv_baseTermNormalizer->zp_normaCustomString();
}
//=========================================================
bool ZCalibration::zp_setBaseTermNormalizerParameters(ZTermNormalizer::NormaType type,
                                                      const QString& customString)
{
    bool res = zv_baseTermNormalizer->zp_setNormalizerParameters(type, customString);
    if(res)
    {
        zv_dirty = true;
        zv_dateTime = QDateTime::currentDateTime();
        emit zg_dirtyChanged(zv_dirty);
    }
    return res;
}
//=========================================================
ZCalibration::EquationType ZCalibration::zp_equationType() const
{
    return zv_equationType;
}
//=========================================================
bool ZCalibration::zp_setEquationType(ZCalibration::EquationType type)
{
    if(zv_equationType == type)
    {
        return false;
    }
    zv_equationType = type;
    zv_dateTime = QDateTime::currentDateTime();
    zv_dirty = true;
    emit zg_dirtyChanged(zv_dirty);

    return true;
}
//=========================================================
qreal ZCalibration::zp_equationIntercept() const
{
    return zv_intercept;
}
//=========================================================
bool ZCalibration::zp_setEquationIntercept(qreal value)
{
    if(zv_intercept - value == 0.0)
    {
        return false;
    }

    zv_intercept = value;
    zv_interceptString = QString::number(value, 'f', 15);
    zh_chopTailZeroesFromInterceptString();
    emit zg_interceptChanged();
    zv_dateTime = QDateTime::currentDateTime();
    zv_dirty = true;
    emit zg_dirtyChanged(zv_dirty);

    return true;
}
//=========================================================
QString ZCalibration::zp_equationInterceptString() const
{
    return zv_interceptString;
}
//=========================================================
bool ZCalibration::zp_setEquationInterceptString(const QString& interceptString)
{
    bool ok;
    qreal intercept = interceptString.toDouble(&ok);
    if(!ok)
    {
        return false;
    }

    zv_dirty = true;
    zv_dateTime = QDateTime::currentDateTime();
    emit zg_dirtyChanged(zv_dirty);
    bool res = zp_setEquationIntercept(intercept);
    if(res)
    {
        zv_dirty = true;
        emit zg_dirtyChanged(zv_dirty);
    }
    return res;
}
//=========================================================
QString ZCalibration::zp_baseTermDisplayString() const
{
    if(zv_baseTermId < 0)
    {
        return QString();
    }

    QString baseTermString;
    for(int t = 0; t < zv_termList.count(); t++)
    {
        if(zv_termList.at(t)->zp_termId() == zv_baseTermId)
        {
            baseTermString = zv_termList.at(t)->zp_termName();
            break;
        }
    }

    if(zv_baseTermNormalizer->zp_normaType() != ZTermNormalizer::NT_NONE)
    {
        QString normaTypeString = ZTermNormalizer::zp_normaTypeString(zv_baseTermNormalizer->zp_normaType());

        if(zv_baseTermNormalizer->zp_normaType() == ZTermNormalizer::NT_COHERENT_INCOHERENT
                || zv_baseTermNormalizer->zp_normaType() == ZTermNormalizer::NT_INCOHERENT_COHERENT)
        {
            normaTypeString = "("+normaTypeString+")";
        }

        baseTermString = baseTermString + "/" + normaTypeString;
    }

    return baseTermString;
}
//=========================================================
QString ZCalibration::zp_baseTermString() const
{
    if(zv_baseTermId < 0)
    {
        return QString();
    }

    QString baseTermString;
    for(int t = 0; t < zv_termList.count(); t++)
    {
        if(zv_termList.at(t)->zp_termId() == zv_baseTermId)
        {
            baseTermString = zv_termList.at(t)->zp_termName();
            break;
        }
    }

    return baseTermString;
}
//=========================================================
qint64 ZCalibration::zp_baseTermId() const
{
    return zv_baseTermId;
}
//=========================================================
bool ZCalibration::zp_setBaseTermId(qint64 id)
{
    if(zv_baseTermId == id)
    {
        return false;
    }

    for(int i = 0; i < zv_termList.count(); i++)
    {
        if(zv_termList.at(i)->zp_termId() == id)
        {
            zv_baseTermId = id;
            zv_dirty = true;
            emit zg_dirtyChanged(zv_dirty);
            return true;
        }
    }
    zv_baseTermId = -1;
    zv_dirty = true;
    zv_dateTime = QDateTime::currentDateTime();
    emit zg_dirtyChanged(zv_dirty);

    return true;
}
//=========================================================
int ZCalibration::zp_baseTermIndex() const
{
    if(zv_baseTermId < 0)
    {
        return -1;
    }

    for(int t = 0; t < zv_termList.count(); t++)
    {
        if(zv_termList.at(t)->zp_termId() == zv_baseTermId)
        {
            return t;
        }
    }
    return -1;
}
//=========================================================
bool ZCalibration::zp_setBaseTermFromName(const QString& baseTermName)
{
    for(int t = 0; t < zv_termList.count(); t++)
    {
        if(zv_termList.at(t)->zp_termName() == baseTermName)
        {
            zv_baseTermId = zv_termList.at(t)->zp_termId();
            return true;
        }
    }

    return false;
}
//=========================================================
void ZCalibration::zp_createEquationDataForEquationRecalc(QMap<int, qreal*>& factorMap, qreal*& freeTermPtr)
{
    ZAbstractTerm::TermState termState;
    for(int t = 0; t < zv_termList.count(); t++)
    {
        if(zv_equationType == ET_FRACTIONAL
                && !zv_useBaseTermInFractionalEquation
                && zv_termList.value(t)->zv_termId == zv_baseTermId )
        {
            continue;
        }

        termState = zv_termList.value(t)->zv_termState;
        if(termState == ZAbstractTerm::TS_CONST_INCLUDED
                || termState == ZAbstractTerm::TS_INCLUDED)
        {
            factorMap.insert(t, zv_termList.value(t)->zh_termFactorPointer());
        }
    }

    freeTermPtr = &zv_intercept;
}
//=========================================================
void ZCalibration::zh_notifyCalibrationRecalc()
{
    // conform free term string to value

    zv_interceptString = ZQrealToStringConverter::zp_convert(zv_intercept);

    // conform equation terms
    for(int t = 0; t < zv_termList.count(); t++)
    {
        zv_termList.value(t)->zh_conformStringWithValue();
    }
    emit zg_termOperation(TOT_TERM_FACTOR_CHANGED, 0, zv_termList.count() - 1);
    emit zg_interceptChanged();
}
//=========================================================
void ZCalibration::zp_resetEquationTerms()
{
    bool res = false;
    for(int t = 0; t < zv_termList.count(); t++)
    {
        if(zv_termList[t]->zv_termFactor != 0.0)
        {
            res = res || true;
        }
        zv_termList[t]->zv_termFactor = 0.0;
        zv_termList[t]->zh_conformStringWithValue();
    }

    emit zg_termOperation(TOT_TERM_FACTOR_CHANGED, 0, zv_termList.count() - 1);
    res = res || zp_setEquationIntercept(0.0);

    if(res)
    {
        zv_dirty = true;
        emit zg_dirtyChanged(zv_dirty);
        zv_dateTime = QDateTime::currentDateTime();
    }
}
//=========================================================
ZCalibrationQualityData ZCalibration::zp_calibrationQualityData() const
{
    return zv_calibrationQualityData;
}
//=========================================================
void ZCalibration::zp_setCalibrationQualityData(ZCalibrationQualityData qualityData)
{
    zv_calibrationQualityData = qualityData;
}
//=========================================================
void ZCalibration::zp_setDetermination(const QString& determination)
{
    zv_calibrationQualityData.determination = determination;
}
//=========================================================
void ZCalibration::zp_setAdjustedDetermination(const QString& adjustedDetermination)
{
    zv_calibrationQualityData.adj_determination = adjustedDetermination;
}
//=========================================================
void ZCalibration::zp_setStandardDeviation(const QString& standardDeviation)
{
    zv_calibrationQualityData.standardDeviation = standardDeviation;
}
//=========================================================
void ZCalibration::zp_setCurrentDateTime()
{
    zv_dateTime = QDateTime::currentDateTime();
}
//=========================================================
void ZCalibration::zp_setDateTime(QDateTime dateTime)
{
    zv_dateTime = dateTime;
}
//=========================================================
void ZCalibration::zh_onTermNameChange()
{
    ZAbstractTerm* term = qobject_cast<ZAbstractTerm*>(sender());
    if(term == nullptr)
    {
        return;
    }

    int termIndex = zh_termIndex(term);

    emit zg_termOperation(TOT_TERM_NAME_CHANGED, termIndex, termIndex);
    zv_dateTime = QDateTime::currentDateTime();
    zv_dirty = true;
    emit zg_dirtyChanged(zv_dirty);
}
//=========================================================
void ZCalibration::zh_onTermWindowMarginChange()
{
    ZAbstractTerm* term = qobject_cast<ZAbstractTerm*>(sender());
    if(!term)
    {
        return;
    }
    int termIndex = zh_termIndex(term);
    emit zg_termOperation(TOT_TERM_WINDOW_MARGIN_CHANGED, termIndex, termIndex);
    zv_dirty = true;
    zv_dateTime = QDateTime::currentDateTime();
    emit zg_dirtyChanged(zv_dirty);
}
//=========================================================
void ZCalibration::zh_removeTerm(ZAbstractTerm* term)
{
    for(int i = 0; i < zv_termList.count(); i ++)
    {
        if(zv_termList.at(i) == term)
        {
            emit zg_termOperation(TOT_BEGIN_REMOVE_TERM, i, i);
            delete zv_termList.takeAt(i);
            emit zg_termOperation(TOT_END_REMOVE_TERM, i, i);
            zv_dirty = true;
            zv_dateTime = QDateTime::currentDateTime();
            emit zg_dirtyChanged(zv_dirty);
            return;
        }
    }
}
//=========================================================
void ZCalibration::zh_onNormalizerChange()
{
    if(sender() == zv_termNormalizer)
    {
        emit zg_normalizerChanged();
        zv_dirty = true;
        emit zg_dirtyChanged(zv_dirty);
    }
    else if(sender() == zv_baseTermNormalizer)
    {
        // TODO create signal base term changed and emit
        zv_dirty = true;
        zv_dateTime = QDateTime::currentDateTime();
        emit zg_dirtyChanged(zv_dirty);
    }
}
//=========================================================
bool ZCalibration::zh_isWindowExist(const QString& windowName)
{
    for(int i = 0; i < zv_windowList.count(); i++)
    {
        if(zv_windowList.at(i)->zp_windowName() == windowName)
        {
            return true;
        }
    }

    return false;
}
//=========================================================
bool ZCalibration::zp_isCalibrationWindowVisible(int windowIndex) const
{
    if(windowIndex < 0 || windowIndex >= zv_windowList.count())
    {
        return false;
    }

    return zv_windowList.at(windowIndex)->zp_isWindowVisible();
}
//=========================================================
bool ZCalibration::zp_setCalibrationWindowVisible(int windowIndex, bool visibility)
{
    if(windowIndex < 0 || windowIndex >= zv_windowList.count())
    {
        return false;
    }

    bool res = zv_windowList[windowIndex]->zp_setWindowVisible(visibility);
    if(res)
    {
        emit zg_windowOperation(WOT_WINDOW_CHANGED, windowIndex, windowIndex);
    }
    return res;
}
//=========================================================
int ZCalibration::zp_calibrationWindowCount() const
{
    return zv_windowList.count();
}
//=========================================================
QString ZCalibration::zp_calibrationWindowName(int windowIndex) const
{
    if(windowIndex < 0 || windowIndex >= zv_windowList.count())
    {
        return QString();
    }

    return zv_windowList.at(windowIndex)->zp_windowName();
}
//=========================================================
const ZCalibrationWindow* ZCalibration::zp_calibrationWindow(int windowIndex) const
{
    if(windowIndex < 0 || windowIndex >= zv_windowList.count())
    {
        return nullptr;
    }

    return zv_windowList.at(windowIndex);
}
//=========================================================
QString ZCalibration::zp_calibrationWindowNameForId(qint64 windowId) const
{
    for(int w = 0; w < zv_windowList.count(); w++)
    {
        if(zv_windowList.at(w)->zp_windowId() == windowId)
        {
            return zv_windowList.at(w)->zp_windowName();
        }
    }

    return QString();
}
//=========================================================
bool ZCalibration::zp_setCalibrationWindowName(int windowIndex, const QString& name)
{
    if(windowIndex < 0 || windowIndex >= zv_windowList.count())
    {
        return false;
    }

    bool res = zv_windowList[windowIndex]->zp_setWindowName(name);
    if(res)
    {
        emit zg_windowOperation(WOT_WINDOW_CHANGED, windowIndex, windowIndex);
        zv_dateTime = QDateTime::currentDateTime();
        zv_dirty = true;
        emit zg_dirtyChanged(zv_dirty);
    }
    return res;
}
//=========================================================
ZCalibrationWindow::WindowType ZCalibration::zp_calibrationWindowType(int windowIndex) const
{
    if(windowIndex < 0 || windowIndex >= zv_windowList.count())
    {
        return ZCalibrationWindow::WT_NOT_DEFINED;
    }

    return zv_windowList.at(windowIndex)->zp_windowType();
}
//=========================================================
bool ZCalibration::zp_setCalibrationWindowType(int windowIndex, ZCalibrationWindow::WindowType type)
{
    if(windowIndex < 0 || windowIndex >= zv_windowList.count())
    {
        return false;
    }

    bool res = zv_windowList[windowIndex]->zp_setWindowType(type);
    if(res)
    {
        emit zg_windowOperation(WOT_WINDOW_CHANGED, windowIndex, windowIndex);
        if(type == ZCalibrationWindow::WT_PEAK)
        {
            zh_createTermsForWindow(zv_windowList.at(windowIndex));
        }
        zv_dateTime = QDateTime::currentDateTime();
        zv_dirty = true;
        emit zg_dirtyChanged(zv_dirty);
    }
    return res;
}
//=========================================================
int ZCalibration::zp_calibrationWindowFirstChannel(int windowIndex) const
{
    if(windowIndex < 0 || windowIndex >= zv_windowList.count())
    {
        return 0;
    }

    return zv_windowList.at(windowIndex)->zp_firstChannel();
}
//=========================================================
bool ZCalibration::zp_setCalibrationWindowFirstChannel(int windowIndex, int channel)
{
    if(windowIndex < 0 || windowIndex >= zv_windowList.count())
    {
        return 0;
    }

    bool res = zv_windowList[windowIndex]->zp_setWindowFirstChannel(channel);
    if(res)
    {
        emit zg_windowOperation(WOT_WINDOW_CHANGED, windowIndex, windowIndex);
        zv_dateTime = QDateTime::currentDateTime();
        zv_dirty = true;
        emit zg_dirtyChanged(zv_dirty);
    }
    return res;
}
//=========================================================
int ZCalibration::zp_calibrationWindowLastChannel(int windowIndex) const
{
    if(windowIndex < 0 || windowIndex >= zv_windowList.count())
    {
        return 0;
    }

    return zv_windowList.at(windowIndex)->zp_lastChannel();
}
//=========================================================
bool ZCalibration::zp_setCalibrationWindowLastChannel(int windowIndex, int channel)
{
    if(windowIndex < 0 || windowIndex >= zv_windowList.count())
    {
        return 0;
    }

    bool res = zv_windowList[windowIndex]->zp_setWindowLastChannel(channel);
    if(res)
    {
        emit zg_windowOperation(WOT_WINDOW_CHANGED, windowIndex, windowIndex);
        zv_dateTime = QDateTime::currentDateTime();
        zv_dirty = true;
        emit zg_dirtyChanged(zv_dirty);
    }
    return res;
}
//=========================================================
qint64 ZCalibration::zp_calibrationWindowId(int windowIndex) const
{
    if(windowIndex < 0 || windowIndex >= zv_windowList.count())
    {
        return 0.0;
    }

    return zv_windowList.at(windowIndex)->zp_windowId();
}
//=========================================================
int ZCalibration::zp_windowIndexForName(const QString& windowName) const
{
    for(int w  = 0; w < zv_windowList.count(); w++)
    {
        if(zv_windowList.at(w)->zp_windowName() == windowName)
        {
            return w;
        }
    }

    return -1;
}
//=========================================================
bool ZCalibration::zp_removeCalibrationWindow(int windowIndex)
{
    if(windowIndex < 0 || windowIndex >= zv_windowList.count())
    {
        return false;
    }

    emit zg_windowOperation(WOT_BEGIN_REMOVE_WINDOWS, windowIndex, windowIndex);
    delete zv_windowList.takeAt(windowIndex);
    emit zg_windowOperation(WOT_END_REMOVE_WINDOWS, windowIndex, windowIndex);
    zv_dateTime = QDateTime::currentDateTime();
    zv_dirty = true;
    emit zg_dirtyChanged(zv_dirty);

    return true;
}
//=========================================================
void ZCalibration::zp_removeCalibrationWindows()
{
    for(int w = zv_windowList.count() - 1; w >= 0; w--)
    {
        zp_removeCalibrationWindow(w);
    }
}
//=========================================================
int ZCalibration::zp_termCount() const
{
    return zv_termList.count();
}
//=========================================================
QString ZCalibration::zp_termDisplayName(int termIndex) const
{
    if(termIndex < 0 || termIndex >= zv_termList.count() )
    {
        return QString();
    }

    QString termName = zv_termList.at(termIndex)->zp_termName();
    if(zv_termNormalizer->zp_normaType() != ZTermNormalizer::NT_NONE)
    {
        termName += " / N";
    }
    return termName;
}
//=========================================================
QString ZCalibration::zp_termName(int termIndex) const
{
    if(termIndex < 0 || termIndex >= zv_termList.count() )
    {
        return QString();
    }

    QString termName = zv_termList.at(termIndex)->zp_termName();
    return termName;
}
//=========================================================
ZAbstractTerm::TermType ZCalibration::zp_termType(int termIndex) const
{
    if(termIndex < 0 || termIndex >= zv_termList.count() )
    {
        return ZAbstractTerm::TT_NOT_DEFINED;
    }

    return zv_termList.at(termIndex)->zp_termType();
}
//=========================================================
bool ZCalibration::zp_mixedTermExists() const
{
    for(int t = 0; t < zv_termList.count(); t++)
    {
        if(zv_termList.at(t)->zp_termType() == ZAbstractTerm::TT_MIXED)
        {
            return true;
        }
    }

    return false;
}
//=========================================================
qint64 ZCalibration::zp_termId(int termIndex) const
{
    if(termIndex < 0 || termIndex >= zv_termList.count())
    {
        return -1;
    }

    return zv_termList.at(termIndex)->zp_termId();
}
//=========================================================
bool ZCalibration::zp_setTermState(int termIndex,
                                   ZAbstractTerm::TermState zp_termState)
{
    if(termIndex < 0 || termIndex >= zv_termList.count())
    {
        return false;
    }

    bool res = zv_termList.at(termIndex)->zp_setTermState(zp_termState);
    if(res)
    {
        emit zg_termOperation(TOT_TERM_STATE_CHANGED, termIndex, termIndex);
        zv_dateTime = QDateTime::currentDateTime();
        zv_dirty = true;
        emit zg_dirtyChanged(zv_dirty);
    }

    return res;
}
//=========================================================
ZAbstractTerm::TermState ZCalibration::zp_termStateForDisplay(int termIndex) const
{
    if(termIndex < 0 || termIndex >= zv_termList.count() )
    {
        return ZAbstractTerm::TS_NOT_DEFINED;
    }

    if(zv_termList.at(termIndex)->zp_termId() == zv_baseTermId && zv_equationType == ET_FRACTIONAL)
    {
        return ZAbstractTerm::TS_BASE;
    }

    return zv_termList.at(termIndex)->zp_termState();
}
//=========================================================
ZAbstractTerm::TermState ZCalibration::zp_termState(int termIndex) const
{
    if(termIndex < 0 || termIndex >= zv_termList.count() )
    {
        return ZAbstractTerm::TS_NOT_DEFINED;
    }

    return zv_termList.at(termIndex)->zp_termState();
}
//=========================================================
void ZCalibration::zp_setNextUsersTermState(int termIndex)
{
    if(termIndex < 0 || termIndex >= zv_termList.count() )
    {
        return;
    }

    if(zv_termList.at(termIndex)->zp_termId() == zv_baseTermId
            && zv_equationType == ET_FRACTIONAL)
    {
        return;
    }

    ZAbstractTerm::TermState state = zv_termList.at(termIndex)->zp_termState();
    bool res = false;
    switch(state)
    {
    case ZAbstractTerm::TS_CONST_INCLUDED:
        res = zv_termList.at(termIndex)->zp_setTermState(ZAbstractTerm::TS_EXAM_WAITING);
        break;
    case ZAbstractTerm::TS_EXAM_WAITING:
        res = zv_termList.at(termIndex)->zp_setTermState(ZAbstractTerm::TS_CONST_EXCLUDED);
        break;
    case ZAbstractTerm::TS_INCLUDED:
        res = zv_termList.at(termIndex)->zp_setTermState(ZAbstractTerm::TS_CONST_EXCLUDED);
        break;
    case ZAbstractTerm::TS_EXCEPTED:
        res = zv_termList.at(termIndex)->zp_setTermState(ZAbstractTerm::TS_CONST_EXCLUDED);
        break;
    case ZAbstractTerm::TS_CONST_EXCLUDED:
        res =zv_termList.at(termIndex)->zp_setTermState(ZAbstractTerm::TS_CONST_INCLUDED);
        break;
    case ZAbstractTerm::TS_BASE:
    case ZAbstractTerm::TS_NOT_DEFINED:
//    default:
        return;
    }

    if(res)
    {
        emit zg_termOperation(TOT_TERM_STATE_CHANGED, termIndex, termIndex);
        zv_dateTime = QDateTime::currentDateTime();
        zv_dirty = true;
        emit zg_dirtyChanged(zv_dirty);
    }
}
//=========================================================
bool ZCalibration::checkColor(QColor color)
{
    if(!color.isValid() || zv_colorList.contains(color))
    {
        return false;
    }

    int colorSumm = color.red() + color.green() + color.blue();
    int averageVol = colorSumm / 3;

    if(colorSumm > 450)
    {
        return false;
    }

    if(abs(averageVol - color.red()) + abs(averageVol - color.green()) + abs(averageVol - color.blue()) < 90)
    {
        return false;
    }

    return true;
}
//======================================================
QList<QColor> ZCalibration::zp_createColorList()
{
    int red;
    int green;
    int blue;
    QColor color;

    QList<QColor> colorList;

    while(colorList.count() < 1500)
    {
        qsrand(static_cast<uint>(QDateTime::currentMSecsSinceEpoch() + 500));
        red = ((qrand() % 24) * 10) + 15;
        green = ((qrand() % 24) * 10) + 15;
        blue = ((qrand() % 24) * 10) +15;

        color = QColor(red, green, blue);
        if(ZCalibration::checkColor(color))
        {
            colorList << color;
        }
    }
    return colorList;
}
//======================================================

