//======================================================
#include "ZChemicalElementPropertyItem.h"
//======================================================
ZChemicalElementPropertyItem::ZChemicalElementPropertyItem(QObject* parent)
    : QObject(parent)
{
    zv_parentItem = qobject_cast<ZChemicalElementPropertyItem*>(parent);
}
//======================================================
ZChemicalElementPropertyItem::ZChemicalElementPropertyItem(const QString& name,
                                                           QObject* parent) : QObject(parent)
{
    zv_parentItem = qobject_cast<ZChemicalElementPropertyItem*>(parent);
    zv_name = name;
}
//======================================================
QString ZChemicalElementPropertyItem::zp_name() const
{
    return zv_name;
}
//======================================================
void ZChemicalElementPropertyItem::zp_setName(const QString& name)
{
    zv_name = name;
    emit zg_itemIsChanged(this);
}
//======================================================
bool ZChemicalElementPropertyItem::zp_createName(const QString& nameTemplate,
                                                 const QString& separator)
{
    QStringList propertyNameList = nameTemplate.split(separator);
    QString newName;
    QString propertyValue;
    for(int i = 0; i < propertyNameList.count(); i++)
    {
        propertyValue = zp_chemicalElementPropertyValue(propertyNameList.at(i));
        if(propertyValue.isEmpty())
        {
            continue;
        }

        newName += propertyValue;
        if(i < propertyNameList.count() - 1)
        {
            newName += separator;
        }
    }

    if(newName.isEmpty() || newName == zv_name)
    {
        return false;
    }

    zv_name = newName;
    emit zg_itemIsChanged(this);
    return true;
}
//======================================================
void ZChemicalElementPropertyItem::zp_addChild(ZChemicalElementPropertyItem* child)
{
    emit zg_itemAboutToBeInserted(zp_childCount(), this);

    child->zv_parentItem = this;
    zv_childItemList.append(child);
    connect(child, &ZChemicalElementPropertyItem::zg_itemInserted,
            this, &ZChemicalElementPropertyItem::zg_itemInserted);
    connect(child, &ZChemicalElementPropertyItem::zg_itemAboutToBeInserted,
            this, &ZChemicalElementPropertyItem::zg_itemAboutToBeInserted);
    connect(child, &ZChemicalElementPropertyItem::zg_requestItemRemovingPermission,
            this, &ZChemicalElementPropertyItem::zg_requestItemRemovingPermission);
    connect(child, &ZChemicalElementPropertyItem::zg_itemRemoved,
            this, &ZChemicalElementPropertyItem::zg_itemRemoved);
    connect(child, &ZChemicalElementPropertyItem::zg_itemAboutToBeRemoved,
            this, &ZChemicalElementPropertyItem::zg_itemAboutToBeRemoved);
    connect(child, &ZChemicalElementPropertyItem::zg_itemIsChanged,
            this, &ZChemicalElementPropertyItem::zg_itemIsChanged);
    connect(child, &ZChemicalElementPropertyItem::zg_itemPropertyNameIsChanged,
            this, &ZChemicalElementPropertyItem::zg_itemPropertyNameIsChanged);
    connect(child, &ZChemicalElementPropertyItem::zg_itemPropertyValueIsChanged,
            this, &ZChemicalElementPropertyItem::zg_itemPropertyValueIsChanged);


    emit zg_itemInserted(child);
}
//======================================================
ZChemicalElementPropertyItem* ZChemicalElementPropertyItem::zp_createChild(const QString& name)
{
    ZChemicalElementPropertyItem* child = new ZChemicalElementPropertyItem(name, this);
    zp_addChild(child);
    return child;
}
//======================================================
void ZChemicalElementPropertyItem::zp_setChemicalElementProperty(const QString& key, const QString& value)
{
    for(int i = 0; i < zv_chemicalElementPropertyList.count(); i++)
    {
        if(zv_chemicalElementPropertyList.at(i).first == key)
        {
            zv_chemicalElementPropertyList[i].second = value;
            emit zg_itemIsChanged(this);
            return;
        }
    }

    QPair<QString, QString> propertyPair = QPair<QString, QString>(key, value);
    zv_chemicalElementPropertyList.append(propertyPair);
    emit zg_itemIsChanged(this);
}
//======================================================
QList<QPair<QString, QString> > ZChemicalElementPropertyItem::zp_chemicalElementPropertyList() const
{
    return zv_chemicalElementPropertyList;
}
//======================================================
QStringList ZChemicalElementPropertyItem::zp_chemicalElementPropertySectionList() const
{
    QStringList sectionStringList;
    foreach(auto child, zv_childItemList)
    {
        sectionStringList.append(child->zp_name());
    }

    return sectionStringList;
}
//======================================================
bool ZChemicalElementPropertyItem::zp_setChemicalElementPropertyName(int index, const QString& name)
{
    if(index < 0 || index >= zv_chemicalElementPropertyList.count())
    {
        return false;
    }

    zv_chemicalElementPropertyList[index].first = name;
    emit zg_itemPropertyNameIsChanged(this);
    return true;
}
//======================================================
bool ZChemicalElementPropertyItem::zp_setChemicalElementPropertyValue(int index, const QString& value)
{
    if(index < 0 || index >= zv_chemicalElementPropertyList.count())
    {
        return false;
    }

    zv_chemicalElementPropertyList[index].second = value;
    emit zg_itemPropertyValueIsChanged(this);
    return true;
}
//======================================================
QString ZChemicalElementPropertyItem::zp_chemicalElementPropertyValue(const QString& key) const
{
    for(int i = 0; i < zv_chemicalElementPropertyList.count(); i++)
    {
        if(zv_chemicalElementPropertyList.at(i).first == key)
        {
            return zv_chemicalElementPropertyList.at(i).second;
        }
    }

    return QString();
}
//======================================================
QPair<QString, QString> ZChemicalElementPropertyItem::zp_chemicalElementProperty(int index) const
{
    if(index < 0 || index >= zv_chemicalElementPropertyList.count() )
    {
        return QPair<QString, QString>();
    }

    return zv_chemicalElementPropertyList.at(index);
}
//======================================================
void ZChemicalElementPropertyItem::zp_setChemicalElementPropertyList(const QList<QPair<QString, QString> >& propertyList)
{
    foreach(auto& proprtyPair, propertyList)
    {
        zp_setChemicalElementProperty(proprtyPair.first, proprtyPair.second);
    }
}
//======================================================
ZChemicalElementPropertyItem* ZChemicalElementPropertyItem::zp_parent() const
{
    return zv_parentItem;
}
//======================================================
ZChemicalElementPropertyItem* ZChemicalElementPropertyItem::zp_childAtRow(int row) const
{
    if(row < 0 || row >= zv_childItemList.count())
    {
        return nullptr;
    }

    return zv_childItemList.at(row);
}
//======================================================
ZChemicalElementPropertyItem* ZChemicalElementPropertyItem::zp_childNamed(const QStringList& nameBranch)
{
    if(nameBranch.isEmpty())
    {
        return nullptr;
    }
    QStringList newNameBranch = nameBranch;
    QString name = newNameBranch.takeFirst();

    ZChemicalElementPropertyItem* child = zp_childNamed(name);

    if(newNameBranch.isEmpty())
    {
        return child;
    }

    return child->zp_childNamed(newNameBranch);
}
//======================================================
ZChemicalElementPropertyItem* ZChemicalElementPropertyItem::zp_childNamed(const QString& name)
{
    for(int i = 0; i < zv_childItemList.count(); i++)
    {
        if(zv_childItemList.at(i)->zp_name() == name)
        {
            return zv_childItemList.value(i);
        }
    }

    return zp_createChild(name);
}
//======================================================
void ZChemicalElementPropertyItem::zp_chemicalElementPropertySectionBranch(QStringList& branch)
{
    if(!zv_parentItem)
    {
        // root
        return;
    }

    branch.push_front(zv_name);
    zv_parentItem->zp_chemicalElementPropertySectionBranch(branch);
}
//======================================================
int ZChemicalElementPropertyItem::zp_childCount() const
{
    return zv_childItemList.count();
}
//======================================================
bool ZChemicalElementPropertyItem::zp_hasCildren() const
{
    return !zv_childItemList.isEmpty();
}
//======================================================
void ZChemicalElementPropertyItem::zp_removeChildren()
{
    while(!zv_childItemList.isEmpty())
    {
        zp_removeChild(zv_childItemList.last());
    }
}
//======================================================
int ZChemicalElementPropertyItem::zp_rowOfChildItem(const ZChemicalElementPropertyItem* child) const
{
    if(!child)
    {
        return -1;
    }

    return zv_childItemList.indexOf(const_cast<ZChemicalElementPropertyItem*>(child));
}
//======================================================
void ZChemicalElementPropertyItem::zp_completeChemicalElementPropertyNameList(PropertyNames& propertyNames) const
{
    QStringList propertyNameStringList = propertyNames.isEmpty()? QStringList() : propertyNames.takeFirst();

    for(int i = 0; i < zv_chemicalElementPropertyList.count(); i++)
    {
        if(propertyNameStringList.contains(zv_chemicalElementPropertyList.at(i).first))
        {
            continue;
        }

        propertyNameStringList.append(zv_chemicalElementPropertyList.at(i).first);
    }

    foreach(ZChemicalElementPropertyItem* item, zv_childItemList)
    {
        item->zp_completeChemicalElementPropertyNameList(propertyNames);
    }

    propertyNames.push_front(propertyNameStringList);
}
//======================================================
int ZChemicalElementPropertyItem::zp_chemicalElementPropertyCount() const
{
    return zv_chemicalElementPropertyList.count();
}
//======================================================
int ZChemicalElementPropertyItem::zh_elementLevelNumber() const
{
    if(zv_parentItem == nullptr)
    {
        return 0;
    }

    return zv_parentItem->zh_elementLevelNumber() + 1;
}
//======================================================
int ZChemicalElementPropertyItem::zp_zNumber(const ZChemicalElementPropertyItem * callingItem) const
{
    if(zv_parentItem == nullptr)
    {
        // root
        int childRow = zp_rowOfChildItem(callingItem);
        return childRow + 1;
    }

    return zv_parentItem->zp_zNumber(this);
}
//======================================================
void ZChemicalElementPropertyItem::zp_totalDescendantList(QList<ZChemicalElementPropertyItem*>& descendantList) const
{
    descendantList.append(zv_childItemList);
    foreach(ZChemicalElementPropertyItem* childItem, zv_childItemList)
    {
        childItem->zp_totalDescendantList(descendantList);
    }
}
//======================================================
bool ZChemicalElementPropertyItem::zp_initRemoving()
{
    for(int i = zv_childItemList.count()-1; i >= 0; i--)
    {
        zv_childItemList[i]->zp_initRemoving();
    }

    if(!zv_childItemList.isEmpty())
    {
        // some children remain
        return false;
    }

    bool removePermission = false;
    emit zg_requestItemRemovingPermission(this, removePermission);

    if(!removePermission)
    {
        return false;
    }

    // check out from parent list
    if(zv_parentItem)
    {
        zv_parentItem->zp_removeChild(this);
        deleteLater();
    }

    return true;
}
//======================================================
void ZChemicalElementPropertyItem::zp_removeChild(ZChemicalElementPropertyItem* child)
{
    int childRow = zp_rowOfChildItem(child);
    if(childRow < 0 || childRow >= zv_childItemList.count() )
    {
        return;
    }

    emit zg_itemAboutToBeRemoved(child);
    zv_childItemList[childRow]->zv_parentItem = nullptr;
    zv_childItemList.removeAt(childRow);
    emit zg_itemRemoved(child);
}
//======================================================
bool ZChemicalElementPropertyItem::zp_removeChemicalElementProperty(int propertyIndex)
{
    if(propertyIndex < 0 || propertyIndex >= zv_chemicalElementPropertyList.count())
    {
        return false;
    }

    zv_chemicalElementPropertyList.removeAt(propertyIndex);
    emit zg_itemIsChanged(this);
    return true;
}
//======================================================
void ZChemicalElementPropertyItem::zp_removeChemicalElementProperties()
{
    while(!zv_chemicalElementPropertyList.isEmpty())
    {
        zp_removeChemicalElementProperty(zv_chemicalElementPropertyList.count() - 1);
    }
}
//======================================================
void ZChemicalElementPropertyItem::zp_clear()
{
    zp_removeChemicalElementProperties();
    zp_removeChildren();
}
//======================================================

