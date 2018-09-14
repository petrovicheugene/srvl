//======================================================
#ifndef ZVERTICALLINEGRAPHICSITEM_H
#define ZVERTICALLINEGRAPHICSITEM_H
//======================================================
#include <QGraphicsItem>
#include "ZGraphicsItemUserTypes.h"
#include <QColor>
//======================================================
class ZEnergyCalibrationLine;
//======================================================
class ZVerticalLineGraphicsItem : public QGraphicsItem
{
public:
    explicit ZVerticalLineGraphicsItem(ZEnergyCalibrationLine* energyCalibrationLine,
                                       QGraphicsItem * parent = 0);
    // overrided
    QRectF boundingRect() const override;
    void paint(QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * widget = 0) override;
    QPainterPath shape() const override;
    int type() const override;

    void zp_updateItem();
    static bool zp_setTopAndButtonMargins(double top, double bottom);



protected:

    void zh_recalcShapeAndBoundingRect();

private:

    //VARS
    static double zv_topMargin;
    static double zv_bottomMargin;

    ZEnergyCalibrationLine* zv_energyCalibrationLine;

    QRectF zv_boundingRect;
    QPainterPath zv_shape;
    QPainterPath zv_linePainterPath;

};
//======================================================
#endif // ZVERTICALLINEGRAPHICSITEM_H
