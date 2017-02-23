//=========================================================
#include "ZSetGainFactorToCalibrationDialog.h"
#include "ZGeneral.h"

#include <QDialogButtonBox>
#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QSpinBox>
#include <QVBoxLayout>

//=========================================================
ZSetGainFactorToCalibrationDialog::ZSetGainFactorToCalibrationDialog(const QString& calibrationName, QWidget *parent)
    : QDialog(parent)
{
    zh_createComponents(calibrationName);
    zh_createConnections();
}
//=========================================================
void ZSetGainFactorToCalibrationDialog::zh_createComponents(const QString& calibrationName)
{
    QVBoxLayout* mainLayout = new QVBoxLayout(this);
    setLayout(mainLayout);

    QLabel* label = new QLabel(this);
    QString msg = tr("Input gain factor value for the calibration \"%1\".").arg(calibrationName);

    label->setText(msg);
    mainLayout->addWidget(label);

    QHBoxLayout* levelLayout = new QHBoxLayout(this);
    mainLayout->addLayout(levelLayout);
    zv_gainFactorSpinBox = new QSpinBox(this);
    zv_gainFactorSpinBox->setRange(1, glMaxGainFactor);
    levelLayout->addWidget(zv_gainFactorSpinBox);
    levelLayout->addStretch();

    // buttons
    QDialogButtonBox* buttonBox = new QDialogButtonBox(this);
    mainLayout->addWidget(buttonBox);

    zv_okButton = new QPushButton(this);
    zv_okButton->setText(tr("Ok"));
    buttonBox->addButton(zv_okButton, QDialogButtonBox::ActionRole);
    zv_cancelButton = new QPushButton(this);
    zv_cancelButton->setText(tr("Cancel"));

    buttonBox->addButton(zv_cancelButton, QDialogButtonBox::ActionRole);
}
//=========================================================
void ZSetGainFactorToCalibrationDialog::zh_createConnections()
{
    connect(zv_okButton, &QPushButton::clicked,
            this, &ZSetGainFactorToCalibrationDialog::accept);
    connect(zv_cancelButton, &QPushButton::clicked,
            this, &ZSetGainFactorToCalibrationDialog::reject);

}
//=========================================================
int ZSetGainFactorToCalibrationDialog::zp_gainFactor() const
{
    return zv_gainFactorSpinBox->value();
}
//=========================================================
