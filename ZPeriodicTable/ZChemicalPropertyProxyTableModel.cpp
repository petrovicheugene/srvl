//====================================================
#include "ZChemicalPropertyProxyTableModel.h"
#include "ZChemicalElementPropertyTreeModel.h"

#include <QAbstractItemModel>
#include <QDebug>

//====================================================
ZChemicalPropertyProxyTableModel::ZChemicalPropertyProxyTableModel(QObject *parent)
    : QAbstractTableModel(parent)
{
    zv_itemIsEditable = true;
    zv_currentSourceModelIndex = QModelIndex();
    zv_headerList.append(tr("Property"));
    zv_headerList.append(tr("Value"));

}
//====================================================
Qt::ItemFlags	ZChemicalPropertyProxyTableModel::flags(const QModelIndex & index) const
{
    Qt::ItemFlags	flags;
    flags |= Qt::ItemIsEnabled
            | Qt::ItemIsSelectable;
    if(zv_itemIsEditable)
    {
        flags |= Qt::ItemIsEditable;
    }

    return flags;
}
//====================================================
int	 ZChemicalPropertyProxyTableModel::columnCount(const QModelIndex & parent) const
{
    return 2;
}
//====================================================
int	 ZChemicalPropertyProxyTableModel::rowCount(const QModelIndex & parent) const
{
    if(!zv_currentSourceModelIndex.isValid() || !zv_currentSourceModelIndex.model())
    {
        return 0;
    }

    QVariant vData = zv_currentSourceModelIndex.data(PropertyCountRole);
    if(!vData.isValid() || !vData.canConvert<int>())
    {
        return 0;
    }

    return vData.toInt();
}
//====================================================
QVariant	ZChemicalPropertyProxyTableModel::data(const QModelIndex & index, int role) const
{
    if(!index.isValid() || index.row() < 0 || index.row() >= rowCount()
            || index.column() < 0 || index.column() >= columnCount()
            || !zv_currentSourceModelIndex.model())
    {
        return QVariant();
    }

    const QAbstractItemModel* model = zv_currentSourceModelIndex.model();
    if(!model)
    {
        return QVariant();
    }

    if(role == Qt::DisplayRole || role == Qt::EditRole)
    {
        // define data role
        int newRole;
        if(index.column() == 0)
        {
            newRole = PropertyNameRole;
        }
        else // column == 1
        {
            newRole = PropertyValueRole;
        }

        // create new source index
        // proxy index.row converts to src column
        QModelIndex newSrcIndex = model->index(zv_currentSourceModelIndex.row(), index.row(),
                                               zv_currentSourceModelIndex.parent());
        if(!newSrcIndex.isValid())
        {
            return QVariant();
        }

        return newSrcIndex.data(newRole);
    }

    return QVariant();
}
//====================================================
bool ZChemicalPropertyProxyTableModel::setData(const QModelIndex & index, const QVariant & value, int role)
{
    if(role == Qt::EditRole)
    {
        // define data role
        int newRole;
        if(index.column() == 0)
        {
            newRole = PropertyNameRole;
        }
        else // column == 1
        {
            newRole = PropertyValueRole;
        }

        const QAbstractItemModel* model = zv_currentSourceModelIndex.model();
        if(!model)
        {
            return false;
        }

        QModelIndex newSrcIndex = model->index(zv_currentSourceModelIndex.row(), index.row(),
                                               zv_currentSourceModelIndex.parent());

        const_cast<QAbstractItemModel*>(model)->setData(newSrcIndex, value, newRole);
    }

    return false;
}
//====================================================
QVariant	ZChemicalPropertyProxyTableModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if(orientation == Qt::Horizontal)
    {
        if(role == Qt::DisplayRole)
        {
            return QVariant(zv_headerList.value(section));
        }
    }
    else // Qt::Vertical
    {
        if(role == Qt::DisplayRole)
        {
            return QVariant(QString::number(section + 1));
        }
    }

    return QVariant();
}
//====================================================
bool ZChemicalPropertyProxyTableModel::setHeaderData(int section, Qt::Orientation orientation, const QVariant & data, int role)
{
    if(orientation != Qt::Horizontal || role != Qt::EditRole || section < 0 || section >= 2 )
    {
        return false;
    }

    while(zv_headerList.count() <= section)
    {
        zv_headerList.append(QString());
    }

    QString dataString;
    if(data.isValid() && data.canConvert<QString>())
    {
        dataString = data.toString();
    }

    zv_headerList[section] = dataString;

    return true;
}
//====================================================
void ZChemicalPropertyProxyTableModel::zp_setItemIsEditable(bool editable)
{
    zv_itemIsEditable = editable;
}
//====================================================
void ZChemicalPropertyProxyTableModel::zp_onCurrentTreeElementChanged(const QModelIndex& current,
                                                                      const QModelIndex& previous)
{
    beginResetModel();
    zv_currentSourceModelIndex = current;
    endResetModel();
}
//====================================================
void ZChemicalPropertyProxyTableModel::zp_onSourceModelDataChanged(const QModelIndex& topLeft,
                                                                   const QModelIndex& bottomRight)
{
    if(topLeft.parent() == zv_currentSourceModelIndex.parent()
            && topLeft.row() == zv_currentSourceModelIndex.row())
    {
        beginResetModel();
        endResetModel();
    }
}
//====================================================
