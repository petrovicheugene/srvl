//=========================================================================
#include "ZChemicalTaskDialog.h"
#include "ZGeneral.h"
#include "ZMeasuringConditionsListDialog.h"
#include "ZChemicalListDialog.h"
#include "ZCalibrationListDialog.h"
#include "ZCalibrationTableWidget.h"
#include "ZCalibrationInTaskTableModel.h"
#include "ZChemicalTaskCalibrationModel.h"
#include "ZControlAction.h"
#include "ZNumericDelegate.h"

#include <QApplication>
#include <QDialogButtonBox>
#include <QHBoxLayout>
#include <QHeaderView>
#include <QLabel>
#include <QLineEdit>
#include <QMessageBox>
#include <QModelIndex>
#include <QPushButton>
#include <QSettings>
#include <QTableView>
#include <QTextEdit>
#include <QVBoxLayout>
#include <QSqlTableModel>
#include <QSqlQuery>
#include <QSqlError>
//=========================================================================
ZChemicalTaskDialog::ZChemicalTaskDialog(QSqlTableModel *chemicalTableModel, QWidget *parent)
    : QDialog(parent)
{
    setWindowTitle(tr("Chemical task"));
    setWindowFlags(Qt::Tool);

    if(chemicalTableModel == 0)
    {
        zv_chemicalSQLTableModel = new QSqlTableModel(this);
        zv_chemicalSQLTableModel->setTable("chemicals");
        zv_chemicalSQLTableModel->select();
        zv_chemicalSQLTableModel->setEditStrategy(QSqlTableModel::OnManualSubmit);
    }
    else
    {
        zv_chemicalSQLTableModel = chemicalTableModel;
    }
    zv_chemicalId = -1;
    zv_gainFactor = -1;
    zv_exposition = -1;
    zv_chemicalTaskId = -1;

    zh_createActions();
    zh_createComponents();
    zh_createConnections();
    zh_restoreSettings();
}
//=========================================================================
ZChemicalTaskDialog::~ZChemicalTaskDialog()
{
    zh_saveSettings();
}
//=========================================================================
void ZChemicalTaskDialog::zh_createActions()
{
    zv_selectCalibrationsAction = new ZControlAction(this);
    zv_selectCalibrationsAction->setText("Select");
    zv_selectCalibrationsAction->setToolTip(tr("Select calibrations"));

    zv_removeCalibrationsAction = new ZControlAction(this);
    zv_removeCalibrationsAction->setText("Remove");
    zv_removeCalibrationsAction->setToolTip(tr("Remove selected calibrations"));

    zv_moveCalibrationUpAction = new ZControlAction(this);
    zv_moveCalibrationUpAction->setText("Up");
    zv_moveCalibrationUpAction->setToolTip(tr("Move the calibration up"));

    zv_moveCalibrationDownAction = new ZControlAction(this);
    zv_moveCalibrationDownAction->setText("Down");
    zv_moveCalibrationDownAction->setToolTip(tr("Move the calibration down"));

}
//=========================================================================
void ZChemicalTaskDialog::zh_createComponents()
{
    // models
    zv_calibrationSQLTableModel = new QSqlTableModel(this);
    zv_calibrationSQLTableModel->setTable("calibrations");
    zv_calibrationSQLTableModel->select();
    zv_calibrationSQLTableModel->setEditStrategy(QSqlTableModel::OnManualSubmit);
    //    zv_filteredCalibrationSQLProxyModel = new ZFilteredCalibrationSQLProxyModel(this);
    //    zv_filteredCalibrationSQLProxyModel->setSourceModel(zv_calibrationSQLTableModel);

    zv_chemicalTaskCalibrationModel = new ZChemicalTaskCalibrationModel(this);
    zv_chemicalTaskCalibrationModel->zp_connectToCalibrationSQLTableModel(zv_calibrationSQLTableModel);
    // widgets
    QVBoxLayout* mainLayout = new QVBoxLayout;
    setLayout(mainLayout);

    // chemical
    QLabel* label = new QLabel(this);
    label->setText(glCreateCaption(tr("Chemical:")));
    mainLayout->addWidget(label);

    QHBoxLayout* levelLayout = new QHBoxLayout;
    mainLayout->addLayout(levelLayout);
    zv_chemicalLineEdfit = new QLineEdit(this);
    zv_chemicalLineEdfit->setReadOnly(true);
    levelLayout->addWidget(zv_chemicalLineEdfit);
    levelLayout->addStretch();

    // task name
    label = new QLabel(this);
    label->setText(glCreateCaption(tr("Task name:")));
    mainLayout->addWidget(label);
    levelLayout = new QHBoxLayout;
    mainLayout->addLayout(levelLayout);
    zv_chemicalTaskNameLineEdit = new QLineEdit(this);
    levelLayout->addWidget(zv_chemicalTaskNameLineEdit);
    levelLayout->addStretch();

    // measuring conditions
    label = new QLabel(this);
    label->setText(glCreateCaption(tr("Measuring conditions:")));
    mainLayout->addWidget(label);

    levelLayout = new QHBoxLayout;
    mainLayout->addLayout(levelLayout);
    zv_measuringConditionsLineEdit = new QLineEdit(this);
    zv_measuringConditionsLineEdit->setReadOnly(true);
    levelLayout->addWidget(zv_measuringConditionsLineEdit);
    levelLayout->addStretch();

    // calibration table
    zv_calibrationTableWidget = new ZCalibrationTableWidget(this);
    zv_calibrationTableWidget->zp_setCaption(glCreateCaption(tr("Calibrations:")));
    mainLayout->addWidget(zv_calibrationTableWidget);

    // description
    label = new QLabel(this);
    label->setText(glCreateCaption(tr("Description:")));
    mainLayout->addWidget(label);

    zv_descriptionTextEdit = new QTextEdit(this);
    int descriptionTextEditMaxHeight = zv_chemicalTaskNameLineEdit->height() * 2;
    zv_descriptionTextEdit->setMaximumHeight(descriptionTextEditMaxHeight);
    zv_descriptionTextEdit->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    mainLayout->addWidget(zv_descriptionTextEdit);

    // message
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
//=========================================================================
void ZChemicalTaskDialog::zh_createConnections()
{
    zv_calibrationTableWidget->zp_tableView()->setModel(zv_chemicalTaskCalibrationModel);

    // delegates for limit columns
    ZNumericDelegate* numericDelegate = new ZNumericDelegate;
    zv_calibrationTableWidget->zp_tableView()->setItemDelegateForColumn(2, numericDelegate);

    // numericDelegate = new ZNumericDelegate;
    zv_calibrationTableWidget->zp_tableView()->setItemDelegateForColumn(3, numericDelegate);

    //    QHeaderView* horizontalHeader = zv_calibrationTableWidget->zp_tableView()->horizontalHeader();
    //    horizontalHeader->setSectionResizeMode(0, QHeaderView::ResizeToContents);
    //    horizontalHeader->setSectionResizeMode(1, QHeaderView::Stretch);
    //    horizontalHeader->setSectionResizeMode(2, QHeaderView::ResizeToContents);
    //    horizontalHeader->setSectionResizeMode(3, QHeaderView::ResizeToContents);

    // actions
    QList<ZControlAction*> actionList;
    actionList.append(zv_moveCalibrationUpAction);
    actionList.append(zv_moveCalibrationDownAction);

    actionList.append(zv_selectCalibrationsAction);
    actionList.append(zv_removeCalibrationsAction);

    zv_calibrationTableWidget->zp_appendButtonActions(actionList);
    zv_calibrationTableWidget->zp_appendContextActions(actionList);

    connect(zv_moveCalibrationUpAction, &ZControlAction::triggered,
            this, &ZChemicalTaskDialog::zh_onMoveCalibrationUpAction);
    connect(zv_moveCalibrationDownAction, &ZControlAction::triggered,
            this, &ZChemicalTaskDialog::zh_onMoveCalibrationDownAction);


    connect(zv_selectCalibrationsAction, &ZControlAction::triggered,
            this, &ZChemicalTaskDialog::zh_onSelectCalibrationsAction);
    connect(zv_removeCalibrationsAction, &ZControlAction::triggered,
            this, &ZChemicalTaskDialog::zh_onRemoveCalibrationsAction);

    connect(zv_okButton, &QPushButton::clicked,
            this, &ZChemicalTaskDialog::zh_onOkButtonClick);
    connect(zv_cancelButton, &QPushButton::clicked,
            this, &ZChemicalTaskDialog::reject);

    connect(zv_chemicalTaskCalibrationModel, &QSqlTableModel::rowsInserted,
            this, &ZChemicalTaskDialog::zh_onCalibrationInserted);

}
//=========================================================================
void ZChemicalTaskDialog::zh_restoreSettings()
{
    QSettings settings;
    QVariant vData;
    settings.beginGroup(qApp->applicationVersion());
    settings.beginGroup("ChemicalTaskDialog");

    vData = settings.value("dialogGeometry");
    if(vData.isValid() && !vData.isNull() && vData.canConvert<QByteArray>())
    {
        this->restoreGeometry(vData.value<QByteArray>());
    }

    settings.endGroup();
    settings.endGroup();
}
//=========================================================================
void ZChemicalTaskDialog::zh_saveSettings() const
{
    QSettings settings;
    settings.beginGroup(qApp->applicationVersion());
    settings.beginGroup("ChemicalTaskDialog");

    settings.setValue("dialogGeometry", QVariant::fromValue<QByteArray>(this->saveGeometry()));
    //settings.setValue("splitterGeometry", QVariant::fromValue<QByteArray>(zv_tableSplitter->saveState()));

    settings.endGroup();
    settings.endGroup();
}
//=========================================================================
void ZChemicalTaskDialog::zh_onOkButtonClick()
{
    // check task name emptyness
    if(zv_chemicalTaskNameLineEdit->text().isEmpty())
    {
        QMessageBox msgBox;
        msgBox.setText(tr("The name of chemical task is empty."));
        //   msgBox.setInformativeText("Input the name of the chemical task.");
        msgBox.setIcon(QMessageBox::Warning);
        msgBox.addButton(QMessageBox::Ok);

        msgBox.exec();
        return;
    }

    // check task name uniqueness
    bool res;
    QString msg;
    emit zg_checkChemicalTaskName(zv_chemicalTaskNameLineEdit->text(), res, msg);
    if(!res)
    {
        QMessageBox msgBox;
        msgBox.setText(msg);
        //   msgBox.setInformativeText("Input the name of the chemical task.");
        msgBox.setIcon(QMessageBox::Warning);
        msgBox.addButton(QMessageBox::Ok);

        msgBox.exec();
        return;
    }

    // check measuring conditions
    if(zv_exposition < 1 || zv_gainFactor < 1)
    {
        QMessageBox msgBox;
        msgBox.setText(tr("Measuring conditions are invalid."));
        //   msgBox.setInformativeText("Input the name of the chemical task.");
        msgBox.setIcon(QMessageBox::Warning);
        msgBox.addButton(QMessageBox::Ok);

        msgBox.exec();
        return;
    }

    // check calibrations
    if(zv_chemicalTaskCalibrationModel->rowCount() < 1)
    {
        QMessageBox msgBox;
        msgBox.setText(tr("Chemical task has no colibrations."));
        //   msgBox.setInformativeText("Input the name of the chemical task.");
        msgBox.setIcon(QMessageBox::Warning);
        msgBox.addButton(QMessageBox::Ok);

        msgBox.exec();
        return;
    }

    // check calibration limihts
    if(zv_chemicalTaskCalibrationModel->rowCount() > 1)
    {
        double min;
        double max;
        for(int row = 0; row < zv_chemicalTaskCalibrationModel->rowCount(); row++)
        {
            zv_chemicalTaskCalibrationModel->zp_limitsForRow(row, min, max);
            if(min >= max)
            {
                QMessageBox msgBox;
                msgBox.setText(tr("Invalid concentration limits for calibration \"%1\" in row %2.")
                               .arg(zv_chemicalTaskCalibrationModel->zp_calibrationName(row), QString::number(row+1)));
                //   msgBox.setInformativeText("Input the name of the chemical task.");
                msgBox.setIcon(QMessageBox::Warning);
                msgBox.addButton(QMessageBox::Ok);
                msgBox.exec();
                return;
            }
        }
    }
    accept();
}
//=========================================================================
void ZChemicalTaskDialog::zh_onMoveCalibrationUpAction()
{
    // get current row
    QModelIndex currentIndex = zv_calibrationTableWidget->zp_tableView()->currentIndex();
    if(!currentIndex.isValid())
    {
        return;
    }

    int row = currentIndex.row();
    if(zv_chemicalTaskCalibrationModel->zp_moveRowUp(row))
    {
        currentIndex = zv_chemicalTaskCalibrationModel->index(row - 1, currentIndex.column());
        if(currentIndex.isValid())
        {
            zv_calibrationTableWidget->zp_tableView()->setCurrentIndex(currentIndex);
        }
    }
}
//=========================================================================
void ZChemicalTaskDialog::zh_onMoveCalibrationDownAction()
{
    // get current row
    QModelIndex currentIndex = zv_calibrationTableWidget->zp_tableView()->currentIndex();
    if(!currentIndex.isValid())
    {
        return;
    }

    int row = currentIndex.row();
    if(zv_chemicalTaskCalibrationModel->zp_moveRowDown(row))
    {
        currentIndex = zv_chemicalTaskCalibrationModel->index(row + 1, currentIndex.column());
        if(currentIndex.isValid())
        {
            zv_calibrationTableWidget->zp_tableView()->setCurrentIndex(currentIndex);
        }
    }
}
//=========================================================================
void ZChemicalTaskDialog::zh_onSelectCalibrationsAction()
{
    ZCalibrationListDialog dialog(zv_chemicalSQLTableModel, 0, zv_calibrationSQLTableModel);
    dialog.zp_setFilter(zv_chemicalId, zv_gainFactor, zv_exposition);
    if(!dialog.exec())
    {
        // reset chemical filter
        zv_calibrationSQLTableModel->setFilter(QString());
        return;
    }

    // get calibration id list
    QList<int> calibrationIdList;
    if(!dialog.zp_selectedCalibrationIdList(calibrationIdList))
    {
        // reset chemical filter
        zv_calibrationSQLTableModel->setFilter(QString());
        return;
    }

    // get measuring conditions from dialog
    int measurementConditionsId;
    int gainFactor;
    int exposition;
    if(!dialog.zp_currentMeasuringConditions(measurementConditionsId, gainFactor, exposition))
    {
        // reset chemical filter
        zv_calibrationSQLTableModel->setFilter(QString());
        return;
    }

    // check and load measuring conditions
    if(zv_gainFactor < 1 || zv_exposition < 1)
    {
        zh_setMeasuringConditions(measurementConditionsId, gainFactor, exposition);
    }
    else if(zv_gainFactor != gainFactor || zv_exposition != exposition)
    {
        // ASK WHAT TO DO
        QMessageBox msgBox;
        msgBox.setWindowTitle(tr("Chemical task"));
        msgBox.setText(tr("Measuring conditions of calibration you want to add are not equal to the current one."));
        msgBox.setInformativeText("Do you want to replace mesuring conditions and all calibrations in the task?");
        msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::Cancel);
        msgBox.setIcon(QMessageBox::Warning);
        msgBox.setDefaultButton(QMessageBox::Yes);
        int ret = msgBox.exec();

        if(ret == QMessageBox::Cancel)
        {
            // reset chemical filter
            zv_calibrationSQLTableModel->setFilter(QString());
            return;
        }

        zh_setMeasuringConditions(measurementConditionsId, gainFactor, exposition);
    }

    // load calibrations
    zv_calibrationSQLTableModel->setFilter(QString());
    zv_chemicalTaskCalibrationModel->zp_addCalibrationIdToFilter(calibrationIdList);

}
//=========================================================================
void ZChemicalTaskDialog::zh_setMeasuringConditions(int measurementConditionsId,
                                                    int gainFactor, int exposition)
{
    zv_measurementConditionsId = measurementConditionsId;
    zv_gainFactor = gainFactor;
    zv_exposition = exposition;

    QString mesauringConditionsString;
    if(zv_gainFactor >= 0 || zv_gainFactor >= 0)
    {
        mesauringConditionsString = tr("G.F.: %1 Exposition: %2").arg(QString::number(zv_gainFactor), QString::number(zv_exposition));
    }

    zv_measuringConditionsLineEdit->setText(mesauringConditionsString);
}
//=========================================================================
void ZChemicalTaskDialog::zh_onRemoveCalibrationsAction()
{
    // get selected calibration
    QModelIndexList selectedIndexList = zv_calibrationTableWidget->zp_tableView()->selectionModel()->selectedIndexes();
    QList<int> selectedRowList;
    foreach(QModelIndex index, selectedIndexList)
    {
        if(selectedRowList.contains(index.row()))
        {
            continue;
        }
        selectedRowList.append(index.row());
    }

    // qSort(selectedRowList);
    std::sort(selectedRowList.begin(), selectedRowList.end());
    for (int i = 0; i < selectedRowList.count(); i++)
    {
        zv_chemicalTaskCalibrationModel->removeRows(selectedRowList.at(i), 1);
    }

    // reset measuring conditions
    if(zv_chemicalTaskCalibrationModel->rowCount() < 1)
    {
        zh_setMeasuringConditions(-1, -1, -1);
    }
}
//=========================================================================
void ZChemicalTaskDialog::zh_onCalibrationInserted(const QModelIndex& parent, int first, int last)
{
    QHeaderView* horizontalHeader = zv_calibrationTableWidget->zp_tableView()->horizontalHeader();
    horizontalHeader->resizeSections(QHeaderView::ResizeToContents);
}
//=========================================================================
bool ZChemicalTaskDialog::zp_setChemicalTask(QSqlTableModel *calibrationStackTableModel,
                                             int chemicalTaskId, bool viewOnly)
{
    if(calibrationStackTableModel == 0 || zv_chemicalSQLTableModel == 0)
    {
        return false;
    }

    zv_chemicalTaskCalibrationModel->zp_setChemicalTaskId(chemicalTaskId);

    // find chemicaltask row
    int chemicalTaskRow = -1;
    QModelIndex index;
    QVariant vData;
    for(int row = 0; row < calibrationStackTableModel->rowCount(); row++ )
    {
        // find id
        index = calibrationStackTableModel->index(row, 0);
        if(!index.isValid())
        {
            continue;
        }
        vData = index.data(Qt::DisplayRole);
        if(!vData.isValid() || !vData.canConvert<int>())
        {
            continue;
        }

        if(vData.toInt() == chemicalTaskId)
        {
            chemicalTaskRow = row;
            break;
        }
    }

    if(chemicalTaskRow < 0)
    {
        return false;
    }

    // load chemical task properties
    zv_chemicalTaskId = chemicalTaskId;
    // chemical task name
    index = calibrationStackTableModel->index(chemicalTaskRow, 1);
    if(!index.isValid())
    {
        return false;
    }
    vData = index.data(Qt::DisplayRole);
    if(!vData.isValid() || !vData.canConvert<QString>())
    {
        return false;
    }
    zv_chemicalTaskNameLineEdit->setText(vData.toString());

    // description
    index = calibrationStackTableModel->index(chemicalTaskRow, 2);
    if(!index.isValid())
    {
        return false;
    }
    vData = index.data(Qt::DisplayRole);
    if(!vData.isValid() || !vData.canConvert<QString>())
    {
        return false;
    }
    zv_descriptionTextEdit->setText(vData.toString());

    // Chemical
    // get chemical id
    index = calibrationStackTableModel->index(chemicalTaskRow, 3);
    if(!index.isValid())
    {
        return false;
    }
    vData = index.data(Qt::DisplayRole);
    if(!vData.isValid() || !vData.canConvert<int>())
    {
        return false;
    }
    zv_chemicalId = vData.toInt();

    // get chemical name
    bool res = false;
    for(int row = 0; row < zv_chemicalSQLTableModel->rowCount(); row++)
    {
        index = zv_chemicalSQLTableModel->index(row, 0);
        if(!index.isValid())
        {
            continue;
        }
        vData = index.data(Qt::DisplayRole);
        if(!vData.isValid() || !vData.canConvert<int>())
        {
            continue;
        }

        if(vData.toInt() == zv_chemicalId)
        {
            // name itself
            index = zv_chemicalSQLTableModel->index(row, 1);
            if(!index.isValid())
            {
                return false;
            }
            vData = index.data(Qt::DisplayRole);
            if(!vData.isValid() || !vData.canConvert<QString>())
            {
                return false;;
            }

            zv_chemicalLineEdfit->setText(vData.toString());
            res = true;
            break;
        }
    }

    if(!res)
    {
        return false;
    }

    // measuring conditions


    // measurementConditionsId
    index = calibrationStackTableModel->index(chemicalTaskRow, 4);
    if(!index.isValid())
    {
        return false;
    }
    vData = index.data(Qt::DisplayRole);
    if(!vData.isValid() || !vData.canConvert<int>())
    {
        return false;
    }

    int measurementConditionsId = vData.toInt();

    QSqlQuery query;
    QString queryStrig = QString("SELECT gain_factor, exposition "
                                 "FROM measuring_conditions "
                                 "WHERE id=%1").arg(QString::number(measurementConditionsId));

    if(!query.prepare(queryStrig))
    {
        QString msg = query.lastError().text();
        QMessageBox::critical(this, tr("Error"), msg, QMessageBox::Ok);
        return false;
    }

    if(!query.exec())
    {
        QString msg = query.lastError().text();
        QMessageBox::critical(this, tr("Error"), msg, QMessageBox::Ok);
        return false;
    }

    if(!query.next())
    {
        QString msg = tr("Cannot get measurement conditions for id \"%1\" from database.").arg(QString::number(measurementConditionsId));
        QMessageBox::critical(this, tr("Error"), msg, QMessageBox::Ok);
        return false;
    }

    // gainFactor query
    vData = query.value(0);
    if(!vData.isValid() || !vData.canConvert<int>())
    {
        return false;
    }

    int gainFactor = vData.toInt();

    // exposition
    vData = query.value(1);
    if(!vData.isValid() || !vData.canConvert<int>())
    {
        return false;
    }

    int exposition = vData.toInt();

    zh_setMeasuringConditions(measurementConditionsId, gainFactor, exposition);

    // Load calibrations
    // request for calibration in the chemical task id list
    QString queryString = QString("SELECT calibrations_id  FROM calibrations_has_calibration_stacks "
                                  "WHERE calibration_stacks_id=%1").arg(QString::number(chemicalTaskId));
    query.clear();
    if(!query.prepare(queryString))
    {
        return false;
    }

    if(!query.exec())
    {
        return false;
    }

    QList<int> idList;
    while(query.next())
    {
        vData = query.value(0);
        if(!vData.isValid() || !vData.canConvert<int>())
        {
            return false;
        }

        idList.append(vData.toInt());
    }

    zv_chemicalTaskCalibrationModel->zp_addCalibrationIdToFilter(idList);

    return true;
}
//=========================================================================
bool ZChemicalTaskDialog::zp_setChemicalId(int id)
{
    if(zv_chemicalSQLTableModel == 0)
    {
        return false;
    }

    // check id
    QModelIndex index;
    QVariant vData;
    bool ok;
    int currentId;
    for(int row = 0; row < zv_chemicalSQLTableModel->rowCount(); row++)
    {
        index = zv_chemicalSQLTableModel->index(row, 0);
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

        if(id != currentId)
        {
            continue;
        }


        // chemical name
        index = zv_chemicalSQLTableModel->index(row, 1);
        if(!index.isValid())
        {
            return false;
        }

        vData = index.data(Qt::DisplayRole);
        if(!vData.isValid() || !vData.canConvert<QString>())
        {
            return false;
        }

        zv_chemicalLineEdfit->setText(vData.toString());
        zv_chemicalId = id;
        return true;
    }

    return false;
}
//=========================================================================
int ZChemicalTaskDialog::zp_chemicalId() const
{
    return zv_chemicalId;
}
//=========================================================================
QString ZChemicalTaskDialog::zp_chemicalTaskName() const
{
    return zv_chemicalTaskNameLineEdit->text();
}
//=========================================================================
QString ZChemicalTaskDialog::zp_description() const
{
    return zv_descriptionTextEdit->toPlainText();
}
//=========================================================================
int ZChemicalTaskDialog::zp_exposition() const
{
    return zv_exposition;
}
//=========================================================================
int ZChemicalTaskDialog::zp_measurementConditionsId() const
{
    return zv_measurementConditionsId;
}
//=========================================================================
int ZChemicalTaskDialog::zp_gainFactor() const
{
    return zv_gainFactor;
}
//=========================================================================
void ZChemicalTaskDialog::zp_calibrationIdList(QList<int>& calibrationIdList) const
{
    for(int row = 0; row < zv_chemicalTaskCalibrationModel->rowCount(); row++)
    {
        calibrationIdList.append(zv_chemicalTaskCalibrationModel->zh_calibrationId(row));
    }
}
//=========================================================================
void ZChemicalTaskDialog::zp_concentrationLimitsList(QList<QPair<double, double> >& limitsList) const
{
    for(int row = 0; row < zv_chemicalTaskCalibrationModel->rowCount(); row++)
    {
        QPair<double, double> limits;
        zv_chemicalTaskCalibrationModel->zp_limitsForRow(row, limits.first, limits.second);
        limitsList.append(limits);
    }
}
//=========================================================================
bool ZChemicalTaskDialog::zp_limitsForRow(int row, double& min, double& max) const
{
    return zv_chemicalTaskCalibrationModel->zp_limitsForRow(row, min, max);
}
//=========================================================================
