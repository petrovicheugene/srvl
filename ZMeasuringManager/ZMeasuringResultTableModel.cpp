//=========================================================
#include "ZMeasuringResultTableModel.h"
#include "ZGeneral.h"
#include "ZSpectrumPaintData.h"
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

    return 2 + zv_chemicalStringList.count() + zv_measuringConditionsStringList.count();
}
//=========================================================
int ZMeasuringResultTableModel::zp_spectrumColumnCount() const
{
    return zv_measuringConditionsStringList.count();
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
            return QVariant(zv_measuringManager->zp_sampleTaskName(index.row()) );
        }

        // spectrum column
        if(index.column() > 1 && index.column() < zv_measuringConditionsStringList.count() + 2)
        {
            ZSpectrumPaintData paintData;
            int gainFactor;
            int exposition;

            paintData.spectrumData = zv_measuringManager->zp_spectrumData(index.row(), gainFactor, exposition);
            paintData.maxChannel = zv_measuringManager->zp_arrayChannelCount(gainFactor, exposition);
            paintData.maxIntensity = zv_measuringManager->zp_arrayMaxIntensity(gainFactor, exposition);

            QVariant::fromValue(paintData);
        }
    }

    if(role == Qt::DecorationRole)
    {
        if(index.column() > 1 && index.column() < zv_measuringConditionsStringList.count() + 2)
        {
             return QVariant(QColor(Qt::red)); // QVariant(zv_dataManager->zp_spectrumColor(index.row()));
        }
    }

    if(role == NS_DataRole::VisibleRole)
    {
        if(index.column() > 1 && index.column() < zv_measuringConditionsStringList.count() + 2)
        {
             return QVariant(true);
            // return QVariant(zv_dataManager->zp_isSpectrumVisible(index.row()));
        }
    }

    if(role == NS_DataRole::DataTypeRole)
    {
        if(index.column() > 1 && index.column() < zv_measuringConditionsStringList.count() + 2)
        {
            return QVariant(glSpectrumDataType);
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

            if(section > 1 && section < zv_measuringConditionsStringList.count() + 2)
            {
                return QVariant(zv_measuringConditionsStringList.at(section - 2));
            }

            if(section > zv_measuringConditionsStringList.count() + 1 && section < columnCount())
            {
                return QVariant(zv_chemicalStringList.at(section - 2 - zv_measuringConditionsStringList.count()));
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

        beginResetModel();
        zh_recalcColumnCount();
        endResetModel();

    }
    else if(type == ZMeasuringManager::SOT_SAMPLE_ABOUT_TO_BE_REMOVED)
    {
        // beginInsertRows(QModelIndex(), first, last);
    }
    else if(type == ZMeasuringManager::SOT_SAMPLE_REMOVED)
    {
        beginRemoveRows(QModelIndex(), first, last);
        endRemoveRows();

        beginResetModel();
        zh_recalcColumnCount();
        endResetModel();
    }
    else if(type == ZMeasuringManager::SOT_CONDITIONS_ABOUT_TO_BE_INSERTED)
    {
        // beginInsertRows(QModelIndex(), first, last);
    }
    else if(type == ZMeasuringManager::SOT_CONDITIONS_INSERTED)
    {
        beginResetModel();
        zh_recalcColumnCount();
        endResetModel();
    }
    else if(type == ZMeasuringManager::SOT_SPECTRUM_CHANGED)
    {
        // beginInsertRows(QModelIndex(), first, last);
    }
    else if(type == ZMeasuringManager::SOT_CONCENTRATIONS_CHANGED)
    {
        // beginInsertRows(QModelIndex(), first, last);
    }

    // emit zg_checkCurrentArray();
}
//=========================================================
void ZMeasuringResultTableModel::zh_recalcColumnCount()
{
    QStringList chemicalStringList;
    QStringList currentSampleChemicalStringList;

    QStringList measuringConditionsStringList;
    QStringList currentSampleMeasuringConditionsStringList;
    zv_measuringConditionsList.clear();

    for(int row = 0; row < zv_measuringManager->zp_sampleCount(); row++)
    {
        // chemicals
        currentSampleChemicalStringList = zv_measuringManager->zp_chemicalListForSample(row);
        QString chemical;
        for(int c = 0; c < currentSampleChemicalStringList.count(); c++)
        {
            chemical = currentSampleChemicalStringList.at(c);
            if(!chemicalStringList.contains(chemical))
            {
                chemicalStringList.append(chemical);
                continue;
            }

            int insertedChemicalCount = chemicalStringList.count(chemical);
            int reviewedChemicalCount = 0;
            for(int i = 0; i < c; i++)
            {
                if(currentSampleChemicalStringList.at(i) == chemical)
                {
                    reviewedChemicalCount++;
                }
            }

            for(int i = 0; i <= reviewedChemicalCount - insertedChemicalCount; i++)
            {
                chemicalStringList.append(chemical);
            }
        }

        // measuring conditions
        QList<QPair<int, int> > measuringConditionsListForSample;
        measuringConditionsListForSample = zv_measuringManager->zp_measuringConditionsListForSample(row);

        for(int c = 0; c < measuringConditionsListForSample.count(); c++)
        {
            if(!zv_measuringConditionsList.contains(measuringConditionsListForSample.at(c)))
            {
                zv_measuringConditionsList.append(measuringConditionsListForSample.at(c));
            }
        }


        currentSampleMeasuringConditionsStringList = zv_measuringManager->zp_measuringConditionsStringListForSample(row);
        QString measuringConditions;
        for(int c = 0; c < currentSampleMeasuringConditionsStringList.count(); c++)
        {
            measuringConditions = currentSampleMeasuringConditionsStringList.at(c);
            if(!measuringConditionsStringList.contains(measuringConditions))
            {
                measuringConditionsStringList.append(measuringConditions);
            }
        }
    }

    zv_chemicalStringList = chemicalStringList;
    zv_measuringConditionsStringList = measuringConditionsStringList;
}
//=========================================================
