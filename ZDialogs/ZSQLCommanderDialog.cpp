//=====================================================
#include "ZSQLCommanderDialog.h"
#include "ZGLConstantsAndDefines.h"

#include <QApplication>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QDialogButtonBox>
#include <QEvent>
#include <QFrame>
#include <QHeaderView>
#include <QLabel>
#include <QListWidget>
#include <QMouseEvent>
#include <QPushButton>
#include <QSplitter>
#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlTableModel>
#include <QSqlQuery>
#include <QSqlRecord>
#include <QSettings>
#include <QSplitter>
#include <QTableView>
#include <QTextEdit>
#include <QTableWidget>
//=====================================================
ZSQLCommanderDialog::ZSQLCommanderDialog(QWidget *parent) : QDialog(parent)
{
    zh_createComponents();
    zh_createConnections();
    zh_loadDatabaseTableList();
    zh_restoreSettings();
}
//=====================================================
void ZSQLCommanderDialog::closeEvent(QCloseEvent* event)
{
    zh_saveSettings();
}
//=====================================================
//bool ZSQLCommanderDialog::eventFilter(QObject* object, QEvent* event)
//{
//    if(event->type() == QEvent::MouseButtonRelease)
//    {
//        QMouseEvent* mouseEvent = static_cast<QMouseEvent*>(event);
//        if(mouseEvent->modifiers() & Qt::ControlModifier)
//        {
//            if(object == zv_dataTableView->horizontalHeader()->viewport())
//            {
//                zv_dataTableView->setSortingEnabled(!zv_dataTableView->isSortingEnabled());
//            }
//            else if(object == zv_selectedDataTableWidget->horizontalHeader()->viewport())
//            {
//                zv_selectedDataTableWidget->setSortingEnabled(!zv_selectedDataTableWidget->isSortingEnabled());
//            }
//        }
//    }

//    return QDialog::eventFilter(object, event);
//}
//=====================================================
void ZSQLCommanderDialog::zh_createComponents()
{
    // model
    zv_dataModel = new QSqlTableModel(this);
    zv_dataModel->setEditStrategy(QSqlTableModel::OnManualSubmit);

    QVBoxLayout* mainLayout = new QVBoxLayout(this);
    setLayout(mainLayout);

    zv_mainSplitter = new QSplitter(Qt::Vertical, this);
    mainLayout->addWidget(zv_mainSplitter);

    // Database Table And Data
    zv_topSplitter = new QSplitter(this);
    zv_mainSplitter->addWidget(zv_topSplitter);

    // table list
    zv_topSplitter->addWidget(zh_createTableListWidget());
    // data table
    zv_topSplitter->addWidget(zh_createDataTableWidget());

    //
    zv_bottomSplitter = new QSplitter(this);
    zv_mainSplitter->addWidget(zv_bottomSplitter);

    // command textEdit
    zv_bottomSplitter->addWidget(zh_createQueryWidget());
    // selection widget
    zv_bottomSplitter->addWidget(zh_createSelectedDataWidget());
    // message textEdit
    zv_bottomSplitter->addWidget(zh_createMessageWidget());


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
    //    zv_runCommanButton = new QPushButton(this);
    //    zv_runCommanButton->setText(tr("Run!"));
    //    buttonBox->addButton(zv_runCommanButton, QDialogButtonBox::ActionRole);

    zv_closeButton = new QPushButton(this);
    zv_closeButton->setText(tr("Close"));
    buttonBox->addButton(zv_closeButton, QDialogButtonBox::ActionRole);

    //zv_selectedDataTableWidget->horizontalHeader()->viewport()->installEventFilter(this);
}
//=====================================================
void ZSQLCommanderDialog::zh_createConnections()
{
    zv_dataTableView->setModel(zv_dataModel);
    //zv_dataTableView->horizontalHeader()->viewport()->installEventFilter(this);

    connect(zv_tableListWidget, &QListWidget::currentRowChanged,
            this, &ZSQLCommanderDialog::zh_onCurrentTableChange);
    connect(zv_updateTableListButton, &QPushButton::clicked,
            this, &ZSQLCommanderDialog::zh_loadDatabaseTableList);

    connect(zv_updateDataTableButton, &QPushButton::clicked,
            this, &ZSQLCommanderDialog::zh_updateDataTable);
    connect(zv_sortDataTableButton, &QPushButton::toggled,
            this, &ZSQLCommanderDialog::zh_setSortingDataTable);

    connect(zv_newRowButton, &QPushButton::clicked,
            this, &ZSQLCommanderDialog::zh_addRow);
    connect(zv_removeRowsButton, &QPushButton::clicked,
            this, &ZSQLCommanderDialog::zh_removeRows);
    connect(zv_commitButton, &QPushButton::clicked,
            this, &ZSQLCommanderDialog::zh_commitData);
    connect(zv_clearQueryButton, &QPushButton::clicked,
            zv_queryTextEdit, &QTextEdit::clear);
    connect(zv_execQueryButton, &QPushButton::clicked,
            this, &ZSQLCommanderDialog::zh_executeQuery);

    connect(zv_sortSelectedDataTableButton, &QPushButton::toggled,
            this, &ZSQLCommanderDialog::zh_setSortingSelectedDataWidget);
    connect(zv_clearSelectedDataTableButton, &QPushButton::clicked,
            this, &ZSQLCommanderDialog::zh_clearSelectedDataWidget);

    connect(zv_clearMessagesButton, &QPushButton::clicked,
            zv_messageTextEdit, &QTextEdit::clear);
    connect(zv_closeButton, &QPushButton::clicked,
            this, &ZSQLCommanderDialog::close);

}
//=====================================================
void ZSQLCommanderDialog::zh_setSortingDataTable(bool sort)
{
    zv_dataTableView->setSortingEnabled(sort);
    if(!sort)
    {
        zv_updateDataTableButton->click();
    }
}
//=====================================================
void ZSQLCommanderDialog::zh_setSortingSelectedDataWidget(bool sort)
{
    zv_selectedDataTableWidget->setSortingEnabled(sort);
}
//=====================================================
QWidget* ZSQLCommanderDialog::zh_createTableListWidget()
{
    QWidget* w = new QWidget(this);
    QVBoxLayout* mainLayout = new QVBoxLayout(this);
    w->setLayout(mainLayout);

    QLabel* label = new QLabel(this);
    label->setText(glCreateCaption("Tables"));
    mainLayout->addWidget(label);

    zv_tableListWidget = new QListWidget(this);
    mainLayout->addWidget(zv_tableListWidget);

    QDialogButtonBox* buttonBox = new QDialogButtonBox(this);
    mainLayout->addWidget(buttonBox);
    zv_updateTableListButton = new QPushButton(this);
    zv_updateTableListButton->setText("Update");
    buttonBox->addButton(zv_updateTableListButton, QDialogButtonBox::ActionRole);

    return w;
}
//=====================================================
QWidget* ZSQLCommanderDialog::zh_createDataTableWidget()
{
    QWidget* w = new QWidget(this);
    QVBoxLayout* mainLayout = new QVBoxLayout(this);
    w->setLayout(mainLayout);

    QLabel* label = new QLabel(this);
    label->setText(glCreateCaption("Data"));
    mainLayout->addWidget(label);

    zv_dataTableView = new QTableView(this);
    mainLayout->addWidget(zv_dataTableView);

    QDialogButtonBox* buttonBox = new QDialogButtonBox(this);
    mainLayout->addWidget(buttonBox);

    zv_sortDataTableButton = new QPushButton(this);
    zv_sortDataTableButton->setText("Sort");
    zv_sortDataTableButton->setCheckable(true);
    buttonBox->addButton(zv_sortDataTableButton, QDialogButtonBox::ActionRole);

    zv_updateDataTableButton = new QPushButton(this);
    zv_updateDataTableButton->setText("Update");
    buttonBox->addButton(zv_updateDataTableButton, QDialogButtonBox::ActionRole);

    zv_newRowButton = new QPushButton(this);
    zv_newRowButton->setText("Add");
    buttonBox->addButton(zv_newRowButton, QDialogButtonBox::ActionRole);

    zv_removeRowsButton  = new QPushButton(this);
    zv_removeRowsButton->setText("Remove");
    buttonBox->addButton(zv_removeRowsButton, QDialogButtonBox::ActionRole);

    zv_commitButton  = new QPushButton(this);
    zv_commitButton->setText("Commit");
    buttonBox->addButton(zv_commitButton, QDialogButtonBox::ActionRole);

    return w;
}
//=====================================================
QWidget* ZSQLCommanderDialog::zh_createQueryWidget()
{
    QWidget* w = new QWidget(this);
    QVBoxLayout* mainLayout = new QVBoxLayout(this);
    w->setLayout(mainLayout);

    QLabel* label = new QLabel(this);
    label->setText(glCreateCaption("SQL query"));
    mainLayout->addWidget(label);

    zv_queryTextEdit = new QTextEdit(this);
    mainLayout->addWidget(zv_queryTextEdit);

    QDialogButtonBox* buttonBox = new QDialogButtonBox(this);
    mainLayout->addWidget(buttonBox);
    zv_execQueryButton = new QPushButton(this);
    zv_execQueryButton->setText("Execute!");
    buttonBox->addButton(zv_execQueryButton, QDialogButtonBox::ActionRole);

    zv_clearQueryButton = new QPushButton(this);
    zv_clearQueryButton->setText("Clear");
    buttonBox->addButton(zv_clearQueryButton, QDialogButtonBox::ActionRole);

    return w;
}
//=====================================================
QWidget* ZSQLCommanderDialog::zh_createSelectedDataWidget()
{
    QWidget* w = new QWidget(this);
    QVBoxLayout* mainLayout = new QVBoxLayout(this);
    w->setLayout(mainLayout);

    QLabel* label = new QLabel(this);
    label->setText(glCreateCaption("Selection table"));
    mainLayout->addWidget(label);

    zv_selectedDataTableWidget = new QTableWidget(this);

    mainLayout->addWidget(zv_selectedDataTableWidget);

    QDialogButtonBox* buttonBox = new QDialogButtonBox(this);
    mainLayout->addWidget(buttonBox);

    zv_sortSelectedDataTableButton = new QPushButton(this);
    zv_sortSelectedDataTableButton->setText("Sort");
    zv_sortSelectedDataTableButton->setCheckable(true);
    buttonBox->addButton(zv_sortSelectedDataTableButton, QDialogButtonBox::ActionRole);

    zv_clearSelectedDataTableButton = new QPushButton(this);
    zv_clearSelectedDataTableButton->setText("Clear");
    buttonBox->addButton(zv_clearSelectedDataTableButton, QDialogButtonBox::ActionRole);

    return w;
}
//=====================================================
QWidget* ZSQLCommanderDialog::zh_createMessageWidget()
{
    QWidget* w = new QWidget(this);
    QVBoxLayout* mainLayout = new QVBoxLayout(this);
    w->setLayout(mainLayout);

    QLabel* label = new QLabel(this);
    label->setText(glCreateCaption("Messages"));
    mainLayout->addWidget(label);

    zv_messageTextEdit = new QTextEdit(this);
    mainLayout->addWidget(zv_messageTextEdit);

    QDialogButtonBox* buttonBox = new QDialogButtonBox(this);
    mainLayout->addWidget(buttonBox);
    zv_clearMessagesButton = new QPushButton(this);
    zv_clearMessagesButton->setText("Clear");
    buttonBox->addButton(zv_clearMessagesButton, QDialogButtonBox::ActionRole);

    return w;
}
//=====================================================
void ZSQLCommanderDialog::zh_restoreSettings()
{
    QSettings settings;
    QVariant vData;
    settings.beginGroup(glAppVersion);
    settings.beginGroup("SQLCommanderDialog");

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

    vData = settings.value("topSplitterState");
    if(vData.isValid() && !vData.isNull() && vData.canConvert<QByteArray>())
    {
        zv_topSplitter->restoreState(vData.value<QByteArray>());
    }

    vData = settings.value("bottomSplitterState");
    if(vData.isValid() && !vData.isNull() && vData.canConvert<QByteArray>())
    {
        zv_bottomSplitter->restoreState(vData.value<QByteArray>());
    }

    settings.endGroup();
    settings.endGroup();
}
//=====================================================
void ZSQLCommanderDialog::zh_saveSettings() const
{
    QSettings settings;
    settings.beginGroup(glAppVersion);
    settings.beginGroup("SQLCommanderDialog");

    settings.setValue("dialogGeometry", QVariant::fromValue<QByteArray>(this->saveGeometry()));
    //settings.setValue("splitterGeometry", QVariant::fromValue<QByteArray>(zv_tableSplitter->saveState()));
    settings.setValue("mainSplitterState", QVariant(zv_mainSplitter->saveState()));
    settings.setValue("topSplitterState", QVariant(zv_topSplitter->saveState()));
    settings.setValue("bottomSplitterState", QVariant(zv_bottomSplitter->saveState()));

    settings.endGroup();
    settings.endGroup();
}
//=====================================================
void ZSQLCommanderDialog::zh_loadDatabaseTableList()
{
    zv_tableListWidget->clear();

    if(!QSqlDatabase::database().open())
    {
        QString msg = "Database is not open!";
        zv_messageTextEdit->append(msg);
        return;
    }
    QSqlQuery query;
    QString sqlCommand = QString("SELECT * FROM sqlite_master WHERE type=\'table\'")/*.arg(QSqlDatabase::database().databaseName())*/;
    if(!query.prepare(sqlCommand))
    {
        QString msg = QString("Cannot prepare query for database table list.");
        QString lastErrorMsg = query.lastError().text().simplified();
        if(!lastErrorMsg.isEmpty())
        {
            msg += QString(" Error: %1.").arg(lastErrorMsg);
        }
        zv_messageTextEdit->append(msg);
        return;
    }

    if(!query.exec())
    {
        QString msg = QString("Cannot execute query for database table list.");
        QString lastErrorMsg = query.lastError().text().simplified();
        if(!lastErrorMsg.isEmpty())
        {
            msg += QString(" Error: %1.").arg(lastErrorMsg);
        }
        zv_messageTextEdit->append(msg);
        return;
    }

    while (query.next())
    {
        QString tableName = query.value(1).toString();
        zv_tableListWidget->addItem(tableName);
    }

}
//=====================================================
void ZSQLCommanderDialog::zh_addRow()
{
    if(!zv_dataModel->insertRows(zv_dataModel->rowCount(), 1, QModelIndex()))
    {
        QString msg = QString("Cannot insert row.");
        QString lastErrorMsg = zv_dataModel->lastError().text().simplified();
        if(!lastErrorMsg.isEmpty())
        {
            msg += QString(" Error: %1.").arg(lastErrorMsg);
        }
        zv_messageTextEdit->append(msg);
    }
}
//=====================================================
void ZSQLCommanderDialog::zh_removeRows()
{
    QModelIndexList indexList = zv_dataTableView->selectionModel()->selectedIndexes();
    if(indexList.isEmpty())
    {
        return;
    }

    QList<int> selectedRowList;
    foreach(QModelIndex index, indexList)
    {
        if(!index.isValid())
        {
            continue;
        }

        if(!selectedRowList.contains(index.row()))
        {
            selectedRowList.append(index.row());
        }
    }

    qSort(selectedRowList);

    for(int i = selectedRowList.count() - 1; i >= 0; i--)
    {
        if(!zv_dataModel->removeRow(selectedRowList.at(i)))
        {
            QString msg = QString("Cannot remove row %1.").arg(QString::number(selectedRowList.at(i)));
            zv_messageTextEdit->append(msg);
        }
    }
}
//=====================================================
void ZSQLCommanderDialog::zh_commitData()
{
    if(!zv_dataModel->submitAll())
    {
        QString msg = QString("Cannot submit data.");
        QString lastErrorMsg = zv_dataModel->lastError().text().simplified();
        if(!lastErrorMsg.isEmpty())
        {
            msg += QString(" Error: %1.").arg(lastErrorMsg);
        }
        zv_messageTextEdit->append(msg);
    }
}
//=====================================================
void ZSQLCommanderDialog::zh_executeQuery()
{
    QString command = zv_queryTextEdit->toPlainText();
    if(command.isEmpty())
    {
        return;
    }

    QSqlQuery query;
    if(!query.prepare(command))
    {
        QString msg = QString("Cannot prepare query.");
        QString lastErrorMsg = query.lastError().text().simplified();
        if(!lastErrorMsg.isEmpty())
        {
            msg += QString(" Error: %1.").arg(lastErrorMsg);
        }
        zv_messageTextEdit->append(msg);
        return;
    }

    if(!query.exec())
    {
        QString msg = QString("Cannot execute query.");
        QString lastErrorMsg = query.lastError().text().simplified();
        if(!lastErrorMsg.isEmpty())
        {
            msg += QString(" Error: %1.").arg(lastErrorMsg);
        }
        zv_messageTextEdit->append(msg);
        return;
    }

    if(query.isSelect())
    {
        // reset selection widget
        zh_clearSelectedDataWidget();
        zv_selectedDataTableWidget->setSortingEnabled(false);
        // define table dimensions
        QSqlRecord record = query.record();
        int columnCount = record.count();
        zv_selectedDataTableWidget->setColumnCount(columnCount);
        // headers
        QStringList headerList;
        for(int i = 0; i < columnCount; i++)
        {
            headerList.append(record.fieldName(i));
        }
        zv_selectedDataTableWidget->setHorizontalHeaderLabels(headerList);
        QTableWidgetItem* item;
        int row = 0;
        while (query.next())
        {
            zv_selectedDataTableWidget->insertRow(row);
            for(int col = 0; col < columnCount; col++)
            {
                item = new QTableWidgetItem;
                item->setData(Qt::DisplayRole, query.value(col));
                zv_selectedDataTableWidget->setItem(row, col, item);
            }
            row++;
        }
        // zv_selectedDataTableWidget->setSortingEnabled(true);
    }
}
//=====================================================
void ZSQLCommanderDialog::zh_clearSelectedDataWidget()
{
    zv_selectedDataTableWidget->clear();
    zv_selectedDataTableWidget->setRowCount(0);
    zv_selectedDataTableWidget->setColumnCount(0);
}
//=====================================================
void ZSQLCommanderDialog::zh_updateDataTable()
{
    zh_onCurrentTableChange(zv_tableListWidget->currentRow());
}
//=====================================================
void ZSQLCommanderDialog::zh_onCurrentTableChange(int row)
{
    zv_dataModel->clear();
    zv_dataTableView->setSortingEnabled(false);

    if(row < 0 || row >= zv_tableListWidget->count())
    {
        return;
    }

    QString tableName =  zv_tableListWidget->item(row)->text();
    zv_dataModel->setTable(tableName);

    if(!zv_dataModel->select())
    {
        QString msg = "Cannot select data.";
        QString lastErrorMsg = zv_dataModel->lastError().text().simplified();
        if(!lastErrorMsg.isEmpty())
        {
            msg += QString(" Error: %1.").arg(lastErrorMsg);
        }
        zv_messageTextEdit->append(msg);
    }

    // zv_dataTableView->setSortingEnabled(true);

}
//=====================================================
