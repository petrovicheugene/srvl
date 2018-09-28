//=========================================================
#include "ZRulerWidget.h"
#include <QLabel>
#include <QResizeEvent>
#include <QPaintEvent>
#include <QPainter>
#include <QDebug>
#include <QPixmap>
//=========================================================
ZRulerWidget::ZRulerWidget(QWidget *parent) : QWidget(parent)
{
    zv_plot = 0;
    zv_rulersAndGreedManager = 0;
    zv_formatSet << 'f' << 'g' << 'e' << 'E' << 'G';
    //    zv_rightRulerProperty.visibility = false;
    //    zv_topRulerProperty.visibility = false;
    zv_lineColor = palette().color(QPalette::Text);
    zv_lineWidth = 1;

    zv_infoLabel = new QLabel(this);
    zh_recalcRuleSizes();
}
//=========================================================
void ZRulerWidget::zp_setPlotView(QWidget* plot)
{
    if(plot != 0)
    {
        plot->setParent(this);
        zv_plot = plot;
    }
    update();
}
//=========================================================
bool ZRulerWidget::zp_setLineWidth(int width)
{
    if(width < 1)
    {
        return false;
    }
    zv_lineWidth = width;
    update();
    return true;
}
//=========================================================
void ZRulerWidget::zp_setLineColor(QColor color)
{
    zv_lineColor = color;
    update();
}
//=========================================================
void ZRulerWidget::zp_setLeftScratchLineSize(int size)
{
    zv_leftRulerProperty.scratchLineSize = size;
    update();
}
//=========================================================
void ZRulerWidget::zp_setRightScratchLineSize(int size)
{
    zv_rightRulerProperty.scratchLineSize = size;
    update();
}
//=========================================================
void ZRulerWidget::zp_setTopScratchLineSize(int size)
{
    zv_topRulerProperty.scratchLineSize = size;
    update();
}
//=========================================================
void ZRulerWidget::zp_setBottomScratchLineSize(int size)
{
    zv_bottomRulerProperty.scratchLineSize = size;
    update();
}
//=========================================================
void ZRulerWidget::zp_setScratchLineSize(int size)
{
    zv_leftRulerProperty.scratchLineSize = size;
    zv_rightRulerProperty.scratchLineSize = size;
    zv_topRulerProperty.scratchLineSize = size;
    zv_bottomRulerProperty.scratchLineSize = size;
    update();
}
//=========================================================
bool ZRulerWidget::zp_setLeftFontSize(int size)
{
    if(size < 2)
    {
        return false;
    }
    zv_leftRulerProperty.fontSize = size;
    return true;
}
//=========================================================
bool ZRulerWidget::zp_setRightFontSize(int size)
{
    if(size < 2)
    {
        return false;
    }
    zv_rightRulerProperty.fontSize = size;
    return true;
}
//=========================================================
bool ZRulerWidget::zp_setTopFontSize(int size)
{
    if(size < 2)
    {
        return false;
    }
    zv_topRulerProperty.fontSize = size;
    return true;
}
//=========================================================
bool ZRulerWidget::zp_setBottomFontSize(int size)
{
    if(size < 2)
    {
        return false;
    }
    zv_bottomRulerProperty.fontSize = size;
    return true;
}
//=========================================================
int ZRulerWidget::zp_leftFontSize()
{
    return zv_leftRulerProperty.fontSize;
}
//=========================================================
int ZRulerWidget::zp_rightFontSize()
{
    return zv_rightRulerProperty.fontSize;
}
//=========================================================
int ZRulerWidget::zp_topFontSize()
{
    return zv_topRulerProperty.fontSize;
}
//=========================================================
int ZRulerWidget::zp_bottomFontSize()
{
    return zv_bottomRulerProperty.fontSize;
}
//=========================================================
bool ZRulerWidget::zp_setLeftMarkFormat(char format)
{
    if(!zv_formatSet.contains(format))
    {
        return false;
    }
    zv_leftRulerProperty.format = format;
    return true;
}
//=========================================================
bool ZRulerWidget::zp_setRightMarkFormat(char format)
{
    if(!zv_formatSet.contains(format))
    {
        return false;
    }
    zv_rightRulerProperty.format = format;
    return true;
}
//=========================================================
bool ZRulerWidget::zp_setTopMarkFormat(char format)
{
    if(!zv_formatSet.contains(format))
    {
        return false;
    }
    zv_topRulerProperty.format = format;
    return true;
}
//=========================================================
bool ZRulerWidget::zp_setBottomMarkFormat(char format)
{
    if(!zv_formatSet.contains(format))
    {
        return false;
    }
    zv_bottomRulerProperty.format = format;
    return true;
}

//=========================================================
bool ZRulerWidget::zp_setLeftMarkPrecision(int precision)
{
    if(precision < 0)
    {
        return false;
    }
    zv_leftRulerProperty.precision = precision;
    return true;
}
//=========================================================
bool ZRulerWidget::zp_setRightMarkPrecision(int precision)
{
    if(precision < 0)
    {
        return false;
    }
    zv_rightRulerProperty.precision = precision;
    return true;
}
//=========================================================
bool ZRulerWidget::zp_setTopMarkPrecision(int precision)
{
    if(precision < 0)
    {
        return false;
    }
    zv_topRulerProperty.precision = precision;
    return true;
}
//=========================================================
bool ZRulerWidget::zp_setBottomMarkPrecision(int precision)
{
    if(precision < 0)
    {
        return false;
    }
    zv_bottomRulerProperty.precision = precision;
    return true;
}
//=========================================================
void ZRulerWidget::zp_setLeftMarkRecalcMetrix(const QString& label, bool labelVisibility,qreal K0, qreal K1, qreal K2)
{
    zv_leftRulerProperty.ruleLabel = label;
    zv_leftRulerProperty.ruleLabelVisibility = labelVisibility;
    zh_recalcRuleSizes();
    zv_leftRulerProperty.K0 = K0;
    zv_leftRulerProperty.K1 = K1;
    zv_leftRulerProperty.K2 = K2;
    update();
}
//=========================================================
void ZRulerWidget::zp_setRightMarkRecalcMetrix(const QString& label, bool labelVisibility,qreal K0, qreal K1, qreal K2)
{
    zv_rightRulerProperty.ruleLabel = label;
    zv_rightRulerProperty.ruleLabelVisibility = labelVisibility;
    zh_recalcRuleSizes();
    zv_rightRulerProperty.K0 = K0;
    zv_rightRulerProperty.K1 = K1;
    zv_rightRulerProperty.K2 = K2;
    update();
}
//=========================================================
void ZRulerWidget::zp_setTopMarkRecalcMetrix(const QString& label, bool labelVisibility,qreal K0, qreal K1, qreal K2)
{
    zv_topRulerProperty.ruleLabel = label;
    zv_topRulerProperty.ruleLabelVisibility = labelVisibility;
    zh_recalcRuleSizes();
    zv_topRulerProperty.K0 = K0;
    zv_topRulerProperty.K1 = K1;
    zv_topRulerProperty.K2 = K2;
    update();
}
//=========================================================
void ZRulerWidget::zp_setBottomMarkRecalcMetrix(const QString& label, bool labelVisibility,qreal K0, qreal K1, qreal K2)
{
    zv_bottomRulerProperty.ruleLabel = label;
    zv_bottomRulerProperty.ruleLabelVisibility = labelVisibility;
    zh_recalcRuleSizes();
    zv_bottomRulerProperty.K0 = K0;
    zv_bottomRulerProperty.K1 = K1;
    zv_bottomRulerProperty.K2 = K2;
    update();
}
//=========================================================
void ZRulerWidget::zp_setLeftMarkRecalcFlag(bool flag)
{
    if(zv_leftRulerProperty.recalcMarkLabel == flag)
    {
        return;
    }

    zv_leftRulerProperty.recalcMarkLabel = flag;
    update();
}
//=========================================================
void ZRulerWidget::zp_setRightMarkRecalcFlag(bool flag)
{
    if(zv_rightRulerProperty.recalcMarkLabel == flag)
    {
        return;
    }

    zv_rightRulerProperty.recalcMarkLabel = flag;
    update();
}
//=========================================================
void ZRulerWidget::zp_setTopMarkRecalcFlag(bool flag)
{
    if(zv_topRulerProperty.recalcMarkLabel == flag)
    {
        return;
    }

    zv_topRulerProperty.recalcMarkLabel = flag;
    update();
}
//=========================================================
void ZRulerWidget::zp_setBottomMarkRecalcFlag(bool flag)
{
    if(zv_bottomRulerProperty.recalcMarkLabel == flag)
    {
        return;
    }

    zv_bottomRulerProperty.recalcMarkLabel = flag;
    update();
}
//=========================================================
void ZRulerWidget::zp_setHorizontalMarkRecalcFactors(const QString& label, qreal K0, qreal K1, qreal K2)
{
    zv_topRulerProperty.ruleLabel = label;
    zv_topRulerProperty.K0 = K0;
    zv_topRulerProperty.K1 = K1;
    zv_topRulerProperty.K2 = K2;

    zv_bottomRulerProperty.ruleLabel = label;
    zv_bottomRulerProperty.K0 = K0;
    zv_bottomRulerProperty.K1 = K1;
    zv_bottomRulerProperty.K2 = K2;
    update();
}
//=========================================================
void ZRulerWidget::zp_horizontalMarkRecalcFactors(QString& label, qreal& K0, qreal& K1, qreal& K2) const
{
    label = zv_topRulerProperty.ruleLabel;
    K0 = zv_topRulerProperty.K0;
    K1 = zv_topRulerProperty.K1;
    K2 = zv_topRulerProperty.K2;

}
//=========================================================
void ZRulerWidget::zp_setVerticalMarkRecalcFactors(const QString& label, qreal K0, qreal K1, qreal K2)
{
    zv_leftRulerProperty.ruleLabel = label;
    zv_leftRulerProperty.K0 = K0;
    zv_leftRulerProperty.K1 = K1;
    zv_leftRulerProperty.K2 = K2;

    zv_rightRulerProperty.ruleLabel = label;
    zv_rightRulerProperty.K0 = K0;
    zv_rightRulerProperty.K1 = K1;
    zv_rightRulerProperty.K2 = K2;
    update();
}
//=========================================================
void ZRulerWidget::zp_setTopRuleVisibility(bool visibility)
{
    zv_topRulerProperty.visibility = visibility;
    update();
}
//=========================================================
void ZRulerWidget::zp_setBottomRuleVisibility(bool visibility)
{
    zv_bottomRulerProperty.visibility = visibility;
    update();
}
//=========================================================
void ZRulerWidget::zp_setLeftRuleVisibility(bool visibility)
{
    zv_leftRulerProperty.visibility = visibility;
    update();
}
//=========================================================
void ZRulerWidget::zp_setRightRuleVisibility(bool visibility)
{
    zv_rightRulerProperty.visibility = visibility;
    update();
}
//=========================================================
void ZRulerWidget::zp_setTopRuleLabel(const QString &label)
{
    zv_topRulerProperty.ruleLabel = label;
    update();
}
//=========================================================
void ZRulerWidget::zp_setBottomRuleLabel(const QString &label)
{
    zv_bottomRulerProperty.ruleLabel = label;
    update();
}
//=========================================================
void ZRulerWidget::zp_setLeftRuleLabel(const QString &label)
{
    zv_leftRulerProperty.ruleLabel = label;
    update();
}
//=========================================================
void ZRulerWidget::zp_setRightRuleLabel(const QString &label)
{
    zv_rightRulerProperty.ruleLabel = label;
    update();
}
//=========================================================
void ZRulerWidget::zp_setTopRuleLabelVisible(bool visible)
{
    if(zv_topRulerProperty.ruleLabelVisibility == visible)
    {
        return;
    }
    zv_topRulerProperty.ruleLabelVisibility = visible;
    update();
}
//=========================================================
void ZRulerWidget::zp_setBottomRuleLabelVisible(bool visible)
{
    if(zv_bottomRulerProperty.ruleLabelVisibility == visible)
    {
        return;
    }
    zv_bottomRulerProperty.ruleLabelVisibility = visible;
    update();
}
//=========================================================
void ZRulerWidget::zp_setLeftRuleLabelVisible(bool visible)
{
    if(zv_leftRulerProperty.ruleLabelVisibility == visible)
    {
        return;
    }
    zv_leftRulerProperty.ruleLabelVisibility = visible;
    update();
}
//=========================================================
void ZRulerWidget::zp_setRightRuleLabelVisible(bool visible)
{
    if(zv_rightRulerProperty.ruleLabelVisibility == visible)
    {
        return;
    }
    zv_rightRulerProperty.ruleLabelVisibility = visible;
    update();
}
//=========================================================
void ZRulerWidget::zp_setHorizontalRulePointList(QList<RulePoint> * XRuleList)
{
    zv_XRuleList = XRuleList;
}
//=========================================================
void ZRulerWidget::zp_setVerticalRulePointList(QList<RulePoint> * YRuleList)
{
    zv_YRuleList = YRuleList;
}
//=========================================================
int ZRulerWidget::zp_maxMarkWidth()
{
    return zv_maxMarkWidth;
}
//=========================================================
int ZRulerWidget::zp_maxMarkHeight()
{
    return zv_maxMarkHeight;
}
//=========================================================
void ZRulerWidget::zp_setInfoLabelText(const QString& text)
{
    zv_infoLabel->setText(text);
}
//=========================================================
void ZRulerWidget::resizeEvent(QResizeEvent* event)
{
    if(zv_plot != 0)
    {
        // plot layout
        int newPlotWidth = event->size().width() - zv_leftRulerProperty.rulerSize() -
                zv_rightRulerProperty.rulerSize();

        int newPliotHeight = event->size().height() - zv_topRulerProperty.rulerSize() -
                zv_bottomRulerProperty.rulerSize();

        if(newPlotWidth < 50 || newPliotHeight < 20)
        {
            QWidget::resizeEvent(event);
            zv_dontRepaint = true;
            zv_plot->setHidden(true);
            return;
        }
        else
        {
            zv_dontRepaint = false;
            zv_plot->setHidden(false);
        }

        QSize plotSize(newPlotWidth, newPliotHeight);
        QPoint plotPos(zv_leftRulerProperty.rulerSize(), zv_topRulerProperty.rulerSize());

        QRect geometryRect(plotPos, plotSize);

        // scratch line rects
        // QRect boundRect = geometryRect.adjusted(-1, -1, 0, 0);
        // left
        QPoint scratchPoint(geometryRect.left() - zv_leftRulerProperty.scratchLineSize, geometryRect.top() - 1);
        QSize scratchRectSize(zv_leftRulerProperty.scratchLineSize, geometryRect.height() + 2);
        zv_leftRulerProperty.scratchRect = QRect(scratchPoint, scratchRectSize);
        // right
        scratchPoint = QPoint(geometryRect.right() + 1, geometryRect.top() - 1);
        scratchRectSize = QSize(zv_rightRulerProperty.scratchLineSize, geometryRect.height() + 2);
        zv_rightRulerProperty.scratchRect = QRect(scratchPoint, scratchRectSize);
        // top
        scratchPoint = QPoint(geometryRect.left() - 1, geometryRect.top() - zv_topRulerProperty.scratchLineSize);
        scratchRectSize = QSize(geometryRect.width() + 2, zv_topRulerProperty.scratchLineSize);
        zv_topRulerProperty.scratchRect = QRect(scratchPoint, scratchRectSize);
        // bottom
        scratchPoint = QPoint(geometryRect.left() - 1, geometryRect.bottom() +1);
        scratchRectSize = QSize(geometryRect.width() + 2, zv_bottomRulerProperty.scratchLineSize);
        zv_bottomRulerProperty.scratchRect = QRect(scratchPoint, scratchRectSize);

        // mark lines rects and rule labelRect
        zv_topRulerProperty.markRect = QRect(QPoint(0,0), QSize(event->size().width(), zv_topRulerProperty.markLineSize));
        if(zv_topRulerProperty.ruleLabelVisibility)
        {
            int dy2 = (zv_topRulerProperty.markRect.height()  - zv_maxMarkHeight*1.5) * -1;
            zv_topRulerProperty.ruleLabelRect = zv_topRulerProperty.markRect.adjusted(0,0,0,dy2);
        }
        else
        {
            zv_topRulerProperty.ruleLabelRect = QRect();
        }

        zv_bottomRulerProperty.markRect = QRect(QPoint(0,event->size().height() - zv_bottomRulerProperty.markLineSize),
                                                QSize(event->size().width(), zv_bottomRulerProperty.markLineSize));
        if(zv_bottomRulerProperty.ruleLabelVisibility)
        {
            int dy1 = (zv_bottomRulerProperty.markRect.height()  - zv_maxMarkHeight*1.5);
            zv_bottomRulerProperty.ruleLabelRect = zv_bottomRulerProperty.markRect.adjusted(0, dy1, 0,0);
        }
        else
        {
            zv_bottomRulerProperty.ruleLabelRect = QRect();
        }

        zv_leftRulerProperty.markRect = QRect(QPoint(0, geometryRect.top() - zv_maxMarkHeight),
                                              QSize(zv_leftRulerProperty.markLineSize, geometryRect.height() + (zv_maxMarkHeight*2)));
        if(zv_leftRulerProperty.ruleLabelVisibility)
        {
            int dx2 = (zv_leftRulerProperty.markRect.width() - zv_maxMarkHeight*1.5) * -1;
            zv_leftRulerProperty.ruleLabelRect = zv_leftRulerProperty.markRect.adjusted(0,0, dx2, 0);
        }
        else
        {
            zv_leftRulerProperty.ruleLabelRect = QRect();
        }

        zv_rightRulerProperty.markRect = QRect(QPoint(geometryRect.right()+1 + zv_rightRulerProperty.scratchLineSize, geometryRect.top() - zv_maxMarkHeight),
                                               QSize(zv_rightRulerProperty.markLineSize, geometryRect.height() + (zv_maxMarkHeight*2)));
        if(zv_rightRulerProperty.ruleLabelVisibility)
        {
            int dx1 = zv_leftRulerProperty.markRect.width() - zv_maxMarkHeight*1.5;
            zv_rightRulerProperty.ruleLabelRect = zv_rightRulerProperty.markRect.adjusted(dx1, 0, 0, 0 );
        }
        else
        {
            zv_rightRulerProperty.ruleLabelRect = QRect();
        }

        zv_plot->setGeometry(geometryRect);

        QRect infoLabelRect = zv_bottomRulerProperty.ruleLabelRect;
        infoLabelRect.setLeft(geometryRect.left());
        QFontMetrics fm(this->font());
        int ruleLabelWidth = fm.width(zv_bottomRulerProperty.ruleLabel);
        infoLabelRect.setRight(geometryRect.center().x() - ruleLabelWidth);
        zv_infoLabel->setGeometry(infoLabelRect);
    }

    QWidget::resizeEvent(event);
}
//=========================================================
void ZRulerWidget::paintEvent(QPaintEvent* event)
{
    if(zv_plot != 0 && !zv_dontRepaint)
    {
        QPainter painter(this);
        painter.setWindow(event->rect());
        QPen pen(QBrush(zv_lineColor), zv_lineWidth);
        painter.setPen(pen);

        zh_paintLeftRule(&painter);
        zh_paintRightRule(&painter);
        zh_paintTopRule(&painter);
        zh_paintBottomRule(&painter);


        QRect boundRect = zv_plot->geometry().adjusted(-1, -1, 0, 0);
        painter.drawRect(boundRect);

        return;
    }

    QWidget::paintEvent(event);
}
//=========================================================
void ZRulerWidget::zh_paintLeftRule(QPainter* painter)
{
    if(!zv_leftRulerProperty.visibility || zv_YRuleList == 0)
    {
        return;
    }
    painter->save();

    QFont font = this->font();
    font.setPixelSize(zv_leftRulerProperty.fontSize);
    QFontMetrics fontMetrics(font);
    QRect markRect;
    painter->setFont(font);

    int y;
    for(int i = 0; i < zv_YRuleList->count(); i++)
    {
        // scratch
        y = mapFromGlobal(QPoint(0, qRound(zv_YRuleList->value(i).globalPos))).y();

        if(zv_leftRulerProperty.scratchRect.isValid())
        {
            painter->drawLine(zv_leftRulerProperty.scratchRect.left() , y,
                              zv_leftRulerProperty.scratchRect.right(), y);

        }
        // Mark
        if(zv_leftRulerProperty.markRect.isValid() && zv_YRuleList->value(i).markType != RulePoint::MT_SCRATCH)
        {
            // scratch
            painter->drawLine(zv_leftRulerProperty.markRect.right() - zv_leftRulerProperty.scratchLineSize, y,
                              zv_leftRulerProperty.markRect.right(), y);


            // mark
            if(zv_YRuleList->value(i).markType == RulePoint::MT_MARK)
            {
                qreal markValue = zv_YRuleList->value(i).markValue * -1;
                if(zv_leftRulerProperty.recalcMarkLabel)
                {
                    markValue = pow(markValue, 2.0)*zv_leftRulerProperty.K2
                            + markValue*zv_leftRulerProperty.K1
                            + zv_leftRulerProperty.K0;
                }
                QString mark = QString::number(markValue, zv_leftRulerProperty.format, zv_leftRulerProperty.precision);
                markRect = QRect(QPoint(0, 0), QSize(fontMetrics.width(mark), zv_maxMarkHeight));
                markRect.moveCenter(QPoint(zv_leftRulerProperty.markRect.right()
                                           - zv_leftRulerProperty.scratchLineSize
                                           - zv_leftRulerProperty.markOffsetFromScratch
                                           - markRect.width()/2, y));
                painter->drawText(markRect, mark);
            }
        }
    }

    if(zv_leftRulerProperty.ruleLabelVisibility)
    {
        painter->rotate(-90);
        QRect ruleLabelRectForRotatedPainter;
        ruleLabelRectForRotatedPainter.setBottom(zv_leftRulerProperty.ruleLabelRect.right());
        ruleLabelRectForRotatedPainter.setTop(zv_leftRulerProperty.ruleLabelRect.left());
        ruleLabelRectForRotatedPainter.setRight(zv_leftRulerProperty.ruleLabelRect.top() * -1);
        ruleLabelRectForRotatedPainter.setLeft(zv_leftRulerProperty.ruleLabelRect.bottom() * -1);
        painter->setRenderHint(QPainter::Antialiasing, true);
        painter->drawText(ruleLabelRectForRotatedPainter, Qt::AlignCenter, zv_leftRulerProperty.ruleLabel);
    }

    painter->restore();
}
//=========================================================
void ZRulerWidget::zh_paintRightRule(QPainter* painter)
{
    if(!zv_rightRulerProperty.visibility || zv_YRuleList == 0)
    {
        return;
    }
    painter->save();

    QFont font = this->font();
    font.setPixelSize(zv_rightRulerProperty.fontSize);
    QFontMetrics fontMetrics(font);
    QRect markRect;
    painter->setFont(font);

    int y;
    for(int i = 0; i < zv_YRuleList->count(); i++)
    {
        // scratch
        y = mapFromGlobal(QPoint(0, qRound(zv_YRuleList->value(i).globalPos))).y();

        if(zv_rightRulerProperty.scratchRect.isValid())
        {
            painter->drawLine(zv_rightRulerProperty.scratchRect.left() , y,
                              zv_rightRulerProperty.scratchRect.right(), y);

        }
        // Mark
        if(zv_rightRulerProperty.markRect.isValid() && zv_YRuleList->value(i).markType != RulePoint::MT_SCRATCH)
        {
            // scratch
            painter->drawLine(zv_rightRulerProperty.markRect.left() + zv_rightRulerProperty.scratchLineSize, y,
                              zv_rightRulerProperty.markRect.left(), y);


            // mark
            if(zv_YRuleList->value(i).markType == RulePoint::MT_MARK)
            {
                qreal markValue = zv_YRuleList->value(i).markValue * -1;
                if(zv_rightRulerProperty.recalcMarkLabel)
                {
                    markValue = pow(markValue, 2.0)*zv_rightRulerProperty.K2
                            + markValue*zv_rightRulerProperty.K1
                            + zv_rightRulerProperty.K0;
                }

                QString mark = QString::number(markValue, zv_rightRulerProperty.format, zv_rightRulerProperty.precision);
                markRect = QRect(QPoint(0, 0), QSize(fontMetrics.width(mark), zv_maxMarkHeight));
                markRect.moveCenter(QPoint(zv_rightRulerProperty.markRect.left()
                                           + zv_rightRulerProperty.scratchLineSize
                                           + zv_rightRulerProperty.markOffsetFromScratch
                                           + markRect.width()/2, y));
                painter->drawText(markRect, mark);
            }
        }
    }

    if(zv_rightRulerProperty.ruleLabelVisibility)
    {
        painter->rotate(-90);
        QRect ruleLabelRectForRotatedPainter;
        ruleLabelRectForRotatedPainter.setBottom(zv_rightRulerProperty.ruleLabelRect.right());
        ruleLabelRectForRotatedPainter.setTop(zv_rightRulerProperty.ruleLabelRect.left());
        ruleLabelRectForRotatedPainter.setRight(zv_rightRulerProperty.ruleLabelRect.top() * -1);
        ruleLabelRectForRotatedPainter.setLeft(zv_rightRulerProperty.ruleLabelRect.bottom() * -1);
        painter->setRenderHint(QPainter::Antialiasing, true);
        painter->drawText(ruleLabelRectForRotatedPainter, Qt::AlignCenter, zv_rightRulerProperty.ruleLabel);
    }
    painter->restore();
}
//=========================================================
void ZRulerWidget::zh_paintTopRule(QPainter* painter)
{
    if(!zv_topRulerProperty.visibility || zv_XRuleList == 0)
    {
        return;
    }
    painter->save();

    int x;
    for(int i = 0; i < zv_XRuleList->count(); i++)
    {
        // scratch
        x = mapFromGlobal(QPoint(qRound(zv_XRuleList->value(i).globalPos), 0)).x();

        if(zv_topRulerProperty.scratchRect.isValid())
        {
            painter->drawLine(x, zv_topRulerProperty.scratchRect.top(),
                              x, zv_topRulerProperty.scratchRect.bottom());
        }
        // Mark
        if(zv_topRulerProperty.markRect.isValid() && zv_XRuleList->value(i).markType != RulePoint::MT_SCRATCH)
        {
            QFont font = this->font();
            font.setPixelSize(zv_topRulerProperty.fontSize);
            QFontMetrics fontMetrics(font);
            QRect markRect;
            painter->setFont(font);
            // scratch
            painter->drawLine(x, zv_topRulerProperty.markRect.bottom() -  zv_topRulerProperty.scratchLineSize, x,
                              zv_topRulerProperty.markRect.bottom());


            // mark
            if(zv_XRuleList->value(i).markType == RulePoint::MT_MARK)
            {
                qreal markValue = zv_XRuleList->value(i).markValue;
                if(zv_topRulerProperty.recalcMarkLabel)
                {
                    markValue = pow(markValue, 2.0)*zv_topRulerProperty.K2
                            + markValue*zv_topRulerProperty.K1
                            + zv_topRulerProperty.K0;
                }

                QString mark = QString::number(markValue, zv_topRulerProperty.format, zv_topRulerProperty.precision);
                markRect = QRect(QPoint(0, 0), QSize(fontMetrics.width(mark), zv_maxMarkHeight));
                markRect.moveCenter(QPoint(x, zv_topRulerProperty.markRect.bottom()
                                           - zv_topRulerProperty.scratchLineSize
                                           - zv_topRulerProperty.markOffsetFromScratch
                                           - markRect.height()/2));
                painter->drawText(markRect, mark);
            }
        }
    }

    if(zv_topRulerProperty.ruleLabelVisibility)
    {
        painter->setRenderHint(QPainter::Antialiasing, true);
        painter->drawText(zv_topRulerProperty.ruleLabelRect, Qt::AlignCenter, zv_topRulerProperty.ruleLabel);
        //painter->fillRect(zv_topRulerProperty.ruleLabelRect, QBrush(QColor(Qt::blue)));
    }

    painter->restore();
}
//=========================================================
void ZRulerWidget::zh_paintBottomRule(QPainter* painter)
{
    if(!zv_bottomRulerProperty.visibility || zv_XRuleList == 0)
    {
        return;
    }
    painter->save();

    int x;
    for(int i = 0; i < zv_XRuleList->count(); i++)
    {
        // scratch
        x = mapFromGlobal(QPoint(qRound(zv_XRuleList->value(i).globalPos), 0)).x();

        if(zv_bottomRulerProperty.scratchRect.isValid())
        {
            painter->drawLine(x, zv_bottomRulerProperty.scratchRect.top(),
                              x, zv_bottomRulerProperty.scratchRect.bottom());
        }
        // Mark
        if(zv_bottomRulerProperty.markRect.isValid() && zv_XRuleList->value(i).markType != RulePoint::MT_SCRATCH)
        {
            QFont font = this->font();
            font.setPixelSize(zv_bottomRulerProperty.fontSize);
            QFontMetrics fontMetrics(font);
            QRect markRect;
            painter->setFont(font);
            // scratch
            painter->drawLine(x, zv_bottomRulerProperty.markRect.top() + zv_bottomRulerProperty.scratchLineSize, x,
                              zv_bottomRulerProperty.markRect.top());


            // mark
            if(zv_XRuleList->value(i).markType == RulePoint::MT_MARK)
            {
                qreal markValue = zv_XRuleList->value(i).markValue;
                if(zv_bottomRulerProperty.recalcMarkLabel)
                {
                    markValue = pow(markValue, 2.0)*zv_bottomRulerProperty.K2
                            + markValue*zv_bottomRulerProperty.K1
                            + zv_bottomRulerProperty.K0;
                }

                QString mark = QString::number(markValue, zv_bottomRulerProperty.format, zv_bottomRulerProperty.precision);
                markRect = QRect(QPoint(0, 0), QSize(fontMetrics.width(mark), zv_maxMarkHeight));
                markRect.moveCenter(QPoint(x, zv_bottomRulerProperty.markRect.top()
                                           + zv_bottomRulerProperty.scratchLineSize
                                           + zv_bottomRulerProperty.markOffsetFromScratch
                                           + markRect.height()/2));
                painter->drawText(markRect, mark);
            }
        }
    }

    if(zv_bottomRulerProperty.ruleLabelVisibility)
    {
        painter->setRenderHint(QPainter::Antialiasing, true);
        painter->drawText(zv_bottomRulerProperty.ruleLabelRect, Qt::AlignCenter, zv_bottomRulerProperty.ruleLabel);
        //painter->fillRect(zv_topRulerProperty.ruleLabelRect, QBrush(QColor(Qt::blue)));
    }

    painter->restore();
}
//=========================================================
void ZRulerWidget::zh_recalcRuleSizes()
{
    int fontSize = qMax( qMax(zv_leftRulerProperty.fontSize, zv_rightRulerProperty.fontSize),
                         qMax(zv_topRulerProperty.fontSize, zv_bottomRulerProperty.fontSize));
    int precision = qMax( qMax(zv_leftRulerProperty.precision, zv_rightRulerProperty.precision),
                          qMax(zv_topRulerProperty.precision, zv_bottomRulerProperty.precision));

    QFont f = font();
    f.setPixelSize(fontSize);
    QString maxString = QString::number(-5.555555555555E-99, 'f', precision);

    QFontMetrics fm(font());
    zv_maxMarkWidth = fm.width(maxString);
    zv_maxMarkHeight = fm.height();

    // horizontal offset
    zv_topRulerProperty.markOffsetFromScratch = 1;
    zv_bottomRulerProperty.markOffsetFromScratch = 1;

    f.setPixelSize(zv_leftRulerProperty.fontSize);
    fm = QFontMetrics(font());
    zv_leftRulerProperty.markOffsetFromScratch = fm.width("8");

    f.setPixelSize(zv_rightRulerProperty.fontSize);
    fm = QFontMetrics(font());
    zv_rightRulerProperty.markOffsetFromScratch = fm.width("8");

    // mark line sizes
    zv_topRulerProperty.markLineSize = zv_maxMarkHeight * 1.5;
    zv_bottomRulerProperty.markLineSize = zv_maxMarkHeight * 1.5;
    zv_leftRulerProperty.markLineSize = zv_maxMarkWidth + zv_leftRulerProperty.markOffsetFromScratch + zv_leftRulerProperty.scratchLineSize;
    zv_rightRulerProperty.markLineSize = zv_maxMarkWidth  + zv_rightRulerProperty.markOffsetFromScratch + zv_rightRulerProperty.scratchLineSize;

    // adding space for rule name
    if(zv_leftRulerProperty.ruleLabelVisibility)
    {
        zv_leftRulerProperty.markLineSize += zv_maxMarkHeight * 1.5;
    }
    if(zv_rightRulerProperty.ruleLabelVisibility)
    {
        zv_rightRulerProperty.markLineSize += zv_maxMarkHeight * 1.5;
    }
    if(zv_topRulerProperty.ruleLabelVisibility)
    {
        zv_topRulerProperty.markLineSize += zv_maxMarkHeight * 1.5;
    }
    if(zv_bottomRulerProperty.ruleLabelVisibility)
    {
        zv_bottomRulerProperty.markLineSize += zv_maxMarkHeight * 1.5;
    }
}
//=========================================================
