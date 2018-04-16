//=============================================================
#ifndef ZPROCESSTIMEINDICATOR_H
#define ZPROCESSTIMEINDICATOR_H
//=============================================================
#include "ZAbstractProcessTimeIndicator.h"
//=============================================================
class QLabel;
class QProgressBar;
class QVBoxLayout;
//=============================================================
/*!
 \brief

*/
class ZProcessTimeClassicIndicator : public ZAbstractProcessTimeIndicator
{
    Q_OBJECT
public:
    /*!
     \brief

     \param parent
    */
    explicit ZProcessTimeClassicIndicator(QWidget *parent = 0);

    /*!
     \brief

     \param bool
    */
    void zp_setPercentTextOnProgressBar(bool) override;
    void zp_applyProgressBarOptions(const ZProgressBarOptions& options) override;
    void zp_reset() override;
    void zp_setRange(double min, double max) override;
    void zp_setValue(double value) override;


signals:

public slots:

private:

    // VARS
    QProgressBar* zv_progressBar; /*!< TODO: describe */

    QLabel* zv_percentElapsedLabel; /*!< TODO: describe */

    int zv_progressBarStandardHeight;


    // FUNCS

    /*!
     \brief

    */
    void zh_createComponents();
    /*!
     \brief

    */

};
//=============================================================
#endif // ZPROCESSTIMEINDICATOR_H
