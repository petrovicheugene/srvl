//======================================================
#include "ZWindowGraphicsItem.h"
#include "ZCalibrationWindow.h"
#include "ZPlotterDefaulVariables.h"

#include <QGraphicsScene>
#include <QPainter>
#include <QStyleOptionGraphicsItem>
//======================================================
QColor ZWindowGraphicsItem::zv_currentWindowColor = QColor(Qt::red);
qreal ZWindowGraphicsItem::zv_topMargin = -100;
qreal ZWindowGraphicsItem::zv_bottomMargin = 0;
qint64 ZWindowGraphicsItem::zv_currentWindowId = -1;
quint8 ZWindowGraphicsItem::zv_alphaCannelValue = 30;

//======================================================
ZWindowGraphicsItem::ZWindowGraphicsItem(const ZCalibrationWindow* window,
                                         QColor color,
                                         QGraphicsItem * parent)
    : QGraphicsItem(parent)
{
    zv_windowId = window->zp_windowId();
    zv_color = color;
    zv_leftMargin = (qreal)window->zp_firstChannel();
    zv_rightMargin = (qreal)(window->zp_lastChannel() + 1);

    setZValue(gl_defaultWindowZValue);
    zv_lineColor = zv_color;
    zv_brushColor = zv_color;
    zv_brushColor.setAlpha(zv_alphaCannelValue);
    zh_recalcShapeAndBoundingRect();
    zp_updateCurrentWindow(window->zp_isWindowVisible());
}
//======================================================
QRectF ZWindowGraphicsItem::boundingRect() const
{
    return zv_boundingRect;
}
//======================================================
void ZWindowGraphicsItem::paint(QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * widget)
{
    painter->save();
    // window body
    painter->setBrush(QBrush(zv_brushColor));
    painter->setPen(Qt::NoPen);
    painter->drawPath(zv_paintRect);

    // margins
    painter->setBrush(Qt::NoBrush);
    QPen pen(zv_lineColor, 1);
    pen.setCosmetic(true);
    painter->setPen(pen);
    painter->drawPath(zv_marginsPainterPath);

    painter->restore();
}
//======================================================
QPainterPath ZWindowGraphicsItem::shape() const
{
    return zv_shape;
}
//======================================================
int ZWindowGraphicsItem::type() const
{
    return WindowItemType;
}
//======================================================
void ZWindowGraphicsItem::zp_setCurrentWindowColor(QColor color)
{
    zv_currentWindowColor = color;
}
//======================================================
void ZWindowGraphicsItem::zp_setCurrentWindowId(qint64 id)
{
    zv_currentWindowId = id;
}
//======================================================
bool ZWindowGraphicsItem::zp_setTopAndButtonMargins(qreal top, qreal bottom)
{
    if(top == bottom)
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
bool ZWindowGraphicsItem::zp_setAlphaChannelValue(quint8 alpha)
{
    zv_alphaCannelValue = alpha;
    return true;
}
//======================================================
qint64 ZWindowGraphicsItem::zp_windowId() const
{
    return zv_windowId;
}
//======================================================
void ZWindowGraphicsItem::zp_setLeftRightMargins(qreal first, qreal last)
{
    if(first > last)
    {
        qSwap(last, last);
    }

    zv_leftMargin = first;
    zv_rightMargin = last + 1;
    zh_recalcShapeAndBoundingRect();
    update();
}
//======================================================
void ZWindowGraphicsItem::zp_updateCurrentWindow(bool visible)
{
    if(zv_currentWindowId == zv_windowId)
    {
        setZValue(gl_currentWindowZValue);
        zv_lineColor = zv_color;
        zv_brushColor = zv_currentWindowColor;
        zv_brushColor.setAlpha(zv_alphaCannelValue);
        setVisible(true);
    }
    else
    {
        //      if(zValue() != gl_defaultWindowZValue)
        //      {
        setZValue(gl_defaultWindowZValue);
        zv_lineColor = zv_color;
        zv_brushColor = zv_color;
        zv_brushColor.setAlpha(zv_alphaCannelValue);
        setVisible(visible);
        //      }
    }
}
//======================================================
void ZWindowGraphicsItem::zp_updateItem()
{
    zh_recalcShapeAndBoundingRect();
    update();
}
//======================================================
void ZWindowGraphicsItem::zp_setVisible(bool visible)
{
    if(zv_windowId == zv_currentWindowId && visible == false)
    {
        return;
    }
    setVisible(visible);
}
//======================================================
void ZWindowGraphicsItem::mouseReleaseEvent(QGraphicsSceneMouseEvent * event)
{
    QGraphicsItem::mouseReleaseEvent(event);
}
//======================================================
//void ZWindowGraphicsItem::zh_recalcShape()
//{
//   QPainterPath path;
//   //    QPointF topPoint(zv_data.first, zv_topY);
//   //    QPointF bottomPoint(zv_data.first, zv_bottomY);
//   //    path.moveTo(topPoint);
//   //    path.lineTo(bottomPoint);
//   //    topPoint.setX(zv_data.second);
//   //    bottomPoint.setX(zv_data.second);
//   //    path.moveTo(topPoint);
//   //    path.lineTo(bottomPoint);

//   path.addRect(zv_boundingRect);
//   zv_shape = path;
//}
//======================================================
void ZWindowGraphicsItem::zh_recalcShapeAndBoundingRect()
{
    prepareGeometryChange();
    // shape
    QPainterPath newShape;
    QPointF topLeftPoint(zv_leftMargin, zv_topMargin);
    QPointF bottomRightPoint(zv_rightMargin, zv_bottomMargin);
    QRectF windowRect(topLeftPoint, bottomRightPoint);

    newShape.addRect(windowRect);
    zv_shape = newShape;
    zv_boundingRect = windowRect;

    // paint rect shape
    QPainterPath newRectPaintShape;
    qreal shift = (qAbs(zv_bottomMargin - zv_topMargin)) / 2;
    topLeftPoint = QPointF(zv_leftMargin, zv_topMargin - shift);
    bottomRightPoint = QPointF(zv_rightMargin, zv_bottomMargin + shift);
    QRectF paintRect(topLeftPoint, bottomRightPoint);
    newRectPaintShape.addRect(paintRect);
    zv_paintRect = newRectPaintShape;

    // line shape
    QPainterPath newLinePaintShape;
    QPointF bottomLeftPoint = QPointF(zv_leftMargin, zv_bottomMargin + shift);
    QPointF topRightPoint = QPointF(zv_rightMargin, zv_topMargin - shift);

    newLinePaintShape.moveTo(topLeftPoint);
    newLinePaintShape.lineTo(bottomLeftPoint);
    newLinePaintShape.moveTo(topRightPoint);
    newLinePaintShape.lineTo(bottomRightPoint);

    zv_marginsPainterPath = newLinePaintShape;
}
//======================================================
