//=================================================================
#include "ZItem.h"
//=================================================================
ZItem::ZItem(ItemType type, QObject* parent) : QObject(parent)
{
    zv_type = type;
}
//=================================================================
ZItem::ItemType ZItem::zp_itemType() const
{
    return zv_type;
}
//=================================================================
ZItem* ZItem::zp_parentItem() const
{
    ZItem* parentItem = qobject_cast<ZItem*>(parent());
    return parentItem;
}
//=================================================================
ZItem* ZItem::zp_childAt(int index) const
{
    if(zv_childList.isEmpty() || index < 0 || index >= zv_childList.count())
    {
        return 0;
    }

    return zv_childList.at(index);
}
//=================================================================
int ZItem::zp_childCount() const
{
    return  zv_childList.count();
}
//=================================================================
bool ZItem::zp_hasChildren() const
{
    return !zv_childList.isEmpty();
}
//=================================================================
int ZItem::zp_childItemIndex(ZItem* item)
{
    if(item == 0)
    {
        return -1;
    }

    for(int i = 0; i < zv_childList.count(); i++)
    {
        if(zv_childList.at(i) == item)
        {
            return i;
        }
    }

    return -1;
}
//=================================================================
