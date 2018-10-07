//======================================================
#include "ZEnergyCalibrationDialog.h"
#include "ZGeneral.h"
#include "ZBaseTableWidget.h"
#include "ZSpeSpectrum.h"
#include "ZPeakWindowTableModel.h"

#include <limits>
#include <QDialogButtonBox>
#include <QFrame>
#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QSettings>
#include <QSpinBox>
#include <QVBoxLayout>

//======================================================
ZEnergyCalibrationDialog::ZEnergyCalibrationDialog(QWidget *parent)
    : QWidget(parent)
{
    setWindowTitle(tr("Energy calbration"));
    setAttribute(Qt::WA_DeleteOnClose);
    setWindowFlags(Qt::WindowStaysOnTopHint | Qt::Tool);
    zh_createComponents();
    zh_createConnections();
    zh_restoreSettings();
}
//======================================================
ZEnergyCalibrationDialog::~ZEnergyCalibrationDialog()
{
    zh_saveSettings();
}
//======================================================
void ZEnergyCalibrationDialog::zh_createComponents()
{
    // invisible components
    zv_peakWindowTableModel = new ZPeakWindowTableModel(this);

    // layouts
    QVBoxLayout* mainLayout = new QVBoxLayout(this);
    setLayout(mainLayout);

    // control widget
    mainLayout->addWidget(zh_createControlWidget());

    // sparator line
    QFrame* line = new QFrame(this);
    line->setFrameStyle(QFrame::HLine | QFrame::Sunken);
    line->setLineWidth(1);
    mainLayout->addWidget(line);

    // bottom
    QDialogButtonBox* bottomButtonBox = new QDialogButtonBox(Qt::Horizontal, this);
    mainLayout->addWidget(bottomButtonBox);

    zv_okButton = new QPushButton(this);
    zv_okButton->setText(tr("OK"));
    bottomButtonBox->addButton(zv_okButton, QDialogButtonBox::ActionRole);

    zv_cancelButton = new QPushButton(this);
    zv_cancelButton->setText(tr("Cancel"));
    bottomButtonBox->addButton(zv_cancelButton, QDialogButtonBox::ActionRole);

    zv_cancelButton->setFocus();
}
//======================================================
QWidget* ZEnergyCalibrationDialog::zh_createControlWidget()
{
    QWidget* controlWidget = new QWidget();

    QVBoxLayout* mainLayout = new QVBoxLayout(this);
    mainLayout->setMargin(0);
    controlWidget->setLayout(mainLayout);

    // gain factor spinbox
    QHBoxLayout* spinBoxLayout = new QHBoxLayout(controlWidget);
    QLabel* label = new QLabel(this);
    label->setText(glCreateCaption(tr("Gain factor:")));
    spinBoxLayout->addWidget(label);

    zv_gainFactorSpinBox = new QSpinBox(this);
    zv_gainFactorSpinBox->setMinimum(0);
    zv_gainFactorSpinBox->setMaximum(std::numeric_limits<quint8>::max());
    spinBoxLayout->addWidget(zv_gainFactorSpinBox);
    spinBoxLayout->addStretch();
    mainLayout->addLayout(spinBoxLayout);

    zv_peakTableWidget = new ZBaseTableWidget(this);
    mainLayout->addWidget(zv_peakTableWidget);

    return controlWidget;
}
//======================================================
void ZEnergyCalibrationDialog::zh_createConnections()
{
    connect(zv_cancelButton, &QPushButton::clicked,
            this, &ZEnergyCalibrationDialog::close);
    connect(zv_okButton, &QPushButton::clicked,
            this, &ZEnergyCalibrationDialog::zh_calculateSaveAndClose);

    connect(zv_okButton, &QPushButton::clicked,
            this, &ZEnergyCalibrationDialog::zh_calculateSaveAndClose);
    connect(zv_okButton, &QPushButton::clicked,
            this, &ZEnergyCalibrationDialog::zh_calculateSaveAndClose);

    zv_peakTableWidget->zp_setModel(zv_peakWindowTableModel);
    connect(zv_peakWindowTableModel, &ZPeakWindowTableModel::zg_inquiryCurrentIndex,
            zv_peakTableWidget, &ZBaseTableWidget::zp_currentIndex);
    connect(zv_peakWindowTableModel, &ZPeakWindowTableModel::zg_inquiryCurrentVisibleSceneRect,
            this, &ZEnergyCalibrationDialog::zg_inquiryCurrentVisibleSceneRect);

    zv_peakTableWidget->zp_appendButtonActions(zv_peakWindowTableModel->zp_buttonActions());
    zv_peakTableWidget->zp_appendContextActions(zv_peakWindowTableModel->zp_contextActions());
}
//======================================================
void ZEnergyCalibrationDialog::zh_restoreSettings()
{
    QSettings settings;
    QVariant vData;
    settings.beginGroup(glAppVersion);
    settings.beginGroup("EnergyCalibratorWidget");

    vData = settings.value("widgetGeometry");
    if(vData.isValid() && !vData.isNull() && vData.canConvert<QByteArray>())
    {
        this->restoreGeometry(vData.value<QByteArray>());
    }

    settings.endGroup();
    settings.endGroup();
}
//======================================================
void ZEnergyCalibrationDialog::zh_saveSettings() const
{
    QSettings settings;
    settings.beginGroup(glAppVersion);
    settings.beginGroup("EnergyCalibratorWidget");

    settings.setValue("widgetGeometry", QVariant::fromValue<QByteArray>(this->saveGeometry()));
    //settings.setValue("splitterGeometry", QVariant::fromValue<QByteArray>(zv_tableSplitter->saveState()));

    settings.endGroup();
    settings.endGroup();
}
//======================================================
void ZEnergyCalibrationDialog::zp_setMeasuringConditionsAndSpectrum(quint8 gainFactor,
                                                                    int exposition,
                                                                    const ZSpeSpectrum* spectrum)
{

    QString spectrumName = spectrum ? spectrum->zp_name() : "NON SPECTRUM";

    // zv_gainFactorSpinBox->setValue(gainFactor);

}
//======================================================
void ZEnergyCalibrationDialog::zh_calculateSaveAndClose()
{

    // close
    close();

}
//======================================================
