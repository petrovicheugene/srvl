//===================================================
#include "ZSampleTaskTreeModel.h"
#include "ZGLConstantsAndDefines.h"

#include "ZChemicalTaskListDialog.h"
#include "ZMeasuringConditionsListDialog.h"
#include "ZSampleTaskTreeItem.h"

#include "ZControlAction.h"
#include <QMap>
#include <QPointF>
#include <QSqlQuery>
#include <QSqlError>
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

    ZSampleTaskTreeBaseItem* item = zp_itemForIndex(index);
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
        if(index.column() == 1)
        {
            if(item->zp_itemType() == ZSampleTaskTreeItemOptions::IT_CALIBRATION)
            {
                ZSampleTaskTreeCalibrationItem* calibrationItem = qobject_cast<ZSampleTaskTreeCalibrationItem*>(item);
                return QVariant(calibrationItem->zp_minConcentration());
            }
        }
        if(index.column() == 2)
        {
            if(item->zp_itemType() == ZSampleTaskTreeItemOptions::IT_CALIBRATION)
            {
                ZSampleTaskTreeCalibrationItem* calibrationItem = qobject_cast<ZSampleTaskTreeCalibrationItem*>(item);
                return QVariant(calibrationItem->zp_maxConcentration());
            }
        }
    }

    if(role == Qt::TextAlignmentRole)
    {
        if(index.column() != 0)
        {
            return QVariant(Qt::AlignHCenter);
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
    ZSampleTaskTreeBaseItem* item = zp_itemForIndex(parent);
    if(item == 0)
    {
        return false;
    }

    return item->zp_hasChildren();
}
//===================================================
QVariant ZSampleTaskTreeModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if(orientation == Qt::Horizontal)
    {
        if(role == Qt::DisplayRole)
        {
            if(section == 0)
            {
                return tr("Task");
            }
            if(section == 1)
            {
                return tr("Min");
            }
            if(section == 2)
            {
                return tr("Max");
            }
        }

        if(role == Qt::TextAlignmentRole)
        {
            return QVariant(Qt::AlignHCenter);
        }
    }

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

    ZSampleTaskTreeBaseItem* parentItem = zp_itemForIndex(parent);
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
    ZSampleTaskTreeBaseItem* item = zp_itemForIndex(index);
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
    ZSampleTaskTreeBaseItem* parentItem = zp_itemForIndex(parent);
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
    zv_removeCurrentTaskElementAction->setText("Clear");
    zv_removeCurrentTaskElementAction->setToolTip(tr("Clear chemical tasks"));

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
void ZSampleTaskTreeModel::zh_onAppendMeasuringConditionsAction()
{
    ZMeasuringConditionsListDialog dialog;
    if(!dialog.exec())
    {
        return;
    }

    // get measuring conditions data
    int gainFactor = dialog.zp_gainFactor();
    int exposition = dialog.zp_exposition();
    int quantity = dialog.zp_quantity();

    // init options
    ZSampleTaskTreeMeasuringConditionsItemOptions options(gainFactor, exposition);
    for(int i = 0; i < quantity; i++)
    {
        zv_rootItem->zp_createChild(&options);
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

    int chemicalTaskId = dialog.zp_currentChemicalTaskId();

    // measuring conditions from chemical task
    QString queryString = QString("SELECT * FROM calibration_stacks WHERE id=%1").arg(QString::number(chemicalTaskId));
    QSqlQuery query;
    if(!query.prepare(queryString))
    {
        return;
    }

    if(!query.exec())
    {
        return;
    }

    if(!query.next())
    {
        return;
    }

    QVariant vData = query.value(4);
    if(!vData.isValid() || !vData.canConvert<int>())
    {
        return;
    }
    int gainFactor = vData.toInt();

    vData = query.value(5);
    if(!vData.isValid() || !vData.canConvert<int>())
    {
        return;
    }
    int exposition = vData.toInt();

    // find measuring conditions item if it does exist
    ZSampleTaskTreeBaseItem* baseItem = 0;
    ZSampleTaskTreeMeasuringConditionsItem* measuringConditionsItem = 0;
    bool itemExist = false;
    for(int r = 0; r < zv_rootItem->zp_childCount(); r++)
    {
        baseItem = zv_rootItem->zp_childAt(r);
        measuringConditionsItem = qobject_cast<ZSampleTaskTreeMeasuringConditionsItem*>(baseItem);

        if(!measuringConditionsItem)
        {
            continue;
        }

        if(measuringConditionsItem->zp_gainFactor() == gainFactor &&
                measuringConditionsItem->zp_exposition() == exposition)
        {
            itemExist = true;
            break;
        }
    }

    // create measuring conditions item if it doesn't exist
    if(!itemExist)
    {
        ZSampleTaskTreeMeasuringConditionsItemOptions options(gainFactor, exposition);
        baseItem = zv_rootItem->zp_createChild(&options);
        measuringConditionsItem = qobject_cast<ZSampleTaskTreeMeasuringConditionsItem*>(baseItem);
    }

    // create chemical task
    query.clear();
    queryString = QString("SELECT calibration_stacks.name, "
                          "chemicals.name "
                          "FROM calibration_stacks "
                          "JOIN chemicals "
                          "ON calibration_stacks.chemicals_id=chemicals.id "
                          "WHERE calibration_stacks.id=%1").arg(QString::number(chemicalTaskId));

    if(!query.prepare(queryString))
    {
        return;
    }

    if(!query.exec())
    {
        return;
    }

    if(!query.next())
    {
        return;
    }

    QString chemicalTaskName;
    QString chemical;

    vData = query.value(0);
    if(!vData.isValid() || !vData.canConvert<QString>())
    {
        chemicalTaskName = tr("Undefined");
    }
    chemicalTaskName = vData.toString();

    vData = query.value(1);
    if(!vData.isValid() || !vData.canConvert<QString>())
    {
        chemicalTaskName = tr("Undefined");
    }
    chemical = vData.toString();


    ZSampleTaskTreeChemicalTaskItemOptions chemicaslTaskOptions(chemicalTaskId,
                                                                chemicalTaskName,
                                                                chemical);

    baseItem = measuringConditionsItem->zp_createChild(&chemicaslTaskOptions);
    // create calibration items
    ZSampleTaskTreeChemicalTaskItem* chemicalTaskItem =  qobject_cast<ZSampleTaskTreeChemicalTaskItem*>(baseItem);
    if(chemicalTaskItem == 0)
    {
        return;
    }

    // get calibration from tasked calibration
    query.clear();
    queryString = QString("SELECT calibrations.name, "
                          "calibrations_has_calibration_stacks.calibration_stacks_id, "
                          "calibrations_has_calibration_stacks.calibration_min_limit, "
                          "calibrations_has_calibration_stacks.calibration_max_limit "
                          "FROM calibrations_has_calibration_stacks "
                          "JOIN calibrations "
                          "ON calibrations_has_calibration_stacks.calibrations_id=calibrations.id "
                          "WHERE calibrations_has_calibration_stacks.calibration_stacks_id=%1").arg(QString::number(chemicalTaskId));

    if(!query.prepare(queryString))
    {
        return;
    }

    if(!query.exec())
    {
        return;
    }

    QString calibrationName;
    double min;
    double max;
    while(query.next())
    {
        vData = query.value(0);
        if(!vData.isValid() || !vData.canConvert<QString>())
        {
            calibrationName = tr("Undefined");
        }
        calibrationName = vData.toString();

        vData = query.value(2);
        if(!vData.isValid() || !vData.canConvert<double>())
        {
            min = -1;
        }
        min = vData.toDouble();

        vData = query.value(3);
        if(!vData.isValid() || !vData.canConvert<double>())
        {
            max = -1;
        }
        max = vData.toDouble();

        ZSampleTaskTreeCalibrationItemOptions calibrationOptions(calibrationName, min, max);
        baseItem = chemicalTaskItem->zp_createChild(&calibrationOptions);

    }

    // set last item current
    QModelIndex index;
    zh_indexForItem(baseItem, index);
    emit zg_setCurrentIndex(index);

}
//===================================================
void ZSampleTaskTreeModel::zh_onRemoveCurrentTaskElementAction()
{
    beginResetModel();
    zv_rootItem->zp_clear();
    endResetModel();
}
//===================================================
ZSampleTaskTreeBaseItem* ZSampleTaskTreeModel::zp_itemForIndex(const QModelIndex& index) const
{
    if(!index.isValid())
    {
        return zv_rootItem;
    }

    ZSampleTaskTreeBaseItem* item = static_cast<ZSampleTaskTreeBaseItem*>(index.internalPointer());
    return item;
}
//===================================================
