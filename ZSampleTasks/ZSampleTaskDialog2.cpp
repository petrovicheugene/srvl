//===============================================================
#include "ZSampleTaskDialog2.h"
#include "ZGeneral.h"
#include "ZSampleTaskTreeWidget.h"
#include "ZSampleTaskTreeModel.h"

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QDialogButtonBox>

#include <QEvent>
#include <QFrame>
#include <QLineEdit>
#include <QLabel>
#include <QMessageBox>
#include <QPushButton>
#include <QSpinBox>
#include <QSettings>
#include <QSplitter>
#include <QTableView>
#include <QTreeView>
#include <QTextEdit>
#include <QSqlTableModel>
#include <QSqlRecord>
#include <QSqlQuery>
#include <QSqlField>
#include <QSqlError>
//===============================================================
ZSampleTaskDialog2::ZSampleTaskDialog2(QSqlTableModel *sampleTaskModel, int sampleTaskId,  QWidget *parent) : QDialog(parent)
{
    setWindowTitle(tr("Sample task"));
    setWindowFlags(Qt::Tool);

    if(sampleTaskModel != 0)
    {
        zv_sampleTaskTableModel = sampleTaskModel;
    }
    else
    {
        zv_sampleTaskTableModel = new QSqlTableModel(this);
        zv_sampleTaskTableModel->setTable("sample_tasks");
        zv_sampleTaskTableModel->select();
        zv_sampleTaskTableModel->setEditStrategy(QSqlTableModel::OnManualSubmit);
    }

    zv_sampleTaskTreeModel = 0;
    zv_sampleTaskId = sampleTaskId;
    zh_createComponents();
    zh_createConnections();
    zh_restoreSettings();
}
//===============================================================
ZSampleTaskDialog2::~ZSampleTaskDialog2()
{
    zh_saveSettings();
}
//===============================================================
void ZSampleTaskDialog2::zh_createComponents()
{
    // model
    zv_sampleTaskTreeModel = new ZSampleTaskTreeModel(this);

    // Controls & widgets
    QVBoxLayout* mainLayout = new QVBoxLayout(this);
    setLayout(mainLayout);

    // name lineEdit
    QLabel* label = new QLabel(this);
    label->setText(glCreateCaption(tr("Task name:")));
    mainLayout->addWidget(label);

    QHBoxLayout* levelLayout = new QHBoxLayout(this);
    mainLayout->addLayout(levelLayout);
    zv_sampleTaskNameLineEdit = new QLineEdit(this);
    levelLayout->addWidget(zv_sampleTaskNameLineEdit);
    levelLayout->addStretch();

    // sample name template
    label = new QLabel(this);
    label->setText(glCreateCaption(tr("Sample name template:")));
    mainLayout->addWidget(label);

    levelLayout = new QHBoxLayout(this);
    mainLayout->addLayout(levelLayout);
    zv_sampleNameTemplateLineEdit = new QLineEdit(this);
    levelLayout->addWidget(zv_sampleNameTemplateLineEdit);
    label = new QLabel(this);
    label->setText(glCreateCaption(tr(" + sample serial number")));
    levelLayout->addWidget(label);
    levelLayout->addStretch();

    // tree view widget
    label = new QLabel(this);
    label->setText(glCreateCaption(tr("Task stages:")));
    mainLayout->addWidget(label);

    zv_sampleTaskTreeWidget = new ZSampleTaskTreeWidget(this);
    mainLayout->addWidget(zv_sampleTaskTreeWidget);

    // description
    label = new QLabel(this);
    label->setText(glCreateCaption(tr("Description:")));
    mainLayout->addWidget(label);

    zv_descriptionTextEdit = new QTextEdit(this);
    int descriptionTextEditMaxHeight = zv_sampleTaskNameLineEdit->height() * 2;
    zv_descriptionTextEdit->setMaximumHeight(descriptionTextEditMaxHeight);
    zv_descriptionTextEdit->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    mainLayout->addWidget(zv_descriptionTextEdit);

    // message
    zv_messageLabel = new QLabel(this);
    zv_messageLabel->setWordWrap(true);
    mainLayout->addWidget(zv_messageLabel);

    // Basement
    // mainLayout->addStretch();
    // sparator line
    QFrame* line = new QFrame(this);
    line->setFrameStyle(QFrame::HLine | QFrame::Sunken);
    line->setLineWidth(1);
    mainLayout->addWidget(line);

    // button box
    QDialogButtonBox* buttonBox = new QDialogButtonBox(this);
    mainLayout->addWidget(buttonBox);

    // buttons
    zv_okButton = new QPushButton(this);
    zv_okButton->setText(tr("OK"));
    buttonBox->addButton(zv_okButton, QDialogButtonBox::ActionRole);

    zv_cancelButton = new QPushButton(this);
    zv_cancelButton->setText(tr("Cancel"));
    buttonBox->addButton(zv_cancelButton, QDialogButtonBox::ActionRole);
}
//===============================================================
void ZSampleTaskDialog2::zh_createConnections()
{
    zv_sampleTaskTreeWidget->zp_setModel(zv_sampleTaskTreeModel);
    zv_sampleTaskTreeWidget->zp_setSectionResizeMode(0, QHeaderView::ResizeToContents);
    zv_sampleTaskTreeWidget->zp_setStretchLastSection(false);
    zv_sampleTaskTreeWidget->zp_setSelectionBehavior(QAbstractItemView::SelectRows);
    zv_sampleTaskTreeWidget->zp_setSelectionMode(QAbstractItemView::SingleSelection);

    zv_sampleTaskTreeWidget->zp_appendButtonActions(zv_sampleTaskTreeModel->zp_buttonTreeActions());
    zv_sampleTaskTreeWidget->zp_appendContextActions(zv_sampleTaskTreeModel->zp_contextTreeActions());

    connect(zv_okButton, &QPushButton::clicked,
            this, &ZSampleTaskDialog2::zh_onOkButtonClick);
    connect(zv_cancelButton, &QPushButton::clicked,
            this, &ZSampleTaskDialog2::reject);
    connect(zv_sampleTaskNameLineEdit, &QLineEdit::textChanged,
            this, &ZSampleTaskDialog2::zh_onLineEditDataChange);
    connect(zv_sampleNameTemplateLineEdit, &QLineEdit::textChanged,
            this, &ZSampleTaskDialog2::zh_onLineEditDataChange);

    connect(zv_sampleTaskTreeModel, &QAbstractItemModel::rowsInserted,
            this, &ZSampleTaskDialog2::zh_onModelRowCountChange);
    connect(zv_sampleTaskTreeModel, &QAbstractItemModel::rowsRemoved,
            this, &ZSampleTaskDialog2::zh_onModelRowCountChange);

}
//===============================================================
bool ZSampleTaskDialog2::zp_loadSampleTask(int sampleTaskId)
{
    // get data from table sample_tasks (model zv_sampleTaskTableModel)
    QSqlQuery query;
    QString queryString = QString("SELECT * FROM sample_tasks WHERE id=%1").arg(QString::number(sampleTaskId));

    if(!query.prepare(queryString))
    {
        return false;
    }

    if(!query.exec())
    {
        return false;
    }

    if(!query.next())
    {
        return false;
    }

    // sample task name
    QVariant vData = query.value(1);
    if(!vData.isValid() || !vData.canConvert<QString>() )
    {
        return false;
    }

    zv_sampleTaskNameLineEdit->setText(vData.toString());

    // sample name template
    vData = query.value(2);
    if(!vData.isValid() || !vData.canConvert<QString>() )
    {
        return false;
    }

    zv_sampleNameTemplateLineEdit->setText(vData.toString());

    // description
    vData = query.value(3);
    if(!vData.isValid() || !vData.canConvert<QString>() )
    {
        return false;
    }

    zv_descriptionTextEdit->setText(vData.toString());

    // Task stages tree
    // measuring conditions
    query.clear();
    queryString = QString("SELECT * FROM conditions_has_sample_tasks "
                          "WHERE sample_tasks_id=%1").arg(QString::number(sampleTaskId));

    if(!query.prepare(queryString))
    {
        return false;
    }

    if(!query.exec())
    {
        return false;
    }

    int gainFactor;
    int exposition;
    while(query.next())
    {
        //gain factor
        vData = query.value(1);
        if(!vData.isValid() || !vData.canConvert<int>() )
        {
            return false;
        }

        gainFactor = vData.toInt();

        vData = query.value(2);
        if(!vData.isValid() || !vData.canConvert<int>() )
        {
            return false;
        }

        exposition = vData.toInt();

        zv_sampleTaskTreeModel->zp_appendMeasuringConditions(gainFactor, exposition, 1);
    }

    // chemical tasks
    query.clear();
    queryString = QString("SELECT * FROM calibration_stacks_has_conditions_has_sample_tasks "
                          "WHERE conditions_has_sample_tasks_id "
                          "IN (SELECT id FROM conditions_has_sample_tasks "
                          "WHERE sample_tasks_id=%1)").arg(QString::number(sampleTaskId));

    if(!query.prepare(queryString))
    {
        return false;
    }

    if(!query.exec())
    {
        return false;
    }

    while(query.next())
    {
        vData = query.value(0);
        if(!vData.isValid() || !vData.canConvert<int>() )
        {
            return false;
        }

        zv_sampleTaskTreeModel->zp_appendChemicalTask(vData.toInt());
    }

    zv_sampleTaskTreeWidget->zp_treeView()->expandAll();

    return true;
}
//===============================================================
void ZSampleTaskDialog2::zh_restoreSettings()
{
    QSettings settings;
    QVariant vData;
    settings.beginGroup(glAppVersion);
    settings.beginGroup("SampleTaskDialog");

    vData = settings.value("dialogGeometry");
    if(vData.isValid() && !vData.isNull() && vData.canConvert<QByteArray>())
    {
        this->restoreGeometry(vData.value<QByteArray>());
    }

    settings.endGroup();
    settings.endGroup();
}
//===============================================================
void ZSampleTaskDialog2::zh_saveSettings() const
{
    QSettings settings;
    settings.beginGroup(glAppVersion);
    settings.beginGroup("SampleTaskDialog");

    settings.setValue("dialogGeometry", QVariant::fromValue<QByteArray>(this->saveGeometry()));
    //settings.setValue("splitterGeometry", QVariant::fromValue<QByteArray>(zv_tableSplitter->saveState()));

    settings.endGroup();
    settings.endGroup();
}
//===============================================================
void ZSampleTaskDialog2::zh_onOkButtonClick()
{
    // Check data
    if(!zh_checkData())
    {
        return;
    }

    // write to database
    if(!zh_writeSampleTaskToDatabase())
    {
        return;
    }

    accept();
}
//===============================================================
void ZSampleTaskDialog2::zh_onLineEditDataChange(const QString& text)
{
    zv_messageLabel->clear();
}
//===============================================================
void ZSampleTaskDialog2::zh_onModelRowCountChange(const QModelIndex& parent, int first, int last)
{
    zv_messageLabel->clear();
}
//===============================================================
bool ZSampleTaskDialog2::zh_checkData() const
{
    // check data fields
    QString msg;
    if(zv_sampleTaskNameLineEdit->text().isEmpty())
    {
        msg = tr("Sample task name ");
    }

    if(zv_sampleNameTemplateLineEdit->text().isEmpty())
    {
        if(msg.isEmpty())
        {
            msg = tr("Sample name template");
        }
        else
        {
            msg += tr("and sample name template");
        }

        msg += tr("cannot be empty.");
    }

    if(zv_sampleTaskTreeModel->zp_childCount(QModelIndex()) < 1)
    {
        if(!msg.isEmpty())
        {
            msg += "<br>";
        }
        msg += tr("Sample task must contain at least 1 measuring task.");
    }

    if(!msg.isEmpty())
    {
        zv_messageLabel->setText(QString("<font color=red>%1</font>").arg(msg));
    }

    if(!msg.isEmpty())
    {
        return false;
    }

    // check sample task name
    QVariant vData;
    QSqlRecord record;
    for(int row = 0; row < zv_sampleTaskTableModel->rowCount(); row++)
    {
        record.clear();
        record = zv_sampleTaskTableModel->record(row);
        if(record.isEmpty())
        {
            continue;
        }

        vData = record.value(1);
        if(!vData.isValid() || !vData.canConvert<QString>())
        {
            continue;
        }

        if(vData.toString() == zv_sampleTaskNameLineEdit->text())
        {
            // check id
            vData = record.value(0);
            if(!vData.isValid() || !vData.canConvert<int>())
            {
                QString msg = tr("Cannot get sample task id for row %1.").arg(QString::number(row));
                zv_messageLabel->setText(QString("<font color=red>%1</font>").arg(msg));
                return false;
            }

            if(zv_sampleTaskId == vData.toInt())
            {
                continue;
            }

            QString msg = tr("The sample task name is not unique.");
            zv_messageLabel->setText(QString("<font color=red>%1</font>").arg(msg));
            return false;
        }
    }

    return true;
}
//===============================================================
bool ZSampleTaskDialog2::zh_writeSampleTaskToDatabase()
{
    // write new sample task
    QSqlRecord record;
    record.append(QSqlField("id", QVariant::Int));
    record.append(QSqlField("name", QVariant::String));
    record.append(QSqlField("sample_name_template", QVariant::String));
    record.append(QSqlField("description", QVariant::String));

    // find new sample task id
    int sampleTaskId = 0;
    int currentId;
    bool ok;
    QModelIndex index;
    QVariant vData;
    for(int row = 0; row < zv_sampleTaskTableModel->rowCount(); row++)
    {
        index = zv_sampleTaskTableModel->index(row, 0);
        if(!index.isValid())
        {
            continue;
        }

        vData = index.data(Qt::DisplayRole);
        if(!vData.isValid() || !vData.canConvert<int>())
        {
            continue;
        }

        currentId = vData.toInt(&ok);
        if(!ok)
        {
            continue;
        }

        if(currentId > sampleTaskId)
        {
            sampleTaskId = currentId;
        }
    }

    sampleTaskId++;

    // write new sample task
    record.setValue(0, QVariant(sampleTaskId));
    record.setValue(1, QVariant(zv_sampleTaskNameLineEdit->text()));
    record.setValue(2, QVariant(zv_sampleNameTemplateLineEdit->text()));
    record.setValue(3, QVariant(zv_descriptionTextEdit->toPlainText()));

    if(!zv_sampleTaskTableModel->insertRecord(-1, record))
    {
        QString msg = zv_sampleTaskTableModel->lastError().text();
        QMessageBox::critical(this, tr("Error"), tr("Model data record error: %1").arg(msg), QMessageBox::Ok);
        return false;
    }

    if(!zv_sampleTaskTableModel->submitAll())
    {
        QString msg = zv_sampleTaskTableModel->lastError().text();
        QMessageBox::critical(this, tr("Error"), tr("Database record error: %1").arg(msg), QMessageBox::Ok);
        zv_sampleTaskTableModel->revertAll();
        return false;
    }

    // Aux Model
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

    int conditionsCount = zv_sampleTaskTreeModel->zp_childCount(QModelIndex());
    ZSampleTaskTreeBaseItem* item;
    ZSampleTaskTreeMeasuringConditionsItem* mcItem;
    int conditionsId;
    // Write measuring conditions
    for(int mc = 0; mc < conditionsCount; mc++)
    {
        index = zv_sampleTaskTreeModel->index(mc, 0, QModelIndex());
        item = zv_sampleTaskTreeModel->zp_itemForIndex(index);
        if(item == 0 || item->zp_itemType() != ZSampleTaskTreeItemOptions::IT_MEASURING_CONDITIONS)
        {
            continue;
        }

        mcItem = qobject_cast<ZSampleTaskTreeMeasuringConditionsItem*>(item);
        if(mcItem == 0)
        {
            continue;
        }

        // find new conditions id
        if(!zh_findNewMeasuringConditionsId(conditionsId))
        {
            QString msg = tr("Cannot define new measuring conditions id.");
            QMessageBox::critical(this, tr("Error"), tr("Model data record error: %1").arg(msg), QMessageBox::Ok);
            return false;
        }

        // write measuring conditions
        record.clear();

        record.append(QSqlField("id", QVariant::Int));
        record.append(QSqlField("measuring_conditions_gain_factor", QVariant::Int));
        record.append(QSqlField("measuring_conditions_exposition", QVariant::Int));
        record.append(QSqlField("sample_tasks_id", QVariant::Int));

        record.setValue(0, QVariant(conditionsId));
        record.setValue(1, QVariant(mcItem->zp_gainFactor()));
        record.setValue(2, QVariant(mcItem->zp_exposition()));
        record.setValue(3, QVariant(sampleTaskId));

        if(!conditionsInTasksModel.insertRecord(-1, record))
        {
            QString msg = conditionsInTasksModel.lastError().text();
            QMessageBox::critical(this, tr("Error"), tr("Model data record error: %1").arg(msg), QMessageBox::Ok);
            zh_removeSampleTaskFromTable();
            return false;
        }

        if(!conditionsInTasksModel.submitAll())
        {
            QString msg = zv_sampleTaskTableModel->lastError().text();
            QMessageBox::critical(this, tr("Error"), tr("Database record error: %1").arg(msg), QMessageBox::Ok);
            zv_sampleTaskTableModel->revertAll();
            zh_removeSampleTaskFromTable();
            return false;
        }

        // Write calibration stacks
        // get chemical tasks id
        QModelIndex mcIndex = zv_sampleTaskTreeModel->index(mc, 0, QModelIndex());
        if(!mcIndex.isValid())
        {
            continue;
        }
        int chemicalTaskCount = zv_sampleTaskTreeModel->zp_childCount(mcIndex);

        QModelIndex ctIndex;
        ZSampleTaskTreeBaseItem* item;
        ZSampleTaskTreeChemicalTaskItem* ctItem;
        int chemicalTaskId;
        int chemicalId;

        for(int ct = 0; ct < chemicalTaskCount; ct++)
        {
            ctIndex = zv_sampleTaskTreeModel->index(ct, 0, mcIndex);
            if(!ctIndex.isValid())
            {
                continue;
            }
            item = zv_sampleTaskTreeModel->zp_itemForIndex(ctIndex);
            if(!item)
            {
                continue;
            }

            if(item->zp_itemType() != ZSampleTaskTreeItemOptions::IT_CHEMICAL_TASK)
            {
                continue;
            }

            ctItem = qobject_cast<ZSampleTaskTreeChemicalTaskItem*>(item);

            chemicalTaskId = ctItem->zp_chemicalTaskId();
            // get for chemical task:

            QSqlQuery query;
            QString queryString = QString("SELECT chemicals_id, "
                                          "measuring_conditions_gain_factor, "
                                          "measuring_conditions_exposition "
                                          "FROM calibration_stacks "
                                          "WHERE id=%1").arg(QString::number(chemicalTaskId));
            if(!query.prepare(queryString))
            {
                continue;
            }

            if(!query.exec())
            {
                continue;
            }

            if(!query.next())
            {
                continue;
            }

            // chemical id
            vData = query.value(0);
            if(!vData.isValid() || !vData.canConvert<int>() )
            {
                continue;
            }

            chemicalId = vData.toInt();

            record.clear();
            record.append(QSqlField("calibration_stacks_id", QVariant::Int));
            record.append(QSqlField("calibration_stacks_chemicals_id", QVariant::Int));
            record.append(QSqlField("calibration_stacks_measuring_conditions_gain_factor", QVariant::Int));
            record.append(QSqlField("calibration_stacks_measuring_conditions_exposition", QVariant::Int));

            record.append(QSqlField("conditions_has_sample_tasks_id", QVariant::Int));
            record.append(QSqlField("conditions_has_sample_tasks_measuring_conditions_gain_factor", QVariant::Int));
            record.append(QSqlField("conditions_has_sample_tasks_measuring_conditions_exposition", QVariant::Int));
            record.append(QSqlField("conditions_has_sample_tasks_sample_tasks_id", QVariant::Int));

            record.setValue(0, QVariant(chemicalTaskId));
            record.setValue(1, QVariant(chemicalId));
            record.setValue(2, QVariant(mcItem->zp_gainFactor()));
            record.setValue(3, QVariant(mcItem->zp_exposition()));

            record.setValue(4, QVariant(conditionsId));
            record.setValue(5, QVariant(mcItem->zp_gainFactor()));
            record.setValue(6, QVariant(mcItem->zp_exposition()));
            record.setValue(7, QVariant(sampleTaskId));

            if(!calibrationStacksInTasksModel.insertRecord(-1, record))
            {
                //                QString msg = conditionsInTasksModel.lastError().text();
                //                QMessageBox::critical(this, tr("Error"), tr("Model data record error: %1").arg(msg), QMessageBox::Ok);
                //                zh_removeSampleTaskFromTable();
                continue;;
            }

            if(!calibrationStacksInTasksModel.submitAll())
            {
                //                QString msg = zv_sampleTaskTableModel->lastError().text();
                //                QMessageBox::critical(this, tr("Error"), tr("Database record error: %1").arg(msg), QMessageBox::Ok);
                //                zv_sampleTaskTableModel->revertAll();
                //                zh_removeSampleTaskFromTable();
                continue;;
            }
        }
    }

    return true;
}
//===============================================================
bool ZSampleTaskDialog2::zh_removeSampleTaskFromTable(int row)
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
//===============================================================
bool ZSampleTaskDialog2::zh_findNewMeasuringConditionsId(int& newId)
{
    QSqlQuery query;
    QString queryString = "SELECT MAX(id) FROM conditions_has_sample_tasks";
    if(!query.prepare(queryString))
    {
        return false;
    }
    if(!query.exec())
    {
        return false;
    }

    if(!query.next())
    {
        newId = 1;
        return true;
    }

    QVariant vData = query.value(0);
    if(!vData.isValid() || !vData.canConvert<int>())
    {
        return false;
    }

    newId = vData.toInt() + 1;

    return true;
}
//===============================================================
