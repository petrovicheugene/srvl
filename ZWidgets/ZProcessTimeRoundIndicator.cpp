//=============================================================
#include "ZProcessTimeRoundIndicator.h"
#include "ZGLConstantsAndDefines.h"
#include "QRoundProgressBar.h"

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
//=============================================================
ZProcessTimeRoundIndicator::ZProcessTimeRoundIndicator(QWidget *parent)
    : ZAbstractProcessTimeIndicator(parent)
{
    zh_createComponents();
    testFilling();
}
//=============================================================
void ZProcessTimeRoundIndicator::zp_setPercentTextOnProgressBar(bool visible)
{

}
//=============================================================
void ZProcessTimeRoundIndicator::zp_applyProgressBarOptions(const ZProgressBarOptions& options)
{
    // color
    QPalette pal = zv_progressBar->palette();
    pal.setColor(QPalette::Highlight, options.zv_progressBarColor);
    pal.setColor(QPalette::Text, options.zv_textColor);
    zv_progressBar->setPalette(pal);

    // style
    if(options.zv_progressBarStyle == ZProgressBarOptions::PS_REGULAR)
    {
        zv_progressBar->setBarStyle(QRoundProgressBar::StyleDonut);
    }
    else // options.zv_style == PS_SLIM
    {
        zv_progressBar->setBarStyle(QRoundProgressBar::StyleLine);
    }

    // size
    zv_progressBar->setMinimumSize(QSize(options.zv_progressBarSiteSize, options.zv_progressBarSiteSize));
    // size to label font
    QFont font = zv_processNameLabel->font();
    font.setPixelSize(options.zv_fontPixelSize);
    QPalette palette = zv_processNameLabel->palette();
    palette.setColor(QPalette::WindowText, options.zv_textColor);

    zv_processNameLabel->setFont(font);
    zv_processNameLabel->setPalette(palette);

    zv_processDurationLabel->setFont(font);
    zv_processDurationLabel->setPalette(palette);

    zv_timeLeftLabel->setFont(font);
    zv_timeLeftLabel->setPalette(palette);

    zv_separatorDashLabel->setFont(font);
    zv_separatorDashLabel->setPalette(palette);

}
//=============================================================
void ZProcessTimeRoundIndicator::zh_createComponents()
{
    QHBoxLayout* mainLayout = new QHBoxLayout(this);
    setLayout(mainLayout);

    // progress bar part
    zv_progressBar = new QRoundProgressBar(this);

    mainLayout->addWidget(zv_progressBar);

    // label part
    QVBoxLayout* rightLayout = new QVBoxLayout(this);
    mainLayout->addLayout(rightLayout);

    // process name
    zv_processNameLabel = new QLabel(this);
    rightLayout->addWidget(zv_processNameLabel);

    // time label part
    QHBoxLayout* timeLabelLayout = new QHBoxLayout(this);
    rightLayout->addLayout(timeLabelLayout);

    zv_processDurationLabel = new QLabel(this);
    timeLabelLayout->addWidget(zv_processDurationLabel);

    zv_separatorDashLabel = new QLabel(this);
    zv_separatorDashLabel->setText(" - ");
    timeLabelLayout->addWidget(zv_separatorDashLabel);

    zv_timeLeftLabel = new QLabel(this);
    timeLabelLayout->addWidget(zv_timeLeftLabel);
    timeLabelLayout->addStretch();

}
//=============================================================
void ZProcessTimeRoundIndicator::testFilling()
{
    zv_processNameLabel->setText("Sample 1");
    zv_processDurationLabel->setText("5:00");
    zv_timeLeftLabel->setText("3:43");

    zv_progressBar->setMinimum(0);
    zv_progressBar->setMaximum(300);

    zv_progressBar->setValue(3*60 + 43);

}
//=============================================================
