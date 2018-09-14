//======================================================
#ifndef ZENERGYLINEGRAPHICSITEM_H
#define ZENERGYLINEGRAPHICSITEM_H
//======================================================
#include <QGraphicsItem>
#include "ZGraphicsItemUserTypes.h"
#include <QColor>
//======================================================
class ZEnergyCalibrationLine;
//======================================================
class ZEnergyLineGraphicsItem : public QGraphicsItem
{
public:
    explicit ZEnergyLineGraphicsItem(ZEnergyCalibrationLine* energyCalibrationLine,
                                     QGraphicsItem * parent = 0);
    // overrided
    QRectF boundingRect() const override;
    void paint(QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * widget = 0) override;
    QPainterPath shape() const override;
    int type() const override;

    void zp_updateItem();

protected:

    void zh_recalcShapeAndBoundingRect();

private:

    //VARS
    double zv_topMargin;
    double zv_bottomMargin;


    ZEnergyCalibrationLine* zv_energyCalibrationLine;

    QRectF zv_boundingRect;
    QPainterPath zv_shape;
    QPainterPath zv_linePainterPath;

};
//======================================================
#endif // ZENERGYLINEGRAPHICSITEM_H
