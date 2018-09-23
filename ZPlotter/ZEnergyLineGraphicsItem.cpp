//======================================================
#include "ZEnergyLineGraphicsItem.h"
#include "ZEnergyCalibrationLine.h"

#include <QPainter>
//======================================================
double ZEnergyLineGraphicsItem::zv_topMargin = -100.0;
double ZEnergyLineGraphicsItem::zv_bottomMargin = 0.0;
//======================================================
ZEnergyLineGraphicsItem::ZEnergyLineGraphicsItem(const QString &chemicalElementSymbol,
                                                 const QString& name,
                                                 int heightPercent,
                                                 QGraphicsItem *parent)
{
    zv_chemicalElementSymbol = chemicalElementSymbol;
    zv_lineName = name;
    zv_heightPercent = heightPercent;
    setToolTip(QString("%1 %2").arg(zv_chemicalElementSymbol, zv_lineName));
    xPosition = 100.5;
    zp_updateItem();
}
//======================================================
QRectF ZEnergyLineGraphicsItem::boundingRect() const
{
    return zv_boundingRect;
}
//======================================================
void ZEnergyLineGraphicsItem::paint(QPainter * painter,
                                    const QStyleOptionGraphicsItem * option,
                                    QWidget * widget)
{
    painter->save();
    painter->setBrush(Qt::NoBrush);

    //QPen pen(zv_energyCalibrationLine->zp_lineColor(), 1);
    QPen pen(zv_color, 1);
    pen.setCosmetic(true);
    painter->setPen(pen);
    painter->drawPath(zv_linePainterPath);


    painter->restore();
}
//======================================================
QPainterPath ZEnergyLineGraphicsItem::shape() const
{
    return zv_shape;
}
//======================================================
int ZEnergyLineGraphicsItem::type() const
{
    return EnergyLineItemType;
}
//======================================================
void ZEnergyLineGraphicsItem::zp_updateItem()
{
    zh_recalcShapeAndBoundingRect();
    update();
}
//======================================================
void ZEnergyLineGraphicsItem::zh_recalcShapeAndBoundingRect()
{
    prepareGeometryChange();

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
    QPointF topPoint = QPointF(xPosition, zv_topMargin + shift);

    newLinePaintShape.moveTo(topPoint);
    newLinePaintShape.lineTo(bottomPoint);

    zv_linePainterPath = newLinePaintShape;

}
//======================================================
bool ZEnergyLineGraphicsItem::zp_setTopAndButtonMargins(qreal top, qreal bottom)
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
void ZEnergyLineGraphicsItem::zp_setXPosition(double xPos)
{
    xPosition = xPos;
    zh_recalcShapeAndBoundingRect();
    update();
}
//======================================================
void ZEnergyLineGraphicsItem::zp_setColor(QColor color)
{
    zv_color = color;
    zh_recalcShapeAndBoundingRect();
    update();
}
//======================================================
QString ZEnergyLineGraphicsItem::zp_chemicalElementSymbol() const
{
    return zv_chemicalElementSymbol;
}
//======================================================
QString ZEnergyLineGraphicsItem::zp_lineName() const
{
    return zv_lineName;
}
//======================================================
