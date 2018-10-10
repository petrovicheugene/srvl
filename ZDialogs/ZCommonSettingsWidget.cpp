//========================================================
#include "ZCommonSettingsWidget.h"
#include "ZGeneral.h"
#include "ZOperatorDialog.h"
#include "ZPasswordChangeDialog.h"

#include <QDialogButtonBox>
#include <QHBoxLayout>
#include <QLabel>
#include <QListWidget>
#include <QMessageBox>
#include <QPushButton>
#include <QSqlError>
#include <QSqlQuery>
#include <QVBoxLayout>

//========================================================
ZCommonSettingsWidget::ZCommonSettingsWidget(QWidget *parent) : QWidget(parent)
{
    zh_createComponents();
    zh_createConnections();
    zh_loadOperators();
}
//========================================================
void ZCommonSettingsWidget::zh_createComponents()
{
    QVBoxLayout* mainLayout = new QVBoxLayout;
    setLayout(mainLayout);

    // app password
    QLabel* captionLabel = new QLabel(this);
    captionLabel->setText(glCreateCaption(tr("Application password settings:")));
    mainLayout->addWidget(captionLabel);

    QHBoxLayout* passwordLayout = new  QHBoxLayout;
    mainLayout->addLayout(passwordLayout);
    zv_passwordSettingsButton = new QPushButton(this);
    zv_passwordSettingsButton->setText(tr("Password"));
    zv_passwordSettingsButton->setToolTip(tr("Password settings dialog"));
    passwordLayout->addWidget(zv_passwordSettingsButton);
    passwordLayout->addStretch();

    // operator list
    captionLabel = new QLabel(this);
    captionLabel->setText(glCreateCaption(tr("Operator list:")));
    mainLayout->addWidget(captionLabel);

    zv_operatorsListWidget = new QListWidget(this);

    mainLayout->addWidget(zv_operatorsListWidget);
    QDialogButtonBox* opratorButtonBox = new QDialogButtonBox(this);
    mainLayout->addWidget(opratorButtonBox);

    zv_newOperatorButton = new QPushButton(this);
    zv_newOperatorButton->setText(tr("New"));
    opratorButtonBox->addButton(zv_newOperatorButton, QDialogButtonBox::ActionRole);

    zv_editOperatorButton = new QPushButton(this);
    zv_editOperatorButton->setText(tr("Edit"));
    opratorButtonBox->addButton(zv_editOperatorButton, QDialogButtonBox::ActionRole);

    zv_removeOperatorButton = new QPushButton(this);
    zv_removeOperatorButton->setText(tr("Remove"));
    opratorButtonBox->addButton(zv_removeOperatorButton, QDialogButtonBox::ActionRole);


    mainLayout->addStretch();
}
//========================================================
void ZCommonSettingsWidget::zh_createConnections()
{
    connect(zv_passwordSettingsButton, &QPushButton::clicked,
            this, &ZCommonSettingsWidget::zh_runPasswordChangeDialog);

    connect(zv_newOperatorButton, &QPushButton::clicked,
            this, &ZCommonSettingsWidget::zh_newOperator);
    connect(zv_editOperatorButton, &QPushButton::clicked,
            this, &ZCommonSettingsWidget::zh_editOperator);
    connect(zv_removeOperatorButton, &QPushButton::clicked,
            this, &ZCommonSettingsWidget::zh_removeOperator);


}
//========================================================
void ZCommonSettingsWidget::zh_runPasswordChangeDialog()
{
    ZPasswordChangeDialog* passwordDialog = new ZPasswordChangeDialog(this);
    passwordDialog->setWindowFlags(Qt::Tool);

    if(passwordDialog->exec())
    {
        QMessageBox::information(this, tr("Password settings"), tr("The password has been saved successfully"), QMessageBox::Ok);
    }

    delete passwordDialog;
}
//========================================================
void ZCommonSettingsWidget::zh_loadOperators()
{
    zv_operatorsListWidget->clear();

    QSqlQuery query;
    QString queryString = QString("SELECT * FROM operators");

    if(!query.prepare(queryString))
    {
        QString msg = query.lastError().text();
        QMessageBox::critical(this, tr("Operator loading error"), msg, QMessageBox::Ok);
        return ;
    }

    if(!query.exec())
    {
        QString msg = query.lastError().text();
        QMessageBox::critical(this, tr("Operator loading error"), msg, QMessageBox::Ok);
        return ;
    }

    QVariant vData;
    int id;
    QString login;
    QByteArray password;
    while(query.next())
    {
        vData = query.value(0);
        if(!vData.isValid() || !vData.canConvert<int>())
        {
            continue;
        }

        id = vData.toInt();

        vData = query.value(1);
        if(!vData.isValid() || !vData.canConvert<QString>())
        {
            continue;
        }

        login = vData.toString();

        vData = query.value(2);
        if(!vData.isValid() || !vData.canConvert<int>())
        {
            continue;
        }

        password = vData.toByteArray();

        zh_addOperatorItem(id, login, password);
    }
}
//========================================================
void ZCommonSettingsWidget::zh_addOperatorItem(int id, const QString& login, const QByteArray& password)
{
    QListWidgetItem* item = new QListWidgetItem;
    item->setText(login);
    item->setData(Qt::UserRole + 1, id);
    item->setData(Qt::UserRole + 2, password);

    zv_operatorsListWidget->addItem(item);
}
//========================================================
void ZCommonSettingsWidget::zh_newOperator()
{
    ZOperatorDialog dialog;
    dialog.setWindowFlags(Qt::Tool);

    connect(&dialog, &ZOperatorDialog::zg_requestLoginExistance,
            this, &ZCommonSettingsWidget::zh_checkLoginExistance);

    if(!dialog.exec())
    {
        return;
    }

    // create new operator
    QString login = dialog.zp_login();
    QByteArray password = dialog.zp_password();

    int id;
    if(!zh_findNewIdInTable(QString("operators"), id))
    {
        QString msg = tr("Cannot define id for new operator");
        QMessageBox::critical(this, tr("Operator saving error"), msg, QMessageBox::Ok);
        return;
    }

    QSqlQuery query;
    QString queryString = QString("INSERT INTO operators (id, name, password) "
                                  "VALUES (:id, :name, :password)");

    if(!query.prepare(queryString))
    {
        QString msg = query.lastError().text();
        QMessageBox::critical(this, tr("Operator saving error"), msg, QMessageBox::Ok);
        return ;
    }

    query.bindValue(":id", id);
    query.bindValue(":name", login);
    query.bindValue(":password", password);

    if(!query.exec())
    {
        QString msg = query.lastError().text();
        QMessageBox::critical(this, tr("Operator saving error"), msg, QMessageBox::Ok);
        return ;
    }

    zh_addOperatorItem(id, login, password);
}
//========================================================
void ZCommonSettingsWidget::zh_editOperator()
{
    QListWidgetItem* item = zv_operatorsListWidget->currentItem();
    QString operatorLogin = item->text();
    QByteArray passwordByteArray = item->data(Qt::UserRole + 2).toByteArray();
    ZOperatorDialog dialog(operatorLogin, passwordByteArray);
    dialog.setWindowFlags(Qt::Tool);

    connect(&dialog, &ZOperatorDialog::zg_requestLoginExistance,
            this, &ZCommonSettingsWidget::zh_checkLoginExistance);

    if(!dialog.exec())
    {
        return;
    }

    // update current operator
    QString login = dialog.zp_login();
    QByteArray password = dialog.zp_password();
    int id = item->data(Qt::UserRole+1).toInt();

    QSqlQuery query;
    QString queryString = QString("UPDATE operators SET name=:name, "
                                  "password=:password "
                                  "WHERE id=%1").arg(QString::number(id)) ;

    if(!query.prepare(queryString))
    {
        QString msg = query.lastError().text();
        QMessageBox::critical(this, tr("Operator update error"), msg, QMessageBox::Yes);
        return;
    }

    query.bindValue(":password", QVariant(password));
    query.bindValue(":name", QVariant(login));

    if(!query.exec())
    {
        QString msg = query.lastError().text();
        QMessageBox::critical(this, tr("Operator update error"), msg, QMessageBox::Yes);
        return;
    }

    zh_loadOperators();
    for(int i = 0; i < zv_operatorsListWidget->count(); i++)
    {
        if(zv_operatorsListWidget->item(i)->data(Qt::UserRole+1).toInt() == id)
        {
            zv_operatorsListWidget->setCurrentItem(zv_operatorsListWidget->item(i));
            zv_operatorsListWidget->scrollToItem(zv_operatorsListWidget->item(i));
            break;
        }
    }

}
//========================================================
void ZCommonSettingsWidget::zh_removeOperator()
{


}
//========================================================
void ZCommonSettingsWidget::zh_checkLoginExistance(const QString& login,
                                                   bool& existance) const
{
    for(int i = 0; i < zv_operatorsListWidget->count(); i++)
    {
        if(zv_operatorsListWidget->item(i)->text() == login)
        {
            existance = true;
            return;
        }
    }

    existance = false;
}
//========================================================
bool ZCommonSettingsWidget::zh_findNewIdInTable(const QString& tableName, int& newId)
{
    QSqlQuery query;
    QString queryString = QString("SELECT MAX(id) FROM %1").arg(tableName);
    if(!query.prepare(queryString))
    {
        return false;
    }
    if(!query.exec())
    {
        return false;
    }

    if(!query.next())
    {
        newId = 1;
        return true;
    }

    QVariant vData = query.value(0);
    if(!vData.isValid() || !vData.canConvert<int>())
    {
        return false;
    }

    newId = vData.toInt() + 1;

    return true;
}
//========================================================


