//======================================================
#ifndef ZADDSAMPLESPECTRUMONLY_H
#define ZADDSAMPLESPECTRUMONLY_H
//======================================================
#include <QDialog>
//======================================================
class QLineEdit;
class QPushButton;
class QSpinBox;
//======================================================
class ZAddSampleSpectrumOnlyDialog : public QDialog
{
    Q_OBJECT
public:
    explicit ZAddSampleSpectrumOnlyDialog(QWidget* parent = nullptr);
    ~ZAddSampleSpectrumOnlyDialog();

    int zp_gainFactor() const;
    int zp_exposition() const;
    QString zp_sampleNameTemplate() const;
    int zp_sampleQuantity() const;

signals:

protected:
    void closeEvent(QCloseEvent*) override;

private slots:

    void zh_onOkButtonClick();

private:
    // VARS
    const QString zv_defaultSampleTemplateString = tr("Sample");

    QPushButton* zv_okButton;
    QPushButton* zv_cancelButton;

    QSpinBox* zv_gainFactorSpinBox;
    QSpinBox* zv_expositionSpinBox;
    QLineEdit* zv_sampleNameLineEdit;
    QSpinBox* zv_quantitySpinBox;

    bool zv_saveControlValuesFlag;

    // FUNCS
    void zh_createActions();
    void zh_createComponents();
    void zh_createConnections();
    void zh_restoreSettings();
    void zh_saveSettings() const;

    bool zh_checkData() const;
};
//======================================================
#endif // ZADDSAMPLESPECTRUMONLY_H
