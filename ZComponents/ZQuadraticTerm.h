//===================================================================
#ifndef ZQUADRATICTERM_H
#define ZQUADRATICTERM_H
//===================================================================
#include "ZAbstractTerm.h"
#include "ZCalibrationWindow.h"

//===================================================================

//===================================================================
class ZQuadraticTerm : public ZAbstractTerm
{
    Q_OBJECT
public:
    explicit ZQuadraticTerm(const ZCalibrationWindow* window,
                            ZCalibration *parent);

    // FUNCS
    bool zp_calcValue(const ZAbstractSpectrum*, qreal&) override;
    bool zp_calcTermVariablePart(const ZAbstractSpectrum*, qreal &value) override; // w/o factor
    bool zp_termBelongsToWindow(const ZCalibrationWindow*) const override;
    QList<qint64> zp_termWindowIdList() const;
    const ZCalibrationWindow* zp_window() const;

signals:

    void zg_inquiryWindowIntensity(const QObject* spectrum, qreal& intensityValue, bool keepBufferClean, bool* ok = 0);

protected slots:

    virtual void zh_updateTermNameForWindowName() override;
    virtual void zh_onWindowTypeChange(ZCalibrationWindow::WindowType previousType, ZCalibrationWindow::WindowType currentType) override;

private:

    // VARS
    const ZCalibrationWindow* zv_window;

    // FUNCS
    void zh_setName();
    void zh_connectToWindow();

};
//===================================================================
#endif // ZQUADRATICTERM_H
