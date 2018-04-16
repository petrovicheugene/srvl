//=========================================================
#include "ZMeasuringResultTableModel.h"
#include "ZGeneral.h"
#include "ZSpectrumPaintData.h"
#include "ZSpeSpectrum.h"
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
    connect(this, &ZMeasuringResultTableModel::zg_currentMeasuringConditions,
            zv_measuringManager, &ZMeasuringManager::zg_currentMeasuringConditions);

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
ZSpeSpectrum* ZMeasuringResultTableModel::zp_spectrum(int row, quint8 gainFactor, int exposition) const
{
    if(row < 0 || row >= rowCount() || !zv_measuringManager)
    {
        return 0;
    }

    return zv_measuringManager->zp_spectrum(row, gainFactor, exposition);
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
QVariant ZMeasuringResultTableModel::data(const QModelIndex & index, int role) const
{
    if(!index.isValid() || zv_measuringManager == 0
            || index.row() < 0 || index.row() >= rowCount()
            || index.column() < 0 || index.column() >= columnCount())
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
        if(index.column() > 1 && index.column() < zv_measuringConditionsList.count() + 2)
        {
            ZSpectrumPaintData paintData;
            quint8 gainFactor = zv_measuringConditionsList.at(index.column() - 2).first;
            int exposition = zv_measuringConditionsList.at(index.column() - 2).second;

            paintData.spectrumData = zv_measuringManager->zp_spectrumData(index.row(), gainFactor, exposition);
            paintData.maxChannel = zv_measuringManager->zp_arrayChannelCount(gainFactor, exposition);
            paintData.maxIntensity = zv_measuringManager->zp_arrayMaxIntensity(gainFactor, exposition);

            return QVariant::fromValue(paintData);
        }

        if(index.column() > zv_measuringConditionsList.count() + 1)
        {
            // get chemical for column
            QString chemical =
                    zv_chemicalStringList.at(index.column() - 2 - zv_measuringConditionsStringList.count());

            if(chemical.isEmpty())
            {
                return QVariant();
            }

            double concentration;
            if(!zv_measuringManager->zp_concentration(index.row(), chemical, concentration))
            {
                return QVariant();
            }

            return QVariant(concentration);
        }
    }

    if(role == Qt::DecorationRole)
    {
        if(index.column() > 1 && index.column() < zv_measuringConditionsStringList.count() + 2)
        {
            //            QList<QPair<int, int> > measuringConditionsListForSample = zv_measuringManager->zp_measuringConditionsListForSample(index.row());
            //            if(!measuringConditionsListForSample.contains(zv_measuringConditionsList.value(index.column() - 2, QPair<int, int>(-1, -1))))
            //            {
            //                return QVariant();
            //            }

            int gainFactor = zv_measuringConditionsList.value(index.column() - 2, QPair<int, int>(-1, -1)).first;
            int exposition = zv_measuringConditionsList.value(index.column() - 2, QPair<int, int>(-1, -1)).second;

            QColor spectrumColor = zv_measuringManager->zp_spectrumColor(index.row(), gainFactor, exposition);
            if(!spectrumColor.isValid())
            {
                return QVariant();
            }

            return QVariant(spectrumColor); // QVariant(zv_dataManager->zp_spectrumColor(index.row()));
        }
    }

    if(role == NS_DataRole::VisibleRole)
    {
        if(index.column() > 1 && index.column() < zv_measuringConditionsList.count() + 2)
        {
            QPair<int, int> measuringConditions = zv_measuringConditionsList.value(index.column() - 2);
            bool visibility;
            if(!zv_measuringManager->zp_spectrumVisibility(index.row(), measuringConditions.first, measuringConditions.second, visibility))
            {
                return QVariant();
            }
            return QVariant(visibility);
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
bool ZMeasuringResultTableModel::zp_measuringConditionsForColumn(int column,
                                                                 QPair<quint8, int>& measuringConditions) const
{
    if(column <  2 || column >= zv_measuringConditionsList.count() + 2)
    {
        return false;
    }

    measuringConditions = zv_measuringConditionsList.value(column - 2, QPair<int, int>(-1, -1));
    return true;
}
//=========================================================
void ZMeasuringResultTableModel::zp_onCurrentIndexChanged(const QModelIndex& current, const QModelIndex& previous)
{
    quint8 gainFactor;
    int exposition;
    const ZSpeSpectrum* spectrum;
    zp_measuringConditionsAndSpectrumForIndex(current, gainFactor, exposition, spectrum);

    emit zg_currentMeasuringConditions(gainFactor, exposition, spectrum);
    return;
}
//=========================================================
void ZMeasuringResultTableModel::zp_measuringConditionsAndSpectrumForIndex(const QModelIndex& index,
                                                                           quint8& gainFactor,
                                                                           int& exposition,
                                                                           const ZSpeSpectrum*& spectrum)
{
    gainFactor = 0;
    exposition = -1;

    // check current index
    if(!index.isValid())
    {
        return;
    }

    // check type of current index data
    QVariant vData = data(index, NS_DataRole::DataTypeRole);
    if(!vData.isValid() || vData.isNull() || !vData.canConvert<int>())
    {
        return;
    }

    if(vData.toInt() != glSpectrumDataType)
    {
        return;
    }

    // current index data is spectrum
    // get list index of measuring conditions of the current spectrum
    int currentMeasuringConditionsIndex = index.column() - 2;
    // check currentMeasuringConditionsIndex
    if(currentMeasuringConditionsIndex < 0
            || currentMeasuringConditionsIndex >= zv_measuringConditionsList.count())
    {
        return;
    }

    gainFactor = zv_measuringConditionsList.at(currentMeasuringConditionsIndex).first;
    exposition = zv_measuringConditionsList.at(currentMeasuringConditionsIndex).second;

    // current spectrum
    spectrum = zv_measuringManager->zp_spectrum(index.row(), gainFactor, exposition);

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
            bool res = zv_measuringManager->zp_setSampleName(index.row(), sampleName);
            if(res)
            {
                emit dataChanged(index, index);
            }
            return res;
        }
    }

    if(role == NS_DataRole::VisibleRole)
    {
        if(index.column() < 2 || index.column() >= zv_measuringConditionsList.count() + 2
                || !value.canConvert<bool>())
        {
            return false;
        }

        QPair<int, int> measuringConditions = zv_measuringConditionsList.value(index.column() - 2);

        bool visibility = value.toBool();
        bool res = zv_measuringManager->zp_setSpectrumVisibility(index.row(), measuringConditions.first, measuringConditions.second, visibility);
        if(res)
        {
            emit dataChanged(index, index);
        }
        return res;
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
        beginInsertRows(QModelIndex(), first, last);
    }
    else if(type == ZMeasuringManager::SOT_SAMPLE_INSERTED)
    {
        //beginInsertRows(QModelIndex(), first, last);
        endInsertRows();

        beginResetModel();
        zh_recalcColumnCount();
        endResetModel();
    }
    else if(type == ZMeasuringManager::SOT_SAMPLE_ABOUT_TO_BE_REMOVED)
    {
        beginRemoveRows(QModelIndex(), first, last);
    }
    else if(type == ZMeasuringManager::SOT_SAMPLE_REMOVED)
    {
        //beginRemoveRows(QModelIndex(), first, last);
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
        zh_repaintAllSpectra();

        //        beginResetModel();
        //        zh_recalcColumnCount();
        //        endResetModel();
    }
    else if(type == ZMeasuringManager::SOT_CONCENTRATIONS_CHANGED)
    {
        QModelIndex topLeft = index(first, 2 + zv_measuringConditionsList.count(), QModelIndex());
        QModelIndex bottomRight = index(last, columnCount() - 1, QModelIndex());
        emit dataChanged(topLeft, bottomRight);
    }

    // emit zg_checkCurrentArray();
}
//=========================================================
void ZMeasuringResultTableModel::zh_repaintAllSpectra()
{
    if(!zv_measuringConditionsList.isEmpty())
    {
        // repaint all spectra
        QModelIndex topLeft = index(0, 2, QModelIndex());
        QModelIndex bottomRight = index(rowCount(), zv_measuringConditionsList.count() + 1, QModelIndex());
        zh_recalcColumnCount();
        emit dataChanged(topLeft, bottomRight);
    }
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
        QList<QPair<quint8, int> > measuringConditionsListForSample;
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
