//======================================================
#ifndef ZSPECTRUMGRAPHICSITEM_H
#define ZSPECTRUMGRAPHICSITEM_H
//======================================================
#include <QGraphicsItem>
#include "ZGraphicsItemUserTypes.h"
//======================================================
class ZAbstractSpectrum;
//======================================================
class ZSpectrumGraphicsItem : public QGraphicsItem
{
public:

    explicit ZSpectrumGraphicsItem(const ZAbstractSpectrum* spectrum,
                          qreal boundingRectTopFactor,
                          qreal distortionFactor,
                          qreal distortionCorrectionFactor,
                          QGraphicsItem * parent = 0);
    QRectF boundingRect() const override;
    void paint(QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * widget = 0) override;
    QPainterPath shape() const override;
    int type() const override;
    static void zp_setCurrentSpectrumColor(QColor);
    static void zp_setCurrentSpectrumId(qint64);
    static qint64 zp_currentSpectrumId();
    //
    void zp_setSpectrumData(const QList<quint32> &);
    qint64 zp_spectrumId();
    void zp_updateSpectrumData(const ZAbstractSpectrum *);

    void zp_setDistortion(qreal distortionFactor, qreal distortionCorrectionFactor);
    bool zp_isSpectrumCurrent();

    void zp_updateCurrentSpectrum(bool visible);

protected:

    // FUNCS
    void mouseReleaseEvent(QGraphicsSceneMouseEvent * event);
    void mousePressEvent(QGraphicsSceneMouseEvent *event);
    void focusInEvent(QFocusEvent *event);
    void focusOutEvent(QFocusEvent *event);

private:

    // VARS
    QList<quint32> zv_spectrumData;
    QPainterPath zv_spectrumPainterPath;
    QRectF zv_boundingRect;
    QColor zv_color;
    QColor zv_paintColor;
    qint64 zv_spectrumId;
    static QColor zv_currentColor;
    static qint64 zv_currentSpectrumId;

    qreal zv_distortionFactor;
    qreal zv_distortionCorrectionFactor;
    // qreal zv_sceneRectTop;
    qreal zv_boundingRectTopFactor;
    // qreal zv_k;

    // y' = y^zv_logFactor * zv_k
    // max = max^logFactor * zv_k

    // FUNCS
    void zh_calculatePainterPath();
    qreal zh_sceneDistortionPositionForValue(qreal value);

};
//======================================================
#endif // ZSPECTRUMGRAPHICSITEM_H
