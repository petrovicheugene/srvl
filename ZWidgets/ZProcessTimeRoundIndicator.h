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

signals:

public slots:

private:

    // VARS

    QRoundProgressBar* zv_progressBar; /*!< TODO: describe */
    QLabel* zv_processNameLabel; /*!< TODO: describe */
    QLabel* zv_processDurationLabel; /*!< TODO: describe */
    QLabel* zv_timeLeftLabel; /*!< TODO: describe */
    QLabel* zv_separatorDashLabel;
    // FUNCS
    /*!
     \brief

    */
    void zh_createComponents();
    /*!
     \brief

    */
    void testFilling();

};
//=============================================================
#endif // ZPROCESSTIMEROUNDINDICATOR_H
