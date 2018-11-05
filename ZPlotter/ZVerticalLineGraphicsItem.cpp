//======================================================
#include "ZVerticalLineGraphicsItem.h"
#include "ZEnergyCalibrationLine.h"

#include <QPainter>
//======================================================
double ZVerticalLineGraphicsItem::zv_topMargin = -100.0;
double ZVerticalLineGraphicsItem::zv_bottomMargin = 0.0;
//======================================================
ZVerticalLineGraphicsItem::ZVerticalLineGraphicsItem(ZEnergyCalibrationLine* energyCalibrationLine,
                                                     QGraphicsItem *parent)
 : QGraphicsItem(parent)
{
    zv_energyCalibrationLine = energyCalibrationLine;
    zp_updateItem();
}
//======================================================
QRectF ZVerticalLineGraphicsItem::boundingRect() const
{
    return zv_boundingRect;
}
//======================================================
void ZVerticalLineGraphicsItem::paint(QPainter * painter,
           const QStyleOptionGraphicsItem * option,
           QWidget * widget)
{
    painter->save();
    painter->setBrush(Qt::NoBrush);
    QPen pen(zv_energyCalibrationLine->zp_lineColor(), 1);
    pen.setCosmetic(true);
    painter->setPen(pen);
    painter->drawPath(zv_linePainterPath);
    painter->restore();

}
//======================================================
QPainterPath ZVerticalLineGraphicsItem::shape() const
{
    return zv_shape;
}
//======================================================
int ZVerticalLineGraphicsItem::type() const
{
    return VerticalLineItemType;
}
//======================================================
void ZVerticalLineGraphicsItem::zp_updateItem()
{
    setVisible(zv_energyCalibrationLine->zp_isEnabled());
    zh_recalcShapeAndBoundingRect();
    update();
}
//======================================================
bool ZVerticalLineGraphicsItem::zp_setTopAndButtonMargins(qreal top, qreal bottom)
{
    if(top - bottom == 0.0)
    {
        return false;
    }

    if(top > bottom)
    {
        qSwap(top, bottom);
    }

    zv_bottomMargin = bottom;
    zv_topMargin = top;
    return true;
}
//======================================================
void ZVerticalLineGraphicsItem::zh_recalcShapeAndBoundingRect()
{
    prepareGeometryChange();
    double xPosition = static_cast<double>(zv_energyCalibrationLine->zp_channelNumber()) + 0.5;
    // shape
    QPainterPath newShape;
    QPointF topLeftPoint(xPosition - 1, zv_topMargin);
    QPointF bottomRightPoint(xPosition + 1, zv_bottomMargin);
    QRectF windowRect(topLeftPoint, bottomRightPoint);

    newShape.addRect(windowRect);
    zv_shape = newShape;
    zv_boundingRect = windowRect;

    // line
    qreal shift = (qAbs(zv_bottomMargin - zv_topMargin)) / 2;

    QPainterPath newLinePaintShape;
    QPointF bottomPoint = QPointF(xPosition, zv_bottomMargin + shift);
    QPointF topPoint = QPointF(xPosition, zv_topMargin - shift);

    newLinePaintShape.moveTo(topPoint);
    newLinePaintShape.lineTo(bottomPoint);

    zv_linePainterPath = newLinePaintShape;
}
//======================================================

