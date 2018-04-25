//===============================================================
#ifndef ZCHEMICALLISTDIALOG_H
#define ZCHEMICALLISTDIALOG_H
//===============================================================
#include <QDialog>
//===============================================================
class ZControlAction;
class QLabel;
class QPushButton;
class ZChemicalTableWidget;
class QSqlTableModel;
//===============================================================
class ZChemicalListDialog : public QDialog
{
    Q_OBJECT
public:
    explicit ZChemicalListDialog(QSqlTableModel* chemicalModel = 0, QWidget *parent = 0);
    ~ZChemicalListDialog();

    int zp_chemicalId() const;
    QString zp_chemical() const;

signals:

public slots:


protected:

    bool eventFilter(QObject* object, QEvent* event);

private slots:

    void zh_onOkButtonClick();
    void zh_onNewChemicalAction();
    void zh_onEditChemicalAction();
    void zh_checkChemical(int id, const QString& chemical, bool& res) const;
    void zh_onChemicalChange(const QModelIndex& current, const QModelIndex& previous);

private:

    // VARS
    ZControlAction* zv_newChemicalAction;
    ZControlAction* zv_editChemicalAction;

    QSqlTableModel* zv_chemicalTableModel;
    ZChemicalTableWidget* zv_chemicalTableWidget;

    QLabel* zv_messageLabel;
    QPushButton* zv_okButton;
    QPushButton* zv_cancelButton;

    // FUNCS
    void zh_createActions();
    void zh_createComponents();
    void zh_createConnections();
    void zh_restoreSettings();
    void zh_saveSettings() const;
    bool zh_checkData();

};
//===============================================================
#endif // ZCHEMICALLISTDIALOG_H
