//======================================================
#include "ZChemicalElementPropertyTreeModel.h"
#include "ZChemicalElementPropertyItem.h"
#include "ZChemicalElementPropertyIOManager.h"

#include <QApplication>
#include <QDebug>
#include <QMetaType>
#include <QSettings>
//======================================================

//======================================================
ZChemicalElementPropertyTreeModel::ZChemicalElementPropertyTreeModel(QObject *parent)
    : QAbstractItemModel(parent), ZAbstractChemicalElementPropertyRepository()
{
    qRegisterMetaType<ZChemicalElementPropertyItemCPtr>();

    zv_zNumberPropertyName = "Z";
    zv_symbolPropertyName = "Symbol";
    zv_namePropertyName = "Name";
    zv_settingsGroupName = metaObject()->className();
    zv_dirty = false;

    zh_createComponents();
    zh_createConnections();
    zh_loadBasicChemicalElementProperties();
    //QString chemicalElementItemNameTemplate = "Symbol";
    //zh_defineChemicalElementItemNames(chemicalElementItemNameTemplate);
}
//======================================================
ZChemicalElementPropertyTreeModel::~ZChemicalElementPropertyTreeModel()
{
    delete zv_root;
}
//======================================================
Qt::ItemFlags	ZChemicalElementPropertyTreeModel::flags(const QModelIndex & index) const
{
    Qt::ItemFlags flags;
    if(index.isValid())
    {
        flags |= Qt::ItemIsEnabled
                | Qt::ItemIsSelectable;

        if(index.column() != 0 || zh_itemForIndex(index.parent()) != zv_root)
        {
            flags |= Qt::ItemIsEditable;
        }

    }
    return flags;
}
//======================================================
QModelIndex ZChemicalElementPropertyTreeModel::index(int row, int column, const QModelIndex &parent) const
{
    if(!zv_root || row < 0 || row >= rowCount(parent) || column < 0 || column >= columnCount()
            /*|| (parent.isValid() && column != 0)*/)
    {
        return QModelIndex();
    }

    ZChemicalElementPropertyItem* parentItem = zh_itemForIndex(parent);
    if(!parentItem)
    {
        return QModelIndex();
    }

    ZChemicalElementPropertyItem* item = parentItem->zp_childAtRow(row);
    if(!item)
    {
        return QModelIndex();
    }
    return createIndex(row, column, item);
}
//======================================================
int ZChemicalElementPropertyTreeModel::columnCount(const QModelIndex & parent) const
{
    if(parent.isValid() && parent.column() != 0)
    {
        return 0;
    }

    ZChemicalElementPropertyItem* parentItem = zh_itemForIndex(parent);

    if(!parentItem)
    {
        return 0;
    }

    if(parentItem == zv_root)
    {
        return zh_propertyNameCount(zv_propertyNames) + 1;
    }

    return 0;
}
//======================================================
int	 ZChemicalElementPropertyTreeModel::rowCount(const QModelIndex & parent) const
{
    if(parent.isValid() && parent.column() != 0)
    {
        return 0;
    }

    ZChemicalElementPropertyItem* parentItem = zh_itemForIndex(parent);
    if(!parentItem)
    {
        return 0;
    }

    return parentItem->zp_childCount();
}
//======================================================
QVariant ZChemicalElementPropertyTreeModel::data(const QModelIndex & index, int role) const
{
    if(!index.isValid())
    {
        return QVariant();
    }

    ZChemicalElementPropertyItem* item = zh_itemForIndex(index);
    if(!item)
    {
        return QVariant();
    }

    if(role == PropertyCountRole)
    {
        // for any column
        return QVariant(item->zp_chemicalElementPropertyCount());
    }

    if(role == PropertyFirstColumnRole)
    {
        int column;
        if(!zh_firstPropertyColumnForItem(item, column))
        {
            return QVariant();
        }

        // increase column by one item tree column
        column++;
        if(column < 0 || column >= columnCount())
        {
            return QVariant();
        }

        return QVariant(column);
    }

    if(role == PropertyNameRole)
    {
        // colunm - number of property
        return item->zp_chemicalElementProperty(index.column()).first;
    }

    if(role == PropertyValueRole)
    {
        // colunm - number of property
        return item->zp_chemicalElementProperty(index.column()).second;
    }

    // common columns count
    if(role == ZNumberRole)
    {
        if(index.column() == 0)
        {
            return QVariant(item->zp_chemicalElementPropertyValue(zv_zNumberPropertyName));
        }
    }

    if(role == SymbolRole)
    {
        if(index.column() == 0)
        {
            return QVariant(item->zp_chemicalElementPropertyValue(zv_symbolPropertyName));
        }
    }

    if(role == NameRole)
    {
        if(index.column() == 0)
        {
            return QVariant(item->zp_chemicalElementPropertyValue(zv_namePropertyName));
        }
    }

    if(role == Qt::DisplayRole || role == Qt::EditRole)
    {
        if(index.column() == 0)
        {
            if(item->zp_parent() == zv_root /*&& !item->zp_chemicalElementPropertyValue(zv_symbolPropertyName).isEmpty()*/)
            {
                return QVariant(item->zp_chemicalElementPropertyValue(zv_symbolPropertyName));
            }

            return QVariant(item->zp_name());
        }

        QVariant vData = headerData(index.column(), Qt::Horizontal, Qt::DisplayRole);
        if(!vData.isValid() || !vData.canConvert<QString>())
        {
            return QVariant();
        }

        return QVariant(item->zp_chemicalElementPropertyValue(vData.toString()));
    }

    return QVariant();
}
//======================================================
bool ZChemicalElementPropertyTreeModel::setData(const QModelIndex & index, const QVariant & value, int role)
{
    if(!index.isValid() || index.row() < 0 || index.row() >= rowCount(index.parent())
            || index.column() < 0 || index.column() >= columnCount() || !value.isValid()
            || !value.canConvert<QString>())
    {
        return false;
    }

    ZChemicalElementPropertyItem* item = zh_itemForIndex(index);
    if(!item || item == zv_root)
    {
        return false;
    }

    if(role == Qt::EditRole)
    {
        if(index.column() == 0)
        {
            // section name
            if(item->zp_parent() == zv_root)
            {
                return false;
            }

            item->zp_setName(value.toString());
        }
        else
        {
            // property
            qDebug() << "EDIT PROPERTY column" << index.column();
            // get property name
            QVariant vData = headerData(index.column(), Qt::Horizontal, Qt::DisplayRole);
            if(!vData.isValid() || !vData.canConvert<QString>())
            {
                return false;
            }

            QString propertyName = vData.toString();
            item->zp_setChemicalElementProperty(propertyName, value.toString());
        }

        //emit dataChanged(index, index);
        zv_dirty = true;
        return true;
    }

    if(role == PropertyNameRole)
    {
        // colunm - number of property
        if(item->zp_setChemicalElementPropertyName(index.column(), value.toString()))
        {
            zv_dirty = true;
            return true;
        }

        return false;
    }

    if(role == PropertyValueRole)
    {
        // colunm - number of property
        if(item->zp_setChemicalElementPropertyValue(index.column(), value.toString()))
        {
            zv_dirty = true;
            return true;
        }

        return false;
    }

    return false;
}
//======================================================
QVariant ZChemicalElementPropertyTreeModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if(orientation == Qt::Horizontal)
    {
        if(role == Qt::DisplayRole)
        {
            if(section == 0)
            {
                return QVariant();
            }

            int levelNameIndex = section - 1;
            foreach(QStringList levelPropertyNameList, zv_propertyNames)
            {
                if(levelPropertyNameList.count() <= levelNameIndex)
                {
                    levelNameIndex -= levelPropertyNameList.count();
                    continue;
                }

                return QVariant(levelPropertyNameList.at(levelNameIndex));
            }
        }
    }

    return QVariant();
}
//======================================================
QModelIndex ZChemicalElementPropertyTreeModel::parent(const QModelIndex& child) const
{
    if(!child.isValid())
    {
        return QModelIndex();
    }

    ZChemicalElementPropertyItem* childItem = zh_itemForIndex(child);
    if(!childItem || childItem == zv_root)
    {
        return QModelIndex();
    }

    ZChemicalElementPropertyItem* parentItem = childItem->zp_parent();
    if(!parentItem || parentItem == zv_root)
    {
        return QModelIndex();
    }

    ZChemicalElementPropertyItem* grandParentItem = parentItem->zp_parent();
    int parentRow = grandParentItem->zp_rowOfChildItem(parentItem);
    return createIndex(parentRow, 0, parentItem);
}
//======================================================
bool ZChemicalElementPropertyTreeModel::hasChildren(const QModelIndex &parent) const
{
    ZChemicalElementPropertyItem* item = zh_itemForIndex(parent);
    if(!item)
    {
        return false;
    }

    return item->zp_hasCildren();
}
//======================================================
void ZChemicalElementPropertyTreeModel::zh_createComponents()
{
    // create root
    zv_root = new ZChemicalElementPropertyItem(nullptr);
    QString defaultName;
    // append 118 chemical Elements to the root
    for(int z = 1; z <= zv_chemicalElementCount; z++)
    {
        defaultName = QString::number(z);
        ZChemicalElementPropertyItem* item = new ZChemicalElementPropertyItem(defaultName, this);
        zv_root->zp_addChild(item);
    }
}
//======================================================
void ZChemicalElementPropertyTreeModel::zh_createConnections()
{
    connect(zv_root, &ZChemicalElementPropertyItem::zg_itemInserted,
            this, &ZChemicalElementPropertyTreeModel::zh_onItemInsertion);
    connect(zv_root, &ZChemicalElementPropertyItem::zg_itemAboutToBeInserted,
            this, &ZChemicalElementPropertyTreeModel::zh_onItemAboutToBeInserted);

    connect(zv_root, &ZChemicalElementPropertyItem::zg_requestItemRemovingPermission,
            this, &ZChemicalElementPropertyTreeModel::zh_itemRemovingPermission);
    connect(zv_root, &ZChemicalElementPropertyItem::zg_itemAboutToBeRemoved,
            this, &ZChemicalElementPropertyTreeModel::zh_notifyItemAboutToBeRemoved);
    connect(zv_root, &ZChemicalElementPropertyItem::zg_itemRemoved,
            this, &ZChemicalElementPropertyTreeModel::zh_notifyItemRemoved);
    connect(zv_root, &ZChemicalElementPropertyItem::zg_itemIsChanged,
            this, &ZChemicalElementPropertyTreeModel::zh_notifyItemIsChanged);
    connect(zv_root, &ZChemicalElementPropertyItem::zg_itemPropertyNameIsChanged,
            this, &ZChemicalElementPropertyTreeModel::zh_notifyItemPropertyNameIsChanged);
    connect(zv_root, &ZChemicalElementPropertyItem::zg_itemPropertyValueIsChanged,
            this, &ZChemicalElementPropertyTreeModel::zh_notifyItemPropertyValueIsChanged);

}
//======================================================
void ZChemicalElementPropertyTreeModel::zh_onItemAboutToBeInserted(int row, const ZChemicalElementPropertyItem* parentItem)
{
    QModelIndex parentIndex = zh_indexForItem(parentItem);
    beginInsertRows(parentIndex, row, row);
}
//======================================================
void ZChemicalElementPropertyTreeModel::zh_onItemInsertion(const ZChemicalElementPropertyItem* item)
{
    endInsertRows();
}
//======================================================
void ZChemicalElementPropertyTreeModel::zh_loadBasicChemicalElementProperties()
{
    // ListOfElementProperties-init.csv
    beginResetModel();
    ZChemicalElementPropertyIOManager chemicalElementPropertyLoader;
    QString propertiesSourceFileName = zp_propertiesSourceFilePath();
    if(!chemicalElementPropertyLoader.zp_extractFromFile(this, propertiesSourceFileName)
            && propertiesSourceFileName != zv_defaultPropertiesSourceFileName)
    {
        zp_clearData();
        chemicalElementPropertyLoader.zp_extractFromFile(this, zv_defaultPropertiesSourceFileName);
    }

    zh_redefinePropertyNameList();
    endResetModel();
    zv_dirty = false;
}
//======================================================
QString ZChemicalElementPropertyTreeModel::zp_propertiesSourceFilePath() const
{
    QSettings settings;
    // TODO basic properties file name from settings
    if(!zh_openDialogSettingsGroup(&settings))
    {
        return zv_defaultPropertiesSourceFileName;
    }

    QVariant vData = settings.value(zv_propertyFilePathSettingsName);
    zh_closeDialogSettingsGroup(&settings);

    if(!vData.isValid() || !vData.canConvert<QString>())
    {
        return zv_defaultPropertiesSourceFileName;
    }

    return vData.toString();
}
//======================================================
bool ZChemicalElementPropertyTreeModel::zp_setPropertiesSourceFileName(const QString& propertiesFileName)
{
    QSettings settings;
    // TODO basic properties file name from settings
    if(!zh_openDialogSettingsGroup(&settings))
    {
        return false;
    }

    settings.setValue(zv_propertyFilePathSettingsName, QVariant(propertiesFileName));

    zh_closeDialogSettingsGroup(&settings);
    return true;
}
//======================================================
bool ZChemicalElementPropertyTreeModel::zp_loadDataFromFile(const QString& filePath)
{
    zv_errorStringList.clear();
    ZChemicalElementPropertyIOManager chemicalElementPropertyLoader;
    if(!chemicalElementPropertyLoader.zp_extractFromFile(this, filePath))
    {
        // TODO error report
        zv_errorStringList.append(chemicalElementPropertyLoader.zp_lastErrors());
        zh_loadBasicChemicalElementProperties();
        return false;
    }

    emit zg_propertiesSourceFilePathChanged(filePath);
    zv_dirty = false;
    return true;
}
//======================================================
QModelIndex ZChemicalElementPropertyTreeModel::zp_indexForSection(int ZNumber, const QStringList& sectionBranch) const
{
    ZChemicalElementPropertyItem* item = zh_itemForZNumber(ZNumber);
    if(!item)
    {
        return QModelIndex();
    }

    // branch is empty => add properties to chemical elemnt item
    if(sectionBranch.isEmpty())
    {
        return zh_indexForItem(item);
    }

    ZChemicalElementPropertyItem* child = item->zp_childNamed(sectionBranch);
    if(!child)
    {
        return QModelIndex();
    }

    return zh_indexForItem(child);
}
//======================================================
bool ZChemicalElementPropertyTreeModel::zp_setChemicalElementProperty(int ZNumber,
                                                                      const QString& key,
                                                                      const QString& value)
{
    //beginResetModel();
    bool res = zh_setCemicalElementProperty(ZNumber,
                                            key,
                                            value);
    zh_redefinePropertyNameList();
    zv_dirty = true;

    //endResetModel();
    return res;
}
//======================================================
bool ZChemicalElementPropertyTreeModel::zp_setChemicalElementProperties(int ZNumber,
                                                                        const QStringList &propertySectionBranch,
                                                                        const PropertyList& propertyList)
{
    //beginResetModel();
    bool res = zh_setCemicalElementProperties(ZNumber,
                                              propertySectionBranch,
                                              propertyList);
    zh_redefinePropertyNameList();
    zv_dirty = true;

    //endResetModel();
    return res;
}
//======================================================
bool ZChemicalElementPropertyTreeModel::zp_chemicalElementPropertySectionBranch(const QModelIndex& index,
                                                                                QStringList& branch)
{
    if(!index.isValid())
    {
        return false;
    }

    ZChemicalElementPropertyItem* item = zh_itemForIndex(index);
    if(!item || item == zv_root)
    {
        return false;
    }

    item->zp_chemicalElementPropertySectionBranch(branch);
    return !branch.isEmpty();
}
//======================================================
bool ZChemicalElementPropertyTreeModel::zp_removeChemicalElementProperty(int ZNumber,
                                                                         const QStringList& propertySectionBranch,
                                                                         int propertyIndex)
{
    ZChemicalElementPropertyItem* item = zh_itemForZNumber(ZNumber);
    if(!item)
    {
        zv_errorMsg = tr("Cannot get model item for Z number %1!").arg(QString::number(ZNumber));
        return false;
    }

    // branch is empty => add properties to chemical elemnt item
    if(propertySectionBranch.isEmpty())
    {
        if(item->zp_removeChemicalElementProperty(propertyIndex))
        {
            zv_dirty = true;
            return true;
        }
    }

    ZChemicalElementPropertyItem* child = item->zp_childNamed(propertySectionBranch);
    if(!child)
    {
        QString currentSectionBranchString;
        for(int s = 0; s < propertySectionBranch.count(); s++)
        {
            currentSectionBranchString += propertySectionBranch.at(s);
            if(s < propertySectionBranch.count() - 1)
            {
                currentSectionBranchString += "/";
            }
        }
        zv_errorMsg = tr("Cannot get model item \"%1\" for Z number %2!").arg(currentSectionBranchString,
                                                                              QString::number(ZNumber));
        return false;
    }

    if(item->zp_removeChemicalElementProperty(propertyIndex))
    {
        zv_dirty = true;
        return true;
    }

    return false;
}
//======================================================
bool ZChemicalElementPropertyTreeModel::zp_chemicalElementProperties(int ZNumber,
                                                                     const QStringList& propertySectionBranch,
                                                                     PropertyList& propertyList)
{
    propertyList.clear();
    ZChemicalElementPropertyItem* item = zh_itemForZNumber(ZNumber);
    if(!item)
    {
        zv_errorMsg = tr("Cannot get model item for Z number %1!").arg(QString::number(ZNumber));
        return false;
    }

    // branch is empty => add properties to chemical elemnt item
    if(propertySectionBranch.isEmpty())
    {
        propertyList = item->zp_chemicalElementPropertyList();
        return true;
    }

    ZChemicalElementPropertyItem* child = item->zp_childNamed(propertySectionBranch);
    if(!child)
    {
        QString currentSectionBranchString;
        for(int s = 0; s < propertySectionBranch.count(); s++)
        {
            currentSectionBranchString += propertySectionBranch.at(s);
            if(s < propertySectionBranch.count() - 1)
            {
                currentSectionBranchString += "/";
            }
        }
        zv_errorMsg = tr("Cannot get model item \"%1\" for Z number %2!").arg(currentSectionBranchString,
                                                                              QString::number(ZNumber));
        return false;
    }

    propertyList = child->zp_chemicalElementPropertyList();
    return true;
}
//======================================================
bool ZChemicalElementPropertyTreeModel::zp_chemicalElementPropertySectionList(int ZNumber,
                                                                              const QStringList& propertySectionBranch,
                                                                              QStringList& sectionList)
{
    sectionList.clear();
    ZChemicalElementPropertyItem* item = zh_itemForZNumber(ZNumber);
    if(!item)
    {
        zv_errorMsg = tr("Cannot get model item for Z number %1!").arg(QString::number(ZNumber));
        return false;
    }

    // branch is empty => add properties to chemical elemnt item
    if(propertySectionBranch.isEmpty())
    {
        sectionList = item->zp_chemicalElementPropertySectionList();
        return true;
    }

    ZChemicalElementPropertyItem* child = item->zp_childNamed(propertySectionBranch);
    if(!child)
    {
        QString currentSectionBranchString;
        for(int s = 0; s < propertySectionBranch.count(); s++)
        {
            currentSectionBranchString += propertySectionBranch.at(s);
            if(s < propertySectionBranch.count() - 1)
            {
                currentSectionBranchString += "/";
            }
        }
        zv_errorMsg = tr("Cannot get model item \"%1\" for Z number %2!").arg(currentSectionBranchString,
                                                                              QString::number(ZNumber));
        return false;
    }

    sectionList = child->zp_chemicalElementPropertySectionList();
    return true;
}
//======================================================
void ZChemicalElementPropertyTreeModel::zp_clearData()
{
    beginResetModel();
    for(int i = 0; i < zv_root->zp_childCount(); i++)
    {
        zv_root->zp_childAtRow(i)->zp_clear();
    }
    endResetModel();
}
//======================================================
QStringList ZChemicalElementPropertyTreeModel::zp_lastErrorStringList() const
{
    return zv_errorStringList;
}
//======================================================
bool ZChemicalElementPropertyTreeModel::zp_addChemicalElementPropertyBranch(int ZNumber,
                                                                            const QStringList& propertyGroupNameBranch)
{
    // beginResetModel();
    bool res = zh_addCemicalElementPropertyBranch(ZNumber,
                                                  propertyGroupNameBranch);
    zh_redefinePropertyNameList();
    zv_dirty = true;
    // endResetModel();
    return res;
}
//======================================================
void ZChemicalElementPropertyTreeModel::zh_defineChemicalElementItemNames(const QString& nameTemplate,
                                                                          QString separator)
{
    if(!zv_root)
    {
        return;
    }

    QModelIndex currentIndex;
    for(int i = 0; i < zv_root->zp_childCount(); i++)
    {
        if(zv_root->zp_childAtRow(i)->zp_createName(nameTemplate, separator))
        {
            currentIndex = index(i, 0, QModelIndex());
            emit dataChanged(currentIndex, currentIndex);
        }
    }
}
//======================================================
bool ZChemicalElementPropertyTreeModel::zp_zNumberForIndex(const QModelIndex& index, int& zNumber) const
{
    ZChemicalElementPropertyItem* item = zh_itemForIndex(index);
    if(!item)
    {
        return false;
    }

    zNumber = item->zp_zNumber(item);
    return zNumber > 0 && zNumber <= zv_chemicalElementCount;
}
//======================================================
QString ZChemicalElementPropertyTreeModel::zp_zNumberPropertyName() const
{
    return zv_zNumberPropertyName;
}
//======================================================
void ZChemicalElementPropertyTreeModel::zp_setZNumberPropertyName(const QString& name)
{
    zv_zNumberPropertyName = name;
}
//======================================================
QString ZChemicalElementPropertyTreeModel::zp_symbolPropertyName() const
{
    return zv_symbolPropertyName;
}
//======================================================
void ZChemicalElementPropertyTreeModel::zp_setSymbolPropertyName(const QString& name)
{
    zv_symbolPropertyName = name;
}
//======================================================
QString ZChemicalElementPropertyTreeModel::zp_namePropertyName() const
{
    return zv_namePropertyName;
}
//======================================================
void ZChemicalElementPropertyTreeModel::zp_setNamePropertyName(const QString& name)
{
    zv_namePropertyName = name;
}
//======================================================
int ZChemicalElementPropertyTreeModel::zp_chemicalElementCount() const
{
    return zv_chemicalElementCount;
}
//======================================================
bool ZChemicalElementPropertyTreeModel::zp_isRemovable(const QModelIndex& index) const
{
    if(!index.isValid())
    {
        // root
        return false;
    }

    if(index.column() != 0)
    {
        return true;
    }

    ZChemicalElementPropertyItem* item = zh_itemForIndex(index);
    if(!item)
    {
        return false;
    }

    int levelNumber = item->zh_elementLevelNumber();
    return levelNumber > 1;
}
//======================================================
void ZChemicalElementPropertyTreeModel::zp_removeItems(const QModelIndexList& indexList)
{
    // create list of removed items
    QList<ZChemicalElementPropertyItem*> removedItemList;
    ZChemicalElementPropertyItem* item;
    foreach(QModelIndex index, indexList)
    {
        item = zh_itemForIndex(index);
        if(!item)
        {
            continue;
        }

        removedItemList.append(item);
    }

    QList<ZChemicalElementPropertyItem*> childItemList;
    while(!removedItemList.isEmpty())
    {
        childItemList.clear();
        item = removedItemList.takeFirst();
        item->zp_totalDescendantList(childItemList);

        // remove children from removeed item list
        while(!childItemList.isEmpty())
        {
            removedItemList.removeAll(childItemList.takeFirst());
        }

        item->zp_initRemoving();
    }
}
//======================================================
bool ZChemicalElementPropertyTreeModel::zp_isDirty() const
{
    return zv_dirty;
}
//======================================================
bool ZChemicalElementPropertyTreeModel::zp_saveDataToXMLFile(const QString& filePath)
{
    ZChemicalElementPropertyIOManager chemicalElementPropertyLoader;
    if(!chemicalElementPropertyLoader.zp_saveToXML(this, filePath))
    {
        return false;
    }

    zg_propertiesSourceFilePathChanged(filePath);
    zv_dirty = false;
    return true;;
}
//======================================================
void ZChemicalElementPropertyTreeModel::zh_notifyItemAboutToBeRemoved(const ZChemicalElementPropertyItem* item)
{
    if(!item)
    {
        return;
    }

    QModelIndex parentIndex = zh_indexForItem(item->zp_parent());
    int itemRow = item->zp_parent()->zp_rowOfChildItem(item);

    if(parentIndex.isValid() && itemRow >= 0)
    {
        beginRemoveRows(parentIndex, itemRow, itemRow);
    }
}
//======================================================
void ZChemicalElementPropertyTreeModel::zh_notifyItemRemoved(const ZChemicalElementPropertyItem *item)
{
    endRemoveRows();
}
//======================================================
void ZChemicalElementPropertyTreeModel::zh_notifyItemIsChanged(const ZChemicalElementPropertyItem *item)
{
    if(!item || item == zv_root || !item->zp_parent() )
    {
        return;
    }

    QModelIndex parent = zh_indexForItem(item->zp_parent());
    int row = item->zp_parent()->zp_rowOfChildItem(item);
    QModelIndex topLeft = index(row, 0, parent);
    QModelIndex bottomRight =  index(row, columnCount() - 1, parent);

    //zh_redefinePropertyNameList();
    emit dataChanged(topLeft, bottomRight);
}
//======================================================
void ZChemicalElementPropertyTreeModel::zh_notifyItemPropertyNameIsChanged(const ZChemicalElementPropertyItem *item)
{
    zh_redefinePropertyNameList();
}
//======================================================
void ZChemicalElementPropertyTreeModel::zh_notifyItemPropertyValueIsChanged(const ZChemicalElementPropertyItem *item)
{
    zh_redefinePropertyNameList();
}
//======================================================
void ZChemicalElementPropertyTreeModel::zh_itemRemovingPermission(const ZChemicalElementPropertyItem* item,
                                                                  bool& permissionToRemove)
{
    if(item == nullptr)
    {
        permissionToRemove = false;
        return;
    }

    QModelIndex itemIndex = zh_indexForItem(item);
    permissionToRemove = zp_isRemovable(itemIndex);
}
//======================================================
bool ZChemicalElementPropertyTreeModel::zh_setCemicalElementProperty(int ZNumber,
                                                                     const QString& key,
                                                                     const QString& value)
{
    ZChemicalElementPropertyItem* item = zh_itemForZNumber(ZNumber);
    if(!item)
    {
        zv_errorMsg = tr("Cannot get model item for Z number %1!").arg(QString::number(ZNumber));
        return false;
    }

    item->zp_setChemicalElementProperty(key, value);
    return true;
}
//======================================================
bool ZChemicalElementPropertyTreeModel::zh_setCemicalElementProperties(int ZNumber,
                                                                       const QStringList& propertySectionBranch,
                                                                       const PropertyList& propertyList)
{
    ZChemicalElementPropertyItem* item = zh_itemForZNumber(ZNumber);
    if(!item)
    {
        zv_errorMsg = tr("Cannot get model item for Z number %1!").arg(QString::number(ZNumber));
        return false;
    }

    // branch is empty => add properties to chemical elemnt item
    if(propertySectionBranch.isEmpty())
    {
        item->zp_setChemicalElementPropertyList(propertyList);
        return true;
    }

    ZChemicalElementPropertyItem* child = item->zp_childNamed(propertySectionBranch);
    if(!child)
    {
        QString currentSectionBranchString;
        for(int s = 0; s < propertySectionBranch.count(); s++)
        {
            currentSectionBranchString += propertySectionBranch.at(s);
            if(s < propertySectionBranch.count() - 1)
            {
                currentSectionBranchString += "/";
            }
        }
        zv_errorMsg = tr("Cannot get model item \"%1\" for Z number %2!").arg(currentSectionBranchString,
                                                                              QString::number(ZNumber));
        return false;
    }

    child->zp_setChemicalElementPropertyList(propertyList);
    return true;
}
//======================================================
bool ZChemicalElementPropertyTreeModel::zh_addCemicalElementPropertyBranch(int ZNumber,
                                                                           const QStringList& propertySectionBranch)
{
    ZChemicalElementPropertyItem* item = zh_itemForZNumber(ZNumber);
    if(!item)
    {
        zv_errorMsg = tr("Cannot get model item for Z number %1!").arg(QString::number(ZNumber));
        return false;
    }

    ZChemicalElementPropertyItem* child = item->zp_childNamed(propertySectionBranch);
    return child != nullptr;
}
//======================================================
ZChemicalElementPropertyItem* ZChemicalElementPropertyTreeModel::zh_itemForIndex(const QModelIndex& index) const
{
    if(!index.isValid())
    {
        return zv_root;
    }

    ZChemicalElementPropertyItem* item = static_cast<ZChemicalElementPropertyItem*>(index.internalPointer());
    if(!item)
    {
        return zv_root;
    }

    return item;
}
//======================================================
QModelIndex ZChemicalElementPropertyTreeModel::zh_indexForItem(const ZChemicalElementPropertyItem* item) const
{
    if(item == nullptr)
    {
        return QModelIndex();
    }

    ZChemicalElementPropertyItem* parentItem = item->zp_parent();
    if(parentItem == nullptr)
    {
        // root
        return QModelIndex();
    }

    int itemRow = parentItem->zp_rowOfChildItem(item);
    if(itemRow < 0 || itemRow >= parentItem->zp_childCount())
    {
        return QModelIndex();
    }

    int column = 0;

    return createIndex(itemRow, column, const_cast<ZChemicalElementPropertyItem*>(item));

}
//======================================================
ZChemicalElementPropertyItem* ZChemicalElementPropertyTreeModel::zh_itemForZNumber(int ZNumber) const
{
    if(ZNumber < 1)
    {
        return zv_root;
    }

    if(!zv_root || zv_root->zp_childCount() < ZNumber)
    {
        return nullptr;
    }

    return zv_root->zp_childAtRow(ZNumber - 1);
}
//======================================================
void ZChemicalElementPropertyTreeModel::zh_redefinePropertyNameList()
{
    if(columnCount() > 1)
    {
        beginRemoveColumns(QModelIndex(), 1, columnCount()-1);
        zv_propertyNames.clear();
        endRemoveColumns();
    }

    if(!zv_root)
    {
        return;
    }

    PropertyNames propertyNames;
    for(int i = 0; i < zv_root->zp_childCount(); i++)
    {
        zv_root->zp_childAtRow(i)->zp_completeChemicalElementPropertyNameList(propertyNames);
        zv_propertyNames.append(propertyNames);
    }

    int newColumnCount = zh_propertyNameCount(propertyNames);
    beginInsertColumns(QModelIndex(), 1, 1 + newColumnCount);
    zv_propertyNames = propertyNames;
    endInsertColumns();
}
//======================================================
bool ZChemicalElementPropertyTreeModel::zh_firstPropertyColumnForItem(ZChemicalElementPropertyItem* item,
                                                                      int& columnNumber) const
{
    // define element level number (zero level - root)
    int levelNumber = item->zh_elementLevelNumber();

    // get first column for appropriate level from zv_propertyNames
    columnNumber = 0;
    for(int i = 0; i < levelNumber - 1; i++)
    {
        if(i >= zv_propertyNames.count())
        {
            return false;
        }

        columnNumber += zv_propertyNames.at(i).count();
    }

    return true;
}
//======================================================
int ZChemicalElementPropertyTreeModel::zh_propertyNameCount(const PropertyNames& propertyNames) const
{
    if(propertyNames.isEmpty())
    {
        return 0;
    }

    int count = 0;
    // columns for properties
    foreach(QStringList levelPropertyNameList, propertyNames)
    {
        count += levelPropertyNameList.count();
    }
    return count;
}
//======================================================
bool ZChemicalElementPropertyTreeModel::zh_openDialogSettingsGroup(QSettings* settings) const
{
    if(settings == nullptr)
    {
        return false;
    }
    // open version group
    QString verString = qApp->property("glAppVersion").toString();
    if(!verString.isEmpty())
    {
        settings->beginGroup(verString);
    }

    // open dialog group
    if(!zv_settingsGroupName.isEmpty())
    {
        settings->beginGroup(zv_settingsGroupName);
    }
    return true;
}
//======================================================
bool ZChemicalElementPropertyTreeModel::zh_closeDialogSettingsGroup(QSettings *settings) const
{
    if(settings == nullptr)
    {
        return false;
    }

    // close groups
    while(!settings->group().isEmpty())
    {
        settings->endGroup();
    }

    return true;
}
//======================================================

