//===============================================================
#ifndef ZCALIBRATIONLISTDIALOG_H
#define ZCALIBRATIONLISTDIALOG_H
//===============================================================
#include <QDialog>
#include <QModelIndex>
#include <QList>
#include <QPair>
//===============================================================
class ZBaseTableWidget;
class ZCalibration;
class ZCalibrationFilterProxyModel;
class QSqlTableModel;
class QLabel;
class QSplitter;
class QStandardItemModel;
class QTableView;
//===============================================================
class ZCalibrationListDialog : public QDialog
{
    Q_OBJECT
public:
    explicit ZCalibrationListDialog(QSqlTableModel *chemicalTableModel = 0,
                                    QSqlTableModel* measuringConditionsSQLTableModel = 0,
                                    QSqlTableModel* calibrationTableSQLModel = 0,
                                    QWidget *parent = 0);

    bool zp_setFilter(int chemicalId , int gainFactor, int exposition);

    bool zp_currentChemical(int& id, QString& chemical) const;
    bool zp_currentMeasuringConditions(int& gainFactor, int& expositions) const;
    bool zp_selectedCalibrationIdList(QList<int>& calibrationIdList) const;

signals:

public slots:

protected:

    void closeEvent(QCloseEvent* event);

private slots:

    void zh_onCurrentChemicalChange(const QModelIndex& current, const QModelIndex previous);
    void zh_onCurrentMeasuringConditionsChange(const QModelIndex& current, const QModelIndex previous);
    void zh_onOkButtonClick();
    void zh_loadCalibrationsFromFile();
    void zh_removeSelectedCalibration();
    void zh_onCalibrationInsertion(const QModelIndex& parent, int first, int last);
    void zh_onCalibrationRemoving(const QModelIndex& parent, int first, int last);

private:

    // VARS
    QSqlTableModel* zv_chemicalTableSQLModel;
    QSqlTableModel* zv_calibrationTableSQLModel;
    QSqlTableModel* zv_measuringConditionsSQLTableModel;
    QStandardItemModel* zv_measuringConditionsModel;
    ZCalibrationFilterProxyModel* zv_calibrationProxyModel;

    QLabel* zv_filterInfoLabel;
    ZBaseTableWidget* zv_chemicalTableWidget;
    ZBaseTableWidget* zv_measuringConditionsTableWidget;
    ZBaseTableWidget* zv_calibrationTableWidget;

    QSplitter* zv_mainSplitter;
    QLabel* zv_messageLabel;

    QPushButton* zv_loadCalibrationButton;
    QPushButton* zv_removeCalibrationButton;
    QPushButton* zv_okButton;
    QPushButton* zv_cancelButton;
    QString zv_calibrationFolderPath;

//    QString zv_chemicalFilter;
//    int zv_gainFactorFilter;
//    int zv_expositionFilter;

    enum LoadingPermission {LP_ASK, LP_LOAD_ALL, LP_LOAD_CONFORMING};

    // FUNCS
    void zh_createComponents(QSqlTableModel *chemicalTableModel,
                             QSqlTableModel* measuringConditionsSQLTableModel,
                             QSqlTableModel* calibrationTableSQLModel);
    void zh_createConnections();

    void zh_restoreSettings();
    void zh_saveSettings() const;

    bool zh_checkChemicalExistance(const QString& chemical, int &chemicalId) const;
    bool zh_checkMeasuringConditionsExistance(int gainFactor, int exposition, int &row) const;

    int zh_findNewChemicalId() const;
    int zh_findNewCalibrationlId(QString &msg) const;
    bool zh_writeCalibrationToDatabase(const ZCalibration& calibration, const QByteArray &calibrationXMLByteArray, QString &msg);
    bool zh_writeChemicalToDatabase(const QString& chemical, int& id, QString &msg);
    bool zh_writeMeasuringConditionsToDatabase(int gainFactor, int exposition, QString &msg);

    bool zh_checkCalibrationConformity(const ZCalibration& calibration,
                                                                   LoadingPermission& loadingPermission) const;

    void zh_getMeasuringConditionsForCurrentChemical(QList<QPair<int, int> >& measuringConditionsList) const;

    void zh_repopulateMeasuringConditionsModel();
    void zh_setFilterToCalibrationTableSQLModel(const QModelIndex& chemicalIndex);

};
//===============================================================
#endif // ZCALIBRATIONLISTDIALOG_H
