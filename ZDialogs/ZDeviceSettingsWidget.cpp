//=======================================================
#include "ZDeviceSettingsWidget.h"
#include "ZGeneral.h"
#include "ZUralAdcDeviceConnector.h"

#include <QApplication>
#include <QDir>
#include <QFileDialog>
#include <QFileInfo>
#include <QGridLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QComboBox>
#include <QPushButton>
#include <QSpinBox>
#include <QVBoxLayout>
//=======================================================
ZDeviceSettingsWidget::ZDeviceSettingsWidget(QWidget *parent)
    : QWidget(parent)
{
    zh_createComponents();
    zh_createConnections();
}
//=======================================================
void ZDeviceSettingsWidget::zp_setAppSettings(const ZDeviceSettings& settings)
{
    zv_deviceSerialLineEdit->setText(settings.zv_deviceSerial);
    zv_deviceSampleQuantitySpinBox->setValue(settings.zv_deviceSampleQuantity);
    zv_deviceChannelQuantitySpinBox->setValue(settings.zv_deviceChannelQuantity);
    zv_deviceLibraryFilePathLineEdit->setText(settings.zv_libraryFileName);

    QString defaultADCString = settings.zv_defaultConnectionADC.isEmpty() ?
                zv_manualConnectionstring : settings.zv_defaultConnectionADC;
    zv_defaultADCType->setCurrentText(defaultADCString);
    zv_expositionStarterComboBox->setCurrentIndex(settings.zv_expositionStarter);
    zv_sampleShiftImpulseDurationSpinBox->setValue(settings.zv_sampleShiftImpulseDuration);
    zv_expositionDelayTimeSpinBox->setValue(settings.zv_expositionDelayTime);

}
//=======================================================
void ZDeviceSettingsWidget::zp_appSettings(ZDeviceSettings& settings)
{
    settings.zv_deviceSerial = zv_deviceSerialLineEdit->text();
    settings.zv_deviceSampleQuantity = zv_deviceSampleQuantitySpinBox->value();
    settings.zv_deviceChannelQuantity = zv_deviceChannelQuantitySpinBox->value();
    settings.zv_libraryFileName = zv_deviceLibraryFilePathLineEdit->text();
    QString defaultADCString = zv_defaultADCType->currentText() == zv_manualConnectionstring ?
                QString() : zv_defaultADCType->currentText();
    settings.zv_defaultConnectionADC = defaultADCString;
    settings.zv_expositionStarter = zv_expositionStarterComboBox->currentIndex();
    settings.zv_sampleShiftImpulseDuration = zv_sampleShiftImpulseDurationSpinBox->value();
    settings.zv_expositionDelayTime = zv_expositionDelayTimeSpinBox->value();

}
//=======================================================
void ZDeviceSettingsWidget::zh_createComponents()
{
    QVBoxLayout* mainLayout = new QVBoxLayout(this);
    setLayout(mainLayout);

    // Serial number
    QLabel* captionLabel = new QLabel(this);
    captionLabel->setText(glCreateCaption(tr("Device serial number:")));
    mainLayout->addWidget(captionLabel);

    QHBoxLayout* serialNumberLayout = new  QHBoxLayout(this);
    mainLayout->addLayout(serialNumberLayout);

    zv_deviceSerialLineEdit = new QLineEdit(this);
    serialNumberLayout->addWidget(zv_deviceSerialLineEdit);
    serialNumberLayout->addStretch();

    // sample quantity
    captionLabel = new QLabel(this);
    captionLabel->setText(glCreateCaption(tr("Device sample quantity:")));
    mainLayout->addWidget(captionLabel);

    QHBoxLayout* sampleNumberLayout = new  QHBoxLayout(this);
    mainLayout->addLayout(sampleNumberLayout);

    zv_deviceSampleQuantitySpinBox = new QSpinBox(this);
    zv_deviceSampleQuantitySpinBox->setMinimum(1);
    zv_deviceSampleQuantitySpinBox->setMaximum(999);
    sampleNumberLayout->addWidget(zv_deviceSampleQuantitySpinBox);
    sampleNumberLayout->addStretch();


    // channel quantity
    captionLabel = new QLabel(this);
    captionLabel->setText(glCreateCaption(tr("Device channel quantity:")));
    mainLayout->addWidget(captionLabel);

    QHBoxLayout* channelQuantityLayout = new  QHBoxLayout(this);
    mainLayout->addLayout(channelQuantityLayout);

    zv_deviceChannelQuantitySpinBox = new QSpinBox(this);
    zv_deviceChannelQuantitySpinBox->setMinimum(1);
    zv_deviceChannelQuantitySpinBox->setMaximum(99999);
    channelQuantityLayout->addWidget(zv_deviceChannelQuantitySpinBox);
    channelQuantityLayout->addStretch();

    // device library file path
    captionLabel = new QLabel(this);
    captionLabel->setText(glCreateCaption(tr("ADC library file:")));
    mainLayout->addWidget(captionLabel);

    QHBoxLayout* deviceLibraryLayout = new  QHBoxLayout(this);
    mainLayout->addLayout(deviceLibraryLayout);

    zv_deviceLibraryFilePathLineEdit = new QLineEdit(this);
    deviceLibraryLayout->addWidget(zv_deviceLibraryFilePathLineEdit);

    zv_deviceLibraryFileBrowseButton = new QPushButton(this);
    zv_deviceLibraryFileBrowseButton->setText(tr("Browse..."));
    deviceLibraryLayout->addWidget(zv_deviceLibraryFileBrowseButton);
    deviceLibraryLayout->addStretch();


    // default ADC type
    captionLabel = new QLabel(this);
    captionLabel->setText(glCreateCaption(tr("Connect to ADC by application start:")));
    mainLayout->addWidget(captionLabel);

    QHBoxLayout* defaultADCTypeLayout = new  QHBoxLayout(this);
    mainLayout->addLayout(defaultADCTypeLayout);

    zv_defaultADCType = new QComboBox(this);

    QStringList itemStringList;
    itemStringList.append(zv_manualConnectionstring);
    itemStringList += ZUralAdcDeviceConnector::zp_deviceNameList();
    zv_defaultADCType->addItems(itemStringList);
    defaultADCTypeLayout->addWidget(zv_defaultADCType);
    defaultADCTypeLayout->addStretch();

    // exposition starte
    captionLabel = new QLabel(this);
    captionLabel->setText(glCreateCaption(tr("Start exposition by:")));
    mainLayout->addWidget(captionLabel);

    QHBoxLayout* expositionStarterLayout = new  QHBoxLayout(this);
    mainLayout->addLayout(expositionStarterLayout);

    zv_expositionStarterComboBox = new QComboBox(this);

    itemStringList.clear();
    itemStringList.append(tr("Application"));
    itemStringList.append(tr("Device button"));
    itemStringList.append(tr("Application or device button"));

    zv_expositionStarterComboBox->addItems(itemStringList);
    expositionStarterLayout->addWidget(zv_expositionStarterComboBox);
    expositionStarterLayout->addStretch();

    // Sample shift impulse duration
    captionLabel = new QLabel(this);
    captionLabel->setText(glCreateCaption(tr("Sample shift impulse duration (ms):")));
    mainLayout->addWidget(captionLabel);

    QHBoxLayout* sampleShiftImpulseDurationLayout = new  QHBoxLayout(this);
    mainLayout->addLayout(sampleShiftImpulseDurationLayout);

    zv_sampleShiftImpulseDurationSpinBox = new QSpinBox(this);
    zv_sampleShiftImpulseDurationSpinBox->setMinimum(0);
    zv_sampleShiftImpulseDurationSpinBox->setMaximum(1000);

    sampleShiftImpulseDurationLayout->addWidget(zv_sampleShiftImpulseDurationSpinBox);
    sampleShiftImpulseDurationLayout->addStretch();

    // Exposition start delay time
    captionLabel = new QLabel(this);
    captionLabel->setText(glCreateCaption(tr("Exposition start delay time (s):")));
    mainLayout->addWidget(captionLabel);

    QHBoxLayout* expositionStartDelayTimeLayout = new  QHBoxLayout(this);
    mainLayout->addLayout(expositionStartDelayTimeLayout);

    zv_expositionDelayTimeSpinBox = new QSpinBox(this);
    zv_expositionDelayTimeSpinBox->setMinimum(0);
    zv_expositionDelayTimeSpinBox->setMaximum(99);

    expositionStartDelayTimeLayout->addWidget(zv_expositionDelayTimeSpinBox);
    expositionStartDelayTimeLayout->addStretch();


    // end
    mainLayout->addStretch();
}
//=======================================================
void ZDeviceSettingsWidget::zh_createConnections()
{
    connect(zv_deviceLibraryFileBrowseButton, &QPushButton::clicked,
            this, &ZDeviceSettingsWidget::zh_onLibraryFileBrowseClick);
}
//=======================================================
void ZDeviceSettingsWidget::zh_onLibraryFileBrowseClick()
{
    QString dirString;
    if(zv_deviceLibraryFilePathLineEdit->text().isEmpty() )
    {
        dirString = qApp->applicationDirPath();
    }
    else
    {
        QFileInfo fileInfo =  QFileInfo(zv_deviceLibraryFilePathLineEdit->text());
        dirString = fileInfo.absoluteFilePath();
    }
    QString fileName = QFileDialog::getOpenFileName(this, tr("Select ADC library"), dirString,
                                                    tr("Dynamic Library Files (*.dll)"));

    if(fileName.isEmpty())
    {
        return;
    }

    zv_deviceLibraryFilePathLineEdit->setText(fileName);
}
//=======================================================
