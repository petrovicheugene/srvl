//====================================================
#include "ZPlotter.h"
#include "ZPlotGraphicsScene.h"
#include "ZPlotGraphicsView.h"
#include "ZRulersAndGridManager.h"
#include "ZRulerWidget.h"
#include "ZEnergyLineGraphicsItem.h"
#include "ZSpectrumGraphicsItem.h"
#include "ZWindowGraphicsItem.h"
#include "ZChartPointGraphicsItem.h"

#include "ZGeneral.h"
#include "ZPlotterDefaulVariables.h"

#include <QApplication>
#include <QLabel>
#include <QScrollBar>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QScrollBar>
#include <QDebug>

//====================================================
extern const qreal gl_defaultSpectrumZValue = 2;
extern const qreal gl_currentSpectrumZValue = 3;
extern const qreal gl_defaultWindowZValue = 1;
extern const qreal gl_currentWindowZValue = 4;


//====================================================
ZPlotter::ZPlotter(QWidget *parent) : QWidget(parent)
{
    zv_rulerWidget = 0;
    zv_plotView = 0;
    zv_verticalScrollBar = 0;
    zv_horizontalScrollBar = 0;
    zv_plotScene = 0;
    zv_rulersAndGreedManager = 0;
    zv_dashBoard = 0;

    zv_verticalAbsMax = 0;
    zv_autoDefineVerticalAbsMax = true;
    zv_verticalDistortionFactor = 0;
    zv_verticalDistortionCorrectionFactor = 0;

    zv_mouseButtonDown = false;
    zv_userResizesWidget = false;
    zh_createComponents();
    zh_createConnections();
    qApp->installEventFilter(this);
}
//====================================================
//ZPlotGraphicsScene* ZPlotter::zp_plotScene()
//{
//    return zv_plotScene;
//}
//====================================================
bool ZPlotter::zp_setMinimalVerticalScaleInterval(qreal interval)
{
    if(!zv_rulersAndGreedManager)
    {
        return false;
    }
    return zv_rulersAndGreedManager->zp_setMinimalVerticalScaleInterval(interval);
}
//====================================================
qreal ZPlotter::zp_minimalVerticalScaleInterval()
{
    if(!zv_rulersAndGreedManager)
    {
        return -1.0;
    }
    return zv_rulersAndGreedManager->zp_minimalVerticalScaleInterval();
}
//====================================================
bool ZPlotter::zp_setMinimalHorizontalScaleInterval(qreal interval)
{
    if(!zv_rulersAndGreedManager)
    {
        return false;
    }
    return zv_rulersAndGreedManager->zp_setMinimalHorizontalScaleInterval(interval);
}
//====================================================
qreal ZPlotter::zp_minimalHorizontalScaleInterval()
{
    if(!zv_rulersAndGreedManager)
    {
        return -1.0;
    }
    return zv_rulersAndGreedManager->zp_minimalHorizontalScaleInterval();
}
//====================================================
bool ZPlotter::zp_setLeftMarkFontSize(int size)
{
    if(!zv_rulerWidget)
    {
        return false;
    }
    return zv_rulerWidget->zp_setLeftFontSize(size);
}
//====================================================
bool ZPlotter::zp_setRightMarkFontSize(int size)
{
    if(!zv_rulerWidget)
    {
        return false;
    }
    return zv_rulerWidget->zp_setRightFontSize(size);
}
//====================================================
bool ZPlotter::zp_setTopMarkFontSize(int size)
{
    if(!zv_rulerWidget)
    {
        return false;
    }
    return zv_rulerWidget->zp_setTopFontSize(size);
}
//====================================================
bool ZPlotter::zp_setBottomMarkFontSize(int size)
{
    if(!zv_rulerWidget)
    {
        return false;
    }
    return zv_rulerWidget->zp_setBottomFontSize(size);
}
//====================================================
int ZPlotter::zp_leftMarkFontSize()
{
    if(!zv_rulerWidget)
    {
        return -1;
    }
    return zv_rulerWidget->zp_leftFontSize();
}
//====================================================
int ZPlotter::zp_rightMarkFontSize()
{
    if(!zv_rulerWidget)
    {
        return -1;
    }
    return zv_rulerWidget->zp_rightFontSize();
}
//====================================================
int ZPlotter::zp_topMarkFontSize()
{
    if(!zv_rulerWidget)
    {
        return -1;
    }
    return zv_rulerWidget->zp_topFontSize();
}
//====================================================
int ZPlotter::zp_bottomMarkFontSize()
{
    if(!zv_rulerWidget)
    {
        return -1;
    }
    return zv_rulerWidget->zp_bottomFontSize();
}
//====================================================
bool ZPlotter::zp_setLeftMarkFormat(char format)
{
    if(!zv_rulerWidget)
    {
        return false;
    }
    return zv_rulerWidget->zp_setLeftMarkFormat(format);
}
//====================================================
bool ZPlotter::zp_setRightMarkFormat(char format)
{
    if(!zv_rulerWidget)
    {
        return false;
    }
    return zv_rulerWidget->zp_setRightMarkFormat(format);
}
//====================================================
bool ZPlotter::zp_setTopMarkFormat(char format)
{
    if(!zv_rulerWidget)
    {
        return false;
    }
    return zv_rulerWidget->zp_setTopMarkFormat(format);
}
//====================================================
bool ZPlotter::zp_setBottomMarkFormat(char format)
{
    if(!zv_rulerWidget)
    {
        return false;
    }
    return zv_rulerWidget->zp_setBottomMarkFormat(format);
}
//====================================================
bool ZPlotter::zp_setLeftMarkPrecision(int precision)
{
    if(!zv_rulerWidget)
    {
        return false;
    }
    return zv_rulerWidget->zp_setLeftMarkPrecision(precision);
}
//====================================================
bool ZPlotter::zp_setRightMarkPrecision(int precision)
{
    if(!zv_rulerWidget)
    {
        return false;
    }
    return zv_rulerWidget->zp_setRightMarkPrecision(precision);
}
//====================================================
bool ZPlotter::zp_setTopMarkPrecision(int precision)
{
    if(!zv_rulerWidget)
    {
        return false;
    }
    return zv_rulerWidget->zp_setTopMarkPrecision(precision);
}
//====================================================
bool ZPlotter::zp_setBottomMarkPrecision(int precision)
{
    if(!zv_rulerWidget)
    {
        return false;
    }
    return zv_rulerWidget->zp_setBottomMarkPrecision(precision);
}
//====================================================
void ZPlotter::zp_setHorizontalMarkRecalcFactors(const QString &label, qreal K0, qreal K1, qreal K2)
{
    if(!zv_rulerWidget)
    {
        return;
    }
    zv_rulerWidget->zp_setHorizontalMarkRecalcFactors(label, K0, K1, K2);
}
//====================================================
void ZPlotter::zp_setVerticalMarkRecalcFactors(const QString &label, qreal K0, qreal K1, qreal K2)
{
    if(!zv_rulerWidget)
    {
        return;
    }
    zv_rulerWidget->zp_setVerticalMarkRecalcFactors(label, K0, K1, K2);
}
//====================================================
void ZPlotter::zp_setLeftMarkRecalcMetrix(const QString& label, bool labelVisibility,qreal K0, qreal K1, qreal K2)
{
    if(!zv_rulerWidget)
    {
        return;
    }
    zv_rulerWidget->zp_setLeftMarkRecalcMetrix(label, labelVisibility, K0, K1, K2);
}
//====================================================
void ZPlotter::zp_setRightMarkRecalcMetrix(const QString& label, bool labelVisibility,qreal K0, qreal K1, qreal K2)
{
    if(!zv_rulerWidget)
    {
        return;
    }
    zv_rulerWidget->zp_setRightMarkRecalcMetrix(label, labelVisibility, K0, K1, K2);
}
//====================================================
void ZPlotter::zp_setTopMarkRecalcMetrix(const QString& label, bool labelVisibility,qreal K0, qreal K1, qreal K2)
{
    if(!zv_rulerWidget)
    {
        return;
    }
    zv_rulerWidget->zp_setTopMarkRecalcMetrix(label, labelVisibility, K0, K1, K2);
}
//====================================================
void ZPlotter::zp_setBottomMarkRecalcMetrix(const QString& label, bool labelVisibility,qreal K0, qreal K1, qreal K2)
{
    if(!zv_rulerWidget)
    {
        return;
    }
    zv_rulerWidget->zp_setBottomMarkRecalcMetrix(label, labelVisibility, K0, K1, K2);
}
//====================================================
void ZPlotter::zp_setLeftMarkRecalcFlag(bool flag)
{
    if(!zv_rulerWidget)
    {
        return;
    }
    zv_rulerWidget->zp_setLeftMarkRecalcFlag(flag);
}
//====================================================
void ZPlotter::zp_setRightMarkRecalcFlag(bool flag)
{
    if(!zv_rulerWidget)
    {
        return;
    }
    zv_rulerWidget->zp_setRightMarkRecalcFlag(flag);
}
//====================================================
void ZPlotter::zp_setTopMarkRecalcFlag(bool flag)
{
    if(!zv_rulerWidget)
    {
        return;
    }
    zv_rulerWidget->zp_setTopMarkRecalcFlag(flag);
}
//====================================================
void ZPlotter::zp_setBottomMarkRecalcFlag(bool flag)
{
    if(!zv_rulerWidget)
    {
        return;
    }
    zv_rulerWidget->zp_setBottomMarkRecalcFlag(flag);
}
//====================================================
void ZPlotter::zp_setTopRuleLabel(const QString &label)
{
    if(!zv_rulerWidget)
    {
        return;
    }
    zv_rulerWidget->zp_setTopRuleLabel(label);
}
//====================================================
void ZPlotter::zp_setBottomRuleLabel(const QString &label)
{
    if(!zv_rulerWidget)
    {
        return;
    }
    zv_rulerWidget->zp_setBottomRuleLabel(label);
}
//====================================================
void ZPlotter::zp_setLeftRuleLabel(const QString &label)
{
    if(!zv_rulerWidget)
    {
        return;
    }
    zv_rulerWidget->zp_setLeftRuleLabel(label);
}
//====================================================
void ZPlotter::zp_setRightRuleLabel(const QString &label)
{
    if(!zv_rulerWidget)
    {
        return;
    }
    zv_rulerWidget->zp_setRightRuleLabel(label);
}
//====================================================
void ZPlotter::zp_clearItemsForType(int type)
{
    QList<QGraphicsItem*> itemList = zv_plotScene->items();
    for(int i = 0; i < itemList.count(); i++)
    {
        if(itemList.value(i)->type() == type)
        {
            //itemList.value(i)->setVisible(false);

            zv_plotScene->zp_removeItem(itemList.value(i));
        }
    }

    //zv_plotView->update();
}
//====================================================
void ZPlotter::zp_clearItemsExeptType(int type)
{
    QList<QGraphicsItem*> itemList = zv_plotScene->items();
    for(int i = 0; i < itemList.count(); i++)
    {
        if(itemList.value(i)->type() == type)
        {
            continue;
        }
        zv_plotScene->zp_removeItem(itemList.value(i));
    }

    //zv_plotView->update();
}
//====================================================
void ZPlotter::zp_addItem(QGraphicsItem * item)
{
    if(item == 0)
    {
        return;
    }

    zv_plotScene->zp_addItem(item);
    QRectF itemBoundingSceneRect = zv_plotScene->itemsBoundingRect();

    if(zv_autoDefineVerticalAbsMax)
    {
        zp_setVerticalAbsMax(itemBoundingSceneRect.top());
    }
}
//====================================================
void ZPlotter::zp_removeItem(QGraphicsItem * item)
{
//    if(!item)
//    {
//        return;
//    }

    //item->setVisible(false);
    zv_plotScene->zp_removeItem(item);
    zv_plotView->update();
}
//====================================================
void ZPlotter::zp_removeItemsForType(int type)
{
    QList<QGraphicsItem*> itemList = zp_itemListForType(type);
    for(int i = itemList.count() - 1; i >= 0; i--)
    {
        zp_removeItem(itemList.at(i));
    }
}
//====================================================
QList<QGraphicsItem*> ZPlotter::zp_itemListForType(int type) const
{
    QList<QGraphicsItem*> itemList = zv_plotScene->items();
    for(int i = itemList.count()-1; i >= 0; i--)
    {
        if(itemList.value(i)->type() != type)
        {
            itemList.removeAt(i);
        }
    }

    return itemList;
}
//====================================================
int ZPlotter::zp_itemCount() const
{
    if(zv_plotScene == 0)
    {
        return 0;
    }

    return zv_plotScene->items().count();
}
//====================================================
void ZPlotter::zp_appendButtonsToDashboard(QList<QAction*>& actionList,
                                           ZHorizontalDashBoard::AuxWidgetPositon position,
                                           Qt::AlignmentFlag alignment,
                                           int extraSeparatorSpacing)
{
    if(!zv_dashBoard)
    {
        return;
    }
    zv_dashBoard->zp_appendButtonsToDashboard(actionList,
                                              position,
                                              alignment,
                                              extraSeparatorSpacing);
}
//====================================================
void ZPlotter::zp_appendWidgetToDashboard(QWidget* widget,
                                          ZHorizontalDashBoard::AuxWidgetPositon position,
                                          Qt::AlignmentFlag alignment,
                                          int extraSeparatorSpacing)
{
    if(!zv_dashBoard)
    {
        return;
    }
    zv_dashBoard->zp_appendWidgetToDashboard(widget,
                                             position,
                                             alignment,
                                             extraSeparatorSpacing);
}
//====================================================
void ZPlotter::zp_setVerticalAbsMax(qreal max)
{
    if(zv_verticalAbsMax - qAbs(max) == 0.0)
    {
        return;
    }
    zv_verticalAbsMax = qAbs(max);

    if(zh_recalcVerticalDistortionFactors(zv_dashBoard->zp_distortionValue()))
    {
        zh_recalcRulesAndItemCoordinates();
    }
}
//====================================================
qreal ZPlotter::zp_verticalAbsMax() const
{
    return zv_verticalAbsMax;
}
//====================================================
void ZPlotter::zp_setAutoDefineVerticalAbsMax(bool flag)
{
    zv_autoDefineVerticalAbsMax = flag;
}
//====================================================
void ZPlotter::zp_verticalDistortionFactors(qreal& distortionFactor, qreal& distortionCorrectionFactor)
{
    distortionFactor = zv_verticalDistortionFactor;
    distortionCorrectionFactor = zv_verticalDistortionCorrectionFactor;
}
//====================================================
bool ZPlotter::zp_isPlotScaled()
{
    return zv_horizontalScrollBar->isVisible()
            || zv_verticalScrollBar->isVisible();
}
//====================================================
void ZPlotter::zp_updatePlot()
{
    zv_plotView->update();
}
//====================================================
QBrush ZPlotter::zp_backgroundBrush()
{
    return zv_plotView->backgroundBrush();
}
//====================================================
void ZPlotter::zp_setContextMenu(QList<QAction*>& actionList)
{
    zv_plotView->addActions(actionList);
    zv_plotView->setContextMenuPolicy(Qt::DefaultContextMenu);
}
//====================================================
void ZPlotter::zp_setEnergyCalibration(QList<double> energyCalibrationFactorList)
{
    zv_energyCalibrationFactorList = energyCalibrationFactorList;
}
//====================================================
QRectF ZPlotter::zp_viewportSceneRect() const
{
    QRectF rect;
    if(!zv_plotView->zp_viewPortSceneRect(rect))
    {
        return QRectF();
    }

    return rect;
}
//====================================================
QSize ZPlotter::zp_viewportPixelSize() const
{
    return zv_plotView->viewport()->size();
}
//====================================================
void ZPlotter::zp_setLeftRuleVisible(bool visible)
{
    if(!zv_rulerWidget)
    {
        return;
    }
    zv_rulerWidget->zp_setLeftRuleVisibility(visible);
}
//====================================================
void ZPlotter::zp_setRightRuleVisible(bool visible)
{
    if(!zv_rulerWidget)
    {
        return;
    }
    zv_rulerWidget->zp_setRightRuleVisibility(visible);
}
//====================================================
void ZPlotter::zp_setTopRuleVisible(bool visible)
{
    if(!zv_rulerWidget)
    {
        return;
    }
    zv_rulerWidget->zp_setTopRuleVisibility(visible);
}
//====================================================
void ZPlotter::zp_setBottomRuleVisible(bool visible)
{
    if(!zv_rulerWidget)
    {
        return;
    }
    zv_rulerWidget->zp_setBottomRuleVisibility(visible);
}
//====================================================
void ZPlotter::zp_setTopRuleLabelVisible(bool visible)
{
    if(!zv_rulerWidget)
    {
        return;
    }
    zv_rulerWidget->zp_setTopRuleLabelVisible(visible);
}
//====================================================
void ZPlotter::zp_setBottomRuleLabelVisible(bool visible)
{
    if(!zv_rulerWidget)
    {
        return;
    }
    zv_rulerWidget->zp_setBottomRuleLabelVisible(visible);
}
//====================================================
void ZPlotter::zp_setLeftRuleLabelVisible(bool visible)
{
    if(!zv_rulerWidget)
    {
        return;
    }
    zv_rulerWidget->zp_setLeftRuleLabelVisible(visible);
}
//====================================================
void ZPlotter::zp_setRightRuleLabelVisible(bool visible)
{
    if(!zv_rulerWidget)
    {
        return;
    }
    zv_rulerWidget->zp_setRightRuleLabelVisible(visible);
}
//====================================================
void ZPlotter::zp_setDistortionSliderVisible(bool visible)
{
    if(!zv_dashBoard)
    {
        return;
    }
    zv_dashBoard->zp_setDistortionSliderVisible(visible);
}
//====================================================
void ZPlotter::zp_currentVisibleSceneRect(QRectF& sceneRect)
{
    zv_plotView->zp_viewPortSceneRect(sceneRect);
}
//====================================================
void ZPlotter::zp_fitInBoundingRect()
{
    if(!zv_plotView || !zv_plotScene)
    {
        return;
    }

    // block signals for scroll bar flicker prevention
    zv_verticalScrollBar->blockSignals(true);
    zv_horizontalScrollBar->blockSignals(true);

    QRectF itemBoundingRect = zv_plotScene->itemsBoundingRect();
    zv_plotScene->setSceneRect(itemBoundingRect);
    QRectF rectToFit;
    if(itemBoundingRect.isValid())
    {
        rectToFit = itemBoundingRect;
    }
    else
    {
        rectToFit = zv_plotScene->sceneRect();
    }
    zv_plotView->zp_fitInView(rectToFit);

    // deblock signals
    zv_verticalScrollBar->blockSignals(false);
    zv_horizontalScrollBar->blockSignals(false);

    zh_updateScrollBarsVisible();
    //zv_plotView->ensureVisible(rectToFit);
}
//====================================================
QRectF ZPlotter::zp_boundingRect() const
{
    return zv_plotScene->itemsBoundingRect();
}
//====================================================
void ZPlotter::resizeEvent(QResizeEvent* event)
{
    if(zv_plotView && zv_plotScene && zv_mouseButtonDown)
    {
        zv_userResizesWidget = true;
        zv_plotView->zp_setScaleViewWhenResizeFlag(true);
    }

    QWidget::resizeEvent(event);
}
//====================================================
bool ZPlotter::eventFilter(QObject *obj, QEvent *event)
{
    if(event->type() == QEvent::MouseButtonPress
            || event->type() == QEvent::NonClientAreaMouseButtonPress)
    {
        zv_mouseButtonDown = true;
        bool res = QObject::eventFilter(obj, event);
        return res;
    }
    else if(event->type() == QEvent::MouseButtonRelease
            || event->type() == QEvent::NonClientAreaMouseButtonRelease)
    {
        zv_mouseButtonDown = false;
        zv_userResizesWidget = false;
        zv_plotView->zp_setScaleViewWhenResizeFlag(false);
        zh_updateScrollBarsVisible();
        bool res = QObject::eventFilter(obj, event);
        return res;
    }


    bool res = QObject::eventFilter(obj, event);
    return res;
}
//====================================================
void ZPlotter::zh_verticalDistortionChanged(int distortionValue)
{
    if(zh_recalcVerticalDistortionFactors(static_cast<qreal>(distortionValue)))
    {
        zh_recalcRulesAndItemCoordinates();
        zv_plotView->zp_update();
    }
}
//====================================================
void ZPlotter::zh_scrollBarVisible(Qt::Orientation orientation, bool& visible)
{
    if(orientation == Qt::Vertical)
    {
        visible = zv_verticalScrollBar->isVisible();
    }
    else
    {
        visible = zv_horizontalScrollBar->isVisible();
    }
}
//====================================================
void ZPlotter::zh_onMousePress(QPointF mouseScenePos)
{

}
//====================================================
void ZPlotter::zh_mouseScenePositionChanged(QPointF scenePos) const
{
    if(!zv_rulerWidget)
    {
        return;
    }


    QString text = tr("Channel: %1").arg(QString::number(scenePos.x(), 'f', 0));

    QString energyString;
    if(!(zv_energyCalibrationFactorList.value(2, 0.0) == 0.0 &&
         (zv_energyCalibrationFactorList.value(1, 0.0) == 1.0 ||
          zv_energyCalibrationFactorList.value(1, 0.0) == 0.0) &&
         zv_energyCalibrationFactorList.value(0, 0.0) == 0.0))
    {
        double XValue = (pow(scenePos.x(), 2.0) *zv_energyCalibrationFactorList.value(2)) +
                (scenePos.x() *zv_energyCalibrationFactorList.value(1)) +
                zv_energyCalibrationFactorList.value(0);
        text += tr(" Energy: %1").arg(QString::number(XValue, 'f', 2));
    }

    double YValue = zv_rulersAndGreedManager->zp_recalcSceneVerticalPos(scenePos.y());
    text += tr(" Intensity: %1").arg(QString::number(YValue, 'f', 0));
    zv_rulerWidget->zp_setInfoLabelText(text);

}
//====================================================
void ZPlotter::zh_mouseLeaved() const
{
    zv_rulerWidget->zp_setInfoLabelText(QString());
}
//====================================================
void ZPlotter::zh_createComponents()
{
    zv_verticalScrollBar = new QScrollBar(Qt::Vertical, this);
    zv_horizontalScrollBar = new QScrollBar(Qt::Horizontal, this);
    zv_plotScene = new ZPlotGraphicsScene(this);
    zv_plotView = new ZPlotGraphicsView(0);
    zv_rulerWidget = new ZRulerWidget(0);
    zv_rulersAndGreedManager = new ZRulersAndGridManager(this);
    zv_dashBoard = new ZHorizontalDashBoard(this);
    zv_rulerWidget->zp_setPlotView(zv_plotView);

    // settings
    zv_plotView->setFrameStyle(QFrame::NoFrame);
    zv_plotView->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    zv_plotView->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    // set outer scrollbars
    zv_plotView->setVerticalScrollBar(zv_verticalScrollBar);
    zv_plotView->setHorizontalScrollBar(zv_horizontalScrollBar);
    zv_plotView->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    zv_verticalScrollBar->setVisible(false);
    zv_horizontalScrollBar->setVisible(false);

    zv_dashBoard->setMaximumHeight(zv_horizontalScrollBar->height());
    zv_dashBoard->setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Maximum);

    // Layout
    QVBoxLayout* mainLayout = new QVBoxLayout;
    mainLayout->setMargin(0);
    mainLayout->setSpacing(0);
    setLayout(mainLayout);

    QHBoxLayout* mainScrollLayout = new QHBoxLayout;
    mainScrollLayout->setMargin(0);
    mainScrollLayout->setSpacing(0);
    mainLayout->addLayout(mainScrollLayout);

    //QVBoxLayout* mainViewAndRulesLayout = new QVBoxLayout(this);
    mainScrollLayout->addWidget(zv_rulerWidget);
    mainScrollLayout->addWidget(zv_verticalScrollBar);

    QHBoxLayout* bottomScrollLayout = new QHBoxLayout;
    bottomScrollLayout->setMargin(0);
    bottomScrollLayout->setSpacing(0);
    mainLayout->addLayout(bottomScrollLayout);
    bottomScrollLayout->addWidget(zv_horizontalScrollBar, 9999999);
    bottomScrollLayout->addSpacing(10);
    bottomScrollLayout->addStretch(0);
    bottomScrollLayout->addWidget(zv_dashBoard);
    bottomScrollLayout->addSpacing(5);

}
//====================================================
void ZPlotter::zh_createConnections()
{
    // scroll bar visible control connection
    zh_connectScrollBars();
    zv_rulersAndGreedManager->zp_setRulerWidget(zv_rulerWidget);
    zv_plotView->setScene(zv_plotScene);
    zv_plotView->zp_connectToRulersAndGridManager(zv_rulersAndGreedManager);
    //zv_plotView->fitInView(QRectF(QPointF(0.0, -100.0), QPointF(2048, -100)));
    zv_dashBoard->zp_setPlotGraphicsView(zv_plotView);
    zv_dashBoard->zp_setPlotGraphicsScene(zv_plotScene);
    connect(zv_dashBoard, &ZHorizontalDashBoard::zg_distortionFactorChanged,
            this, &ZPlotter::zh_verticalDistortionChanged);
    connect(zv_plotView, &ZPlotGraphicsView::zg_inquiryForScrollBarVisible,
            this, &ZPlotter::zh_scrollBarVisible);
    connect(zv_plotView, &ZPlotGraphicsView::zg_cursorAreaImage,
            this, &ZPlotter::zg_cursorAreaImage);
    connect(zv_plotView, &ZPlotGraphicsView::zg_mousePressedAt,
            this, &ZPlotter::zg_mousePressedAt);
    connect(zv_plotView, &ZPlotGraphicsView::zg_viewportRectChanged,
            this, &ZPlotter::zg_viewportRectChanged);

    connect(zv_plotView, &ZPlotGraphicsView::zg_mouseScenePositionChanged,
            this, &ZPlotter::zh_mouseScenePositionChanged);
    connect(zv_plotView, &ZPlotGraphicsView::zg_mouseLeaved,
            this, &ZPlotter::zh_mouseLeaved);
    connect(zv_plotView, &ZPlotGraphicsView::zg_rulerToolChanged,
            this, &ZPlotter::zg_rulerToolChanged);


}
//====================================================
void ZPlotter::zh_connectScrollBars()
{
    connect(zv_horizontalScrollBar, &QScrollBar::rangeChanged,
            this, &ZPlotter::zh_scrollBarVisibleControl);
    connect(zv_verticalScrollBar, &QScrollBar::rangeChanged,
            this, &ZPlotter::zh_scrollBarVisibleControl);

    connect(zv_horizontalScrollBar, &QScrollBar::valueChanged,
            this, &ZPlotter::zh_notifySceneRect);
    connect(zv_verticalScrollBar, &QScrollBar::valueChanged,
            this, &ZPlotter::zh_notifySceneRect);

}
//====================================================
void ZPlotter::zh_updateScrollBarsVisible()
{
    zv_horizontalScrollBar->setHidden(zv_horizontalScrollBar->maximum() == 0
                                      && zv_horizontalScrollBar->minimum() == 0);

    zv_verticalScrollBar->setHidden(zv_verticalScrollBar->maximum() == 0
                                    && zv_verticalScrollBar->minimum() == 0);
}
//====================================================
void ZPlotter::zh_scrollBarVisibleControl(int min , int max)
{
    QScrollBar* scrollBar = qobject_cast<QScrollBar*>(sender());
    if(!scrollBar || zv_userResizesWidget)
    {
        return;
    }
    scrollBar->setHidden(min == 0 && max == 0);
}
//====================================================
void ZPlotter::zh_notifySceneRect(int value)
{
    QRectF viewportRect;
    zv_plotView->zp_viewPortSceneRect(viewportRect);
    emit zg_viewportRectChanged(viewportRect);
}
//====================================================
bool ZPlotter::zh_recalcVerticalDistortionFactors(qreal distortionValue)
{
    if(zv_verticalAbsMax == 0.0)
    {
        return false;
    }

    if(distortionValue < 0)
    {
        distortionValue = 0;
    }
    else if(distortionValue > 15)
    {
        distortionValue = 15;
    }
    zv_verticalDistortionFactor = 1.0 - static_cast<qreal>(distortionValue) / 20.0;
    zv_verticalDistortionCorrectionFactor = zv_verticalAbsMax / pow(zv_verticalAbsMax, zv_verticalDistortionFactor);

    return true;
}
//====================================================
void ZPlotter::zh_recalcRulesAndItemCoordinates()
{
    if(zv_rulersAndGreedManager != 0)
    {
        zv_rulersAndGreedManager->zp_recalcRulesAndGrid();
        zv_rulerWidget->update();
    }

    QList<QGraphicsItem*> itemList = zv_plotScene->items();
    for(int i = 0; i < itemList.count(); i++)
    {
        if(itemList.value(i)->type() == SpectrumItemType)
        {
            ZSpectrumGraphicsItem* spectrumItem = qgraphicsitem_cast<ZSpectrumGraphicsItem*>(itemList.value(i));
            if(!spectrumItem)
            {
                continue;
            }

            spectrumItem->zp_setDistortion(zv_verticalDistortionFactor, zv_verticalDistortionCorrectionFactor);
        }
        else if(itemList.value(i)->type() == EnergyLineItemType)
        {
            ZEnergyLineGraphicsItem* energyLineItem = qgraphicsitem_cast<ZEnergyLineGraphicsItem*>(itemList.value(i));
            if(!energyLineItem)
            {
                continue;
            }

            energyLineItem->zp_updateItem();
        }
    }
}
//====================================================
