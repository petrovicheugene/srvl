//==========================================================
#ifndef ZROUNDPROGRESSBAR_H
#define ZROUNDPROGRESSBAR_H
//==========================================================
#include <QWidget>
#include <QColor>
#include <QMap>
#include <QFont>
#include <QPainterPath>
//==========================================================
class QPainter;
//==========================================================
class ZRoundProgressBar : public QWidget
{
    Q_OBJECT
public:
    explicit ZRoundProgressBar(QWidget *parent = 0);

    enum ProgressBarStyle {PS_LINE, PS_DONUT};
    enum GrowingDirection {GD_CCW, GD_CW};

    void zp_setStyle(ProgressBarStyle);

    // range
    void zp_setRange(double dMinimum, double dMaximum);
    void zp_setRange(int nMinimum, int nMaximum);
    void zp_setMinimum(double dMinimum);
    void zp_setMinimum(int nMinimum);
    void zp_setMaximum(double dMaximum);
    void zp_setMaximum(int nMaximum);

    // colors
    void zp_setBackgroundColor(QColor);
    void zp_setRibbonOutlineColor(QColor);
    void zp_setRibbonColor(QColor);
    void zp_setRibbonPathOutlineColor(QColor);
    void zp_setRibbonPathColor(QColor);

    void zp_setInnerEllipseColor(QColor);
    void zp_setTextColor( QColor);

    // other options
    void zp_setFont(QFont);
    void zp_setRibbonOutlineWidth(uint);
    void zp_setRibbonWidth(uint);
    void zp_setRibbonPathOutlineWidth(uint);
    void zp_setRibbonPathWidth(uint);

    void zp_setElliptical(bool);
    void zp_setStartAngleGrad(double);
    void zp_setDirections();

signals:

public slots:

    void zp_setValue(double);
    void zp_setValue(int);
    void zp_setPercent(double);

protected:

    void paintEvent(QPaintEvent *event);
    void resizeEvent(QResizeEvent* event);
    void zh_drawBackground(QPainter*);
    void zh_drawRibbonPath(QPainter*);
    void zh_drawRibbon(QPainter*);
    void zh_drawInner(QPainter*);
    void zh_drawText(QPainter*);

private:

    // VARS
    ProgressBarStyle zv_style;
    GrowingDirection zv_growingDirection;

    // painterpaths
    QPainterPath zv_ribbonPathPainterPath;
    QPainterPath zv_ribbonPainterPath;
    QPainterPath zv_innerEllipsePainterPath;

    QColor zv_backgroundColor;
    QColor zv_ribbonOutlineColor;
    QColor zv_ribbonColor;
    QColor zv_ribbonPathOutlineColor;
    QColor zv_ribbonPathColor;
    QColor zv_innerEllipseColor;
    QColor zv_textColor;
    QFont zv_font;

    uint zv_ribbonOutlineWidth;
    uint zv_ribbonWidth;
    uint zv_ribbonPathOutlineWidth;
    uint zv_ribbonPathWidth;

    double zv_minimum;
    double zv_maximum;

    // FUNCS
    void zh_definePainterPaths();

};
//==========================================================
#endif // ZROUNDPROGRESSBAR_H
