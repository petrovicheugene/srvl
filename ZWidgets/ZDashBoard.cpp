//============================================================
#include "ZDashboard.h"
#include "ZProcessTimeClassicIndicator.h"
#include "ZProcessTimeRoundIndicator.h"
#include "ZStartStopButtonWidget.h"
#include "ZSeriesLabelWidget.h"

#include <QDebug>
#include <QVBoxLayout>
#include <QHBoxLayout>

#include <QPushButton>
//============================================================
ZDashboard::ZDashboard(QWidget *parent) : QWidget(parent)
{
    zv_seriesTimeIndicator = 0;
    zv_sampleTimeIndicator = 0;
    zv_startStopButtonWidget = 0;
    zv_seriesLabelWidget = 0;

    zv_fontPixelSizeMinimum = 10;
    zv_fontPixelSizeMaximum = 30;
    zv_progressBarSiteMinimum = 50;
    zv_progressBarSiteMaximum = 150;

    zh_createControls();
    zh_createConnections();

    // button state 0-stopped 1-started 2-suspended
    zv_startStopButtonWidget->zp_setButtonState(0);

}
//============================================================
void ZDashboard::zp_applyDashboardSettings(const ZDashboardSettings& settings)
{
    zv_settings = settings;
    zh_createControls();
    //zh_createConnections();
}
//============================================================
ZDashboardSettings::DashboardLocation ZDashboard::zp_location() const
{
    return zv_settings.zv_dashboardLocation;
}
//============================================================
void ZDashboard::zp_setMainLayoutMargin(int margin)
{
    QLayout* mainLayout = layout();

    if(mainLayout == 0 || margin < 0)
    {
        return;
    }

    mainLayout->setMargin(margin);
}
//============================================================
void ZDashboard::zp_setFontPixelSizeMinimumMaximum(int min, int max)
{
    if(min < 0 || max < 0)
    {
        return;
    }

    if(min > max)
    {
        qSwap(min, max);
    }

    zv_fontPixelSizeMinimum = min;
    zv_fontPixelSizeMaximum = max;
}
//============================================================
void ZDashboard::zp_setProgressBarSizeMinimumMaximum(int min, int max)
{
    if(min < 0 || max < 0)
    {
        return;
    }

    if(min > max)
    {
        qSwap(min, max);
    }

    zv_progressBarSiteMinimum = min;
    zv_progressBarSiteMaximum = max;

}
//============================================================
void ZDashboard::zp_setMeasuringState(ZMeasuringState measuringState)
{
    QString seriesNameString;
    if(measuringState.zp_seriesTaskDirty())
    {
        seriesNameString = "*";
    }

    seriesNameString += measuringState.zp_currentSeriesName();
    zv_seriesTimeIndicator->zp_setProcessNameString(seriesNameString);
    if(seriesNameString.isEmpty())
    {
        zv_seriesTimeIndicator->zp_reset();
    }
    else
    {
        zv_seriesTimeIndicator->zp_setRange(0, measuringState.zp_seriesDuration());
        zv_seriesTimeIndicator->zp_setValue(measuringState.zp_seriesTimePassed());
    }

    zv_sampleTimeIndicator->zp_setProcessNameString(measuringState.zp_currentSampleName());
    if(measuringState.zp_currentSampleName().isEmpty())
    {
        zv_sampleTimeIndicator->zp_reset();
    }
    else
    {
        zv_sampleTimeIndicator->zp_setRange(0, measuringState.zp_sampleDuration());
        zv_sampleTimeIndicator->zp_setValue( measuringState.zp_sampleTimePassed());
    }

    zv_startStopButtonWidget->zp_setButtonState(measuringState.zp_measuringAction());

}
//============================================================
void ZDashboard::zh_createControls()
{
    // old layout
    QLayout* oldMainLayout = layout();

    // the start-stop button have to be created once
    if(zv_startStopButtonWidget == 0)
    {
        zv_startStopButtonWidget = new ZStartStopButtonWidget();
    }

    //    if(zv_seriesLabelWidget == 0)
    //    {
    //        zv_seriesLabelWidget = new ZSeriesLabelWidget();
    //    }

    //    zv_seriesLabelWidget->zp_applyDashBoardSettings(zv_settings);

    // Sample time indicator
    // delete previous if it exists
    if(zv_sampleTimeIndicator != 0)
    {
        delete zv_sampleTimeIndicator;
    }

    // Total time indicator
    // delete previous if it exists
    if(zv_seriesTimeIndicator != 0)
    {
        delete zv_seriesTimeIndicator;
    }

    // create new indicators and layouts
    QLayout* mainLayout;
    if(zv_settings.zv_dashboardLocation == ZDashboardSettings::DBL_BOTTOM ||
            zv_settings.zv_dashboardLocation == ZDashboardSettings::DBL_TOP)
    {
        zv_startStopButtonWidget->zp_setOrientation(Qt::Vertical);

        if(zv_settings.zv_dashboardStyle == ZDashboardSettings::DBS_CLASSIC ||
                zv_settings.zv_dashboardStyle == ZDashboardSettings::DBS_CLASSIC_SLIM)
        {
            mainLayout = zh_createHorizontalClassic();
        }
        else
        {
            mainLayout = zh_createHorizontalRound();
        }
    }
    else
    {
        zv_startStopButtonWidget->zp_setOrientation(Qt::Horizontal);

        if(zv_settings.zv_dashboardStyle == ZDashboardSettings::DBS_CLASSIC ||
                zv_settings.zv_dashboardStyle == ZDashboardSettings::DBS_CLASSIC_SLIM)
        {
            mainLayout = zh_createVerticalClassic();
        }
        else
        {
            mainLayout = zh_createVerticalRound();
        }
    }

    // delete old layout
    if(oldMainLayout != 0)
    {
        delete oldMainLayout;
    }

    // replace layout
    setLayout(mainLayout);
}
//============================================================
QLayout* ZDashboard::zh_createVerticalClassic()
{
    // progress bar options
    ZProgressBarOptions options;
    zh_initProgressBarOptons(zv_settings, options);

    zv_sampleTimeIndicator = new ZProcessTimeClassicIndicator();
    //zv_sampleTimeIndicator->setMinimumWidth(zv_sampleTimeIndicator->sizeHint().width() * 2);

    // adjust progress bar
    zv_sampleTimeIndicator->zp_setMainLayoutMargin(0);
    zv_sampleTimeIndicator->zp_setMainLayoutSpacing(0);
    zv_sampleTimeIndicator->zp_applyProgressBarOptions(options);
    zv_sampleTimeIndicator->zp_setPercentTextOnProgressBar(false);

    zv_seriesTimeIndicator = new ZProcessTimeClassicIndicator();
    //zv_totalTimeIndicator->setMinimumWidth(zv_totalTimeIndicator->sizeHint().width() * 2);

    // adjust progress bar
    zv_seriesTimeIndicator->zp_setMainLayoutMargin(0);
    zv_seriesTimeIndicator->zp_setMainLayoutSpacing(0);
    zv_seriesTimeIndicator->zp_applyProgressBarOptions(options);
    zv_seriesTimeIndicator->zp_setPercentTextOnProgressBar(false);

    //    QVBoxLayout* indicatorLayout = new QVBoxLayout();
    //    mainLayout->addLayout(indicatorLayout);

    // placing
    QVBoxLayout* mainLayout = new QVBoxLayout();
    // mainLayout->addWidget(zv_seriesLabelWidget);
    mainLayout->addWidget(zv_seriesTimeIndicator);
    mainLayout->addWidget(zv_sampleTimeIndicator);
    mainLayout->addWidget(zv_startStopButtonWidget);
    mainLayout->addStretch();

    return mainLayout;
}
//============================================================
QLayout* ZDashboard::zh_createHorizontalClassic()
{
    // progress bar options
    ZProgressBarOptions options;
    zh_initProgressBarOptons(zv_settings, options);

    zv_sampleTimeIndicator = new ZProcessTimeClassicIndicator();
    // doubling sizehint
    //zv_sampleTimeIndicator->setMinimumWidth(zv_sampleTimeIndicator->sizeHint().width() * 2);

    // adjust progress bar
    zv_sampleTimeIndicator->zp_setMainLayoutMargin(0);
    zv_sampleTimeIndicator->zp_setMainLayoutSpacing(0);
    zv_sampleTimeIndicator->zp_setPercentTextOnProgressBar(false);
    zv_sampleTimeIndicator->zp_applyProgressBarOptions(options);


    zv_seriesTimeIndicator = new ZProcessTimeClassicIndicator();
    // doubling sizehint
    //zv_totalTimeIndicator->setMinimumWidth(zv_totalTimeIndicator->sizeHint().width() * 2);

    // adjust progress bar
    zv_seriesTimeIndicator->zp_setMainLayoutMargin(0);
    zv_seriesTimeIndicator->zp_setMainLayoutSpacing(0);
    zv_seriesTimeIndicator->zp_setPercentTextOnProgressBar(false);
    zv_seriesTimeIndicator->zp_applyProgressBarOptions(options);

    // placing
    QVBoxLayout* vMainLayout = new QVBoxLayout();
    QHBoxLayout* mainLayout = new QHBoxLayout();
    QVBoxLayout* indicatorLayout = new QVBoxLayout();

    // vMainLayout->addWidget(zv_seriesLabelWidget);
    vMainLayout->addLayout(mainLayout);
    mainLayout->addLayout(indicatorLayout);

    indicatorLayout->addWidget(zv_seriesTimeIndicator);
    indicatorLayout->addWidget(zv_sampleTimeIndicator);
    mainLayout->addWidget(zv_startStopButtonWidget);

    // add stretch
    mainLayout->addStretch();

    return vMainLayout;
}
//============================================================
QLayout* ZDashboard::zh_createVerticalRound()
{
    // progress bar options
    ZProgressBarOptions options;
    zh_initProgressBarOptons(zv_settings, options);

    zv_sampleTimeIndicator = new ZProcessTimeRoundIndicator();
    zv_sampleTimeIndicator->zp_setMainLayoutMargin(0);
    zv_sampleTimeIndicator->zp_applyProgressBarOptions(options);
    zv_sampleTimeIndicator->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);

    zv_seriesTimeIndicator = new ZProcessTimeRoundIndicator();
    zv_seriesTimeIndicator->zp_setMainLayoutMargin(0);
    zv_seriesTimeIndicator->zp_applyProgressBarOptions(options);
    zv_seriesTimeIndicator->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);


    // placing
    QVBoxLayout* mainLayout = new QVBoxLayout();
    mainLayout->setObjectName("VERTICAL ROUND LAYOUT");
    //mainLayout->addWidget(zv_seriesLabelWidget);
    mainLayout->addWidget(zv_seriesTimeIndicator);
    mainLayout->addWidget(zv_sampleTimeIndicator);
    mainLayout->addWidget(zv_startStopButtonWidget);
    mainLayout->addStretch();

    return mainLayout;
}
//============================================================
QLayout* ZDashboard::zh_createHorizontalRound()
{
    // progress bar options
    ZProgressBarOptions options;
    zh_initProgressBarOptons(zv_settings, options);

    zv_sampleTimeIndicator = new ZProcessTimeRoundIndicator();
    zv_sampleTimeIndicator->zp_setMainLayoutMargin(0);
    zv_sampleTimeIndicator->zp_applyProgressBarOptions(options);
    zv_sampleTimeIndicator->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);

    zv_seriesTimeIndicator = new ZProcessTimeRoundIndicator();
    zv_seriesTimeIndicator->zp_setMainLayoutMargin(0);
    zv_seriesTimeIndicator->zp_applyProgressBarOptions(options);
    zv_seriesTimeIndicator->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);


    // placing
    QVBoxLayout* vMainLayout = new QVBoxLayout();
    QHBoxLayout* mainLayout = new QHBoxLayout();
    //vMainLayout->addWidget(zv_seriesLabelWidget);
    vMainLayout->addLayout(mainLayout);

    mainLayout->addWidget(zv_seriesTimeIndicator);
    mainLayout->addSpacing(10);
    mainLayout->addWidget(zv_sampleTimeIndicator);
    mainLayout->addWidget(zv_startStopButtonWidget);
    mainLayout->addStretch();

    return vMainLayout;
}
//============================================================
void ZDashboard::zh_createConnections()
{
    connect(zv_startStopButtonWidget, &ZStartStopButtonWidget::zg_start,
            this, &ZDashboard::zg_startSeries);
    connect(zv_startStopButtonWidget, &ZStartStopButtonWidget::zg_stop,
            this, &ZDashboard::zg_stopSeries);

}
//============================================================
void ZDashboard::zh_initProgressBarOptons(const ZDashboardSettings& settings,
                                          ZProgressBarOptions& options)
{
    options.zv_progressBarColor = settings.zv_progressBarColor;
    options.zv_textColor = settings.zv_textColor;

    if(settings.zv_dashboardStyle == ZDashboardSettings::DBS_CLASSIC ||
            settings.zv_dashboardStyle == ZDashboardSettings::DBS_DONUT)
    {
        options.zv_progressBarStyle = ZProgressBarOptions::PS_REGULAR;
    }
    else
    {
        options.zv_progressBarStyle = ZProgressBarOptions::PS_SLIM;
    }

    // recalculate settings.zv_progressBarSize to zv_fontPixelSize and zv_progressBarSiteSize
    qreal K;
    qreal A;

    zh_calculateFontSizeScaleFactors(static_cast<qreal>(zv_progressBarSiteMinimum),
                                     static_cast<qreal>(zv_progressBarSiteMaximum), K, A);

    options.zv_progressBarSiteSize = static_cast<int>(settings.zv_progressBarSize*K + A) ;

    zh_calculateFontSizeScaleFactors(static_cast<qreal>(zv_fontPixelSizeMinimum),
                                     static_cast<qreal>(zv_fontPixelSizeMaximum), K, A);

    options.zv_fontPixelSize = static_cast<int>(settings.zv_progressBarSize*K + A);
}
//============================================================
void ZDashboard::zh_calculateFontSizeScaleFactors(qreal min, qreal max, qreal& K, qreal& A)
{
    if(ZDashboardSettings::zv_progressBarMinimumSize
            == ZDashboardSettings::zv_progressBarMaximumSize)
    {
        K = 0.0;
        A = min;
        return;
    }

    A = (max*ZDashboardSettings::zv_progressBarMinimumSize - min*ZDashboardSettings::zv_progressBarMaximumSize)
            / (ZDashboardSettings::zv_progressBarMinimumSize - ZDashboardSettings::zv_progressBarMaximumSize);

    if(ZDashboardSettings::zv_progressBarMinimumSize != 0)
    {
        K = (min - A) / ZDashboardSettings::zv_progressBarMinimumSize;
    }
    else
    {
        K = (max - A) / ZDashboardSettings::zv_progressBarMaximumSize;
    }
}
//============================================================
