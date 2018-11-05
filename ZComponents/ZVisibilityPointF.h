//===============================================================
#ifndef ZVISIBILITYPOINTF_H
#define ZVISIBILITYPOINTF_H
//===============================================================
#include <QPointF>
//===============================================================
class ZVisibilityPointF : public QPointF
{
public:
    ZVisibilityPointF();
    ZVisibilityPointF(const QPoint & point, bool visible = true);
    ZVisibilityPointF(qreal xpos, qreal ypos, bool visible = true);

    bool zp_isVisible() const;
    void zp_setVisible(bool visible = true);

private:

    bool zv_visibility;

};
//===============================================================
#endif // ZVISIBILITYPOINTF_H
