//======================================================
#include "ZOperatorDialog.h"
#include "ZGeneral.h"
#include "simplecrypt.h"

#include <QApplication>
#include <QBuffer>
#include <QCheckBox>
#include <QDataStream>
#include <QDialogButtonBox>
#include <QLabel>
#include <QLineEdit>
#include <QMessageBox>
#include <QPushButton>
#include <QSettings>
#include <QSqlQuery>
#include <QVBoxLayout>

//======================================================
ZOperatorDialog::ZOperatorDialog(QWidget *parent) : QDialog(parent)
{
    setWindowTitle(tr("New operator"));

    zh_createComponents(QString(), QByteArray());
    zh_createConnections();
}
//======================================================
ZOperatorDialog::ZOperatorDialog(const QString& operatorLogin,
                                 const QByteArray& password,
                                 QWidget *parent)
{
    setWindowTitle(tr("%1 operator settings").arg(operatorLogin));

    zh_createComponents(operatorLogin, password);
    zh_createConnections();

}
//======================================================
QString ZOperatorDialog::zp_login() const
{
    return zv_loginLineEdit->text();
}
//======================================================
QByteArray ZOperatorDialog::zp_password() const
{
    return zv_passwordByteArray;
}
//======================================================
void ZOperatorDialog::zh_createComponents(const QString& operatorLogin,
                                          const QByteArray& passwordByteArray)
{

    zv_oldLogin = operatorLogin;

    if(!zh_decryptByteArrayToString(zv_oldPasswordString, passwordByteArray))
    {
        zv_oldPasswordString = QString();
    }

    QVBoxLayout* mainLayout = new QVBoxLayout(this);
    setLayout(mainLayout);
    QLabel* label = new QLabel(this);
    label->setText(tr("Login:"));

    zv_loginLineEdit = new QLineEdit(this);
    label->setBuddy(zv_loginLineEdit);
    mainLayout->addWidget(label);
    mainLayout->addWidget(zv_loginLineEdit);
    zv_loginLineEdit->setText(operatorLogin);

    if(!zv_oldPasswordString.isEmpty())
    {
        label = new QLabel(this);
        label->setText(tr("Enter current password"));

        zv_oldPasswordLineEdit = new QLineEdit(this);
        zv_oldPasswordLineEdit->setEchoMode(QLineEdit::Password);
        label->setBuddy(zv_oldPasswordLineEdit);

        mainLayout->addWidget(label);
        mainLayout->addWidget(zv_oldPasswordLineEdit);
        mainLayout->addSpacing(10);

    }
    else
    {
        zv_oldPasswordLineEdit = 0;
    }

    label = new QLabel(this);
    label->setText(tr("Enter new password"));

    zv_newPasswordFirstLineEdit = new QLineEdit(this);
    zv_newPasswordFirstLineEdit->setEchoMode(QLineEdit::Password);
    label->setBuddy(zv_newPasswordFirstLineEdit);

    mainLayout->addWidget(label);
    mainLayout->addWidget(zv_newPasswordFirstLineEdit);
    mainLayout->addSpacing(10);

    label = new QLabel(this);
    label->setText(tr("Repeat new password"));

    zv_newPasswordSecondLineEdit = new QLineEdit(this);
    zv_newPasswordSecondLineEdit->setEchoMode(QLineEdit::Password);
    label->setBuddy(zv_newPasswordSecondLineEdit);

    mainLayout->addWidget(label);
    mainLayout->addWidget(zv_newPasswordSecondLineEdit);
    mainLayout->addSpacing(10);

    zv_showSymbolsCheckBox = new QCheckBox(this);
    zv_showSymbolsCheckBox->setText(tr("Show symbols"));
    mainLayout->addWidget(zv_showSymbolsCheckBox);
    mainLayout->addSpacing(10);

    QDialogButtonBox* buttonBox= new QDialogButtonBox(this);
    mainLayout->addWidget(buttonBox);

    zv_okButton = new QPushButton(this);
    zv_okButton->setText(tr("OK"));
    buttonBox->addButton(zv_okButton, QDialogButtonBox::ActionRole);

    zv_cancelButton = new QPushButton(this);
    zv_cancelButton->setText(tr("Cancel"));

    buttonBox->addButton(zv_cancelButton, QDialogButtonBox::ActionRole);
}
//======================================================
void ZOperatorDialog::zh_createConnections()
{
    connect(zv_showSymbolsCheckBox, &QPushButton::toggled,
            this, &ZOperatorDialog::zh_showSymbolControl);
    connect(zv_okButton, &QPushButton::clicked,
            this,  &ZOperatorDialog::zh_checkDataAndAccept);
    connect(zv_cancelButton, &QPushButton::clicked,
            this,  &ZOperatorDialog::reject);
}
//======================================================
void ZOperatorDialog::zh_showSymbolControl(bool showFlag)
{
    QLineEdit::EchoMode echoMode = showFlag? QLineEdit::Normal : QLineEdit::Password;

    if(zv_oldPasswordLineEdit)
    {
        zv_oldPasswordLineEdit->setEchoMode(echoMode);
        zv_oldPasswordLineEdit->update();
    }
    zv_newPasswordFirstLineEdit->setEchoMode(echoMode);
    zv_newPasswordFirstLineEdit->update();

    zv_newPasswordSecondLineEdit->setEchoMode(echoMode);
    zv_newPasswordSecondLineEdit->update();
}
//======================================================
void ZOperatorDialog::zh_checkDataAndAccept()
{
    if(!zv_oldPasswordString.isEmpty() && zv_oldPasswordLineEdit->text() != zv_oldPasswordString)
    {
        QString msg = tr("Wrong password.");
        QMessageBox::critical(this, tr("Data check error"), msg, QMessageBox::Ok);
        return;
    }

    if(zv_loginLineEdit->text().isEmpty())
    {
        QString msg = tr("Empty login.");
        QMessageBox::critical(this, tr("Data check error"), msg, QMessageBox::Ok);
        return;
    }

    // check name
    if(zv_oldLogin != zv_loginLineEdit->text())
    {
        bool exists = false;
        emit zg_requestLoginExistance(zv_loginLineEdit->text(), exists);
        if(exists)
        {
            QString msg = tr("The login already exists.");
            QMessageBox::critical(this, tr("Data check error"), msg, QMessageBox::Ok);
            return;
        }
    }

    // compare passwords
    if(zv_newPasswordFirstLineEdit->text() != zv_newPasswordSecondLineEdit->text())
    {
        QString msg = tr("The first password input is not equal to the second one.");
        QMessageBox::critical(this, tr("Data check error"), msg, QMessageBox::Ok);
        return;
    }

    // try to encrypt password
    zv_passwordByteArray.clear();
    if(!zv_newPasswordFirstLineEdit->text().isEmpty() &&
            !zh_encryptStringToByteArray(zv_newPasswordFirstLineEdit->text(), zv_passwordByteArray))
    {
        QString msg = tr("Cannot encrypt the password.");
        QMessageBox::critical(this, tr("Data check error"), msg, QMessageBox::Ok);
        return;
    }

    accept();
}
//======================================================
bool ZOperatorDialog::zh_encryptStringToByteArray(const QString &string, QByteArray& byteArray)
{
    QBuffer buffer;
    buffer.open(QIODevice::WriteOnly);

    QDataStream dataStream(&buffer);
    dataStream.setVersion(QDataStream::Qt_5_1);
    dataStream << string;

    SimpleCrypt crypto(zv_cryptoKey);
    crypto.setCompressionMode(SimpleCrypt::CompressionAlways); //always compress the data, see section below
    crypto.setIntegrityProtectionMode(SimpleCrypt::ProtectionHash);  //properly protect the integrity of the data

    byteArray = crypto.encryptToByteArray(buffer.data());
    SimpleCrypt::Error cryptoError =  crypto.lastError();
    buffer.close();

    if (cryptoError == SimpleCrypt::ErrorNoError)
    {
        // do something relevant with the cyphertext, such as storing it or sending it over a socket to another machine.
        return true;
    }
    else
    {
        //            ErrorNoError,         /*!< No error occurred. */
        //            ErrorNoKeySet,        /*!< No key was set. You can not encrypt or decrypt without a valid key. */
        //            ErrorUnknownVersion,  /*!< The version of this data is unknown, or the data is otherwise not valid. */
        //            ErrorIntegrityFailed, /*!< The integrity check of the data failed. Perhaps the wrong key was used. */
        QString errorString;
        if(cryptoError == SimpleCrypt::ErrorNoKeySet)
        {
            errorString = tr("No key was set. You can not encrypt or decrypt without a valid key.") ;
        }
        else if(cryptoError == SimpleCrypt::ErrorUnknownVersion)
        {
            errorString = tr("The version of this data is unknown, or the data is otherwise not valid.");
        }
        else if(cryptoError == SimpleCrypt::ErrorIntegrityFailed)
        {
            errorString = tr("The integrity check of the data failed. Perhaps the wrong key was used.");
        }

        QMessageBox::warning(this, tr("Password settings"), errorString, QMessageBox::Ok);

        return false;
    }
}
//======================================================
bool ZOperatorDialog::zh_decryptByteArrayToString(QString& string, const QByteArray& byteArray)
{
    SimpleCrypt crypto(zv_cryptoKey);
    crypto.setCompressionMode(SimpleCrypt::CompressionAlways); //always compress the data, see section below
    crypto.setIntegrityProtectionMode(SimpleCrypt::ProtectionHash);  //properly protect the integrity of the data

    QByteArray decryptedArray;
    decryptedArray = crypto.decryptToByteArray(byteArray);
    SimpleCrypt::Error cryptoError =  crypto.lastError();

    if (cryptoError == SimpleCrypt::ErrorNoError)
    {
        // do something relevant with the cyphertext, such as storing it or sending it over a socket to another machine.
        //        QBuffer buffer;
        //        buffer.open(QIODevice::WriteOnly);

        QDataStream dataStream(&decryptedArray, QIODevice::ReadOnly);
        dataStream.setVersion(QDataStream::Qt_5_1);
        dataStream >> string;

        return true;
    }
    else
    {
        //            ErrorNoError,         /*!< No error occurred. */
        //            ErrorNoKeySet,        /*!< No key was set. You can not encrypt or decrypt without a valid key. */
        //            ErrorUnknownVersion,  /*!< The version of this data is unknown, or the data is otherwise not valid. */
        //            ErrorIntegrityFailed, /*!< The integrity check of the data failed. Perhaps the wrong key was used. */
        QString errorString;
        if(cryptoError == SimpleCrypt::ErrorNoKeySet)
        {
            errorString = tr("No key was set. You can not encrypt or decrypt without a valid key.") ;
        }
        else if(cryptoError == SimpleCrypt::ErrorUnknownVersion)
        {
            errorString = tr("The version of this data is unknown, or the data is otherwise not valid.");
        }
        else if(cryptoError == SimpleCrypt::ErrorIntegrityFailed)
        {
            errorString = tr("The integrity check of the data failed. Perhaps the wrong key was used.");
        }

        QMessageBox::warning(this, tr("Password settings"), errorString, QMessageBox::Ok);

        return false;
    }

}
//======================================================
