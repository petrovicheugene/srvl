//===============================================================
#ifndef ZCHEMICALDIALOG_H
#define ZCHEMICALDIALOG_H
//===============================================================
#include <QDialog>
//===============================================================
class QLabel;
class QLineEdit;
class QPushButton;
//===============================================================
class ZChemicalDialog : public QDialog
{
    Q_OBJECT
public:

    explicit ZChemicalDialog(QWidget *parent = 0);
    ~ZChemicalDialog();

    QString zp_chemical() const;
    int zp_chemicalId() const;
    void zp_setChemical(int id, const QString& chemical);

signals:

    void zg_checkChemical(int id, const QString& chemical, bool& res) const;

protected:


private slots:

    void zh_onOkButtonClick();
    void zh_resetMessageLabel(const QString &text);

private:

    // VARS
    QLineEdit* zv_chemicalLineEdit;
    QPushButton* zv_okButton;
    QPushButton* zv_cancelButton;
    QLabel* zv_messageLabel;
    int zv_id;

    // FUNCS
    void zv_createComponents();
    void zv_createConnections();
    void zh_restoreSettings();
    void zh_saveSettings() const;


};
//===============================================================
#endif // ZCHEMICALDIALOG_H
