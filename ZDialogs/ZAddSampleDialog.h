//======================================================
#ifndef ZADDSAMPLEDIALOG_H
#define ZADDSAMPLEDIALOG_H
//======================================================
#include <QDialog>
//======================================================
class QPushButton;
class QLineEdit;
class QComboBox;
class QSpinBox;
//======================================================
class ZAddSampleDialog : public QDialog
{
    Q_OBJECT
public:
    explicit ZAddSampleDialog(QWidget *parent = 0);

    QString zp_sampleName() const;
    int zp_sampleQuantity() const;

signals:

    // void zg_checkSampleName(const QString& samplename, bool& res);

public slots:


private slots:

    void zh_onOkButtonClick();
    void zh_onSampleNameChange(const QString &text);

private:

    // VARS
    QPushButton* zv_okButton;
    QPushButton* zv_cancelButton;

    QLineEdit* zv_sampleNameLineEdit;
    QSpinBox* zv_quantitySpinBox;
    QComboBox* zv_taskComboBox;

    // FUNCS
    void zh_createComponents();
    void zh_createConnections();
};
//======================================================
#endif // ZADDSAMPLEDIALOG_H
