//===============================================================
#ifndef ZSAMPLETASKDIALOG_H
#define ZSAMPLETASKDIALOG_H
//===============================================================
#include "ZMeasuringTaskInitStruct.h"
#include <QDialog>
//===============================================================
class QLabel;
class QLineEdit;
class QPushButton;
class QSpinBox;
class QSplitter;
class QTextEdit;
class QTableView;

//class ZTaskMeasuringConditionTableWidget;
//class ZChemicalTaskTableWidget;
//class ZTaskCalibrationStackTableWidget;
class ZSampleTaskTreeWidget;
class ZSampleTaskTreeModel;
//===============================================================
class ZSampleTaskDialog : public QDialog
{
    Q_OBJECT
public:
    explicit ZSampleTaskDialog(QWidget *parent = 0);

    QString zp_sampleTaskName() const;
    QString zp_sampleNameTemplate() const;
    QString zp_description() const;
    QList<ZMeasuringTaskInitStruct> zp_measuringTaskInitList() const;

    bool zp_loadSampleTask(int sampleTaskId,
                           const QString& sampleTaskName,
                           const QString& sampleNameTemplate,
                           const QString& description,
                           const QList<ZMeasuringTaskInitStruct>& measuringTaskInitStructList);


signals:

    void zg_checkTaskName(int id, const QString& taskName, bool& res);

public slots:

protected:

    void closeEvent(QCloseEvent* event);

private slots:

    void zh_onOkButtonClick();
    void zh_onLineEditDataChange(const QString& text);
    void zh_onModelRowCountChange(const QModelIndex& parent, int first, int last);

private:

    // VARS
    int zv_sampleTaskId;
    QLineEdit* zv_sampleTaskNameLineEdit;
    QLineEdit* zv_sampleNameTemplateLineEdit;
    QTextEdit* zv_descriptionTextEdit;
    QLabel* zv_messageLabel;
    QPushButton* zv_okButton;
    QPushButton* zv_cancelButton;

    ZSampleTaskTreeWidget* zv_sampleTaskTreeWidget;
    ZSampleTaskTreeModel* zv_sampleTaskTreeModel;

    //    ZTaskMeasuringConditionTableWidget* zv_measuringTableWidget;
    //    ZChemicalTaskTableWidget* zv_chemicalTaskTableWidget;
    //    ZTaskCalibrationStackTableWidget* zv_calibrationStackTableWidget;
    //QSplitter* zv_tableSplitter;

    // FUNCS
    void zh_createComponents();
    void zh_createConnections();
    void zh_restoreSettings();
    void zh_saveSettings() const;
    bool zh_checkDataEmptiness();

};
//===============================================================
#endif // ZSAMPLETASKDIALOG_H
