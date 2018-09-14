//=========================================================
#include "ZEnergyCalibrationSpectrumTableModel.h"
#include "ZGeneral.h"
#include "ZSpectrumPaintData.h"
#include "ZSpeSpectrum.h"

#include <QDebug>
//=========================================================
ZEnergyCalibrationSpectrumTableModel::ZEnergyCalibrationSpectrumTableModel(QObject *parent)
    : QAbstractTableModel(parent)
{
    zv_gainFactorFilter = 0;
}
//=========================================================
ZEnergyCalibrationSpectrumTableModel::~ZEnergyCalibrationSpectrumTableModel()
{
    zh_restoreSpectrumVisibleAndCheckedState();
}
//=========================================================
Qt::ItemFlags ZEnergyCalibrationSpectrumTableModel::flags(const QModelIndex & index) const
{
    Qt::ItemFlags flags;
    flags = Qt::ItemIsEnabled;

    if(index.column() == 0)
    {
        flags |= Qt::ItemIsUserCheckable;
    }
    return flags;
}
//=========================================================
QModelIndex ZEnergyCalibrationSpectrumTableModel::index(int row, int column, const QModelIndex &parent) const
{
    if(row < 0 || row >= rowCount() || column < 0 || column >= columnCount() )
    {
        return QModelIndex();
    }

    return createIndex(row, column, zv_spectrumMap.value(zv_gainFactorFilter).at(row));
}
//=========================================================
int ZEnergyCalibrationSpectrumTableModel::columnCount(const QModelIndex & parent) const
{
    return 2;
}
//=========================================================
int ZEnergyCalibrationSpectrumTableModel::rowCount(const QModelIndex & parent) const
{
    return zv_spectrumMap.value(zv_gainFactorFilter).count();
}
//=========================================================
QVariant ZEnergyCalibrationSpectrumTableModel::data(const QModelIndex & index, int role) const
{
    if(!index.isValid() || index.row() < 0 || index.row() >= rowCount() ||
            index.column() < 0 || index.column() >= columnCount())
    {
        return QVariant();
    }

    if(role == Qt::DisplayRole)
    {
        if(index.column() == 0)
        {
            QString speName = zv_spectrumMap.value(zv_gainFactorFilter).at(index.row())->zp_name();
            return QVariant(zv_spectrumMap.value(zv_gainFactorFilter).at(index.row())->zp_name());
        }

        if(index.column() == 1)
        {
            ZSpectrumPaintData paintData;
            paintData.spectrumData = zv_spectrumMap.value(zv_gainFactorFilter).at(index.row())->zp_spectrumData();
            paintData.maxChannel = zv_maxChannelNumber;
            paintData.maxIntensity = zv_maxIntensity;

            return QVariant::fromValue(paintData);
        }
    }

    if(role == Qt::DecorationRole)
    {
        if(index.column() == 1)
        {
            QColor spectrumColor = zv_spectrumMap.value(zv_gainFactorFilter).at(index.row())->zp_color();
            if(!spectrumColor.isValid())
            {
                return QVariant();
            }

            return QVariant(spectrumColor); // QVariant(zv_dataManager->zp_spectrumColor(index.row()));
        }
    }

    if(role == NS_DataRole::VisibleRole)
    {
        if(index.column() == 1)
        {
            bool visibility = zv_spectrumMap.value(zv_gainFactorFilter).at(index.row())->zp_isSpectrumVisible();
            return QVariant(visibility);
        }
    }

    if(role == NS_DataRole::DataTypeRole)
    {
        if(index.column() == 1)
        {
            return QVariant(glSpectrumDataType);
        }
    }

    if(role == Qt::CheckStateRole)
    {
        if(index.column() == 0)
        {
            if(zv_spectrumMap.value(zv_gainFactorFilter).at(index.row())->zp_isSpectrumChecked())
            {
                return Qt::Checked;
            }
            else
            {
                return Qt::Unchecked;
            }
        }
    }

    return QVariant();
}
//=========================================================
bool ZEnergyCalibrationSpectrumTableModel::setData(const QModelIndex & index, const QVariant & value, int role)
{
    if(!index.isValid() || index.row() < 0 || index.row() >= rowCount() ||
            index.column() < 0 || index.column() >= columnCount() || !value.isValid())
    {
        return false;
    }

// here
    if(role == Qt::CheckStateRole)
    {
        if(index.column() == 0)
        {
            bool checked = zv_spectrumMap.value(zv_gainFactorFilter).at(index.row())->zp_isSpectrumChecked();
            zv_spectrumMap.value(zv_gainFactorFilter)[index.row()]->zp_setSpectrumChecked(!checked);
            emit dataChanged(index, index);
            return true;
        }
    }

    if(role == NS_DataRole::VisibleRole)
    {
        if(index.column() == 1)
        {
            bool visibility = zv_spectrumMap.value(zv_gainFactorFilter).at(index.row())->zp_isSpectrumVisible();
            zv_spectrumMap.value(zv_gainFactorFilter).at(index.row())->zp_setSpectrumVisible(!visibility);
            emit dataChanged(index, index);
            return true;
        }
    }

    return false;
}
//=========================================================
QVariant ZEnergyCalibrationSpectrumTableModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if(role == Qt::DisplayRole)
    {
        if(orientation == Qt::Vertical)
        {
            return QVariant(QString::number(section + 1));
        }
        else // Horizontal
        {
            return tr("Name");
        }
    }
    return QVariant();
}
//=========================================================
void ZEnergyCalibrationSpectrumTableModel::zp_loadSpectrumData(QMap<quint8, QList<ZSpeSpectrum*> >& spectrumMap)
{
    beginResetModel();
    zv_spectrumMap = spectrumMap;
    zh_recalcMaxSpectrumParameters();
    zh_saveSpectrumVisibleAndCheckedState();
    endResetModel();
}
//=========================================================
ZSpeSpectrum* ZEnergyCalibrationSpectrumTableModel::zp_spectrumForRow(int row)
{
    if(row < 0 || row >= rowCount())
    {
        return nullptr;
    }

    return zv_spectrumMap.value(zv_gainFactorFilter).at(row);
}
//=========================================================
void ZEnergyCalibrationSpectrumTableModel::zp_writeEnergyCalibrationToCheckedSpectra(QList<double>& factorList)
{
    QList<ZSpeSpectrum*> spectrumList = zv_spectrumMap.value(zv_gainFactorFilter, QList<ZSpeSpectrum*>());

    foreach(ZSpeSpectrum* spectrum, spectrumList)
    {
        if(spectrum->zp_isSpectrumChecked())
        {
            spectrum->zp_setEnergyCalibration(factorList);
        }
    }
}
//=========================================================
void ZEnergyCalibrationSpectrumTableModel::zp_setGainFactorFilter(quint8 currentGainFactor)
{
    beginResetModel();
    zv_gainFactorFilter = currentGainFactor;
    zh_recalcMaxSpectrumParameters();
    endResetModel();
}
//=========================================================
void ZEnergyCalibrationSpectrumTableModel::zh_recalcMaxSpectrumParameters()
{
    zv_maxIntensity = 0;
    zv_maxChannelNumber = 0;
    for(auto & spectrum : zv_spectrumMap.value(zv_gainFactorFilter))
    {
        if(zv_maxIntensity < spectrum->zp_maxIntensity())
        {
            zv_maxIntensity = spectrum->zp_maxIntensity();
        }

        if(zv_maxChannelNumber < spectrum->zp_spectrumData().count())
        {
            zv_maxChannelNumber = spectrum->zp_spectrumData().count();
        }
    }
}
//=========================================================
void ZEnergyCalibrationSpectrumTableModel::zh_saveSpectrumVisibleAndCheckedState()
{
    for(auto gainFactor :  zv_spectrumMap.keys())
    {
        for(auto spectrum : zv_spectrumMap.value(gainFactor))
        {
            zv_spectrumVisibilityMap[gainFactor].append(QPair<ZSpeSpectrum*, QPair<bool, bool> >(spectrum,
                                                                                               QPair<bool, bool>(spectrum->zp_isSpectrumChecked(), spectrum->zp_isSpectrumVisible())));
        }
    }
}
//=========================================================
void ZEnergyCalibrationSpectrumTableModel::zh_restoreSpectrumVisibleAndCheckedState()
{
    int spectrumIndex = 0;
    for(auto gainFactor :  zv_spectrumVisibilityMap.keys())
    {
        if(!zv_spectrumMap.keys().contains(gainFactor))
        {
            continue;
        }
        for(auto spectrumProperties : zv_spectrumVisibilityMap.value(gainFactor))
        {
            spectrumIndex = zv_spectrumMap[gainFactor].indexOf(spectrumProperties.first);
            if(spectrumIndex < 0 || spectrumIndex >= zv_spectrumMap[gainFactor].count())
            {
                continue;
            }
            zv_spectrumMap[gainFactor][spectrumIndex]->zp_setSpectrumChecked(spectrumProperties.second.first);
            zv_spectrumMap[gainFactor][spectrumIndex]->zp_setSpectrumVisible(spectrumProperties.second.second);
        }
    }
}
//=========================================================
