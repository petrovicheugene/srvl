//===========================================================
#include "ZStartStopButtonWidget.h"
#include <QVBoxLayout>
#include <QHBoxLayout>

#include <QButtonGroup>
#include <QPushButton>
#include <QHBoxLayout>
#include <QToolButton>
//===========================================================
ZStartStopButtonWidget::ZStartStopButtonWidget(Qt::Orientation orientation, QWidget *parent)
    : QWidget(parent)
{
    zh_createComponents();
    zh_createConnections();
    zp_setOrientation(orientation);
    zp_setButtonState(0);
}
//===========================================================
//void ZStartStopButton::zp_setSizeHint(QSize size)
//{
//    zv_sizeHint = size;
//}
////===========================================================
//void ZStartStopButton::paintEvent(QPaintEvent* event)
//{
//    QPainter painter(this);
//    QPen pen = QPen(QBrush(QColor(Qt::black)), 1);
//    painter.setPen(pen);
//    painter.setBrush(QBrush(QColor(Qt::darkYellow)));

//    painter.drawEllipse(event->rect().adjusted(0,0, -1, -1));
//}
////===========================================================
//QSize ZStartStopButton::sizeHint() const
//{
//    return zv_sizeHint;
//}
////===========================================================
//void ZStartStopButton::mouseReleaseEvent(QMouseEvent *event)
//{

//}
////===========================================================
//void ZStartStopButton::mousePressEvent(QMouseEvent *event)
//{

//}
//===========================================================
void ZStartStopButtonWidget::zp_setButtonState(int state)
{
    //zv_buttonGroup->blockSignals(true);
    zv_startButton->blockSignals(true);
    zv_stopButton->blockSignals(true);
    switch(state)
    {
    case 0: // stopped
        zv_startButton->setChecked(false);
        zv_stopButton->setChecked(true);
        break;

    case 1: // started
        zv_startButton->setChecked(true);
        zv_stopButton->setChecked(false);
        break;

    case 2: // suspended
        zv_startButton->setChecked(false);
        zv_stopButton->setChecked(false);
        break;
    default: // stopped
        zv_startButton->setChecked(false);
        zv_stopButton->setChecked(true);
    }
    zv_startButton->blockSignals(false);
    zv_stopButton->blockSignals(false);

    //zv_buttonGroup->blockSignals(false);
}
//===========================================================
void ZStartStopButtonWidget::zh_createComponents()
{
    QHBoxLayout* mainLayout = new QHBoxLayout;
    setLayout(mainLayout);

    // stretch on left
    mainLayout->addStretch();

    //zv_buttonGroup = new QButtonGroup(this);

    zv_startButton = new QPushButton(this);
    zv_startButton->setMinimumHeight(30);
    // zv_startButton->setFlat(true);
    zv_startButton->setText(tr("Start"));
    zv_startButton->setToolTip(tr("Start measuring"));
    zv_startButton->setIcon(QIcon(":/images/ZImages/start-8"));
    zv_startButton->setCheckable(true);
    mainLayout->addWidget(zv_startButton);
    //zv_buttonGroup->addButton(zv_startButton);

    zv_stopButton = new QPushButton(this);
    zv_stopButton->setMinimumHeight(30);
    // zv_stopButton->setFlat(true);
    zv_stopButton->setText(tr("Stop"));
    zv_stopButton->setToolTip(tr("Stop measuring"));
    zv_stopButton->setIcon(QIcon(":/images/ZImages/stop-10"));
    zv_stopButton->setCheckable(true);
    mainLayout->addWidget(zv_stopButton);
    //zv_buttonGroup->addButton(zv_stopButton);

}
//===========================================================
void ZStartStopButtonWidget::zh_createConnections()
{
    connect(zv_startButton, &QPushButton::toggled,
            this, &ZStartStopButtonWidget::zh_onButtonToggle);
    connect(zv_stopButton, &QPushButton::toggled,
            this, &ZStartStopButtonWidget::zh_onButtonToggle);


//        connect(zv_buttonGroup, SIGNAL(buttonToggled(QAbstractButton*, bool)),
//                this, SLOT(zh_onButtonGroupToggle(QAbstractButton*, bool)));

}
//===========================================================
void ZStartStopButtonWidget::zh_onButtonToggle(bool checked)
{
    QAbstractButton* button = qobject_cast<QAbstractButton*>(sender());
    if(!button || !checked)
    {
        return;
    }

    if(button == zv_startButton)
    {
        zv_stopButton->setChecked(false);
        emit zg_start();
    }
    else if(button == zv_stopButton)
    {
        zv_startButton->setChecked(false);
        emit zg_stop();
    }
}
//===========================================================
void ZStartStopButtonWidget::zh_onButtonGroupToggle(QAbstractButton* button, bool checked)
{
    if(!button)
    {
        return;
    }

    if(checked)
    {
        if(button == zv_startButton)
        {
            zv_stopButton->setChecked(false);
            emit zg_start();
        }
        else
        {
            zv_startButton->setChecked(false);
            emit zg_stop();
        }
    }
}
//===========================================================
void ZStartStopButtonWidget::zp_setOrientation(Qt::Orientation orientation)
{
    QLayout* oldLayout = layout();
    QLayout* mainLayout;
    if(orientation == Qt::Vertical)
    {
        mainLayout = new QVBoxLayout;
    }
    else
    {
        QHBoxLayout* hLayout = new QHBoxLayout;
        hLayout->addStretch();
        mainLayout = hLayout;
    }

    mainLayout->addWidget(zv_startButton);
    mainLayout->addWidget(zv_stopButton);

    // delete old layout
    if(oldLayout != 0)
    {
        delete oldLayout;
    }

    // replace layout
    setLayout(mainLayout);
}
//===========================================================
