//==========================================================
#include "ZMeasuringCommonWidget.h"
#include "ZGLConstantsAndDefines.h"
#include "ZDashboard.h"
#include "ZMeasuringResultTableWidget.h"
#include "ZMeasuringSeriesTaskTreeWidget.h"
#include "ZWidgetWithSidebar.h"

#include <QVBoxLayout>
#include <QHBoxLayout>

//==========================================================
ZMeasuringCommonWidget::ZMeasuringCommonWidget(QWidget *parent) : QWidget(parent)
{
    zh_createComponents();
    zh_createConnections();
    zh_rebuildLayout();
}
//==========================================================
void ZMeasuringCommonWidget::zp_applyAppSettings(const ZAppSettings& appSettings)
{
    zv_dashboard->zp_applyDashboardSettings(appSettings.zv_dashboardSettings);
    zh_rebuildLayout();
}
//==========================================================
void ZMeasuringCommonWidget::zp_appendTaskButtonActions(const QList<ZControlAction*>& actionList)
{
    //zv_measuringSeriesTaskWidget->zp_appendButtonActions(actionList);
}
//==========================================================
void ZMeasuringCommonWidget::zp_appendTaskContextMenuActions(const QList<ZControlAction*>& actionList)
{
   // zv_measuringSeriesTaskWidget->zp_appendContextActions(actionList);
}
//==========================================================
void ZMeasuringCommonWidget::zp_appendSampleButtonActions(const QList<ZControlAction *> &actionList)
{
    zv_measuringResultTableWidget->zp_appendButtonActions(actionList);
}
//==========================================================
void ZMeasuringCommonWidget::zp_appendSampleContextMenuActions(const QList<ZControlAction *> &actionList)
{
    zv_measuringResultTableWidget->zp_appendContextActions(actionList);
}
//==========================================================
void ZMeasuringCommonWidget::zp_setMeasuringResultTableModel(QAbstractItemModel* model)
{
    zv_measuringResultTableWidget->zp_setModel(model);
}
//==========================================================
void ZMeasuringCommonWidget::zp_setMeasuringSeriesTaskTreeModel(QAbstractItemModel* model)
{
    //zv_measuringSeriesTaskWidget->zp_setModel(model);
}
//==========================================================
void ZMeasuringCommonWidget::zp_saveSettings() const
{
    //zv_sideBarTableWidget->zp_saveSettings();
}
//==========================================================
void ZMeasuringCommonWidget::zh_createComponents()
{
    // Measuring widget
    // tables
    zv_measuringResultTableWidget = new ZMeasuringResultTableWidget(this);
    zv_measuringResultTableWidget->zp_setCaption(glCreateCaption(tr("Samples:")));
//    zv_measuringSeriesTaskWidget = new ZMeasuringSeriesTaskTreeWidget(this);
//    zv_measuringSeriesTaskWidget->zp_setCaption(glCreateCaption(tr("Series task:")));

    // side bar widget
//    zv_sideBarTableWidget = new ZWidgetWithSidebar("MEASUREMENT_WIDGET", true, this);
//    zv_sideBarTableWidget->setFrameStyle(QFrame::StyledPanel | QFrame::Plain);
//    zv_sideBarTableWidget->setLineWidth(1);
//    zv_sideBarTableWidget->zp_setSidebarWidget(zv_measuringSeriesTaskWidget);
//    zv_sideBarTableWidget->zp_setMainWidget(zv_measuringResultTableWidget);
//    zv_sideBarTableWidget->zp_insertSpacingUpperHideLabel(5);

    // dashboard
    zv_dashboard = new ZDashboard;
}
//==========================================================
void ZMeasuringCommonWidget::zh_createConnections()
{

}
//==========================================================
void ZMeasuringCommonWidget::zh_rebuildLayout()
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
//        newMainLayout->addWidget(zv_sideBarTableWidget);
         newMainLayout->addWidget(zv_measuringResultTableWidget);
    }
    else if(dashboardLocation == ZDashboardSettings::DBL_RIGHT)
    {
        zv_dashboard->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Expanding);
        newMainLayout = new QHBoxLayout;
        //newMainLayout->addWidget(zv_sideBarTableWidget);
        newMainLayout->addWidget(zv_measuringResultTableWidget);
        newMainLayout->addWidget(zv_dashboard);
    }
    else if(dashboardLocation == ZDashboardSettings::DBL_TOP)
    {
        zv_dashboard->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
        newMainLayout = new QVBoxLayout;
        newMainLayout->addWidget(zv_dashboard);
        //newMainLayout->addWidget(zv_sideBarTableWidget);
        newMainLayout->addWidget(zv_measuringResultTableWidget);

    }
    else //(dashboardLocation == ZDashboardSettings::DBL_BOTTOM)
    {
        zv_dashboard->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
        newMainLayout = new QVBoxLayout;
        //newMainLayout->addWidget(zv_sideBarTableWidget);
        newMainLayout->addWidget(zv_measuringResultTableWidget);
        newMainLayout->addWidget(zv_dashboard);
    }

    if(oldMainLayout != 0)
    {
        delete oldMainLayout;
    }

    setLayout(newMainLayout);
}
//==========================================================
void ZMeasuringCommonWidget::zp_selectedSampleList(QList<int>& selectedSampleList) const
{
    selectedSampleList = zv_measuringResultTableWidget->zp_selectedRowList();
}
//==========================================================
