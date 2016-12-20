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
bool ZSampleTaskTreeRootItem::zp_createChild(ZSampleTaskTreeItemOptions* options)
{
    if(options->zp_type() == ZSampleTaskTreeItemOptions::IT_MEASURING_CONDITIONS)
    {
        ZSampleTaskTreeMeasuringConditionsItemOptions* mcOptions =
                static_cast<ZSampleTaskTreeMeasuringConditionsItemOptions*>(options);

        ZSampleTaskTreeMeasuringConditionsItem* item 
                = new ZSampleTaskTreeMeasuringConditionsItem(mcOptions, this);
        int row = zv_childList.count();
        emit zg_itemOperation(IOT_ABOUT_TO_INSERT_ROW, QModelIndex(), row, row);
        zv_childList.append(item);
        emit zg_itemOperation(IOT_ROW_INSERTED, QModelIndex(), row, row);
    }

    return true;
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
bool ZSampleTaskTreeMeasuringConditionsItem::zp_createChild(ZSampleTaskTreeItemOptions* options)
{

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
