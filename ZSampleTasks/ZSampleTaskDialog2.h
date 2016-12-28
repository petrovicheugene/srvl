//===============================================================
#ifndef ZSAMPLETASKDIALOG2_H
#define ZSAMPLETASKDIALOG2_H
//===============================================================
#include <QDialog>
#include "ZMeasuringTaskInitStruct.h"
//===============================================================
class QLabel;
class QLineEdit;
class QPushButton;
class QSpinBox;
class QSplitter;
class QTextEdit;
class QTableView;
class ZSampleTaskTreeWidget;
class ZSampleTaskTreeModel;
class QSqlTableModel;
//===============================================================
class ZSampleTaskDialog2 : public QDialog
{
    Q_OBJECT
public:
    explicit ZSampleTaskDialog2(QSqlTableModel* taskModel = 0, int sampleTaskId = -1, QWidget *parent = 0);

signals:

public slots:

private slots:

    void zh_onOkButtonClick();
    void zh_onLineEditDataChange(const QString& text);
    void zh_onModelRowCountChange(const QModelIndex& parent, int first, int last);

protected:

    void closeEvent(QCloseEvent* event);

private:

    // VARS
    int zv_sampleTaskId;
    QLineEdit* zv_sampleTaskNameLineEdit;
    QLineEdit* zv_sampleNameTemplateLineEdit;
    QTextEdit* zv_descriptionTextEdit;
    QLabel* zv_messageLabel;
    QPushButton* zv_okButton;
    QPushButton* zv_cancelButton;
    QSqlTableModel *zv_sampleTaskTableModel;
    ZSampleTaskTreeWidget* zv_sampleTaskTreeWidget;
    ZSampleTaskTreeModel* zv_sampleTaskTreeModel;

    // FUNCS
    void zh_createComponents();
    void zh_createConnections();
    void zh_restoreSettings();
    void zh_saveSettings() const;

    bool zh_checkData() const;
    bool zh_writeSampleTaskToDatabase();
    bool zh_removeSampleTaskFromTable(int row = -1);

    bool zh_loadSampleTask();
    bool zh_findNewMeasuringConditionsId(int &newId);


};
//===============================================================
#endif // ZSAMPLETASKDIALOG2_H
