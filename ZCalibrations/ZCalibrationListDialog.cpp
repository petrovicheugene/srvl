//===============================================================
#include "ZCalibrationListDialog.h"
#include "ZGeneral.h"
#include "ZBaseTableWidget.h"
#include "ZCalibration.h"
#include "ZReadOnlyStyledItemDelegate.h"
#include "ZXMLCalibrationIOHandler.h"
#include "ZSetGainFactorToCalibrationDialog.h"
#include "ZCalibrationFilterProxyModel.h"

#include <QItemSelectionModel>
#include <QDir>
#include <QDialogButtonBox>
#include <QHBoxLayout>
#include <QHeaderView>
#include <QLabel>
#include <QMessageBox>
#include <QPushButton>
#include <QSettings>
#include <QSqlTableModel>
#include <QSqlRecord>
#include <QSqlQuery>
#include <QSqlField>
#include <QSqlError>
#include <QSplitter>
#include <QStandardPaths>
#include <QTableView>
#include <QStandardItemModel>
#include <QStandardItem>
#include <QVBoxLayout>
//===============================================================
ZCalibrationListDialog::ZCalibrationListDialog(QSqlTableModel *chemicalTableModel,
                                               QSqlTableModel* measuringConditionsSQLTableModel,
                                               QSqlTableModel* calibrationTableSQLModel,
                                               QWidget *parent) : QDialog(parent)
{
    setWindowTitle(tr("Calibrations"));
    setWindowFlags(Qt::Tool);

    //    zv_chemicalFilter = QString();
    //    zv_gainFactorFilter = -1;
    //    zv_expositionFilter = -1;

    zh_createComponents(chemicalTableModel, measuringConditionsSQLTableModel, calibrationTableSQLModel);
    zh_createConnections();
    zh_restoreSettings();
}
//===============================================================
ZCalibrationListDialog::~ZCalibrationListDialog()
{
    zh_saveSettings();
}
//===============================================================
bool ZCalibrationListDialog::zp_setFilter(int chemicalId , int gainFactor, int exposition)
{
    QModelIndex index;
    QVariant vData;
    bool ok;
    bool res = false;
    int currentId;
    QString chemicalFilter;
    QString filterInfoString;
    // chemical
    for(int row = 0; row < zv_chemicalTableSQLModel->rowCount(); row++)
    {
        index = zv_chemicalTableSQLModel->index(row, 0);
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

        if(currentId != chemicalId)
        {
            continue;
        }

        // set current chem element and hide chem element tableWidget
        index = zv_chemicalTableSQLModel->index(row, 1);
        if(!index.isValid())
        {
            return false;
        }

        vData = index.data(Qt::DisplayRole);
        if(!vData.isValid() || !vData.canConvert<QString>())
        {
            return false;
        }

        chemicalFilter = vData.toString();
        zv_chemicalTableWidget->zp_tableView()->setCurrentIndex(index);
        zv_chemicalTableWidget->setHidden(true);
        // zv_chemicalFilter = vData.toString();
        res = true;
        break;
    }

    if(res)
    {
        filterInfoString = tr("Chemical %1").arg(chemicalFilter);
        if(gainFactor > 0 && exposition > 0)
        {
            // set measuring conditions filter
            int row = -1;
            if(zh_checkMeasuringConditionsExistance(gainFactor, exposition, row))
            {
                if(row >= 0 && row < zv_measuringConditionsModel->rowCount())
                {
                    index = zv_measuringConditionsModel->index(row, 0);
                    if(index.isValid())
                    {
                        zv_measuringConditionsTableWidget->zp_tableView()->setCurrentIndex(index);
                        zv_measuringConditionsTableWidget->setHidden(true);
                        filterInfoString += tr(" G.F. - %1, exposition - %2").arg(QString::number(gainFactor), QString::number(exposition));
                    }
                }
            }
        }
    }

    if(!filterInfoString.isEmpty())
    {
        zv_filterInfoLabel->setText(glCreateCaption(filterInfoString));
        zv_filterInfoLabel->setVisible(true);
    }

    return res;
}
//===============================================================
bool ZCalibrationListDialog::zp_selectedCalibrationIdList(QList<int>& calibrationIdList) const
{
    calibrationIdList.clear();
    // get selected calibration rows
    QModelIndexList selectedIndexes = zv_calibrationTableWidget->zp_tableView()->selectionModel()->selectedIndexes();
    if(selectedIndexes.isEmpty())
    {
        return false;
    }

    QList<int> rowList;
    for(int i = 0; i < selectedIndexes.count(); i++)
    {
        if(!rowList.contains(selectedIndexes.at(i).row()))
        {
            rowList.append(selectedIndexes.at(i).row());
        }
    }

    qSort(rowList);

    // get selected calibration id list
    QModelIndex index;
    QVariant vData;
    int id;
    bool ok;
    for(int i = 0; i < rowList.count(); i++)
    {
        index = zv_calibrationProxyModel->index(rowList.at(i), 0);
        if(!index.isValid())
        {
            continue;
        }

        vData = index.data(Qt::DisplayRole);
        if(!vData.isValid() || !vData.canConvert<int>())
        {
            continue;
        }

        id = vData.toInt(&ok);
        if(!ok)
        {
            continue;
        }

        calibrationIdList.append(id);
    }

    return !calibrationIdList.isEmpty();
}
//===============================================================
void ZCalibrationListDialog::zh_createComponents(QSqlTableModel *chemicalTableModel,
                                                 QSqlTableModel* measuringConditionsSQLTableModel,
                                                 QSqlTableModel* calibrationTableSQLModel)
{
    // Models
    // chemicals model
    if(chemicalTableModel == 0)
    {
        zv_chemicalTableSQLModel = new QSqlTableModel(this);
        zv_chemicalTableSQLModel->setTable("chemicals");
        zv_chemicalTableSQLModel->select();
        zv_chemicalTableSQLModel->setEditStrategy(QSqlTableModel::OnManualSubmit);
        zv_chemicalTableSQLModel->setHeaderData(1, Qt::Horizontal, QVariant(tr("Name")));
    }
    else
    {
        zv_chemicalTableSQLModel = chemicalTableModel;
    }

    // calibrations model
    if(calibrationTableSQLModel == 0)
    {
        zv_calibrationTableSQLModel = new QSqlTableModel(this);
        zv_calibrationTableSQLModel->setTable("calibrations");
        zv_calibrationTableSQLModel->select();
        zv_calibrationTableSQLModel->setEditStrategy(QSqlTableModel::OnManualSubmit);
    }
    else
    {
        zv_calibrationTableSQLModel = calibrationTableSQLModel;
    }

    zv_calibrationProxyModel = new ZCalibrationFilterProxyModel(this);
    zv_calibrationProxyModel->setSourceModel(zv_calibrationTableSQLModel);

    // measuring conditions model
    if(measuringConditionsSQLTableModel == 0)
    {
        zv_measuringConditionsSQLTableModel = new QSqlTableModel(this);
        zv_measuringConditionsSQLTableModel->setTable("measuring_conditions");
        zv_measuringConditionsSQLTableModel->select();
        zv_measuringConditionsSQLTableModel->setEditStrategy(QSqlTableModel::OnManualSubmit);
    }
    else
    {
        zv_measuringConditionsSQLTableModel = measuringConditionsSQLTableModel;
    }

    zv_measuringConditionsSQLTableModel->setHeaderData(1, Qt::Horizontal, QVariant(tr("Gain factor")));
    zv_measuringConditionsSQLTableModel->setHeaderData(2, Qt::Horizontal, QVariant(tr("Exposition")));


    zv_measuringConditionsModel = new QStandardItemModel(this);
    QStringList horizontalHeaderLabelList;
    horizontalHeaderLabelList.append(tr("Gain Factor"));
    horizontalHeaderLabelList.append(tr("Exposition"));
    zv_measuringConditionsModel->setHorizontalHeaderLabels(horizontalHeaderLabelList);

    // Controls
    QVBoxLayout* mainLayout = new QVBoxLayout(this);
    setLayout(mainLayout);

    // filter info label
    zv_filterInfoLabel = new QLabel(this);
    mainLayout->addWidget(zv_filterInfoLabel);
    zv_filterInfoLabel->setVisible(false);

    // splitter
    zv_mainSplitter = new QSplitter(this);
    zv_mainSplitter->setChildrenCollapsible(false);
    mainLayout->addWidget(zv_mainSplitter, 999999);

    zv_chemicalTableWidget =  new ZBaseTableWidget(this);
    zv_chemicalTableWidget->zp_setCaption(glCreateCaption(tr("Chemicals")));
    zv_mainSplitter->addWidget(zv_chemicalTableWidget);

    zv_measuringConditionsTableWidget =  new ZBaseTableWidget(this);
    zv_measuringConditionsTableWidget->zp_setCaption(glCreateCaption(tr("Measuring conditions")));
    zv_mainSplitter->addWidget(zv_measuringConditionsTableWidget);

    zv_calibrationTableWidget =  new ZBaseTableWidget(this);
    zv_calibrationTableWidget->zp_setCaption(glCreateCaption(tr("Calibrations")));
    zv_mainSplitter->addWidget(zv_calibrationTableWidget);


    // load and remove calibration button
    QHBoxLayout* levelLayout = new QHBoxLayout(this);
    mainLayout->addLayout(levelLayout);
    levelLayout->addStretch();

    zv_loadCalibrationButton = new QPushButton(this);
    zv_loadCalibrationButton->setText(tr("Load"));
    zv_loadCalibrationButton->setFlat(true);
    levelLayout->addWidget(zv_loadCalibrationButton);

    zv_removeCalibrationButton = new QPushButton(this);
    zv_removeCalibrationButton->setText(tr("Remove"));
    zv_removeCalibrationButton->setFlat(true);
    levelLayout->addWidget(zv_removeCalibrationButton);

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
//===============================================================
void ZCalibrationListDialog::zh_createConnections()
{
    zv_chemicalTableWidget->zp_setModel(zv_chemicalTableSQLModel);
    if(zv_chemicalTableWidget->zp_tableView()->itemDelegate())
    {
        delete zv_chemicalTableWidget->zp_tableView()->itemDelegate();
    }
    ZReadOnlyStyledItemDelegate* delegate = new ZReadOnlyStyledItemDelegate;
    zv_chemicalTableWidget->zp_tableView()->setItemDelegate(delegate);
    zv_chemicalTableWidget->zp_tableView()->setSelectionMode(QAbstractItemView::SingleSelection);
    zv_chemicalTableWidget->zp_tableView()->setSelectionBehavior(QAbstractItemView::SelectRows);

    zv_chemicalTableWidget->zp_setColumnHidden(0, true);
    zv_chemicalTableWidget->zp_tableView()->horizontalHeader()->setSectionResizeMode(1, QHeaderView::Stretch);


    zv_calibrationTableWidget->zp_setModel(zv_calibrationProxyModel);
    zv_calibrationTableWidget->zp_tableView()->setSelectionBehavior(QAbstractItemView::SelectRows);
    zv_calibrationTableWidget->zp_tableView()->setColumnHidden(0, true);
    zv_calibrationTableWidget->zp_tableView()->setColumnHidden(3, true);
    zv_calibrationTableWidget->zp_tableView()->setColumnHidden(4, true);
    zv_calibrationTableWidget->zp_tableView()->setColumnHidden(5, true);
    zv_calibrationTableWidget->zp_tableView()->setColumnHidden(6, true);
    zv_calibrationTableWidget->zp_tableView()->setColumnHidden(7, true);
    zv_calibrationTableWidget->zp_tableView()->horizontalHeader()->setStretchLastSection(true);

    zv_measuringConditionsTableWidget->zp_setModel(zv_measuringConditionsModel);

    zv_measuringConditionsTableWidget->zp_tableView()->setSelectionBehavior(QAbstractItemView::SelectRows);
    zv_measuringConditionsTableWidget->zp_tableView()->horizontalHeader()->setStretchLastSection(true);

    // connect dependent tables filtering
    connect(zv_chemicalTableWidget->zp_tableView()->selectionModel(), &QItemSelectionModel::currentChanged,
            this, &ZCalibrationListDialog::zh_onCurrentChemicalChange);
    connect(zv_measuringConditionsTableWidget->zp_tableView()->selectionModel(), &QItemSelectionModel::currentChanged,
            this, &ZCalibrationListDialog::zh_onCurrentMeasuringConditionsChange);
    connect(zv_calibrationTableSQLModel, &ZCalibrationFilterProxyModel::rowsInserted,
            this, &ZCalibrationListDialog::zh_onCalibrationInsertion);
    connect(zv_calibrationTableSQLModel, &ZCalibrationFilterProxyModel::rowsRemoved,
            this, &ZCalibrationListDialog::zh_onCalibrationRemoving);

    //
    connect(zv_okButton, &QPushButton::clicked,
            this, &ZCalibrationListDialog::zh_onOkButtonClick);
    connect(zv_cancelButton, &QPushButton::clicked,
            this, &ZCalibrationListDialog::reject);

    connect(zv_loadCalibrationButton, &QPushButton::clicked,
            this, &ZCalibrationListDialog::zh_loadCalibrationsFromFile);
    connect(zv_removeCalibrationButton, &QPushButton::clicked,
            this, &ZCalibrationListDialog::zh_removeSelectedCalibration);

}
//===============================================================
void ZCalibrationListDialog::zh_onCurrentChemicalChange(const QModelIndex& current, const QModelIndex previous)
{
    zh_setFilterToCalibrationTableSQLModel(current);
}
//===============================================================
void ZCalibrationListDialog::zh_setFilterToCalibrationTableSQLModel(const QModelIndex& chemicalIndex)
{
    int row = chemicalIndex.row();
    QModelIndex index = zv_chemicalTableSQLModel->index(row, 0);
    int chemicalId = -1;
    if(index.isValid())
    {
        QVariant vData = index.data(Qt::DisplayRole);
        if(vData.isValid() && vData.canConvert<int>())
        {
            bool ok;
            chemicalId = vData.toInt(&ok);
            if(!ok)
            {
                chemicalId = -1;
            }
        }
    }

    QString filterString = QString("chemicals_id=%1").arg(QString::number(chemicalId));
    zv_calibrationTableSQLModel->setFilter(filterString);
    // repopulate measuring conditions model
    zh_repopulateMeasuringConditionsModel();
}
//===============================================================
void ZCalibrationListDialog::zh_repopulateMeasuringConditionsModel()
{
    // store current measuring conditions
    int currentMeasurementConditionsId = -1;
    int currentGainFactor = -1;
    int currentExposition = -1;
    bool currentMeasuringConditionsisValid = zp_currentMeasuringConditions(currentMeasurementConditionsId,
                                                                           currentGainFactor, currentExposition);

    zv_measuringConditionsModel->removeRows(0, zv_measuringConditionsModel->rowCount());

    QList<int>  measuringConditionsList;
    zh_getMeasuringConditionsForCurrentChemical(measuringConditionsList);
    QStandardItem* item;
    QSqlQuery query;
    QString queryString;
    for(int row = 0; row < measuringConditionsList.count(); row++)
    {

        query.clear();
        queryString = QString("SELECT gain_factor, exposition FROM measuring_conditions "
                              "WHERE id=%1").arg(QString::number(measuringConditionsList.at(row)));

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

        bool ok;
        QVariant vData = query.value(0);
        if(!vData.isValid() || !vData.canConvert<int>())
        {
            continue;
        }

        int gainFactor = vData.toInt(&ok);
        if(!ok)
        {
            continue;
        }

        vData = query.value(1);
        if(!vData.isValid() || !vData.canConvert<int>())
        {
            continue;
        }

        int exposition = vData.toInt(&ok);
        if(!ok)
        {
            continue;
        }

        // measuring conditions id
        item = new QStandardItem(QString::number(measuringConditionsList.at(row)));
        item->setEditable(false);
        zv_measuringConditionsModel->setItem(row , 0, item);

        item = new QStandardItem(QString::number(gainFactor));
        item->setEditable(false);
        zv_measuringConditionsModel->setItem(row , 1, item);

        // exposition
        item = new QStandardItem(QString::number(exposition));
        item->setEditable(false);
        zv_measuringConditionsModel->setItem(row, 2, item);
    }

    zv_measuringConditionsModel->setHeaderData(1, Qt::Horizontal, QVariant(tr("Gain factor")));
    zv_measuringConditionsModel->setHeaderData(2, Qt::Horizontal, QVariant(tr("Exposition")));

    if(zv_measuringConditionsModel->rowCount() > 0)
    {
        int currentRow;
        if(!currentMeasuringConditionsisValid || !zh_checkMeasuringConditionsExistance(currentGainFactor, currentExposition, currentRow))
        {
            currentRow = 0;
        }

        zv_measuringConditionsTableWidget->zp_setColumnHidden(0, true);
        zv_measuringConditionsTableWidget->zp_tableView()->horizontalHeader()->setStretchLastSection(true);

        QModelIndex index = zv_measuringConditionsModel->index(currentRow,0);
        if(index.isValid())
        {
            zv_measuringConditionsTableWidget->zp_tableView()->setCurrentIndex(index);
        }
    }
}
//===============================================================
void ZCalibrationListDialog::zh_onCurrentMeasuringConditionsChange(const QModelIndex& current, const QModelIndex previous)
{
    //zv_calibrationProxyModel->invalidate();
    int row = current.row();

    // id
    QModelIndex index = zv_measuringConditionsModel->index(row, 0);
    if(!index.isValid())
    {
        zv_calibrationProxyModel->zp_setMeasuringConditionsFilter(-1);
        zv_calibrationTableWidget->zp_tableView()->reset();
        return;
    }

    QVariant vData = index.data(Qt::DisplayRole);
    if(!vData.isValid() || !vData.canConvert<QString>())
    {
        zv_calibrationProxyModel->zp_setMeasuringConditionsFilter(-1);
        zv_calibrationTableWidget->zp_tableView()->reset();
        return;
    }

    QString sData = vData.toString();
    bool ok;
    int id = sData.toInt(&ok);
    if(!ok)
    {
        zv_calibrationProxyModel->zp_setMeasuringConditionsFilter(-1);
        zv_calibrationTableWidget->zp_tableView()->reset();
        return;
    }

    zv_calibrationProxyModel->zp_setMeasuringConditionsFilter(id);
    zv_calibrationTableWidget->zp_tableView()->horizontalHeader()->setStretchLastSection(true);
//    QHeaderView* horizontalHeader = zv_calibrationTableWidget->zp_tableView()->horizontalHeader();
//    horizontalHeader->resizeSections(QHeaderView::ResizeToContents);

}
//===============================================================
void ZCalibrationListDialog::zh_getMeasuringConditionsForCurrentChemical(QList<int>& measuringConditionsList) const
{
    measuringConditionsList.clear();
    QModelIndex index;
    QVariant vData;
    bool ok;
    int measurementConditionsId;
    for(int row = 0; row < zv_calibrationTableSQLModel->rowCount(); row++)
    {
        // gain factor
        index = zv_calibrationTableSQLModel->index(row, 5);
        if(!index.isValid())
        {
            continue;
        }
        vData = index.data(Qt::DisplayRole);
        if(!vData.isValid() || !vData.canConvert<int>())
        {
            continue;
        }

        measurementConditionsId = vData.toInt(&ok);

        if(!ok)
        {
            continue;
        }

        if(measuringConditionsList.contains(measurementConditionsId))
        {
            continue;
        }

        measuringConditionsList.append(measurementConditionsId);
    }
}
//===============================================================
void ZCalibrationListDialog::zh_onOkButtonClick()
{
    accept();
}
//===============================================================
void ZCalibrationListDialog::zh_loadCalibrationsFromFile()
{
    QStringList calibrationPathList = ZXMLCalibrationIOHandler::zp_getCalibrationOpenFiles(zv_calibrationFolderPath);
    if(calibrationPathList.isEmpty())
    {
        return;
    }

    QFileInfo fileInfo(calibrationPathList.last());
    zv_calibrationFolderPath = fileInfo.absolutePath();

    QFile file;
    ZXMLCalibrationIOHandler calibrationIOHandler;

    LoadingPermission loadingPermission = LP_ASK;

    for(int i = 0; i < calibrationPathList.count(); i++)
    {
        if(file.isOpen())
        {
            file.close();
        }

        file.setFileName(calibrationPathList.at(i));
        if(!file.open(QIODevice::ReadOnly))
        {
            QString msg = file.errorString();
            QMessageBox::critical(this, tr("Error"), msg, QMessageBox::Ok);
            continue;
        }

        // create calibration
        ZCalibration calibration(calibrationPathList.at(i));
        if(!calibrationIOHandler.zp_getCalibrationFromFile(file, &calibration))
        {
            continue;
        }

        QByteArray calibrationXMLByteArray;
        if(!calibrationIOHandler.zp_getCalibrationXMLByteArrayFromFile(file, calibrationXMLByteArray))
        {
            continue;
        }

        // input gain factor if it doen't exist
        if(calibration.zp_gainFactor() < 1)
        {
            ZSetGainFactorToCalibrationDialog dialog(calibration.zp_baseName());
            if(!dialog.exec())
            {
                QString msg = tr("Calibration %1 will not be loaded.").arg(calibration.zp_baseName());
                QMessageBox::warning(this, tr("Calibration loading"), msg, QMessageBox::Ok);
                continue;
            }
        }

        // check conformity and load calibration
        if(loadingPermission == LP_LOAD_ALL || zh_checkCalibrationConformity(calibration, loadingPermission))
        {
            QString msg;
            if(!zh_writeCalibrationToDatabase(calibration, calibrationXMLByteArray, msg))
            {
                QMessageBox msgBox;
                msgBox.setWindowTitle(tr("Calibration loading"));
                msgBox.setText(tr("Error writing the calibration \"%1\" to database. %2")
                               .arg(calibration.zp_baseName(), msg));
                msgBox.setInformativeText("Continue loading the calibration?");
                msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::Cancel);
                msgBox.setIcon(QMessageBox::Critical);
                msgBox.setDefaultButton(QMessageBox::Yes);
                int ret = msgBox.exec();

                if(ret == QMessageBox::Yes)
                {
                    continue;
                }
                else // QMessageBox::Cancel:
                {
                    break;
                }
            }
        }

        if(file.isOpen())
        {
            file.close();
        }
    }

    QHeaderView* horizontalHeader = zv_calibrationTableWidget->zp_tableView()->horizontalHeader();
    horizontalHeader->resizeSections(QHeaderView::ResizeToContents);
}
//===============================================================
bool ZCalibrationListDialog::zh_checkCalibrationConformity(const ZCalibration& calibration,
                                                           LoadingPermission& loadingPermission) const
{
    if(loadingPermission == LP_LOAD_ALL)
    {
        return true;
    }

    else if(loadingPermission == LP_ASK)
    {
        if(zv_chemicalTableSQLModel->rowCount() < 1)
        {
            return true;
        }

        // get current chemical
        QString currentChemical;
        int currentChemicalId;
        if(!zp_currentChemical(currentChemicalId, currentChemical))
        {
            return true;
        }

        // get measuring conditions
        int measurementConditionsId;
        int gainFactor;
        int exposition;
        if(!zp_currentMeasuringConditions(measurementConditionsId, gainFactor, exposition))
        {
            return true;
        }

        // check conformity of chemical, measuring conditions and calibration properties
        bool chemicalConformity = currentChemical == calibration.zp_chemElement();
        bool measuringConditionsComformity = gainFactor == calibration.zp_gainFactor()
                && exposition == calibration.zp_exposition();
        if(!chemicalConformity || !measuringConditionsComformity)
        {
            // ask
            QMessageBox msgBox;
            QString msg = tr("The calibration \"%1\" has nonconforming ").arg(calibration.zp_baseName());
            if(!chemicalConformity)
            {
                msg += tr("chemical \"%1\"").arg(calibration.zp_chemElement());
                if(!measuringConditionsComformity)
                {
                    msg += tr(" and measuring conditions gain factor - %1, exposition - %2").
                            arg(QString::number(calibration.zp_gainFactor()), QString::number(calibration.zp_exposition()));
                }
            }
            else if(!measuringConditionsComformity)
            {
                msg += tr("measuring conditions gain factor - %1, exposition - %2").
                        arg(QString::number(calibration.zp_gainFactor()), QString::number(calibration.zp_exposition()));
            }

            msg += ".";


            msgBox.setText(msg);
            msg.clear();
            msg = tr("Do you want to create ");
            if(!chemicalConformity)
            {
                msg += tr("a new chemical");
                if(!measuringConditionsComformity)
                {
                    msg += tr(" and measuring conditions");
                }
            }
            else if(!measuringConditionsComformity)
            {
                msg += tr("new measuring conditions");
            }
            msg += "?";
            msgBox.setInformativeText(msg);
            msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::YesToAll | QMessageBox::No | QMessageBox::NoToAll);
            msgBox.setIcon(QMessageBox::Warning);
            msgBox.setDefaultButton(QMessageBox::Yes);
            int ret = msgBox.exec();

            switch (ret)
            {
            case QMessageBox::Yes:
                return true;

            case QMessageBox::YesToAll:
                loadingPermission = LP_LOAD_ALL;
                return true;

            case QMessageBox::No:
                return false;

            case QMessageBox::NoToAll:
                loadingPermission = LP_LOAD_CONFORMING;
                return false;

            default:
                // should never be reached
                return true;
            }
        }
        else
        {
            return true;
        }
    }
    else // if(loadingPermission == LP_LOAD_CONFORMING)
    {
        // get current chemical
        QString currentChemical;
        int currentChemicalId;
        if(!zp_currentChemical(currentChemicalId, currentChemical))
        {
            return false;
        }

        // get measuring conditions
        int measurementConditionsId;
        int gainFactor;
        int exposition;
        if(!zp_currentMeasuringConditions(measurementConditionsId, gainFactor, exposition))
        {
            return false;
        }

        // check conformity of chemical, measuring conditions and calibration properties
        if(currentChemical != calibration.zp_chemElement()
                ||  gainFactor != calibration.zp_gainFactor()
                || exposition != calibration.zp_exposition())
        {
            return false;
        }
        else
        {
            return true;
        }
    }

    return false;
}
//===============================================================
bool ZCalibrationListDialog::zp_currentChemical(int& id, QString& chemical) const
{
    // checks
    QModelIndex index = zv_chemicalTableWidget->zp_tableView()->currentIndex();
    if(!index.isValid() || index.row() < 0 || index.row() >= zv_chemicalTableSQLModel->rowCount())
    {
        return false;
    }

    // chemical name index
    index = zv_chemicalTableSQLModel->index(index.row(), 1);
    if(!index.isValid())
    {
        return false;
    }

    QVariant vData = index.data(Qt::DisplayRole);
    if(!vData.isValid() || !vData.canConvert<QString>())
    {
        return false;
    }

    chemical = vData.toString();
    return true;
}
//===============================================================
bool ZCalibrationListDialog::zp_currentMeasuringConditions(int& measurementConditionsId,
                                                           int& gainFactor, int& exposition) const
{
    // checks
    QModelIndex index = zv_measuringConditionsTableWidget->zp_tableView()->currentIndex();
    if(!index.isValid() || index.row() < 0 || index.row() >= zv_measuringConditionsSQLTableModel->rowCount())
    {
        return false;
    }

    int row = index.row();
    // gainFactor
    index = zv_measuringConditionsSQLTableModel->index(row, 0);
    if(!index.isValid())
    {
        return false;
    }

    QVariant vData = index.data(Qt::DisplayRole);
    if(!vData.isValid() || !vData.canConvert<int>())
    {
        return false;
    }

    bool ok;
    measurementConditionsId = vData.toInt(&ok);
    if(!ok)
    {
        return false;
    }

    // gainFactor
    index = zv_measuringConditionsSQLTableModel->index(row, 1);
    if(!index.isValid())
    {
        return false;
    }

    vData = index.data(Qt::DisplayRole);
    if(!vData.isValid() || !vData.canConvert<int>())
    {
        return false;
    }

    gainFactor = vData.toInt(&ok);
    if(!ok)
    {
        return false;
    }

    // exposition
    index = zv_measuringConditionsSQLTableModel->index(row, 2);
    if(!index.isValid())
    {
        return false;
    }

    vData = index.data(Qt::DisplayRole);
    if(!vData.isValid() || !vData.canConvert<int>())
    {
        return false;
    }

    exposition = vData.toInt(&ok);
    if(!ok)
    {
        return false;
    }

    return true;
}
//===============================================================
bool ZCalibrationListDialog::zh_writeCalibrationToDatabase(const ZCalibration& calibration,
                                                           const QByteArray& calibrationXMLByteArray,  QString& msg)
{
    int calibrationId = zh_findNewCalibrationlId(msg);
    if(!msg.isEmpty())
    {
        return false;
    }

    // write chemElement to database if it doesn't exist
    int chemicalId;
    if(!zh_writeChemicalToDatabase(calibration.zp_chemElement(), chemicalId, msg))
    {
        return false;
    }

    // write measuring conditions to database if they don't exist
    int measurementConditionsId;
    if(!zh_writeMeasuringConditionsToDatabase(calibration.zp_gainFactor(), calibration.zp_exposition(), measurementConditionsId, msg))
    {
        return false;
    }

    // write calibration
    QSqlRecord record;
    record.append(QSqlField("id", QVariant::Int));
    record.append(QSqlField("name", QVariant::String));
    record.append(QSqlField("description", QVariant::String));
    record.append(QSqlField("calibration_data", QVariant::ByteArray));
    record.append(QSqlField("chemicals_id", QVariant::Int));
    record.append(QSqlField("measuring_conditions_id", QVariant::Int));
    record.append(QSqlField("measurement_units_id", QVariant::Int));

    record.setValue(0, QVariant(calibrationId)); // id
    record.setValue(1, QVariant(calibration.zp_baseName())); // name
    record.setValue(2, QVariant()); // description
    record.setValue(3, QVariant(calibrationXMLByteArray)); // byte array
    record.setValue(4, QVariant(chemicalId)); // chemical id
    record.setValue(5, QVariant(measurementConditionsId)); // measurementConditionsId id
    record.setValue(6, QVariant()); // measurement unit

    //    record.setValue(5, QVariant(calibration.zp_gainFactor())); // gain factor
    //    record.setValue(6, QVariant(calibration.zp_exposition())); // exposition

    if(!zv_calibrationTableSQLModel->insertRecord(-1, record))
    {
        msg = zv_calibrationTableSQLModel->lastError().text();
        return false;
    }

    if(!zv_calibrationTableSQLModel->submitAll())
    {
        msg = zv_calibrationTableSQLModel->lastError().text();
        zv_calibrationTableSQLModel->revertAll();
        return false;
    }

    zv_calibrationTableSQLModel->select();
    zh_repopulateMeasuringConditionsModel();
    return true;
}
//===============================================================
bool ZCalibrationListDialog::zh_writeChemicalToDatabase(const QString& chemical, int& id, QString& msg)
{
    if(zh_checkChemicalExistance(chemical, id))
    {
        return true;
    }

    QSqlRecord record;
    record.append(QSqlField("id", QVariant::Int));
    record.append(QSqlField("name", QVariant::String));

    id = zh_findNewChemicalId();
    if(id < 1)
    {
        msg = tr("Cannot define id for writing chemical \"%1\"").arg(chemical);
        return false;
    }
    record.setValue(0, QVariant(id));
    record.setValue(1, QVariant(chemical));

    if(!zv_chemicalTableSQLModel->insertRecord(-1, record))
    {
        msg = zv_chemicalTableSQLModel->lastError().text();
        return false;
    }

    if(!zv_chemicalTableSQLModel->submitAll())
    {
        msg = zv_chemicalTableSQLModel->lastError().text();
        zv_chemicalTableSQLModel->revertAll();
        return false;
    }

    return true;
}
//===============================================================
bool ZCalibrationListDialog::zh_writeMeasuringConditionsToDatabase(int gainFactor, int exposition, int &measurementConditionId, QString& msg)
{
    int row;
    if(zh_checkMeasuringConditionsExistance(gainFactor, exposition, measurementConditionId, row))
    {
        return true;
    }

    // write gain factor
    if(!zh_checkGainFactorExistance(gainFactor, msg))
    {
        return false;
    }

    // define new measurement id
    if(!zh_findNewMeasurementConditionsId(measurementConditionId, msg))
    {
        return false;
    }

    QSqlRecord record;
    record.append(QSqlField("id", QVariant::Int));
    record.append(QSqlField("gain_factor", QVariant::Int));
    record.append(QSqlField("exposition", QVariant::Int));

    record.setValue(0, QVariant(measurementConditionId));
    record.setValue(1, QVariant(gainFactor));
    record.setValue(2, QVariant(exposition));

    if(!zv_measuringConditionsSQLTableModel->insertRecord(-1, record))
    {
        msg = zv_measuringConditionsSQLTableModel->lastError().text();
        return false;
    }

    if(!zv_measuringConditionsSQLTableModel->submitAll())
    {
        msg = zv_chemicalTableSQLModel->lastError().text();
        zv_measuringConditionsSQLTableModel->revertAll();
        return false;
    }

    return true;
}
//===============================================================
int ZCalibrationListDialog::zh_findNewChemicalId() const
{
    // find new chemical id
    int newChemicalId = 0;
    int currentChemicalId;
    QModelIndex index;
    QVariant vData;
    bool ok;
    for(int row = 0; row < zv_chemicalTableSQLModel->rowCount(); row++)
    {
        index = zv_chemicalTableSQLModel->index(row, 0);
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
    return ++newChemicalId;
}
//===============================================================
bool ZCalibrationListDialog::zh_findNewMeasurementConditionsId(int& newId, QString& msg) const
{
    QSqlQuery query;
    QString queryString = "SELECT MAX(id) FROM measuring_conditions";
    if(!query.prepare(queryString))
    {
        msg = query.lastError().text();
        return false;
    }
    if(!query.exec())
    {
        msg = query.lastError().text();
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
        msg = tr("Cannot convert measurement conditions id from QVariant to Int.");
        return false;
    }

    newId = vData.toInt() + 1;

    return true;
}
//===============================================================
int ZCalibrationListDialog::zh_findNewCalibrationlId(QString& msg) const
{
    // New Calibration model
    QSqlQuery query;
    QString queryString = "SELECT id FROM calibrations";

    if(!query.prepare(queryString))
    {
        msg = query.lastError().text();
        return -1;
    }

    if(!query.exec())
    {
        msg = query.lastError().text();
        return -1;
    }

    int maxId = 0;
    int currentId;
    QVariant vData;
    bool ok;
    while(query.next())
    {
        vData = query.value(0);
        if(!vData.isValid() || !vData.canConvert<int>())
        {
            continue;
        }

        currentId = vData.toInt(&ok);
        if(!ok)
        {
            continue;
        }

        if(maxId < currentId)
        {
            maxId = currentId;
        }
    }

    return  ++maxId;
}
//===============================================================
bool ZCalibrationListDialog::zh_checkChemicalExistance(const QString& chemical, int& chemicalId) const
{
    QModelIndex index;
    QVariant vData;
    for(int row = 0; row < zv_chemicalTableSQLModel->rowCount(); row++)
    {
        index = zv_chemicalTableSQLModel->index(row, 1);
        if(!index.isValid())
        {
            continue;
        }
        vData = index.data(Qt::DisplayRole);
        if(!vData.isValid() || !vData.canConvert<QString>())
        {
            continue;
        }

        if(vData.toString() ==  chemical)
        {
            // get chemical id
            index = zv_chemicalTableSQLModel->index(row, 0);
            if(!index.isValid())
            {
                return false;
            }
            vData = index.data(Qt::DisplayRole);
            if(!vData.isValid() || !vData.canConvert<int>())
            {
                return false;;
            }

            bool ok;
            chemicalId = vData.toInt(&ok);
            if(!ok)
            {
                return false;
            }
            return true;
        }
    }

    return false;
}
//===============================================================
bool ZCalibrationListDialog::zh_checkMeasuringConditionsExistance(int gainFactor, int exposition, int &modelRow) const
{
    QModelIndex index;
    QVariant vData;
    bool ok;
    int currentGainFactor;
    int currentExposition;
    for(int row = 0; row < zv_measuringConditionsSQLTableModel->rowCount(); row++)
    {
        // gain factor
        index = zv_measuringConditionsSQLTableModel->index(row, 1);
        if(!index.isValid())
        {
            continue;;
        }
        vData = index.data(Qt::DisplayRole);
        if(!vData.isValid() || !vData.canConvert<int>())
        {
            continue;
        }
        currentGainFactor = vData.toInt(&ok);
        if(!ok)
        {
            continue;
        }

        // exposition
        index = zv_measuringConditionsSQLTableModel->index(row, 2);
        if(!index.isValid())
        {
            continue;;
        }
        vData = index.data(Qt::DisplayRole);
        if(!vData.isValid() || !vData.canConvert<int>())
        {
            continue;
        }
        currentExposition = vData.toInt(&ok);
        if(!ok)
        {
            continue;
        }

        if(gainFactor == currentGainFactor && exposition == currentExposition)
        {
            modelRow = row;
            return true;
        }
    }

    return false;
}
//===============================================================
bool ZCalibrationListDialog::zh_checkMeasuringConditionsExistance(int gainFactor, int exposition, int& measurementConditionsId,  int& modelRow) const
{
    if(zh_checkMeasuringConditionsExistance(gainFactor, exposition, modelRow) )
    {
        // measurementConditionsId
        QModelIndex index = zv_measuringConditionsSQLTableModel->index(modelRow, 0);
        if(!index.isValid())
        {
            return false;
        }
        QVariant vData = index.data(Qt::DisplayRole);
        if(!vData.isValid() || !vData.canConvert<int>())
        {
            return false;
        }

        bool ok;
        measurementConditionsId = vData.toInt(&ok);
        if(!ok)
        {
            return false;
        }

        return true;
    }

    return false;
}
//===============================================================
bool ZCalibrationListDialog::zh_checkGainFactorExistance(int gainFactor, QString& msg) const
{
    QSqlQuery query;
    QString queryString = QString("SELECT * FROM gain_factors "
                                  "WHERE gain_factor=%1").arg(QString::number(gainFactor));

    if(!query.prepare(queryString))
    {
        msg = query.lastError().text();
        return -1;
    }

    if(!query.exec())
    {
        msg = query.lastError().text();
        return -1;
    }

    if(query.next())
    {
        return true;
    }

    query.clear();
    queryString = QString("INSERT INTO gain_factors (gain_factor, energyFactorK0, energyFactorK1, energyFactorK2) "
                          "VALUES (:gain_factor, :energyFactorK0, :energyFactorK1, :energyFactorK2)");

    if(!query.prepare(queryString))
    {
        msg = query.lastError().text();
        return -1;
    }

    query.bindValue(":gain_factor", gainFactor);
    query.bindValue(":energyFactorK0", 0.0);
    query.bindValue(":energyFactorK1", 0.0);
    query.bindValue(":energyFactorK2", 0.0);

    if(!query.exec())
    {
        msg = query.lastError().text();
        return false;
    }

    return true;
}
//===============================================================
void ZCalibrationListDialog::zh_removeSelectedCalibration()
{

}
//===============================================================
void ZCalibrationListDialog::zh_onCalibrationInsertion(const QModelIndex& parent, int first, int last)
{
    // zh_repopulateMeasuringConditionsModel();
    QHeaderView* horizontalHeader = zv_calibrationTableWidget->zp_tableView()->horizontalHeader();
    horizontalHeader->resizeSections(QHeaderView::ResizeToContents);

}
//===============================================================
void ZCalibrationListDialog::zh_onCalibrationRemoving(const QModelIndex& parent, int first, int last)
{
    // zh_repopulateMeasuringConditionsModel();
}
//===============================================================
void ZCalibrationListDialog::zh_restoreSettings()
{
    QSettings settings;
    QVariant vData;
    settings.beginGroup(glAppVersion);
    settings.beginGroup("CalibrationListDialog");

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

    vData = settings.value("calibrationFolderPath");
    if(vData.isValid() && !vData.isNull() && vData.canConvert<QString>())
    {
        zv_calibrationFolderPath = vData.toString();
    }
    else
    {
        QStringList standardPathList = QStandardPaths::standardLocations(QStandardPaths::DocumentsLocation);
        if(!standardPathList.isEmpty())
        {
            zv_calibrationFolderPath = standardPathList.value(0);
        }
        else
        {
            zv_calibrationFolderPath = QDir::currentPath();
        }
    }

    settings.endGroup();
    settings.endGroup();
}
//===============================================================
void ZCalibrationListDialog::zh_saveSettings() const
{
    QSettings settings;
    settings.beginGroup(glAppVersion);
    settings.beginGroup("CalibrationListDialog");

    settings.setValue("dialogGeometry", QVariant::fromValue<QByteArray>(this->saveGeometry()));
    //settings.setValue("splitterGeometry", QVariant::fromValue<QByteArray>(zv_tableSplitter->saveState()));
    settings.setValue("mainSplitterState", QVariant::fromValue<QByteArray>(zv_mainSplitter->saveState()));
    settings.setValue("calibrationFolderPath", QVariant(zv_calibrationFolderPath));

    settings.endGroup();
    settings.endGroup();
}
//===============================================================
