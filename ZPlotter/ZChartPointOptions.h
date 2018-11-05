//============================================
#ifndef ZSERIESPOINTOPTIONS_H
#define ZSERIESPOINTOPTIONS_H
//============================================
#include <QList>
#include <QRectF>
#include <QColor>
#include <QPainterPath>
//============================================
class ZChartPointGraphicsItem;
//============================================
class ZChartPointOptions
{
    friend class ZChartPointGraphicsItem;
public:
    enum PointType{PT_ROUND,
                   PT_SQUARE,
                   PT_TRIANGLE
                  };

    ZChartPointOptions();

    ZChartPointOptions::PointType zp_pointType() const;
    bool zp_setPointType(ZChartPointOptions::PointType);

    int zp_pointPixelSize() const;
    bool zp_setPointPixelSize(int);

    int zp_outLineWidth() const;
    bool zp_setOutLineWidth(int);

    QColor zp_currentPointColor() const;
    bool zp_setCurrentPointColor(QColor);

    QColor zp_outLineColor() const;
    bool zp_setOutLineColor(QColor);

    QColor zp_pointColor() const;
    bool zp_setPointColor(QColor);

    qreal zp_rulerScaleValue(Qt::Orientation) const;
    bool zp_setRulerScaleValue(Qt::Orientation, qreal);

    QString zp_rulerLabelString(Qt::Orientation) const;
    bool zp_setRulerLabelString(Qt::Orientation, const QString&);

    int zp_rulerMarkPrecision(Qt::Orientation) const;
    bool zp_setRulerMarkPrecision(Qt::Orientation, int);

private:

    // VARS
    QList<ZChartPointGraphicsItem*> zv_seriesPointItemList;
    QRectF zv_boundingRect;
    QPainterPath zv_shape;

    qreal zv_horizontalRulerScaleValue;
    QString zv_horizontalRulerLabelString;
    int zv_horizontalRulerMarkPrecision;

    qreal zv_verticalRulerScaleValue;
    QString zv_verticalRulerLabelString;
    int zv_verticalRulerMarkPrecision;


    ZChartPointOptions::PointType zv_pointType;
    int zv_pointSize;
    int zv_outLineWidth;
    QColor zv_currentPointColor;
    QColor zv_outLineColor;
    QColor zv_pointColor;

    // FUNCS
    void zh_recalcShapeAndBoundingRect();

    void zh_updateItems() const;

};
//============================================
#endif // ZSERIESPOINTOPTIONS_H
