//===========================================================
#include "ZColorLabel.h"

#include <QPaintEvent>
#include <QPainter>
#include <QColorDialog>

//===========================================================
ZColorLabel::ZColorLabel(QWidget *parent) : QWidget(parent)
{
    zp_setSizeHint(16);
    zv_color = QColor(Qt::transparent);
    setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
}
//===========================================================
void ZColorLabel::zp_setSizeHint(int size)
{
    if(size > 3)
    {
        zv_sizeHint = QSize(size, size);
    }
}
//===========================================================
void ZColorLabel::zp_setColor(QColor color)
{
    zv_color = color;
    update();
    emit zg_colorChanged();
}
//===========================================================
QColor ZColorLabel::zp_color() const
{
    return zv_color;
}
//===========================================================
void ZColorLabel::paintEvent(QPaintEvent* event)
{
    QPainter painter(this);
    QPen pen = QPen(QBrush(QColor(Qt::black)), 1);
    painter.setPen(pen);

    if(isEnabled())
    {
        painter.setBrush(QBrush(zv_color));
    }
    else
    {
        int gray = qGray(zv_color.rgb());
        painter.setBrush(QBrush(QColor(gray, gray, gray, 255)));
    }

    painter.drawRect(event->rect().adjusted(0,0, -1, -1));
}
//===========================================================
QSize ZColorLabel::sizeHint() const
{
    return zv_sizeHint;
}
//===========================================================
void ZColorLabel::mouseReleaseEvent(QMouseEvent *)
{
    zp_runColorSelectionDialog();
}
//===========================================================
void ZColorLabel::zp_runColorSelectionDialog()
{
    QColor color = QColorDialog::getColor(zv_color, this, tr("Select color"));
    if(!color.isValid())
    {
        return;
    }

    zp_setColor(color);
}
//===========================================================
