//================================================
#ifndef ZPASSWORDDIALOG_H
#define ZPASSWORDDIALOG_H
//================================================
#include <QDialog>
//================================================
class QLineEdit;
class QPushButton;
class QCheckBox;

//================================================
class ZPasswordDialog : public QDialog
{
    Q_OBJECT
public:
    explicit ZPasswordDialog(const QString& password, QWidget *parent = nullptr);
    static bool mf_decryptByteArrayToString(QString& , const QByteArray& );

protected:

    // VARS
    QLineEdit* mv_passwordLineEdit ;
    QPushButton* mv_okButton;
    QPushButton* mv_cancelButton;
    QCheckBox* mv_showSymbolsCheckBox;
    QString mv_passwordString;

    // FUNCS

protected slots:

    void mf_showSymbolControl(bool);
    void mf_checkPassword();

signals:

public slots:

};
//================================================
#endif // ZPASSWORDDIALOG_H
