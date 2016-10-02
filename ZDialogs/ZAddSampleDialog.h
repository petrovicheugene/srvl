//======================================================
#ifndef ZADDSAMPLEDIALOG_H
#define ZADDSAMPLEDIALOG_H
//======================================================
#include <QDialog>
//======================================================
class QPushButton;
class QLineEdit;
class QComboBox;

//======================================================
class ZAddSampleDialog : public QDialog
{
    Q_OBJECT
public:
    explicit ZAddSampleDialog(QWidget *parent = 0);

    QString zp_sampleName() const;
    // bool zp_setSampleName(const QString& name);

signals:

    void zg_checkSampleName(const QString& samplename, bool& res);

public slots:


private slots:

    void zh_onOkButtonClick();

private:

    // VARS
    QPushButton* zv_okButton;
    QPushButton* zv_cancelButton;

    QLineEdit* zv_sampleNameLineEdit;
    QComboBox* zv_taskComboBox;

    // FUNCS
    void zh_createComponents();
    void zh_createConnections();
};
//======================================================
#endif // ZADDSAMPLEDIALOG_H
