//======================================================
#include "ZChemicalElementButton.h"

#include <QApplication>
#include <QDebug>
#include <QFontMetrics>
#include <QFrame>
#include <QPainter>
#include <QPalette>
#include <QPaintEvent>
#include <QPixmap>
#include <QPen>
#include <QRect>
#include <QResizeEvent>
#include <QStyle>
#include <QStyleOptionToolButton>
#include <QStylePainter>

//======================================================
// STATIC
//
QSize ZChemicalElementButton::zv_symbolBoundingRectSize = QSize();
QSize ZChemicalElementButton::zv_zNumberBoundingRectSize = QSize();
QSize ZChemicalElementButton::zv_nameBoundingRectSize = QSize();
double ZChemicalElementButton::zv_smallFontSizeRatio = 1.8;
//======================================================
//======================================================
QSize ZChemicalElementButton::zp_symbolBoundingRectSize()
{
    return zv_symbolBoundingRectSize;
}
//======================================================
QSize ZChemicalElementButton::zp_zNumberBoundingRectSize()
{
    return zv_zNumberBoundingRectSize;
}
//======================================================
QSize ZChemicalElementButton::zp_nameBoundingRectSize()
{
    return zv_nameBoundingRectSize;
}
//======================================================
void ZChemicalElementButton::zp_setSymbolBoundingRectSize(QSize size)
{
    zv_symbolBoundingRectSize = size;
}
//======================================================
void ZChemicalElementButton::zp_setZNumberBoundingRectSize(QSize size)
{
    zv_zNumberBoundingRectSize = size;
}
//======================================================
void ZChemicalElementButton::zp_setNameBoundingRectSize(QSize size)
{
    zv_nameBoundingRectSize = size;
}
//======================================================
int ZChemicalElementButton::zp_smallFontSizeRatio()
{
    return zv_smallFontSizeRatio;
}
//======================================================
bool ZChemicalElementButton::zp_setSmallFontSizeRatio(int ratio)
{
    if(ratio <= 0)
    {
        return false;
    }

    zv_smallFontSizeRatio = ratio;
    return true;
}
//======================================================
// END STATIC
//======================================================
ZChemicalElementButton::ZChemicalElementButton(ZNumber zNumber, QWidget *parent)
    : QToolButton(parent)
{
    zv_frame = new QFrame;
    zv_frameLineWidth = 1;
    zv_margin = 1;
    zv_zNumber = zNumber;
    zv_name = QString();
    zh_setDefaultFonts();
    zh_setDefaultColors();

    zh_createConnections();
}
//======================================================
ZChemicalElementButton::~ZChemicalElementButton()
{
    delete zv_frame;
}
//======================================================
void ZChemicalElementButton::zh_setDefaultFonts()
{
    QFont font = this->font();
    //    font.setBold(true);
    zp_setPropertyFont(PN_SYMBOL, font);

    qreal fontPointSizeF = font.pointSizeF() / zv_smallFontSizeRatio;
    font.setPointSizeF(fontPointSizeF);
    font.setBold(false);

    zp_setPropertyFont(PN_ZNUMBER, font);
    zp_setPropertyFont(PN_NAME, font);
}
//======================================================
void ZChemicalElementButton::zh_setDefaultColors()
{
    //    QColor color = palette().color(QPalette::Active, QPalette::ButtonText);
    //    zp_setPropertyColor(PN_SYMBOL, color);
    //    zp_setPropertyColor(PN_ZNUMBER, color);
    //    zp_setPropertyColor(PN_NAME, color);
}
//======================================================
void ZChemicalElementButton::zh_createConnections()
{
    connect(qApp, &QApplication::fontDatabaseChanged,
            this, &ZChemicalElementButton::zh_onAppFontDatabaseChange);
}
//======================================================
void ZChemicalElementButton::resizeEvent(QResizeEvent* event)
{
    QToolButton::resizeEvent(event);
}
//======================================================
void ZChemicalElementButton::paintEvent(QPaintEvent *event)
{
    if(zv_frame == nullptr)
    {
        return;
    }

    QStyleOptionToolButton option;
    initStyleOption(&option);

    int frameStyle = QFrame::Panel;
    if(this->isDown() || this->isChecked())
    {
        frameStyle |= QFrame::Sunken;
    }
    else
    {
        frameStyle |= QFrame::Raised;
    }

    zv_frame->setPalette(zh_currentPalette());

    zv_frame->setFrameStyle(frameStyle);
    zv_frame->setLineWidth(zv_frameLineWidth);
    zv_frame->resize(event->rect().size());
    // create pixmap and grab the frame on it
    QPixmap buttonPixmap(event->rect().size());
    buttonPixmap =  zv_frame->grab();
    // painter
    QStylePainter pixPainter(&buttonPixmap, this);
    // view port rect without frame
    QRect commonViewportRect = zp_buttonViewPortRect();

    // z number
    if(!zv_zNumberViewPortRect.isEmpty() && !zv_zNumberWindowRect.isEmpty())
    {
        pixPainter.save();
        pixPainter.setFont(zh_zNumberFont());
        pixPainter.setPen(zh_textColor());
        pixPainter.setBrush(zh_textColor());
        pixPainter.setRenderHint(QPainter::Antialiasing);

        zv_zNumberViewPortRect.moveTopRight(commonViewportRect.topRight());
        pixPainter.setViewport(zv_zNumberViewPortRect);
        pixPainter.setWindow(zv_zNumberWindowRect);
        pixPainter.drawText(zv_zNumberWindowRect,
                            Qt::AlignCenter,
                            QString::number(zv_zNumber));
        pixPainter.restore();
    }

    // symbol
    if(!zv_symbolViewPortRect.isEmpty() && !zv_symbolWindowRect.isEmpty())
    {
        pixPainter.save();
        pixPainter.setFont(zh_symbolFont());
        pixPainter.setPen(zh_textColor());
        pixPainter.setBrush(zh_textColor());
        pixPainter.setRenderHint(QPainter::Antialiasing);

        // free part of commonViewportRect
        QRect freePartCommonViewportRect = commonViewportRect.adjusted(0,zv_zNumberViewPortRect.height(),
                                                                       0, -zv_nameViewPortRect.height());

        zv_symbolViewPortRect.moveCenter(freePartCommonViewportRect.center());
        pixPainter.setViewport(zv_symbolViewPortRect);
        pixPainter.setWindow(zv_symbolWindowRect);
        pixPainter.drawText(zv_symbolWindowRect,
                            Qt::AlignCenter,
                            zv_symbol);

        pixPainter.restore();
    }

    // name
    if(!zv_nameViewPortRect.isEmpty() && !zv_nameWindowRect.isEmpty())
    {
        pixPainter.save();
        pixPainter.setFont(zh_nameFont());
        pixPainter.setPen(zh_textColor());
        pixPainter.setBrush(zh_textColor());
        pixPainter.setRenderHint(QPainter::Antialiasing);

        QPoint nameCenterPoint;
        nameCenterPoint.setX(commonViewportRect.center().x());
        nameCenterPoint.setY(commonViewportRect.bottom() - (zv_nameViewPortRect.height() / 2));

        zv_nameViewPortRect.moveCenter(nameCenterPoint);
        pixPainter.setViewport(zv_nameViewPortRect);
        pixPainter.setWindow(zv_nameWindowRect);
        pixPainter.drawText(zv_nameWindowRect,
                            Qt::AlignCenter,
                            zv_name);

        pixPainter.restore();
    }

    // Draw Pixmap
    QPainter painter(this);
    painter.drawPixmap(event->rect(), buttonPixmap);
}
//======================================================
void ZChemicalElementButton::zh_onAppFontDatabaseChange()
{
    zh_recalcPropertyWindowRects();
    update();
}
//======================================================
void ZChemicalElementButton::zp_setChemicalElementSymbol(const QString& symbol)
{
    zv_symbol = symbol;
    zh_recalcPropertyWindowRects();
    emit zg_symbolChanged(zv_symbol);
    update();
}
//======================================================
QString ZChemicalElementButton::zp_chemicalElementSymbol() const
{
    return zv_symbol;
}
//======================================================
void ZChemicalElementButton::zp_setChemicalElementName(const QString& name)
{
    zv_name = name;
    zh_recalcPropertyWindowRects();
    update();
}
//======================================================
QString ZChemicalElementButton::zp_chemicalElementName() const
{
    return zv_name;
}
//======================================================
void ZChemicalElementButton::zp_updatePropertyCaptions()
{
    zp_updatePropertyViewPortRects();
    update();
}
//======================================================
QRect ZChemicalElementButton::zp_buttonViewPortRect() const
{
    QRect rect;
    if(this->isDown())
    {
        rect = this->rect().adjusted(1,1,-1,-1);
        // rect = this->rect().adjusted(0,0,-2,-2);
    }
    else
    {
        rect = this->rect().adjusted(0,0,-2,-2);
        // rect = this->rect().adjusted(1,1,-1,-1);
    }

    int spacing = zh_marginSpacing();
    rect.adjust(spacing, spacing, -spacing, -spacing);
    return rect;
}
//======================================================
int ZChemicalElementButton::zh_marginSpacing() const
{
    int spacing = zv_frameLineWidth + zv_margin;
    return spacing;
}
//======================================================
QSize ZChemicalElementButton::zp_symbolWindowRectSize() const
{
    return zv_symbolWindowRect.size();
}
//======================================================
QSize ZChemicalElementButton::zp_zNumberWindowRectSize() const
{
    return zv_zNumberWindowRect.size();
}
//======================================================
QSize ZChemicalElementButton::zp_nameWindowRectSize() const
{
    return zv_nameWindowRect.size();
}
//======================================================
void ZChemicalElementButton::zp_updateMinimumSizeForSymbolRectSize(QSize symbolSize)
{
    int spacing = zh_marginSpacing();
    symbolSize.setHeight(symbolSize.height() + (spacing * 2));
    symbolSize.setWidth(symbolSize.width() + (spacing * 2));

    setMinimumSize(symbolSize);
    setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
    updateGeometry();
}
//======================================================
void ZChemicalElementButton::zp_setPropertyFont(PropertyName proprtyName,
                                                QFont font)
{
    zv_fontMap[proprtyName] = font;
    update();
}
//======================================================
void ZChemicalElementButton::zp_updatePropertyViewPortRects()
{
    QSize symbolViewPortRectSize;
    zh_recalcViewPortRectSize(zv_symbolBoundingRectSize,
                              zv_symbolWindowRect.size(),
                              symbolViewPortRectSize);
    zv_symbolViewPortRect = QRect(QPoint(0, 0), symbolViewPortRectSize);

    QSize zNumberViewPortRectSize;
    zh_recalcViewPortRectSize(zv_zNumberBoundingRectSize,
                              zv_zNumberWindowRect.size(),
                              zNumberViewPortRectSize);
    zv_zNumberViewPortRect = QRect(QPoint(0, 0), zNumberViewPortRectSize);

    QSize nameViewPortRectSize;
    zh_recalcViewPortRectSize(zv_nameBoundingRectSize,
                              zv_nameWindowRect.size(),
                              nameViewPortRectSize);
    zv_nameViewPortRect = QRect(QPoint(0, 0), nameViewPortRectSize);

    update();
}
//======================================================
void ZChemicalElementButton::zh_recalcViewPortRectSize(QSize boundingViewPortSize,
                                                       QSize windowSize,
                                                       QSize&  viewPortSize) const
{
    if(boundingViewPortSize.isEmpty() || windowSize.isEmpty())
    {
        viewPortSize = QSize();
    }
    else
    {
        double windowRectSideRatio = static_cast<double>(windowSize.width())
                / static_cast<double>(windowSize.height());
        double viewPortRectSideRatio = static_cast<double>(boundingViewPortSize.width())
                / static_cast<double>(boundingViewPortSize.height());

        double scaleFactor;
        if(windowRectSideRatio > viewPortRectSideRatio)
        {
            // fit by width
            scaleFactor = boundingViewPortSize.width()
                    / static_cast<double>(windowSize.width());
        }
        else
        {
            // fit by height
            scaleFactor = boundingViewPortSize.height()
                    / static_cast<double>(windowSize.height());
        }

        viewPortSize.setHeight(qRound(windowSize.height() * scaleFactor));
        viewPortSize.setWidth(qRound(windowSize.width() * scaleFactor));
    }
}
//======================================================
void ZChemicalElementButton::zh_recalcPropertyWindowRects()
{
    QFontMetrics fontMetrics(zh_symbolFont());
    zv_symbolWindowRect.setWidth(fontMetrics.width(zv_symbol));
    zv_symbolWindowRect.setHeight(fontMetrics.height());

    fontMetrics = QFontMetrics(zh_zNumberFont());
    zv_zNumberWindowRect.setWidth(fontMetrics.width(QString::number(zv_zNumber)));
    zv_zNumberWindowRect.setHeight(fontMetrics.height());

    fontMetrics = QFontMetrics(zh_nameFont());
    zv_nameWindowRect.setWidth(fontMetrics.width(zv_name));
    zv_nameWindowRect.setHeight(fontMetrics.height());
}
//======================================================
ZNumber ZChemicalElementButton::zp_zNumber() const
{
    return zv_zNumber;
}
//======================================================
QFont ZChemicalElementButton::zh_symbolFont() const
{
    QFont font = zv_fontMap.value(PN_SYMBOL);
    return font;
}
//======================================================
QFont ZChemicalElementButton::zh_zNumberFont() const
{
    QFont font = zv_fontMap.value(PN_ZNUMBER);
    return font;
}
//======================================================
QFont ZChemicalElementButton::zh_nameFont() const
{
    QFont font = zv_fontMap.value(PN_NAME);
    return font;
}
//======================================================
QColor ZChemicalElementButton::zh_textColor() const
{
    QColor color;
    if(isChecked())
    {
        color = this->palette().color(QPalette::HighlightedText);
    }
    else
    {
        color = this->palette().color(QPalette::ButtonText);
    }
    return color;
}
//======================================================
QPalette ZChemicalElementButton::zh_currentPalette() const
{
    QPalette palette = this->palette();
    if(isChecked())
    {
        palette.setColor(QPalette::Window, palette.color(QPalette::Highlight));
    }

    return palette;
}
//======================================================
