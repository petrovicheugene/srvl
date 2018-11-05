//======================================================
#include "ZChartPointGraphicsItem.h"
#include "ZGraphicsItemUserTypes.h"
#include "ZChartPointOptions.h"
#include "ZGLConstantsAndDefines.h"
#include "ZPlotterDefaulVariables.h"
#include "ZCorrelationPlotterDataManager.h"

#include <QGraphicsRectItem>
#include <QBrush>
#include <QPen>
#include <QPainter>
#include <QPolygonF>
//======================================================
qint64 ZChartPointGraphicsItem::zv_currentSpectrumId = -1;
QColor ZChartPointGraphicsItem::zv_currentColor = QColor(Qt::red);
//======================================================
ZChartPointGraphicsItem::ZChartPointGraphicsItem(const ZCorrelationPlotterDataManager* dataManager,
                                                 const ZVisibilityPointF& visibilityPoint,
                                                 ZChartPointOptions *seriesPointOptions,
                                                 qint64 relatedObjectId,
                                                 QGraphicsItem *parent) :
    QGraphicsItem(parent)
{
    zv_dataManager = dataManager;
    zv_relatedObjectId = relatedObjectId;
    // color of current point by default
    prepareGeometryChange();
    zv_chartPointOptions = seriesPointOptions;
    zv_chartPointOptions->zv_seriesPointItemList.append(this);
    setFlag(QGraphicsItem::ItemIgnoresTransformations);
    setZValue(gl_defaultSpectrumZValue);

    zp_applyVisibilityAndPos(visibilityPoint, zv_chartPointOptions);
}
//======================================================
ZChartPointGraphicsItem::~ZChartPointGraphicsItem()
{
    int index = zv_chartPointOptions->zv_seriesPointItemList.indexOf(this);
    if(index >= 0)
    {
        zv_chartPointOptions->zv_seriesPointItemList.removeAt(index);
    }
}
//======================================================
QRectF ZChartPointGraphicsItem::boundingRect() const
{
    return zv_chartPointOptions->zv_boundingRect;
}
//======================================================
void ZChartPointGraphicsItem::zp_setCurrentSpectrumColor(QColor color)
{
    zv_currentColor = color;
}
//======================================================
void ZChartPointGraphicsItem::zp_setCurrentSpectrumId(qint64 currentSpectrumId)
{
    zv_currentSpectrumId = currentSpectrumId;
}
//======================================================
void ZChartPointGraphicsItem::paint(QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * widget)
{
    painter->save();
    painter->setRenderHint(QPainter::Antialiasing);
    // window body
    painter->setBrush(QBrush(zv_paintColor));
    QPen pen(QBrush(zv_chartPointOptions->zv_outLineColor), zv_chartPointOptions->zv_outLineWidth);
    //pen.setCosmetic(true);
    painter->setPen(pen);
    painter->drawPath(zv_chartPointOptions->zv_shape);
    painter->restore();
}
//======================================================
QPainterPath ZChartPointGraphicsItem::shape() const
{
    return zv_chartPointOptions->zv_shape;
}
//======================================================
int ZChartPointGraphicsItem::type() const
{
    return ChartPointItemType;
}
//======================================================
ZChartPointOptions* ZChartPointGraphicsItem::zp_seriesPointOptions() const
{
    return zv_chartPointOptions;
}
//======================================================
void ZChartPointGraphicsItem::zp_setNewBoundingRect(const QRectF& boundingRect)
{
    //    if(zv_boundingRect == boundingRect)
    //    {
    //        return;
    //    }

    prepareGeometryChange();
    // zv_boundingRect = boundingRect;
}
//======================================================
qint64 ZChartPointGraphicsItem::zp_relatedObjectId() const
{
    return zv_relatedObjectId;
}
//======================================================
void ZChartPointGraphicsItem::zp_applyVisibilityAndPos(const ZVisibilityPointF &visibilityPoint,
                                                       ZChartPointOptions* chartPointOptions)
{
    zv_chartPointOptions = chartPointOptions;
    ZVisibilityPointF vPoint = visibilityPoint;
    vPoint.setY(vPoint.y() * (-1) / zv_chartPointOptions->zp_rulerScaleValue(Qt::Vertical));
    vPoint.setX(vPoint.x() / zv_chartPointOptions->zp_rulerScaleValue(Qt::Horizontal));
    // zv_boundingRect = seriesPointOptions->zv_boundingRect;
    setPos(vPoint);
    //    setZValue(2);
    setVisible(vPoint.zp_isVisible());
    zp_updateCurrentItem();
}
//======================================================
void ZChartPointGraphicsItem::zp_updateCurrentItem()
{
    if(zv_currentSpectrumId == zv_relatedObjectId)
    {
        setZValue(gl_currentSpectrumZValue);
        zv_paintColor = zv_currentColor;
        setVisible(isVisible());
    }
    else
    {
        //      if(zValue() != gl_defaultSpectrumZValue)
        //      {
        setZValue(gl_defaultSpectrumZValue);
        zv_paintColor = zv_chartPointOptions->zv_pointColor;
        setVisible(isVisible());
        //      }
    }
}
//======================================================
void ZChartPointGraphicsItem::mousePressEvent(QGraphicsSceneMouseEvent* event)
{
    zv_dataManager->zp_setCurrentSpectrum(zv_relatedObjectId);

    //QGraphicsItem::mousePressEvent(event);
}
//======================================================
void ZChartPointGraphicsItem::mouseReleaseEvent(QGraphicsSceneMouseEvent* event)
{
#ifdef DBG
    qDebug() << "ITEM MOUSE RELEASE";
#endif

    zv_dataManager->zp_setCurrentSpectrum(zv_relatedObjectId);
}
//======================================================
