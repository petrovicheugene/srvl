//====================================================
#include "ZSeriesLabelWidget.h"
#include <QLabel>
#include <QFont>
#include <QVBoxLayout>
#include <QHBoxLayout>
//====================================================
ZSeriesLabelWidget::ZSeriesLabelWidget(Qt::Orientation orientation,
                                       QWidget *parent)
    : QWidget(parent)
{
    zh_createComponents();
}
//====================================================
void ZSeriesLabelWidget::zh_createComponents()
{
    QVBoxLayout* mainLayout = new QVBoxLayout;
    mainLayout->setMargin(0);
    setLayout(mainLayout);

    zv_seriesLabel = new QLabel(this);
    zv_seriesLabel->setWordWrap(true);
    mainLayout->addWidget(zv_seriesLabel);
}
//====================================================
void ZSeriesLabelWidget::zp_applyDashBoardSettings(const ZDashboardSettings& settings)
{
    QFont font = zv_seriesLabel->font();
    int fontSize = 10 + settings.zv_progressBarSize / 5;
    font.setPixelSize(fontSize);
    QPalette palette = zv_seriesLabel->palette();

    palette.setColor(QPalette::WindowText, settings.zv_textColor);

    zv_seriesLabel->setFont(font);
    zv_seriesLabel->setPalette(palette);
}
//====================================================
void ZSeriesLabelWidget::zp_setSeriesName(const QString& seriesName)
{
    zv_seriesNameString = seriesName;
    zv_seriesLabel->setText(zv_seriesNameString);
}
//====================================================
void ZSeriesLabelWidget::zp_setSeriesDirty(bool dirty)
{
    if(dirty)
    {
        zv_seriesLabel->setText("* " +zv_seriesNameString);
    }
    else
    {
        zv_seriesLabel->setText(zv_seriesNameString);
    }
}
//====================================================
