//===========================================================
#include "ZStartStopButtonWidget.h"
#include <QVBoxLayout>
#include <QHBoxLayout>

#include <QPushButton>
#include <QHBoxLayout>
//===========================================================
ZStartStopButtonWidget::ZStartStopButtonWidget(Qt::Orientation orientation, QWidget *parent)
    : QWidget(parent)
{
   zh_createComponents();
   zp_setOrientation(orientation);
}
//===========================================================
//void ZStartStopButton::zp_setSizeHint(QSize size)
//{
//    zv_sizeHint = size;
//}
////===========================================================
//void ZStartStopButton::paintEvent(QPaintEvent* event)
//{
//    QPainter painter(this);
//    QPen pen = QPen(QBrush(QColor(Qt::black)), 1);
//    painter.setPen(pen);
//    painter.setBrush(QBrush(QColor(Qt::darkYellow)));

//    painter.drawEllipse(event->rect().adjusted(0,0, -1, -1));
//}
////===========================================================
//QSize ZStartStopButton::sizeHint() const
//{
//    return zv_sizeHint;
//}
////===========================================================
//void ZStartStopButton::mouseReleaseEvent(QMouseEvent *event)
//{

//}
////===========================================================
//void ZStartStopButton::mousePressEvent(QMouseEvent *event)
//{

//}
//===========================================================
void ZStartStopButtonWidget::zh_createComponents()
{
    QLayout* mainLayout = new QHBoxLayout(this);
    setLayout(mainLayout);

    zv_startButton = new QPushButton(this);
    zv_startButton->setMinimumHeight(30);
    zv_startButton->setText(tr("Start"));
    mainLayout->addWidget(zv_startButton);

    zv_stopButton = new QPushButton(this);
    zv_stopButton->setMinimumHeight(30);
    zv_stopButton->setText(tr("Stop"));
    mainLayout->addWidget(zv_stopButton);

}
//===========================================================
void ZStartStopButtonWidget::zp_setOrientation(Qt::Orientation orientation)
{
    QLayout* oldLayout = layout();
    QLayout* mainLayout;
    if(orientation == Qt::Vertical)
    {
        mainLayout = new QVBoxLayout(this);
    }
    else
    {
        mainLayout = new QHBoxLayout(this);
    }

    mainLayout->addWidget(zv_startButton);
    mainLayout->addWidget(zv_stopButton);

    // delete old layout
    if(oldLayout != 0)
    {
        delete oldLayout;
    }

    // replace layout
    setLayout(mainLayout);
}
//===========================================================
