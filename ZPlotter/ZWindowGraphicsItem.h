//======================================================
#ifndef ZWINDOWGRAPHICSITEM_H
#define ZWINDOWGRAPHICSITEM_H
//======================================================
#include <QGraphicsItem>
#include "ZGraphicsItemUserTypes.h"
#include <QColor>
#include <QPair>
//======================================================
class ZCalibrationWindow;
//======================================================
class ZWindowGraphicsItem : public QGraphicsItem
{
public:
   explicit ZWindowGraphicsItem(const ZCalibrationWindow*,
                                QColor color,
                                QGraphicsItem * parent = 0);
   // overrided
   QRectF boundingRect() const override;
   void paint(QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * widget = 0) override;
   QPainterPath shape() const;
   int type() const;

   // FUNCS
   static void zp_setCurrentWindowColor(QColor);
   static void zp_setCurrentWindowId(qint64);
   static bool zp_setTopAndButtonMargins(qreal top, qreal bottom);
   static bool zp_setAlphaChannelValue(quint8 alpha);

   qint64 zp_windowId() const;
   void zp_setLeftRightMargins(qreal, qreal);
   void zp_updateCurrentWindow(bool);
   void zp_updateItem();

   void zp_setVisible(bool visible);

protected:

   // FUNCS
   void mouseReleaseEvent(QGraphicsSceneMouseEvent * event);
   void zh_recalcShapeAndBoundingRect();

private:

   // VARS
   QColor zv_color;
   QColor zv_lineColor;
   QColor zv_brushColor;
   qint64 zv_windowId;
   qreal zv_leftMargin;
   qreal zv_rightMargin;

   static qreal zv_topMargin;
   static qreal zv_bottomMargin;
   static qint64 zv_currentWindowId;
   static quint8 zv_alphaCannelValue;

   static QColor zv_currentWindowColor;
   QPainterPath zv_shape;
   QPainterPath zv_paintRect;
   QPainterPath zv_marginsPainterPath;
   QRectF zv_boundingRect;


   // FUNCS

};
//======================================================
#endif // ZWINDOWGRAPHICSITEM_H
