//======================================================
#include "ZAddSampleDialog.h"
#include "ZGeneral.h"
#include "ZSampleTaskDialog2.h"
#include "ZControlAction.h"
#include "ZReadOnlyStyledItemDelegate.h"
#include "ZMeasuringTaskInitStruct.h"
#include "ZSampleTaskTableWidget.h"

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
    setWindowTitle(tr("Samples"));
    setWindowFlags(Qt::Tool);

    zv_selectedSampleTaskId = -1;
    zh_createActions();
    zh_createComponents();
    zh_createConnections();
    zh_restoreSettings();
}
//======================================================
ZAddSampleDialog::~ZAddSampleDialog()
{
    zh_saveSettings();
}
//======================================================
void ZAddSampleDialog::zh_createActions()
{
    zv_newSampleTaskAction = new ZControlAction(this);
    zv_newSampleTaskAction->setText(tr("New"));

    zv_editSampleTaskAction = new ZControlAction(this);
    zv_editSampleTaskAction->setEnabled(false);
    zv_editSampleTaskAction->setText(tr("Edit"));

    zv_reviewSampleTaskAction = new ZControlAction(this);
    zv_reviewSampleTaskAction->setEnabled(false);
    zv_reviewSampleTaskAction->setText(tr("Review"));

}
//======================================================
void ZAddSampleDialog::zh_createComponents()
{
    // task model
    zv_sampleTaskTableModel  = new QSqlTableModel(this);
    zv_sampleTaskTableModel->setTable("sample_tasks");
    zv_sampleTaskTableModel->setEditStrategy(QSqlTableModel::OnManualSubmit);
    zv_sampleTaskTableModel->select();

    QVBoxLayout* mainLayout = new QVBoxLayout;
    setLayout(mainLayout);

    QLabel* label;
    QHBoxLayout* levelLayout;

    zv_sampleTaskTableWidget = new ZSampleTaskTableWidget(this);
    zv_sampleTaskTableWidget->zp_setCaption(glCreateCaption(tr("Sample tasks:")));
    mainLayout->addWidget(zv_sampleTaskTableWidget);

    // Sample name
    label = new QLabel(this);
    label->setText(glCreateCaption(tr("Sample name template:")));
    mainLayout->addWidget(label);

    levelLayout = new QHBoxLayout;
    mainLayout->addLayout(levelLayout);

    zv_sampleNameLineEdit = new QLineEdit(this);
    zv_sampleNameLineEdit->setText(zv_noSelectedTaskString);
    levelLayout->addWidget(zv_sampleNameLineEdit);
    levelLayout->addStretch();

    // task quantity
    label = new QLabel(this);
    label->setText(glCreateCaption(tr("Sample quantity:")));
    mainLayout->addWidget(label);

    levelLayout = new QHBoxLayout;
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
    zv_sampleTaskTableWidget->zp_setModel(zv_sampleTaskTableModel);
    // set read only delegate
    QTableView* sampleTaskTable = zv_sampleTaskTableWidget->zp_tableView();
    if(sampleTaskTable->itemDelegate())
    {
        delete sampleTaskTable->itemDelegate();
    }

    ZReadOnlyStyledItemDelegate* delegate = new ZReadOnlyStyledItemDelegate;
    sampleTaskTable->setItemDelegate(delegate);
    zv_sampleTaskTableWidget->zp_setColumnHidden(0, true);
    zv_sampleTaskTableWidget->zp_setColumnHidden(2, true);

    zv_sampleTaskTableWidget->zp_setStretchLastSection(true);
    sampleTaskTable->viewport()->installEventFilter(this);

    // add button actions
    QList<ZControlAction*> actionList;
    actionList.append(zv_newSampleTaskAction);
    actionList.append(zv_editSampleTaskAction);
    // actionList.append(zv_reviewSampleTaskAction);
    zv_sampleTaskTableWidget->zp_appendButtonActions(actionList);

    connect(sampleTaskTable->selectionModel(), &QItemSelectionModel::selectionChanged,
            this, &ZAddSampleDialog::zh_onSelectionChange);
    connect(zv_okButton, &QPushButton::clicked,
            this, &ZAddSampleDialog::zh_onOkButtonClick);
    connect(zv_cancelButton, &QPushButton::clicked,
            this, &ZAddSampleDialog::reject);
    connect(zv_newSampleTaskAction, &ZControlAction::triggered,
            this, &ZAddSampleDialog::zh_onNewSampleTaskButtonClick);
    connect(zv_editSampleTaskAction, &ZControlAction::triggered,
            this, &ZAddSampleDialog::zh_onEditSampleTaskButtonClick);
    connect(zv_reviewSampleTaskAction, &ZControlAction::triggered,
            this, &ZAddSampleDialog::zh_onReviewSampleTaskButtonClick);
    connect(zv_sampleNameLineEdit, &QLineEdit::textChanged,
            this, &ZAddSampleDialog::zh_onSampleNameChange);

}
//======================================================
bool ZAddSampleDialog::eventFilter(QObject *object, QEvent *event)
{
    if(object == zv_sampleTaskTableWidget->zp_tableView()->viewport() && event->type() == QEvent::MouseButtonDblClick)
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
void ZAddSampleDialog::zh_onEditSampleTaskButtonClick()
{
    zv_messageLabel->clear();

    // get current sample task id
    int row = zv_sampleTaskTableWidget->zp_tableView()->currentIndex().row();
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

    // id from record
    bool ok;
    int currentSampleTaskId = record.value(0).toInt(&ok);
    if(!ok)
    {
        QString msg = "Cannot get sample task id from current record.";
        zv_messageLabel->setText(QString("<font color=red>%1</font>").arg(msg));
        return;
    }

    ZSampleTaskDialog2 dialog(zv_sampleTaskTableModel);
    if(!dialog.zp_loadSampleTask(currentSampleTaskId))
    {
        return;
    }

    if(!dialog.exec())
    {
        return;
    }

    zh_saveNewSampleTaskToDatabase(dialog);
}
//======================================================
void ZAddSampleDialog::zh_saveNewSampleTaskToDatabase(ZSampleTaskDialog2& dialog)
{

}
//======================================================
void ZAddSampleDialog::zh_onReviewSampleTaskButtonClick() const
{

}
//======================================================
void ZAddSampleDialog::zh_onNewSampleTaskButtonClick() const
{
    zv_messageLabel->clear();
    ZSampleTaskDialog2 dialog(zv_sampleTaskTableModel);
    //    connect(&dialog, &ZSampleTaskDialog2::zg_checkTaskName,
    //            this, &ZAddSampleDialog::zh_checkTaskName);

    if(!dialog.exec())
    {
        return;
    }

    int row = zv_sampleTaskTableModel->rowCount() - 1;
    QModelIndex insertedIndex = zv_sampleTaskTableModel->index(row, 0);
    zv_sampleTaskTableWidget->zp_tableView()->setCurrentIndex(insertedIndex);
    zv_sampleTaskTableWidget->zp_tableView()->scrollTo(insertedIndex);
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
    //    if(zv_selectedSampleTaskId < 0)
    //    {
    //        if(zv_taskNameLineEdit->text() == zv_noSelectedTaskString)
    //        {
    //            msg = tr("Sample task is not selected.");
    //        }
    //        else
    //        {
    //            msg = tr("The id of the selected sample task is invalid.");
    //        }

    //        zv_messageLabel->setText(QString("<font color=red>%1</font>").arg(msg));
    //        return false;
    //    }

    //    if(zv_taskNameLineEdit->text() == zv_noSelectedTaskString)
    //    {
    //        msg = tr("Sample task name is invalid.");
    //        zv_messageLabel->setText(QString("<font color=red>%1</font>").arg(msg));
    //        return false;
    //    }

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
    // previously set edit and review button disabled
    zv_editSampleTaskAction->setEnabled(false);
    zv_reviewSampleTaskAction->setEnabled(false);

    QModelIndex currentIndex = zv_sampleTaskTableWidget->zp_tableView()->currentIndex();
    if(!currentIndex.isValid() || currentIndex.row() < 0
            || currentIndex.row() >= zv_sampleTaskTableModel->rowCount())
    {
        zv_selectedSampleTaskId = -1;
        // zv_taskNameLineEdit->setText(zv_noSelectedTaskString);
        zv_sampleNameLineEdit->setText(zv_noSelectedTaskString);
        return;
    }

    int row = currentIndex.row();

    // id
    currentIndex = zv_sampleTaskTableModel->index(row, 0);
    if(!currentIndex.isValid())
    {
        zv_selectedSampleTaskId = -1;
        // zv_taskNameLineEdit->setText(zv_noSelectedTaskString);
        zv_sampleNameLineEdit->setText(zv_noSelectedTaskString);
        return;
    }
    QVariant vData = currentIndex.data(Qt::DisplayRole);
    if(!vData.isValid() || !vData.canConvert<int>())
    {
        zv_selectedSampleTaskId = -1;
        // zv_taskNameLineEdit->setText(zv_noSelectedTaskString);
        zv_sampleNameLineEdit->setText(zv_noSelectedTaskString);
        return;
    }
    zv_selectedSampleTaskId = vData.toInt();

    // task name
    currentIndex = zv_sampleTaskTableModel->index(row, 1);
    if(!currentIndex.isValid())
    {
        zv_selectedSampleTaskId = -1;
        //zv_taskNameLineEdit->setText(zv_noSelectedTaskString);
        zv_sampleNameLineEdit->setText(zv_noSelectedTaskString);
        return;
    }
    vData = currentIndex.data(Qt::DisplayRole);
    if(!vData.isValid() || !vData.canConvert<QString>())
    {
        zv_selectedSampleTaskId = -1;
        //zv_taskNameLineEdit->setText(zv_noSelectedTaskString);
        zv_sampleNameLineEdit->setText(zv_noSelectedTaskString);
        return;
    }
    //zv_taskNameLineEdit->setText(vData.toString());

    // sample name template
    currentIndex = zv_sampleTaskTableModel->index(row, 2);
    if(!currentIndex.isValid())
    {
        zv_selectedSampleTaskId = -1;
        //zv_taskNameLineEdit->setText(zv_noSelectedTaskString);
        zv_sampleNameLineEdit->setText(zv_noSelectedTaskString);
        return;
    }
    vData = currentIndex.data(Qt::DisplayRole);
    if(!vData.isValid() || !vData.canConvert<QString>())
    {
        zv_selectedSampleTaskId = -1;
        //zv_taskNameLineEdit->setText(zv_noSelectedTaskString);
        zv_sampleNameLineEdit->setText(zv_noSelectedTaskString);
        return;
    }

    zv_sampleNameLineEdit->setText(vData.toString());
    zv_editSampleTaskAction->setEnabled(true);
    zv_reviewSampleTaskAction->setEnabled(true);

}
//======================================================
