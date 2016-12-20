//=====================================================
#ifndef ZRUNSQLCOMMANDDIALOG_H
#define ZRUNSQLCOMMANDDIALOG_H
//=====================================================
#include <QDialog>

//=====================================================
class QPushButton;
class QTextEdit;
class QTableView;
class QTableWidget;
class QListWidget;
class QSqlTableModel;
class QSplitter;
//=====================================================
class ZSQLCommanderDialog : public QDialog
{
    Q_OBJECT
public:
    explicit ZSQLCommanderDialog(QWidget *parent = 0);

signals:

public slots:


protected:

    void closeEvent(QCloseEvent* event);
    // bool eventFilter(QObject* object, QEvent* event);

private slots:

    void zh_onCurrentTableChange(int row);
    void zh_loadDatabaseTableList();
    void zh_addRow();
    void zh_removeRows();
    void zh_commitData();
    void zh_executeQuery();
    void zh_clearSelectedDataWidget();
    void zh_updateDataTable();
    void zh_setSortingDataTable(bool sort);
    void zh_setSortingSelectedDataWidget(bool sort);

private:

    // VARS
    QSplitter* zv_mainSplitter;
    QSplitter* zv_topSplitter;
    QSplitter* zv_bottomSplitter;

    QSqlTableModel* zv_dataModel;
    QTableView* zv_dataTableView;
    QListWidget* zv_tableListWidget;
    QTextEdit* zv_queryTextEdit;
    QTextEdit* zv_messageTextEdit;
    QTableWidget* zv_selectedDataTableWidget;

    QPushButton* zv_updateTableListButton;

    QPushButton* zv_updateDataTableButton;
    QPushButton* zv_sortDataTableButton;
    QPushButton* zv_newRowButton;
    QPushButton* zv_removeRowsButton;
    QPushButton* zv_commitButton;

    QPushButton* zv_clearQueryButton;
    QPushButton* zv_execQueryButton;

    QPushButton* zv_sortSelectedDataTableButton;
    QPushButton* zv_clearSelectedDataTableButton;

    QPushButton* zv_clearMessagesButton;

    QPushButton* zv_closeButton;

    // FUNCS

    void zh_createComponents();
    void zh_createConnections();

    QWidget* zh_createTableListWidget();
    QWidget* zh_createDataTableWidget();
    QWidget* zh_createQueryWidget();
    QWidget* zh_createSelectedDataWidget();
    QWidget* zh_createMessageWidget();

    void zh_restoreSettings();
    void zh_saveSettings() const;

};
//=====================================================
#endif // ZRUNSQLCOMMANDDIALOG_H
