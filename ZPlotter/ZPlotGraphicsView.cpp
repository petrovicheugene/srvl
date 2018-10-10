//=============================================================
#include "ZPlotGraphicsView.h"
#include "ZRulersAndGridManager.h"
#include "ZGeneral.h"

#include <QApplication>
#include <QRubberBand>
#include <QGraphicsItem>
#include <QWheelEvent>
#include <QMenu>
#include <QMouseEvent>
#include <QScrollBar>
#include <QPixmap>
//=============================================================
ZPlotGraphicsView::ZPlotGraphicsView(QWidget* parent) : QGraphicsView(parent)
{
    zv_plotMode = PM_REGULAR;
    zv_sceneCenterPos = QPointF();
    zv_sceneMousePos = QPointF();
    zv_mousePressStartViewPos = QPoint();
    zv_rubberBand = 0;
    zv_scaleViewWhenResizeFlag = false;
    zv_drawGridFlag = true;
    zv_rulersAndGreedManager = 0;
    zv_minSideSizeOfVisibleScene = 15;
    zv_rubberBandSideMinSize = 5;
    zv_ruleToolMinSize = 5;
    zv_colorPickUpAuxCoverageSize = 1;
    zv_gridColor = viewport()->palette().color(QPalette::Mid);

    zv_XRuleList = 0;
    zv_YRuleList = 0;

    setViewportUpdateMode(QGraphicsView::FullViewportUpdate);
    //setViewportUpdateMode(QGraphicsView::SmartViewportUpdate);
    setMouseTracking(true);
    zh_createConnections();
}
//=============================================================
void ZPlotGraphicsView::zp_setScaleViewWhenResizeFlag(bool flag)
{
    zv_scaleViewWhenResizeFlag = flag;
}
//=============================================================
void ZPlotGraphicsView::zp_setViewport(QWidget * widget)
{
    QGraphicsView::setViewport(widget);
}
//=============================================================
void ZPlotGraphicsView::zp_connectToRulersAndGridManager(ZRulersAndGridManager* manager)
{
    zv_rulersAndGreedManager = manager;
    zv_rulersAndGreedManager->zp_setPlotGraphicsView(this);
}
//=============================================================
void ZPlotGraphicsView::zp_setHorizontalRulePointList(QList<RulePoint> * XRuleList)
{
    zv_XRuleList = XRuleList;
}
//=============================================================
void ZPlotGraphicsView::zp_setVerticalRulePointList(QList<RulePoint> * YRuleList)
{
    zv_YRuleList = YRuleList;
}
//=============================================================
bool ZPlotGraphicsView::zp_viewPortCoordinates(QRect& globalRect, QRectF& sceneRect, QPoint& globalZeroPoint)
{
    if(!scene())
    {
        return false;
    }

    // rects
    QRect viewportRect = viewport()->rect();
    sceneRect = mapToScene(viewport()->rect().adjusted(1,1,-1,-1)).boundingRect();

    globalRect = QRect(viewport()->mapToGlobal(viewportRect.topLeft()),
                       viewport()->mapToGlobal(viewportRect.bottomRight()));

    // global Zero Point
    globalZeroPoint = viewport()->mapToGlobal(mapFromScene(0.0, 0.0));

    return true;
}
//=============================================================
bool ZPlotGraphicsView::zp_viewPortSceneRect(QRectF&sceneRect)
{
    if(!scene())
    {
        return false;
    }

    sceneRect = mapToScene(viewport()->rect().adjusted(1,1,-1,-1)).boundingRect();
    return true;
}
//=============================================================
void ZPlotGraphicsView::zp_scale(Qt::Orientation orientation, bool increase)
{
    QRectF oldDisplayedSceneRect = mapToScene(viewport()->rect()).boundingRect();
    QRectF sceneItemBoundingRect = scene()->itemsBoundingRect();

    qreal scaleRate;
    if(orientation == Qt::Vertical)
    {
        if(increase)
        {
            scaleRate = oldDisplayedSceneRect.height() / zv_minSideSizeOfVisibleScene;
            if(scaleRate > 1.1)
            {
                scaleRate = 1.1;
            }
            if(scaleRate < 1.0)
            {
                scaleRate = 1.0;
            }
        }
        else
        {
            scaleRate = oldDisplayedSceneRect.height() / sceneItemBoundingRect.height();
            if(scaleRate < 0.9)
            {
                scaleRate = 0.9;
            }
            if(scaleRate > 1.0)
            {
                scaleRate = 1.0;
            }
        }

        scale(1, scaleRate);
        // center moving
        if(scaleRate > 1)
        {
            QRectF newDisplayedSceneRect = mapToScene(viewport()->rect()).boundingRect();
            qreal centerPosShift = (oldDisplayedSceneRect.height() - newDisplayedSceneRect.height()) / 2;
            zv_sceneCenterPos = mapToScene(viewport()->rect()).boundingRect().center();
            zv_sceneCenterPos.setY(zv_sceneCenterPos.y() + centerPosShift);
            centerOn(zv_sceneCenterPos);
        }
    }
    else
    {
        if(increase)
        {
            scaleRate = oldDisplayedSceneRect.width() / zv_minSideSizeOfVisibleScene;
            if(scaleRate > 1.1)
            {
                scaleRate = 1.1;
            }
            else if(scaleRate < 1.0)
            {
                scaleRate = 1.0;
            }
        }
        else
        {
            scaleRate = oldDisplayedSceneRect.width() / sceneItemBoundingRect.width();
            if(scaleRate < 0.9)
            {
                scaleRate = 0.9;
            }
            else if(scaleRate > 1.0)
            {
                scaleRate = 1.0;
            }
        }
        scale(scaleRate, 1);
    }

    //    QRectF oldDisplayedSceneRect = mapToScene(viewport()->rect().adjusted(2,-1,-2,1)).boundingRect();
    //    //QRectF oldDisplayedSceneRect = mapToScene(viewport()->rect()).boundingRect();
    //    QRectF displayedSceneRect = oldDisplayedSceneRect;
    //    QRectF sceneItemBoundingRect = scene()->itemsBoundingRect();

    //    qreal scaleRate;
    //    if(increase)
    //    {
    //        scaleRate = 0.1;
    //    }
    //    else
    //    {
    //        scaleRate = -0.1;
    //    }

    //    if(orientation == Qt::Vertical)
    //    {
    //        if(increase)
    //        {
    //            displayedSceneRect.setTop(displayedSceneRect.top() + displayedSceneRect.height() * scaleRate);
    //        }
    //        else
    //        {
    //            displayedSceneRect.setTop(displayedSceneRect.top() + displayedSceneRect.height() * scaleRate/2);
    //            // displayedSceneRect.setBottom(displayedSceneRect.bottom() - displayedSceneRect.height() * scaleRate/2);
    //        }

    ////        if(displayedSceneRect.top() < sceneItemBoundingRect.top())
    ////        {
    ////            displayedSceneRect.setTop(sceneItemBoundingRect.top());
    ////        }
    ////        if(displayedSceneRect.bottom() > sceneItemBoundingRect.bottom())
    ////        {
    ////            displayedSceneRect.setBottom(sceneItemBoundingRect.bottom());
    ////        }
    //    }
    //    else
    //    {
    //        displayedSceneRect.setRight(displayedSceneRect.right() - displayedSceneRect.width() * scaleRate);
    //        displayedSceneRect.setLeft(displayedSceneRect.left() + displayedSceneRect.width() * scaleRate);

    ////        if(displayedSceneRect.left() < sceneItemBoundingRect.left())
    ////        {
    ////            displayedSceneRect.setLeft(sceneItemBoundingRect.left());
    ////        }
    ////        if(displayedSceneRect.right() > sceneItemBoundingRect.right())
    ////        {
    ////            displayedSceneRect.setRight(sceneItemBoundingRect.right());
    ////        }
    //    }

    //    if(orientation == Qt::Horizontal && (displayedSceneRect.width() <= zv_minSideSizeOfVisibleScene
    //                                         || displayedSceneRect.width() > sceneItemBoundingRect.width()))
    //    {
    //        return;
    //    }

    //    if(orientation == Qt::Vertical && (displayedSceneRect.height() <= zv_minSideSizeOfVisibleScene
    //                                       || displayedSceneRect.height() > sceneItemBoundingRect.height()))
    //    {
    //        return;
    //    }

    //    fitInView(displayedSceneRect.normalized());
    //    //ensureVisible(displayedSceneRect.normalized(), 2, 2);
}
//=============================================================
void ZPlotGraphicsView::zp_setGridHidden(bool hideGrid)
{
    zv_drawGridFlag = !hideGrid;
    viewport()->update();
}
//=============================================================
bool ZPlotGraphicsView::zp_setColorPickUpAuxCoverageSize(int size)
{
    if(size < 0)
    {
        return false;
    }
    zv_colorPickUpAuxCoverageSize = size;
    return true;
}
//=============================================================
int ZPlotGraphicsView::zp_colorPickUpAuxCoverageSize() const
{
    return zv_colorPickUpAuxCoverageSize;
}
//=============================================================
bool ZPlotGraphicsView::zp_setRubberBandSideMinSize(int size)
{
    if(size < 0)
    {
        return false;
    }
    zv_rubberBandSideMinSize = size;
    return true;
}
//=============================================================
int ZPlotGraphicsView::zp_rubberBandSideMinSize() const
{
    return zv_rubberBandSideMinSize;
}
//=============================================================
QColor ZPlotGraphicsView::zp_gridColor() const
{
    return zv_gridColor;
}
//=============================================================
void ZPlotGraphicsView::zp_fitInView(const QRectF &rect, Qt::AspectRatioMode aspectRatioMode)
{
    QGraphicsView::fitInView(rect, aspectRatioMode);
    emit zg_viewportRectChanged(rect);

    if(zv_plotMode == PM_RULER)
    {
        emit zg_rulerToolChanged(mapToScene(zv_mousePressStartViewPos),
                                 mapToScene(zv_currentMousePos), true);
    }
}
//=============================================================
void ZPlotGraphicsView::zp_update()
{
    viewport()->update();
}
//=============================================================
QRectF ZPlotGraphicsView::zp_currentVisibleSceneRect() const
{
    // QRect rect = viewport()->rect().adjusted(1,1,-1,-1);
    QRect rect = viewport()->rect().adjusted(2,-1,-2,1);
    QPointF sceneTopLeft = mapToScene(rect.topLeft());
    QPointF scenebottomRight = mapToScene(rect.bottomRight());

    QRectF sRect(sceneTopLeft, scenebottomRight);
    return sRect;
}
//=============================================================
void ZPlotGraphicsView::wheelEvent(QWheelEvent * event)
{
    if(zv_plotMode != PM_REGULAR || !scene())
    {
        return;
    }

    zv_sceneMousePos = mapToScene(event->pos());
    QRectF oldDisplayedSceneRect = mapToScene(viewport()->rect().adjusted(2,-1,-2,1)).boundingRect();
    //QRectF oldDisplayedSceneRect = mapToScene(viewport()->rect()).boundingRect();
    QRectF displayedSceneRect = oldDisplayedSceneRect;
    qreal rightPartFactor = 0.1 *(displayedSceneRect.right() - zv_sceneMousePos.x())
            / (displayedSceneRect.right() - displayedSceneRect.left());
    qreal topPartFactor = 0.1* (zv_sceneMousePos.y() - displayedSceneRect.top())
            / (displayedSceneRect.bottom() - displayedSceneRect.top());

    qreal leftPartFactor = 0.1 - rightPartFactor;
    qreal bottomPartFactor = 0.1 - topPartFactor;

    if(event->delta() > 0)
    {
        topPartFactor *= -1;
        leftPartFactor *= -1;
    }
    else
    {
        rightPartFactor *= -1;
        bottomPartFactor *= -1;
    }

    displayedSceneRect.setTop(displayedSceneRect.top() + topPartFactor*displayedSceneRect.height());
    displayedSceneRect.setBottom(displayedSceneRect.bottom() + bottomPartFactor*displayedSceneRect.height());
    displayedSceneRect.setLeft(displayedSceneRect.left() + leftPartFactor*displayedSceneRect.width());
    displayedSceneRect.setRight(displayedSceneRect.right() + rightPartFactor*displayedSceneRect.width());

    QRectF sceneItemBoundingRect = scene()->itemsBoundingRect();
    if(displayedSceneRect.top() < sceneItemBoundingRect.top())
    {
        displayedSceneRect.setTop(sceneItemBoundingRect.top());
    }
    if(displayedSceneRect.bottom() > sceneItemBoundingRect.bottom())
    {
        displayedSceneRect.setBottom(sceneItemBoundingRect.bottom());
    }
    if(displayedSceneRect.left() < sceneItemBoundingRect.left())
    {
        displayedSceneRect.setLeft(sceneItemBoundingRect.left());
    }
    if(displayedSceneRect.right() > sceneItemBoundingRect.right())
    {
        displayedSceneRect.setRight(sceneItemBoundingRect.right());
    }

    if(displayedSceneRect.height() < zv_minSideSizeOfVisibleScene)
    {
        displayedSceneRect.setTop(oldDisplayedSceneRect.top());
        displayedSceneRect.setBottom(oldDisplayedSceneRect.bottom());
    }
    if(displayedSceneRect.width() < zv_minSideSizeOfVisibleScene)
    {
        displayedSceneRect.setLeft(oldDisplayedSceneRect.left());
        displayedSceneRect.setRight(oldDisplayedSceneRect.right());
    }

    zp_fitInView(displayedSceneRect.normalized());
    //ensureVisible(displayedSceneRect.normalized(), 2, 2);
    zv_sceneCenterPos = mapToScene(viewport()->rect()).boundingRect().center();
    zv_sceneMousePos = mapToScene(event->pos());
}
//=============================================================
void ZPlotGraphicsView::mousePressEvent(QMouseEvent* event)
{
    if(event->button() == Qt::LeftButton)
    {
        // pad moving
        //      QGraphicsItem* item = itemAt(event->pos());
        //      if(!item || !(item->flags() & QGraphicsItem::ItemIsMovable))
        //      if(!zh_findSpectrumInCursorArea(event->pos()))
        //      {
        //zv_plotMode = PM_PAD_DRAGGING;
        zv_sceneCenterPos = mapToScene(viewport()->rect()).boundingRect().center();
        zv_sceneMousePos = mapToScene(event->pos());
        zv_mousePressStartViewPos = event->pos();
        emit zg_mousePressedAt(zv_sceneMousePos);
        return;
        // }
    }
    else if(event->button() == Qt::RightButton)
    {
        zv_mousePressStartViewPos = event->pos();
    }

    QGraphicsView::mousePressEvent(event);
}
//=============================================================
void ZPlotGraphicsView::mouseReleaseEvent(QMouseEvent* event)
{
    if(zv_plotMode == PM_PAD_DRAGGING)
    {

    }
    else if(zv_plotMode == PM_RUBBER_BAND)
    {
        zv_rubberBand->hide();
        QPoint topLeft;
        if(zv_mousePressStartViewPos.x() > event->pos().x())
        {
            topLeft.setX(event->pos().x());
        }
        else
        {
            topLeft.setX(zv_mousePressStartViewPos.x());
        }

        if(zv_mousePressStartViewPos.y() > event->pos().y())
        {
            topLeft.setY(event->pos().y());
        }
        else
        {
            topLeft.setY(zv_mousePressStartViewPos.y());
        }

        QRect rubberBandRect = zv_rubberBand->rect();
        rubberBandRect.moveTo(topLeft);

        QRectF newSceneRect = mapToScene(rubberBandRect).boundingRect().normalized();
        QPointF newSceneCenter =  newSceneRect.center();
        if(newSceneRect.width() < zv_minSideSizeOfVisibleScene)
        {
            newSceneRect.setWidth(zv_minSideSizeOfVisibleScene);
        }
        if(newSceneRect.height() < zv_minSideSizeOfVisibleScene)
        {
            newSceneRect.setHeight(zv_minSideSizeOfVisibleScene);
        }

        if(newSceneRect.center() != newSceneCenter)
        {
            newSceneRect.moveCenter(newSceneCenter);
        }
        zp_fitInView(newSceneRect);
        //ensureVisible(newSceneRect, 2, 2);
    }
    else if(zv_plotMode == PM_REGULAR && zv_mousePressStartViewPos == event->pos())
    {
        emit zg_cursorAreaImage(zh_grabCursorArea(event->pos()));
    }
    else if(zv_plotMode == PM_RULER)
    {
        emit zg_rulerToolChanged(QPointF(), QPointF(), false);
    }

    qApp->restoreOverrideCursor();
    zv_plotMode = PM_REGULAR;
    QGraphicsView::mouseReleaseEvent(event);
}
//=============================================================
void ZPlotGraphicsView::mouseMoveEvent(QMouseEvent* event)
{
    emit zg_mouseScenePositionChanged(mapToScene(event->pos()));

    zv_currentMousePos = event->pos();

    if(zv_plotMode == PM_PAD_DRAGGING)
    {
        QPointF newSceneMousePos = mapToScene(event->pos());
        qreal deltaX = zv_sceneMousePos.x() - newSceneMousePos.x();
        qreal deltaY = zv_sceneMousePos.y() - newSceneMousePos.y();
        QPointF newSceneCenter(zv_sceneCenterPos.x() + deltaX, zv_sceneCenterPos.y() + deltaY);
        centerOn(newSceneCenter);
        zv_sceneCenterPos = mapToScene(viewport()->rect()).boundingRect().center();
        zv_sceneMousePos = mapToScene(event->pos());
        return;
    }
    else if(zv_plotMode == PM_RUBBER_BAND)
    {
        if(qAbs(zv_mousePressStartViewPos.x() - zv_currentMousePos.x()) < zv_rubberBandSideMinSize
                || qAbs(zv_mousePressStartViewPos.y() - zv_currentMousePos.y()) < zv_rubberBandSideMinSize)
        {
            zv_rubberBand->hide();
            zv_plotMode = PM_REGULAR;
        }
        else
        {
            zv_rubberBand->setGeometry(QRect(zv_mousePressStartViewPos, event->pos()).normalized());
            return;
        }
    }
    else if(zv_plotMode == PM_REGULAR && event->buttons() == Qt::RightButton)
    {
        if(qAbs(zv_mousePressStartViewPos.x() - zv_currentMousePos.x()) > zv_rubberBandSideMinSize
                && qAbs(zv_mousePressStartViewPos.y() - zv_currentMousePos.y()) > zv_rubberBandSideMinSize
                && !(event->modifiers() & Qt::CTRL))
        {
            QRectF sceneRect = mapToScene(viewport()->rect().adjusted(1,1,-1,-1)).boundingRect().normalized();
            if(sceneRect.width() > zv_minSideSizeOfVisibleScene + 1
                    || sceneRect.height() > zv_minSideSizeOfVisibleScene +1)
            {
                zv_plotMode = PM_RUBBER_BAND;
                if(!zv_rubberBand)
                {
                    zv_rubberBand = new QRubberBand(QRubberBand::Rectangle, this);
                }
                zv_rubberBand->setGeometry(QRect(zv_mousePressStartViewPos, QSize()));
                zv_rubberBand->show();
            }
        }
        else if(event->modifiers() & Qt::CTRL/*qAbs(zv_mousePressStartViewPos.x() - zv_currentMousePos.x()) > zv_ruleToolMinSize*/)
        {
            zv_plotMode = PM_RULER;
            emit zg_rulerToolChanged(mapToScene(zv_mousePressStartViewPos),
                                     mapToScene(zv_currentMousePos), true);
        }
        else
        {
            zv_plotMode = PM_REGULAR;
            emit zg_rulerToolChanged(QPointF(), QPointF(), false);
        }
    }
    else if(zv_plotMode == PM_REGULAR && event->buttons() == Qt::LeftButton)
    {
        if(zv_mousePressStartViewPos != event->pos())
        {
            qApp->setOverrideCursor(Qt::ClosedHandCursor);
            zv_plotMode = PM_PAD_DRAGGING;
            QPointF newSceneMousePos = mapToScene(event->pos());
            qreal deltaX = zv_sceneMousePos.x() - newSceneMousePos.x();
            qreal deltaY = zv_sceneMousePos.y() - newSceneMousePos.y();
            QPointF newSceneCenter(zv_sceneCenterPos.x() + deltaX, zv_sceneCenterPos.y() + deltaY);
            centerOn(newSceneCenter);
            zv_sceneCenterPos = mapToScene(viewport()->rect()).boundingRect().center();
            zv_sceneMousePos = mapToScene(event->pos());
            return;
        }
    }
    else if(zv_plotMode == PM_RULER)
    {
        if(qAbs(zv_mousePressStartViewPos.x() - zv_currentMousePos.x()) > zv_rubberBandSideMinSize
                && qAbs(zv_mousePressStartViewPos.y() - zv_currentMousePos.y()) > zv_rubberBandSideMinSize
                && !(event->modifiers() & Qt::CTRL))
        {
            QRectF sceneRect = mapToScene(viewport()->rect().adjusted(1,1,-1,-1)).boundingRect().normalized();
            if(sceneRect.width() > zv_minSideSizeOfVisibleScene + 1
                    || sceneRect.height() > zv_minSideSizeOfVisibleScene +1)
            {
                zv_plotMode = PM_RUBBER_BAND;
                emit zg_rulerToolChanged(QPointF(), QPointF(), false);
                if(!zv_rubberBand)
                {
                    zv_rubberBand = new QRubberBand(QRubberBand::Rectangle, this);
                }
                zv_rubberBand->setGeometry(QRect(zv_mousePressStartViewPos, QSize()));
                zv_rubberBand->show();
            }
        }
        else if(event->modifiers() & Qt::CTRL)
        {
            emit zg_rulerToolChanged(mapToScene(zv_mousePressStartViewPos),
                                     mapToScene(zv_currentMousePos), true);
        }
        else
        {
            zv_plotMode = PM_REGULAR;
            emit zg_rulerToolChanged(QPointF(), QPointF(), false);
        }
    }
    QGraphicsView::mouseMoveEvent(event);
}
//=============================================================
bool ZPlotGraphicsView::viewportEvent(QEvent * event)
{
    if(event->type() == QEvent::Paint && zv_plotMode == PM_RULER)
    {
        emit zg_rulerToolChanged(mapToScene(zv_mousePressStartViewPos),
                                 mapToScene(zv_currentMousePos), true);
    }

    if(event->type() == QEvent::HoverLeave || event->type() == QEvent::Leave)
    {
        emit zg_mouseLeaved();
    }

    if(event->type() == QEvent::Resize && scene())
    {
        QResizeEvent* resizeEvent = static_cast<QResizeEvent*>(event);

        if(zv_scaleViewWhenResizeFlag
                &&resizeEvent->oldSize().width() != 0
                && resizeEvent->oldSize().height() != 0)
        {
            QPointF sceneCenterPos = mapToScene(viewport()->rect()).boundingRect().center();

            double dx = (double)(resizeEvent->size().width())
                    / (double)(resizeEvent->oldSize().width());

            double dy = (double)(resizeEvent->size().height())
                    / (double)(resizeEvent->oldSize().height());

            scale(dx, dy);
            centerOn(sceneCenterPos);
        }
    }
    else if(event->type() == QEvent::Paint && scene())
    {
        if(zv_rulersAndGreedManager)
        {
            zv_rulersAndGreedManager->zp_recalcRulesAndGrid();
        }
    }

    return QGraphicsView::viewportEvent(event);
}
//=============================================================
void ZPlotGraphicsView::drawBackground(QPainter * painter, const QRectF & rect)
{
    if(zv_rulersAndGreedManager != 0 && zv_drawGridFlag)
    {
        // rect adjusting
        qreal K_width = rect.width() * 1.1/
                rect.width();
        qreal K_height = rect.height()  * 1.1/
                rect.height();

        int dPix = 1;
        const QRectF adjustedRect = rect.adjusted(-1*K_width*dPix, -1*K_height*dPix, K_width*dPix, K_height*dPix);
        // painter for MARK GRID
        painter->save();

        // X
        if(zv_XRuleList != 0)
        {
            for(int i = 0; i < zv_XRuleList->count(); i++)
            {
                // pen settings
                QPen pen;
                if(zv_XRuleList->value(i).markType == RulePoint::MT_SCRATCH)
                {
                    pen = QPen(QBrush(zv_gridColor), 1, Qt::DotLine);
                    pen.setCosmetic(true);
                }
                else
                {
                    pen = QPen(QBrush(zv_gridColor), 1, Qt::DashLine);
                    pen.setCosmetic(true);
                }
                painter->setPen(pen);
                painter->drawLine(zv_XRuleList->value(i).scenePos, adjustedRect.top(),
                                  zv_XRuleList->value(i).scenePos, adjustedRect.bottom());
            }
        }

        // Y
        if(zv_YRuleList != 0)
        {
            for(int i = 0; i < zv_YRuleList->count(); i++)
            {
                // pen settings
                QPen pen;
                if(zv_YRuleList->value(i).markType == RulePoint::MT_SCRATCH)
                {
                    pen = QPen(QBrush(zv_gridColor), 1, Qt::DotLine);
                    pen.setCosmetic(true);
                }
                else
                {
                    pen = QPen(QBrush(zv_gridColor), 1, Qt::DashLine);
                    pen.setCosmetic(true);
                }
                painter->setPen(pen);
                painter->drawLine(adjustedRect.left(), zv_YRuleList->value(i).scenePos,
                                  adjustedRect.right(), zv_YRuleList->value(i).scenePos);
            }
        }
        painter->restore();
        return;
    }

    QGraphicsView::drawBackground(painter, rect);
}
//=============================================================
void ZPlotGraphicsView::drawForeground(QPainter *painter, const QRectF &rect)
{
    QGraphicsView::drawForeground(painter, rect);
}
//=============================================================
void ZPlotGraphicsView::contextMenuEvent(QContextMenuEvent *event)
{
    if(zv_plotMode == PM_REGULAR && zv_mousePressStartViewPos == event->pos())
    {
        QMenu menu;
        menu.addActions(this->actions());
        menu.move(mapToGlobal(event->pos()));
        QAction* action =  menu.exec();
        if(action != nullptr)
        {
            action->setData(QVariant(mapToScene(event->pos())));
            action->trigger();
        }
    }
}
//=============================================================
void ZPlotGraphicsView::zh_createConnections()
{

}
//=============================================================
QImage ZPlotGraphicsView::zh_grabCursorArea(QPoint mousePos)
{
    int rectSide = zv_colorPickUpAuxCoverageSize*2 + 1; // 1 - central pixel
    QPoint topLeft;
    topLeft.setY(mousePos.y() - zv_colorPickUpAuxCoverageSize);
    topLeft.setX(mousePos.x() - zv_colorPickUpAuxCoverageSize);

    QRect rect = QRect(topLeft.x(), topLeft.y(), rectSide, rectSide);
    QImage grabbedArea = grab(rect).toImage();

    return grabbedArea;
}
//=============================================================
