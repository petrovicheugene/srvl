//=======================================================
#include "ZSampleTaskListDialog.h"
#include "ZGeneral.h"
#include "ZSampleTaskDialog2.h"
#include "ZMeasuringTaskInitStruct.h"

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QDialogButtonBox>

#include <QEvent>
#include <QHeaderView>
#include <QLabel>
#include <QMouseEvent>
#include <QMessageBox>
#include <QPushButton>
#include <QTableView>
#include <QSettings>
#include <QSqlError>
#include <QSqlTableModel>
#include <QSqlRecord>
#include <QSqlField>
//=======================================================
ZSampleTaskListDialog::ZSampleTaskListDialog(QWidget *parent)
    : QDialog(parent)
{
    setWindowTitle(tr("Sample task"));
    setWindowFlags(Qt::Tool);

    zh_createComponents();
    zh_createConnections();
    zh_restoreSettings();
}
//=======================================================
ZSampleTaskListDialog::~ZSampleTaskListDialog()
{
    zh_saveSettings();
}
//=======================================================
bool ZSampleTaskListDialog::eventFilter(QObject* object, QEvent* event)
{
    if(object == zv_sampleTaskTable->viewport() && event->type() == QEvent::MouseButtonDblClick)
    {
        QMouseEvent* mouseEvent = static_cast<QMouseEvent*>(event);
        if(mouseEvent && mouseEvent->button() == Qt::LeftButton)
        {
            zh_onOkButtonClick();
            return true;
        }
    }

    return QDialog::eventFilter(object, event);
}
//=======================================================
void ZSampleTaskListDialog::zh_createComponents()
{
    // model
    zv_sampleTaskTableModel = new QSqlTableModel(this);
    zv_sampleTaskTableModel->setTable("sample_tasks");
    zv_sampleTaskTableModel->setEditStrategy(QSqlTableModel::OnManualSubmit);
    zv_sampleTaskTableModel->select();

    QVBoxLayout* mainLayout = new QVBoxLayout(this);
    setLayout(mainLayout);

    // name
    //    QLabel* label = new QLabel(this);
    //    label->setText(glCreateCaption(tr("Sample tasks:")));
    //    mainLayout->addWidget(label);

    zv_sampleTaskTable = new QTableView(this);
    mainLayout->addWidget(zv_sampleTaskTable);

    zv_messageLabel = new QLabel(this);
    zv_messageLabel->setWordWrap(true);
    mainLayout->addWidget(zv_messageLabel);

    // basement
    // Separator
    QFrame* line = new QFrame(this);
    line->setFrameStyle(QFrame::HLine | QFrame::Sunken);
    line->setLineWidth(1);
    mainLayout->addWidget(line);

    QDialogButtonBox* buttonBox = new QDialogButtonBox(this);
    mainLayout->addWidget(buttonBox);

    // buttons
    zv_newTaskButton = new QPushButton(NS_Buttons::glButtonNew, this);
    buttonBox->addButton(zv_newTaskButton, QDialogButtonBox::ActionRole);

    zv_okButton = new QPushButton(NS_Buttons::glButtonOk, this);
    buttonBox->addButton(zv_okButton, QDialogButtonBox::ActionRole);

    zv_cancelButton = new QPushButton(NS_Buttons::glButtonCancel, this);
    buttonBox->addButton(zv_cancelButton, QDialogButtonBox::ActionRole);
}
//======================================================
void ZSampleTaskListDialog::zh_createConnections()
{
    zv_sampleTaskTable->setModel(zv_sampleTaskTableModel);
    zv_sampleTaskTable->setColumnHidden(0, true);

    connect(zv_okButton, &QPushButton::clicked,
            this, &ZSampleTaskListDialog::zh_onOkButtonClick);
    connect(zv_cancelButton, &QPushButton::clicked,
            this, &ZSampleTaskListDialog::reject);
    connect(zv_newTaskButton, &QPushButton::clicked,
            this, &ZSampleTaskListDialog::zh_newSampleTask);

}
//======================================================
void ZSampleTaskListDialog::zh_restoreSettings()
{
    QSettings settings;
    QVariant vData;
    settings.beginGroup(glAppVersion);
    settings.beginGroup("SampleTaskListDialog");

    vData = settings.value("dialogGeometry");
    if(vData.isValid() && !vData.isNull() && vData.canConvert<QByteArray>())
    {
        this->restoreGeometry(vData.value<QByteArray>());
    }

    settings.endGroup();
    settings.endGroup();
}
//======================================================
void ZSampleTaskListDialog::zh_saveSettings() const
{
    QSettings settings;
    settings.beginGroup(glAppVersion);
    settings.beginGroup("SampleTaskListDialog");

    settings.setValue("dialogGeometry", QVariant::fromValue<QByteArray>(this->saveGeometry()));
    //settings.setValue("splitterGeometry", QVariant::fromValue<QByteArray>(zv_tableSplitter->saveState()));

    settings.endGroup();
    settings.endGroup();
}
//======================================================
void ZSampleTaskListDialog::zh_onOkButtonClick() const
{

}
//======================================================
void ZSampleTaskListDialog::zh_newSampleTask()
{
    ZSampleTaskDialog2 dialog;
//    connect(&dialog, &ZSampleTaskDialog2::zg_checkTaskName,
//            this, &ZSampleTaskListDialog::zh_checkTaskName);

    if(!dialog.exec())
    {
        return;
    }

//    // get and check input data
//    QString sampleTaskName = dialog.zp_sampleTaskName();
//    QString sampleNameTemplate = dialog.zp_sampleNameTemplate();
//    QString description = dialog.zp_description();
//    QList<ZMeasuringTaskInitStruct> measuringTaskInitList = dialog.zp_measuringTaskInitList();

//    // sample task (table sample_tasks)
//    QSqlRecord record;
//    record.append(QSqlField("id", QVariant::Int));
//    record.append(QSqlField("name", QVariant::String));
//    record.append(QSqlField("sample_name_template", QVariant::String));
//    record.append(QSqlField("description", QVariant::String));

//    record.setValue(1, QVariant(sampleTaskName));
//    record.setValue(2, QVariant(sampleNameTemplate));
//    record.setValue(3, QVariant(description));

//    if(!zv_sampleTaskTableModel->insertRecord(-1, record))
//    {
//        QString msg = zv_sampleTaskTableModel->lastError().text();
//        QMessageBox::critical(this, tr("Model data error"), tr("Model data record error: %1").arg(msg), QMessageBox::Ok);
//        return;
//    }


//    //    // SUBMIT FOR EVERY record or for all rec ????????????
//    //    if(!zv_sampleTaskTableModel->submitAll())
//    //    {
//    //        QString msg = zv_sampleTaskTableModel->lastError().text();
//    //        QMessageBox::critical(this, tr("Database record error"), tr("Database record error: %1").arg(msg), QMessageBox::Ok);
//    //        zv_sampleTaskTableModel->revertAll();
//    //    }
//    //    // ????????????

//    // Get sample task id
//    // get sample task index
//    QModelIndex index = zv_sampleTaskTableModel->index(zv_sampleTaskTableModel->rowCount()-1, 0);
//    if(!index.isValid())
//    {
//        QString msg = "Cannot get model index for inserted sample task id.";
//        QMessageBox::critical(this, tr("Error"), tr("%1").arg(msg), QMessageBox::Ok);
//        zh_removeSampleTaskFromTable();
//        return;
//    }

//    // get sample task id
//    QVariant vData = index.data(Qt::DisplayRole);
//    if(!vData.isValid() || !vData.canConvert<int>())
//    {
//        QString msg = "Cannot get model data for inserted sample task id.";
//        QMessageBox::critical(this, tr("Error"), tr("%1").arg(msg), QMessageBox::Ok);
//        zh_removeSampleTaskFromTable();
//        return;
//    }

//    bool res;
//    int sampleTaskId = vData.toInt(&res);
//    if(!res)
//    {
//        QString msg = "Cannot cast sample task id model data from QVariant to int.";
//        QMessageBox::critical(this, tr("Error"), tr("%1").arg(msg), QMessageBox::Ok);
//        zh_removeSampleTaskFromTable();
//        return;
//    }


//    // conditions has task (table conditions_has_sample_tasks)
//    // create conditions_has_sample_tasks sql model
//    QSqlTableModel conditions_has_sample_tasksModel;
//    conditions_has_sample_tasksModel.setTable("conditions_has_sample_tasks");
//    conditions_has_sample_tasksModel.select();

//    for(int i = 0; i < measuringTaskInitList.count(); i++)
//    {
//        // write measuring conditions
//        record.clear();

//        record.append(QSqlField("id", QVariant::Int));
//        record.append(QSqlField("measuring_conditions_gain_factor", QVariant::Int));
//        record.append(QSqlField("measuring_conditions_exposition", QVariant::Int));
//        record.append(QSqlField("sample_tasks_id", QVariant::Int));

//        record.setValue(1, QVariant(measuringTaskInitList.at(i).gainFactor));
//        record.setValue(2, QVariant(measuringTaskInitList.at(i).exposition));
//        record.setValue(3, QVariant(sampleTaskId));

//        if(!conditions_has_sample_tasksModel.insertRecord(-1, record))
//        {
//            QString msg = conditions_has_sample_tasksModel.lastError().text();
//            QMessageBox::critical(this, tr("Model data error"), tr("Model data record error: %1").arg(msg), QMessageBox::Ok);
//            zh_removeSampleTaskFromTable();
//            return;
//        }
//    }






//    //    int newRow = zv_sampleTaskTableModel->rowCount();
//    //    zv_sampleTaskTableModel->insertRow(newRow);
//    //    QModelIndex index = zv_sampleTaskTableModel->index(newRow, 1);
//    //    if(!index.isValid())
//    //    {
//    //        zv_sampleTaskTableModel->removeRow(newRow);
//    //        return;
//    //    }

//    //    if(!zv_sampleTaskTableModel->setData(index, sampleTaskName))
//    //    {
//    //        zv_sampleTaskTableModel->removeRow(newRow);
//    //        return;
//    //    }

//    //    index = zv_sampleTaskTableModel->index(newRow, 2);
//    //    if(!index.isValid())
//    //    {
//    //        zv_sampleTaskTableModel->removeRow(newRow);
//    //        return;
//    //    }

//    //    if(!zv_sampleTaskTableModel->setData(index, QString("Description %1").arg(QString::number(newRow))))
//    //    {
//    //        zv_sampleTaskTableModel->removeRow(newRow);
//    //        return;
//    //    }

//    //    if(!zv_sampleTaskTableModel->submitAll())
//    //    {
//    //        zv_sampleTaskTableModel->removeRow(newRow);
//    //    }
}
//======================================================
bool ZSampleTaskListDialog::zh_removeSampleTaskFromTable(int row)
{
    if(row >= zv_sampleTaskTableModel->rowCount())
    {
        return false;
    }

    if(row < 0)
    {
        // last row
        row = zv_sampleTaskTableModel->rowCount() - 1;
    }

    if(!zv_sampleTaskTableModel->removeRow(zv_sampleTaskTableModel->rowCount()-1))
    {
        return false;
    }

    //zv_sampleTaskTableModel->submitAll();
    return true;
}
//======================================================
void ZSampleTaskListDialog::zh_checkTaskName(int sampleTaskId, const QString& taskName, bool& res) const
{
    QModelIndex index;
    QVariant vData;
    for(int row = 0; row < zv_sampleTaskTableModel->rowCount(); row++)
    {
        index = zv_sampleTaskTableModel->index(row, 1);
        if(!index.isValid())
        {
            continue;
        }

        vData = index.data(Qt::DisplayRole);
        if(!vData.isValid() || !vData.canConvert<QString>())
        {
            continue;
        }

        if(vData.toString() == taskName)
        {
            // check id
            index = zv_sampleTaskTableModel->index(row, 0);
            if(!index.isValid())
            {
                res = false;
                return;
            }

            vData = index.data(Qt::DisplayRole);
            if(!vData.isValid() || !vData.canConvert<int>())
            {
                res = false;
                return;
            }
            if(vData.toInt() == sampleTaskId)
            {
                continue;
            }

            res = false;
            return;
        }
    }

    res = true;
}
//======================================================
