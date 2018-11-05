//============================================================
#include "ZLabeledSliderWidget.h"

#include <QFont>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QLabel>
//============================================================
ZLabeledSliderWidget::ZLabeledSliderWidget(Qt::Orientation orientation, QWidget *parent)
    : QWidget(parent)
{
    zh_createComponents();
    zh_createConnctions();
    zv_slider->setOrientation(orientation);
    zh_rebuildLayout();
}
//============================================================
QSlider* ZLabeledSliderWidget::zp_slider() const
{
    return zv_slider;
}
//============================================================
void ZLabeledSliderWidget::zp_setMinimumCaption(const QString& caption)
{
    zv_minimumLabel->setText(QString("<font color=%1>%2</font>").arg(zv_htmlColorString, caption));
    zh_rebuildLayout();
}
//============================================================
void ZLabeledSliderWidget::zp_setMaximumCaption(const QString& caption)
{
    zv_maximumLabel->setText(QString("<font color=%1>%2</font>").arg(zv_htmlColorString, caption));
    zh_rebuildLayout();
}
//============================================================
void ZLabeledSliderWidget::zp_setTextColor(QColor color)
{
    QPalette palette = zv_minimumLabel->palette();
    palette.setColor(QPalette::WindowText, color);
    zv_minimumLabel->setPalette(palette);
    zv_maximumLabel->setPalette(palette);
}
//============================================================
void ZLabeledSliderWidget::zp_setTextHTMLColorString(const QString& htmlColorString)
{
    zv_htmlColorString = htmlColorString;
    zv_maximumLabel->setText(QString("<font color=%1>%2</font>").arg(zv_htmlColorString, zv_maximumLabel->text()));
    zv_minimumLabel->setText(QString("<font color=%1>%2</font>").arg(zv_htmlColorString, zv_minimumLabel->text()));
    zh_rebuildLayout();
}
//============================================================
int ZLabeledSliderWidget::zp_value() const
{
    return zv_slider->value();
}
//============================================================
void ZLabeledSliderWidget::zp_setValue(int value)
{
    zv_slider->setValue(value);
}
//============================================================
void ZLabeledSliderWidget::zp_setFontBold(bool boldFlag)
{
    QFont font = zv_minimumLabel->font();
    font.setBold(boldFlag);
    zv_minimumLabel->setFont(font);
    zv_maximumLabel->setFont(font);
    zh_rebuildLayout();
}
//============================================================
void ZLabeledSliderWidget::zp_setFontPixelSize(int size)
{
    if(size < 1)
    {
        return;
    }

    QFont font = zv_minimumLabel->font();
    font.setPixelSize(size);
    zv_minimumLabel->setFont(font);
    zv_maximumLabel->setFont(font);

    zh_rebuildLayout();
}
//============================================================
void ZLabeledSliderWidget::zp_setTickPosition(QSlider::TickPosition tickPosition)
{
    if(zv_slider->tickPosition() == tickPosition)
    {
        return;
    }

    zv_slider->setTickPosition(tickPosition);
    zh_rebuildLayout();
}
//============================================================
void ZLabeledSliderWidget::zp_setOrientation(Qt::Orientation orientation)
{
    if(zv_slider->orientation() == orientation)
    {
        return;
    }

    zv_slider->setOrientation(orientation);
    zh_rebuildLayout();
}
//============================================================
void ZLabeledSliderWidget::zp_setMinimum(int value)
{
    zv_slider->setMinimum(value);
    zh_rebuildLayout();
}
//============================================================
void ZLabeledSliderWidget::zp_setMaximum(int value)
{
    zv_slider->setMaximum(value);
    zh_rebuildLayout();
}
//============================================================
void ZLabeledSliderWidget::zh_createComponents()
{
    zv_slider = new QSlider;
    zv_minimumLabel = new QLabel;
    zv_maximumLabel = new QLabel;
}
//============================================================
void ZLabeledSliderWidget::zh_createConnctions()
{
    connect(zv_slider, &QSlider::valueChanged,
            this, &ZLabeledSliderWidget::zg_valueChanged);
}
//============================================================
void ZLabeledSliderWidget::zh_rebuildLayout()
{
    QLayout* newLayout;
    if(zv_slider->orientation() == Qt::Horizontal)
    {
        QVBoxLayout* VLayout = new QVBoxLayout;
        VLayout->setMargin(0);
        //setLayout(newLayout);

        if((zv_maximumLabel->text().isEmpty() && zv_minimumLabel->text().isEmpty()) ||
                zv_slider->tickPosition() == QSlider::NoTicks)
        {
            zv_minimumLabel->setParent(0);
            zv_maximumLabel->setParent(0);
            VLayout->addWidget(zv_slider);
        }
        else
        {
            // create label layout
            QHBoxLayout* labelLayout = new QHBoxLayout;
            labelLayout->setMargin(0);
            labelLayout->addWidget(zv_minimumLabel);
            labelLayout->addStretch();
            labelLayout->addWidget(zv_maximumLabel);

            // create slider layout
            QHBoxLayout* sliderLayout = new QHBoxLayout;
            sliderLayout->setMargin(0);
            sliderLayout->addWidget(zv_slider);

            if(zv_slider->tickPosition() == QSlider::TicksAbove)
            {
                VLayout->addLayout(labelLayout);
                VLayout->addLayout(sliderLayout);
            }
            else
            {
                VLayout->addLayout(sliderLayout);
                VLayout->addLayout(labelLayout);
            }
        }

        newLayout = VLayout;
    }
    else // Qt::Verticals
    {
        QHBoxLayout* HLayout = new QHBoxLayout;
        newLayout = HLayout;
    }

    // store old layout
    QLayout* oldLayout = this->layout();

    if(oldLayout != 0)
    {
        delete oldLayout;
    }

    setLayout(newLayout);

}
//============================================================
