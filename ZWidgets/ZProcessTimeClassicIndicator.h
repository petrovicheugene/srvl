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
    void zp_setPercentTextOnProgressBar(bool);
    void zp_applyProgressBarOptions(const ZProgressBarOptions& options);


signals:

public slots:

private:

    // VARS
    QProgressBar* zv_progressBar; /*!< TODO: describe */
    QLabel* zv_processNameLabel; /*!< TODO: describe */
    QLabel* zv_percentElapsedLabel; /*!< TODO: describe */
    QLabel* zv_processDurationLabel; /*!< TODO: describe */
    QLabel* zv_timeLeftLabel; /*!< TODO: describe */

    int zv_progressBarStandardHeight;


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
#endif // ZPROCESSTIMEINDICATOR_H
