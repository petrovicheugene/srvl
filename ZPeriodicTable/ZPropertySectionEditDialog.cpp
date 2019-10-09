//======================================================
#include "ZPropertySectionEditDialog.h"

#include <QApplication>
#include <QGroupBox>
#include <QHBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QMessageBox>
#include <QPushButton>
#include <QRadioButton>
#include <QRegExp>
#include <QVBoxLayout>
//======================================================
ZPropertySectionEditDialog::ZPropertySectionEditDialog(const QStringList& branch,
                                                       bool currentVisible,
                                                       bool selectedVisible,
                                                       QWidget *parent) : ZBaseDialog("ZPropertySectionEditDialog", parent)
{
    setWindowTitle(qApp->applicationDisplayName());

    zh_createComponents();
    zh_createConnections();
    zp_setCurrentSectionBranch(branch);
    zh_setUpAdditionTargetButtons(currentVisible, selectedVisible );

}
//======================================================
void ZPropertySectionEditDialog::zh_createComponents()
{
    zh_addWidgetToMainLayout(zh_createControlWidget());
    zh_addStretchToMainLayout();

    zv_okButton = zh_createBasementButton(tr("Ok"),
                                          tr("Save section and close dialog"),
                                          QIcon());
    zv_cancelButton = zh_createBasementButton(tr("Cancel"),
                                              tr("Close dialog without savimg section"),
                                              QIcon());
}
//======================================================
void ZPropertySectionEditDialog::zh_createConnections()
{
    connect(zv_okButton, &QPushButton::clicked,
            this, &ZPropertySectionEditDialog::zh_onOkButtonClick);
    connect(zv_cancelButton, &QPushButton::clicked,
            this, &ZPropertySectionEditDialog::zh_onCloseButtonClick);

}
//======================================================
QWidget* ZPropertySectionEditDialog::zh_createControlWidget()
{
    QWidget* widget = new QWidget(this);
    QVBoxLayout* mainLayout = new QVBoxLayout;
    widget->setLayout(mainLayout);

    QLabel* label = new QLabel(this);
    label->setText(tr("Current section branch:"));
    mainLayout->addWidget(label);

    zv_parentSectionBranchLabel = new QLabel(this);
    mainLayout->addWidget(zv_parentSectionBranchLabel);

    //    zv_parentSectionBranchLineEdit = new QLineEdit(this);
    //    mainLayout->addWidget(zv_parentSectionBranchLineEdit);

    label = new QLabel(this);
    label->setText(tr("Section name:"));
    mainLayout->addWidget(label);

    zv_sectionLineEdit  = new QLineEdit(this);
    zv_sectionLineEdit->installEventFilter(this);
    mainLayout->addWidget(zv_sectionLineEdit);

    QGroupBox* groupBox = new QGroupBox(widget);
    groupBox->setTitle(tr("Add to"));
    mainLayout->addWidget(groupBox);
    QVBoxLayout* groupBoxLayout = new QVBoxLayout;
    groupBox->setLayout(groupBoxLayout);

    zv_toCurrentChemicalElementRadioButton = new QRadioButton(this);
    zv_toCurrentChemicalElementRadioButton->installEventFilter(this);
    zv_toCurrentChemicalElementRadioButton->setText(tr("Current chemical element"));
    groupBoxLayout->addWidget(zv_toCurrentChemicalElementRadioButton);

    zv_toSelectedChemicalElementsRadioButton = new QRadioButton(this);
    zv_toSelectedChemicalElementsRadioButton->installEventFilter(this);
    zv_toSelectedChemicalElementsRadioButton->setText(tr("Selected chemical elements"));
    groupBoxLayout->addWidget(zv_toSelectedChemicalElementsRadioButton);

    zv_toAllChemicalElementsRadioButton = new QRadioButton(this);
    zv_toAllChemicalElementsRadioButton->installEventFilter(this);
    zv_toAllChemicalElementsRadioButton->setText(tr("All chemical elements"));
    groupBoxLayout->addWidget(zv_toAllChemicalElementsRadioButton);

    return widget;
}
//======================================================
void ZPropertySectionEditDialog::zh_onOkButtonClick()
{
    if(zv_sectionLineEdit->text().isEmpty())
    {
        QString msg = tr("");
        QMessageBox::critical(this, tr("Error"), msg, QMessageBox::Ok);
        return;
    }

    accept();
}
//======================================================
void ZPropertySectionEditDialog::zh_onCloseButtonClick()
{
    reject();
}
//======================================================
void ZPropertySectionEditDialog::zh_setUpAdditionTargetButtons(bool currentVisible,
                                                               bool selectedVisible)
{
    zv_toCurrentChemicalElementRadioButton->setVisible(currentVisible);
    zv_toSelectedChemicalElementsRadioButton->setVisible(selectedVisible);

    if(currentVisible)
    {
        zv_toCurrentChemicalElementRadioButton->setChecked(true);
    }
    else
    {
        if(selectedVisible)
        {
            zv_toSelectedChemicalElementsRadioButton->setChecked(true);
        }
        else
        {
            zv_toAllChemicalElementsRadioButton->setChecked(true);
        }
    }
}
//======================================================
ZPropertySectionEditDialog::AdditionTarget ZPropertySectionEditDialog::zp_additionTarget() const
{
    if(zv_toAllChemicalElementsRadioButton->isChecked())
    {
        return AT_ALL;
    }
    else if(zv_toSelectedChemicalElementsRadioButton->isChecked())
    {
        return AT_SELECTED;
    }
    else // to current
    {
        return AT_CURRENT;
    }
}
//======================================================
QStringList ZPropertySectionEditDialog::zp_newSectionBranch() const
{
    QStringList sectionBranch = zv_parentSectionBranch;
    QStringList newSectionBranch = zv_sectionLineEdit->text().split(QRegExp("[\\\\/-]"),
                                                                    QString::QString::SkipEmptyParts);

    foreach(QString section, newSectionBranch)
    {
        sectionBranch.append(section);
    }

    return sectionBranch;
}
//======================================================
void ZPropertySectionEditDialog::zp_setCurrentSectionBranch(const QStringList& branch)
{
    zv_parentSectionBranch = branch;
    QString currentSectionBranchString;
    for(int s = 0; s < zv_parentSectionBranch.count(); s++)
    {
        currentSectionBranchString += zv_parentSectionBranch.at(s);
        if(s < zv_parentSectionBranch.count() - 1)
        {
            currentSectionBranchString += "/";
        }

    }
    zv_parentSectionBranchLabel->setText(QString("<b>%1</b>").arg(currentSectionBranchString));

    zv_toCurrentChemicalElementRadioButton->setEnabled(!zv_parentSectionBranch.isEmpty());
    zv_toSelectedChemicalElementsRadioButton->setEnabled(!zv_parentSectionBranch.isEmpty());
    zv_toAllChemicalElementsRadioButton->setEnabled(!zv_parentSectionBranch.isEmpty());
}
//======================================================
