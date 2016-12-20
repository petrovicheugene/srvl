//=========================================================
#ifndef ZSETGAINFACTORTOCALIBRATIONDIALOG_H
#define ZSETGAINFACTORTOCALIBRATIONDIALOG_H
//=========================================================
#include <QDialog>
//=========================================================
class QPushButton;
class QSpinBox;
//=========================================================
class ZSetGainFactorToCalibrationDialog : public QDialog
{
    Q_OBJECT
public:
    explicit ZSetGainFactorToCalibrationDialog(const QString& calibrationName, QWidget *parent = 0);

    int zp_gainFactor() const;

signals:

public slots:


private:
    // VARS

    QSpinBox* zv_gainFactorSpinBox;
    QPushButton* zv_okButton;
    QPushButton* zv_cancelButton;


    // FUNCS
    void zh_createComponents(const QString &calibrationName);
    void zh_createConnections();


};
//=========================================================
#endif // ZSETGAINFACTORTOCALIBRATIONDIALOG_H
