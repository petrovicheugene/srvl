//=========================================================
#ifndef ZRULERWIDGET_H
#define ZRULERWIDGET_H
//=========================================================
#include <QWidget>
#include <QSet>
#include "ZRulersAndGridManager.h"
//=========================================================

//=========================================================
class ZRulerProperty
{
public:
    ZRulerProperty()
    {
         scratchLineSize = 3;
         markLineSize = 0;
         precision = 6;
         format = 'g';
         fontSize = 10;
         visibility = true;
         scratchRect = QRect();
         markRect = QRect();
         ruleLabelRect = QRect();
         markOffsetFromScratch = 3;
         recalcMarkLabel = true;
         ruleLabelVisibility = true;
         ruleLabel = QString();

         K0 = 0.0;
         K1 = 1.0;
         K2 = 0.0;
    }

    int rulerSize() const
    {
        if(!visibility || scratchLineSize + markLineSize < 1)
        {
            return 1;
        }
        return scratchLineSize + markLineSize;
    }

    int scratchLineSize;
    int markLineSize;
    int precision;
    char format;
    int fontSize;
    bool visibility;
    QRect scratchRect;
    QRect markRect;
    QRect ruleLabelRect;
    int markOffsetFromScratch;
    bool recalcMarkLabel;
    bool ruleLabelVisibility;
    QString ruleLabel;

    // recalc marks factors
    qreal K0; // free member
    qreal K1;
    qreal K2; // square member factor
};
//=========================================================
class ZRulerWidget : public QWidget
{
    Q_OBJECT
public:
    explicit ZRulerWidget(QWidget *parent = 0);

    void zp_setPlotView(QWidget* plot);

    bool zp_setLineWidth(int);
    void zp_setLineColor(QColor);

    void zp_setLeftScratchLineSize(int);
    void zp_setRightScratchLineSize(int);
    void zp_setTopScratchLineSize(int);
    void zp_setBottomScratchLineSize(int);
    void zp_setScratchLineSize(int);

    bool zp_setLeftFontSize(int size);
    bool zp_setRightFontSize(int size);
    bool zp_setTopFontSize(int size);
    bool zp_setBottomFontSize(int size);

    int zp_leftFontSize();
    int zp_rightFontSize();
    int zp_topFontSize();
    int zp_bottomFontSize();

    bool zp_setLeftMarkFormat(char format);
    bool zp_setRightMarkFormat(char format);
    bool zp_setTopMarkFormat(char format);
    bool zp_setBottomMarkFormat(char format);

    bool zp_setLeftMarkPrecision(int precision);
    bool zp_setRightMarkPrecision(int precision);
    bool zp_setTopMarkPrecision(int precision);
    bool zp_setBottomMarkPrecision(int precision);

    void zp_setLeftMarkRecalcMetrix(const QString&, bool labelVisibility,qreal K0, qreal K1, qreal K2);
    void zp_setRightMarkRecalcMetrix(const QString&, bool labelVisibility,qreal K0, qreal K1, qreal K2);
    void zp_setTopMarkRecalcMetrix(const QString&, bool labelVisibility,qreal K0, qreal K1, qreal K2);
    void zp_setBottomMarkRecalcMetrix(const QString&, bool labelVisibility,qreal K0, qreal K1, qreal K2);

    void zp_setLeftMarkRecalcFlag(bool flag);
    void zp_setRightMarkRecalcFlag(bool flag);
    void zp_setTopMarkRecalcFlag(bool flag);
    void zp_setBottomMarkRecalcFlag(bool flag);

    void zp_setHorizontalMarkRecalcFactors(const QString &label, qreal K0, qreal K1, qreal K2);
    void zp_setVerticalMarkRecalcFactors(const QString &label, qreal K0, qreal K1, qreal K2);

    void zp_setTopRuleVisibility(bool visibility);
    void zp_setBottomRuleVisibility(bool visibility);
    void zp_setLeftRuleVisibility(bool visibility);
    void zp_setRightRuleVisibility(bool visibility);

    void zp_setTopRuleLabel(const QString &label);
    void zp_setBottomRuleLabel(const QString &label);
    void zp_setLeftRuleLabel(const QString &label);
    void zp_setRightRuleLabel(const QString &label);

    void zp_setTopRuleLabelVisible(bool visible);
    void zp_setBottomRuleLabelVisible(bool visible);
    void zp_setLeftRuleLabelVisible(bool visible);
    void zp_setRightRuleLabelVisible(bool visible);

    void zp_setHorizontalRulePointList(QList<RulePoint> *);
    void zp_setVerticalRulePointList(QList<RulePoint> *);

    int zp_maxMarkWidth();
    int zp_maxMarkHeight();

signals:

public slots:

protected:

    // VARS
    QWidget* zv_plot;
    ZRulerProperty zv_topRulerProperty;
    ZRulerProperty zv_bottomRulerProperty;
    ZRulerProperty zv_leftRulerProperty;
    ZRulerProperty zv_rightRulerProperty;

    QColor zv_lineColor;
    int zv_lineWidth;
    bool zv_dontRepaint;

    int zv_maxMarkHeight;
    int zv_maxMarkWidth;

    ZRulersAndGridManager* zv_rulersAndGreedManager;

    QList<RulePoint> * zv_XRuleList;
    QList<RulePoint> * zv_YRuleList;
    QSet<char> zv_formatSet;

    // FUNCS
    void resizeEvent(QResizeEvent* event);
    void paintEvent(QPaintEvent* event);

    void zh_paintLeftRule(QPainter* painter);
    void zh_paintRightRule(QPainter* painter);
    void zh_paintTopRule(QPainter* painter);
    void zh_paintBottomRule(QPainter* painter);

    void zh_recalcRuleSizes();
};
//=========================================================
#endif // ZRULERWIDGET_H
