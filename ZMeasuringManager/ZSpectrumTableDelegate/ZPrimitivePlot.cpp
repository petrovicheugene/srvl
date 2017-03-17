//=======================================================
#include "ZPrimitivePlot.h"
#include <QPaintEvent>
#include <QStylePainter>
//=======================================================
ZPrimitivePlot::ZPrimitivePlot(QWidget *parent) : QFrame(parent)
{
    setFrameStyle(QFrame::Box | QFrame::Plain);
    setLineWidth(1);
    zv_paintStyle = PS_STAIRS_POLYGON;
    zv_antialiasingFlag = false;
}
//=======================================================
void ZPrimitivePlot::zp_paintData(const ZSpectrumPaintData &paintData)
{
    zh_recalcPaintData(paintData);
    repaint();
}
//=======================================================
void ZPrimitivePlot::zp_setPaintStyle(PaintStyle style)
{
    zv_paintStyle = style;
}
//=======================================================
void ZPrimitivePlot::zp_setPenColor(QColor color)
{
    zv_penColor = color;
}
//=======================================================
void ZPrimitivePlot::zp_setBrushColor(QColor color)
{
    zv_brushColor = color;
}
//=======================================================
void ZPrimitivePlot::zp_setAntialiasingFlag(bool flag)
{
    zv_antialiasingFlag = flag;
}
//=======================================================
void ZPrimitivePlot::paintEvent(QPaintEvent * event)
{
    QStylePainter painter(this);
    painter.setViewport(1, 1, this->width() - 2, this->height() - 2);
    painter.setWindow(0, (this->height() - 2) * (-1), this->width() - 2, this->height() - 2);
    painter.setRenderHint(QPainter::Antialiasing, zv_antialiasingFlag);


    if(zv_paintStyle == PS_DOTS)
    {
        zh_paintDots(&painter);
    }
    else if(zv_paintStyle == PS_STAIRS)
    {
        zh_paintStairs(&painter);
    }
    else if(zv_paintStyle == PS_POLYGON)
    {
        zh_paintPlygon(&painter);
    }
    else if(zv_paintStyle == PS_STAIRS_POLYGON)
    {
        zh_paintStairsPolygon(&painter);
    }
    else
    {
        zh_paintStraight(&painter);
    }
}
//=======================================================
void ZPrimitivePlot::zh_paintDots(QStylePainter* painter)
{

}
//=======================================================
void ZPrimitivePlot::zh_paintPlygon(QStylePainter* painter)
{
    //QPolygon polygon(zv_paintData.spectrumData);
    painter->save();
    painter->setPen(QPen(QBrush(zv_penColor), 1));
    painter->setBrush(QBrush(zv_brushColor));

    QPolygon polygon;
    foreach(QPoint point, zv_chartData)
    {
        polygon << point;
    }
    painter->drawPolygon(polygon);
    painter->restore();
}
//=======================================================
void ZPrimitivePlot::zh_paintStairsPolygon(QStylePainter* painter)
{
    painter->save();
    painter->setPen(QPen(QBrush(zv_penColor), 1));
    painter->setBrush(QBrush(zv_brushColor));

    //painter->setRenderHint(QPainter::Antialiasing);

    QPolygon polygon;
    // start point
    polygon << QPoint(0,zv_verticalShift * -1);
    for(int i = 0; i < zv_chartData.count() - 1; i++)
    {
        polygon << zv_chartData.value(i);
        polygon << QPoint(zv_chartData.value(i+1).x(), zv_chartData.value(i).y());
    }
    polygon << zv_chartData.last();
    // finish points
    polygon << QPoint(zv_chartData.last().x(), 0);

    painter->drawPolygon(polygon);
    painter->restore();
}
//=======================================================
void ZPrimitivePlot::zh_paintStairs(QStylePainter* painter)
{

}
//=======================================================
void ZPrimitivePlot::zh_paintStraight(QStylePainter* painter)
{

}
//=======================================================
void ZPrimitivePlot::zh_recalcPaintData(const ZSpectrumPaintData &paintData)
{
    // log Coordinates
    double channelPerPixel = (double)paintData.maxChannel / (double)(this->width() - 2);
    if(channelPerPixel <= 0)
    {
        channelPerPixel = 1;
    }

    double intensityPerPixel = (double)paintData.maxIntensity / (double)(this->height() - 2 - zv_verticalShift);
    if(intensityPerPixel <= 0)
    {
        intensityPerPixel = 1;
    }
    zv_chartData.clear();

    for(int i = 0; i < paintData.spectrumData.count(); i++)
    {
        int x = qRound((double)i / channelPerPixel);
        int y = -1 * qRound(((double)(paintData.spectrumData.value(i)) / intensityPerPixel) + zv_verticalShift);
        zv_chartData.append(QPoint(x,y));
    }
}
//=======================================================
