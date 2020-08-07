//=======================================================
#include "ZAbstractProcessTimeIndicator.h"

#include <QFontMetrics>
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
    zv_timeLeftLabel->setAlignment(Qt::AlignRight | zv_timeLeftLabel->alignment());
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
QString ZAbstractProcessTimeIndicator::zh_convertTimeToString(double time)
{
    int seconds = static_cast<int>(time);
    int minutes = seconds / 60;
    int hours = minutes / 60;

    // recalc
    seconds -= minutes * 60;
    minutes -= hours * 60;

    // create string
    QString timeString;
    if(hours > 0)
    {
        timeString = QString::number(hours) + QString(":");
    }

    if(minutes < 10)
    {
        // set 0 in decimal
        timeString += "0";
    }
    timeString += QString::number(minutes) + QString(":");

    if(seconds < 10)
    {
        // set 0 in decimal
        timeString += "0";
    }
    timeString += QString::number(seconds);

    return timeString;
}
//=======================================================
void ZAbstractProcessTimeIndicator::zh_recalcTimeLabelMinWidth()
{
    QFont font = zv_timeLeftLabel->font();
    QFontMetrics fontMetrics(font);

    // 59 h 59 m 59 s
    int width = fontMetrics.horizontalAdvance(zh_convertTimeToString(59 * (60 * 60 + 60 + 1)));

    zv_timeLeftLabel->setMinimumWidth(width);
    zv_processDurationLabel->setMinimumWidth(width);
}
//=======================================================
