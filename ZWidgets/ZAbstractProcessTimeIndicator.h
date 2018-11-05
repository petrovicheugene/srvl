//=======================================================
#ifndef ZABSTRACTPROCESSTIMEINDICATOR_H
#define ZABSTRACTPROCESSTIMEINDICATOR_H
//=======================================================
#include <QWidget>
#include <QColor>
#include <ZProgressBarOptions.h>
//=======================================================
class QLabel;
//=======================================================
class ZAbstractProcessTimeIndicator : public QWidget
{
    Q_OBJECT
public:
    explicit ZAbstractProcessTimeIndicator(QWidget *parent = 0);

    void zp_setMainLayoutMargin(int margin);
    void zp_setMainLayoutSpacing(int spacing);
    virtual void zp_setPercentTextOnProgressBar(bool) = 0;
    virtual void zp_applyProgressBarOptions(const ZProgressBarOptions& options) = 0;

    virtual void zp_reset() = 0;
    virtual void zp_setRange(double min, double max) = 0;
    virtual void zp_setValue(double value) = 0;

signals:

public slots:

    void zp_setProcessNameString(const QString& name);

protected:

    // VARS
    QLabel* zv_processNameLabel; /*!< TODO: describe */
    QLabel* zv_timeLeftLabel; /*!< TODO: describe */
    QLabel* zv_processDurationLabel; /*!< TODO: describe */


    // FUNCS
    void zh_createComponents();
    QString zh_convertTimeToString(double time);
    void zh_recalcTimeLabelMinWidth();

};
//=======================================================
#endif // ZABSTRACTPROCESSTIMEINDICATOR_H
