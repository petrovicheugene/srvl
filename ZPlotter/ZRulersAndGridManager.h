//========================================================
#ifndef ZRULERSANDGREEDMANAGER_H
#define ZRULERSANDGREEDMANAGER_H
//========================================================
#include <QObject>
#include <QPair>
#include <QMap>
#include <QRect>
#include <QRectF>
#include <QList>
//========================================================
class ZPlotGraphicsView;
class ZRulerWidget;
class ZPlotter;
//========================================================
class RulePoint
{
public:
    enum MarkType{MT_SCRATCH, MT_MARK, MT_EMPTY_MARK};

    RulePoint()
    {
        globalPos = 0;
        scenePos = 0;
        markValue = 0;
        markType = MT_SCRATCH;
    }

    RulePoint(qreal glPos, qreal scPos, RulePoint::MarkType type)
    {
        globalPos = glPos;
        scenePos = scPos;
        markValue = scPos;
        markType = type;
    }


    RulePoint(qreal glPos, qreal scPos,  qreal markVal, RulePoint::MarkType type)
    {
        globalPos = glPos;
        scenePos = scPos;
        markValue = markVal;
        markType = type;
    }

    // VARS
    MarkType markType;
    qreal globalPos;
    qreal scenePos;
    qreal markValue;
};

//========================================================
class ZRulersAndGridManager : public QObject
{
    Q_OBJECT
public:
    explicit ZRulersAndGridManager(ZPlotter *parent);

    void zp_setPlotGraphicsView(ZPlotGraphicsView* plotView);
    void zp_setRulerWidget(ZRulerWidget* rulerWidget);

    bool zp_setMinimalVerticalScaleInterval(qreal interval);
    qreal zp_minimalVerticalScaleInterval();
    bool zp_setMinimalHorizontalScaleInterval(qreal interval);
    qreal zp_minimalHorizontalScaleInterval();

    void zp_recalcRulesAndGrid();

signals:

public slots:


private:

    // VARS
    ZPlotGraphicsView* zv_plotView;
    ZRulerWidget* zv_rulerWidget;
    ZPlotter* zv_plotter;

    QList<RulePoint> zv_XRulePointList;
    QList<RulePoint> zv_YRulePointList;


    qreal zv_minimalVerticalScaleInterval;
    qreal zv_minimalHorizontalScaleInterval;
    int zv_minimalScratchVpInterval;


    // for recalc only
    int zv_minVpMarkInterval;
    qreal zv_K;
    qreal zv_A;
    qreal zv_displayedTopYValue;
    qreal zv_displayedBottomYValue;
    qreal zv_distortionFactor;
    qreal zv_distortionCorrectionFactor;


    QRect zv_viewportGlobalRect;
    QRectF zv_viewportSceneRect;

    QRect zv_globalRectForPaint; // +1 pix more than zv_viewportGlobalRect
    QRectF zv_sceneRectForPaint; // +1 pix more than zv_viewportSceneRect

    QPoint zv_globalZeroPoint;
    QList<qreal> zv_markIntervalList;

    // FUNCS
    void zh_recalcBottomRule();
    void zh_recalcLeftRule();
    void zh_recalcDistortedLeftRule(qreal zv_distortionFactor,
                                    qreal zv_distortionCorrectionFactor);

    void zh_recalcRectsForPaint();
    bool zh_divideDistortedVerticalInterval(qreal p, qreal currentMarkValue,
                           qreal currentMarkVpPos, qreal prevMarkValue);
};
//========================================================
#endif // ZRULERSANDGREEDMANAGER_H
