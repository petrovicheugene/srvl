//=======================================================
#ifndef ZPASSWORDCHANGINGDIALOG_H
#define ZPASSWORDCHANGINGDIALOG_H
//=======================================================
#include <QDialog>
#include <QVariant>
//=======================================================
class QPushButton;
class QLineEdit;
class QCheckBox;
//=======================================================
class ZPasswordChangeDialog : public QDialog
{
    Q_OBJECT
public:
    explicit ZPasswordChangeDialog(QWidget *parent = nullptr);


protected:

    // VARS
    QLineEdit* mv_oldPasswordLine;
    QLineEdit* mv_newPasswordFirstLine;
    QLineEdit* mv_newPasswordSecondLine;

    QCheckBox* mv_showSymbolsChekBox;

    QPushButton* mv_okButton;
    QPushButton* mv_cancelButton;
    //QVariant mv_vPassword;

    QString mv_oldPasswordString;
    // FUNCS

    bool mf_encryptStringToByteArray(const QString& , QByteArray&);
    bool mf_decryptByteArrayToString(QString& , const QByteArray&);

signals:

protected slots:

    void mf_changePassword();
    void mf_showSymbolControl(bool);

public slots:


};
//=======================================================
#endif // ZPASSWORDCHANGINGDIALOG_H
