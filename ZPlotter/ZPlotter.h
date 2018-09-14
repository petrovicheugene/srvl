//====================================================
#ifndef ZPLOTTER_H
#define ZPLOTTER_H
//====================================================
#include <QWidget>
#include <QGraphicsItem>
#include "ZHorizontalDashBoard.h"
//====================================================
class ZMarkLine;
class ZVerticalMarkLine;
class ZScratchLine;
class ZVerticalScratchLine;
class ZPlotGraphicsView;
class ZPlotGraphicsScene;
class QScrollBar;
class ZRulersAndGridManager;
class ZRulerWidget;
//====================================================
class ZPlotter : public QWidget
{
    Q_OBJECT
public:
    explicit ZPlotter(QWidget *parent = 0);

    // ZPlotGraphicsScene* zp_plotScene();

    bool zp_setMinimalVerticalScaleInterval(qreal);
    qreal zp_minimalVerticalScaleInterval();
    bool zp_setMinimalHorizontalScaleInterval(qreal);
    qreal zp_minimalHorizontalScaleInterval();

    // rule control
    bool zp_setLeftMarkFontSize(int size) ;
    bool zp_setRightMarkFontSize(int size);
    bool zp_setTopMarkFontSize(int size);
    bool zp_setBottomMarkFontSize(int size);

    int zp_leftMarkFontSize();
    int zp_rightMarkFontSize();
    int zp_topMarkFontSize();
    int zp_bottomMarkFontSize();

    bool zp_setLeftMarkFormat(char);
    bool zp_setRightMarkFormat(char);
    bool zp_setTopMarkFormat(char);
    bool zp_setBottomMarkFormat(char);

    bool zp_setLeftMarkPrecision(int);
    bool zp_setRightMarkPrecision(int);
    bool zp_setTopMarkPrecision(int);
    bool zp_setBottomMarkPrecision(int);

    // rule control
    void zp_setHorizontalMarkRecalcFactors(const QString &label, qreal K0, qreal K1, qreal K2);
    void zp_setVerticalMarkRecalcFactors(const QString &label, qreal K0, qreal K1, qreal K2);

    void zp_setLeftMarkRecalcMetrix(const QString&label, bool labelVisibility, qreal K0, qreal K1, qreal K2);
    void zp_setRightMarkRecalcMetrix(const QString&label, bool labelVisibility, qreal K0, qreal K1, qreal K2);
    void zp_setTopMarkRecalcMetrix(const QString&label, bool labelVisibility, qreal K0, qreal K1, qreal K2);
    void zp_setBottomMarkRecalcMetrix(const QString& label, bool labelVisibility,qreal K0, qreal K1, qreal K2);

    void zp_setLeftMarkRecalcFlag(bool flag);
    void zp_setRightMarkRecalcFlag(bool flag);
    void zp_setTopMarkRecalcFlag(bool flag);
    void zp_setBottomMarkRecalcFlag(bool flag);

    void zp_setTopRuleLabel(const QString &label);
    void zp_setBottomRuleLabel(const QString &label);
    void zp_setLeftRuleLabel(const QString &label);
    void zp_setRightRuleLabel(const QString &label);

    // ***
    void zp_clearItemsForType(int type);
    void zp_clearItemsExeptType(int type);
    void zp_addItem(QGraphicsItem * item);
    void zp_removeItem(QGraphicsItem * item);
    void zp_removeItemsForType(int type);
    QList<QGraphicsItem*> zp_itemListForType(int type) const;
    int zp_itemCount() const;

    void zp_appendButtonsToDashboard(QList<QAction*>&,
                                     ZHorizontalDashBoard::AuxWidgetPositon position,
                                     Qt::AlignmentFlag align,
                                     int extraSperatorSpacing = 0);
    void zp_appendWidgetToDashboard(QWidget* widget,
                                    ZHorizontalDashBoard::AuxWidgetPositon position,
                                    Qt::AlignmentFlag alignment,
                                    int extraSeparatorSpacing = 0);

    void zp_setVerticalAbsMax(qreal);
    qreal zp_verticalAbsMax() const;
    void zp_setAutoDefineVerticalAbsMax(bool);
    void zp_verticalDistortionFactors(qreal& distortionFactor, qreal& distortionCorrectionFactor);

    bool zp_isPlotScaled();
    void zp_updatePlot();

    QBrush zp_backgroundBrush();

    void zp_setContextMenu(QList<QAction *> &actionList);

signals:

    void zg_cursorAreaImage(QImage);
    void zg_mousePressedAt(QPointF mousePos) const;

public slots:

    void zp_setLeftRuleVisible(bool visible);
    void zp_setRightRuleVisible(bool visible);
    void zp_setTopRuleVisible(bool visible);
    void zp_setBottomRuleVisible(bool visible);

    void zp_setTopRuleLabelVisible(bool visible);
    void zp_setBottomRuleLabelVisible(bool visible);
    void zp_setLeftRuleLabelVisible(bool visible);
    void zp_setRightRuleLabelVisible(bool visible);

    void zp_setDistortionSliderVisible(bool visible);

    void zp_currentVisibleSceneRect(QRectF& sceneRect);
    void zp_fitInBoundingRect();
    QRectF zp_boundingRect() const;

protected:

    // FUNCS
    void resizeEvent(QResizeEvent* event);
    bool eventFilter(QObject *obj, QEvent *event);

private slots:

    void zh_verticalDistortionChanged(int);
    void zh_scrollBarVisible(Qt::Orientation, bool&);
    void zh_onMousePress(QPointF mouseScenePos);

private:

    // VARS
    ZRulersAndGridManager* zv_rulersAndGreedManager;

    ZRulerWidget* zv_rulerWidget;
    ZPlotGraphicsView* zv_plotView;
    ZPlotGraphicsScene* zv_plotScene;
    ZHorizontalDashBoard* zv_dashBoard;

    qreal zv_verticalAbsMax;
    bool zv_autoDefineVerticalAbsMax;
    qreal zv_verticalDistortionFactor;
    qreal zv_verticalDistortionCorrectionFactor;
    // recalc when max or log base are changed
    //
    //
    // zv_verticalDistortionCorrectionFactor =  max / max^zv_verticalDistortionFactor
    //
    //

    //  Y_log = Y^zv_verticalDistortionFactor * zv_verticalDistortionCorrectionFactor


    // scrollbar
    QScrollBar* zv_verticalScrollBar;
    QScrollBar* zv_horizontalScrollBar;
    bool zv_mouseButtonDown;
    bool zv_userResizesWidget;

    // FUNCS
    void zh_createComponents();
    void zh_createConnections();
    void zh_connectScrollBars();
    void zh_updateScrollBarsVisible();

    void zh_scrollBarVisibleControl(int min , int max);
    bool zh_recalcVerticalDistortionFactors(qreal distortionValue);
    void zh_recalcRulesAndItemCoordinates();

};
//====================================================
#endif // ZPLOTTER_H
