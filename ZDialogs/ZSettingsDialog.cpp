//===============================================================
#include "ZSettingsDialog.h"
#include "ZGeneral.h"
#include "ZDashboardSettingsWidget.h"
#include "ZDeviceSettingsWidget.h"
#include "ZCommonSettingsWidget.h"

#include <QDialogButtonBox>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QTabWidget>
#include <QSettings>
// controls
#include <QComboBox>
#include <QLabel>
#include <QPushButton>
//===============================================================
ZSettingsDialog::ZSettingsDialog(QWidget *parent) : QDialog(parent)
{
    setWindowTitle(NS_CommonStrings::glSettings);
    setWindowFlags(Qt::Tool);

    zh_createComponents();
    zh_createConnections();
    zh_restoreSettings();
}
//===============================================================
ZSettingsDialog::~ZSettingsDialog()
{
    zh_saveSettings();
}
//===============================================================
void ZSettingsDialog::zh_appSettings(ZAppSettings &appSettings) const
{
    zv_dashboardSettingsWidget->zp_appSettings(appSettings.zv_dashboardSettings);
    zv_deviceSettingsWidget->zp_appSettings(appSettings.zv_deviceSettings);
}
//===============================================================
void ZSettingsDialog::zh_setAppSettings(const ZAppSettings& appSettings)
{
    zv_dashboardSettingsWidget->zp_setAppSettings(appSettings.zv_dashboardSettings);
    zv_deviceSettingsWidget->zp_setAppSettings(appSettings.zv_deviceSettings);
}
//===============================================================
void ZSettingsDialog::zh_createComponents()
{
    // layout
    QVBoxLayout* mainLayout = new QVBoxLayout(this);
    setLayout(mainLayout);

    // tabs
    QTabWidget* mainTabWidget = new QTabWidget(this);

    // common settings
    zv_commonSettingsWidget = new ZCommonSettingsWidget(this);
    mainTabWidget->addTab(zv_commonSettingsWidget, tr("Common"));

    // device settings widget
    zv_deviceSettingsWidget = new ZDeviceSettingsWidget(this);
    mainTabWidget->addTab(zv_deviceSettingsWidget, tr("Device"));

    // dashboard
    zv_dashboardSettingsWidget = new ZDashboardSettingsWidget;
    mainTabWidget->addTab(zv_dashboardSettingsWidget, tr("View"));

    // add tabwidget to main layout
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

    //    zv_applyButton = new QPushButton(NS_Buttons::glButtonApply, this);
    //    buttonBox->addButton(zv_applyButton, QDialogButtonBox::ActionRole);

    zv_cancelButton = new QPushButton(NS_Buttons::glButtonCancel, this);
    buttonBox->addButton(zv_cancelButton, QDialogButtonBox::ActionRole);

}
//===============================================================
void ZSettingsDialog::zh_createConnections()
{
    connect(zv_okButton, &QPushButton::clicked,
            this, &ZSettingsDialog::zh_onOkClick);
//    connect(zv_applyButton, &QPushButton::clicked,
//            this, &ZSettingsDialog::zh_onApplyClick);
    connect(zv_cancelButton, &QPushButton::clicked,
            this, &ZSettingsDialog::reject);
    connect(zv_dashboardSettingsWidget, &ZDashboardSettingsWidget::zg_settingsChanged,
            this, &ZSettingsDialog::zh_onApplyClick);

}
//===============================================================
void ZSettingsDialog::zh_saveSettings()
{
    QSettings settings;
    settings.beginGroup(glAppVersion);
    settings.beginGroup("SettingsDialog");

    settings.setValue("dialogGeometry", QVariant::fromValue<QByteArray>(this->saveGeometry()));
    //settings.setValue("splitterGeometry", QVariant::fromValue<QByteArray>(zv_tableSplitter->saveState()));

    settings.endGroup();
    settings.endGroup();
}
//===============================================================
void ZSettingsDialog::zh_restoreSettings()
{
    QSettings settings;
    QVariant vData;
    settings.beginGroup(glAppVersion);
    settings.beginGroup("SettingsDialog");

    vData = settings.value("dialogGeometry");
    if(vData.isValid() && !vData.isNull() && vData.canConvert<QByteArray>())
    {
        this->restoreGeometry(vData.value<QByteArray>());
    }

    settings.endGroup();
    settings.endGroup();
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
