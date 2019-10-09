//=============================================================
#ifndef ZGRAPHICSVIEWPLOT_H
#define ZGRAPHICSVIEWPLOT_H
//=============================================================
#include <QGraphicsView>
#include "ZRulersAndGridManager.h"
//=============================================================
class QWheelEvent;
class QRubberBand;
class ZHorizontalDashBoard;
//=============================================================
class ZPlotGraphicsView : public QGraphicsView
{
    Q_OBJECT
public:
    explicit ZPlotGraphicsView(QWidget *parent = nullptr);

    // FUNC
    QRectF zp_currentVisibleSceneRect() const;

    void zp_setScaleViewWhenResizeFlag(bool);
    void zp_setViewport(QWidget * widget);
    void zp_connectToRulersAndGridManager(ZRulersAndGridManager*);

    void zp_setHorizontalRulePointList(QList<RulePoint> *);
    void zp_setVerticalRulePointList(QList<RulePoint> *);
    bool zp_viewPortCoordinates(QRect&globalRect, QRectF&sceneRect, QPoint &nullGlobalPoint);
    bool zp_viewPortSceneRect(QRectF&sceneRect);

    void zp_scale(Qt::Orientation orientation, bool increase);
    void zp_setGridHidden(bool);

    bool zp_setColorPickUpAuxCoverageSize(int);
    int zp_colorPickUpAuxCoverageSize() const;

    bool zp_setRubberBandSideMinSize(int);
    int zp_rubberBandSideMinSize() const;

    QColor zp_gridColor() const;

    void zp_fitInView(const QRectF &rect, Qt::AspectRatioMode aspectRatioMode = Qt::IgnoreAspectRatio);

    void zp_update();

signals:

    void zg_mouseScenePositionChanged(QPointF scenePos) const;
    void zg_mouseLeaved() const;

    void zg_inquiryForScrollBarVisible(Qt::Orientation, bool&);
    void zg_cursorAreaImage(QImage);
    void zg_mousePressedAt(QPointF);
    void zg_viewportRectChanged(QRectF) const;
    void zg_rulerToolChanged(QPointF startPoint, QPointF endPoint, bool visibility) const;


protected:

    // FUNCS
    // reimplemented
    void wheelEvent(QWheelEvent * event);
    void mousePressEvent(QMouseEvent* event);
    void mouseReleaseEvent(QMouseEvent* event);

    void mouseMoveEvent(QMouseEvent *event);
    bool viewportEvent(QEvent* event);
    void drawBackground(QPainter * painter, const QRectF & rect);
    void drawForeground(QPainter *painter, const QRectF &rect);

    void contextMenuEvent(QContextMenuEvent *event);

    // just hidden
    void	setViewport(QWidget * widget);


    void fitInView(const QRectF &rect, Qt::AspectRatioMode aspectRatioMode = Qt::IgnoreAspectRatio);
    void fitInView(qreal x, qreal y, qreal w, qreal h, Qt::AspectRatioMode aspectRatioMode = Qt::IgnoreAspectRatio);
    void fitInView(const QGraphicsItem *item, Qt::AspectRatioMode aspectRatioMode = Qt::IgnoreAspectRatio);


private:

    // VARS
    enum PlotMode{PM_REGULAR,
                  PM_PAD_DRAGGING,
                  PM_RUBBER_BAND,
                  PM_RULER};

    ZRulersAndGridManager* zv_rulersAndGreedManager;
    PlotMode zv_plotMode;
    QPointF zv_sceneCenterPos;
    QPointF zv_sceneMousePos;
    QPoint zv_mousePressStartViewPos;
    QPoint zv_currentMousePos;
    QRubberBand* zv_rubberBand;
    bool zv_scaleViewWhenResizeFlag;
    bool zv_drawGridFlag;

    qreal zv_minSideSizeOfVisibleScene;
    int zv_rubberBandSideMinSize;
    int zv_ruleToolMinSize;
    QList<RulePoint> * zv_XRuleList;
    QList<RulePoint> * zv_YRuleList;

    int zv_colorPickUpAuxCoverageSize;
    QColor zv_gridColor;
    // FUNCS
    void zh_createConnections();
    QImage zh_grabCursorArea(QPoint);
};
//=============================================================
#endif // ZGRAPHICSVIEWPLOT_H
