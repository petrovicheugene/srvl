//============================================================
#ifndef ZDASHBOARD_H
#define ZDASHBOARD_H
//============================================================
#include <QWidget>
#include "ZDashboardSettings.h"
#include "ZProgressBarOptions.h"
#include "ZMeasuringManager.h"
//============================================================
class ZAbstractProcessTimeIndicator;
class ZStartStopButtonWidget;
class QLayout;
class QLabel;
class ZSeriesLabelWidget;
//============================================================
/*!
 \brief

*/
class ZDashboard : public QWidget
{
    Q_OBJECT
public:
    /*!
     \brief

     \param parent
    */
    explicit ZDashboard(QWidget *parent = 0);
    void zp_applyDashboardSettings(const ZDashboardSettings& settings);

    ZDashboardSettings::DashboardLocation zp_location() const;
    void zp_setMainLayoutMargin(int margin);

    void zp_setFontPixelSizeMinimumMaximum(int min, int max);
    void zp_setProgressBarSizeMinimumMaximum(int min, int max);

signals:

    void zg_startSeries() const;
    void zg_stopSeries() const;

public slots:

    void zp_setMeasuringState(ZMeasuringState measuringState);
    void zp_setSeriesTaskDirty(bool dirty);

private:

    // VARS
    ZSeriesLabelWidget* zv_seriesLabelWidget;
    ZAbstractProcessTimeIndicator* zv_seriesTimeIndicator; /*!< TODO: describe */
    ZAbstractProcessTimeIndicator* zv_sampleTimeIndicator; /*!< TODO: describe */
    ZStartStopButtonWidget* zv_startStopButtonWidget;

    ZDashboardSettings zv_settings;

    int zv_fontPixelSizeMinimum;
    int zv_fontPixelSizeMaximum;
    int zv_progressBarSiteMinimum;
    int zv_progressBarSiteMaximum;

    // FUNCS

    /*!
     \brief

    */
    void zh_createControls();
    QLayout* zh_createVerticalClassic();
    QLayout* zh_createHorizontalClassic();
    QLayout* zh_createVerticalRound();
    QLayout* zh_createHorizontalRound();

    void zh_createConnections();
    void zh_saveControlState();
    void zh_applyControlState();
    void zh_initProgressBarOptons(const ZDashboardSettings& settings,
                                  ZProgressBarOptions& options);
    void zh_calculateFontSizeScaleFactors(qreal min, qreal max, qreal& K, qreal& A);
};
//============================================================
#endif // ZDASHBOARD_H
