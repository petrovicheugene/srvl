//===============================================================
#include "ZVisibilityPointF.h"
//===============================================================
ZVisibilityPointF::ZVisibilityPointF() : QPointF()
{
    zv_visibility = true;
}
//===============================================================
ZVisibilityPointF::ZVisibilityPointF(const QPoint & point, bool visible) : QPointF(point)
{
    zv_visibility = visible;
}
//===============================================================
ZVisibilityPointF::ZVisibilityPointF(qreal xpos, qreal ypos, bool visible) : QPointF(xpos, ypos)
{
    zv_visibility = visible;
}
//===============================================================
bool ZVisibilityPointF::zp_isVisible() const
{
    return zv_visibility;
}
//===============================================================
void ZVisibilityPointF::zp_setVisible(bool visible)
{
    zv_visibility = visible;
}
//===============================================================
