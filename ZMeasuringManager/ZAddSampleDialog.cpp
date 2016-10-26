//======================================================
#include "ZAddSampleDialog.h"
#include "ZGLConstantsAndDefines.h"
#include "ZSampleTaskDialog.h"
#include "ZReadOnlyStyledItemDelegate.h"
#include "ZMeasuringTaskInitStruct.h"

#include <QVBoxLayout>
#include <QHBoxLayout>

#include <QGroupBox>
#include <QDialogButtonBox>
#include <QEvent>
#include <QFrame>
#include <QHeaderView>
#include <QItemSelectionModel>
#include <QLabel>
#include <QLineEdit>
#include <QMouseEvent>
#include <QMessageBox>
#include <QPushButton>
#include <QSettings>
#include <QSpinBox>
#include <QSqlTableModel>
#include <QSqlRecord>
#include <QSqlError>
#include <QSqlField>
#include <QTableView>
//======================================================
ZAddSampleDialog::ZAddSampleDialog(QWidget *parent) : QDialog(parent)
{
    setWindowTitle(tr("Sample"));
    zv_selectedSampleTaskId = -1;
    zh_createComponents();
    zh_createConnections();
    zh_restoreSettings();
}
//======================================================
void ZAddSampleDialog::zh_createComponents()
{
    // task model
    zv_sampleTaskTableModel  = new QSqlTableModel(this);
    zv_sampleTaskTableModel->setTable("sample_tasks");
    zv_sampleTaskTableModel->setEditStrategy(QSqlTableModel::OnManualSubmit);
    zv_sampleTaskTableModel->select();

    QVBoxLayout* mainLayout = new QVBoxLayout(this);
    setLayout(mainLayout);

    QLabel* label;
    QHBoxLayout* levelLayout;

    // group box
    QGroupBox* selectedGroupBox = new QGroupBox(this);
    selectedGroupBox->setTitle(tr("Selected"));
    mainLayout->addWidget(selectedGroupBox);
    QVBoxLayout* groupBoxLayout = new QVBoxLayout(this);
    selectedGroupBox->setLayout(groupBoxLayout);

    // task name
    label = new QLabel(this);
    label->setText(glCreateCaption(tr("Task name:")));
    groupBoxLayout->addWidget(label);

    levelLayout = new QHBoxLayout(this);
    groupBoxLayout->addLayout(levelLayout);

    zv_taskNameLineEdit = new QLineEdit(this);
    zv_taskNameLineEdit->setReadOnly(true);
    zv_taskNameLineEdit->setText(zv_noSelectedTaskString);
    levelLayout->addWidget(zv_taskNameLineEdit);
    levelLayout->addStretch();

    // Sample name
    label = new QLabel(this);
    label->setText(glCreateCaption(tr("Sample name template:")));
    groupBoxLayout->addWidget(label);

    levelLayout = new QHBoxLayout(this);
    groupBoxLayout->addLayout(levelLayout);

    zv_sampleNameLineEdit = new QLineEdit(this);
    zv_sampleNameLineEdit->setText(zv_noSelectedTaskString);
    levelLayout->addWidget(zv_sampleNameLineEdit);
    levelLayout->addStretch();

    // task list
    label = new QLabel(this);
    label->setText(glCreateCaption(tr("Sample tasks:")));
    mainLayout->addWidget(label);

    zv_sampleTaskTable = new QTableView(this);
    zv_sampleTaskTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    zv_sampleTaskTable->setSelectionMode(QAbstractItemView::SingleSelection);
    mainLayout->addWidget(zv_sampleTaskTable);

    // new and review sample task button
    levelLayout = new QHBoxLayout(this);
    mainLayout->addLayout(levelLayout);
    zv_newSampleTaskButton = new QPushButton(this);
    zv_newSampleTaskButton->setText(tr("New"));
    levelLayout->addStretch();
    levelLayout->addWidget(zv_newSampleTaskButton, 0, Qt::AlignRight | Qt::AlignVCenter);
    zv_editSampleTaskButton = new QPushButton(this);
    zv_editSampleTaskButton->setEnabled(false);
    zv_editSampleTaskButton->setText(tr("Edit"));
    levelLayout->addWidget(zv_editSampleTaskButton, 0, Qt::AlignRight | Qt::AlignVCenter);

    // task quantity
    label = new QLabel(this);
    label->setText(glCreateCaption(tr("Sample quantity:")));
    mainLayout->addWidget(label);

    levelLayout = new QHBoxLayout(this);
    mainLayout->addLayout(levelLayout);
    zv_quantitySpinBox = new QSpinBox(this);
    zv_quantitySpinBox->setRange(1, 999);
    levelLayout->addWidget(zv_quantitySpinBox);
    levelLayout->addStretch();

    // message label
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
    zv_okButton = new QPushButton(NS_Buttons::glButtonOk, this);
    buttonBox->addButton(zv_okButton, QDialogButtonBox::ActionRole);

    zv_cancelButton = new QPushButton(NS_Buttons::glButtonCancel, this);
    buttonBox->addButton(zv_cancelButton, QDialogButtonBox::ActionRole);
}
//======================================================
void ZAddSampleDialog::zh_createConnections()
{
    zv_sampleTaskTable->setModel(zv_sampleTaskTableModel);
    // set read only delegate
    if(zv_sampleTaskTable->itemDelegate())
    {
        delete zv_sampleTaskTable->itemDelegate();
    }

    ZReadOnlyStyledItemDelegate* delegate = new ZReadOnlyStyledItemDelegate(zv_sampleTaskTable);
    zv_sampleTaskTable->setItemDelegate(delegate);
    zv_sampleTaskTable->setColumnHidden(0, true);
    zv_sampleTaskTable->setColumnHidden(2, true);

    zv_sampleTaskTable->horizontalHeader()->setStretchLastSection(true);
    zv_sampleTaskTable->viewport()->installEventFilter(this);

    connect(zv_sampleTaskTable->selectionModel(), &QItemSelectionModel::selectionChanged,
            this, &ZAddSampleDialog::zh_onSelectionChange);
    connect(zv_okButton, &QPushButton::clicked,
            this, &ZAddSampleDialog::zh_onOkButtonClick);
    connect(zv_cancelButton, &QPushButton::clicked,
            this, &ZAddSampleDialog::reject);
    connect(zv_newSampleTaskButton, &QPushButton::clicked,
            this, &ZAddSampleDialog::zh_onNewSampleTaskButtonClick);
    connect(zv_editSampleTaskButton, &QPushButton::clicked,
            this, &ZAddSampleDialog::zh_onEditSampleTaskButtonClick);
    connect(zv_sampleNameLineEdit, &QLineEdit::textChanged,
            this, &ZAddSampleDialog::zh_onSampleNameChange);

}
//======================================================
void ZAddSampleDialog::closeEvent(QCloseEvent* event)
{
    zh_saveSettings();
}
//======================================================
bool ZAddSampleDialog::eventFilter(QObject *object, QEvent *event)
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
//===============================================================
void ZAddSampleDialog::zh_restoreSettings()
{
    QSettings settings;
    QVariant vData;
    settings.beginGroup(glAppVersion);
    settings.beginGroup("AddSampleDialog");

    vData = settings.value("dialogGeometry");
    if(vData.isValid() && !vData.isNull() && vData.canConvert<QByteArray>())
    {
        this->restoreGeometry(vData.value<QByteArray>());
    }

    settings.endGroup();
    settings.endGroup();
}
//===============================================================
void ZAddSampleDialog::zh_saveSettings() const
{
    QSettings settings;
    settings.beginGroup(glAppVersion);
    settings.beginGroup("AddSampleDialog");

    settings.setValue("dialogGeometry", QVariant::fromValue<QByteArray>(this->saveGeometry()));

    settings.endGroup();
    settings.endGroup();
}
//======================================================
QString ZAddSampleDialog::zp_sampleName() const
{
    return zv_sampleNameLineEdit->text().trimmed();
}
//======================================================
void ZAddSampleDialog::zh_checkTaskName(int sampleTaskId, const QString& taskName, bool& res) const
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
int ZAddSampleDialog::zp_sampleQuantity() const
{
    return zv_quantitySpinBox->value();
}
//======================================================
int ZAddSampleDialog::zp_sampleTaskId() const
{
    return zv_selectedSampleTaskId;
}
//======================================================
void ZAddSampleDialog::zh_onOkButtonClick()
{
    // Check data
    if(!zh_checkData())
    {
        return;
    }

    accept();
}
//======================================================
void ZAddSampleDialog::zh_onEditSampleTaskButtonClick() const
{
    zv_messageLabel->clear();
    ZSampleTaskDialog dialog;
    connect(&dialog, &ZSampleTaskDialog::zg_checkTaskName,
            this, &ZAddSampleDialog::zh_checkTaskName);

    // current row
    int row = zv_sampleTaskTable->currentIndex().row();
    if(row < 0 || row >= zv_sampleTaskTableModel->rowCount())
    {
        QString msg = "Sample task is not selected.";
        zv_messageLabel->setText(QString("<font color=red>%1</font>").arg(msg));
        return;
    }

    // get current record
    QSqlRecord record = zv_sampleTaskTableModel->record(row);

    if(record.isEmpty())
    {
        QString msg = "Cannot get current record from sample task table.";
        zv_messageLabel->setText(QString("<font color=red>%1</font>").arg(msg));
        return;
    }

    // id
    bool ok;
    int currentSampleTaskId = record.value(0).toInt(&ok);
    if(!ok)
    {
        QString msg = "Cannot get sample task id from current record.";
        zv_messageLabel->setText(QString("<font color=red>%1</font>").arg(msg));
        return;
    }

    // sample task name
    QString sampleTaskName = record.value(1).toString();
    // sample name template
    QString sampleNameTemplate = record.value(2).toString();
    // description
    QString description = record.value(3).toString();


    // get QList<ZMeasuringTaskInitStruct>& measuringTaskInitStructList
    // V V V
    QList<ZMeasuringTaskInitStruct> measuringTaskInitStructList;

    if(!dialog.zp_loadSampleTask(currentSampleTaskId,
                                 sampleTaskName,
                                 sampleNameTemplate,
                                 description,
                                 measuringTaskInitStructList
                                 ))
    {
        QString msg = "Cannot load current task data to edit dialog.";
        zv_messageLabel->setText(QString("<font color=red>%1</font>").arg(msg));
        return;
    }

    if(!dialog.exec())
    {

    }
}
//======================================================
void ZAddSampleDialog::zh_onNewSampleTaskButtonClick()
{
    zv_messageLabel->clear();
    ZSampleTaskDialog dialog;
    connect(&dialog, &ZSampleTaskDialog::zg_checkTaskName,
            this, &ZAddSampleDialog::zh_checkTaskName);

    if(!dialog.exec())
    {
        return;
    }

    // get data from dialog
    QString sampleTaskName = dialog.zp_sampleTaskName();
    QString sampleNameTemplate = dialog.zp_sampleNameTemplate();
    QString description = dialog.zp_description();
    QList<ZMeasuringTaskInitStruct> measuringTaskInitList = dialog.zp_measuringTaskInitList();

    //    if(sampleTaskName.isEmpty() || sampleNameTemplate.isEmpty())
    //    {
    //        return;
    //    }

    // record new sample task
    QSqlRecord record;
    record.append(QSqlField("id", QVariant::Int));
    record.append(QSqlField("name", QVariant::String));
    record.append(QSqlField("sample_name_template", QVariant::String));
    record.append(QSqlField("description", QVariant::String));

    record.setValue(1, QVariant(sampleTaskName));
    record.setValue(2, QVariant(sampleNameTemplate));
    record.setValue(3, QVariant(description));

    if(!zv_sampleTaskTableModel->insertRecord(-1, record))
    {
        QString msg = zv_sampleTaskTableModel->lastError().text();
        QMessageBox::critical(this, tr("Model data error"), tr("Model data record error: %1").arg(msg), QMessageBox::Ok);
        return;
    }

    if(!zv_sampleTaskTableModel->submitAll())
    {
        QString msg = zv_sampleTaskTableModel->lastError().text();
        QMessageBox::critical(this, tr("Database record error"), tr("Database record error: %1").arg(msg), QMessageBox::Ok);
        zv_sampleTaskTableModel->revertAll();
    }

    // Get sample task id
    // get sample task index
    QModelIndex index = zv_sampleTaskTableModel->index(zv_sampleTaskTableModel->rowCount()-1, 0);
    if(!index.isValid())
    {
        QString msg = "Cannot get model index for inserted sample task id.";
        QMessageBox::critical(this, tr("Error"), tr("%1").arg(msg), QMessageBox::Ok);
        zh_removeSampleTaskFromTable();
        return;
    }

    // get sample task id
    QVariant vData = index.data(Qt::DisplayRole);
    if(!vData.isValid() || !vData.canConvert<int>())
    {
        QString msg = "Cannot get model data for inserted sample task id.";
        QMessageBox::critical(this, tr("Error"), tr("%1").arg(msg), QMessageBox::Ok);
        zh_removeSampleTaskFromTable();
        return;
    }

    bool res;
    int sampleTaskId = vData.toInt(&res);
    if(!res)
    {
        QString msg = "Cannot cast sample task id model data from QVariant to int.";
        QMessageBox::critical(this, tr("Error"), tr("%1").arg(msg), QMessageBox::Ok);
        zh_removeSampleTaskFromTable();
        return;
    }

    // Aux Models
    // conditions has task (table conditions_has_sample_tasks)
    // create conditions_has_sample_tasks sql model
    QSqlTableModel conditionsInTasksModel;
    conditionsInTasksModel.setTable("conditions_has_sample_tasks");
    conditionsInTasksModel.select();

    // calibration stacks has measuring conditions table (table calibration_stacks_has_conditions_has_sample_tasks)
    // create calibration_stacks_has_conditions_has_sample_tasks sql model
    QSqlTableModel calibrationStacksInTasksModel;
    calibrationStacksInTasksModel.setTable("calibration_stacks_has_conditions_has_sample_tasks");
    calibrationStacksInTasksModel.select();

    // Write maesuring conditions
    for(int i = 0; i < measuringTaskInitList.count(); i++)
    {
        // write measuring conditions
        record.clear();

        record.append(QSqlField("id", QVariant::Int));
        record.append(QSqlField("measuring_conditions_gain_factor", QVariant::Int));
        record.append(QSqlField("measuring_conditions_exposition", QVariant::Int));
        record.append(QSqlField("sample_tasks_id", QVariant::Int));

        record.setValue(1, QVariant(measuringTaskInitList.at(i).gainFactor));
        record.setValue(2, QVariant(measuringTaskInitList.at(i).exposition));
        record.setValue(3, QVariant(sampleTaskId));

        if(!conditionsInTasksModel.insertRecord(-1, record))
        {
            QString msg = conditionsInTasksModel.lastError().text();
            QMessageBox::critical(this, tr("Model data error"), tr("Model data record error: %1").arg(msg), QMessageBox::Ok);
            zh_removeSampleTaskFromTable();
            return;
        }

        if(!conditionsInTasksModel.submitAll())
        {
            QString msg = zv_sampleTaskTableModel->lastError().text();
            QMessageBox::critical(this, tr("Database record error"), tr("Database record error: %1").arg(msg), QMessageBox::Ok);
            zv_sampleTaskTableModel->revertAll();
            zh_removeSampleTaskFromTable();
            return;
        }

        // write calibration stacks
        int stacksCount = measuringTaskInitList.at(i).calibrationStackIdList.count();
        for(int s = 0; s < stacksCount; s++)
        {
            record.clear();

            //  V V V


        }


    }

    int row = zv_sampleTaskTableModel->rowCount() - 1;
    QModelIndex insertedIndex = zv_sampleTaskTableModel->index(row, 0);
    zv_sampleTaskTable->setCurrentIndex(insertedIndex);
    zv_sampleTaskTable->scrollTo(insertedIndex);
}
//======================================================
bool ZAddSampleDialog::zh_removeSampleTaskFromTable(int row)
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

    zv_sampleTaskTableModel->submitAll();
    return true;
}
//======================================================
void ZAddSampleDialog::zh_onSelectionChange(const QItemSelection& current,
                                            const QItemSelection& previous)
{
    zv_messageLabel->clear();
    zh_updateSeletedTaskPropertiesStrings();
}
//======================================================
void ZAddSampleDialog::zh_onSampleNameChange(const QString& text)
{
    zv_messageLabel->clear();
}
//======================================================
bool ZAddSampleDialog::zh_checkData()
{
    QString msg;
    if(zv_selectedSampleTaskId < 0)
    {
        if(zv_taskNameLineEdit->text() == zv_noSelectedTaskString)
        {
            msg = tr("Sample task is not selected.");
        }
        else
        {
            msg = tr("The id of the selected sample task is invalid.");
        }

        zv_messageLabel->setText(QString("<font color=red>%1</font>").arg(msg));
        return false;
    }

    if(zv_taskNameLineEdit->text() == zv_noSelectedTaskString)
    {
        msg = tr("Sample task name is invalid.");
        zv_messageLabel->setText(QString("<font color=red>%1</font>").arg(msg));
        return false;
    }

    if(zv_sampleNameLineEdit->text() == zv_noSelectedTaskString
            || zv_sampleNameLineEdit->text().isEmpty())
    {
        msg = tr("Sample name template is invalid.");
        zv_messageLabel->setText(QString("<font color=red>%1</font>").arg(msg));
        return false;
    }

    return true;
}
//======================================================
void ZAddSampleDialog::zh_updateSeletedTaskPropertiesStrings()
{
    // previously set review button disabled
    zv_editSampleTaskButton->setEnabled(false);

    QModelIndex currentIndex = zv_sampleTaskTable->currentIndex();
    if(!currentIndex.isValid() || currentIndex.row() < 0
            || currentIndex.row() >= zv_sampleTaskTableModel->rowCount())
    {
        zv_selectedSampleTaskId = -1;
        zv_taskNameLineEdit->setText(zv_noSelectedTaskString);
        zv_sampleNameLineEdit->setText(zv_noSelectedTaskString);
        return;
    }

    int row = currentIndex.row();

    // id
    currentIndex = zv_sampleTaskTableModel->index(row, 0);
    if(!currentIndex.isValid())
    {
        zv_selectedSampleTaskId = -1;
        zv_taskNameLineEdit->setText(zv_noSelectedTaskString);
        zv_sampleNameLineEdit->setText(zv_noSelectedTaskString);
        return;
    }
    QVariant vData = currentIndex.data(Qt::DisplayRole);
    if(!vData.isValid() || !vData.canConvert<int>())
    {
        zv_selectedSampleTaskId = -1;
        zv_taskNameLineEdit->setText(zv_noSelectedTaskString);
        zv_sampleNameLineEdit->setText(zv_noSelectedTaskString);
        return;
    }
    zv_selectedSampleTaskId = vData.toInt();

    // task name
    currentIndex = zv_sampleTaskTableModel->index(row, 1);
    if(!currentIndex.isValid())
    {
        zv_selectedSampleTaskId = -1;
        zv_taskNameLineEdit->setText(zv_noSelectedTaskString);
        zv_sampleNameLineEdit->setText(zv_noSelectedTaskString);
        return;
    }
    vData = currentIndex.data(Qt::DisplayRole);
    if(!vData.isValid() || !vData.canConvert<QString>())
    {
        zv_selectedSampleTaskId = -1;
        zv_taskNameLineEdit->setText(zv_noSelectedTaskString);
        zv_sampleNameLineEdit->setText(zv_noSelectedTaskString);
        return;
    }
    zv_taskNameLineEdit->setText(vData.toString());

    // sample name template
    currentIndex = zv_sampleTaskTableModel->index(row, 2);
    if(!currentIndex.isValid())
    {
        zv_selectedSampleTaskId = -1;
        zv_taskNameLineEdit->setText(zv_noSelectedTaskString);
        zv_sampleNameLineEdit->setText(zv_noSelectedTaskString);
        return;
    }
    vData = currentIndex.data(Qt::DisplayRole);
    if(!vData.isValid() || !vData.canConvert<QString>())
    {
        zv_selectedSampleTaskId = -1;
        zv_taskNameLineEdit->setText(zv_noSelectedTaskString);
        zv_sampleNameLineEdit->setText(zv_noSelectedTaskString);
        return;
    }

    zv_sampleNameLineEdit->setText(vData.toString());
    zv_editSampleTaskButton->setEnabled(true);

}
//======================================================
