//======================================================
#include "ZControlAction.h"
#include "QActionEvent"
#include <QDebug>
//======================================================
ZControlAction::ZControlAction(QObject *parent) :
    QAction(parent)
{
    zv_enabled = this->isEnabled();
}
//======================================================
bool ZControlAction::event(QEvent *event)
{
    if(isEnabled() != zv_enabled)
    {
        zv_enabled = isEnabled();
        emit zg_enableChanged(zv_enabled);
    }

    return false;
}
//======================================================
