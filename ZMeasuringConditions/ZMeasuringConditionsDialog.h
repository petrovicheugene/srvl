//=======================================================
#ifndef ZMEASURINGCONDITIONSDIALOG_H
#define ZMEASURINGCONDITIONSDIALOG_H
//=======================================================
#include <QDialog>
//=======================================================
class QSpinBox;
class QPushButton;
class QLabel;
//=======================================================
class ZMeasuringConditionsDialog : public QDialog
{
    Q_OBJECT
public:
    explicit ZMeasuringConditionsDialog(QWidget *parent = 0);
    ~ZMeasuringConditionsDialog();

    int zp_gainFactor() const;
    int zp_exposition() const;

signals:

    void zg_checkMeasuringConditions(int gf, int expo, bool& res) const;

protected:


private slots:

    void zh_onOkButtonClick();
    void zh_resetMessageLabel(int);

private:

    // VARS
    QSpinBox* zv_gainFactorSpinBox;
    QSpinBox* zv_expositionSpinBox;
    QPushButton* zv_okButton;
    QPushButton* zv_cancelButton;
    QLabel* zv_messageLabel;

    // FUNCS
    void zh_createComponents();
    void zh_createConnections();
    void zh_restoreSettings();
    void zh_saveSettings() const;

};
//=======================================================
#endif // ZMEASURINGCONDITIONSDIALOG_H
