//=======================================================
#include "ZAbstractProcessTimeIndicator.h"
#include <QLayout>
#include <QLabel>
//=======================================================
ZAbstractProcessTimeIndicator::ZAbstractProcessTimeIndicator(QWidget *parent)
    : QWidget(parent)
{
    zh_createComponents();
}
//=======================================================
void ZAbstractProcessTimeIndicator::zh_createComponents()
{
    zv_processNameLabel = new QLabel(this);
    zv_timeLeftLabel = new QLabel(this);
    zv_processDurationLabel = new QLabel(this);
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
void ZAbstractProcessTimeIndicator::zp_setProcessNameString(const QString& name)
{
    zv_processNameLabel->setText(name);
}
//=======================================================
