//===========================================================
#include "ZStartStopButton.h"

#include <QPaintEvent>
#include <QPainter>
//===========================================================
ZStartStopButton::ZStartStopButton(QWidget *parent) : QWidget(parent)
{
    zv_sizeHint = QSize(32, 32);
}
//===========================================================
void ZStartStopButton::zp_setSizeHint(QSize size)
{
    zv_sizeHint = size;
}
//===========================================================
void ZStartStopButton::paintEvent(QPaintEvent* event)
{
    QPainter painter(this);
    QPen pen = QPen(QBrush(QColor(Qt::black)), 1);
    painter.setPen(pen);
    painter.setBrush(QBrush(QColor(Qt::darkYellow)));

    painter.drawEllipse(event->rect().adjusted(0,0, -1, -1));
}
//===========================================================
QSize ZStartStopButton::sizeHint() const
{
    return zv_sizeHint;
}
//===========================================================
void ZStartStopButton::mouseReleaseEvent(QMouseEvent *event)
{

}
//===========================================================
void ZStartStopButton::mousePressEvent(QMouseEvent *event)
{

}
//===========================================================
