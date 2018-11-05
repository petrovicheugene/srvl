//===================================================================
#ifndef ZSIMPLETERM_H
#define ZSIMPLETERM_H
//===================================================================
#include "ZAbstractTerm.h"
#include "ZCalibrationWindow.h"
//===================================================================

//===================================================================
class ZSimpleTerm : public ZAbstractTerm
{
    Q_OBJECT
public:
    explicit ZSimpleTerm(const ZCalibrationWindow* window,
                         ZCalibration *parent);

    // FUNCS
    bool zp_calcValue(const ZAbstractSpectrum*, qreal&) override;
    bool zp_calcTermVariablePart(const ZAbstractSpectrum*, qreal &value) override; // w/o factor
    bool zp_termBelongsToWindow(const ZCalibrationWindow*) const override;
    QList<qint64> zp_termWindowIdList() const;
    const ZCalibrationWindow* zp_window() const;

signals:

    void zg_inquiryWindowIntensity(const QObject* spectrum, qreal& intensityValue, bool useBuffer,  bool* ok = 0);

protected slots:

    virtual void zh_updateTermNameForWindowName() override;
    void zh_onWindowTypeChange(ZCalibrationWindow::WindowType previousType,
                               ZCalibrationWindow::WindowType currentType) override;
private:

    // VARS
    const ZCalibrationWindow* zv_window;


    // FUNCS

    void zh_connectToWindow(const ZCalibrationWindow* window);

};
//===================================================================
#endif // ZSIMPLETERM_H
