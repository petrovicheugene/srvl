//===============================================================
#ifndef ZCHEMICALTASKLISTDIALOG_H
#define ZCHEMICALTASKLISTDIALOG_H
//===============================================================
#include <QDialog>
//===============================================================
class QLabel;
class QPushButton;
class QTableView;
class QSqlTableModel;
class QSplitter;

class ZControlAction;
class ZChemicalTableWidget;
class ZChemicalTaskTableWidget;
//===============================================================
class ZChemicalTaskListDialog : public QDialog
{
    Q_OBJECT
public:
    explicit ZChemicalTaskListDialog(QWidget *parent = 0);

signals:

public slots:

protected:

    void closeEvent(QCloseEvent* event);

private slots:

    void zh_onChemicalChange(const QModelIndex& current, const QModelIndex& previous);
    void zh_onChemicalTaskChange(const QModelIndex& current, const QModelIndex& previous);

    void zh_onOkButtonClick();
    void zh_onNewChemicalAction();
    void zh_onEditChemicalAction();
    void zh_onNewChemicalTaskAction();
    void zh_onEditChemicalTaskAction() const;
    void zh_onReviewChemicalTaskAction() const;

    void zh_checkChemical(int id, const QString& chemical, bool& res) const;
    void zh_checkChemicalTaskName(const QString& chemicalTaskName, bool &res, QString& msg) const;

private:

    // VARS
    ZControlAction* zv_newChemicalAction;
    ZControlAction* zv_editChemicalAction;

    ZControlAction* zv_newChemicalTaskAction;
    ZControlAction* zv_editChemicalTaskAction;
    ZControlAction* zv_reviewChemicalTaskAction;

    QSqlTableModel* zv_chemicalTableModel;
    QSqlTableModel* zv_calibrationStackTableModel;

    QSplitter* zv_mainSplitter;
    ZChemicalTableWidget* zv_chemicalTableWidget;
    ZChemicalTaskTableWidget* zv_chemicalTaskTableWidget;

    QPushButton* zv_okButton;
    QPushButton* zv_cancelButton;

    // FUNCS

    void zh_createActions();
    void zh_createComponents();
    void zh_createConnections();

    void zh_restoreSettings();
    void zh_saveSettings() const;

    int zh_findNewChemicalTaskId() const;


};
//===============================================================
#endif // ZCHEMICALTASKLISTDIALOG_H
