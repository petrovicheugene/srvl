//=============================================================
#include "ZProcessTimeClassicIndicator.h"
#include "ZGeneral.h"

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QProgressBar>
#include <QLabel>
//=============================================================
ZProcessTimeClassicIndicator::ZProcessTimeClassicIndicator(QWidget *parent)
    : ZAbstractProcessTimeIndicator(parent)
{
    zh_createComponents();
    zp_reset();
}
//=============================================================
void ZProcessTimeClassicIndicator::zp_setPercentTextOnProgressBar(bool visible)
{
    zv_progressBar->setTextVisible(visible);
    //zv_percentElapsedLabel->setVisible(!visible);
}
//=============================================================
void ZProcessTimeClassicIndicator::zp_reset()
{
    zv_progressBar->setRange(1.0,0.0);
    zv_progressBar->setValue(0);
}
//=============================================================
void ZProcessTimeClassicIndicator::zp_applyProgressBarOptions(const ZProgressBarOptions& options)
{
    // style
    if(options.zv_progressBarStyle == ZProgressBarOptions::PS_REGULAR)
    {
        zv_progressBar->setMaximumHeight(zv_progressBarStandardHeight);
    }
    else // options.zv_style == PS_SLIM
    {
        zv_progressBar->setMaximumHeight(zv_progressBarStandardHeight / 4);
    }

    // size to label font

    QFont font = zv_processNameLabel->font();
    font.setPixelSize(options.zv_fontPixelSize);
    QPalette palette = zv_processNameLabel->palette();
    palette.setColor(QPalette::WindowText, options.zv_textColor);

    zv_processNameLabel->setFont(font);
    zv_processNameLabel->setPalette(palette);

    zv_processDurationLabel->setFont(font);
    zv_processDurationLabel->setPalette(palette);

    zv_percentElapsedLabel->setFont(font);
    zv_percentElapsedLabel->setPalette(palette);

    zv_timeLeftLabel->setFont(font);
    zv_timeLeftLabel->setPalette(palette);

    // color
//    QPalette pal = zv_progressBar->palette();
//    pal.setBrush(QPalette::Highlight, options.zv_barColor);
//    zv_progressBar->setPalette(pal);

}
//=============================================================
void ZProcessTimeClassicIndicator::zh_createComponents()
{
    QVBoxLayout* mainLayout = new QVBoxLayout(this);
    setLayout(mainLayout);

    // top level
    QHBoxLayout* topLayout = new QHBoxLayout(this);
    topLayout->setMargin(0);
    mainLayout->addLayout(topLayout);

    //zv_processNameLabel = new QLabel(this);
    topLayout->addWidget(zv_processNameLabel);
    topLayout->addStretch();
    topLayout->addSpacing(10);

    zv_percentElapsedLabel = new QLabel(this);
    topLayout->addWidget(zv_percentElapsedLabel);

    // middle level
    zv_progressBar = new QProgressBar(this);
    mainLayout->addWidget(zv_progressBar);

    // bottom level
    QHBoxLayout* bottomLayout = new QHBoxLayout(this);
    bottomLayout->setMargin(0);
    mainLayout->addLayout(bottomLayout);

    bottomLayout->addWidget(zv_processDurationLabel);
    bottomLayout->addStretch();
    bottomLayout->addSpacing(10);
    bottomLayout->addWidget(zv_timeLeftLabel);

    // save standard progressBar height
    zv_progressBarStandardHeight = zv_progressBar->sizeHint().height();
}
//=============================================================
