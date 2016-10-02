//===============================================================
#include "ZSettingsDialog.h"
#include "ZGLConstantsAndDefines.h"
#include "ZDashboardSettingsWidget.h"

#include <QDialogButtonBox>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QTabWidget>

// controls
#include <QComboBox>
#include <QLabel>
#include <QPushButton>
//===============================================================
ZSettingsDialog::ZSettingsDialog(QWidget *parent) : QDialog(parent)
{
    setWindowTitle(NS_CommonStrings::glSettings);

    zh_createComponents();
    zh_createConnections();
}
//===============================================================
void ZSettingsDialog::zh_appSettings(ZAppSettings &appSettings) const
{
    zv_dashboardSettingsWidget->zp_appSettings(appSettings.zv_dashboardSettings);
}
//===============================================================
void ZSettingsDialog::zh_setAppSettings(const ZAppSettings& appSettings)
{
    zv_dashboardSettingsWidget->zp_setAppSettings(appSettings.zv_dashboardSettings);
}
//===============================================================
void ZSettingsDialog::zh_createComponents()
{
    // layout
    QVBoxLayout* mainLayout = new QVBoxLayout(this);
    setLayout(mainLayout);

    // tabs
    QTabWidget* mainTabWidget = new QTabWidget(this);
    // dashboard
    zv_dashboardSettingsWidget = new ZDashboardSettingsWidget;
    mainTabWidget->addTab(zv_dashboardSettingsWidget, NS_CommonStrings::glCommon);
    mainLayout->addWidget(mainTabWidget);

    // Basement
    // sparator line
    QFrame* line = new QFrame(this);
    line->setFrameStyle(QFrame::HLine | QFrame::Sunken);
    line->setLineWidth(1);
    mainLayout->addWidget(line);

    QDialogButtonBox* buttonBox = new QDialogButtonBox(this);
    mainLayout->addWidget(buttonBox);

    // buttons
    zv_okButton = new QPushButton(NS_Buttons::glButtonOk, this);
    buttonBox->addButton(zv_okButton, QDialogButtonBox::ActionRole);

    zv_applyButton = new QPushButton(NS_Buttons::glButtonApply, this);
    buttonBox->addButton(zv_applyButton, QDialogButtonBox::ActionRole);

    zv_cancelButton = new QPushButton(NS_Buttons::glButtonCancel, this);
    buttonBox->addButton(zv_cancelButton, QDialogButtonBox::ActionRole);

}
//===============================================================
void ZSettingsDialog::zh_createConnections()
{
    connect(zv_okButton, &QPushButton::clicked,
            this, &ZSettingsDialog::zh_onOkClick);
    connect(zv_applyButton, &QPushButton::clicked,
            this, &ZSettingsDialog::zh_onApplyClick);
    connect(zv_cancelButton, &QPushButton::clicked,
            this, &ZSettingsDialog::reject);
    connect(zv_dashboardSettingsWidget, &ZDashboardSettingsWidget::zg_settingsChanged,
            this, &ZSettingsDialog::zh_onApplyClick);

}
//===============================================================
void ZSettingsDialog::zh_onOkClick()
{
    accept();
}
//===============================================================
void ZSettingsDialog::zh_onApplyClick()
{
    ZAppSettings appSettings;
    zh_appSettings(appSettings);
    emit zg_applySettings(appSettings);
}
//===============================================================
