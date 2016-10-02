//===============================================================
#ifndef ZSAMPLETASKDIALOG_H
#define ZSAMPLETASKDIALOG_H
//===============================================================
#include <QDialog>
//===============================================================
class QLineEdit;
class QPushButton;
class QSpinBox;
class QTableView;
class QSplitter;

class ZTaskMeasurementConditionTableWidget;
class ZChemicalTaskTableWidget;
class ZTaskCalibrationStackTableWidget;

//===============================================================
class ZSampleTaskDialog : public QDialog
{
    Q_OBJECT
public:
    explicit ZSampleTaskDialog(bool withQuantityFlag, QWidget *parent = 0);

    QString zp_sampleTaskName() const;
    QString zp_defaultSampleName() const;
    int zp_sampleTaskQuantity() const;

signals:

    void zg_checkTaskName(const QString& taskName, bool& res);

public slots:

protected:

    void closeEvent(QCloseEvent* event);

private slots:

    void zh_onOkButtonClick();

private:

    // VARS
    QLineEdit* zv_sampleTaskNameLineEdit;
    QLineEdit* zv_defaultSampleNameLineEdit;
    QSpinBox* zv_sampleTaskQuantitySpinBox;
    QPushButton* zv_okButton;
    QPushButton* zv_cancelButton;
    ZTaskMeasurementConditionTableWidget* zv_measurementTableWidget;
    ZChemicalTaskTableWidget* zv_chemicalTaskTableWidget;
    ZTaskCalibrationStackTableWidget* zv_calibrationStackTableWidget;
    QSplitter* zv_tableSplitter;
    // FUNCS

    void zh_createComponents(bool withQuantityFlag);
    void zh_createConnections();
    void zh_restoreSettings();
    void zh_saveSettings() const;
};
//===============================================================
#endif // ZSAMPLETASKDIALOG_H
