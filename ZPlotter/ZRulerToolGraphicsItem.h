//======================================================
#ifndef ZROLERTOOLGRAPHICSITEM_H
#define ZROLERTOOLGRAPHICSITEM_H
//======================================================
#include <QGraphicsItem>
#include "ZGraphicsItemUserTypes.h"
//======================================================
class ZTextGraphicsItem;
//======================================================
class ZRulerToolGraphicsItem: public QGraphicsItem
{
public:
    explicit ZRulerToolGraphicsItem(QGraphicsItem * parent = 0);
    ~ZRulerToolGraphicsItem();
    // overrided
    QRectF boundingRect() const override;
    void paint(QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * widget = 0) override;
    QPainterPath shape() const override;
    int type() const override;

    void zp_setProperties(QPointF startPosition,
                          QPointF endPosition, QRectF viewPortSceneRect, QSize viewportSize, const QString &text);
    void zp_updateItem();

protected:

    void zh_recalcShapeAndBoundingRect();

private:

    // VARS
    QGraphicsTextItem* zv_textItem;

    QPointF zv_startPosition;
    QPointF zv_endPosition;
    QRectF zv_viewportSceneRect;
    QSize zv_viewportSize;
    QString zv_text;

    QRectF zv_boundingRect;
    QPainterPath zv_shape;

    QPainterPath zv_linePainterPath;

    // FUNCS
    double zh_convertPixelsToSceneWidth(int pixelCount) const;
    double zh_convertPixelsToSceneHeight(int pixelCount) const;


};
//======================================================
#endif // ZROLERTOOLGRAPHICSITEM_H
