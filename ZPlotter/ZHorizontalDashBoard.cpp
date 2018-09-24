//===========================================================
#include "ZHorizontalDashBoard.h"
#include "ZPlotGraphicsView.h"
#include "ZPlotGraphicsScene.h"
#include "ZGeneral.h"

#include <QToolButton>
#include <QHBoxLayout>
#include <QTimerEvent>
#include <QAction>
#include <QSlider>

//===========================================================
ZHorizontalDashBoard::ZHorizontalDashBoard(QWidget *parent) : QFrame(parent)
{
    zv_plotGraphicsView = 0;
    zv_plotGraphicsScene = 0;
    zv_timer = 0;
    zh_createComponents();
    zh_createConnections();
}
//===========================================================
void ZHorizontalDashBoard::zh_createComponents()
{
    zv_mainLayout = new QHBoxLayout(this);
    zv_mainLayout->setMargin(0);
    zv_mainLayout->setSpacing(3);
    setLayout(zv_mainLayout);

    zv_distortionSlider = new QSlider(this);
    zv_distortionSlider->setMinimum(0);
    zv_distortionSlider->setMaximum(15);
    zv_distortionSlider->setOrientation(Qt::Horizontal);
    zv_distortionSlider->setTickInterval(3);
    zv_distortionSlider->setTickPosition(QSlider::TicksAbove);
    zv_distortionSlider->setValue(0);
    zv_mainLayout->addWidget(zv_distortionSlider);

    zv_hideGridButton = new QToolButton(this);
    zv_hideGridButton->setIcon(QIcon(NS_Icons::glIconShowGrid));
    zv_hideGridButton->setCheckable(true);
    zv_mainLayout->addWidget(zv_hideGridButton, 0, Qt::AlignRight);

    zv_increaseVerticalButton = new QToolButton(this);
    zv_increaseVerticalButton->setIcon(QIcon(NS_Icons::glIconVerticalIncrease));
    zv_mainLayout->addWidget(zv_increaseVerticalButton, 0, Qt::AlignRight);

    zv_decreaseVerticalButton = new QToolButton(this);
    zv_decreaseVerticalButton->setIcon(QIcon(NS_Icons::glIconVerticalDecrease));
    zv_mainLayout->addWidget(zv_decreaseVerticalButton, 0, Qt::AlignRight);

    zv_increaseHorizontalButton = new QToolButton(this);
    zv_increaseHorizontalButton->setIcon(QIcon(NS_Icons::glIconHorizontalIncrease));
    zv_mainLayout->addWidget(zv_increaseHorizontalButton, 0, Qt::AlignRight);

    zv_decreaseHorizontalButton = new QToolButton(this);
    zv_decreaseHorizontalButton->setIcon(QIcon(NS_Icons::glIconHorizontalDecrease));
    zv_mainLayout->addWidget(zv_decreaseHorizontalButton, 0, Qt::AlignRight);

    zv_fitViewInSceneButton = new QToolButton(this);
    zv_fitViewInSceneButton->setIcon(QIcon(NS_Icons::glIconFitInRect));
    zv_mainLayout->addWidget(zv_fitViewInSceneButton, 0, Qt::AlignRight);

}
//===========================================================
void ZHorizontalDashBoard::zp_setPlotGraphicsView(ZPlotGraphicsView* plotGraphicsView)
{
    zv_plotGraphicsView = plotGraphicsView;
}
//===========================================================
void ZHorizontalDashBoard::zp_setPlotGraphicsScene(ZPlotGraphicsScene* plotGraphicsScene)
{
    zv_plotGraphicsScene = plotGraphicsScene;
}
//===========================================================
void ZHorizontalDashBoard::zp_appendButtonsToDashboard(QList<QAction*>& actionList,
                                                       AuxWidgetPositon position,
                                                       Qt::AlignmentFlag alignment,
                                                       int extraSeparatorSpacing)
{
    int insertIndex;
    if(position == AWP_LEFT_OF_BUTTONS)
    {
        insertIndex = 1; // 0 - log slider
    }
    else
    {
        //insertIndex = actionList.count();
        insertIndex = zv_mainLayout->count();
        zv_mainLayout->addSpacing(extraSeparatorSpacing);
    }

    QToolButton* button;
    for(int i = 0; i < actionList.count(); i++, insertIndex++)
    {
        button = new QToolButton(this);
        button->setIcon(actionList.value(i)->icon());
        button->setToolTip(actionList.value(i)->toolTip());
        button->setCheckable(actionList.value(i)->isCheckable());
        if(actionList.value(i)->isCheckable())
        {
            //            connect(button, &QToolButton::toggled ,
            //                    actionList[i], &QAction::trigger);
            connect(button, &QToolButton::toggled ,
                    actionList[i], &QAction::setChecked);
            connect(actionList[i], &QAction::toggled,
                    button, &QToolButton::setChecked);
        }
        else
        {
            connect(button, &QToolButton::clicked,
                    actionList[i], &QAction::trigger);

        }
        zv_mainLayout->insertWidget(insertIndex, button, alignment);
    }

    if(position == AWP_LEFT_OF_BUTTONS)
    {
        if(alignment == Qt::AlignLeft)
        {
            zv_mainLayout->insertStretch(actionList.count(), 999999);
        }

        if(extraSeparatorSpacing > 0)
        {
            zv_mainLayout->insertSpacing(actionList.count(), extraSeparatorSpacing);
        }
    }
}
//===========================================================
void ZHorizontalDashBoard::zp_appendWidgetToDashboard(QWidget* widget,
                                                      AuxWidgetPositon position,
                                                      Qt::AlignmentFlag alignment,
                                                      int extraSeparatorSpacing)
{
    int insertIndex;
    if(position == AWP_LEFT_OF_BUTTONS)
    {
        insertIndex = 0; // 0 - log slider
    }
    else
    {
        insertIndex = zv_mainLayout->count();
        if(extraSeparatorSpacing > 0)
        {
            zv_mainLayout->addSpacing(extraSeparatorSpacing);
        }
    }

    zv_mainLayout->insertWidget(insertIndex, widget, alignment);
    if(position == AWP_LEFT_OF_BUTTONS)
    {
        if(alignment == Qt::AlignLeft)
        {
            zv_mainLayout->insertStretch(1, 999999);
        }

        if(extraSeparatorSpacing > 0)
        {
            zv_mainLayout->insertSpacing(1, extraSeparatorSpacing);
        }
    }
}
//===========================================================
int ZHorizontalDashBoard::zp_distortionValue()
{
    if(zv_distortionSlider == 0)
    {
        return 0;
    }
    return zv_distortionSlider->value();
}
//===========================================================
void ZHorizontalDashBoard::zp_setDistortionSliderVisible(bool visible)
{
    if(zv_distortionSlider == 0)
    {
        return;
    }
    zv_distortionSlider->setVisible(visible);
}
//===========================================================
void ZHorizontalDashBoard::timerEvent(QTimerEvent* event)
{
    zh_killTimer();
    if(!zv_plotGraphicsView)
    {
        return;
    }

    zv_plotGraphicsView->zp_scale(zv_orientation, zv_increase);
    zv_timer = startTimer(100);
}
//===========================================================
void ZHorizontalDashBoard::zh_createConnections()
{
    connect(zv_increaseVerticalButton, &QToolButton::pressed,
            this, &ZHorizontalDashBoard::zh_increaseVertical);
    connect(zv_increaseHorizontalButton, &QToolButton::pressed,
            this, &ZHorizontalDashBoard::zh_increaseHorizontal);
    connect(zv_decreaseVerticalButton, &QToolButton::pressed,
            this, &ZHorizontalDashBoard::zh_decreaseVertical);
    connect(zv_decreaseHorizontalButton, &QToolButton::pressed,
            this, &ZHorizontalDashBoard::zh_decreaseHorizontal);

    connect(zv_increaseVerticalButton, &QToolButton::released,
            this, &ZHorizontalDashBoard::zh_killTimer);
    connect(zv_increaseHorizontalButton, &QToolButton::released,
            this, &ZHorizontalDashBoard::zh_killTimer);
    connect(zv_decreaseVerticalButton, &QToolButton::released,
            this, &ZHorizontalDashBoard::zh_killTimer);
    connect(zv_decreaseHorizontalButton, &QToolButton::released,
            this, &ZHorizontalDashBoard::zh_killTimer);

    connect(zv_hideGridButton, SIGNAL(toggled(bool)),
            this, SLOT(zh_hideGrid(bool)));
    connect(zv_fitViewInSceneButton, &QToolButton::clicked,
            this, &ZHorizontalDashBoard::zh_fitViewInScene);

    connect(zv_distortionSlider, &QSlider::valueChanged,
            this, &ZHorizontalDashBoard::zg_distortionFactorChanged);
}
//===========================================================
void ZHorizontalDashBoard::zh_killTimer()
{
    if(zv_timer)
    {
        killTimer(zv_timer);
        zv_timer = 0;
    }
}
//===========================================================
void ZHorizontalDashBoard::zh_increaseVertical()
{
    if(zv_plotGraphicsView == 0)
    {
        return;
    }
    zv_plotGraphicsView->zp_scale(Qt::Vertical, true);
    zv_orientation = Qt::Vertical;
    zv_increase = true;
    zh_startTimer();
}
//===========================================================
void ZHorizontalDashBoard::zh_increaseHorizontal()
{
    if(zv_plotGraphicsView == 0)
    {
        return;
    }
    zv_plotGraphicsView->zp_scale(Qt::Horizontal, true);
    zv_orientation = Qt::Horizontal;
    zv_increase = true;
    zh_startTimer();
}
//===========================================================
void ZHorizontalDashBoard::zh_decreaseVertical()
{
    if(zv_plotGraphicsView == 0)
    {
        return;
    }
    zv_plotGraphicsView->zp_scale(Qt::Vertical, false);
    zv_orientation = Qt::Vertical;
    zv_increase = false;
    zh_startTimer();
}
//===========================================================
void ZHorizontalDashBoard::zh_decreaseHorizontal()
{
    if(zv_plotGraphicsView == 0)
    {
        return;
    }
    zv_plotGraphicsView->zp_scale(Qt::Horizontal, false);
    zv_orientation = Qt::Horizontal;
    zv_increase = false;
    zh_startTimer();
}
//===========================================================
void ZHorizontalDashBoard::zh_fitViewInScene() const
{
    if(zv_plotGraphicsView == 0 || zv_plotGraphicsScene == 0)
    {
        return;
    }

    QRectF itemBoundingRect = zv_plotGraphicsScene->itemsBoundingRect();
    zv_plotGraphicsScene->setSceneRect(itemBoundingRect);
    QRectF rectToFit;
    if(itemBoundingRect.isValid())
    {
        rectToFit = itemBoundingRect;
    }
    else
    {
        rectToFit = zv_plotGraphicsScene->sceneRect();
    }
    zv_plotGraphicsView->zp_fitInView(rectToFit);
    //zv_plotGraphicsView->ensureVisible(rectToFit, 2, 2);
}
//===========================================================
void ZHorizontalDashBoard::zh_hideGrid(bool toggled) const
{
    if(zv_plotGraphicsView == 0)
    {
        return;
    }
    zv_plotGraphicsView->zp_setGridHidden(toggled);
}
//===========================================================
void ZHorizontalDashBoard::zh_startTimer()
{
    zv_timer = startTimer(300);
}
//===========================================================
