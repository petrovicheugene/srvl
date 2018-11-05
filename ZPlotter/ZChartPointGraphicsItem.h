//======================================================
#ifndef ZSERIESPOINTGRAPHICSITEM_H
#define ZSERIESPOINTGRAPHICSITEM_H
//======================================================
#include <QGraphicsItem>
#include "ZVisibilityPointF.h"

//======================================================
class ZChartPointOptions;
class ZCorrelationPlotterDataManager;
//======================================================
class ZChartPointGraphicsItem : public QGraphicsItem
{
    friend class ZChartPointOptions;
public:

    explicit ZChartPointGraphicsItem(const ZCorrelationPlotterDataManager* dataManager,
                                     const ZVisibilityPointF &visibilityPoint,
                                      ZChartPointOptions* seriesPointOptions,
                                      qint64 relatedObjectId,
                                      QGraphicsItem * parent = 0);
    ~ZChartPointGraphicsItem();
    static void zp_setCurrentSpectrumId(qint64);
    static void zp_setCurrentSpectrumColor(QColor);

    // overrided
    QRectF boundingRect() const override;
    void paint(QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * widget = 0) override;
    QPainterPath shape() const override;
    int type() const override;

    // FUNCS
    ZChartPointOptions* zp_seriesPointOptions() const;
    void zp_setNewBoundingRect(const QRectF& boundingRect);

    qint64 zp_relatedObjectId() const;
    void zp_applyVisibilityAndPos(const ZVisibilityPointF&, ZChartPointOptions* chartPointOptions);
    //qint64 zp_pointId();
    void zp_updateCurrentItem();

protected:

    // FUNCS
    virtual void	mousePressEvent(QGraphicsSceneMouseEvent * event);
    virtual void	mouseReleaseEvent(QGraphicsSceneMouseEvent * event);

private:

    // VARS
    const ZCorrelationPlotterDataManager* zv_dataManager;
    ZChartPointOptions* zv_chartPointOptions;
    qint64 zv_relatedObjectId;
    QColor zv_paintColor;
    static QColor zv_currentColor;
    static qint64 zv_currentSpectrumId;

    //QRectF zv_boundingRect;
};
//======================================================
#endif // ZSERIESPOINTGRAPHICSITEM_H
