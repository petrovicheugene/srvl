//=================================================
#include "ZSampleTaskTreeItem.h"
//=================================================
// STATIC
const QMap<ZSampleTaskTreeItemOptions::ItemType, QString> ZSampleTaskTreeBaseItem::zv_tableNameForTypeMap =
        ZSampleTaskTreeBaseItem::zp_initTableNameForTypeMap();
//=================================================
QMap<ZSampleTaskTreeItemOptions::ItemType, QString> ZSampleTaskTreeBaseItem::zp_initTableNameForTypeMap()
{
    QMap<ZSampleTaskTreeItemOptions::ItemType, QString> map;
    map.insert(ZSampleTaskTreeItemOptions::IT_ROOT, "sample_tasks");
    map.insert(ZSampleTaskTreeItemOptions::IT_MEASURING_CONDITIONS, "conditions");
    map.insert(ZSampleTaskTreeItemOptions::IT_CHEMICAL_TASK, "chemical_task");
    map.insert(ZSampleTaskTreeItemOptions::IT_CALIBRATION, "calibrations");
    return map;
}
// END STATIC
// BASE
//=================================================
ZSampleTaskTreeBaseItem::ZSampleTaskTreeBaseItem(ZSampleTaskTreeItemOptions::ItemType type,
                                                 ZSampleTaskTreeBaseItem *parent)
    : QObject(parent)
{
    zv_itemName = QString();
    zv_parent = parent;
    zv_itemType = type;
    zv_id = -1;
}
//=================================================
ZSampleTaskTreeBaseItem::~ZSampleTaskTreeBaseItem()
{
    //qDeleteAll(zv_childrenList);
}
//=================================================
QString ZSampleTaskTreeBaseItem::zp_itemName() const
{
    return zv_itemName;
}
//=================================================
bool ZSampleTaskTreeBaseItem::zp_setItemName(const QString& displayString)
{
    if(displayString.isEmpty() || zv_itemName == displayString)
    {
        return false;
    }

    zv_itemName = displayString;
    return true;
}
//=================================================
ZSampleTaskTreeBaseItem* ZSampleTaskTreeBaseItem::zp_parent() const
{
    return zv_parent;
}
//=================================================
ZSampleTaskTreeBaseItem* ZSampleTaskTreeBaseItem::zp_childAt(int childIndex) const
{
    if(childIndex < 0 || childIndex >= zv_childList.count())
    {
        return 0;
    }

    return zv_childList.at(childIndex);
}
//=================================================
ZSampleTaskTreeItemOptions::ItemType ZSampleTaskTreeBaseItem::zp_itemType() const
{
    return zv_itemType;
}
//=================================================
int ZSampleTaskTreeBaseItem::zp_id() const
{
    return zv_id;
}
//=================================================
bool ZSampleTaskTreeBaseItem::zp_hasChildren() const
{
    return !zv_childList.isEmpty();
}
//=================================================
int ZSampleTaskTreeBaseItem::zp_childCount() const
{
    return zv_childList.count();
}
//=================================================
int ZSampleTaskTreeBaseItem::zp_rowOfChild(ZSampleTaskTreeBaseItem* item) const
{
    for(int i = 0; i < zv_childList.count(); i++ )
    {
        if(zv_childList.at(i) == item)
        {
            return i;
        }
    }

    return -1;
}
//=================================================
bool ZSampleTaskTreeBaseItem::zp_findItemRow(ZSampleTaskTreeBaseItem* item,
                                             ZSampleTaskTreeBaseItem*& parentItem,
                                             int& row)
{
    if(zv_childList.contains(item))
    {
        row = zv_childList.indexOf(item);
        parentItem = this;
        return true;
    }

    foreach(ZSampleTaskTreeBaseItem* childItem, zv_childList)
    {
        if(childItem->zp_findItemRow(item, parentItem, row))
        {
            return true;
        }
    }

    return false;
}
//=================================================
// END BASE
// ROOT
ZSampleTaskTreeRootItem::ZSampleTaskTreeRootItem()
    : ZSampleTaskTreeBaseItem(ZSampleTaskTreeItemOptions::IT_ROOT, 0)
{

}
//=================================================
ZSampleTaskTreeBaseItem* ZSampleTaskTreeRootItem::zp_createChild(ZSampleTaskTreeItemOptions* options)
{
    ZSampleTaskTreeMeasuringConditionsItem* item = 0;
    if(options->zp_type() == ZSampleTaskTreeItemOptions::IT_MEASURING_CONDITIONS)
    {
        ZSampleTaskTreeMeasuringConditionsItemOptions* mcOptions =
                static_cast<ZSampleTaskTreeMeasuringConditionsItemOptions*>(options);

        item = new ZSampleTaskTreeMeasuringConditionsItem(mcOptions, this);
        connect(item, &ZSampleTaskTreeBaseItem::zg_inquiryIndexForItem,
                this, &ZSampleTaskTreeBaseItem::zg_inquiryIndexForItem);
        connect(item, &ZSampleTaskTreeBaseItem::zg_itemOperation,
                this, &ZSampleTaskTreeBaseItem::zg_itemOperation);

        int row = zv_childList.count();
        emit zg_itemOperation(IOT_ABOUT_TO_INSERT_ROW, QModelIndex(), row, row);
        zv_childList.append(item);
        emit zg_itemOperation(IOT_ROW_INSERTED, QModelIndex(), row, row);
    }

    return item;
}
//=================================================
void ZSampleTaskTreeRootItem::zp_clear()
{
    qDeleteAll(zv_childList);
    zv_childList.clear();
}
//=================================================
// END ROOT
// MEAS. COND.
//=================================================
ZSampleTaskTreeMeasuringConditionsItem::ZSampleTaskTreeMeasuringConditionsItem(ZSampleTaskTreeMeasuringConditionsItemOptions* options,
                                                                               ZSampleTaskTreeBaseItem* parent)
    : ZSampleTaskTreeBaseItem(ZSampleTaskTreeItemOptions::IT_MEASURING_CONDITIONS, parent)
{
    zv_gainFactor = options->zp_gainFactor();
    zv_exposition = options->zp_exposition();
    zh_createItemName();
}
//=================================================
ZSampleTaskTreeBaseItem* ZSampleTaskTreeMeasuringConditionsItem::zp_createChild(ZSampleTaskTreeItemOptions* options)
{
    ZSampleTaskTreeChemicalTaskItem* item = 0;
    if(options->zp_type() == ZSampleTaskTreeItemOptions::IT_CHEMICAL_TASK)
    {
        ZSampleTaskTreeChemicalTaskItemOptions* chtOptions =
                static_cast<ZSampleTaskTreeChemicalTaskItemOptions*>(options);

        QModelIndex parentIndex;
        emit zg_inquiryIndexForItem(this, parentIndex);
        if(!parentIndex.isValid())
        {
            return 0;
        }

        item = new ZSampleTaskTreeChemicalTaskItem(chtOptions, this);
        connect(item, &ZSampleTaskTreeBaseItem::zg_inquiryIndexForItem,
                this, &ZSampleTaskTreeBaseItem::zg_inquiryIndexForItem);
        connect(item, &ZSampleTaskTreeBaseItem::zg_itemOperation,
                this, &ZSampleTaskTreeBaseItem::zg_itemOperation);

        int row = zv_childList.count();

        emit zg_itemOperation(IOT_ABOUT_TO_INSERT_ROW, parentIndex, row, row);
        zv_childList.append(item);
        emit zg_itemOperation(IOT_ROW_INSERTED, parentIndex, row, row);
    }

    return item;
}
//=================================================
int ZSampleTaskTreeMeasuringConditionsItem::zp_gainFactor() const
{
    return zv_gainFactor;
}
//=================================================
int ZSampleTaskTreeMeasuringConditionsItem::zp_exposition() const
{
    return zv_exposition;
}
//=================================================
void ZSampleTaskTreeMeasuringConditionsItem::zh_createItemName()
{
    zv_itemName = tr("G.F.-%1; Expo-%2").arg(QString::number(zv_gainFactor), QString::number(zv_exposition));
}
//=================================================
// END MEAS. COND.
// CHEMICAL TASK
//=================================================
ZSampleTaskTreeChemicalTaskItem::ZSampleTaskTreeChemicalTaskItem(ZSampleTaskTreeChemicalTaskItemOptions* options,
                                                                 ZSampleTaskTreeBaseItem* parent)
    : ZSampleTaskTreeBaseItem(ZSampleTaskTreeItemOptions::IT_CHEMICAL_TASK, parent)
{
    zv_chemicalTaskId = options->zp_chemicalTaskId();
    zv_name = options->zp_chemicalTaskName();
    zv_chemical = options->zp_chemical();

    zh_createItemName();
}
//=================================================
ZSampleTaskTreeBaseItem* ZSampleTaskTreeChemicalTaskItem::zp_createChild(ZSampleTaskTreeItemOptions* options)
{
    ZSampleTaskTreeCalibrationItem* item = 0;
    if(options->zp_type() == ZSampleTaskTreeItemOptions::IT_CALIBRATION)
    {
        ZSampleTaskTreeCalibrationItemOptions* chtOptions =
                static_cast<ZSampleTaskTreeCalibrationItemOptions*>(options);

        QModelIndex parentIndex;
        emit zg_inquiryIndexForItem(this, parentIndex);
        if(!parentIndex.isValid())
        {
            return 0;
        }

        item = new ZSampleTaskTreeCalibrationItem(chtOptions, this);
        connect(item, &ZSampleTaskTreeBaseItem::zg_inquiryIndexForItem,
                this, &ZSampleTaskTreeBaseItem::zg_inquiryIndexForItem);
        connect(item, &ZSampleTaskTreeBaseItem::zg_itemOperation,
                this, &ZSampleTaskTreeBaseItem::zg_itemOperation);

        int row = zv_childList.count();

        emit zg_itemOperation(IOT_ABOUT_TO_INSERT_ROW, parentIndex, row, row);
        zv_childList.append(item);
        emit zg_itemOperation(IOT_ROW_INSERTED, parentIndex, row, row);
    }

    return item;
}
//=================================================
int ZSampleTaskTreeChemicalTaskItem::zp_chemicalTaskId() const
{
    return zv_chemicalTaskId;
}
//=================================================
QString ZSampleTaskTreeChemicalTaskItem::zp_name() const
{
    return zv_name;
}
//=================================================
QString ZSampleTaskTreeChemicalTaskItem::zp_chemical() const
{
    return zv_chemical;
}
//=================================================
void ZSampleTaskTreeChemicalTaskItem::zh_createItemName()
{
    zv_itemName = tr("%1 - %2").arg(zv_chemical, zv_name);
}
//=================================================
// END CHEMICAL TASK
// CALIBRATION
//=================================================
ZSampleTaskTreeCalibrationItem::ZSampleTaskTreeCalibrationItem(ZSampleTaskTreeCalibrationItemOptions* options,
                                                               ZSampleTaskTreeBaseItem* parent)
    :ZSampleTaskTreeBaseItem(ZSampleTaskTreeItemOptions::IT_CALIBRATION, parent)
{
    zv_calibrationName = options->zp_calibrationName();
    zv_minConcentration = options->zp_minConcentration();
    zv_maxConcentration = options->zp_maxConcentration();
    zv_itemName = zv_calibrationName;
}
//=================================================
ZSampleTaskTreeBaseItem* ZSampleTaskTreeCalibrationItem::zp_createChild(ZSampleTaskTreeItemOptions* options)
{
    return 0;
}
//=================================================
QString ZSampleTaskTreeCalibrationItem::zp_calibrationName() const
{
    return zv_calibrationName;
}
//=================================================
double ZSampleTaskTreeCalibrationItem::zp_minConcentration() const
{
    return zv_minConcentration;
}
//=================================================
double ZSampleTaskTreeCalibrationItem::zp_maxConcentration() const
{
    return zv_maxConcentration;
}
//=================================================
