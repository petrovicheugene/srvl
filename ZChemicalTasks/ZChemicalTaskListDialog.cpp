//===============================================================
#include "ZChemicalTaskListDialog.h"
#include "ZGeneral.h"
#include "ZChemicalTableWidget.h"
#include "ZChemicalTaskTableWidget.h"
#include "ZChemicalTaskDialog.h"
#include "ZChemicalDialog.h"
#include "ZControlAction.h"
#include "ZReadOnlyStyledItemDelegate.h"

#include <QApplication>
#include <QHBoxLayout>
#include <QDialogButtonBox>
#include <QHeaderView>
#include <QLabel>
#include <QMessageBox>
#include <QPushButton>
#include <QTableView>
#include <QSettings>
#include <QSplitter>
#include <QSqlError>
#include <QSqlTableModel>
#include <QSqlRecord>
#include <QSqlField>
#include <QSqlQuery>
#include <QVBoxLayout>
//===============================================================
ZChemicalTaskListDialog::ZChemicalTaskListDialog(QWidget *parent) : QDialog(parent)
{
    setWindowTitle(tr("Chemical tasks"));
    setWindowFlags(Qt::Tool);

    zh_createActions();
    zh_createComponents();
    zh_createConnections();
    zh_restoreSettings();

    // set current chemical row 0
    QModelIndex index = zv_chemicalTableModel->index(0, 1);
    if(index.isValid())
    {
        zv_chemicalTableWidget->zp_tableView()->setCurrentIndex(index);
    }

}
//===============================================================
ZChemicalTaskListDialog::~ZChemicalTaskListDialog()
{
    zh_saveSettings();
}
//===============================================================
void ZChemicalTaskListDialog::zh_createActions()
{
    zv_newChemicalAction = new ZControlAction(this);
    zv_newChemicalAction->setText(tr("New"));
    zv_newChemicalAction->setIcon(QIcon(":/images/ZImages/new-8"));
    zv_newChemicalAction->setToolTip(tr("Create new chemical"));

    zv_copyChemicalAction = new ZControlAction(this);
    zv_copyChemicalAction->setEnabled(false);
    zv_copyChemicalAction->setText(tr("Copy"));
    zv_copyChemicalAction->setIcon(QIcon(":/images/ZImages/copy-8"));
    zv_copyChemicalAction->setToolTip(tr("Copy current chemical"));

    zv_newChemicalTaskAction = new ZControlAction(this);
    zv_newChemicalTaskAction->setText(tr("New"));
    zv_newChemicalTaskAction->setIcon(QIcon(":/images/ZImages/new-8"));
    zv_newChemicalTaskAction->setToolTip(tr("Create new chemical task"));

    zv_copyChemicalTaskAction = new ZControlAction(this);
    zv_copyChemicalTaskAction->setEnabled(false);
    zv_copyChemicalTaskAction->setText(tr("Copy"));
    zv_copyChemicalTaskAction->setIcon(QIcon(":/images/ZImages/copy-8"));
    zv_copyChemicalTaskAction->setToolTip(tr("Copy current chemical task"));


    zv_reviewChemicalTaskAction = new ZControlAction(this);
    zv_reviewChemicalTaskAction->setEnabled(false);
    zv_reviewChemicalTaskAction->setText(tr("Review"));

}
//===============================================================
void ZChemicalTaskListDialog::zh_createComponents()
{
    // models
    zv_chemicalTableModel = new QSqlTableModel(this);
    zv_chemicalTableModel->setTable("chemicals");
    zv_chemicalTableModel->select();
    zv_chemicalTableModel->setEditStrategy(QSqlTableModel::OnManualSubmit);

    zv_chemicalTableModel->setHeaderData(0, Qt::Horizontal, QVariant(tr("Id")));
    zv_chemicalTableModel->setHeaderData(1, Qt::Horizontal, QVariant(tr("Name")));

    zv_calibrationStackTableModel = new QSqlTableModel(this);
    zv_calibrationStackTableModel->setTable("calibration_stacks");
    zv_calibrationStackTableModel->select();
    zv_calibrationStackTableModel->setEditStrategy(QSqlTableModel::OnManualSubmit);

    zv_calibrationStackTableModel->setHeaderData(0, Qt::Horizontal, QVariant(tr("Id")));
    zv_calibrationStackTableModel->setHeaderData(1, Qt::Horizontal, QVariant(tr("Name")));
    zv_calibrationStackTableModel->setHeaderData(2, Qt::Horizontal, QVariant(tr("Description")));


    // controls
    QVBoxLayout* mainLayout = new QVBoxLayout;
    setLayout(mainLayout);

    zv_mainSplitter = new QSplitter(this);
    zv_mainSplitter->setChildrenCollapsible(false);
    mainLayout->addWidget(zv_mainSplitter, 999999);

    zv_chemicalTableWidget = new ZChemicalTableWidget(this);
    zv_chemicalTableWidget->zp_setCaption(glCreateCaption(tr("Chemicals:")));
    zv_mainSplitter->addWidget(zv_chemicalTableWidget);

    zv_chemicalTaskTableWidget = new ZChemicalTaskTableWidget(this);
    zv_chemicalTaskTableWidget->zp_setCaption(glCreateCaption(tr("Chemical tasks:")));
    zv_mainSplitter->addWidget(zv_chemicalTaskTableWidget);

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
void ZChemicalTaskListDialog::zh_createConnections()
{
    // chemical table
    zv_chemicalTableWidget->zp_setModel(zv_chemicalTableModel);
    zv_chemicalTableWidget->zp_setColumnHidden(0, true);
    zv_chemicalTableWidget->zp_setVerticalHeaderHidden(true);
    zv_chemicalTableWidget->zp_setStretchLastSection(true);
    if(zv_chemicalTableWidget->zp_tableView()->itemDelegate())
    {
        delete zv_chemicalTableWidget->zp_tableView()->itemDelegate();
    }
    ZReadOnlyStyledItemDelegate* delegate = new ZReadOnlyStyledItemDelegate;
    zv_chemicalTableWidget->zp_tableView()->setItemDelegate(delegate);
    zv_chemicalTableWidget->zp_tableView()->setSelectionMode(QAbstractItemView::SingleSelection);
    zv_chemicalTableWidget->zp_tableView()->setSelectionBehavior(QAbstractItemView::SelectRows);
    zv_chemicalTableWidget->zp_tableView()->horizontalHeader()->setSectionResizeMode(1, QHeaderView::Stretch);

    // chemical task table
    zv_chemicalTaskTableWidget->zp_setModel(zv_calibrationStackTableModel);
    zv_chemicalTaskTableWidget->zp_setColumnHidden(0, true);
    zv_chemicalTaskTableWidget->zp_setColumnHidden(3, true);
    zv_chemicalTaskTableWidget->zp_setColumnHidden(4, true);
    zv_chemicalTaskTableWidget->zp_tableView()->horizontalHeader()->setSectionResizeMode(4, QHeaderView::Stretch);

    // add actions to widgets
    QList<ZControlAction*> actionList;
    actionList.append(zv_newChemicalAction);
    actionList.append(zv_copyChemicalAction);
    zv_chemicalTableWidget->zp_appendButtonActions(actionList);

    actionList.clear();
    actionList.append(zv_newChemicalTaskAction);
    actionList.append(zv_copyChemicalTaskAction);
    //actionList.append(zv_reviewChemicalTaskAction);
    zv_chemicalTaskTableWidget->zp_appendButtonActions(actionList);

    connect(zv_chemicalTableWidget, &ZChemicalTableWidget::zg_currentChanged,
            this, &ZChemicalTaskListDialog::zh_onChemicalChange);
    connect(zv_chemicalTaskTableWidget, &ZChemicalTaskTableWidget::zg_currentChanged,
            this, &ZChemicalTaskListDialog::zh_onChemicalTaskChange);

    connect(zv_okButton, &QPushButton::clicked,
            this, &ZChemicalTaskListDialog::zh_onOkButtonClick);
    connect(zv_cancelButton, &QPushButton::clicked,
            this, &ZChemicalTaskListDialog::reject);

    connect(zv_newChemicalAction, &ZControlAction::triggered,
            this, &ZChemicalTaskListDialog::zh_onNewChemicalAction);
    connect(zv_copyChemicalAction, &ZControlAction::triggered,
            this, &ZChemicalTaskListDialog::zh_onEditChemicalAction);

    connect(zv_newChemicalTaskAction, &ZControlAction::triggered,
            this, &ZChemicalTaskListDialog::zh_onNewChemicalTaskAction);
    connect(zv_copyChemicalTaskAction, &ZControlAction::triggered,
            this, &ZChemicalTaskListDialog::zh_onEditChemicalTaskAction);
    connect(zv_reviewChemicalTaskAction, &ZControlAction::triggered,
            this, &ZChemicalTaskListDialog::zh_onReviewChemicalTaskAction);

}
//===============================================================
void ZChemicalTaskListDialog::zh_restoreSettings()
{
    QSettings settings;
    QVariant vData;
    settings.beginGroup(qApp->applicationVersion());
    settings.beginGroup("ChemicalTaskListDialog");

    vData = settings.value("dialogGeometry");
    if(vData.isValid() && !vData.isNull() && vData.canConvert<QByteArray>())
    {
        this->restoreGeometry(vData.value<QByteArray>());
    }

    vData = settings.value("mainSplitterState");
    if(vData.isValid() && !vData.isNull() && vData.canConvert<QByteArray>())
    {
        zv_mainSplitter->restoreState(vData.value<QByteArray>());
    }

    settings.endGroup();
    settings.endGroup();
}
//===============================================================
void ZChemicalTaskListDialog::zh_saveSettings() const
{
    QSettings settings;
    settings.beginGroup(qApp->applicationVersion());
    settings.beginGroup("ChemicalTaskListDialog");

    settings.setValue("dialogGeometry", QVariant::fromValue<QByteArray>(this->saveGeometry()));
    //settings.setValue("splitterGeometry", QVariant::fromValue<QByteArray>(zv_tableSplitter->saveState()));
    settings.setValue("mainSplitterState", QVariant::fromValue<QByteArray>(zv_mainSplitter->saveState()));

    settings.endGroup();
    settings.endGroup();
}
//===============================================================
int ZChemicalTaskListDialog::zp_currentChemicalTaskId() const
{
    // current index
    QModelIndex index = zv_chemicalTaskTableWidget->zp_tableView()->currentIndex();
    if(!index.isValid())
    {
        return -1;
    }

    // index chemical task id
    index = zv_calibrationStackTableModel->index(index.row(), 0);
    if(!index.isValid())
    {
        return -1;
    }

    QVariant vData = index.data(Qt::DisplayRole);
    if(!vData.isValid() || !vData.canConvert<int>())
    {
        return -1;
    }

    return vData.toInt();
}
//===============================================================
void ZChemicalTaskListDialog::zh_onChemicalChange(const QModelIndex& current, const QModelIndex& previous)
{
    // reset edit review task actions
    zv_copyChemicalTaskAction->setEnabled(false);
    zv_reviewChemicalTaskAction->setEnabled(false);

    // get current chemical Id
    int currentChemicalId = 0;
    if(current.isValid())
    {
        int currentRow = current.row();
        QModelIndex idIndex = zv_chemicalTableModel->index(currentRow, 0);
        if(idIndex.isValid())
        {
            QVariant vData = idIndex.data(Qt::DisplayRole);
            if(vData.isValid() && vData.canConvert<int>())
            {
                currentChemicalId = vData.toInt();
            }
        }
    }

    QString filterString = QString("chemicals_id=%1").arg(QString::number(currentChemicalId));
    zv_calibrationStackTableModel->setFilter(filterString);
    zv_copyChemicalAction->setEnabled(currentChemicalId > 0);

    zv_chemicalTaskTableWidget->zp_tableView()->horizontalHeader()->setStretchLastSection(true);
}
//===============================================================
void ZChemicalTaskListDialog::zh_onChemicalTaskChange(const QModelIndex& current, const QModelIndex& previous)
{
    bool actionsEnable = current.isValid();

    zv_copyChemicalTaskAction->setEnabled(actionsEnable);
    zv_reviewChemicalTaskAction->setEnabled(actionsEnable);

}
//===============================================================
bool ZChemicalTaskListDialog::zh_checkData()
{
    QModelIndex index = zv_chemicalTaskTableWidget->zp_tableView()->selectionModel()->currentIndex();
    if(!index.isValid() || index.row() < 0 || index.row() >= zv_calibrationStackTableModel->rowCount())
    {
        QString msg = tr("No chemical task selected.");
        QMessageBox::critical(this, tr("Error"), msg, QMessageBox::Ok);
        return false;
    }

    return true;
}
//===============================================================
void ZChemicalTaskListDialog::zh_onOkButtonClick()
{
    if(!zh_checkData())
    {
        return;
    }

    accept();
}
//===============================================================
void ZChemicalTaskListDialog::zh_onNewChemicalAction()
{
    ZChemicalDialog dialog;
    connect(&dialog, &ZChemicalDialog::zg_checkChemical,
            this, &ZChemicalTaskListDialog::zh_checkChemical);

    if(!dialog.exec())
    {
        return;
    }

    QString newChemical = dialog.zp_chemical();
    if(newChemical.isEmpty())
    {
        return;
    }

    // find new chemical id
    int newChemicalId = 0;
    int currentChemicalId;
    QModelIndex index;
    QVariant vData;
    bool ok;
    for(int row = 0; row < zv_chemicalTableModel->rowCount(); row++)
    {
        index = zv_chemicalTableModel->index(row, 0);
        if(!index.isValid())
        {
            continue;
        }
        vData = index.data(Qt::DisplayRole);
        if(!vData.isValid() || !vData.canConvert<int>())
        {
            continue;
        }

        currentChemicalId = vData.toInt(&ok);
        if(!ok)
        {
            continue;
        }
        if(newChemicalId < currentChemicalId)
        {
            newChemicalId = currentChemicalId;
        }
    }
    newChemicalId++;
    // write to model
    QSqlRecord record;
    record.append(QSqlField("id", QVariant::Int));
    record.append(QSqlField("name", QVariant::String));

    record.setValue(0, QVariant(newChemicalId));
    record.setValue(1, QVariant(newChemical));

    if(!zv_chemicalTableModel->insertRecord(-1, record))
    {
        QString msg = zv_chemicalTableModel->lastError().text();
        QMessageBox::critical(this, tr("Error"), tr("Model data record error: %1").arg(msg), QMessageBox::Ok);
        return;
    }

    if(!zv_chemicalTableModel->submitAll())
    {
        QString msg = zv_chemicalTableModel->lastError().text();
        QMessageBox::critical(this, tr("Error"), tr("Database record error: %1").arg(msg), QMessageBox::Ok);
        zv_chemicalTableModel->revertAll();
        return;
    }

    int row = zv_chemicalTableModel->rowCount() - 1;
    QModelIndex insertedIndex = zv_chemicalTableModel->index(row, 1);
    zv_chemicalTableWidget->zp_tableView()->setCurrentIndex(insertedIndex);
    zv_chemicalTableWidget->zp_tableView()->scrollTo(insertedIndex);
}
//===============================================================
void ZChemicalTaskListDialog::zh_onEditChemicalAction()
{
    // get current chemical data
    QModelIndex index = zv_chemicalTableWidget->zp_tableView()->currentIndex();
    if(!index.isValid())
    {
        QString msg = tr("Cannot get model current index.");
        QMessageBox::critical(this, tr("Error"), tr("Model error: %1").arg(msg), QMessageBox::Ok);
        return;
    }

    int row = index.row();
    // id
    index = zv_chemicalTableModel->index(row, 0);
    if(!index.isValid())
    {
        QString msg = tr("Cannot get model index for id of current chemical.");
        QMessageBox::critical(this, tr("Error"), tr("Model error: %1").arg(msg), QMessageBox::Ok);
        return;
    }

    QVariant vData = index.data(Qt::DisplayRole);
    if(!vData.isValid() || !vData.canConvert<int>())
    {
        QString msg = tr("Cannot get model data for id of current chemical.");
        QMessageBox::critical(this, tr("Error"), tr("Model error: %1").arg(msg), QMessageBox::Ok);
        return;
    }

    bool ok;
    int chemicalId = vData.toInt(&ok);
    if(!ok)
    {
        QString msg = tr("Cannot cast id value from QVariant.");
        QMessageBox::critical(this, tr("Error"), tr("Model error: %1").arg(msg), QMessageBox::Ok);
        return;
    }

    // chemical string
    index = zv_chemicalTableModel->index(row, 1);
    if(!index.isValid())
    {
        QString msg = tr("Cannot get model index for current chemical name.");
        QMessageBox::critical(this, tr("Error"), tr("Model error: %1").arg(msg), QMessageBox::Ok);
        return;
    }

    vData = index.data(Qt::DisplayRole);
    if(!vData.isValid() || !vData.canConvert<QString>())
    {
        QString msg = tr("Cannot get model data for current chemical name.");
        QMessageBox::critical(this, tr("Error"), tr("Model error: %1").arg(msg), QMessageBox::Ok);
        return;
    }

    QString chemical = vData.toString();

    ZChemicalDialog dialog;
    connect(&dialog, &ZChemicalDialog::zg_checkChemical,
            this, &ZChemicalTaskListDialog::zh_checkChemical);

    dialog.zp_setChemical(chemicalId, chemical);

    if(!dialog.exec())
    {
        return;
    }

    // get data from dialog
    chemical = dialog.zp_chemical();

    // write chemical
    index = zv_chemicalTableModel->index(row, 1);
    if(!index.isValid())
    {
        QString msg = tr("Cannot get model index for current chemical name.");
        QMessageBox::critical(this, tr("Error"), tr("Model error: %1").arg(msg), QMessageBox::Ok);
        return;
    }

    if(!zv_chemicalTableModel->setData(index, QVariant(chemical)))
    {
        QString msg = zv_chemicalTableModel->lastError().text();
        QMessageBox::critical(this, tr("Error"), tr("Model error: %1").arg(msg), QMessageBox::Ok);
        return;
    }

    if(!zv_chemicalTableModel->submitAll())
    {
        QString msg = zv_chemicalTableModel->lastError().text();
        QMessageBox::critical(this, tr("Error"), tr("Database record error: %1").arg(msg), QMessageBox::Ok);
        zv_chemicalTableModel->revertAll();
        return;
    }
}
//===============================================================
void ZChemicalTaskListDialog::zh_onNewChemicalTaskAction()
{
    // current chemical id

    int chemicalId = -1;
    QModelIndex index = zv_chemicalTableWidget->zp_tableView()->currentIndex();
    if(index.isValid())
    {
        int row = index.row();
        index = zv_chemicalTableModel->index(row, 0);
        if(index.isValid())
        {
            QVariant vData = index.data(Qt::DisplayRole);
            if(vData.isValid() || !vData.canConvert<int>())
            {
                bool ok;
                chemicalId = vData.toInt(&ok);
                if(!ok)
                {
                    chemicalId = -1;
                }
            }
        }
    }

    if(chemicalId < 0)
    {
        QString msg = tr("Select a chemical element for which the task is created.");
        QMessageBox::critical(this, tr("Error"), tr("Error: %1").arg(msg), QMessageBox::Ok);
        return;
    }

    // dialog
    ZChemicalTaskDialog dialog(zv_chemicalTableModel);
    connect(&dialog, &ZChemicalTaskDialog::zg_checkChemicalTaskName,
            this, &ZChemicalTaskListDialog::zh_checkChemicalTaskName);

    //dialog.zp_setChemicalId(chemicalId);
    if(!dialog.zp_setChemicalId(chemicalId))
    {
        QString msg = tr("Cannot transfer the current chemical element id to the dialog.");
        QMessageBox::critical(this, tr("Error"), tr("Error: %1").arg(msg), QMessageBox::Ok);
        return;
    }

    if(!dialog.exec())
    {
        return;
    }

    // Write chemical task to database
    zh_saveNewChemicalTaskToDatabase(dialog);
}
//===============================================================
void ZChemicalTaskListDialog::zh_onEditChemicalTaskAction()
{
    // dialog
    ZChemicalTaskDialog dialog(zv_chemicalTableModel);
    connect(&dialog, &ZChemicalTaskDialog::zg_checkChemicalTaskName,
            this, &ZChemicalTaskListDialog::zh_checkChemicalTaskName);

    // load chemical task to dialog
    if(!zh_loadChemicalTaskDialog(dialog, false))
    {
        return;
    }
    //    if(!dialog.zp_setChemicalId(chemicalId))
    //    {
    //        QString msg = tr("Cannot transfer chemical id to dialog.");
    //        QMessageBox::critical(this, tr("Error"), tr("Error: %1").arg(msg), QMessageBox::Ok);
    //        return;
    //    }

    if(!dialog.exec())
    {
        return;
    }

    zh_saveNewChemicalTaskToDatabase(dialog);
}
//===============================================================
void ZChemicalTaskListDialog::zh_onReviewChemicalTaskAction() const
{
    // dialog
    ZChemicalTaskDialog dialog(zv_chemicalTableModel);
    connect(&dialog, &ZChemicalTaskDialog::zg_checkChemicalTaskName,
            this, &ZChemicalTaskListDialog::zh_checkChemicalTaskName);

    // load chemical task to dialog
    if(!zh_loadChemicalTaskDialog(dialog, true))
    {
        return;
    }

    //    if(!dialog.zp_setChemicalId(chemicalId))
    //    {
    //        QString msg = tr("Cannot transfer chemical id to dialog.");
    //        QMessageBox::critical(this, tr("Error"), tr("Error: %1").arg(msg), QMessageBox::Ok);
    //        return;
    //    }

    // set dialog read only

    if(!dialog.exec())
    {
        return;
    }

}
//===============================================================
bool ZChemicalTaskListDialog::zh_loadChemicalTaskDialog(ZChemicalTaskDialog& dialog, bool viewOnly) const
{
    // get current chemical task id
    QModelIndex index = zv_chemicalTaskTableWidget->zp_tableView()->currentIndex();
    if(!index.isValid())
    {
        return false;
    }

    int row = index.row();
    index = zv_calibrationStackTableModel->index(row, 0);
    if(!index.isValid())
    {
        return false;
    }

    QVariant vData = index.data(Qt::DisplayRole);
    if(!vData.isValid() || !vData.canConvert<int>())
    {
        return false;
    }

    int chemicalTaskId = vData.toInt();

    return dialog.zp_setChemicalTask(zv_calibrationStackTableModel, chemicalTaskId, viewOnly);
}
//===============================================================
void ZChemicalTaskListDialog::zh_saveNewChemicalTaskToDatabase(ZChemicalTaskDialog& dialog)
{
    // new id
    int id = zh_findNewChemicalTaskId();
    if(id < 0)
    {
        QString msg = tr("Cannot define id of new chemical task.");
        QMessageBox::critical(this, tr("Error"), msg, QMessageBox::Ok);
        return;
    }

    // get calibration stack data
    QList<int> calibrationIdList;
    dialog.zp_calibrationIdList(calibrationIdList);
    if(calibrationIdList.isEmpty())
    {
        QString msg = tr("No calibrations to append to chemical task.");
        QMessageBox::critical(this, tr("Error"), msg, QMessageBox::Ok);
        return;
    }

    // get calibration limits
    QList<QPair<double, double> > concentrationLimitsList;
    dialog.zp_concentrationLimitsList(concentrationLimitsList);

    if(concentrationLimitsList.count() != calibrationIdList.count())
    {
        QString msg = tr("Cannot get concentration limits for calibrations.");
        QMessageBox::critical(this, tr("Error"), msg, QMessageBox::Ok);
        return;
    }

    // write to database
    QSqlRecord record;
    record.append(QSqlField("id", QVariant::Int));
    record.append(QSqlField("name", QVariant::String));
    record.append(QSqlField("description", QVariant::String));
    record.append(QSqlField("chemicals_id", QVariant::Int));
    record.append(QSqlField("measuring_conditions_id", QVariant::Int));

    record.setValue(0, QVariant(id));
    record.setValue(1, QVariant(dialog.zp_chemicalTaskName()));
    record.setValue(2, QVariant(dialog.zp_description()));
    record.setValue(3, QVariant(dialog.zp_chemicalId()));
    record.setValue(4, QVariant(dialog.zp_measurementConditionsId()));

    if(!zv_calibrationStackTableModel->insertRecord(-1, record))
    {
        QString msg = zv_calibrationStackTableModel->lastError().text();
        QMessageBox::critical(this, tr("Error"), tr("Model data record error: %1").arg(msg), QMessageBox::Ok);
        zv_calibrationStackTableModel->revertAll();
        zv_chemicalTaskTableWidget->zp_tableView()->reset();
        return;
    }

    if(!zv_calibrationStackTableModel->submitAll())
    {
        QString msg = zv_calibrationStackTableModel->lastError().text();
        QMessageBox::critical(this, tr("Error"), tr("Database record error: %1").arg(msg), QMessageBox::Ok);
        zv_calibrationStackTableModel->revertAll();
        zv_chemicalTaskTableWidget->zp_tableView()->reset();
        return;
    }

    // calibrations_has_calibration_stacks table
    // create SQL model
    QSqlTableModel stackedCalibrationModel;
    stackedCalibrationModel.setTable("calibrations_has_calibration_stacks");
    stackedCalibrationModel.select();
    stackedCalibrationModel.setEditStrategy(QSqlTableModel::OnManualSubmit);

    // write to database
    record.clear();
    record.append(QSqlField("calibrations_id", QVariant::Int));
    record.append(QSqlField("calibrations_chemicals_id", QVariant::Int));
    record.append(QSqlField("calibrations_measuring_conditions_id", QVariant::Int));
    record.append(QSqlField("calibration_stacks_id", QVariant::Int));
    record.append(QSqlField("calibration_stacks_chemicals_id", QVariant::Int));
    record.append(QSqlField("calibration_stacks_measuring_conditions_id", QVariant::Int));
    record.append(QSqlField("calibration_min_limit", QVariant::Double));
    record.append(QSqlField("calibration_max_limit", QVariant::Double));

    for(int row = 0; row < calibrationIdList.count(); row++)
    {

        record.clearValues();
        record.setValue(0, QVariant(calibrationIdList.at(row)));  // calibration id
        record.setValue(1, QVariant(dialog.zp_chemicalId())); // calibration chemical id
        record.setValue(2, QVariant(dialog.zp_measurementConditionsId()));

        record.setValue(3, QVariant(id)); // stack id
        record.setValue(4, QVariant(dialog.zp_chemicalId()));
        record.setValue(5, QVariant(dialog.zp_measurementConditionsId()));
        record.setValue(6, QVariant(concentrationLimitsList.at(row).first)); // min
        record.setValue(7, QVariant(concentrationLimitsList.at(row).second)); // max

        if(!stackedCalibrationModel.insertRecord(-1, record))
        {
            QString msg = stackedCalibrationModel.lastError().text();
            QMessageBox::critical(this, tr("Error"), tr("Model data record error: %1").arg(msg), QMessageBox::Ok);
            stackedCalibrationModel.revertAll();
            zv_calibrationStackTableModel->removeRows(zv_calibrationStackTableModel->rowCount() - 1, 1);
            zv_calibrationStackTableModel->submitAll();
            return;
        }

        if(!stackedCalibrationModel.submitAll())
        {
            QString msg = stackedCalibrationModel.lastError().text();
            QMessageBox::critical(this, tr("Error"), tr("Database record error: %1").arg(msg), QMessageBox::Ok);
            stackedCalibrationModel.revertAll();
            zv_calibrationStackTableModel->removeRows(zv_calibrationStackTableModel->rowCount() - 1, 1);
            zv_calibrationStackTableModel->submitAll();
            return;
        }
    }
}
//===============================================================
int ZChemicalTaskListDialog::zh_findNewChemicalTaskId() const
{
    QString queryString = QString("SELECT id FROM calibration_stacks");
    QSqlQuery query;
    if(!query.prepare(queryString))
    {
        return -1;
    }

    if(!query.exec())
    {
        return -1;
    }

    int id = 0;
    int currentId;
    QVariant vData;
    while(query.next())
    {
        vData = query.value(0);
        if(!vData.isValid() || !vData.canConvert<int>())
        {
            continue;
        }
        currentId = vData.toInt();
        if(id < currentId)
        {
            id = currentId;
        }
    }

    return ++id;
}
//===============================================================
void ZChemicalTaskListDialog::zh_checkChemical(int id, const QString& chemical, bool& res) const
{
    QModelIndex index;
    QVariant vData;
    for(int row = 0; row < zv_chemicalTableModel->rowCount(); row++)
    {
        index = zv_chemicalTableModel->index(row, 1);
        if(!index.isValid())
        {
            continue;
        }

        vData = index.data(Qt::DisplayRole);
        if(!vData.isValid() || !vData.canConvert<QString>())
        {
            continue;
        }

        if(chemical == vData.toString())
        {
            // check Id
            index = zv_chemicalTableModel->index(row, 0);
            if(!index.isValid())
            {
                continue;
            }
            vData = index.data(Qt::DisplayRole);
            if(!vData.isValid() || !vData.canConvert<int>())
            {
                continue;
            }
            bool ok;
            int currentId = vData.toInt(&ok);
            if(!ok)
            {
                continue;
            }

            if(id ==  currentId)
            {
                continue;
            }
            res = false;
            return;
        }
    }

    res = true;
}
//===============================================================
void ZChemicalTaskListDialog::zh_checkChemicalTaskName(const QString& chemicalTaskName,
                                                       bool& res, QString &msg) const
{
    QModelIndex index;
    QVariant vData;
    for(int row = 0; row < zv_calibrationStackTableModel->rowCount(); row++)
    {
        index = zv_calibrationStackTableModel->index(row, 1);
        if(!index.isValid())
        {
            continue;
        }

        vData = index.data(Qt::DisplayRole);
        if(!vData.isValid() || !vData.canConvert<QString>())
        {
            continue;
        }

        if(vData.toString() == chemicalTaskName)
        {
            res = false;
            msg = tr("The chemical task name is not unique.");
            return;
        }
    }

    res = true;
}
//===============================================================
