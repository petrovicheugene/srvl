//===================================================
#include "ZSampleTaskTreeModel.h"
#include "ZGLConstantsAndDefines.h"

#include "ZChemicalTaskListDialog.h"
#include "ZMeasuringConditionsListDialog.h"
#include "ZSampleTaskTreeItem.h"

#include "ZControlAction.h"
//===================================================
ZSampleTaskTreeModel::ZSampleTaskTreeModel(QObject *parent)
    : QAbstractItemModel(parent)
{
    zv_rootItem = new ZSampleTaskTreeRootItem();
    zh_createActions();
    zh_createConnections();
}
//===================================================
ZSampleTaskTreeModel::~ZSampleTaskTreeModel()
{
    delete zv_rootItem;
}
//===================================================
QList<ZControlAction*> ZSampleTaskTreeModel::zp_buttonTreeActions() const
{
    QList<ZControlAction*> actionList;
    actionList.append(zv_appendMeasuringConditionsAction);
    actionList.append(zv_appendChemicalTaskAction);
    actionList.append(zv_removeCurrentTaskElementAction);

    return actionList;
}
//===================================================
QList<ZControlAction*> ZSampleTaskTreeModel::zp_contextTreeActions() const
{
    QList<ZControlAction*> actionList;
    actionList.append(zv_appendMeasuringConditionsAction);
    actionList.append(zv_appendChemicalTaskAction);
    actionList.append(0); // separator
    actionList.append(zv_removeCurrentTaskElementAction);

    return actionList;
}
//===================================================
QVariant ZSampleTaskTreeModel::data(const QModelIndex &index, int role) const
{
    if(!index.isValid() || index.column() >= zv_columnCount || index.column() < 0
            || index.row() < 0)
    {
        return QVariant();
    }

    ZSampleTaskTreeBaseItem* item = zh_itemForIndex(index);
    if(item == 0)
    {
        return QVariant();
    }

    if(role == Qt::DisplayRole)
    {
        if(index.column() == 0)
        {
            return QVariant(item->zp_itemName());
        }
    }

    return QVariant();
}
//===================================================
Qt::ItemFlags ZSampleTaskTreeModel::flags(const QModelIndex &index) const
{
    Qt::ItemFlags flags = Qt::ItemIsEnabled | Qt::ItemIsSelectable;
    return flags;
}
//===================================================
bool ZSampleTaskTreeModel::hasChildren(const QModelIndex &parent) const
{
    ZSampleTaskTreeBaseItem* item = zh_itemForIndex(parent);
    if(item == 0)
    {
        return false;
    }

    return item->zp_hasChildren();
}
//===================================================
QVariant ZSampleTaskTreeModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    return QVariant();
}
//===================================================
QModelIndex ZSampleTaskTreeModel::index(int row, int column, const QModelIndex &parent) const
{
    if(row < 0 || column < 0 || column >= columnCount()
            || (parent.isValid() && parent.column() != 0))
    {
        return QModelIndex();
    }

    ZSampleTaskTreeBaseItem* parentItem = zh_itemForIndex(parent);
    if(parentItem == 0 || row >= parentItem->zp_childCount())
    {
        return QModelIndex();
    }

    ZSampleTaskTreeBaseItem* item = parentItem->zp_childAt(row);
    if(item == 0)
    {
        return QModelIndex();
    }

    return createIndex(row, column, item);
}
//===================================================
int ZSampleTaskTreeModel::columnCount(const QModelIndex &parent) const
{
    if(parent.isValid() && parent.column() != 0)
    {
        return 0;
    }
    return zv_columnCount;
}
//===================================================
int ZSampleTaskTreeModel::rowCount(const QModelIndex &parent) const
{
    if(parent.isValid() && parent.column() != 0)
    {
        return 0;
    }

    return zp_childCount(parent);
}
//===================================================
QModelIndex ZSampleTaskTreeModel::parent(const QModelIndex &index) const
{
    if(!index.isValid() || index.column() != 0)
    {
        return QModelIndex();
    }

    // child item
    ZSampleTaskTreeBaseItem* item = zh_itemForIndex(index);
    if(item == 0 || item == zv_rootItem)
    {
        return QModelIndex();
    }

    // parent item
    ZSampleTaskTreeBaseItem* parentItem = item->zp_parent();
    if(parentItem == 0)
    {
        return QModelIndex();
    }

    // grand parent
    ZSampleTaskTreeBaseItem* grandParentItem = parentItem->zp_parent();
    if(grandParentItem == 0)
    {
        return QModelIndex();
    }

    int row = grandParentItem->zp_rowOfChild(parentItem);
    return createIndex(row, 0, parentItem);
}
//===================================================
int ZSampleTaskTreeModel::zp_childCount(QModelIndex parent) const
{
    ZSampleTaskTreeBaseItem* parentItem = zh_itemForIndex(parent);
    if(parentItem == 0)
    {
        return 0;
    }

    return parentItem->zp_childCount();
}
//===================================================
void ZSampleTaskTreeModel::zh_createActions()
{
    zv_appendMeasuringConditionsAction = new ZControlAction(this);
    zv_appendMeasuringConditionsAction->setText("Add M.C.");
    zv_appendMeasuringConditionsAction->setToolTip(tr("Append measuring conditions"));

    zv_appendChemicalTaskAction = new ZControlAction(this);
    zv_appendChemicalTaskAction->setText("Add Ch.T.");
    zv_appendChemicalTaskAction->setToolTip(tr("Append chemical task"));

    zv_removeCurrentTaskElementAction = new ZControlAction(this);
    zv_removeCurrentTaskElementAction->setText("Remove");
    zv_removeCurrentTaskElementAction->setToolTip(tr("Remove current task"));

}
//===================================================
void ZSampleTaskTreeModel::zh_createConnections()
{
    connect(zv_rootItem, &ZSampleTaskTreeBaseItem::zg_requestIndexForItem,
            this, &ZSampleTaskTreeModel::zh_indexForItem);
    connect(zv_rootItem, &ZSampleTaskTreeBaseItem::zg_itemOperation,
            this, &ZSampleTaskTreeModel::zh_onItemOperation);

    connect(zv_appendMeasuringConditionsAction, &ZControlAction::triggered,
            this, &ZSampleTaskTreeModel::zh_onAppendMeasuringConditionsAction);
    connect(zv_appendChemicalTaskAction, &ZControlAction::triggered,
            this, &ZSampleTaskTreeModel::zh_onAppendChemicalTaskAction);
    connect(zv_removeCurrentTaskElementAction, &ZControlAction::triggered,
            this, &ZSampleTaskTreeModel::zh_onRemoveCurrentTaskElementAction);

}
//===================================================
void ZSampleTaskTreeModel::zh_onAppendMeasuringConditionsAction()
{
    ZMeasuringConditionsListDialog dialog;
    if(!dialog.exec())
    {
        return;
    }

    // get measuring conditions data
    int gf = dialog.zp_gainFactor();
    int expo = dialog.zp_exposition();
    int quantity = dialog.zp_quantity();

    // init options
    ZSampleTaskTreeMeasuringConditionsItemOptions options(gf, expo);
    for(int i = 0; i < quantity; i++)
    {
        zv_rootItem->zp_createChild(&options);
    }
}
//===================================================
void ZSampleTaskTreeModel::zh_indexForItem(ZSampleTaskTreeBaseItem* item,
                            QModelIndex& index)
{
    if(item == 0 || item == zv_rootItem)
    {
        index = QModelIndex();
        return;
    }

    int row;
    ZSampleTaskTreeBaseItem* parentItem;
    if(!zv_rootItem->zp_findItemRow(item, parentItem, row))
    {
        index = QModelIndex();
        return;
    }

    index = createIndex(row, 0, item);
}
//===================================================
void ZSampleTaskTreeModel::zh_onItemOperation(ZSampleTaskTreeBaseItem::ItemOperationType type,
                   QModelIndex parent,
                   int first,
                   int last)
{
    if(type == ZSampleTaskTreeBaseItem::IOT_ABOUT_TO_INSERT_ROW)
    {
        beginInsertRows(parent, first, last);
    }
    else if(type == ZSampleTaskTreeBaseItem::IOT_ROW_INSERTED)
    {
        endInsertRows();
    }
    else if(type == ZSampleTaskTreeBaseItem::IOT_ABOUT_TO_REMOVE_ROW)
    {
        beginRemoveRows(parent, first, last);
    }
    else if(type == ZSampleTaskTreeBaseItem::IOT_ROW_REMOVED)
    {
        endRemoveRows();
    }
}
//===================================================
void ZSampleTaskTreeModel::zh_onAppendChemicalTaskAction()
{
    ZChemicalTaskListDialog dialog;
    if(!dialog.exec())
    {
        return;
    }
}
//===================================================
void ZSampleTaskTreeModel::zh_onRemoveCurrentTaskElementAction()
{

}
//===================================================
ZSampleTaskTreeBaseItem* ZSampleTaskTreeModel::zh_itemForIndex(const QModelIndex& index) const
{
    if(!index.isValid())
    {
        return zv_rootItem;
    }

    ZSampleTaskTreeBaseItem* item = static_cast<ZSampleTaskTreeBaseItem*>(index.internalPointer());
    return item;
}
//===================================================
