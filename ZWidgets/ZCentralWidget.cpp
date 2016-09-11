//==========================================================
#include "ZCentralWidget.h"
#include "ZGLConstantsAndDefines.h"
#include "ZDashboard.h"

#include <QVBoxLayout>
#include <QHBoxLayout>
//==========================================================
ZCentralWidget::ZCentralWidget(QWidget * tableWidget, QWidget *parent) : QWidget(parent)
{
    zv_tableWidget = tableWidget;
    zh_createComponents();
    zh_createConnections();
    zh_rebuildLayout();
}
//==========================================================
void ZCentralWidget::zp_applyAppSettings(const ZAppSettings& appSettings)
{
    zv_dashboard->zp_applyDashboardSettings(appSettings.zv_dashboardSettings);
    zh_rebuildLayout();
}
//==========================================================
void ZCentralWidget::zp_setTableWidget(QWidget* newTableWidget, QWidget*& previousTableWidget)
{
    if(newTableWidget == zv_tableWidget)
    {
        return;
    }

    // release old tableWidget
    previousTableWidget = zv_tableWidget;
    if(zv_tableWidget != 0)
    {
        zv_tableWidget->setParent(0);
    }

    // set new one and rebuild
    zv_tableWidget = newTableWidget;
    zh_rebuildLayout();
}
//==========================================================
void ZCentralWidget::zh_createComponents()
{
    // dashboard
    zv_dashboard = new ZDashboard;
}
//==========================================================
void ZCentralWidget::zh_createConnections()
{

}
//==========================================================
void ZCentralWidget::zh_rebuildLayout()
{
    // old layout
    QLayout* oldMainLayout = layout();
    // release widgets

//    oldMainLayout->removeWidget(zv_dashboard);
//    oldMainLayout->removeWidget(zv_tableWidget);

    // new layout
    ZDashboardSettings::DashboardLocation dashboardLocation =  zv_dashboard->zp_location();
    QLayout* newMainLayout;
    if(dashboardLocation == ZDashboardSettings::DBL_LEFT)
    {
        zv_dashboard->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Expanding);
        newMainLayout = new QHBoxLayout;
        newMainLayout->addWidget(zv_dashboard);
        newMainLayout->addWidget(zv_tableWidget);
    }
    else if(dashboardLocation == ZDashboardSettings::DBL_RIGHT)
    {
        zv_dashboard->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Expanding);
        newMainLayout = new QHBoxLayout;
        newMainLayout->addWidget(zv_tableWidget);
        newMainLayout->addWidget(zv_dashboard);
    }
    else if(dashboardLocation == ZDashboardSettings::DBL_TOP)
    {
        zv_dashboard->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
        newMainLayout = new QVBoxLayout;
        newMainLayout->addWidget(zv_dashboard);
        newMainLayout->addWidget(zv_tableWidget);
    }
    else //(dashboardLocation == ZDashboardSettings::DBL_BOTTOM)
    {
        zv_dashboard->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
        newMainLayout = new QVBoxLayout;
        newMainLayout->addWidget(zv_tableWidget);
        newMainLayout->addWidget(zv_dashboard);
    }

    if(oldMainLayout != 0)
    {
        delete oldMainLayout;
    }

    setLayout(newMainLayout);
}
//==========================================================
