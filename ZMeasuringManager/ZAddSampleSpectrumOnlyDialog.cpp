//======================================================
#include "ZAddSampleSpectrumOnlyDialog.h"
#include "ZGeneral.h"

#include <QApplication>
#include <QDebug>
#include <QDialogButtonBox>
#include <QFrame>
#include <QHBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QMessageBox>
#include <QPushButton>
#include <QSettings>
#include <QSpinBox>
#include <QVBoxLayout>
//======================================================
ZAddSampleSpectrumOnlyDialog::ZAddSampleSpectrumOnlyDialog(QWidget* parent)
    : QDialog(parent)
{
    setWindowTitle(tr("Samples"));
    setWindowFlags(Qt::Tool);

    zv_saveControlValuesFlag = false;
    zh_createActions();
    zh_createComponents();
    zh_createConnections();
    zh_restoreSettings();
}
//======================================================
ZAddSampleSpectrumOnlyDialog::~ZAddSampleSpectrumOnlyDialog()
{
    zh_saveSettings();
}
//======================================================
void ZAddSampleSpectrumOnlyDialog::closeEvent(QCloseEvent*)
{
    
}
//======================================================
void ZAddSampleSpectrumOnlyDialog::zh_createActions() {}
//======================================================
void ZAddSampleSpectrumOnlyDialog::zh_createComponents()
{
    QLabel* label;
    QHBoxLayout* levelLayout;

    QVBoxLayout* mainLayout = new QVBoxLayout;
    setLayout(mainLayout);
    // gain factor
    label = new QLabel(this);
    label->setText(glCreateCaption(tr("Gain factor:")));
    mainLayout->addWidget(label);

    levelLayout = new QHBoxLayout;
    mainLayout->addLayout(levelLayout);

    zv_gainFactorSpinBox = new QSpinBox(this);
    zv_gainFactorSpinBox->setObjectName("GainFactorSpinBox");
    zv_gainFactorSpinBox->setRange(1, glMaxGainFactor);
    label->setBuddy(zv_gainFactorSpinBox);
    levelLayout->addWidget(zv_gainFactorSpinBox);
    levelLayout->addStretch();

    // exposition
    label = new QLabel(this);
    label->setText(glCreateCaption(tr("Exposition:")));
    mainLayout->addWidget(label);

    levelLayout = new QHBoxLayout;
    mainLayout->addLayout(levelLayout);

    zv_expositionSpinBox = new QSpinBox(this);
    zv_expositionSpinBox->setRange(1, 99999);
    zv_expositionSpinBox->setObjectName("ExpositionSpinBox");
    label->setBuddy(zv_expositionSpinBox);
    levelLayout->addWidget(zv_expositionSpinBox);
    levelLayout->addStretch();

    // Sample name
    label = new QLabel(this);
    label->setText(glCreateCaption(tr("Sample name template:")));
    mainLayout->addWidget(label);

    levelLayout = new QHBoxLayout;
    mainLayout->addLayout(levelLayout);

    zv_sampleNameLineEdit = new QLineEdit(this);
    zv_sampleNameLineEdit->setText(zv_defaultSampleTemplateString);
    zv_sampleNameLineEdit->setObjectName("SampleNameLineEdit");
    levelLayout->addWidget(zv_sampleNameLineEdit);
    levelLayout->addStretch();

    // task quantity
    label = new QLabel(this);
    label->setText(glCreateCaption(tr("Sample quantity:")));
    mainLayout->addWidget(label);

    levelLayout = new QHBoxLayout;
    mainLayout->addLayout(levelLayout);
    zv_quantitySpinBox = new QSpinBox(this);
    zv_quantitySpinBox->setRange(1, 999);
    levelLayout->addWidget(zv_quantitySpinBox);
    levelLayout->addStretch();
    mainLayout->addStretch();

    int minWidth = zv_sampleNameLineEdit->sizeHint().width();
    zv_gainFactorSpinBox->setMinimumWidth(minWidth);
    zv_expositionSpinBox->setMinimumWidth(minWidth);
    zv_quantitySpinBox->setMinimumWidth(minWidth);

    // basement
    // Separator
    QFrame* line = new QFrame(this);
    line->setFrameStyle(QFrame::HLine | QFrame::Sunken);
    line->setLineWidth(1);
    mainLayout->addWidget(line);

    QDialogButtonBox* buttonBox = new QDialogButtonBox(this);
    mainLayout->addWidget(buttonBox);

    // buttons
    zv_okButton = new QPushButton(NS_Buttons::glButtonOk, this);
    buttonBox->addButton(zv_okButton, QDialogButtonBox::ActionRole);

    zv_cancelButton = new QPushButton(NS_Buttons::glButtonCancel, this);
    buttonBox->addButton(zv_cancelButton, QDialogButtonBox::ActionRole);
}
//======================================================
void ZAddSampleSpectrumOnlyDialog::zh_createConnections()
{
    connect(zv_okButton,
            &QPushButton::clicked,
            this,
            &ZAddSampleSpectrumOnlyDialog::zh_onOkButtonClick);
    connect(zv_cancelButton,
            &QPushButton::clicked,
            this,
            &ZAddSampleSpectrumOnlyDialog::reject);
}
//======================================================
void ZAddSampleSpectrumOnlyDialog::zh_restoreSettings()
{
    QSettings settings;
    QVariant vData;
    settings.beginGroup(qApp->applicationVersion());
    settings.beginGroup("AddSampleForSpectraDialog");

    vData = settings.value("dialogGeometry");
    if (vData.isValid() && !vData.isNull() && vData.canConvert<QByteArray>())
    {
        this->restoreGeometry(vData.value<QByteArray>());
    }

    vData = settings.value(zv_gainFactorSpinBox->objectName());
    if (vData.isValid() && !vData.isNull() && vData.canConvert<int>())
    {
        zv_gainFactorSpinBox->setValue(vData.toInt());
    }

    vData = settings.value(zv_expositionSpinBox->objectName());
    if (vData.isValid() && !vData.isNull() && vData.canConvert<int>())
    {
        zv_expositionSpinBox->setValue(vData.toInt());
    }

    settings.endGroup();
    settings.endGroup();
}
//======================================================
void ZAddSampleSpectrumOnlyDialog::zh_saveSettings() const
{
    QSettings settings;
    settings.beginGroup(qApp->applicationVersion());
    settings.beginGroup("AddSampleForSpectraDialog");

    settings.setValue("dialogGeometry",
                      QVariant::fromValue<QByteArray>(this->saveGeometry()));

    if (zv_saveControlValuesFlag)
    {
        settings.setValue(zv_gainFactorSpinBox->objectName(),
                          zv_gainFactorSpinBox->value());
        settings.setValue(zv_expositionSpinBox->objectName(),
                          zv_expositionSpinBox->value());
    }

    settings.endGroup();
    settings.endGroup();
}
//======================================================
void ZAddSampleSpectrumOnlyDialog::zh_onOkButtonClick()
{
    if (zh_checkData())
    {
        zv_saveControlValuesFlag = true;
        accept();
    }
}
//======================================================
bool ZAddSampleSpectrumOnlyDialog::zh_checkData() const
{
    return true;
}
//======================================================
int ZAddSampleSpectrumOnlyDialog::zp_gainFactor() const
{
    return zv_gainFactorSpinBox->value();
}
//======================================================
int ZAddSampleSpectrumOnlyDialog::zp_exposition() const
{
    return zv_expositionSpinBox->value();
}
//======================================================
QString ZAddSampleSpectrumOnlyDialog::zp_sampleNameTemplate() const
{
    return zv_sampleNameLineEdit->text();
}
//======================================================
int ZAddSampleSpectrumOnlyDialog::zp_sampleQuantity() const
{
    return zv_quantitySpinBox->value();
}
//======================================================
