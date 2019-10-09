//=======================================================
#include "ZPasswordChangeDialog.h"

#include <QApplication>
#include <QSettings>
#include "ZGeneral.h"
#include <QPushButton>
#include <QCheckBox>
#include <QLineEdit>
#include <QVBoxLayout>
#include <QDialogButtonBox>
#include <QLabel>
#include <QBuffer>
#include <QDataStream>
#include <QMessageBox>

#include "simplecrypt.h"

//=======================================================
ZPasswordChangeDialog::ZPasswordChangeDialog(QWidget *parent) :
    QDialog(parent)
{
    setWindowTitle(tr("Password settings"));

    QSettings settings;
    settings.beginGroup(qApp->applicationVersion());
    settings.beginGroup("AppSettings");
    QVariant vPassword = settings.value("pw");
    settings.endGroup();
    settings.endGroup();

    if(vPassword.isValid() && !vPassword.isNull())
    {
        QByteArray byteArray = vPassword.toByteArray();
        if(!byteArray.isEmpty())
        {
            if(!mf_decryptByteArrayToString(mv_oldPasswordString, byteArray))
            {
                mv_oldPasswordString = QString();
            }
        }
    }


    QVBoxLayout* mainLayout = new QVBoxLayout;
    setLayout(mainLayout);
    QLabel* label = 0;

    if(!mv_oldPasswordString.isEmpty())
    {
        label = new QLabel(this);
        label->setText(tr("Enter current password"));

        mv_oldPasswordLine = new QLineEdit(this);
        mv_oldPasswordLine->setEchoMode(QLineEdit::Password);
        label->setBuddy(mv_oldPasswordLine);

        mainLayout->addWidget(label);
        mainLayout->addWidget(mv_oldPasswordLine);
        mainLayout->addSpacing(10);

    }
    else
    {
        mv_oldPasswordLine = 0;
    }

    label = new QLabel(this);
    label->setText(tr("Enter new password"));

    mv_newPasswordFirstLine = new QLineEdit(this);
    mv_newPasswordFirstLine->setEchoMode(QLineEdit::Password);
    label->setBuddy(mv_newPasswordFirstLine);

    mainLayout->addWidget(label);
    mainLayout->addWidget(mv_newPasswordFirstLine);
    mainLayout->addSpacing(10);

    label = new QLabel(this);
    label->setText(tr("Repeat new password"));

    mv_newPasswordSecondLine = new QLineEdit(this);
    mv_newPasswordSecondLine->setEchoMode(QLineEdit::Password);
    label->setBuddy(mv_newPasswordSecondLine);

    mainLayout->addWidget(label);
    mainLayout->addWidget(mv_newPasswordSecondLine);
    mainLayout->addSpacing(10);


    mv_showSymbolsChekBox = new QCheckBox(this);
    mv_showSymbolsChekBox->setText(tr("Show symbols"));
    connect(mv_showSymbolsChekBox, SIGNAL(toggled(bool)), this, SLOT(mf_showSymbolControl(bool)));
    mainLayout->addWidget(mv_showSymbolsChekBox);
    mainLayout->addSpacing(10);


    QDialogButtonBox* buttonBox= new QDialogButtonBox(this);
    mainLayout->addWidget(buttonBox);

    mv_okButton = new QPushButton(this);
    mv_okButton->setText(tr("OK"));
    connect(mv_okButton, SIGNAL(clicked()), this, SLOT(mf_changePassword()));
    buttonBox->addButton(mv_okButton, QDialogButtonBox::ActionRole);

    mv_cancelButton = new QPushButton(this);
    mv_cancelButton->setText(tr("Cancel"));
    connect(mv_cancelButton, SIGNAL(clicked()), this, SLOT(reject()));
    buttonBox->addButton(mv_cancelButton, QDialogButtonBox::ActionRole);
}
//=======================================================
void ZPasswordChangeDialog::mf_changePassword()
{
    // current password check out
    if(!mv_oldPasswordString.isEmpty() )
    {
        if(mv_oldPasswordLine->text().isEmpty())
        {
            QMessageBox::warning(this, tr("Password settings"), tr("Enter current password"), QMessageBox::Ok);
            return;
        }

        // current password decryption to str

//        QString oldPasswordString;
//        QByteArray byteArray = mv_vPassword.toByteArray();
//        if(!mf_decryptByteArrayToString(oldPasswordString, byteArray))
//        {
//            return;
//        }

        // comparing current password to entered password
        if(mv_oldPasswordString != mv_oldPasswordLine->text())
        {
            QMessageBox::warning(this, tr("Password settings"), tr("Wrong current password"), QMessageBox::Ok);
            return;
        }
    }

    // new password check out
    if(mv_newPasswordFirstLine->text() != mv_newPasswordSecondLine->text())
    {
        QMessageBox::warning(this, tr("Password settings"), tr("Wrong new password"), QMessageBox::Ok);
        return;
    }

    QByteArray byteArray;
    if(!mv_newPasswordFirstLine->text().isEmpty())
    {
        QString passwordString = mv_newPasswordFirstLine->text();
        if(!mf_encryptStringToByteArray(passwordString, byteArray))
        {
            return;
        }
    }

    QString str;
    mf_decryptByteArrayToString(str, byteArray);
//    accept();
//    return;

    QSettings settings;
    settings.beginGroup(qApp->applicationVersion());
    settings.beginGroup("AppSettings");
    settings.setValue("pw", QVariant(byteArray));
    settings.endGroup();
    settings.endGroup();
    accept();
}
//=======================================================
void ZPasswordChangeDialog::mf_showSymbolControl(bool showFlag)
{
    QLineEdit::EchoMode echoMode = showFlag? QLineEdit::Normal : QLineEdit::Password;

    if(mv_oldPasswordLine)
    {
        mv_oldPasswordLine->setEchoMode(echoMode);
        mv_oldPasswordLine->update();
    }
    mv_newPasswordFirstLine->setEchoMode(echoMode);
    mv_newPasswordFirstLine->update();

    mv_newPasswordSecondLine->setEchoMode(echoMode);
    mv_newPasswordSecondLine->update();
}
//=======================================================
bool ZPasswordChangeDialog::mf_encryptStringToByteArray(const QString &string, QByteArray& byteArray)
{
    QBuffer buffer;
    buffer.open(QIODevice::WriteOnly);

    QDataStream dataStream(&buffer);
    dataStream.setVersion(QDataStream::Qt_5_1);
    dataStream << string;

    SimpleCrypt crypto(Q_UINT64_C(0x0c2ad4a4acb9f023)); //some random number
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
//=======================================================
bool ZPasswordChangeDialog::mf_decryptByteArrayToString(QString& string, const QByteArray& byteArray)
{
    SimpleCrypt crypto(Q_UINT64_C(0x0c2ad4a4acb9f023)); //some random number
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
//=======================================================
