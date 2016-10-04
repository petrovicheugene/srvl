//======================================================
#include "ZAddSampleDialog.h"
#include "ZGLConstantsAndDefines.h"

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QDialogButtonBox>

#include <QComboBox>
#include <QFrame>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QSpinBox>
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

    // task quantity
    label = new QLabel(this);
    label->setText(glCreateCaption(tr("Quantity:")));
    mainLayout->addWidget(label);

    levelLayout = new QHBoxLayout(this);
    mainLayout->addLayout(levelLayout);
    zv_quantitySpinBox = new QSpinBox(this);
    zv_quantitySpinBox->setRange(1, 999);
    levelLayout->addWidget(zv_quantitySpinBox);
    levelLayout->addStretch();

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
    zv_okButton->setDisabled(true);
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

     connect(zv_sampleNameLineEdit, &QLineEdit::textChanged,
             this, &ZAddSampleDialog::zh_onSampleNameChange);

}
//======================================================
QString ZAddSampleDialog::zp_sampleName() const
{
    return zv_sampleNameLineEdit->text();
}
//======================================================
int ZAddSampleDialog::zp_sampleQuantity() const
{
    return zv_quantitySpinBox->value();
}
//======================================================
void ZAddSampleDialog::zh_onOkButtonClick()
{
//    // Check data
//    bool res = false;
//    // check sample name
//    emit zg_checkSampleName(zv_sampleNameLineEdit->text(), res);

//    if(!res)
//    {
//        return;
//    }

    accept();
}
//======================================================
void ZAddSampleDialog::zh_onSampleNameChange(const QString & text)
{
    zv_okButton->setDisabled(zv_sampleNameLineEdit->text().isEmpty());
}
//======================================================
