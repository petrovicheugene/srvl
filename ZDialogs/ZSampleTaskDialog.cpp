//===============================================================
#include "ZSampleTaskDialog.h"
#include "ZGLConstantsAndDefines.h"
#include "ZTaskMeasurementConditionTableWidget.h"
#include "ZChemicalTaskTableWidget.h"
#include "ZTaskCalibrationStackTableWidget.h"

#include <QVBoxLayout>
#include <QHBoxLayout>

#include <QDialogButtonBox>
#include <QFrame>
#include <QLineEdit>
#include <QLabel>
#include <QPushButton>
#include <QSpinBox>
#include <QTableView>
#include <QSettings>
#include <QSplitter>
//===============================================================
ZSampleTaskDialog::ZSampleTaskDialog(bool withQuantityFlag, QWidget *parent) : QDialog(parent)
{
    setWindowTitle(tr("Sample task"));
    zv_sampleTaskQuantitySpinBox = 0;

    zh_createComponents(withQuantityFlag);
    zh_createConnections();
    zh_restoreSettings();
}
//===============================================================
QString ZSampleTaskDialog::zp_sampleTaskName() const
{
    return zv_sampleTaskNameLineEdit->text();
}
//===============================================================
QString ZSampleTaskDialog::zp_defaultSampleName() const
{
    return zv_defaultSampleNameLineEdit->text();
}
//===============================================================
int ZSampleTaskDialog::zp_sampleTaskQuantity() const
{
    return zv_sampleTaskQuantitySpinBox->value();
}
//===============================================================
void ZSampleTaskDialog::closeEvent(QCloseEvent* event)
{
    zh_saveSettings();
}
//===============================================================
void ZSampleTaskDialog::zh_createComponents(bool withQuantityFlag)
{
    QVBoxLayout* mainLayout = new QVBoxLayout(this);
    setLayout(mainLayout);

    // name lineEdit
    QLabel* label = new QLabel(this);
    label->setText(glCreateCaption(tr("Task name:")));
    mainLayout->addWidget(label);

    QHBoxLayout* levelLayout = new QHBoxLayout(this);
    mainLayout->addLayout(levelLayout);
    zv_sampleTaskNameLineEdit = new QLineEdit(this);
    levelLayout->addWidget(zv_sampleTaskNameLineEdit);
    levelLayout->addStretch();

    // default sample name
    label = new QLabel(this);
    label->setText(glCreateCaption(tr("Default sample name:")));
    mainLayout->addWidget(label);

    levelLayout = new QHBoxLayout(this);
    mainLayout->addLayout(levelLayout);
    zv_defaultSampleNameLineEdit = new QLineEdit(this);
    levelLayout->addWidget(zv_defaultSampleNameLineEdit);
    label = new QLabel(this);
    label->setText(glCreateCaption(tr(" + sample serial number")));
    levelLayout->addWidget(label);
    levelLayout->addStretch();

    if(withQuantityFlag)
    {
        // withQuantityFlag is for add task dialog
        // task quantity
        label = new QLabel(this);
        label->setText(glCreateCaption(tr("Quantity:")));
        mainLayout->addWidget(label);

        levelLayout = new QHBoxLayout(this);
        mainLayout->addLayout(levelLayout);
        zv_sampleTaskQuantitySpinBox = new QSpinBox(this);
        zv_sampleTaskQuantitySpinBox->setRange(1, 999);
        levelLayout->addWidget(zv_sampleTaskQuantitySpinBox);
        levelLayout->addStretch();
    }
    // Table level
    zv_tableSplitter = new QSplitter(this);
    zv_tableSplitter->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    mainLayout->addWidget(zv_tableSplitter);

    zv_measurementTableWidget = new ZTaskMeasurementConditionTableWidget(this);
    zv_measurementTableWidget->zp_setCaption(glCreateCaption(tr("Measurement conditions:")));

    zv_chemicalTaskTableWidget = new ZChemicalTaskTableWidget(this);
    zv_chemicalTaskTableWidget->zp_setCaption(glCreateCaption(tr("Chemical tasks:")));

    zv_calibrationStackTableWidget = new ZTaskCalibrationStackTableWidget(this);
    zv_calibrationStackTableWidget->zp_setCaption(glCreateCaption(tr("Calibrations:")));

    zv_tableSplitter->addWidget(zv_measurementTableWidget);
    zv_tableSplitter->addWidget(zv_chemicalTaskTableWidget);
    zv_tableSplitter->addWidget(zv_calibrationStackTableWidget);

    // Basement
    //mainLayout->addStretch();
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
//===============================================================
void ZSampleTaskDialog::zh_createConnections()
{
    connect(zv_okButton, &QPushButton::clicked,
            this, &ZSampleTaskDialog::zh_onOkButtonClick);

    connect(zv_cancelButton, &QPushButton::clicked,
            this, &ZSampleTaskDialog::reject);

}
//===============================================================
void ZSampleTaskDialog::zh_restoreSettings()
{
    QSettings settings;
    QVariant vData;
    settings.beginGroup(glAppVersion);
    settings.beginGroup("SampleTaskDialog");

    vData = settings.value("dialogGeometry");
    if(vData.isValid() && !vData.isNull() && vData.canConvert<QByteArray>())
    {
        this->restoreGeometry(vData.value<QByteArray>());
    }

    vData = settings.value("splitterGeometry");
    if(vData.isValid() && !vData.isNull() && vData.canConvert<QByteArray>())
    {
        zv_tableSplitter->restoreState(vData.value<QByteArray>());
    }

    settings.endGroup();
    settings.endGroup();
}
//===============================================================
void ZSampleTaskDialog::zh_saveSettings() const
{
    QSettings settings;
    settings.beginGroup(glAppVersion);
    settings.beginGroup("SampleTaskDialog");

    settings.setValue("dialogGeometry", QVariant::fromValue<QByteArray>(this->saveGeometry()));
    settings.setValue("splitterGeometry", QVariant::fromValue<QByteArray>(zv_tableSplitter->saveState()));

    settings.endGroup();
    settings.endGroup();
}
//===============================================================
void ZSampleTaskDialog::zh_onOkButtonClick()
{
    // Check data
    bool res = false;
    // check sample name
    emit zg_checkTaskName(zv_sampleTaskNameLineEdit->text(), res);

    if(!res)
    {
        return;
    }

    accept();
}
//===============================================================
