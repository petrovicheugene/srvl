//=======================================================
#include "ZAbstractProcessTimeIndicator.h"
#include <QLayout>
//=======================================================
ZAbstractProcessTimeIndicator::ZAbstractProcessTimeIndicator(QWidget *parent)
    : QWidget(parent)
{


}
//=======================================================
void ZAbstractProcessTimeIndicator::zp_setMainLayoutMargin(int margin)
{
    QLayout* mainLayout = layout();
    if(mainLayout == 0 || margin < 0)
    {
        return;
    }

    mainLayout->setMargin(margin);
}
//=======================================================
void ZAbstractProcessTimeIndicator::zp_setMainLayoutSpacing(int spacing)
{
    QLayout* mainLayout = layout();
    if(mainLayout == 0 || spacing < 0)
    {
        return;
    }

    mainLayout->setSpacing(spacing);
}
//=======================================================
