//==========================================================
#include "ZRoundProgressBar.h"
#include <QPainter>
#include <QResizeEvent>
#include <QPaintEvent>
//==========================================================
ZRoundProgressBar::ZRoundProgressBar(QWidget *parent)
    : QWidget(parent)
{
    zv_minimum = 0;
    zv_maximum = 100;

    zv_backgroundColor = QColor(Qt::transparent);
    zv_ribbonOutlineColor = ;
    zv_ribbonColor;
    zv_ribbonPathOutlineColor;
    zv_ribbonPathColor;
    zv_innerEllipseColor;
    zv_textColor;
    zv_font;

    zv_ribbonOutlineWidth;
    zv_ribbonWidth;
    zv_ribbonPathOutlineWidth;
    zv_ribbonPathWidth;
}
//==========================================================
void ZRoundProgressBar::zp_setStyle(ProgressBarStyle style)
{

}
//==========================================================
void ZRoundProgressBar::zp_setRange(double dMinimum, double dMaximum)
{

}
//==========================================================
void ZRoundProgressBar::zp_setRange(int nMinimum, int nMaximum)
{

}
//==========================================================
void ZRoundProgressBar::zp_setMinimum(double dMinimum)
{

}
//==========================================================
void ZRoundProgressBar::zp_setMinimum(int nMinimum)
{

}
//==========================================================
void ZRoundProgressBar::zp_setMaximum(double dMaximum)
{

}
//==========================================================
void ZRoundProgressBar::zp_setMaximum(int nMaximum)
{

}
//==========================================================
void ZRoundProgressBar::zp_setBackgroundColor(QColor color)
{

}
//==========================================================
void ZRoundProgressBar::zp_setRibbonOutlineColor(QColor color)
{

}
//==========================================================
void ZRoundProgressBar::zp_setRibbonColor(QColor color)
{

}
//==========================================================
void ZRoundProgressBar::zp_setInnerEllipseColor(QColor color)
{

}
//==========================================================
void ZRoundProgressBar::zp_setRibbonPathColor(QColor color)
{

}
//==========================================================
void ZRoundProgressBar::zp_setTextColor(QColor color)
{

}
//==========================================================
void ZRoundProgressBar::zp_setFont(QFont font)
{

}
//==========================================================
void ZRoundProgressBar::zp_setRibbonOutlineWidth(uint width)
{

}
//==========================================================
void ZRoundProgressBar::zp_setRibbonWidth(uint width)
{

}
//==========================================================
void ZRoundProgressBar::zp_setRibbonPathOutlineWidth(uint width)
{

}
//==========================================================
void ZRoundProgressBar::zp_setRibbonPathWidth(uint width)
{

}
//==========================================================
void ZRoundProgressBar::zp_setElliptical(bool elliptical)
{

}
//==========================================================
void ZRoundProgressBar::zp_setStartAngleGrad(double angle)
{

}

//==========================================================
void ZRoundProgressBar::zp_setValue(double value)
{

}
//==========================================================
void ZRoundProgressBar::zp_setValue(int value)
{

}
//==========================================================
void ZRoundProgressBar::zp_setPercent(double percent)
{

}
//==========================================================
void ZRoundProgressBar::paintEvent(QPaintEvent *event)
{
    QPainter painter;
    zh_drawBackground(&painter);
    zh_drawRibbonPath(&painter);
    zh_drawRibbon(&painter);
    zh_drawInner(&painter);
    zh_drawText(&painter);

}
//==========================================================
void ZRoundProgressBar::resizeEvent(QResizeEvent* event)
{

}
//==========================================================
void ZRoundProgressBar::zh_drawBackground(QPainter* painter)
{

}
//==========================================================
void ZRoundProgressBar::zh_drawRibbonPath(QPainter* painter)
{

}
//==========================================================
void ZRoundProgressBar::zh_drawRibbon(QPainter* painter)
{

}
//==========================================================
void ZRoundProgressBar::zh_drawInner(QPainter* painter)
{

}
//==========================================================
void ZRoundProgressBar::zh_drawText(QPainter* painter)
{

}
//==========================================================
