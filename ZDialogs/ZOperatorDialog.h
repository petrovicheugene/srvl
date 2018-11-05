//======================================================
#ifndef ZOPERATORDIALOG_H
#define ZOPERATORDIALOG_H
//======================================================
#include <QDialog>
//======================================================
class QCheckBox;
class QLineEdit;
class QPushButton;

//======================================================
class ZOperatorDialog : public QDialog
{
    Q_OBJECT
public:
    explicit ZOperatorDialog(QWidget *parent = nullptr);
    explicit ZOperatorDialog(const QString& operatorLogin, const QByteArray &password,
                             QWidget *parent = nullptr);
    QString zp_login() const;
    QByteArray zp_password() const;

signals:

    void zg_requestLoginExistance(const QString& login, bool& existance) const;

public slots:

private slots:

    void zh_showSymbolControl(bool showFlag);
    void zh_checkDataAndAccept();

private:

    // VARS
    QLineEdit* zv_loginLineEdit;
    QLineEdit* zv_oldPasswordLineEdit;
    QLineEdit* zv_newPasswordFirstLineEdit;
    QLineEdit* zv_newPasswordSecondLineEdit;

    QCheckBox* zv_showSymbolsCheckBox;

    QPushButton* zv_okButton;
    QPushButton* zv_cancelButton;
    QByteArray zv_passwordByteArray;

    QString zv_oldPasswordString;
    QString zv_oldLogin;

    const QString zv_operatorArrayPrefix = QString("Operators");
    const QString zv_loginString = QString("login");
    const QString zv_passwordString = QString("pw");

    const quint64 zv_cryptoKey = 0x0c2ad4a4acb9f023;

    // FUNCS
    bool zh_encryptStringToByteArray(const QString& , QByteArray&);
    bool zh_decryptByteArrayToString(QString& , const QByteArray&);


    void zh_createComponents(const QString &operatorLogin,
                             const QByteArray &passwordByteArray);
    void zh_createConnections();


};
//======================================================
#endif // ZOPERATORDIALOG_H
