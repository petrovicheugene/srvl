//==================================================================
#include "ZMeasurementResultTableModel.h"
#include "ZSeriesTaskTreeModel.h"
//==================================================================
ZMeasurementResultTableModel::ZMeasurementResultTableModel(QObject *parent) : QAbstractTableModel(parent)
{
    zv_sourceModel = 0;
}
//==================================================================
void ZMeasurementResultTableModel::zp_connectToSourceModel(ZSeriesTaskTreeModel* sourceModel)
{
    zv_sourceModel = sourceModel;
    connect(zv_sourceModel, &ZSeriesTaskTreeModel::rowsAboutToBeInserted,
            this, &ZMeasurementResultTableModel::zh_onRowsAboutToBeInserted);
    connect(zv_sourceModel, &ZSeriesTaskTreeModel::rowsAboutToBeRemoved,
            this, &ZMeasurementResultTableModel::zh_onRowsAboutToBeRemoved);
    connect(zv_sourceModel, &ZSeriesTaskTreeModel::rowsInserted,
            this, &ZMeasurementResultTableModel::zh_onRowsInserted);
    connect(zv_sourceModel, &ZSeriesTaskTreeModel::rowsRemoved,
            this, &ZMeasurementResultTableModel::zh_onRowsRemoved);

}
//==================================================================
Qt::ItemFlags	ZMeasurementResultTableModel::flags(const QModelIndex & index) const
{
    Qt::ItemFlags flags;
    flags |= Qt::ItemIsEnabled | Qt::ItemIsEditable | Qt::ItemIsSelectable;
    return flags;
}
//==================================================================
QModelIndex ZMeasurementResultTableModel::index(int row, int column, const QModelIndex &parent) const
{
    return QAbstractTableModel::index(row, column, parent);
}
//==================================================================
int ZMeasurementResultTableModel::columnCount(const QModelIndex & parent) const
{
    return 2;
}
//==================================================================
int ZMeasurementResultTableModel::rowCount(const QModelIndex & parent) const
{
    if(zv_sourceModel == 0)
    {
        return 0;
    }

    return zv_sourceModel->rowCount(QModelIndex());
}
//==================================================================
QVariant ZMeasurementResultTableModel::data(const QModelIndex & index, int role) const
{
    if(zv_sourceModel == 0 || !index.isValid())
    {
        return QVariant();
    }

    if(role == Qt::DisplayRole)
    {
        if(index.column() == 0)
        {
            // in result table order task - sample is inverted
            QModelIndex sourceIndex = zv_sourceModel->index(index.row(), 1, QModelIndex());
            return zv_sourceModel->data(sourceIndex, Qt::DisplayRole);
        }

        if(index.column() == 1)
        {
            QModelIndex sourceIndex = zv_sourceModel->index(index.row(), 0, QModelIndex());
            return zv_sourceModel->data(sourceIndex, Qt::DisplayRole);
        }

    }

    return QVariant();
}
//==================================================================
bool	ZMeasurementResultTableModel::setData(const QModelIndex & index, const QVariant & value, int role)
{
    return false;
}
//==================================================================
QVariant ZMeasurementResultTableModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if(orientation == Qt::Vertical)
    {
        if(role == Qt::DisplayRole)
        {
            return QVariant(section + 1);
        }
    }
    else
    {
        if(role == Qt::DisplayRole)
        {
            if(section == 0)
            {
                return QVariant(tr("Name"));
            }
            else if(section == 1)
            {
                return QVariant(tr("Task"));
            }
            //            else if(section == 2)
            //            {
            //                return QVariant("Name");
            //            }
            //            else if(section == 3)
            //            {
            //                return QVariant("Task");
            //            }
            //            else if(section == 4)
            //            {
            //                return QVariant("Operator");
            //            }
        }

        return QVariant();
    }

    return QVariant();
}
//==================================================================
void ZMeasurementResultTableModel::zh_onRowsAboutToBeInserted(const QModelIndex &parent, int start, int end)
{
    if(!parent.isValid())
    {
        beginInsertRows(QModelIndex(), start, end);
        // emit rowsAboutToBeInserted(QModelIndex(), start, end);
    }
}
//==================================================================
void ZMeasurementResultTableModel::zh_onRowsAboutToBeRemoved(const QModelIndex &parent, int start, int end)
{
    if(!parent.isValid())
    {
        beginRemoveRows(QModelIndex(), start, end);
        // emit rowsAboutToBeRemoved(QModelIndex(), start, end);
    }

}
//==================================================================
void ZMeasurementResultTableModel::zh_onRowsInserted(const QModelIndex &parent, int start, int end)
{
    if(!parent.isValid())
    {
        endInsertRows();
        // emit rowsInserted(QModelIndex(), start, end);
    }

}
//==================================================================
void ZMeasurementResultTableModel::zh_onRowsRemoved(const QModelIndex &parent, int start, int end)
{
    if(!parent.isValid())
    {
        endRemoveRows();
        //emit rowsRemoved(QModelIndex(), start, end);
    }

}
//==================================================================
