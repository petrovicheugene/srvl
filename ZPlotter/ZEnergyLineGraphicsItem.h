//======================================================
#ifndef ZENERGYLINEGRAPHICSITEM_H
#define ZENERGYLINEGRAPHICSITEM_H
//======================================================
#include <QGraphicsItem>
#include "ZGraphicsItemUserTypes.h"
#include <QColor>
//======================================================

//======================================================
class ZEnergyLineGraphicsItem : public QGraphicsItem
{
public:
    explicit ZEnergyLineGraphicsItem(const QString& chemicalElementSymbol,
                                     const QString &name,
                                     int heightPercent,
                                     QGraphicsItem * parent = 0);
    // overrided
    QRectF boundingRect() const override;
    void paint(QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * widget = 0) override;
    QPainterPath shape() const override;
    int type() const override;

    void zp_updateItem();
    static bool zp_setTopAndButtonMargins(double top, double bottom);

    void zp_setXPosition(double xPos);
    QString zp_chemicalElementSymbol() const;
    QString zp_lineName() const;
    void zp_setColor(QColor color);


protected:

    void zh_recalcShapeAndBoundingRect();

private:

    //VARS
    static double zv_topMargin;
    static double zv_bottomMargin;

    double xPosition;
    int zv_heightPercent;
    QString zv_chemicalElementSymbol;
    QString zv_lineName;
    QColor zv_color;

    QRectF zv_boundingRect;
    QPainterPath zv_shape;
    QPainterPath zv_linePainterPath;

};
//======================================================
#endif // ZENERGYLINEGRAPHICSITEM_H
