//=======================================================
#include "ZMeasuringConditionsDialog.h"
#include "ZGLConstantsAndDefines.h"

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QDialogButtonBox>
#include <QFrame>
#include <QLabel>
#include <QPushButton>
#include <QSettings>
#include <QSpinBox>
//=======================================================
ZMeasuringConditionsDialog::ZMeasuringConditionsDialog(QWidget *parent) : QDialog(parent)
{
    zh_createComponents();
    zh_createConnections();
}
//=======================================================
void ZMeasuringConditionsDialog::closeEvent(QCloseEvent* event)
{
    zh_saveSettings();
}
//=======================================================
void ZMeasuringConditionsDialog::zh_restoreSettings()
{
    QSettings settings;
    QVariant vData;
    settings.beginGroup(glAppVersion);
    settings.beginGroup("MeasuringConditionsDialog");

    vData = settings.value("dialogGeometry");
    if(vData.isValid() && !vData.isNull() && vData.canConvert<QByteArray>())
    {
        this->restoreGeometry(vData.value<QByteArray>());
    }

    settings.endGroup();
    settings.endGroup();
}
//=======================================================
void ZMeasuringConditionsDialog::zh_saveSettings() const
{
    QSettings settings;
    settings.beginGroup(glAppVersion);
    settings.beginGroup("MeasuringConditionsDialog");

    settings.setValue("dialogGeometry", QVariant::fromValue<QByteArray>(this->saveGeometry()));
    //settings.setValue("splitterGeometry", QVariant::fromValue<QByteArray>(zv_tableSplitter->saveState()));

    settings.endGroup();
    settings.endGroup();
}
//=======================================================
int ZMeasuringConditionsDialog::zp_gainFactor() const
{
    return zv_gainFactorSpinBox->value();
}
//=======================================================
int ZMeasuringConditionsDialog::zp_exposition() const
{
    return zv_expositionSpinBox->value();
}
//=======================================================
void ZMeasuringConditionsDialog::zh_createComponents()
{
    QVBoxLayout* mainLayout = new QVBoxLayout(this);
    setLayout(mainLayout);

    // gain factor
    QLabel* label = new QLabel(this);
    label->setText(glCreateCaption(tr("Gain factor:")));
    mainLayout->addWidget(label);

    zv_gainFactorSpinBox = new QSpinBox(this);
    zv_gainFactorSpinBox->setRange(1, 255);
    mainLayout->addWidget(zv_gainFactorSpinBox);

    // exposition
    label = new QLabel(this);
    label->setText(glCreateCaption(tr("Exposition (sec.):")));
    mainLayout->addWidget(label);

    zv_expositionSpinBox = new QSpinBox(this);
    zv_expositionSpinBox->setRange(1, 99999);
    mainLayout->addWidget(zv_expositionSpinBox);

    zv_messageLabel = new QLabel(this);
    zv_messageLabel->setWordWrap(true);
    mainLayout->addWidget(zv_messageLabel);

    // Basement
    mainLayout->addStretch();
    // sparator line
    QFrame* line = new QFrame(this);
    line->setFrameStyle(QFrame::HLine | QFrame::Sunken);
    line->setLineWidth(1);
    mainLayout->addWidget(line);

    // button box
    QDialogButtonBox* buttonBox = new QDialogButtonBox(this);
    mainLayout->addWidget(buttonBox);

    // buttons
    zv_okButton = new QPushButton(this);
    zv_okButton->setText(tr("OK"));
    buttonBox->addButton(zv_okButton, QDialogButtonBox::ActionRole);

    zv_cancelButton = new QPushButton(this);
    zv_cancelButton->setText(tr("Cancel"));
    buttonBox->addButton(zv_cancelButton, QDialogButtonBox::ActionRole);

}
//=======================================================
void ZMeasuringConditionsDialog::zh_createConnections()
{
    connect(zv_okButton, &QPushButton::clicked,
            this, &ZMeasuringConditionsDialog::zh_onOkButtonClick);
    connect(zv_cancelButton, &QPushButton::clicked,
            this, &ZMeasuringConditionsDialog::reject);
    connect(zv_gainFactorSpinBox, SIGNAL(valueChanged(int)),
            zv_messageLabel, SLOT(clear()));
    connect(zv_expositionSpinBox, SIGNAL(valueChanged(int)),
            this, SLOT(zh_resetMessageLabel()));
}
//=======================================================
void ZMeasuringConditionsDialog::zh_onOkButtonClick()
{
    // Check data
    bool res = false;
    emit zg_checkMeasuringConditions(zv_gainFactorSpinBox->value(),
                                     zv_expositionSpinBox->value(), res);
    if(!res)
    {
        zv_messageLabel->setText(QString("<font color=red>%1</font>").arg(tr("Such measuring conditions alredy exist.")));
        return;
    }

    accept();
}
//=======================================================
void ZMeasuringConditionsDialog::zh_resetMessageLabel(int val)
{
    zv_messageLabel->clear();
}
//=======================================================
