//=======================================================
#ifndef ZSAMPLETASKLISTDIALOG_H
#define ZSAMPLETASKLISTDIALOG_H
//=======================================================
#include <QDialog>
//=======================================================
class QLabel;
class QPushButton;
class QSqlTableModel;
class QTableView;
//=======================================================
class ZSampleTaskListDialog : public QDialog
{
    Q_OBJECT
public:
    explicit ZSampleTaskListDialog(QWidget *parent = 0);
    ~ZSampleTaskListDialog();

signals:

protected:

    bool eventFilter(QObject* object, QEvent* event);

private slots:

    void zh_onOkButtonClick() const;
    void zh_newSampleTask();
    void zh_checkTaskName(int sampleTaskId, const QString& taskName, bool &res) const;

private:

    // VARS
    QSqlTableModel* zv_sampleTaskTableModel;
    QTableView* zv_sampleTaskTable;
    QLabel* zv_messageLabel;

    QPushButton* zv_newTaskButton;
    QPushButton* zv_okButton;
    QPushButton* zv_cancelButton;


    // FUNCS
    void zh_createComponents();
    void zh_createConnections();
    void zh_restoreSettings();
    void zh_saveSettings() const;

    bool zh_removeSampleTaskFromTable(int row = -1);

};
//=======================================================
#endif // ZSAMPLETASKLISTDIALOG_H
