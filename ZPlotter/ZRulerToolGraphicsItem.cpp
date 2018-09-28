//======================================================
#include "ZRulerToolGraphicsItem.h"
#include "ZTextGraphicsItem.h"

#include <QBrush>
#include <QColor>
#include <QGraphicsTextItem>
#include <QPainter>
#include <QPen>

//======================================================
ZRulerToolGraphicsItem::ZRulerToolGraphicsItem(QGraphicsItem *parent)
{
    zv_textItem = new QGraphicsTextItem(this);

    zv_textItem->setVisible(true);
    setZValue(9999);
    zv_textItem->setFlag(QGraphicsItem::ItemIgnoresTransformations);
    zp_updateItem();
}
//======================================================
ZRulerToolGraphicsItem::~ZRulerToolGraphicsItem()
{
    delete zv_textItem;
}
//======================================================
QRectF ZRulerToolGraphicsItem::boundingRect() const
{
    return zv_boundingRect;
}
//======================================================
void ZRulerToolGraphicsItem::paint(QPainter * painter,
                                   const QStyleOptionGraphicsItem * option,
                                   QWidget * widget)
{
    painter->save();
    QBrush brush(QColor(255, 0, 255, 20));
    QPen pen(Qt::transparent);
    pen.setCosmetic(true);
    painter->setPen(pen);
    painter->setBrush(brush);
    painter->drawRect(zv_boundingRect);

    pen = QPen(QColor(Qt::red), 1);
    pen.setCosmetic(true);
    painter->setPen(pen);
    painter->drawPath(zv_linePainterPath);

    painter->restore();
}
//======================================================
QPainterPath ZRulerToolGraphicsItem::shape() const
{
    return zv_shape;
}
//======================================================
int ZRulerToolGraphicsItem::type() const
{
    return RulerToolItemType;
}
//======================================================
void ZRulerToolGraphicsItem::zp_setProperties(QPointF startPosition,
                                              QPointF endPosition,
                                              QRectF viewPortSceneRect,
                                              QSize viewportSize,
                                              const QString& text)
{
    zv_startPosition = startPosition;
    zv_endPosition = endPosition;
    zv_viewportSceneRect = viewPortSceneRect;
    zv_viewportSize = viewportSize;
    zv_text = text;

    zp_updateItem();
}
//======================================================
void ZRulerToolGraphicsItem::zp_updateItem()
{
    zh_recalcShapeAndBoundingRect();
    update();
}
//======================================================
void ZRulerToolGraphicsItem::zh_recalcShapeAndBoundingRect()
{
    prepareGeometryChange();

    QPainterPath linePainterPath;

    double offset = zh_convertPixelsToSceneHeight(5);
    linePainterPath.moveTo(zv_startPosition.x(), zv_viewportSceneRect.top() + offset);
    linePainterPath.lineTo(zv_startPosition.x(), 0.0);

    linePainterPath.moveTo(zv_endPosition.x(), zv_viewportSceneRect.top() + offset);
    linePainterPath.lineTo(zv_endPosition.x(), 0.0);

    zv_linePainterPath = linePainterPath;
    zv_shape = zv_linePainterPath;

    double left = qMin(zv_startPosition.x(), zv_endPosition.x());

    zv_boundingRect = QRectF(left,
                             zv_viewportSceneRect.top() + offset,
                             qAbs(zv_startPosition.x() - zv_endPosition.x()),
                             -(zv_viewportSceneRect.top() + offset));


    zv_textItem->setHtml(zv_text);
    QRectF textRect = zv_textItem->boundingRect();
    QPointF textPos = zv_endPosition;

    textPos.setY(textPos.y() - zh_convertPixelsToSceneHeight(qRound(textRect.height())));

    if(zv_endPosition.x() + zh_convertPixelsToSceneWidth(qRound(textRect.width())) >  zv_viewportSceneRect.right())
    {
        textPos.setX(zv_viewportSceneRect.right() - zh_convertPixelsToSceneWidth(qRound(textRect.width())));
    }

    zv_textItem->setPos(textPos);
}
//======================================================
double ZRulerToolGraphicsItem::zh_convertPixelsToSceneWidth(int pixelCount) const
{
    return (zv_viewportSceneRect.width() / zv_viewportSize.width()) * pixelCount;
}
//======================================================
double ZRulerToolGraphicsItem::zh_convertPixelsToSceneHeight(int pixelCount) const
{
    return (zv_viewportSceneRect.height() / zv_viewportSize.height()) * pixelCount;
}
//======================================================

