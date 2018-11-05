//================================================
#include "ZPasswordDialog.h"
#include <QPushButton>
#include <QLineEdit>
#include <QCheckBox>
#include <QVBoxLayout>
#include <QLabel>
#include <QDialogButtonBox>
//#include <QSettings>
#include <QMessageBox>
#include <QDataStream>
#include "simplecrypt.h"


//================================================
ZPasswordDialog::ZPasswordDialog(const QString &password, QWidget *parent) :
    QDialog(parent)
{
    setWindowTitle(tr("Password"));

    mv_passwordString = password;

    QVBoxLayout* mainLayout = new QVBoxLayout;
    setLayout(mainLayout);

    QLabel* label = new QLabel(this);
    label->setText(tr("Enter password"));
    mv_passwordLineEdit = new QLineEdit();
    mv_passwordLineEdit->setEchoMode(QLineEdit::Password);
    label->setBuddy(mv_passwordLineEdit);
    mainLayout->addWidget(label);
    mainLayout->addWidget(mv_passwordLineEdit);
    mainLayout->addSpacing(10);

    mv_showSymbolsCheckBox = new QCheckBox(this);
    mv_showSymbolsCheckBox->setText(tr("Show symbols"));
    connect(mv_showSymbolsCheckBox, SIGNAL(toggled(bool)), this, SLOT(mf_showSymbolControl(bool)));
    mainLayout->addWidget(mv_showSymbolsCheckBox);
    mainLayout->addSpacing(10);

    QDialogButtonBox* buttonBox = new QDialogButtonBox(this);
    mainLayout->addWidget(buttonBox);

    mv_okButton = new QPushButton(this);
    mv_okButton->setText(tr("OK"));
    connect(mv_okButton, SIGNAL(clicked()), this, SLOT(mf_checkPassword()) );
    buttonBox->addButton(mv_okButton, QDialogButtonBox::ActionRole);


    mv_cancelButton = new QPushButton(this);
    mv_cancelButton->setText(tr("Cancel"));
    connect(mv_cancelButton, SIGNAL(clicked()), this, SLOT(reject()) );
    buttonBox->addButton(mv_cancelButton, QDialogButtonBox::ActionRole);

}
//================================================
void ZPasswordDialog::mf_showSymbolControl(bool showFlag)
{
    QLineEdit::EchoMode echoMode = showFlag? QLineEdit::Normal : QLineEdit::Password;

    mv_passwordLineEdit->setEchoMode(echoMode);
    mv_passwordLineEdit->update();
}
//================================================
void ZPasswordDialog::mf_checkPassword()
{
    if(mv_passwordLineEdit->text() != mv_passwordString)
    {
        QMessageBox::critical(this, tr("Password"), tr("Wrong password"), QMessageBox::Ok);
        return;
    }

    accept();
}
//=======================================================
bool ZPasswordDialog::mf_decryptByteArrayToString(QString& string, const QByteArray& byteArray)
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

        QMessageBox::warning(0, tr("Password settings"), errorString, QMessageBox::Ok);

        return false;
    }

}
//=======================================================
