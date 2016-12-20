//===================================================================
#ifndef ZCROSSPRODUCTTERM_H
#define ZCROSSPRODUCTTERM_H
//===================================================================
#include "ZAbstractTerm.h"
//===================================================================
class ZCalibrationWindow;
//===================================================================
class ZMixedTerm : public ZAbstractTerm
{
    Q_OBJECT
public:
    explicit ZMixedTerm(const ZCalibrationWindow *window1,
                        const ZCalibrationWindow *window2,
                        ZCalibration *parent);

    // FUNCS
    bool zp_calcValue(const ZAbstractSpectrum*, qreal&) override;
    bool zp_calcTermVariablePart(const ZAbstractSpectrum*, qreal &value) override; // w/o factor
    bool zp_termBelongsToWindow(const ZCalibrationWindow*) const override;
    QList<qint64> zp_termWindowIdList() const;

    //QString zp_termName() const override;

signals:

    void zg_requestWindow1Intensity(const QObject* spectrum, qreal& intensityValue, bool useBuffer,  bool* ok = 0);
    void zg_requestWindow2Intensity(const QObject* spectrum, qreal& intensityValue, bool useBuffer,  bool* ok = 0);

protected slots:

    virtual void zh_updateTermNameForWindowName() override;
    virtual void zh_onWindowTypeChange(ZCalibrationWindow::WindowType previousType,
                                       ZCalibrationWindow::WindowType currentType) override;

private:

    // VARS
    const ZCalibrationWindow* zv_window1;
    const ZCalibrationWindow* zv_window2;

    // FUNCS
    void zh_setName();
    void zh_connectToWindows();

};
//===================================================================
#endif // ZCROSSPRODUCTTERM_H
