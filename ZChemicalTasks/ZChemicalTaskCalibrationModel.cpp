//==========================================================
#include "ZChemicalTaskCalibrationModel.h"
#include "ZGeneral.h"

#include <QSqlTableModel>
#include <QSqlQuery>
//==========================================================
ZChemicalTaskCalibrationModel::ZChemicalTaskCalibrationModel(QObject *parent)
    : QAbstractTableModel(parent)
{
    zv_calibrationSQLTableModel = 0;
    zv_chemicalId = -1;
}
//==========================================================
void ZChemicalTaskCalibrationModel::zp_connectToCalibrationSQLTableModel(QSqlTableModel *model)
{
    zv_calibrationSQLTableModel = model;
    //    connect(zv_filteredCalibrationModel, &QSqlTableModel::dataChanged,
    //            this, &ZChemicalTaskCalibrationModel::zh_sourceDataChanged);
    //    connect(zv_filteredCalibrationModel, &QSqlTableModel::modelAboutToBeReset,
    //            this, &ZChemicalTaskCalibrationModel::zh_sourceModelAboutToBeReset);
    //    connect(zv_filteredCalibrationModel, &QSqlTableModel::modelReset,
    //            this, &ZChemicalTaskCalibrationModel::zh_sourceModelReset);
    //    connect(zv_filteredCalibrationModel, &QSqlTableModel::rowsAboutToBeInserted,
    //            this, &ZChemicalTaskCalibrationModel::zh_sourceRowsAboutToBeInserted);
    //    connect(zv_filteredCalibrationModel, &QSqlTableModel::rowsAboutToBeRemoved,
    //            this, &ZChemicalTaskCalibrationModel::zh_sourceRowsAboutToBeRemoved);
    //    connect(zv_filteredCalibrationModel, &QSqlTableModel::rowsInserted,
    //            this, &ZChemicalTaskCalibrationModel::zh_sourceRowsInserted);
    //    connect(zv_filteredCalibrationModel, &QSqlTableModel::rowsRemoved,
    //            this, &ZChemicalTaskCalibrationModel::zh_sourceRowsRemoved);

}
//==========================================================
Qt::ItemFlags ZChemicalTaskCalibrationModel::flags(const QModelIndex &index) const
{
    Qt::ItemFlags flags;
    if(zv_calibrationSQLTableModel == 0 || !index.isValid())
    {
        return flags;
    }

    flags = Qt::ItemIsEnabled | Qt::ItemIsSelectable;

    if(index.column() > 0)
    {
        flags |= Qt::ItemIsEditable;
    }

    return flags;
}
//==========================================================
QVariant ZChemicalTaskCalibrationModel::data(const QModelIndex &index, int role) const
{
    if(zv_calibrationSQLTableModel == 0 || !index.isValid() || index.row() < 0
            || index.row() >= zv_calibrationItemList.count() || index.column() < 0 || index.column() >= columnCount() )
    {
        return QVariant();
    }

    if(role == Qt::DisplayRole)
    {
        // get calibration row
        const ZCalibrationItem* calibrationItem = &zv_calibrationItemList.at(index.row());

        if(zh_calibrationIdForSourceRow(calibrationItem->calibrationRow)
                != calibrationItem->calibrationId)
        {
            if(!zh_calibrationRowForItem(index.row()))
            {
                return QVariant();
            }
        }

        QVariant vData;
        if(index.column() == 0)
        {
            // name
            return zh_sourceModelData(calibrationItem->calibrationRow, 1);
        }
        if(index.column() == 1)
        {
            // description
            return zh_sourceModelData(calibrationItem->calibrationRow, 2);
        }
        if(index.column() == 2)
        {
            // min
            return QVariant(calibrationItem->minConcentration);
        }
        if(index.column() == 3)
        {
            return QVariant(calibrationItem->maxConcentration);
        }
    }

    return QVariant();
}
//==========================================================
QVariant ZChemicalTaskCalibrationModel::zh_sourceModelData(int row, int column, int role) const
{
    if(zv_calibrationSQLTableModel == 0)
    {
        return QVariant();
    }

    QModelIndex index = zv_calibrationSQLTableModel->index(row, column);
    if(!index.isValid())
    {
        return QVariant();
    }

    return zv_calibrationSQLTableModel->data(index, role);
}
//==========================================================
bool ZChemicalTaskCalibrationModel::zh_calibrationRowForItem(int calibrationItemIndex) const
{
    if(zv_calibrationSQLTableModel == 0
            || calibrationItemIndex < 0
            || calibrationItemIndex >= zv_calibrationItemList.count())
    {
        return false;
    }

    QModelIndex index;
    QVariant vData;
    bool ok;
    int id;
    ZCalibrationItem* pItem = const_cast<ZCalibrationItem*>(&zv_calibrationItemList[calibrationItemIndex]);

    for(int row = 0; row < zv_calibrationSQLTableModel->rowCount(); row++)
    {
        index = zv_calibrationSQLTableModel->index(row, 0);
        if(!index.isValid())
        {
            continue;
        }

        vData = zv_calibrationSQLTableModel->data(index, Qt::DisplayRole);
        if(!vData.isValid() || !vData.canConvert<int>())
        {
            continue;
        }

        id = vData.toInt(&ok);
        if(id == zv_calibrationItemList.at(calibrationItemIndex).calibrationId)
        {
            pItem->calibrationRow = row;
            return true;
        }
    }

    pItem->calibrationRow = -1;
    return false;
}
//==========================================================
int ZChemicalTaskCalibrationModel::zh_calibrationRowForId(int id) const
{
    if(zv_calibrationSQLTableModel == 0)
    {
        return -1;
    }

    QModelIndex index;
    QVariant vData;
    for(int row = 0; row < zv_calibrationSQLTableModel->rowCount(); row++)
    {
        index = zv_calibrationSQLTableModel->index(row, 0);
        if(!index.isValid())
        {
            continue;
        }

        vData = index.data(Qt::DisplayRole);
        if(!vData.isValid() || !vData.canConvert<int>())
        {
            continue;
        }

        if(vData.toInt() == id)
        {
            return row;
        }
    }

    return -1;
}
//==========================================================
bool ZChemicalTaskCalibrationModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if(zv_calibrationSQLTableModel == 0 || !index.isValid() || index.row() < 0
            || index.row() >= zv_calibrationItemList.count() || index.column() < 0
            || index.column() >= columnCount() || !value.isValid())
    {
        return false;
    }

    if(role == Qt::EditRole)
    {
        ZCalibrationItem* calibrationItem = const_cast<ZCalibrationItem*>(&zv_calibrationItemList.at(index.row()));
        // description
        if(index.column() == 1)
        {
            if(!value.canConvert<QString>())
            {
                return false;
            }

            // get calibration row

            if(zh_calibrationIdForSourceRow(calibrationItem->calibrationRow)
                    != calibrationItem->calibrationId)
            {
                if(!zh_calibrationRowForItem(index.row()))
                {
                    return false;
                }
            }

            QModelIndex sourceIndex = zv_calibrationSQLTableModel->index(calibrationItem->calibrationRow, 2);
            if(!sourceIndex.isValid())
            {
                return false;
            }

            if(!zv_calibrationSQLTableModel->setData(sourceIndex, value))
            {
                zv_calibrationSQLTableModel->revertAll();
                return false;
            }

            if(!zv_calibrationSQLTableModel->submitAll())
            {
                zv_calibrationSQLTableModel->revertAll();
                return false;
            }

            return true;
        }

        if(!value.canConvert<double>())
        {
            return false;
        }

        bool ok;
        if(index.column() == 2)
        {
            double dVal = value.toDouble(&ok);
            if(ok)
            {
                calibrationItem->minConcentration = dVal;
                return true;
            }
        }

        if(index.column() == 3)
        {
            double dVal = value.toDouble(&ok);
            if(ok)
            {
                calibrationItem->maxConcentration = dVal;
                return true;
            }
        }

    }


    //    bool ok;
    //    double limit = value.toDouble(&ok);
    //    if(!ok)
    //    {
    //        return false;
    //    }

    //    // get calibration id
    //    int id = zp_calibrationId(index.row());
    //    if(id < 0)
    //    {
    //        return false;
    //    }

    //    QPair<double, double> limits;
    //    if(zv_idAndLimitsMap.keys().contains(id))
    //    {
    //        limits = zv_idAndLimitsMap.value(id);
    //    }
    //    else
    //    {
    //        limits = QPair<double, double>(0.0, 0.0);
    //    }

    //    if(index.column() == zv_calibrationSQLTableModel->columnCount())
    //    {
    //        limits.first = limit;
    //    }

    //    if(index.column() == zv_calibrationSQLTableModel->columnCount() + 1)
    //    {
    //        limits.second = limit;
    //    }

    //    zv_idAndLimitsMap.insert(id, limits);
    return false;
}
//==========================================================
QVariant ZChemicalTaskCalibrationModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if(orientation == Qt::Vertical)
    {
        if(role == Qt::DisplayRole )
        {
            return QVariant(QString::number(section + 1));
        }
    }
    else // orientation == Qt::Horizontal
    {
        if(role == Qt::DisplayRole )
        {
            if(section == 0)
            {
                return QVariant(tr("Name"));
            }
            if(section == 1)
            {
                return QVariant(tr("Description"));
            }
            if(section == 2)
            {
                return QVariant(tr("Min concentration"));
            }
            if(section == 3)
            {
                return QVariant(tr("Max concentration"));
            }
        }
    }

    return QVariant();
}
//==========================================================
QModelIndex ZChemicalTaskCalibrationModel::index(int row, int column, const QModelIndex &parent) const
{
    return createIndex(row, column);
}
//==========================================================
int ZChemicalTaskCalibrationModel::columnCount(const QModelIndex &parent) const
{
    if(zv_calibrationSQLTableModel == 0)
    {
        return 0;
    }

    return 4;
}
//==========================================================
int ZChemicalTaskCalibrationModel::rowCount(const QModelIndex &parent) const
{
    if(zv_calibrationSQLTableModel == 0)
    {
        return 0;
    }

    return zv_calibrationItemList.count();
}
//==========================================================
bool ZChemicalTaskCalibrationModel::removeRows(int row, int count, const QModelIndex &parent)
{
    int removedRow;
    for(int i = count - 1; i >= 0; i--)
    {
        removedRow = row + i;
        if(removedRow < 0 || removedRow >= zv_calibrationItemList.count())
        {
            continue;
        }
        beginRemoveRows(QModelIndex(), removedRow, removedRow);
        zv_calibrationItemList.removeAt(removedRow);
        endRemoveRows();
    }

    return true;
}
//==========================================================
void ZChemicalTaskCalibrationModel::zh_sourceDataChanged(const QModelIndex &sTopLeft, const QModelIndex &sBottomRight, const QVector<int> &roles)
{
    QModelIndex topLeft = zh_mapIndexFromSource(sTopLeft);
    QModelIndex bottomRight = zh_mapIndexFromSource(sBottomRight);

    emit dataChanged(topLeft, bottomRight, roles);
}
//==========================================================
void ZChemicalTaskCalibrationModel::zh_sourceModelAboutToBeReset()
{
    beginResetModel();
}
//==========================================================
void ZChemicalTaskCalibrationModel::zh_sourceModelReset()
{
    endResetModel();
}
//==========================================================
void ZChemicalTaskCalibrationModel::zh_sourceRowsAboutToBeInserted(const QModelIndex &parent, int start, int end)
{
    beginInsertRows(parent, start, end);
}
//==========================================================
void ZChemicalTaskCalibrationModel::zh_sourceRowsAboutToBeRemoved(const QModelIndex &parent, int first, int last)
{
    beginRemoveRows(parent, first, last);
}
//==========================================================
void ZChemicalTaskCalibrationModel::zh_sourceRowsInserted(const QModelIndex &parent, int first, int last)
{
    endInsertRows();
}
//==========================================================
void ZChemicalTaskCalibrationModel::zh_sourceRowsRemoved(const QModelIndex &parent, int first, int last)
{
    endRemoveRows();
}
//==========================================================
QModelIndex ZChemicalTaskCalibrationModel::zh_mapIndexFromSource(const QModelIndex& sourceIndex) const
{
    if(zv_calibrationSQLTableModel == 0 || !sourceIndex.isValid())
    {
        return QModelIndex();
    }

    return createIndex(sourceIndex.row(), sourceIndex.column());
}
//==========================================================
int ZChemicalTaskCalibrationModel::zh_calibrationIdForSourceRow(int sourceRow) const
{
    if(zv_calibrationSQLTableModel == 0 || sourceRow < 0 || sourceRow >= zv_calibrationSQLTableModel->rowCount())
    {
        return -1;
    }

    QModelIndex index = zv_calibrationSQLTableModel->index(sourceRow, 0);
    if(!index.isValid())
    {
        return -1;
    }

    QVariant vData = zv_calibrationSQLTableModel->data(index, Qt::DisplayRole);
    if(!vData.isValid() || !vData.canConvert<int>())
    {
        return -1;
    }

    bool ok;
    int id = vData.toInt(&ok);
    if(!ok)
    {
        return -1;
    }

    return id;
}
//==========================================================
void ZChemicalTaskCalibrationModel::zp_setChemicalTaskId(int chemicalTaskId)
{
    zv_chemicalId = chemicalTaskId;
}
//==========================================================
void ZChemicalTaskCalibrationModel::zp_addCalibrationIdToFilter(const QList<int> idList)
{
    QSqlQuery query;
    QString queryString;
    QVariant vData;
    for(int i = 0; i < idList.count(); i++)
    {
        bool idExists = false;
        for(int k = 0; k < zv_calibrationItemList.count(); k++)
        {
            if(zv_calibrationItemList.at(k).calibrationId == idList.at(i))
            {
                idExists = true;
                break;
            }
        }

        if(!idExists)
        {
            int newRow = zv_calibrationItemList.count();
            beginInsertRows(QModelIndex(), newRow, newRow);
            ZCalibrationItem calibrationItem(idList.at(i));
            calibrationItem.calibrationRow = zh_calibrationRowForId(idList.at(i));

            query.clear();
            queryString = QString("SELECT calibration_min_limit, calibration_max_limit "
                                  "FROM calibrations_has_calibration_stacks "
                                  "WHERE calibrations_id=%1 AND calibration_stacks_id=%2").arg(QString::number(idList.at(i)), QString::number(zv_chemicalId));

            if(query.prepare(queryString))
            {
                if(query.exec())
                {
                    if(query.next())
                    {
                        vData = query.value(0);
                        if(vData.isValid() && vData.canConvert<double>())
                        {
                            calibrationItem.minConcentration = vData.toDouble();
                        }

                        vData = query.value(1);
                        if(vData.isValid() && vData.canConvert<double>())
                        {
                            calibrationItem.maxConcentration = vData.toDouble();
                        }
                    }
                }
            }

            zv_calibrationItemList.append(calibrationItem);
            endInsertRows();
        }
    }
}
//==========================================================
bool ZChemicalTaskCalibrationModel::zp_moveRowUp(int row)
{
    if(row < 1 || row >= zv_calibrationItemList.count())
    {
        return false;
    }

    zv_calibrationItemList.swap(row, row - 1);

    QModelIndex topLeft = index(row - 1, 0);
    QModelIndex bottomRight = index(row, columnCount() - 1);
    emit dataChanged(topLeft, bottomRight);
    return true;
}
//==========================================================
bool ZChemicalTaskCalibrationModel::zp_moveRowDown(int row)
{
    if(row < 0 || row >= zv_calibrationItemList.count() - 1)
    {
        return false;
    }

    zv_calibrationItemList.swap(row, row + 1);

    QModelIndex topLeft = index(row, 0);
    QModelIndex bottomRight = index(row + 1, columnCount() - 1);
    emit dataChanged(topLeft, bottomRight);
    return true;
}
//==========================================================
bool ZChemicalTaskCalibrationModel::zp_limitsForRow(int row, double& min, double& max) const
{
    if(row < 0 || row >= zv_calibrationItemList.count() )
    {
        return false;
    }

    min = zv_calibrationItemList.at(row).minConcentration;
    max = zv_calibrationItemList.at(row).maxConcentration;

    return true;
}
//==========================================================
QString ZChemicalTaskCalibrationModel::zp_calibrationName(int row) const
{
    if(row < 0 || row >= zv_calibrationItemList.count() )
    {
        return QString();
    }

    QModelIndex index = this->index(row, 0);
    if(!index.isValid())
    {
        return QString();
    }

    QVariant vData = index.data(Qt::DisplayRole);
    if(!vData.isValid() || !vData.canConvert<QString>())
    {
        return QString();
    }

    return vData.toString();
}
//==========================================================
int ZChemicalTaskCalibrationModel::zh_calibrationId(int row) const
{
    if(row < 0 || row >= zv_calibrationItemList.count())
    {
        return -1;
    }

    return zv_calibrationItemList.at(row).calibrationId;
}
//==========================================================
