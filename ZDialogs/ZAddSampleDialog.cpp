//======================================================
#include "ZAddSampleDialog.h"
#include "ZGLConstantsAndDefines.h"

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QDialogButtonBox>

#include <QFrame>
#include <QLabel>
#include <QPushButton>
#include <QLineEdit>
#include <QComboBox>
//======================================================
ZAddSampleDialog::ZAddSampleDialog(QWidget *parent) : QDialog(parent)
{
    setWindowTitle(tr("Sample insertion"));

    zh_createComponents();
    zh_createConnections();
}
//======================================================
void ZAddSampleDialog::zh_createComponents()
{
    QVBoxLayout* mainLayout = new QVBoxLayout(this);
    setLayout(mainLayout);

    // name
    QLabel* label = new QLabel(this);
    label->setText(glCreateCaption(tr("Sample name:")));
    mainLayout->addWidget(label);

    QHBoxLayout* levelLayout = new QHBoxLayout(this);
    mainLayout->addLayout(levelLayout);

    zv_sampleNameLineEdit = new QLineEdit(this);
    levelLayout->addWidget(zv_sampleNameLineEdit);
    levelLayout->addStretch();

    // task
    label = new QLabel(this);
    label->setText(glCreateCaption(tr("Task:")));
    mainLayout->addWidget(label);

    levelLayout = new QHBoxLayout(this);
    mainLayout->addLayout(levelLayout);

    zv_taskComboBox = new QComboBox(this);
    levelLayout->addWidget(zv_taskComboBox);
    levelLayout->addStretch();

    mainLayout->addStretch();

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
void ZAddSampleDialog::zh_createConnections()
{
    connect(zv_okButton, &QPushButton::clicked,
            this, &ZAddSampleDialog::zh_onOkButtonClick);

    connect(zv_cancelButton, &QPushButton::clicked,
            this, &ZAddSampleDialog::reject);

}
//======================================================
QString ZAddSampleDialog::zp_sampleName() const
{
    return zv_sampleNameLineEdit->text();
}
//======================================================
//bool ZAddSampleDialog::zp_setSampleName(const QString& name)
//{
//    if(!name.isEmpty() || name == zv_sampleNameLineEdit->text())
//    {
//        return false;
//    }

//    zv_sampleNameLineEdit->setText(name);
//    return true;
//}
//======================================================
void ZAddSampleDialog::zh_onOkButtonClick()
{
    // Check data
    bool res = false;
    // check sample name
    emit zg_checkSampleName(zv_sampleNameLineEdit->text(), res);

    if(!res)
    {
        return;
    }

    accept();
}
//======================================================
