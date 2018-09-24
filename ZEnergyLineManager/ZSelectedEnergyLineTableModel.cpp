//=============================================================
#include "ZSelectedEnergyLineTableModel.h"
#include "ZGeneral.h"

#include <algorithm>
#include <QDebug>
//=============================================================
//=============================================================
ZSelectedEnergyLineTableModel::ZSelectedEnergyLineTableModel(QObject *parent)
    : QAbstractTableModel(parent)
{

}
//=============================================================
ZSelectedEnergyLineTableModel::~ZSelectedEnergyLineTableModel()
{
    qDeleteAll(zv_itemMap.values());
}
//=============================================================
Qt::ItemFlags	ZSelectedEnergyLineTableModel::flags(const QModelIndex & index) const
{
    Qt::ItemFlags	flags;
    flags |= Qt::ItemIsEnabled;

    // flags |= Qt::ItemIsEditable;

    return flags;
}
//=============================================================
int	 ZSelectedEnergyLineTableModel::columnCount(const QModelIndex & parent) const
{
    return zv_columnHederList.count() + 1;
}
//=============================================================
int	 ZSelectedEnergyLineTableModel::rowCount(const QModelIndex & parent) const
{
    return zv_itemMap.count();
}
//=============================================================
QVariant ZSelectedEnergyLineTableModel::data(const QModelIndex & index, int role) const
{
    if(!index.isValid() || index.row() < 0 || index.row() >= rowCount() || index.column() < 0
            || index.column() >= columnCount())
    {
        return QVariant();
    }

    int ZNumber = zv_itemMap.keys().at(index.row());
    QString energyLineName = zv_columnHederList.value(index.column() - 1, QString());
    if(role == Qt::DisplayRole)
    {
        if(index.column() == 0)
        {
            QString symbolString = zv_itemMap.value(ZNumber)->zp_symbol();
            return QVariant(symbolString);
        }
        else
        {
            QString energyLineValue;
            if(zv_itemMap.value(ZNumber)->zp_energyLineValue(energyLineName, energyLineValue))
            {
                return QVariant(energyLineValue);
            }
        }
    }

    if(role == NS_DataRole::VisibleRole)
    {
        if(index.column() == 0)
        {
            return QVariant();
        }
        else
        {
            bool visible;
            if(!zv_itemMap.value(ZNumber)->zp_isEnergyLineVisible(energyLineName, visible))
            {
                return QVariant();
            }

            return QVariant(visible);
        }
    }

    if(role == Qt::DecorationRole)
    {
        if(index.column() == 0)
        {
            return QVariant();
        }
        else
        {
            QVariant vData = index.data(Qt::DisplayRole);
            if(!vData.isValid() || vData.isNull() || !vData.canConvert<QString>() ||
                    vData.toString().isEmpty())
            {
                return QVariant();
            }

            QColor energyLineColor;
            if(zv_itemMap.value(ZNumber)->zp_energyLineColor(energyLineName, energyLineColor))
            {
                return QVariant(energyLineColor);
            }

            return QVariant(QColor(Qt::black));
        }
    }

    return QVariant();
}
//=============================================================
bool ZSelectedEnergyLineTableModel::setData(const QModelIndex & index, const QVariant & value, int role)
{
    if(!index.isValid() || index.row() < 0 || index.row() >= rowCount() || index.column() < 1
            || index.column() >= columnCount() || !value.isValid() || !value.canConvert<bool>())
    {
        return false;
    }

    if(role ==  NS_DataRole::VisibleRole)
    {
        int ZNumber = zv_itemMap.keys().at(index.row());
        QString energyLineName = zv_columnHederList.value(index.column() - 1, QString());
        return zv_itemMap.value(ZNumber)->zp_setEnergyLineVisible(energyLineName, value.toBool());
    }

    return false;
}
//=============================================================
QVariant ZSelectedEnergyLineTableModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if(role == Qt::DisplayRole)
    {
        if(orientation == Qt::Vertical)
        {
            return QVariant(QString::number(section+1));
        }
        else
        {
            if(section == 0)
            {
                return QVariant(tr("Element"));
            }

            if(section - 1 < zv_columnHederList.count())
            {
                return QVariant(zv_columnHederList.at(section - 1));
            }
        }
    }

    return QVariant();
}
//=============================================================
bool ZSelectedEnergyLineTableModel::setHeaderData(int section, Qt::Orientation orientation,
                                                  const QVariant & data, int role)
{
    return false;
}
//=============================================================
void ZSelectedEnergyLineTableModel::zp_onSelectedChemicalElementChange(int ZNumber,
                                                                       bool selected)
{
    if(selected)
    {
        if(!zv_itemMap.keys().contains(ZNumber))
        {
            int row = zh_findRowToInsert(ZNumber);
            QString symbol;
            emit zg_requestChemicalElementSymbol(ZNumber, symbol);

            beginInsertRows(QModelIndex(), row, row);
            ZEnergyLineSetItem* item = new ZEnergyLineSetItem(ZNumber,
                                                              symbol);
            connect(item, &ZEnergyLineSetItem::zg_energyLineOperation,
                    this, &ZSelectedEnergyLineTableModel::zg_energyLineOperation);

            zv_itemMap.insert(ZNumber, item);
            zh_loadEnergyLinesToItem(item);

            endInsertRows();
        }
    }
    else
    {
        if(zv_itemMap.keys().contains(ZNumber))
        {
            int row = zv_itemMap.keys().indexOf(ZNumber);
            beginRemoveRows(QModelIndex(), row, row);

            ZEnergyLineSetItem* item  = zv_itemMap.take(ZNumber);
            if(item)
            {
                delete item;
            }

            endRemoveRows();
        }
    }

    zh_updateColumns();
}
//=============================================================
void ZSelectedEnergyLineTableModel::zh_loadEnergyLinesToItem(ZEnergyLineSetItem* item)

{
    // energy values
    PropertyList propertyList;
    emit zg_requestEnergyLinesForZNumber(item->zp_ZNumber(), propertyList);

    item->zp_createEnergyLines(propertyList);

    // relative intensity
    propertyList.clear();
    emit zg_requestEnergyLineRelativeIntensityForZNumber(item->zp_ZNumber(), propertyList);
    item->zp_loadRelativeIntensity(propertyList);
}
//=============================================================
int ZSelectedEnergyLineTableModel::zh_findRowToInsert(int ZNumber) const
{
    if(zv_itemMap.isEmpty())
    {
        return 0;
    }

    for(int row = 0; row < zv_itemMap.count(); row++)
    {
        if(zv_itemMap.keys().at(row) < ZNumber)
        {
            continue;
        }

        return row;
    }

    return zv_itemMap.count();
}
//=============================================================
void ZSelectedEnergyLineTableModel::zh_updateColumns()
{
    // create new column header list
    QStringList newColumnHeaderList;

    QMap<int, ZEnergyLineSetItem*>::const_iterator it;
    QStringList elementEnergyNameStringList;
    for(it = zv_itemMap.begin(); it != zv_itemMap.end(); it++ )
    {
        elementEnergyNameStringList = it.value()->zp_energyLineNameStringList();
        foreach(auto& energyLineName, elementEnergyNameStringList)
        {
            if(!newColumnHeaderList.contains(energyLineName))
            {
                newColumnHeaderList.append(energyLineName);
            }
        }
    }

    qSort(newColumnHeaderList);

    // remove all columns except 0
    if(columnCount() > 1)
    {
        beginRemoveColumns(QModelIndex(), 1, columnCount()-1);
        zv_columnHederList.clear();
        endRemoveColumns();
    }

    if(!newColumnHeaderList.isEmpty())
    {
        // insert new columns
        beginInsertColumns(QModelIndex(), 1, newColumnHeaderList.count());
        zv_columnHederList = newColumnHeaderList;
        endInsertColumns();
    }

    return;
}
//=============================================================
bool ZSelectedEnergyLineTableModel::zp_energyLineEnergyValue(const QString& elementSymbol,
                       const QString& lineName,
                       double& energyValue) const
{
    QMap<int, ZEnergyLineSetItem*>::const_iterator it;
    for(it = zv_itemMap.begin(); it != zv_itemMap.end(); it++ )
    {
        if(it.value()->zp_symbol() != elementSymbol)
        {
            continue;
        }

        if(!it.value()->zp_energyLineNameStringList().contains(lineName))
        {
            continue;
        }

        QString energyValueString;
        if(!it.value()->zp_energyLineValue(lineName, energyValueString))
        {
            continue;
        }

        bool ok;
        energyValue = energyValueString.toDouble(&ok);
        if(!ok)
        {
            continue;
        }

        return true;
    }

    return false;
}
//=============================================================
bool ZSelectedEnergyLineTableModel::zp_energyLineReletiveIntensity(const QString& elementSymbol,
                                                                   const QString& lineName,
                                                                   int& relativeIntensity) const
{
    QMap<int, ZEnergyLineSetItem*>::const_iterator it;
    for(it = zv_itemMap.begin(); it != zv_itemMap.end(); it++ )
    {
        if(it.value()->zp_symbol() != elementSymbol)
        {
            continue;
        }

        if(!it.value()->zp_energyLineNameStringList().contains(lineName))
        {
            continue;
        }

        QString relativeIntensityString;
        if(!it.value()->zp_energyLineRelativeIntensity(lineName, relativeIntensityString))
        {
            continue;
        }

        bool ok;
        relativeIntensity = relativeIntensityString.toInt(&ok);
        if(!ok)
        {
            continue;
        }

        return true;
    }

    return false;
}
//=============================================================
bool ZSelectedEnergyLineTableModel::zp_energyLineVisibility(const QString& elementSymbol,
                                                            const QString& lineName,
                                                            bool& visible) const
{
    QMap<int, ZEnergyLineSetItem*>::const_iterator it;
    for(it = zv_itemMap.begin(); it != zv_itemMap.end(); it++ )
    {
        if(it.value()->zp_symbol() != elementSymbol)
        {
            continue;
        }

        if(!it.value()->zp_energyLineNameStringList().contains(lineName))
        {
            continue;
        }

        QString energyValueString;
        if(!it.value()->zp_isEnergyLineVisible(lineName, visible))
        {
            continue;
        }

        return true;
    }

    return false;
}
//=============================================================
bool ZSelectedEnergyLineTableModel::zp_energyLineColor(const QString& elementSymbol,
                       const QString& lineName,
                       QColor& color) const
{
    QMap<int, ZEnergyLineSetItem*>::const_iterator it;
    for(it = zv_itemMap.begin(); it != zv_itemMap.end(); it++ )
    {
        if(it.value()->zp_symbol() != elementSymbol)
        {
            continue;
        }

        if(!it.value()->zp_energyLineNameStringList().contains(lineName))
        {
            continue;
        }

        QString energyValueString;
        if(!it.value()->zp_energyLineColor(lineName, color))
        {
            continue;
        }

        return true;
    }

    return false;
}
//=============================================================

