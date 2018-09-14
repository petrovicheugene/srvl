//=========================================================
#include "ZGeneral.h"
#include "ZStartDialog.h"
#include "ZDatabasePropertiesDialog.h"
#include "ZDatabaseInspector.h"

#include <QApplication>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QDialogButtonBox>

#include <QPushButton>
#include <QListWidget>
#include <QItemSelectionModel>
#include <QHeaderView>
#include <QTableWidgetItem>
#include <QFrame>
#include <QLabel>
#include <QCheckBox>

#include <QMessageBox>
#include <QFileInfo>
#include <QDir>
#include <QSqlDatabase>
#include <QSettings>
//=========================================================
ZStartDialog::ZStartDialog(QWidget *parent) : QDialog(parent)
{
    setWindowTitle(qApp->property("glAppProduct").toString());
    setWindowFlags(Qt::WindowStaysOnTopHint | Qt::Tool);

    zv_editedRow = -1;
    zh_createComponents();
    zh_createConnections();
    zh_loadDatabaseList();
    zv_dbListWidget->setCurrentRow(0);
    zh_interfaceEnablingControl(0);
}
//=========================================================
void ZStartDialog::zh_createComponents()
{
    // layouts
    QVBoxLayout* mainLayout = new QVBoxLayout(this);
    setLayout(mainLayout);

    QHBoxLayout* topLayout = new QHBoxLayout(this);
    mainLayout->addLayout(topLayout);

    // sparator line
    QFrame* line = new QFrame(this);
    line->setFrameStyle(QFrame::HLine | QFrame::Sunken);
    line->setLineWidth(1);
    mainLayout->addWidget(line);

    // button boxes
    QDialogButtonBox* topRightButtonBox = new QDialogButtonBox(Qt::Vertical, this);
    topLayout->addWidget(topRightButtonBox);

    QDialogButtonBox* bottomButtonBox = new QDialogButtonBox(Qt::Horizontal, this);
    mainLayout->addWidget(bottomButtonBox);

    // components
    zv_dbListWidget = new QListWidget(this);
    //   zv_dbListWidget->setColumnCount(2);
    //   zv_dbListWidget->horizontalHeader()->setVisible(false);
    //   zv_dbListWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
    //zv_dbListWidget->setSelectionBehavior(QAbstractItemView::SelectRows);
    topLayout->insertWidget(0, zv_dbListWidget);

    zv_currentPathLabel = new QLabel(this);
    zv_currentPathLabel->setWordWrap(true);
    mainLayout->insertWidget(1, zv_currentPathLabel);

    zv_newButton = new QPushButton(this);
    zv_newButton->setText(tr("New"));
    topRightButtonBox->addButton(zv_newButton, QDialogButtonBox::ActionRole);

    zv_editButton = new QPushButton(this);
    zv_editButton->setText(tr("Edit"));
    zv_editButton->setDisabled(true);
    topRightButtonBox->addButton(zv_editButton, QDialogButtonBox::ActionRole);

    zv_removeButton = new QPushButton(this);
    zv_removeButton->setText(tr("Remove"));
    zv_removeButton->setDisabled(true);
    topRightButtonBox->addButton(zv_removeButton, QDialogButtonBox::ActionRole);

    zv_connectButton = new QPushButton(this);
    zv_connectButton->setText(tr("Connect"));
    zv_connectButton->setDisabled(true);
    topRightButtonBox->addButton(zv_connectButton, QDialogButtonBox::ActionRole);

    // bottom
    zv_cancelButton = new QPushButton(this);
    zv_cancelButton->setText(tr("Cancel"));
    bottomButtonBox->addButton(zv_cancelButton, QDialogButtonBox::ActionRole);

    zv_newButton->setFocus();
}
//=========================================================
void ZStartDialog::zh_createConnections()
{
    // buttons
    connect(zv_newButton, &QPushButton::clicked,
            this, &ZStartDialog::zh_onNewDatabase);
    connect(zv_editButton, &QPushButton::clicked,
            this, &ZStartDialog::zh_onEditDatabase);
    connect(zv_removeButton, &QPushButton::clicked,
            this, &ZStartDialog::zh_onRemoveDatabase);
    connect(zv_connectButton, &QPushButton::clicked,
            this, &ZStartDialog::zh_onConnectToDatabase);
    connect(zv_cancelButton, &QPushButton::clicked,
            this, &ZStartDialog::reject);

    // selection model signals
    connect(zv_dbListWidget->selectionModel(), &QItemSelectionModel::currentRowChanged,
            this, &ZStartDialog::zh_onCurrentRowChange);
    connect(zv_dbListWidget->selectionModel(), &QItemSelectionModel::selectionChanged,
            this, &ZStartDialog::zh_onSelectionChange);
}
//=========================================================
void ZStartDialog::zp_databaseProperties(QString& name, QString& path) const
{
    name = zv_dbName;
    path = zv_dbPath;
}
//=========================================================
void ZStartDialog::zh_onCurrentRowChange(const QModelIndex& current, const QModelIndex& previous)
{
    zh_interfaceEnablingControl(current.row());
}
//=========================================================
void ZStartDialog::zh_interfaceEnablingControl(int currentRow)
{
    // buttons
    bool disabled = currentRow < 0 || currentRow >= zv_dbListWidget->count();
    zv_editButton->setDisabled(disabled);
    zv_connectButton->setDisabled(disabled);

    // set current path label text
    QString path;
    QString comment;
    if(!disabled)
    {
        QListWidgetItem* item = zv_dbListWidget->item(currentRow);
        if(item)
        {
            QVariant vData =  item->data(NS_DataRole::PathRole);
            if(vData.isValid() && !vData.isNull() && vData.canConvert<QString>())
            {
                path = vData.toString();
            }
            vData =  item->data(NS_DataRole::CommentRole);
            if(vData.isValid() && !vData.isNull() && vData.canConvert<QString>())
            {
                comment = vData.toString();
            }
        }
    }

    QString pathLabelString = QString("<b>%1<></b>").arg(path);
    if(!comment.isEmpty())
    {
        pathLabelString += QString("<b> - %1<></b>").arg(comment);
        pathLabelString = "<font color=red>" + pathLabelString + "</font>";
    }

    zv_currentPathLabel->setText(pathLabelString);
}
//=========================================================
void ZStartDialog::zh_onSelectionChange(const QItemSelection &selected,
                                        const QItemSelection &deselected)
{
    QList<QListWidgetItem*>selectedItemList = zv_dbListWidget->selectedItems();
    zv_removeButton->setDisabled(selectedItemList.isEmpty());
}
//=========================================================
void ZStartDialog::zh_checkAndStoreDatabase(const QString &name,
                                            const QString& path,
                                            bool &res)
{
    // the slot is called from dialog ZDatabasePropertiesDialog
    // in order to check database properties and storing the database to list

    // check emptyness
    QString msg;
    if(!zh_nameAndPathEmptynessCheck(name, path, msg))
    {
        QMessageBox::critical(this, NS_CommonStrings::glError, msg, QMessageBox::Ok);
        res = false;
        return;
    }

    // append suffix if necessary
    QString newPath = zh_suffixCheck(path);

    // check db record existance
    QList<int> samePathRowList;
    if(!zh_existanceOfRecordCheck(name, newPath, msg, samePathRowList))
    {
        QMessageBox::critical(this, NS_CommonStrings::glError, msg, QMessageBox::Ok);
        res = false;
        return;
    }
    else if(!samePathRowList.isEmpty())
    {
        msg = msg + "\nDo you want to create a new record anyway?";
        if(QMessageBox::warning(this, NS_CommonStrings::glWarning, msg, QMessageBox::Yes | QMessageBox::No) == QMessageBox::No)
        {
            res = false;
            return;
        }
    }

    if(!ZDatabaseInspector::zp_inspectNewDataBase(name, newPath, msg))
    {
        QMessageBox::critical(this, NS_CommonStrings::glError, msg, QMessageBox::Ok);
        res = false;
        return;
    }

    // Store the db record to tableWidget
    QString comment; // checking result string

    // record gotten data
    int row = zh_recordRow(name, newPath, comment, zv_editedRow);

    // set edited row as current and manage UI
    if(row >= 0 && row < zv_dbListWidget->count())
    {
        zv_dbListWidget->setCurrentRow(row);
        zh_interfaceEnablingControl(row);
    }

    // reset zv_editedRow
    zv_editedRow = -1;

    res = true;
    zh_saveDatabaseList();
}
//=========================================================
void ZStartDialog::zh_loadDatabaseList()
{
    QSettings settings;
    settings.beginGroup(glAppVersion);
    int size = settings.beginReadArray(zv_databaseArrayCapture);
    QString name;
    QString path;
    QString msg;

    for (int row = 0; row < size; row++)
    {
        settings.setArrayIndex(row);
        name = settings.value(zv_databaseNameValueCaption).toString();
        path = settings.value(zv_databasePathValueCaption).toString();
        if(ZDatabaseInspector::zp_inspectExistingDataBase(name, path, msg))
        {
            msg = QString();
        }
        zh_recordRow(name, path, msg, -1);
    }

    settings.endArray();
    settings.endGroup();
}
//=========================================================
void ZStartDialog::zh_saveDatabaseList() const
{
    QSettings settings;
    settings.beginGroup(glAppVersion);

    settings.beginWriteArray(zv_databaseArrayCapture);
    QString name;
    QString path;
    QVariant vData;

    for(int row = 0; row < zv_dbListWidget->count(); row++)
    {
        // get name
        name = zv_dbListWidget->item(row)->text();

        // get path
        vData = zv_dbListWidget->item(row)->data(NS_DataRole::PathRole);
        if(vData.isValid() && !vData.isNull() && vData.canConvert<QString>())
        {
            path = vData.toString();
        }
        else
        {
            path = QString();
        }

        // write to the settings
        settings.setArrayIndex(row);
        settings.setValue(zv_databaseNameValueCaption, name);
        settings.setValue(zv_databasePathValueCaption, path);
    }

    settings.endArray();
    settings.endGroup();
}
//=========================================================
int ZStartDialog::zh_recordRow(const QString& name, const QString& path,
                               const QString& comment, int row)
{
    QListWidgetItem* item;
    // check row and create row if necessary
    if(row < 0 || row > zv_dbListWidget->count())
    {
        row = zv_dbListWidget->count();
        //zv_dbListWidget->insertRow(row);
        item = new QListWidgetItem();
        zv_dbListWidget->insertItem(row, item);
    }
    else
    {
        item = zv_dbListWidget->item(row);
    }

    // name
    // QListWidgetItem* item = new QListWidgetItem();
    item->setFlags(item->flags() &  ~Qt::ItemIsEditable);
    item->setText(name);

    // path and comment
    item->setData(NS_DataRole::PathRole, QVariant(path));
    item->setData(NS_DataRole::CommentRole, QVariant(comment));

    return row;
}
//=========================================================
void ZStartDialog::zh_editAndRecordDatabase(QString name, QString path)
{
    // run dialog
    ZDatabasePropertiesDialog dialog(name, path, this);
    connect(&dialog, &ZDatabasePropertiesDialog::zg_inquiryRecordDatabase,
            this, &ZStartDialog::zh_checkAndStoreDatabase);
    // dialog closes itself after database is checked / created and stored to the list
    dialog.exec();
}
//=========================================================
bool ZStartDialog::zh_nameAndPathEmptynessCheck(const QString &name,
                                                const QString& path, QString& msg) const
{
    bool error = false;
    // name check
    if(name.isEmpty())
    {
        msg = tr("The name");
        error = true;
    }

    // path check
    if(path.isEmpty())
    {
        if(error)
        {
            msg = msg + " " + tr("and the path of database cannot be empty!");
        }
        else
        {
            msg = tr("The path of database cannot be empty!");
            error = true;
        }
    }
    else if(error)
    {
        msg = msg + " " + tr("of database cannot be empty!");
    }

    return !error;
}
//=========================================================
QString ZStartDialog::zh_suffixCheck(const QString& path) const
{
    QString newPath = path;
    QFileInfo fileInfo(path);

    if(fileInfo.absolutePath() !=  fileInfo.absoluteFilePath() && fileInfo.suffix().isEmpty())
    {
        newPath += "." + glDefaultDBSuffixString;
    }

    return newPath;
}
//=========================================================
bool ZStartDialog::zh_existanceOfRecordCheck(const QString &name,
                                             const QString& path, QString& msg, QList<int>& samePathRowList) const
{
    QString currentName;
    QString currentPath;
    QVariant vData;

    for(int row = 0; row < zv_dbListWidget->count(); row++)
    {
        if(row == zv_editedRow)
        {
            // dont compare record to it self
            continue;
        }

        // name check
        currentName = zv_dbListWidget->item(row)->text();
        if(name == currentName)
        {
            msg = tr("Database name \"%1\" is already exist! The name of database must be unique!").arg(name);
            return false;
        }

        // path check
        vData = zv_dbListWidget->item(row)->data(NS_DataRole::PathRole);
        if(vData.isValid() && !vData.isNull() && vData.canConvert<QString>())
        {
            currentPath = vData.toString();
        }
        else
        {
            currentPath = QString();
        }

        if(path.toLower() == currentPath.toLower())
        {
            msg = tr("Database path \"%1\" is already exist!").arg(path);
            samePathRowList.append(row);
        }
    }

    return true;
}
//=========================================================
void ZStartDialog::zh_onNewDatabase()
{
    // get database default name
    QString currentName;
    QString nameTailString;
    int tailLenght = 0;
    int currentTailNumber = 0;
    int maxTailNumber = 0;
    bool ok = false;
    for(int row = 0; row < zv_dbListWidget->count(); row++)
    {
        currentName = zv_dbListWidget->item(row)->text();
        if(!currentName.startsWith(zv_defaultDatabaseName))
        {
            continue;
        }

        tailLenght = currentName.count() - zv_defaultDatabaseName.count();
        nameTailString = currentName.right(tailLenght);
        currentTailNumber = nameTailString.toInt(&ok);
        if(ok && maxTailNumber < currentTailNumber)
        {
            maxTailNumber = currentTailNumber;
        }
    }

    QString name = zv_defaultDatabaseName + QString::number(++maxTailNumber);

    // run db properties dialog, check and record data
    QString path;
    zv_editedRow = -1;
    zh_editAndRecordDatabase(name, path);
}
//=========================================================
void ZStartDialog::zh_onEditDatabase()
{
    // get edited row
    zv_editedRow = zv_dbListWidget->currentRow();
    if(zv_editedRow < 0 || zv_editedRow >= zv_dbListWidget->count())
    {
        zv_editButton->setDisabled(true);
        return;
    }

    // get edited data
    QString name = zv_dbListWidget->item(zv_editedRow)->text();
    QString path;
    QVariant vData = zv_dbListWidget->item(zv_editedRow)->data(NS_DataRole::PathRole);
    if(vData.isValid() && !vData.isNull() && vData.canConvert<QString>())
    {
        path = vData.toString();
    }

    zh_editAndRecordDatabase(name, path);
}
//=========================================================
void ZStartDialog::zh_onRemoveDatabase()
{
    // get selection
    QList<QListWidgetItem*>selectedItemList = zv_dbListWidget->selectedItems();
    if(selectedItemList.isEmpty())
    {
        zv_removeButton->setDisabled(true);
        return;
    }

    // run remove confirm dialog
    QMessageBox messageBox;
    QString checkBoxString = tr("Remove the selected databases physically.\n"
                                "This operation is not reversible!");
    QCheckBox checkBox(checkBoxString);
    messageBox.setCheckBox(&checkBox);
    messageBox.setWindowTitle(tr("Remove database"));
    messageBox.setText(tr("Do you want to remove the selected databases from the list?"));
    messageBox.setIcon(QMessageBox::Warning);
    messageBox.setStandardButtons(QMessageBox::Ok | QMessageBox::Cancel);

    if(messageBox.exec() == QMessageBox::Cancel)
    {
        return;
    }

    // get selected rows from selection
    QList<int> selectedRowList;
    //   QListWidgetItem* pathItem;
    QVariant vData;
    QString currentDBPath;
    QList<int> dublicateRecordList;

    foreach(QListWidgetItem* item, selectedItemList)
    {
        if(!selectedRowList.contains(zv_dbListWidget->row(item)))
        {
            selectedRowList.append(zv_dbListWidget->row(item));
        }

        // find records with the same database
        //      pathItem = zv_dbListWidget->item(item->row());
        //      if(!pathItem)
        //      {
        //         continue;
        //      }

        vData = item->data(NS_DataRole::PathRole);
        if(!vData.isValid() || vData.isNull() || !vData.canConvert<QString>())
        {
            continue;
        }

        currentDBPath = vData.toString();
        zh_findDublicateRows(currentDBPath, dublicateRecordList);
    }

    qSort(selectedRowList);
    qSort(dublicateRecordList);

    bool deleteFiles = messageBox.checkBox()->isChecked();

    if(deleteFiles && selectedRowList != dublicateRecordList)
    {
        // there are some records refer to deleted files
        QString msg = tr("There are other records referring to the files "
                         "going to be deleted and which will be removed from the list!!"
                         "\nDo you want to continue?");
        if(QMessageBox::question(this, NS_CommonStrings::glWarning, msg, QMessageBox::Yes | QMessageBox::No) ==
                QMessageBox::No)
        {
            return;
        }

        // yes, remove records
        // add rows into list of row to delete
        foreach(int row, dublicateRecordList)
        {
            if(!selectedRowList.contains(row))
            {
                selectedRowList.append(row);
            }
        }
        qSort(selectedRowList);
    }

    QString filePath;
    // remove all selected rows from the end
    for(int i = selectedRowList.count()-1; i >= 0; i--)
    {
        // delete files in nesessary
        if(deleteFiles)
        {
            QListWidgetItem* item = zv_dbListWidget->item(selectedRowList.value(i));
            if(item)
            {
                vData = item->data(NS_DataRole::PathRole);
                if(vData.isValid() && !vData.isNull() && vData.canConvert<QString>())
                {
                    filePath = vData.toString();
                    QFile file(filePath);
                    file.remove();
                }
            }
        }

        QListWidgetItem* item = zv_dbListWidget->takeItem(selectedRowList.value(i));
        if(item)
        {
            delete item;
        }
    }

    zh_saveDatabaseList();
}
//=========================================================
void ZStartDialog::zh_onConnectToDatabase()
{
    QString name;
    QString path;

    QListWidgetItem* item = zv_dbListWidget->currentItem();
    if(item)
    {
        name = item->text();
        QVariant vData = item->data(NS_DataRole::PathRole);
        if(vData.isValid() && !vData.isNull() && vData.canConvert<QString>())
        {
            path = vData.toString();
        }
    }

    QString msg;
    if(!ZDatabaseInspector::zp_inspectExistingDataBase(name, path, msg))
    {
        QMessageBox::critical(this, NS_CommonStrings::glError, msg, QMessageBox::Ok);
        return;
    }


    zv_dbName = name;
    zv_dbPath = path;
    accept();
}
//=========================================================
void ZStartDialog::zh_findDublicateRows(const QString& dbPath, QList<int>&rowList)
{
    // search database table for another records contains physically deleted file
    QListWidgetItem* item;
    QVariant vData;
    QString currentDBPath;
    for(int row = 0; row < zv_dbListWidget->count(); row++)
    {
        item = zv_dbListWidget->item(row);
        if(!item)
        {
            continue;
        }

        vData = item->data(NS_DataRole::PathRole);
        if(!vData.isValid() || vData.isNull() || !vData.canConvert<QString>())
        {
            continue;
        }
        currentDBPath = vData.toString();

        if(currentDBPath.toLower() == dbPath.toLower() && !rowList.contains(row))
        {
            rowList.append(row);
        }
    }
}
//=========================================================
