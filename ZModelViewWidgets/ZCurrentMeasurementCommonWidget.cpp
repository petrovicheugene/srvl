//==========================================================
#include "ZCurrentMeasurementCommonWidget.h"
#include "ZGLConstantsAndDefines.h"
#include "ZDashboard.h"
#include "ZCurrentMeasurementSampleTableWidget.h"
#include "ZCurrentMeasurementTaskTreeWidget.h"
#include "ZWidgetWithSidebar.h"

#include <QVBoxLayout>
#include <QHBoxLayout>

//==========================================================
ZCurrentMeasurementCommonWidget::ZCurrentMeasurementCommonWidget(QWidget *parent) : QWidget(parent)
{
    zh_createComponents();
    zh_createConnections();
    zh_rebuildLayout();
}
//==========================================================
void ZCurrentMeasurementCommonWidget::zp_applyAppSettings(const ZAppSettings& appSettings)
{
    zv_dashboard->zp_applyDashboardSettings(appSettings.zv_dashboardSettings);
    zh_rebuildLayout();
}
//==========================================================
void ZCurrentMeasurementCommonWidget::zp_appendTaskButtonActions(const QList<ZControlAction*>& actionList)
{
    zv_currentMeasurementTaskWidget->zp_appendButtonActions(actionList);
}
//==========================================================
void ZCurrentMeasurementCommonWidget::zp_appendTaskContextMenuActions(const QList<ZControlAction*>& actionList)
{
    zv_currentMeasurementTaskWidget->zp_appendContextActions(actionList);
}
//==========================================================
void ZCurrentMeasurementCommonWidget::zp_appendSampleButtonActions(const QList<ZControlAction *> &actionList)
{
    zv_currentMeasurementSampleTableWidget->zp_appendButtonActions(actionList);
}
//==========================================================
void ZCurrentMeasurementCommonWidget::zp_appendSampleContextMenuActions(const QList<ZControlAction *> &actionList)
{
    zv_currentMeasurementSampleTableWidget->zp_appendContextActions(actionList);
}
//==========================================================
void ZCurrentMeasurementCommonWidget::zp_setMeasurementSampleTableModel(QAbstractItemModel* model)
{
    zv_currentMeasurementSampleTableWidget->zp_setModel(model);
}
//==========================================================
void ZCurrentMeasurementCommonWidget::zp_setMeasurementTaskTreeModel(QAbstractItemModel* model)
{
    zv_currentMeasurementTaskWidget->zp_setModel(model);
}
//==========================================================
void ZCurrentMeasurementCommonWidget::zp_saveSettings() const
{
    zv_sideBarTableWidget->zp_saveSettings();
}
//==========================================================
void ZCurrentMeasurementCommonWidget::zh_createComponents()
{
    // Measurement widget
    // tables
    zv_currentMeasurementSampleTableWidget = new ZCurrentMeasurementSampleTableWidget(this);
    zv_currentMeasurementSampleTableWidget->zp_setCaption(glCreateCaption(tr("Samples:")));
    zv_currentMeasurementTaskWidget = new ZCurrentMeasurementTaskTreeWidget(this);
    zv_currentMeasurementTaskWidget->zp_setCaption(glCreateCaption(tr("Series task:")));

    // side bar widget
    zv_sideBarTableWidget = new ZWidgetWithSidebar("MEASUREMENT_WIDGET", true, this);
    zv_sideBarTableWidget->setFrameStyle(QFrame::StyledPanel | QFrame::Plain);
    zv_sideBarTableWidget->setLineWidth(1);
    zv_sideBarTableWidget->zp_setSidebarWidget(zv_currentMeasurementTaskWidget);
    zv_sideBarTableWidget->zp_setMainWidget(zv_currentMeasurementSampleTableWidget);
    zv_sideBarTableWidget->zp_insertSpacingUpperHideLabel(5);

    // dashboard
    zv_dashboard = new ZDashboard;
}
//==========================================================
void ZCurrentMeasurementCommonWidget::zh_createConnections()
{

}
//==========================================================
void ZCurrentMeasurementCommonWidget::zh_rebuildLayout()
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
        newMainLayout->addWidget(zv_sideBarTableWidget);
    }
    else if(dashboardLocation == ZDashboardSettings::DBL_RIGHT)
    {
        zv_dashboard->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Expanding);
        newMainLayout = new QHBoxLayout;
        newMainLayout->addWidget(zv_sideBarTableWidget);
        newMainLayout->addWidget(zv_dashboard);
    }
    else if(dashboardLocation == ZDashboardSettings::DBL_TOP)
    {
        zv_dashboard->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
        newMainLayout = new QVBoxLayout;
        newMainLayout->addWidget(zv_dashboard);
        newMainLayout->addWidget(zv_sideBarTableWidget);
    }
    else //(dashboardLocation == ZDashboardSettings::DBL_BOTTOM)
    {
        zv_dashboard->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
        newMainLayout = new QVBoxLayout;
        newMainLayout->addWidget(zv_sideBarTableWidget);
        newMainLayout->addWidget(zv_dashboard);
    }

    if(oldMainLayout != 0)
    {
        delete oldMainLayout;
    }

    setLayout(newMainLayout);
}
//==========================================================
