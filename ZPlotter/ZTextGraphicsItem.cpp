//======================================================
#include "ZTextGraphicsItem.h"
#include "ZGraphicsItemUserTypes.h"

#include <QApplication>
#include <QDebug>
#include <QFontMetrics>
#include <QPainter>
#include <QPen>
#include <QStyleOptionGraphicsItem>
#include <QTextItem>
//======================================================
ZTextGraphicsItem::ZTextGraphicsItem(const QString& text, QGraphicsItem * parent)
    : QGraphicsItem(parent)
{
    zv_text = text;
    zv_font = qApp->font();
    zv_textPixelSize = 6;
    zh_recalcShapeAndBoundingRect();
}
//======================================================
QRectF ZTextGraphicsItem::boundingRect() const
{
    return zv_boundingRect;
}
//======================================================
void ZTextGraphicsItem::paint(QPainter * painter,
                              const QStyleOptionGraphicsItem * option,
                              QWidget * widget)
{
    painter->save();

    QRect bkgRect = option->rect;
    bkgRect.adjust(-1, +2, 0, 0);

    QColor bkgColor(255, 255, 255, 255);
    painter->setPen(QPen(bkgColor, 1));
    painter->setBrush(QBrush(bkgColor));
    painter->drawRect(bkgRect);

    painter->setPen(QPen(Qt::black, 1));
    painter->setFont(zv_font);
    painter->drawText(option->rect, Qt::AlignCenter, zv_text);

    painter->restore();
}
//======================================================
QPainterPath ZTextGraphicsItem::shape() const
{
    return zv_shape;
}
//======================================================
int ZTextGraphicsItem::type() const
{
    return TextItemType;
}
//======================================================
void ZTextGraphicsItem::zh_recalcShapeAndBoundingRect()
{
    QFontMetrics fm(zv_font);
    //zv_boundingRect = fm.boundingRect(zv_text);
    zv_boundingRect = QRect(0,0, fm.width(zv_text), fm.height());
    zv_boundingRect.moveCenter(zv_boundingRect.topLeft());

    QPainterPath newShapePainterPath;
    newShapePainterPath.addRect(zv_boundingRect);
    zv_shape = newShapePainterPath;

//    QPainterPath newTextPainterPath;
//    newTextPainterPath.addText(zv_boundingRect.topLeft(), zv_font, zv_text);
//    zv_textPainterPath = newTextPainterPath;

}
//======================================================
void ZTextGraphicsItem::zp_updateItem()
{
    zh_recalcShapeAndBoundingRect();
    update();
}
//======================================================
void ZTextGraphicsItem::zp_setText(const QString& text)
{
    zv_text = text;
    zp_updateItem();
}
//======================================================
void ZTextGraphicsItem::zp_setFont(QFont font)
{
    zv_font = font;
    zp_updateItem();
}
//======================================================
void ZTextGraphicsItem::zp_setTextPixelSize(int pixelSize)
{
    zv_textPixelSize = pixelSize;
    zp_updateItem();
}
//======================================================
void ZTextGraphicsItem::zp_setColor(QColor color)
{
    zv_color = color;
    zp_updateItem();
}
//======================================================
