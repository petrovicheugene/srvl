//=============================================================
#include "ZSelectedEnergyLineTableModel.h"

#include <algorithm>
#include <QDebug>
//=============================================================
ZSelectedEnergyLineTableModel::ZSelectedEnergyLineTableModel(QObject *parent)
    : QAbstractTableModel(parent)
{

}
//=============================================================
Qt::ItemFlags	ZSelectedEnergyLineTableModel::flags(const QModelIndex & index) const
{
    Qt::ItemFlags	flags;
    flags |= Qt::ItemIsEnabled
            | Qt::ItemIsSelectable;
    if(true)
    {
        flags |= Qt::ItemIsEditable;
    }

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

    if(role == Qt::DisplayRole)
    {
        int ZNumber = zv_itemMap.keys().at(index.row());

        if(index.column() == 0)
        {
            QString symbolString = zv_itemMap.value(ZNumber)->zp_symbol();
            return QVariant(symbolString);
        }
        else
        {
            QString energyLineName = zv_columnHederList.value(index.column() - 1, QString());
            QString energyLineValue;
            if(zv_itemMap.value(ZNumber)->zp_energyLineValue(energyLineName, energyLineValue))
            {
                return QVariant(energyLineValue);
            }
        }
    }

    return QVariant();
}
//=============================================================
bool ZSelectedEnergyLineTableModel::setData(const QModelIndex & index, const QVariant & value, int role)
{
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

            // get chemelement energy lines
            PropertyList propertyList;
            emit zg_requestEnergyLinesForZNumber(ZNumber, propertyList);

            beginInsertRows(QModelIndex(), row, row);
            ZEnergyLineSetItem* item = new ZEnergyLineSetItem(ZNumber,
                                                              symbol,
                                                              propertyList);
            zv_itemMap.insert(ZNumber, item);
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

