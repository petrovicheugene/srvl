//======================================================
#ifndef ZADDSAMPLEDIALOG_H
#define ZADDSAMPLEDIALOG_H
//======================================================
#include <QDialog>
#include <QModelIndex>
#include <QItemSelection>
#include "ZSampleTaskDialog2.h"
//======================================================
class ZControlAction;
class ZSampleTaskTableWidget;

class QPushButton;
class QLabel;
class QLineEdit;
class QSpinBox;
class QSqlTableModel;
// class QTableView;

//======================================================
class ZAddSampleDialog : public QDialog
{
    Q_OBJECT
public:
    explicit ZAddSampleDialog(QWidget *parent = 0);
    ~ZAddSampleDialog();

    QString zp_sampleName() const;
    int zp_sampleQuantity() const;
    int zp_sampleTaskId() const;

signals:


protected:

    bool eventFilter(QObject *object, QEvent *event);

public slots:


private slots:

    void zh_onOkButtonClick();
    void zh_onNewSampleTaskButtonClick() const;
    void zh_onEditSampleTaskButtonClick();
    void zh_onRemoveSampleTaskButtonClick();
    void zh_onReviewSampleTaskButtonClick() const;
    void zh_onSelectionChange(const QItemSelection &current, const QItemSelection &previous);
    void zh_onSampleNameChange(const QString& text);
    void zh_checkTaskName(int sampleTaskId, const QString& taskName, bool &res) const;

private:

    // VARS
    ZControlAction* zv_newSampleTaskAction;
    ZControlAction* zv_copySampleTaskAction;
    ZControlAction* zv_reviewSampleTaskAction;
    ZControlAction* zv_removeSampleTaskAction;

    QPushButton* zv_okButton;
    QPushButton* zv_cancelButton;

    ZSampleTaskTableWidget* zv_sampleTaskTableWidget;
    QSqlTableModel* zv_sampleTaskTableModel;

    QLineEdit* zv_sampleNameLineEdit;
    //QLineEdit* zv_taskNameLineEdit;
    QSpinBox* zv_quantitySpinBox;
    QLabel* zv_messageLabel;
    const QString zv_noSelectedTaskString = tr("None");
    int zv_selectedSampleTaskId;

    // FUNCS
    void zh_createActions();
    void zh_createComponents();
    void zh_createConnections();
    void zh_restoreSettings();
    void zh_saveSettings() const;
    bool zh_checkData();
    void zh_updateSeletedTaskPropertiesStrings();
    bool zh_removeSampleTaskFromTable(int row = -1);
    void zh_saveNewSampleTaskToDatabase(ZSampleTaskDialog2 &dialog);

};
//======================================================
#endif // ZADDSAMPLEDIALOG_H
