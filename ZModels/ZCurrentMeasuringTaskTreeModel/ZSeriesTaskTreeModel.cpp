//===========================================================
#include "ZSeriesTaskTreeModel.h"
#include "ZGeneral.h"

#include "ZControlAction.h"
#include "ZSampleTaskItem.h"

#include "ZAddSampleDialog.h"
#include "ZSampleTaskDialog2.h"

//qt
#include <QMessageBox>
//===========================================================
ZSeriesTaskTreeModel::ZSeriesTaskTreeModel(QObject *parent)
    : QAbstractItemModel(parent)
{
    zh_createActions();
    zh_createConnections();
}
//===========================================================
void ZSeriesTaskTreeModel::zh_createActions()
{
    // sample actions
    zv_newSampleAction = new ZControlAction(this);
    zv_newSampleAction->setText("New");
    zv_newSampleAction->setToolTip(tr("Append a new sample to the series task"));

    zv_removeSampleAction = new ZControlAction(this);
    zv_removeSampleAction->setText("Remove");
    zv_removeSampleAction->setToolTip(tr("Remove current sample"));

    // task actions
    zv_newTaskAction = new ZControlAction(this);
    zv_newTaskAction->setText("New");
    zv_newTaskAction->setToolTip(tr("Append a new sample task to the series task"));

    zv_addTaskAction = new ZControlAction(this);
    zv_addTaskAction->setText("Add");
    zv_addTaskAction->setToolTip(tr("Append an existing sample task to the series task"));

    zv_editTaskAction = new ZControlAction(this);
    zv_editTaskAction->setText("Edit");
    zv_editTaskAction->setToolTip(tr("Edit current sample task"));

    zv_removeTaskAction = new ZControlAction(this);
    zv_removeTaskAction->setText("Remove");
    zv_removeTaskAction->setToolTip(tr("Remove current sample task"));

}
//===========================================================
void ZSeriesTaskTreeModel::zh_createConnections()
{
    connect(zv_newSampleAction, &ZControlAction::triggered,
            this, &ZSeriesTaskTreeModel::zh_onNewSampleAction);
    connect(zv_removeSampleAction, &ZControlAction::triggered,
            this, &ZSeriesTaskTreeModel::zh_onRemoveSampleAction);

    connect(zv_newTaskAction, &ZControlAction::triggered,
            this, &ZSeriesTaskTreeModel::zh_onNewTaskAction);
    connect(zv_addTaskAction, &ZControlAction::triggered,
            this, &ZSeriesTaskTreeModel::zh_onAddTaskAction);
    connect(zv_editTaskAction, &ZControlAction::triggered,
            this, &ZSeriesTaskTreeModel::zh_onEditTaskAction);
    connect(zv_removeTaskAction, &ZControlAction::triggered,
            this, &ZSeriesTaskTreeModel::zh_onRemoveTaskAction);
}
//===========================================================
QVariant ZSeriesTaskTreeModel::data(const QModelIndex &index, int role) const
{
    if(!index.isValid() || index.row() < 0 || index.column() < 0
            || index.column() >= columnCount())
    {
        return QVariant();
    }

    if(role == Qt::DisplayRole)
    {
        if(index.column() == 0)
        {
            ZItem* item = zh_itemForIndex(index);
            if(item != 0)
            {
                return QVariant(item->zp_displayString());
            }
        }
        if(index.column() == 1)
        {
            if(!index.parent().isValid())
            {
                // invalid parent has taskItemOnly only
                // column 1 - sampleName request
                ZItem* item = zh_itemForIndex(index);
                if(item != 0)
                {
                    ZSampleTaskItem* sampleTaskItem = static_cast<ZSampleTaskItem*>(item);
                    return QVariant(sampleTaskItem->zp_sampleName());
                }
            }
        }
    }

    return QVariant();
}
//===========================================================
Qt::ItemFlags ZSeriesTaskTreeModel::flags(const QModelIndex &index) const
{
    Qt::ItemFlags flags;
    flags |= Qt::ItemIsEnabled;
    return flags;
}
//===========================================================
bool ZSeriesTaskTreeModel::hasChildren(const QModelIndex &parent) const
{
    if(!parent.isValid())
    {
        return !zv_sampleTaskList.isEmpty();
    }

    ZItem* parentItem = zh_itemForIndex(parent);
    if(parentItem == 0)
    {
        return false;
    }

    return parentItem->zp_hasChildren();
}
//===========================================================
QVariant ZSeriesTaskTreeModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    return QVariant();
}
//===========================================================
QModelIndex ZSeriesTaskTreeModel::index(int row, int column, const QModelIndex &parent) const
{
    if(row < 0 || column < 0 || column >= columnCount())
    {
        return QModelIndex();
    }

    if(!parent.isValid())
    {
        if(row >= zv_sampleTaskList.count())
        {
            return QModelIndex();
        }

        ZItem* item = zv_sampleTaskList.at(row);
        return createIndex(row, column, item);
    }

    ZItem* parentItem = zh_itemForIndex(parent);
    if(parentItem == 0 || row >= parentItem->zp_childCount())
    {
        return QModelIndex();
    }

    ZItem* item = parentItem->zp_childAt(row);
    if(item == 0)
    {
        return QModelIndex();
    }

    return createIndex(row, column, item);
}
//===========================================================
int ZSeriesTaskTreeModel::columnCount(const QModelIndex &parent) const
{
    return 2;
}
//===========================================================
int ZSeriesTaskTreeModel::rowCount(const QModelIndex &parent) const
{
    if(zv_sampleTaskList.isEmpty())
    {
        return 0;
    }

    if(!parent.isValid())
    {
        return zv_sampleTaskList.count();
    }

    ZItem* parentItem = zh_itemForIndex(parent);
    if(parentItem == 0)
    {
        return 0;
    }

    return parentItem->zp_childCount();
}
//===========================================================
QModelIndex ZSeriesTaskTreeModel::parent(const QModelIndex &index) const
{
    if(!index.isValid() || index.column() < 0 || index.column() >= columnCount()  || index.row() < 0)
    {
        return QModelIndex();
    }

    ZItem* item = zh_itemForIndex(index);
    if(item == 0)
    {
        return QModelIndex();
    }

    ZItem* parentItem = item->zp_parentItem();
    if(parentItem == 0)
    {
        return QModelIndex();
    }

    ZItem* parentForParentItem = parentItem->zp_parentItem();
    if(parentForParentItem == 0)
    {
        return QModelIndex();
    }

    int row = parentForParentItem->zp_childItemIndex(parentItem);
    if(row < 0 || row >= parentForParentItem->zp_childCount())
    {
        return QModelIndex();
    }

    return createIndex(row, 0, parentItem);
}
//===========================================================
QList<ZControlAction*> ZSeriesTaskTreeModel::zp_sampleActions() const
{
    QList<ZControlAction*> actionList;
    actionList.append(zv_newSampleAction);
    actionList.append(zv_removeSampleAction);

    return actionList;
}
//===========================================================
QList<ZControlAction*> ZSeriesTaskTreeModel::zp_contextSampleActions() const
{
    QList<ZControlAction*> actionList;
    actionList.append(zv_newSampleAction);
    actionList.append(0); // separator
    actionList.append(zv_removeSampleAction);

    return actionList;
}
//===========================================================
QList<ZControlAction*> ZSeriesTaskTreeModel::zp_taskActions() const
{
    QList<ZControlAction*> actionList;
    actionList.append(zv_newTaskAction);
    actionList.append(zv_addTaskAction);
    actionList.append(zv_editTaskAction);
    actionList.append(zv_removeTaskAction);

    return actionList;
}
//===========================================================
QList<ZControlAction*> ZSeriesTaskTreeModel::zp_contextTaskActions() const
{
    QList<ZControlAction*> actionList;
    actionList.append(zv_newTaskAction);
    actionList.append(zv_addTaskAction);
    actionList.append(zv_editTaskAction);
    actionList.append(0);
    actionList.append(zv_removeTaskAction);

    return actionList;
}
//===========================================================
void ZSeriesTaskTreeModel::zh_onNewSampleAction()
{
    ZAddSampleDialog dialog;
    connect(&dialog, SIGNAL(zg_checkSampleName(const QString&, bool&)),
            this, SLOT(zh_checkSampleName(const QString&, bool&)));

    if(!dialog.exec())
    {
        return;
    }

    // zh_insertSampleTaskIntoList(dialog.zp_sampleName());
}
//===========================================================
void ZSeriesTaskTreeModel::zh_onRemoveSampleAction()
{

}
//===========================================================
void ZSeriesTaskTreeModel::zh_onNewTaskAction()
{
    ZSampleTaskDialog2 dialog;
//    connect(&dialog, &ZSampleTaskDialog2::zg_checkTaskName,
//            this, &ZSeriesTaskTreeModel::zh_checkTaskName);

    if(!dialog.exec())
    {
        return;
    }

//    int sampleTaskQuantity = 0;//dialog.zp_sampleTaskQuantity();
//    //QString defaultSampleName = dialog.zp_sampleNameTemplate();
//    int lastSampleSerialNumber = zh_findLastSampleSerialNumber(defaultSampleName);
//    QString sampleName;

//    for(int i = 0; i < sampleTaskQuantity; i++)
//    {
//        for(int c = 0; c < 999999999; c++)
//        {
//            sampleName = defaultSampleName + QString::number(++lastSampleSerialNumber);
//            if(zh_checkSampleName(sampleName))
//            {
//               break;
//            }
//        }

//        zh_insertSampleTaskIntoList(sampleName, dialog.zp_sampleTaskName());
//    }
}
//===========================================================
int ZSeriesTaskTreeModel::zh_findLastSampleSerialNumber(const QString& sampleNameTemplate) const
{
    QString currentSampleName;
    int lastSerialNumber = 0;
    QString extraPart;
    bool ok;
    int currentSerialNumber;
    foreach(ZSampleTaskItem* item, zv_sampleTaskList)
    {
        currentSampleName = item->zp_sampleName();
        if(!currentSampleName.startsWith(sampleNameTemplate))
        {
            continue;
        }

        // try to convert right part to Int
        currentSerialNumber = currentSampleName.right(currentSampleName.length() - sampleNameTemplate.length()).toInt(&ok);
        if(!ok)
        {
            continue;
        }

        if(lastSerialNumber < currentSerialNumber)
        {
            lastSerialNumber = currentSerialNumber;
        }
    }

    return lastSerialNumber;
}
//===========================================================
bool ZSeriesTaskTreeModel::zh_checkSampleName(const QString& sampleName) const
{
    foreach(ZSampleTaskItem* item, zv_sampleTaskList)
    {
        if(item->zp_sampleName() == sampleName)
        {
            return false;
        }
    }
    return true;
}
//===========================================================
void ZSeriesTaskTreeModel::zh_onAddTaskAction()
{

}
//===========================================================
void ZSeriesTaskTreeModel::zh_onEditTaskAction()
{
//    ZSampleTaskDialog dialog(false);
//    if(!dialog.exec())
//    {
//        return;
//    }
}
//===========================================================
void ZSeriesTaskTreeModel::zh_onRemoveTaskAction()
{

}
//===========================================================
void  ZSeriesTaskTreeModel::zh_insertSampleTaskIntoList(const QString& sampleName,
                                                        const QString& sampleTaskName)
{
    ZSampleTaskItem* sample = new ZSampleTaskItem(sampleName, sampleTaskName, this);
    beginInsertRows(QModelIndex(), zv_sampleTaskList.count(), zv_sampleTaskList.count());
    zv_sampleTaskList.append(sample);
    endInsertRows();
}
//===========================================================
void ZSeriesTaskTreeModel::zh_checkSampleName(const QString& sampleName, bool& res)
{
    if(sampleName.isEmpty())
    {
        QString msg = tr("The name of sample cannot be empty!");
        QMessageBox::critical(0, tr(""), msg, QMessageBox::Ok);
        res = false;
        return;
    }

    foreach(ZSampleTaskItem* sample, zv_sampleTaskList)
    {
        if(sample->zp_displayString() == sampleName)
        {
            QString msg = tr("The name of the sample \"%1\" is not unique!").arg(sampleName);
            QMessageBox::critical(0, tr("Error"), msg, QMessageBox::Ok);
            res = false;
            return;
        }
    }

    res = true;
}
//===========================================================
void ZSeriesTaskTreeModel::zh_checkTaskName(int sampleTaskId, const QString& taskName, bool& res)
{
    if(taskName.isEmpty())
    {
        QString msg = tr("The name of task cannot be empty!");
        QMessageBox::critical(0, tr(""), msg, QMessageBox::Ok);
        res = false;
        return;
    }

    foreach(ZSampleTaskItem* sample, zv_sampleTaskList)
    {
        if(sample->zp_sampleTaskName() == taskName)
        {
            QString msg = tr("The name of the task \"%1\" is not unique!").arg(taskName);
            QMessageBox::critical(0, tr("Error"), msg, QMessageBox::Ok);
            res = false;
            return;
        }
    }

    res = true;
}
//===========================================================
ZItem* ZSeriesTaskTreeModel::zh_itemForIndex(const QModelIndex& index) const
{
    if(!index.isValid())
    {
        return 0;
    }

    ZItem* item = static_cast<ZItem*>(index.internalPointer());
    return item;
}
//===========================================================
