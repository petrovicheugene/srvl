//=========================================================
#include "ZMeasuringResultTableModel.h"
//=========================================================
ZMeasuringResultTableModel::ZMeasuringResultTableModel(QObject *parent)
    : QAbstractTableModel(parent)
{
    zv_measuringManager = 0;
}
//=========================================================
void ZMeasuringResultTableModel::zp_connectToMeasuringManager(ZMeasuringManager* measuringManager)
{
    zv_measuringManager = measuringManager;
    connect(zv_measuringManager, &ZMeasuringManager::zg_sampleOperation,
            this, &ZMeasuringResultTableModel::zh_onSampleOperation);
}
//=========================================================
Qt::ItemFlags ZMeasuringResultTableModel::flags(const QModelIndex & index) const
{
    Qt::ItemFlags flags;
    flags |= Qt::ItemIsEnabled | Qt::ItemIsSelectable;
    if(index.isValid() && index.column() == 0)
    {
        flags |= Qt::ItemIsEditable;
    }
    return flags;
}
//=========================================================
QModelIndex ZMeasuringResultTableModel::index(int row, int column, const QModelIndex &parent) const
{
    return QAbstractTableModel::index(row, column, parent);
}
//=========================================================
int ZMeasuringResultTableModel::columnCount(const QModelIndex & parent) const
{
    if(zv_measuringManager == 0)
    {
        return 0;
    }

    return 2;
}
//=========================================================
int	 ZMeasuringResultTableModel::rowCount(const QModelIndex & parent) const
{
    if(zv_measuringManager == 0)
    {
        return 0;
    }

    return zv_measuringManager->zp_sampleCount();
}
//=========================================================
QVariant	ZMeasuringResultTableModel::data(const QModelIndex & index, int role) const
{
    if(!index.isValid() || zv_measuringManager == 0)
    {
        return QVariant();
    }

    if(role == Qt::DisplayRole)
    {
        if(index.column() == 0)
        {
            return QVariant(zv_measuringManager->zp_sampleName(index.row()) );
        }

        if(index.column() == 1)
        {
            return QVariant(zv_measuringManager->zp_taskName(index.row()) );
        }
    }

    return QVariant();
}
//=========================================================
bool ZMeasuringResultTableModel::setData(const QModelIndex & index, const QVariant & value, int role)
{
    if(!index.isValid() || !value.isValid())
    {
        return false;
    }

    if(role == Qt::EditRole)
    {
        if(index.column() == 0)
        {
            if(!value.canConvert<QString>())
            {
                return false;
            }
            QString sampleName = value.toString();
            return zv_measuringManager->zp_setSampleName(index.row(), sampleName);
            // emit dataChanged(index);
        }
    }
    return false;
}
//=========================================================
QVariant ZMeasuringResultTableModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if(role == Qt::DisplayRole)
    {
        if(orientation == Qt::Vertical)
        {
            return QVariant(QString::number(section + 1));
        }
        else // HORIZONTAL
        {
            if(section == 0)
            {
                return QVariant(tr("Sample"));
            }

            if(section == 1)
            {
                return QVariant(tr("Task"));
            }
        }
    }

    return QVariant();
}
//=========================================================
void ZMeasuringResultTableModel::zh_onSampleOperation(ZMeasuringManager::SampleOperationType type,
                                                        int first,
                                                        int last)
{
    if(type == ZMeasuringManager::SOT_SAMPLE_ABOUT_TO_BE_INSERTED)
    {
        // beginInsertRows(QModelIndex(), first, last);
    }
    else if(type == ZMeasuringManager::SOT_SAMPLE_INSERTED)
    {
        beginInsertRows(QModelIndex(), first, last);
        endInsertRows();
    }
    else if(type == ZMeasuringManager::SOT_SAMPLE_ABOUT_TO_BE_REMOVED)
    {
        // beginInsertRows(QModelIndex(), first, last);
    }
    else if(type == ZMeasuringManager::SOT_SAMPLE_REMOVED)
    {
        beginRemoveRows(QModelIndex(), first, last);
        endRemoveRows();
    }

    // emit zg_checkCurrentArray();
}
//=========================================================
