//======================================================
#ifndef ZSERIESMEASUREMENTDIALOG_H
#define ZSERIESMEASUREMENTDIALOG_H
//======================================================
#include "ZBaseDialog.h"
//======================================================
class QPushButton;
class QSplitter;
class QSqlTableModel;
class QTableView;

class ZPlotter;
//======================================================
class ZSeriesMeasurementDialog : public ZBaseDialog
{
    Q_OBJECT
public:
    explicit ZSeriesMeasurementDialog(QWidget *parent = nullptr);
    ~ZSeriesMeasurementDialog() override;

signals:

public slots:


private slots:

    void zh_onCloseButtonClick();

protected:

    void zh_saveSettings() override;
    void zh_restoreSettings() override;

private:

    // VARS
    QSqlTableModel* zv_seriesNameTableModel;
    QSqlTableModel* zv_seriesTableModel;


    QSplitter* zv_mainSplitter;
    QSplitter* zv_tableSplitter;
    const QString zv_mainSplitterStateName = "mainSplitterState";
    const QString zv_tableSplitterStateName = "tableSplitterState";


    QTableView* zv_seriesListView;
    QTableView* zv_seriesResultView;
    ZPlotter* zv_plotter;

    QPushButton* zv_closeButton;

    // FUNCS
    void zh_createComponents();
    void zh_createConnections();

    QWidget* zh_createTablesWidget();


};
//======================================================
#endif // ZSERIESMEASUREMENTDIALOG_H
