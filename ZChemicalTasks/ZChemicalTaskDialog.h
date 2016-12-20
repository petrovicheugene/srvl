//=========================================================================
#ifndef ZCHEMICALTASKDIALOG_H
#define ZCHEMICALTASKDIALOG_H
//=========================================================================
#include <QDialog>
#include <QList>
//=========================================================================
class ZCalibrationTableWidget;
class ZChemicalTaskCalibrationModel;
class ZControlAction;

class QSqlTableModel;
class QLabel;
class QLineEdit;
class QPushButton;
class QTableView;
class QTextEdit;
//=========================================================================
class ZChemicalTaskDialog : public QDialog
{
    Q_OBJECT
public:
    explicit ZChemicalTaskDialog(QSqlTableModel* chemicalTableModel = 0,
                                 QWidget *parent = 0);
    bool zp_setChemicalId(int id);
    QString zp_chemicalTaskName() const;
    QString zp_description() const;
    int zp_exposition() const;
    int zp_gainFactor() const;
    void zp_calibrationIdList(QList<int> &calibrationIdList) const;
    void zp_concentrationLimitsList(QList<QPair<double, double> >& limitsList) const;

    bool zp_limitsForRow(int row, double& min, double& max) const;

signals:

    void zg_checkChemicalTaskName(const QString& chemicalTaskName, bool& res, QString& msg) const;

public slots:


protected:

    void closeEvent(QCloseEvent* event);

private slots:

    void zh_onOkButtonClick();
    void zh_onMoveCalibrationUpAction();
    void zh_onMoveCalibrationDownAction();
    void zh_onSelectCalibrationsAction();
    void zh_onRemoveCalibrationsAction();

    void zh_onCalibrationInserted(const QModelIndex& parent, int first, int last);

private:

    // VARS
    ZControlAction* zv_selectCalibrationsAction;
    ZControlAction* zv_removeCalibrationsAction;
    ZControlAction* zv_moveCalibrationUpAction;
    ZControlAction* zv_moveCalibrationDownAction;

    QSqlTableModel* zv_chemicalSQLTableModel;
    QSqlTableModel* zv_calibrationSQLTableModel;
    ZChemicalTaskCalibrationModel* zv_chemicalTaskCalibrationModel;

    QLineEdit* zv_chemicalTaskNameLineEdit;
    QLineEdit* zv_chemicalLineEdfit;
    QLineEdit* zv_measuringConditionsLineEdit;
    ZCalibrationTableWidget* zv_calibrationTableWidget;
    int zv_chemicalId;
    int zv_gainFactor;
    int zv_exposition;

    QTextEdit* zv_descriptionTextEdit;
    QLabel* zv_messageLabel;
    QPushButton* zv_okButton;
    QPushButton* zv_cancelButton;

    // FUNCS
    void zh_createActions();
    void zh_createComponents();
    void zh_createConnections();
    void zh_restoreSettings();
    void zh_saveSettings() const;

    void zh_setMeasuringConditions(int gainFactor, int exposition);

};
//=========================================================================
#endif // ZCHEMICALTASKDIALOG_H
