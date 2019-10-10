//======================================================
#include "ZGeneral.h"
#include "ZSeriesTableModel.h"
#include "ZSample.h"
#include "ZSampleTask.h"
#include "ZSpectrumPaintData.h"

#include <QDataStream>
#include <QDebug>
#include <QSqlError>
#include <QSqlQuery>

//======================================================
ZSeriesTableModel::ZSeriesTableModel(QObject *parent)
    : ZDependentModel(parent)
{
    zv_concentrationDisplayPrecisioin = 3;
}
//======================================================
ZSeriesTableModel::~ ZSeriesTableModel()
{
    beginResetModel();
    zh_clearData();
    endResetModel();
}
//======================================================
Qt::ItemFlags ZSeriesTableModel::flags(const QModelIndex & index) const
{
    Qt::ItemFlags flags;
    flags |= Qt::ItemIsEnabled | Qt::ItemIsSelectable;
    if(index.isValid() && index.column() < 2 )
    {
        flags |= Qt::ItemIsEditable;
    }

    return flags;
}
//======================================================
int ZSeriesTableModel::columnCount(const QModelIndex & parent) const
{
    if(zv_sampleList.isEmpty())
    {
        return 0;
    }

    return 2 + zv_chemicalStringList.count() + zv_measuringConditionsStringList.count();
}
//======================================================
int	 ZSeriesTableModel::rowCount(const QModelIndex & parent) const
{
    return zv_sampleList.count();
}
//======================================================
QVariant	ZSeriesTableModel::data(const QModelIndex & index, int role) const
{
    if(!index.isValid() || index.row() < 0 ||index.row() >= rowCount()
            || index.column() < 0 || index.column() >= columnCount())
    {
        return QVariant();
    }

    if(role == Qt::DisplayRole)
    {
        if(index.column() == 0)
        {
            return QVariant(zv_sampleList.at(index.row())->zp_sampleName());
        }

        if(index.column() == 1)
        {
            return QVariant(zv_sampleList.at(index.row())->zp_sampleTaskName());
        }

        // spectrum columns
        if(index.column() > 1 && index.column() < zv_measuringConditionsList.count() + 2)
        {
            ZSpectrumPaintData paintData;
            quint8 gainFactor = zv_measuringConditionsList.at(index.column() - 2).first;
            int exposition = zv_measuringConditionsList.at(index.column() - 2).second;

            paintData.spectrumData = zh_spectrumData(index.row(), gainFactor, exposition);
            paintData.maxChannel = zh_arrayChannelCount(gainFactor, exposition);
            paintData.maxIntensity = zh_arrayMaxIntensity(gainFactor, exposition);

            return QVariant::fromValue(paintData);
        }

        // Chem concentration columns
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
            if(!zh_concentration(index.row(), chemical, concentration))
            {
                return QVariant();
            }

            QString concentrationString = QString::number(concentration, 'f', zv_concentrationDisplayPrecisioin);
            return QVariant(concentrationString);
        }
    }

    if(role == Qt::DecorationRole)
    {
        if(index.column() > 1 && index.column() < zv_measuringConditionsStringList.count() + 2)
        {

            int gainFactor = zv_measuringConditionsList.value(index.column() - 2, QPair<int, int>(-1, -1)).first;
            int exposition = zv_measuringConditionsList.value(index.column() - 2, QPair<int, int>(-1, -1)).second;

            QColor spectrumColor = zp_spectrumColor(index.row(), gainFactor, exposition);
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
            if(!zh_spectrumVisibility(index.row(), measuringConditions.first, measuringConditions.second, visibility))
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
//======================================================
bool ZSeriesTableModel::setData(const QModelIndex & index, const QVariant & value, int role)
{
    if(!index.isValid() || !value.isValid())
    {
        return false;
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
        bool res = zp_setSpectrumVisibility(index.row(), measuringConditions.first, measuringConditions.second, visibility);
        if(res)
        {
            emit dataChanged(index, index);
        }

        return res;
    }

    return false;
}
//======================================================
QVariant ZSeriesTableModel::headerData(int section, Qt::Orientation orientation, int role) const
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
//======================================================
void ZSeriesTableModel::zp_reload(qint64 id)
{
    beginResetModel();
    zh_clearData();

    QString queryString = QString("SELECT measured_samples.id, "
                                  "measured_samples.name, "
                                  "measured_samples.sample_tasks_id,"
                                  "sample_tasks.name "
                                  "FROM measured_samples "
                                  "JOIN sample_tasks "
                                  "ON measured_samples.sample_tasks_id=sample_tasks.id "
                                  "WHERE series_id=%1").arg(QString::number(id));


    QSqlQuery query;
    if(query.prepare(queryString) && query.exec())
    {
        QVariant vData;
        while(query.next())
        {
            // sample task
            ZSampleTask* task = nullptr;
            vData = query.value(2);
            if(vData.isValid() && vData.canConvert<int>())
            {
                task = zh_instanceSampleTask(vData.toInt());
            }

            // sample
            vData = query.value(1);
            int sampleIndex = -1;
            if(vData.isValid() && vData.canConvert<QString>())
            {
                sampleIndex = zh_createSample(vData.toString(), task);
            }

            // sample id
            vData = query.value(0);
            int sampleId = -1;
            if(vData.isValid() && vData.canConvert<int>())
            {
                sampleId = vData.toInt();
            }

            ZSample* sample = zv_sampleList.at(sampleIndex);
            zh_setMeasuredDataToSample(sample, sampleId);
        }
    }
    else
    {
        // report error
    }

    zh_recalcColumnCount();

    endResetModel();
}
//======================================================
void ZSeriesTableModel::zh_setMeasuredDataToSample(ZSample* sample, int sampleId)
{
    // concentrations
    QString queryString = QString("SELECT measured_chemicals.chemicals_id, "
                                  "measured_chemicals.value,"
                                  "chemicals.name "
                                  "FROM measured_chemicals "
                                  "JOIN chemicals "
                                  "ON measured_chemicals.chemicals_id=chemicals.id "
                                  "WHERE measured_chemicals.measured_samples_id=%1").arg(QString::number(sampleId));

    QSqlQuery query;
    if(query.prepare(queryString) && query.exec())
    {
        while(query.next())
        {
            // write concentrations to sample
            // value
            double concentrationVal = 0.0;
            QVariant vData = query.value(1);
            if(vData.isValid() && vData.canConvert<double>())
            {
                concentrationVal = vData.toDouble();
            }

            // chemical
            QString chemicalString;
            vData = query.value(2);
            if(vData.isValid() && vData.canConvert<QString>())
            {
                chemicalString = vData.toString();
            }

            sample->zp_setConcentration(chemicalString, concentrationVal);
        }
    }

    // spectra
    queryString = QString("SELECT measured_spectra.spectrum_data, "
                          "measuring_conditions.gain_factor, "
                          "measuring_conditions.exposition "
                          "FROM measured_spectra "
                          "JOIN measuring_conditions "
                          "ON measured_spectra.measuring_conditions_id=measuring_conditions.id "
                          "WHERE measured_spectra.measured_samples_id=%1").arg(QString::number(sampleId));
    query.clear();

    if(query.prepare(queryString) && query.exec())
    {
        while(query.next())
        {
            // write spectra to sample
            // measuring conditions
            //            double concentrationVal = 0.0;
            int gainFactor = -1;
            QVariant vData = query.value(1);
            if(vData.isValid() && vData.canConvert<int>())
            {
                gainFactor = vData.toInt();
            }

            int exposition = -1;
            vData = query.value(2);
            if(vData.isValid() && vData.canConvert<int>())
            {
                exposition = vData.toInt();
            }

            // spectrum
            const QList<quint32> intensityList;
            const ZSpeAuxData speAuxdata;
            const QString path;

            ZSpeSpectrum* spectrum = new ZSpeSpectrum(intensityList,
                                                      speAuxdata, path, QColor(), true, this);
            vData = query.value(0);
            if(vData.isValid() && vData.canConvert<QByteArray>())
            {
                QByteArray byteArray;
                byteArray = vData.toByteArray();
                QDataStream dataStream(&byteArray, QIODevice::ReadWrite);
                dataStream >> *spectrum;
                spectrum->setParent(sample);

                sample->zp_setSpectrum(spectrum, gainFactor, exposition);

            }

            zh_calcSpectrumCommonProperties(static_cast<quint8>(gainFactor), exposition);


            //            // chemical
            //            QString chemicalString;
            //            vData = query.value(2);
            //            if(vData.isValid() && vData.canConvert<QString>())
            //            {
            //                chemicalString = vData.toString();
            //            }

            //            sample->zp_setConcentration(chemicalString, concentrationVal);
        }
    }


}
//======================================================
void ZSeriesTableModel::zh_clearData()
{
    for(int i = zv_sampleList.count() - 1; i >= 0; i--)
    {
        ZSample* sample = zv_sampleList.takeAt(i);
        delete sample;
    }

    for(int i = zv_sampleTaskList.count() - 1; i >= 0; i--)
    {
        ZSampleTask* sampleTask = zv_sampleTaskList.takeAt(i);
        delete sampleTask;
    }

    zv_spectrumCommonPropertiesList.clear();

    zv_chemicalStringList.clear();
    zv_measuringConditionsStringList.clear();
    zv_measuringConditionsList.clear();
}
//======================================================
void ZSeriesTableModel::zh_recalcColumnCount()
{
    QStringList chemicalStringList;
    QStringList currentSampleChemicalStringList;

    QStringList measuringConditionsStringList;
    QStringList currentSampleMeasuringConditionsStringList;
    zv_measuringConditionsList.clear();

    for(int row = 0; row < zv_sampleList.count(); row++)
    {
        // chemicals
        currentSampleChemicalStringList = zh_chemicalListForSample(row);
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
        measuringConditionsListForSample = zh_measuringConditionsListForSample(row);

        for(int c = 0; c < measuringConditionsListForSample.count(); c++)
        {
            if(!zv_measuringConditionsList.contains(measuringConditionsListForSample.at(c)))
            {
                zv_measuringConditionsList.append(measuringConditionsListForSample.at(c));
            }
        }

        currentSampleMeasuringConditionsStringList = zh_measuringConditionsStringListForSample(row);
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
//======================================================
QStringList ZSeriesTableModel::zh_chemicalListForSample(int sampleIndex) const
{
    if(sampleIndex < 0 || sampleIndex >= zv_sampleList.count())
    {
        return QStringList();
    }

    return zv_sampleList.at(sampleIndex)->zp_sampleChemicalList();
}
//======================================================
QStringList ZSeriesTableModel::zh_measuringConditionsStringListForSample(int sampleIndex) const
{
    if(sampleIndex < 0 || sampleIndex >= zv_sampleList.count())
    {
        return QStringList();
    }

    return zv_sampleList.at(sampleIndex)->zp_sampleMeasuringConditionsStringList();
}
//======================================================
QList<QPair<quint8, int> > ZSeriesTableModel::zh_measuringConditionsListForSample(int sampleIndex) const
{
    if(sampleIndex < 0 || sampleIndex >= zv_sampleList.count())
    {
        return QList<QPair<quint8, int> >();
    }

    return zv_sampleList.at(sampleIndex)->zp_sampleMeasuringConditionsList();
}
//======================================================
int ZSeriesTableModel::zh_createSample(const QString& sampleName,
                                       ZSampleTask* sampleTask)
{
    ZSample* sample = new ZSample(sampleName, nullptr, this);
    //    connect(sample, &ZSample::zg_inquirySpectrumColor,
    //            this, &ZMeasuringManager::zh_nextSpectrumColor);
    //    connect(sample, &ZSample::zg_measuringFinished,
    //            this, &ZMeasuringManager::zh_onSampleMeasuringFinish);
    //    connect(sample, &ZSample::zg_spectrumDataChanged,
    //            this, &ZMeasuringManager::zh_onSpectrumDataChange);
    //    connect(sample, &ZSample::zg_concentrationChanged,
    //            this, &ZMeasuringManager::zh_onConcentrationChange);


    sample->zp_setSampleTask(sampleTask, ZSample::STSF_CLEAR_SPE_LIST);
    int sampleIndex = zv_sampleList.count();
    //    emit zg_sampleOperation(SOT_SAMPLE_ABOUT_TO_BE_INSERTED, sampleIndex, sampleIndex);
    zv_sampleList.append(sample);
    //    emit zg_sampleOperation(SOT_SAMPLE_INSERTED, sampleIndex, sampleIndex);
    return sampleIndex;
}
//======================================================
ZSampleTask* ZSeriesTableModel::zh_instanceSampleTask(int sampleTaskId)
{
    if(sampleTaskId < 0)
    {
        return nullptr;
    }

    // search in already created
    foreach(ZSampleTask* task, zv_sampleTaskList)
    {
        if(task->zp_id() == sampleTaskId)
        {
            return task;
        }
    }

    // create new sampleTask
    ZSampleTask* task;
    QString msg;
    if(ZSampleTask::zp_instanceSampleTaskObject(sampleTaskId,
                                                task,
                                                msg,
                                                nullptr,
                                                this))
    {
        zv_sampleTaskList.append(task);
        return task;
    }

    return nullptr;
}
//======================================================
QList<quint32> ZSeriesTableModel::zh_spectrumData(int row, int gainFactor, int exposition) const
{
    ZSpeSpectrum* spectrum = zv_sampleList.at(row)->zp_spectrumForMeasuringConditions(static_cast<quint8>(gainFactor),
                                                                                      exposition);
    if(!spectrum)
    {
        return QList<quint32>();
    }

    return spectrum->zp_spectrumData();
}
//======================================================
int ZSeriesTableModel::zh_arrayChannelCount(int gainFactor, int exposition) const
{
    foreach(ZSpectrumCommonProperties properties, zv_spectrumCommonPropertiesList)
    {
        if(properties.gainFactor == gainFactor && properties.exposition == exposition)
        {
            return properties.channelCount;
        }
    }

    return 0;
}
//======================================================
int ZSeriesTableModel::zh_arrayMaxIntensity(int gainFactor, int exposition) const
{
    foreach(ZSpectrumCommonProperties properties,  zv_spectrumCommonPropertiesList)
    {
        if(properties.gainFactor == gainFactor && properties.exposition == exposition)
        {
            return properties.maxValue;
        }
    }

    return 0;
}
//======================================================
bool ZSeriesTableModel::zh_concentration(int row,
                                         const QString& chemical,
                                         double& concentration) const
{
    if(row < 0 || row >= zv_sampleList.count())
    {
        return false;
    }

    return zv_sampleList.at(row)->zp_concentration(chemical, concentration);
}
//======================================================
QColor ZSeriesTableModel::zp_spectrumColor(int sampleRow, int gainFactor, int exposition) const
{
    if(sampleRow < 0 || sampleRow >= zv_sampleList.count())
    {
        return QColor();
    }

    ZSpeSpectrum* spectrum = zv_sampleList.at(sampleRow)->zp_spectrumForMeasuringConditions(static_cast<quint8>(gainFactor), exposition);
    if(!spectrum)
    {
        return QColor();
    }

    return spectrum->zp_color();
}
//======================================================
bool ZSeriesTableModel::zh_spectrumVisibility(int sampleRow,
                                              int gainFactor, int exposition, bool& visibility) const
{
    if(sampleRow < 0 || sampleRow >= zv_sampleList.count())
    {
        return false;
    }

    ZSpeSpectrum* spectrum = zv_sampleList.at(sampleRow)->zp_spectrumForMeasuringConditions(static_cast<quint8>(gainFactor), exposition);
    if(!spectrum)
    {
        return false;
    }

    visibility = spectrum->zp_isSpectrumVisible();
    return true;
}
//======================================================
void ZSeriesTableModel::zh_calcSpectrumCommonProperties(quint8 gainFactor, int exposition)
{
    ZSpeSpectrum* spectrum;
    int channelCount = 0;
    int maxValue = 0;
    int currentChannelCount;
    int currentMaxValue;

    // spectrum for measuring conditions cycle
    for(int i = 0; i < zv_sampleList.count(); i++)
    {
        spectrum = zv_sampleList.at(i)->zp_spectrumForMeasuringConditions(gainFactor, exposition);
        if(!spectrum)
        {
            continue;
        }

        currentChannelCount = spectrum->zp_channelCount();
        if(channelCount < currentChannelCount)
        {
            channelCount = currentChannelCount;
        }

        currentMaxValue = spectrum->zp_maxIntensity();
        if(maxValue < currentMaxValue)
        {
            maxValue = currentMaxValue;
        }
    }

    // find and redefine spectrum properties
    ZSpectrumCommonProperties properties;
    for(int i = 0; i < zv_spectrumCommonPropertiesList.count(); i++)
    {
        properties = zv_spectrumCommonPropertiesList.at(i);
        if(properties.gainFactor == gainFactor
                && properties.exposition == exposition)
        {
            zv_spectrumCommonPropertiesList[i].channelCount = channelCount;
            zv_spectrumCommonPropertiesList[i].maxValue = maxValue;
            return;
        }
    }

    // if spectrum properties doesn't exist create it
    properties.gainFactor = gainFactor;
    properties.exposition = exposition;
    properties.channelCount = channelCount;
    properties.maxValue = maxValue;

    zv_spectrumCommonPropertiesList.append(properties);
}
//======================================================
bool ZSeriesTableModel::zp_setSpectrumVisibility(int sampleRow, int gainFactor, int exposition, bool visibility)
{
    if(sampleRow < 0 || sampleRow >= zv_sampleList.count())
    {
        return false;
    }

    ZSpeSpectrum* spectrum = zv_sampleList.at(sampleRow)->zp_spectrumForMeasuringConditions(static_cast<quint8>(gainFactor), exposition);
    if(!spectrum)
    {
        return false;
    }

    spectrum->zp_setSpectrumVisible(visibility);

    return true;
}
//======================================================


