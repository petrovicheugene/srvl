//======================================================
#include "ZPropertyEditDialog.h"

#include <QApplication>
#include <QLabel>
#include <QLineEdit>
#include <QMessageBox>
#include <QPushButton>
#include <QVBoxLayout>
//======================================================
ZPropertyEditDialog::ZPropertyEditDialog(QWidget *parent)
    : ZBaseDialog("ZPropertyEditDialog", parent)
{
    setWindowTitle(qApp->applicationDisplayName());

    zh_createComponents();
    zh_createConnections();
}
//======================================================
void ZPropertyEditDialog::zh_createComponents()
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
void ZPropertyEditDialog::zh_createConnections()
{
    connect(zv_okButton, &QPushButton::clicked,
            this, &ZPropertyEditDialog::zh_onOkButtonClick);
    connect(zv_cancelButton, &QPushButton::clicked,
            this, &ZPropertyEditDialog::zh_onCloseButtonClick);

}
//======================================================
QWidget* ZPropertyEditDialog::zh_createControlWidget()
{
    QWidget* widget = new QWidget(this);
    QVBoxLayout* mainLayout = new QVBoxLayout;
    widget->setLayout(mainLayout);

    QLabel* label = new QLabel(this);
    label->setText(tr("Chemical property name:"));
    mainLayout->addWidget(label);

    zv_propertyNameLineEdit =  new QLineEdit(this);
    zv_propertyNameLineEdit->installEventFilter(this);
    mainLayout->addWidget(zv_propertyNameLineEdit);


    label = new QLabel(this);
    label->setText(tr("Chemical property value:"));
    mainLayout->addWidget(label);

    zv_propertyValueLineEdit =  new QLineEdit(this);
    zv_propertyValueLineEdit->installEventFilter(this);
    mainLayout->addWidget(zv_propertyValueLineEdit);

    return widget;
}
//======================================================
void ZPropertyEditDialog::zh_onOkButtonClick()
{
    // check data
    if( zp_propertyName().isEmpty())
    {
        QString msg = tr("Input property name!");
        QMessageBox::critical(this, tr("Error"), msg, QMessageBox::Ok);
        return;
    }

    accept();
}
//======================================================
void ZPropertyEditDialog::zh_onCloseButtonClick()
{
    reject();
}
//======================================================
QString ZPropertyEditDialog::zp_propertyName() const
{
    return zv_propertyNameLineEdit->text();
}
//======================================================
QString ZPropertyEditDialog::zp_propertyValue() const
{
    return zv_propertyValueLineEdit->text();
}
//======================================================
QPair<QString, QString> ZPropertyEditDialog::zp_property() const
{
    return QPair<QString, QString>(zp_propertyName(), zp_propertyValue());
}
//======================================================

