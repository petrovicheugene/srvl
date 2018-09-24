#ifndef ZTEXTGRAPHICSITEM_H
#define ZTEXTGRAPHICSITEM_H
//======================================================
#include <QGraphicsItem>

#include <QColor>
#include <QFont>

#include "ZGraphicsItemUserTypes.h"
//======================================================
class ZTextGraphicsItem: public QGraphicsItem
{
public:
    ZTextGraphicsItem(const QString &text, QGraphicsItem * parent = 0);

    // overrided
    QRectF boundingRect() const override;
    void paint(QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * widget = 0) override;
    QPainterPath shape() const override;
    int type() const override;

    void zp_updateItem();
    void zp_setText(const QString& text);
    void zp_setFont(QFont font);
    void zp_setTextPixelSize(int pixelSize);
    void zp_setColor(QColor color);

protected:

    void zh_recalcShapeAndBoundingRect();

private:

    // VARS
    QRectF zv_boundingRect;
    QPainterPath zv_shape;
    QPainterPath zv_textPainterPath;

    QString zv_text;
    QFont zv_font;
    int zv_textPixelSize;
    QColor zv_color;
    // FUNCS

};
//======================================================
#endif // ZTEXTGRAPHICSITEM_H
