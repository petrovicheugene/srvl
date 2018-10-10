//===============================================================
#include "ZChemicalDialog.h"
#include "ZGeneral.h"
#include <QDialogButtonBox>
#include <QHBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QSettings>
#include <QVBoxLayout>
//===============================================================
ZChemicalDialog::ZChemicalDialog(QWidget* parent) : QDialog(parent)
{
    setWindowFlags(Qt::Tool);

    zv_id = -1;
    zv_createComponents();
    zv_createConnections();
    zh_restoreSettings();
}
//===============================================================
ZChemicalDialog::~ZChemicalDialog()
{
    zh_saveSettings();
}
//===============================================================
QString ZChemicalDialog::zp_chemical() const
{
    return zv_chemicalLineEdit->text();
}
//===============================================================
int ZChemicalDialog::zp_chemicalId() const
{
    return zv_id;
}
//===============================================================
void ZChemicalDialog::zp_setChemical(int id, const QString& chemical)
{
    zv_id = id;
    zv_chemicalLineEdit->setText(chemical);
}
//===============================================================
void ZChemicalDialog::zv_createComponents()
{
    QVBoxLayout* mainLayout = new QVBoxLayout;
    setLayout(mainLayout);

    // chemical line edit
    QLabel* label = new QLabel(this);
    label->setText(glCreateCaption(tr("Chemical:")));
    mainLayout->addWidget(label);

    zv_chemicalLineEdit = new QLineEdit(this);
    mainLayout->addWidget(zv_chemicalLineEdit);

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
//===============================================================
void ZChemicalDialog::zv_createConnections()
{
    connect(zv_okButton, &QPushButton::clicked,
            this, &ZChemicalDialog::zh_onOkButtonClick);
    connect(zv_cancelButton, &QPushButton::clicked,
            this, &ZChemicalDialog::reject);
    connect(zv_chemicalLineEdit, &QLineEdit::textChanged,
            this, &ZChemicalDialog::zh_resetMessageLabel);
}
//===============================================================
void ZChemicalDialog::zh_restoreSettings()
{
    QSettings settings;
    QVariant vData;
    settings.beginGroup(glAppVersion);
    settings.beginGroup("ChemicalDialog");

    vData = settings.value("dialogGeometry");
    if(vData.isValid() && !vData.isNull() && vData.canConvert<QByteArray>())
    {
        this->restoreGeometry(vData.value<QByteArray>());
    }

    settings.endGroup();
    settings.endGroup();
}
//===============================================================
void ZChemicalDialog::zh_saveSettings() const
{
    QSettings settings;
    settings.beginGroup(glAppVersion);
    settings.beginGroup("ChemicalDialog");

    settings.setValue("dialogGeometry", QVariant::fromValue<QByteArray>(this->saveGeometry()));

    settings.endGroup();
    settings.endGroup();
}
//===============================================================
void ZChemicalDialog::zh_onOkButtonClick()
{
    // Check data
    if(zv_chemicalLineEdit->text().isEmpty())
    {
        zv_messageLabel->setText(QString("<font color=red>%1</font>").arg(tr("Chemical name cannot be empty.")));
        return;
    }

    bool res = false;
    emit zg_checkChemical(zv_id, zv_chemicalLineEdit->text(), res);
    if(!res)
    {
        zv_messageLabel->setText(QString("<font color=red>%1</font>").arg(tr("Such chemical alredy exists.")));
        return;
    }

    accept();
}
//===============================================================
void ZChemicalDialog::zh_resetMessageLabel(const QString& text)
{
    zv_messageLabel->clear();
}
//===============================================================
