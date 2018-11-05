//=============================================================
#ifndef ZPROCESSTIMEROUNDINDICATOR_H
#define ZPROCESSTIMEROUNDINDICATOR_H
//=============================================================
#include "ZAbstractProcessTimeIndicator.h"
//=============================================================
class QLabel;
class QRoundProgressBar;
class QHBoxLayout;
//=============================================================
/*!
 \brief

*/
class ZProcessTimeRoundIndicator : public ZAbstractProcessTimeIndicator
{
    Q_OBJECT
public:
    /*!
     \brief

     \param parent
    */
    explicit ZProcessTimeRoundIndicator(QWidget *parent = 0);

    void zp_setPercentTextOnProgressBar(bool visible);
    void zp_applyProgressBarOptions(const ZProgressBarOptions& options);
    void zp_reset() override;
    void zp_setRange(double min, double max) override;
    void zp_setValue(double value) override;

signals:

public slots:

protected slots:

    void zh_updateSeparatorDashLabel();

private:

    // VARS

    QRoundProgressBar* zv_progressBar; /*!< TODO: describe */

    QLabel* zv_separatorDashLabel;
    // FUNCS
    void zh_createComponents();

};
//=============================================================
#endif // ZPROCESSTIMEROUNDINDICATOR_H
