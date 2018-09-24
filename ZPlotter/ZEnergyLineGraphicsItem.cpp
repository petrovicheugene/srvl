//======================================================
#include "ZEnergyLineGraphicsItem.h"
#include "ZEnergyCalibrationLine.h"
#include <QDebug>
#include "ZTextGraphicsItem.h"
#include <QPainter>
#include <QFontMetrics>
//======================================================
double ZEnergyLineGraphicsItem::zv_topMargin = -100.0;
double ZEnergyLineGraphicsItem::zv_bottomMargin = 0.0;
int ZEnergyLineGraphicsItem::zv_fontPixelSize = 10;
//======================================================
ZEnergyLineGraphicsItem::ZEnergyLineGraphicsItem(const QString &chemicalElementSymbol,
                                                 const QString& name,
                                                 int heightPercent,
                                                 double energyValue,
                                                 QGraphicsItem *parent)
{
    zv_chemicalElementSymbol = chemicalElementSymbol;
    zv_lineName = name;
    zv_heightPercent = heightPercent;
    zv_energyValue = energyValue;
    zv_textItem = new ZTextGraphicsItem(zv_chemicalElementSymbol, this);

    //zv_textItem->setVisible(true);
    setZValue(999);
    zv_textItem->setToolTip(QString("%1 %2").arg(zv_chemicalElementSymbol, zv_lineName));
    zv_textItem->setFlag(QGraphicsItem::ItemIgnoresTransformations);
    setToolTip(zv_textItem->toolTip());
    zv_xPosition = 0.0;
    zp_updateItem();
}
//======================================================
ZEnergyLineGraphicsItem::~ZEnergyLineGraphicsItem()
{
    delete zv_textItem;
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
    if(zv_xPosition <= 0.0)
    {
        zv_textItem->setVisible(false);
        return;
    }

    painter->save();

    // line
    QPen pen(zv_color, 1);
    pen.setCosmetic(true);
    painter->setPen(pen);
    painter->setBrush(Qt::NoBrush);
    painter->drawPath(zv_linePainterPath);

    painter->restore();

    zv_textItem->setVisible(true);
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

    // line
    qreal shift = (qAbs(zv_bottomMargin - zv_topMargin)) * (1 - ((0.8/100) * zv_heightPercent));
    QPainterPath newLinePainterPath;
    QPointF bottomPoint = QPointF(zv_xPosition, 0.0);
    QPointF topPoint = QPointF(zv_xPosition, zv_topMargin + shift);
    newLinePainterPath.moveTo(topPoint);
    newLinePainterPath.lineTo(bottomPoint);

    zv_linePainterPath = newLinePainterPath;

    zv_shape = zv_linePainterPath;
    zv_boundingRect = zv_shape.boundingRect();

    // text
    QPointF textPos(topPoint);
    zv_textItem->setPos(textPos);
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
void ZEnergyLineGraphicsItem::zp_setFontPixelSize(int pixelSize)
{
    zv_fontPixelSize = pixelSize;
}
//======================================================
void ZEnergyLineGraphicsItem::zp_setHeightPercent(int percent)
{
    zv_heightPercent = percent;
    zh_recalcShapeAndBoundingRect();
    update();
}
//======================================================
void ZEnergyLineGraphicsItem::zp_setXPosition(double xPos)
{
    zv_xPosition = xPos;
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
double ZEnergyLineGraphicsItem::zp_energyValue() const
{
    return zv_energyValue;
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
