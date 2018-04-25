//===========================================================
#ifndef ZMEASUREMENTCONDITIONSLISTDIALOG_H
#define ZMEASUREMENTCONDITIONSLISTDIALOG_H
//===========================================================
#include <QDialog>
#include <QModelIndex>
#include <QItemSelection>
//===========================================================
class QLabel;
class QLineEdit;
class QPushButton;
class QSqlTableModel;
class QSpinBox;
class QTableView;
//===========================================================
class ZMeasuringConditionsListDialog : public QDialog
{
    Q_OBJECT
public:
    explicit ZMeasuringConditionsListDialog(bool forSelection = true, QWidget *parent = 0);
    ~ZMeasuringConditionsListDialog();

    int zp_gainFactor() const;
    int zp_exposition() const;

    int zp_quantity() const;

signals:

public slots:

protected:

    bool eventFilter(QObject* object, QEvent* event);

private slots:

    void zh_onOkButtonClick();
    void zh_newConditions();
    void zh_checkMeasuringConditions(int gf, int expo, bool& res) const;
    void zh_onSelectionChange(const QItemSelection &current,
                               const QItemSelection &previous);

private:

    // VARS
    QSqlTableModel* zv_measuringConditionsModel;
    //QLineEdit* zv_selectedConditionsLineEdit;
//    QLineEdit* zv_selectedGainFactorLineEdit;
//    QLineEdit* zv_selectedExpositionLineEdit;


    QTableView* zv_measuringConditionsTable;
    QSpinBox* zv_quantitySpinBox;
    QLabel* zv_messageLabel;

    QPushButton* zv_newConditionsButton;
    QPushButton* zv_okButton;
    QPushButton* zv_cancelButton;
    const int zv_maxQuantity = 999999;
    const QString zv_noSelectedConditionsString = tr("None");

    // FUNCS
    void zh_createComponents(bool forSelection);
    void zh_createConnections();
    void zh_restoreSettings();
    void zh_saveSettings() const;


    int zh_currentDataFromColumn(int column) const;
    void zh_updateSelectedMeasuringConditionsString();

    bool zh_checkData();
};
//===========================================================
#endif // ZMEASUREMENTCONDITIONSLISTDIALOG_H
