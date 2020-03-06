//===================================================================
#include "ZQuadraticTerm.h"
#include "ZCalibrationWindow.h"
#include <math.h>
//===================================================================
ZQuadraticTerm::ZQuadraticTerm(const ZCalibrationWindow *window,
                               ZCalibration *parent) :
    ZAbstractTerm(parent)
{
    zv_type = TT_QUADRATIC;
    zv_window = window;
    zh_setName();
    zh_connectToWindow();
    zh_connectToCalibration(parent);
}
//===================================================================
bool ZQuadraticTerm::zp_calcValue(const ZAbstractSpectrum * spectrum, qreal& value)
{
    value = 0;
    if(zv_termState != TS_CONST_INCLUDED && zv_termState != TS_INCLUDED)
    {
        return true;
    }

    bool ok;
    emit zg_inquiryWindowIntensity((const QObject*)spectrum, value, true, &ok);
    if(!ok)
    {
        return false;
    }

    value = pow(value, 2.0) * zv_termFactor;
    return true;
}
//===================================================================
bool ZQuadraticTerm::zp_calcTermVariablePart(const ZAbstractSpectrum* spectrum, qreal& value)  // w/o factor
{
    value = 0.0;
    bool ok;
    emit zg_inquiryWindowIntensity((const QObject*)spectrum, value, false, &ok);
    value = value * value;
    return ok;
}
//===================================================================
bool ZQuadraticTerm::zp_termBelongsToWindow(const ZCalibrationWindow* window) const
{
    return window == zv_window;
}
//===================================================================
QList<qint64> ZQuadraticTerm::zp_termWindowIdList() const
{
    QList<qint64> windowIdList;
    windowIdList << zv_window->zp_windowId();
    return windowIdList;
}
//===================================================================
const ZCalibrationWindow* ZQuadraticTerm::zp_window() const
{
    return zv_window;
}
//===================================================================
void ZQuadraticTerm::zh_updateTermNameForWindowName()
{
    if(!sender())
    {
        return;
    }

    zh_setName();
    emit zg_termNameChanged();
}
//===================================================================
void ZQuadraticTerm::zh_setName()
{
    // zv_name = "(" + windowName + ")" + QChar(0x00B2);
    zv_name = zv_window->zp_windowName() + QChar(0x00B2);
}
//===================================================================
void ZQuadraticTerm::zh_onWindowTypeChange(ZCalibrationWindow::WindowType previousType,
                                           ZCalibrationWindow::WindowType currentType)
{
    if(!sender() || sender() != zv_window)
    {
        return;
    }

    if(currentType != ZCalibrationWindow::WT_PEAK)
    {
        emit zg_inquiryForDelete(this);
    }
}
//===================================================================
void ZQuadraticTerm::zh_connectToWindow()
{
    if(zv_window)
    {
        connect(zv_window, &ZCalibrationWindow::destroyed,
                this, &ZQuadraticTerm::zh_onWindowDestroying);
        connect(zv_window, &ZCalibrationWindow::zg_windowNameChanged,
                this, &ZQuadraticTerm::zh_updateTermNameForWindowName);
        connect(this, &ZQuadraticTerm::zg_inquiryWindowIntensity,
                zv_window, &ZCalibrationWindow::zp_calcWindowIntensity);
        connect(zv_window, &ZCalibrationWindow::zg_windowMarginsChanged,
                this, &ZQuadraticTerm::zg_termWindowMarginChanged);
        connect(zv_window, &ZCalibrationWindow::zg_windowTypeChanged,
                this, &ZQuadraticTerm::zh_onWindowTypeChange);
    }
}
//===================================================================

