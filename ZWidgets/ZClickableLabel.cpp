//=================================================
#include "ZClickableLabel.h"
#include <QMoveEvent>
//=================================================
ZClickableLabel::ZClickableLabel(QWidget* parent) : QLabel(parent)
{

}
//=================================================
void ZClickableLabel::mouseReleaseEvent(QMouseEvent* e)
{
    emit clicked();
}
//=================================================
