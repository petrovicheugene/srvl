//===================================================================
#include "ZMixedTerm.h"
//===================================================================
ZMixedTerm::ZMixedTerm(const ZCalibrationWindow* window1,
                       const ZCalibrationWindow* window2,
                       ZCalibration *parent) : ZAbstractTerm(parent)
{
    zv_type = TT_MIXED;
    zv_window1 = window1;
    zv_window2 = window2;

    zh_setName();
    zh_connectToWindows();
    zh_connectToCalibration(parent);
}
//===================================================================
bool ZMixedTerm::zp_calcValue(const ZAbstractSpectrum * spectrum, qreal& value)
{
    value = 0;
    if(zv_termState != TS_CONST_INCLUDED && zv_termState != TS_INCLUDED)
    {
        return true;
    }

    bool ok;
    qreal win1Intensity = 0.0;

    emit zg_requestWindow1Intensity((const QObject*)spectrum, win1Intensity, true, &ok);
    if(!ok)
    {
        return false;
    }

    qreal win2Intensity = 0.0;
    emit zg_requestWindow2Intensity((const QObject*)spectrum, win2Intensity, true, &ok);
    if(!ok)
    {
        return false;
    }

    value = win1Intensity * win2Intensity * zv_termFactor;
    return true;
}
//===================================================================
bool ZMixedTerm::zp_calcTermVariablePart(const ZAbstractSpectrum* spectrum, qreal &value)  // w/o factor
{
    value= 0.0;
    bool ok;
    qreal win1Intensity = 0.0;
    emit zg_requestWindow1Intensity((const QObject*)spectrum, win1Intensity, false, &ok);
    if(!ok)
    {
        return false;
    }

    qreal win2Intensity = 0.0;
    emit zg_requestWindow2Intensity((const QObject*)spectrum, win2Intensity, true, &ok);
    if(!ok)
    {
        return false;
    }

    value = win1Intensity * win2Intensity;
    return ok;
}
//===================================================================
bool ZMixedTerm::zp_termBelongsToWindow(const ZCalibrationWindow* window) const
{
    return window == zv_window1 || window == zv_window2;
}
//===================================================================
QList<qint64> ZMixedTerm::zp_termWindowIdList() const
{
    QList<qint64> windowIdList;
    windowIdList << zv_window1->zp_windowId();
    windowIdList << zv_window2->zp_windowId();
    return windowIdList;
}
//===================================================================
void ZMixedTerm::zh_updateTermNameForWindowName()
{
    if(!sender())
    {
        return;
    }
    else if(sender() != zv_window1 && sender() != zv_window2)
    {
        return;
    }

    zh_setName();
    emit zg_termNameChanged();
}
//===================================================================
void ZMixedTerm::zh_onWindowTypeChange(ZCalibrationWindow::WindowType previousType,
                                   ZCalibrationWindow::WindowType currentType)
{
    if(!sender() || (sender() != zv_window1 && sender() != zv_window2))
    {
        return;
    }

    if(currentType != ZCalibrationWindow::WT_PEAK)
    {
        emit zg_requestForDelete(this);
    }
}
//===================================================================
void ZMixedTerm::zh_setName()
{
    zv_name = zv_window1->zp_windowName() +"*"+ zv_window2->zp_windowName();
}
//===================================================================
void ZMixedTerm::zh_connectToWindows()
{
    if(zv_window1)
    {
        connect(zv_window1, &ZCalibrationWindow::destroyed,
                this, &ZMixedTerm::zh_onWindowDestroying);
        connect(zv_window1, &ZCalibrationWindow::zg_windowNameChanged,
                this, &ZMixedTerm::zh_updateTermNameForWindowName);
        connect(this, &ZMixedTerm::zg_requestWindow1Intensity,
                zv_window1, &ZCalibrationWindow::zp_calcWindowIntensity);
        connect(zv_window1, &ZCalibrationWindow::zg_windowMarginsChanged,
                this, &ZMixedTerm::zg_termWindowMarginChanged);
        connect(zv_window1, &ZCalibrationWindow::zg_windowTypeChanged,
                this, &ZMixedTerm::zh_onWindowTypeChange);
    }

    if(zv_window2)
    {
        connect(zv_window2, &ZCalibrationWindow::destroyed,
                this, &ZMixedTerm::zh_onWindowDestroying);
        connect(zv_window2, &ZCalibrationWindow::zg_windowNameChanged,
                this, &ZMixedTerm::zh_updateTermNameForWindowName);
        connect(this, &ZMixedTerm::zg_requestWindow2Intensity,
                zv_window2, &ZCalibrationWindow::zp_calcWindowIntensity);
        connect(zv_window2, &ZCalibrationWindow::zg_windowMarginsChanged,
                this, &ZMixedTerm::zg_termWindowMarginChanged);
        connect(zv_window2, &ZCalibrationWindow::zg_windowTypeChanged,
                this, &ZMixedTerm::zh_onWindowTypeChange);
    }

}
//===================================================================
