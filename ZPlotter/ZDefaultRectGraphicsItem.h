//========================================================
#ifndef ZDEFAULTRECTGRAPHICSITEM_H
#define ZDEFAULTRECTGRAPHICSITEM_H
//========================================================
#include <QGraphicsItem>
#include <QGraphicsLineItem>
#include <QPainter>
//========================================================
class ZAntialiasingLine : public QGraphicsLineItem
{
public:
    explicit ZAntialiasingLine(QGraphicsItem * parent = 0) : QGraphicsLineItem(parent){}
    explicit ZAntialiasingLine(const QLineF & line, QGraphicsItem * parent = 0) : QGraphicsLineItem(line, parent){}
    explicit ZAntialiasingLine(qreal x1, qreal y1, qreal x2, qreal y2, QGraphicsItem * parent = 0) : QGraphicsLineItem(x1, y1, x2, y2,parent){}
    virtual ~ZAntialiasingLine(){}
    void paint(QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * widget = 0) override
    {
        painter->save();
        painter->setRenderHint(QPainter::Antialiasing);
        QGraphicsLineItem::paint(painter, option, widget);
        painter->restore();
    }
};

//========================================================
class ZDefaultRectGraphicsItem : public QGraphicsItem
{
public:
    ZDefaultRectGraphicsItem(const QRectF &, bool XAxesVisible, bool YAxesVisible, bool medianaLineVisible, QGraphicsItem* parent = 0);
    QRectF boundingRect() const override;
    void paint(QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * widget = 0) override;
    QPainterPath shape() const override;
    int type() const override;

    void zp_fitItemInRect(const QRectF& rect);

    void zp_setAxisVisible(Qt::Orientation orientation, bool visibilityFlag);
    void zp_setAxisColor(QColor);
    void zp_setMedianaLineColor(QColor);

    void zp_setMedianaLineVisible(bool visibilityFlag);


private:

    // VARS
    QRectF zv_boundingRect;
    QPainterPath zv_shape;
    QPainterPath zv_axisPath;
    QPainterPath zv_medianaLinePath;

    QColor zv_axisColor;
    QColor zv_medianaLineColor;


    bool zv_XAxesVisible;
    bool zv_YAxesVisible;
    bool zv_medianaLineVisible;

    // FUNCS
    void zh_remakePainterPaths();


};
//========================================================
#endif // ZDEFAULTRECTGRAPHICSITEM_H
