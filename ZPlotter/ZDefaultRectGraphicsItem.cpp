//========================================================
#include "ZDefaultRectGraphicsItem.h"
#include "ZGraphicsItemUserTypes.h"
#include <QPainter>
//========================================================
ZDefaultRectGraphicsItem::ZDefaultRectGraphicsItem(const QRectF& rect,
                                                   bool XAxesVisible,
                                                   bool YAxesVisible, bool medianaLineVisible,
                                                   QGraphicsItem *parent) :
    QGraphicsItem(parent)
{
    zv_axisColor = QColor(Qt::black);
    zv_medianaLineColor = QColor(Qt::green);

    zv_XAxesVisible = XAxesVisible;
    zv_YAxesVisible = YAxesVisible;
    zv_medianaLineVisible = medianaLineVisible;
    QRectF itemRect;
    if(!rect.normalized().isValid() || rect.normalized().isEmpty())
    {
        itemRect = QRectF(0.0, -100.0, 100.0, 100.0);
    }
    else
    {
        itemRect = rect.normalized();
    }


    zp_fitItemInRect(itemRect);
}
//========================================================
QRectF ZDefaultRectGraphicsItem::boundingRect() const
{
    return zv_boundingRect;
}
//========================================================
void ZDefaultRectGraphicsItem::paint(QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * widget)
{
    painter->save();
    // painter->setClipRect(zv_boundingRect);
    QPen pen;
    pen.setCosmetic(true);

    pen.setBrush(zv_medianaLineColor);
    painter->setPen(pen);
    painter->setRenderHint(QPainter::Antialiasing, true);
    painter->drawPath(zv_medianaLinePath);

    pen.setBrush(zv_axisColor);
    painter->setPen(pen);
    painter->setRenderHint(QPainter::Antialiasing, false);
    painter->drawPath(zv_axisPath);

    painter->restore();
}
//========================================================
QPainterPath ZDefaultRectGraphicsItem::shape() const
{
    return zv_shape;
}
//========================================================
int ZDefaultRectGraphicsItem::type() const
{
    return DefaultRectItemType;
}
//========================================================
void ZDefaultRectGraphicsItem::zp_fitItemInRect(const QRectF& rect)
{
    if(!rect.normalized().isValid() || rect.normalized().isEmpty())
    {
        return;
    }

    prepareGeometryChange();
    zv_boundingRect = rect.normalized().adjusted(-1,-1,1,1);
    QPainterPath newShape;
    newShape.addRect(zv_boundingRect);
    zv_shape = newShape;

    zh_remakePainterPaths();

    //    QPainterPath newAxisPath;
    //    if(zv_XAxesVisible)
    //    {
    //        newAxisPath.moveTo(zv_boundingRect.left(), 0.0);
    //        newAxisPath.lineTo(zv_boundingRect.right(), 0.0);
    //    }
    //    if(zv_YAxesVisible)
    //    {
    //        newAxisPath.moveTo(0.0, zv_boundingRect.top());
    //        newAxisPath.lineTo(0.0, zv_boundingRect.bottom());
    //    }
    //    zv_axisPath = newAxisPath;

    //    QPainterPath newMedianaLinePath;
    //    if(zv_medianaLineVisible)
    //    {
    //        newMedianaLinePath.moveTo( -1*(zv_boundingRect.bottom() + 0.5), (zv_boundingRect.bottom() + 0.5));
    //        newMedianaLinePath.lineTo( -1*(zv_boundingRect.top() + 0.5), (zv_boundingRect.top() + 0.5));
    //    }
    //    zv_medianaLinePath = newMedianaLinePath;

    update();
}
//========================================================
void ZDefaultRectGraphicsItem::zh_remakePainterPaths()
{
    QPainterPath newAxisPath;
    if(zv_XAxesVisible)
    {
        newAxisPath.moveTo(zv_boundingRect.left(), 0.0);
        newAxisPath.lineTo(zv_boundingRect.right(), 0.0);
    }
    if(zv_YAxesVisible)
    {
        newAxisPath.moveTo(0.0, zv_boundingRect.top());
        newAxisPath.lineTo(0.0, zv_boundingRect.bottom());
    }
    zv_axisPath = newAxisPath;

    QPainterPath newMedianaLinePath;
    if(zv_medianaLineVisible)
    {
        newMedianaLinePath.moveTo( -1*(zv_boundingRect.bottom() + 0.5), (zv_boundingRect.bottom() + 0.5));
        newMedianaLinePath.lineTo( -1*(zv_boundingRect.top() + 0.5), (zv_boundingRect.top() + 0.5));
    }
    zv_medianaLinePath = newMedianaLinePath;
}
//========================================================
void ZDefaultRectGraphicsItem::zp_setAxisVisible(Qt::Orientation orientation, bool visibilityFlag )
{
    if(orientation == Qt::Vertical)
    {
        zv_YAxesVisible = visibilityFlag;
    }
    else
    {
        zv_XAxesVisible = visibilityFlag;
    }

    zh_remakePainterPaths();
    update();
}
//========================================================
void ZDefaultRectGraphicsItem::zp_setAxisColor(QColor color)
{
    if(zv_axisColor == color)
    {
        return;
    }
    zv_axisColor = color;
    update();
}
//========================================================
void ZDefaultRectGraphicsItem::zp_setMedianaLineColor(QColor color)
{
    if(zv_medianaLineColor == color)
    {
        return;
    }
    zv_medianaLineColor = color;
    update();
}
//========================================================
void ZDefaultRectGraphicsItem::zp_setMedianaLineVisible(bool visibilityFlag)
{
    if(zv_medianaLineVisible == visibilityFlag)
    {
        return;
    }
    zv_medianaLineVisible = visibilityFlag;
    zh_remakePainterPaths();
    update();
}
//========================================================
