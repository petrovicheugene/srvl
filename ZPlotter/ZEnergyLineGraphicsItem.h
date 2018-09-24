//======================================================
#ifndef ZENERGYLINEGRAPHICSITEM_H
#define ZENERGYLINEGRAPHICSITEM_H
//======================================================
#include <QGraphicsItem>
#include "ZGraphicsItemUserTypes.h"
#include <QColor>
//======================================================
class ZTextGraphicsItem;
//======================================================
class ZEnergyLineGraphicsItem : public QGraphicsItem
{
public:
    explicit ZEnergyLineGraphicsItem(const QString& chemicalElementSymbol,
                                     const QString &name,
                                     int heightPercent,
                                     double energyValue,
                                     QGraphicsItem * parent = 0);
    ~ZEnergyLineGraphicsItem();
    // overrided
    QRectF boundingRect() const override;
    void paint(QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * widget = 0) override;
    QPainterPath shape() const override;
    int type() const override;

    void zp_updateItem();
    static bool zp_setTopAndButtonMargins(double top, double bottom);
    static void zp_setFontPixelSize(int pixelSize);

    void zp_setHeightPercent(int percent);
    void zp_setXPosition(double xPos);
    QString zp_chemicalElementSymbol() const;
    QString zp_lineName() const;
    void zp_setColor(QColor color);

    double zp_energyValue() const;

protected:

    void zh_recalcShapeAndBoundingRect();

private:

    //VARS
    static double zv_topMargin;
    static double zv_bottomMargin;
    static int zv_fontPixelSize;
    ZTextGraphicsItem* zv_textItem;

    double zv_xPosition;
    int zv_heightPercent;
    QString zv_chemicalElementSymbol;
    QString zv_lineName;
    QColor zv_color;
    double zv_energyValue;

    QRectF zv_boundingRect;
    QPainterPath zv_shape;
    QPainterPath zv_linePainterPath;

};
//======================================================
#endif // ZENERGYLINEGRAPHICSITEM_H
