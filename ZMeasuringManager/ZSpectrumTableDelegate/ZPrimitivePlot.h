//=======================================================
#ifndef ZPRIMITIVEPLOT_H
#define ZPRIMITIVEPLOT_H
//=======================================================
#include <QFrame>
#include "ZSpectrumPaintData.h"
//=======================================================
class QStylePainter;
//=======================================================
class ZPrimitivePlot : public QFrame
{
    Q_OBJECT
public:
    explicit ZPrimitivePlot(QWidget *parent = 0);

    enum PaintStyle {PS_STRIGHT,
                     PS_STAIRS,
                     PS_DOTS,
                     PS_POLYGON,
                     PS_STAIRS_POLYGON};

    void zp_paintData(const ZSpectrumPaintData& paintData);
    void zp_setPaintStyle(PaintStyle);

    void zp_setPenColor(QColor);
    void zp_setBrushColor(QColor);
    void zp_setAntialiasingFlag(bool);

protected:
    // FUNCS
    void paintEvent(QPaintEvent * event);

private:

    // VARS
    PaintStyle zv_paintStyle;
    QColor zv_penColor;
    QColor zv_brushColor;
    const int zv_verticalShift = 1;
    bool zv_antialiasingFlag;
    // recalced paint data
    double zv_channelPerPixel;
    QVector<QPoint> zv_chartData;

    // FUNCS
    void zh_paintDots(QStylePainter*);
    void zh_paintPlygon(QStylePainter*);
    void zh_paintStairs(QStylePainter*);
    void zh_paintStairsPolygon(QStylePainter*);
    void zh_paintStraight(QStylePainter*);

    void zh_recalcPaintData(const ZSpectrumPaintData &paintData);

signals:

public slots:
};
//=======================================================
#endif // ZPRIMITIVEPLOT_H
