//====================================================
#include "ZChartPointOptions.h"
#include "ZChartPointGraphicsItem.h"
//====================================================
ZChartPointOptions::ZChartPointOptions()
{
    zv_pointType = PT_ROUND;
    zv_pointSize = 3;
    zv_outLineWidth = 1;

    zv_currentPointColor = QColor(Qt::red);
    zv_outLineColor = QColor(Qt::black);
    zv_pointColor = QColor(Qt::green);

    zv_horizontalRulerScaleValue = 0.001;
    zv_horizontalRulerMarkPrecision = 3;
    zv_horizontalRulerLabelString = QString();

    zv_verticalRulerScaleValue = 0.001;
    zv_verticalRulerMarkPrecision = 3;
    zv_verticalRulerLabelString = QString();

    zh_recalcShapeAndBoundingRect();
}
//====================================================
ZChartPointOptions::PointType ZChartPointOptions::zp_pointType() const
{
    return zv_pointType;
}
//====================================================
bool ZChartPointOptions::zp_setPointType(ZChartPointOptions::PointType type)
{
    if(zv_pointType == type)
    {
        return false;
    }

    zv_pointType = type;
    zh_recalcShapeAndBoundingRect();
    zh_updateItems();
    return true;
}
//====================================================
int ZChartPointOptions::zp_pointPixelSize() const
{
    return zv_pointSize;
}
//====================================================
bool ZChartPointOptions::zp_setPointPixelSize(int pointSize)
{
    if(zv_pointSize == pointSize)
    {
        return false;
    }

    zv_pointSize = pointSize;
    zh_recalcShapeAndBoundingRect();
    zh_updateItems();
    return true;
}
//====================================================
int ZChartPointOptions::zp_outLineWidth() const
{
    return zv_outLineWidth;
}
//====================================================
bool ZChartPointOptions::zp_setOutLineWidth(int outLineWidth)
{
    if(zv_outLineWidth == outLineWidth)
    {
        return false;
    }

    zv_outLineWidth = outLineWidth;
    zh_updateItems();
    return true;
}
//====================================================
QColor ZChartPointOptions::zp_currentPointColor() const
{
    return zv_currentPointColor;
}
//====================================================
bool ZChartPointOptions::zp_setCurrentPointColor(QColor currentPointColor)
{
    if(zv_currentPointColor == currentPointColor)
    {
        return false;
    }

    zv_currentPointColor = currentPointColor;
    zh_updateItems();
    return true;
}
//====================================================
QColor ZChartPointOptions::zp_outLineColor() const
{
    return zv_outLineColor;
}
//====================================================
bool ZChartPointOptions::zp_setOutLineColor(QColor outLineColor)
{
    if(zv_outLineColor == outLineColor)
    {
        return false;
    }

    zv_outLineColor = outLineColor;
    zh_updateItems();
    return true;
}
//====================================================
QColor ZChartPointOptions::zp_pointColor() const
{
    return zv_pointColor;
}
//====================================================
bool ZChartPointOptions::zp_setPointColor(QColor pointColor)
{
    if(zv_pointColor == pointColor)
    {
        return false;
    }

    zv_pointColor = pointColor;
    zh_updateItems();
    return true;
}
//====================================================
qreal ZChartPointOptions::zp_rulerScaleValue(Qt::Orientation orientation) const
{
    if(orientation == Qt::Horizontal)
    {
        return zv_horizontalRulerScaleValue;
    }
    else
    {
        return zv_verticalRulerScaleValue;
    }
}
//====================================================
bool ZChartPointOptions::zp_setRulerScaleValue(Qt::Orientation orientation, qreal scaleValue)
{
    if(orientation == Qt::Horizontal)
    {
         if(zv_horizontalRulerScaleValue == scaleValue)
         {
             return false;
         }
         zv_horizontalRulerScaleValue = scaleValue;
    }
    else
    {
        if(zv_verticalRulerScaleValue == scaleValue)
        {
            return false;
        }
        zv_verticalRulerScaleValue = scaleValue;
    }
    return  true;
}
//====================================================
QString ZChartPointOptions::zp_rulerLabelString(Qt::Orientation orientation) const
{
    if(orientation == Qt::Horizontal)
    {
        return zv_horizontalRulerLabelString;
    }
    else
    {
        return zv_verticalRulerLabelString;
    }
}
//====================================================
bool ZChartPointOptions::zp_setRulerLabelString(Qt::Orientation orientation, const QString& rulerLabelString)
{
    if(orientation == Qt::Horizontal)
    {
        if(zv_horizontalRulerLabelString == rulerLabelString)
        {
            return false;
        }
        zv_horizontalRulerLabelString = rulerLabelString;
    }
    else
    {
        if(zv_verticalRulerLabelString == rulerLabelString)
        {
            return false;
        }
        zv_verticalRulerLabelString = rulerLabelString;
    }
    return true;
}
//====================================================
int ZChartPointOptions::zp_rulerMarkPrecision(Qt::Orientation orientation) const
{
    if(orientation == Qt::Horizontal)
    {
        return zv_horizontalRulerMarkPrecision;
    }
    else
    {
        return zv_verticalRulerMarkPrecision;
    }
}
//====================================================
bool ZChartPointOptions::zp_setRulerMarkPrecision(Qt::Orientation orientation, int precision)
{
    if(orientation == Qt::Horizontal)
    {
        if(zv_horizontalRulerMarkPrecision == precision)
        {
            return false;
        }
        zv_horizontalRulerMarkPrecision = precision;
    }
    else
    {
        if(zv_verticalRulerMarkPrecision == precision)
        {
            return false;
        }
        zv_verticalRulerMarkPrecision = precision;
    }
    return true;
}
//====================================================
void ZChartPointOptions::zh_recalcShapeAndBoundingRect()
{
    // shape
    qreal pointHalfSize = (qreal)zv_pointSize / 2.0;
    QRectF rect = QRectF(-1*pointHalfSize, -1*pointHalfSize,
                         (qreal)zv_pointSize, (qreal)zv_pointSize);

    QPainterPath newShape;
    QPolygonF polygon;
    qreal a;
    switch(zv_pointType)
    {
    case PT_SQUARE:
        newShape.addRect(rect);
        break;
    case PT_TRIANGLE:
        a = pointHalfSize * 3.0 / sqrt(3.0);
        polygon.append(QPointF(0.0, -1*pointHalfSize));
        polygon.append(QPointF(-1*a/2, pointHalfSize/2));
        polygon.append(QPointF(a/2, pointHalfSize/2));
        polygon.append(QPointF(0.0, -1*pointHalfSize));

        newShape.addPolygon(polygon);
        break;
    case PT_ROUND:
        newShape.addEllipse(rect);
        break;
    default:
        newShape.addEllipse(rect);
    }

    zv_shape = newShape;
    zv_boundingRect = rect;
}
//====================================================
void ZChartPointOptions::zh_updateItems() const
{
    foreach(ZChartPointGraphicsItem* item, zv_seriesPointItemList)
    {
        item->zp_setNewBoundingRect(zv_boundingRect);
        item->update();
    }
}
//====================================================
