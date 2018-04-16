//======================================================
#include "ZSeriesTaskListManager.h"
#include "ZGeneral.h"
#include "ZControlAction.h"

#include <QAbstractItemView>
#include <QSqlError>
#include <QSqlField>
#include <QSqlRecord>
#include <QSqlTableModel>
#include <QSqlQuery>

//======================================================
ZSeriesTaskListManager::ZSeriesTaskListManager(QObject *parent)
    : QObject(parent)
{
    zv_currentIndex = QModelIndex();
    zh_createComponents();
    zh_createActions();
    zh_createConnections();
}
//======================================================
void ZSeriesTaskListManager::zh_createComponents()
{
    zv_seriesSqlTableModel = new QSqlTableModel(this);
    zv_seriesSqlTableModel->setTable("series_tasks");
    zv_seriesSqlTableModel->select();
    zv_seriesSqlTableModel->setEditStrategy(QSqlTableModel::OnManualSubmit);


}
//======================================================
void ZSeriesTaskListManager::zh_createActions()
{

}
//======================================================
void ZSeriesTaskListManager::zh_createConnections()
{

}
//======================================================
void ZSeriesTaskListManager::zp_connectViewToModel(QAbstractItemView* view)
{
    if(!view)
    {
        return;
    }

    view->setModel(zv_seriesSqlTableModel);
}
//======================================================
QAbstractTableModel* ZSeriesTaskListManager::zp_model()
{
    return zv_seriesSqlTableModel;
}
//======================================================
bool ZSeriesTaskListManager::zp_loadSeriesTask()
{


    return true;
}
//======================================================
bool ZSeriesTaskListManager::zp_saveSeriesTask(const QString& taskName, const QString& description)
{
    int newSeriesTaskId;
    if(!zh_checkTaskNameAndFindNewId(taskName, newSeriesTaskId))
    {
        return false;
    }

    // get sample task id list
    QList<int> sampleTaskIdList;
    emit zg_inquirySampleTaskIdList(sampleTaskIdList);

    if(sampleTaskIdList.isEmpty())
    {
        zv_errorMsg = tr("There is no sample to save!");
        return false;
    }

    // find new sampleTaskHasSeriesId
    QSqlQuery query;
    QString queryString = "SELECT MAX(id) FROM sample_tasks_has_series_tasks";

    if(!query.prepare(queryString))
    {
        zv_errorMsg = query.lastError().text();
        return false;
    }

    if(!query.exec())
    {
        zv_errorMsg = query.lastError().text();
        return false;
    }

    int sampleTaskHasSeriesId;
    if(!query.next())
    {
        sampleTaskHasSeriesId = 0;
    }
    else
    {
        QVariant vData = query.value(0);
        if(!vData.isValid() || !vData.canConvert<int>())
        {
            zv_errorMsg = tr("Cannot cast maximum id for table sample_tasks_has_series_tasks "
                             "from QVariant value.");
            return false;
        }
        sampleTaskHasSeriesId = vData.toInt();
    }


    // Saving series task to database
    // series task

    QSqlRecord record;
    record.append(QSqlField("id", QVariant::Int));
    record.append(QSqlField("name", QVariant::String));
    record.append(QSqlField("description", QVariant::String));

    record.setValue(0, QVariant(newSeriesTaskId));
    record.setValue(1, QVariant(taskName));
    record.setValue(2, QVariant(description));

    if(!zv_seriesSqlTableModel->insertRecord(-1, record))
    {
        zv_errorMsg = zv_seriesSqlTableModel->lastError().text();
        return false;
    }

    if(!zv_seriesSqlTableModel->submitAll())
    {
        zv_errorMsg = zv_seriesSqlTableModel->lastError().text();
        zv_seriesSqlTableModel->revertAll();
        return false;
    }

    // sample tsak is list
    // sample_tasks_has_series_tasks

    QSqlTableModel sampleTaskHasSeriesModel;
    sampleTaskHasSeriesModel.setTable("sample_tasks_has_series_tasks");
    sampleTaskHasSeriesModel.select();

    for(int i = 0; i < sampleTaskIdList.count(); i++)
    {
        record.clear();
        record.append(QSqlField("id", QVariant::Int));
        record.append(QSqlField("sample_tasks_id", QVariant::Int));
        record.append(QSqlField("series_tasks_id", QVariant::Int));

        record.setValue(0, QVariant(++sampleTaskHasSeriesId));
        record.setValue(1, QVariant(sampleTaskIdList.at(i)));
        record.setValue(2, QVariant(newSeriesTaskId));

        if(!sampleTaskHasSeriesModel.insertRecord(-1, record))
        {
            zv_errorMsg = sampleTaskHasSeriesModel.lastError().text();
            zv_seriesSqlTableModel->removeRows(zv_seriesSqlTableModel->rowCount() - 1, 1);
            zv_seriesSqlTableModel->submitAll();
            sampleTaskHasSeriesModel.revertAll();
            sampleTaskHasSeriesModel.submitAll();
            return false;
        }
    }

    if(!sampleTaskHasSeriesModel.submitAll())
    {
        zv_errorMsg = sampleTaskHasSeriesModel.lastError().text();
        zv_seriesSqlTableModel->removeRows(zv_seriesSqlTableModel->rowCount() - 1, 1);
        zv_seriesSqlTableModel->submitAll();
        sampleTaskHasSeriesModel.revertAll();
        sampleTaskHasSeriesModel.submitAll();
        return false;
    }

    return true;
}
//======================================================
bool ZSeriesTaskListManager::zh_checkTaskNameAndFindNewId(const QString& name, int& newId)
{
    QString queryString = QString("SELECT * FROM series_tasks");
    QSqlQuery query;
    if(!query.prepare(queryString))
    {
        zv_errorMsg = query.lastError().text();
        return -1;
    }

    if(!query.exec())
    {
        zv_errorMsg = query.lastError().text();
        return -1;
    }

    int id = 0;
    int currentId;
    QVariant vData;
    while(query.next())
    {
        // id
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

        // name
        vData = query.value(1);
        if(!vData.isValid() || !vData.canConvert<QString>())
        {
            continue;
        }

        vData.toString();
        if(vData.toString() == name)
        {
            zv_errorMsg = tr("The series task name is not unique!");
            return false;
        }
    }

    newId = ++id;
    return true;
}
//======================================================
QString ZSeriesTaskListManager::zp_lastError() const
{
    return zv_errorMsg;
}
//======================================================
void ZSeriesTaskListManager::zp_onCurrentChange(const QModelIndex& current,
                                                const QModelIndex& previous)
{
    QString name;
    QString description;
    zv_currentIndex = current;

    if(current.isValid())
    {
        int row = current.row();
        QVariant vData;

        QModelIndex index;
        index = zv_seriesSqlTableModel->index(row, 1);

        if(index.isValid())
        {
            vData = index.data(Qt::DisplayRole);
            if(vData.isValid() && vData.canConvert<QString>())
            {
                name = vData.toString();
            }
        }

        index = zv_seriesSqlTableModel->index(row, 2);

        if(index.isValid())
        {
            vData = index.data(Qt::DisplayRole);
            if(vData.isValid() && vData.canConvert<QString>())
            {
                description = vData.toString();
            }
        }
    }


    emit zg_currentTaskProperties(name, description);
}
//======================================================
int ZSeriesTaskListManager::zp_currentSeriesTaskId()
{
    if(!zv_currentIndex.isValid())
    {
        zv_errorMsg = tr("Current series task table index is invalid.");
        return -1;
    }

    QModelIndex idIndex = zv_seriesSqlTableModel->index(zv_currentIndex.row(), 0);
    if(!idIndex.isValid())
    {
        zv_errorMsg = tr("Current series task table index for Id is invalid.");
        return -1;
    }

    QVariant vData = idIndex.data(Qt::DisplayRole);
    if(!vData.isValid() || !vData.canConvert<int>())
    {
        zv_errorMsg = tr("Cannot cast series task Id from QVariant.");
        return -1;
    }

    return vData.toInt();
}
//======================================================

