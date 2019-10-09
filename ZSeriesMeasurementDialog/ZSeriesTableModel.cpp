//======================================================
#include "ZSeriesTableModel.h"
#include "ZSample.h"
#include "ZSampleTask.h"


#include <QDebug>
#include <QSqlError>
#include <QSqlQuery>

//======================================================
ZSeriesTableModel::ZSeriesTableModel(QObject *parent)
    : ZDependentModel(parent)
{

}
//======================================================
ZSeriesTableModel::~ ZSeriesTableModel()
{
    beginResetModel();
    zh_clearData();
    endResetModel();
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
            return zv_sampleList.at(index.row())->zp_sampleName();
        }

        if(index.column() == 1)
        {
            return zv_sampleList.at(index.row())->zp_sampleTaskName();
        }
    }

    return QVariant();
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
    qDebug() << "RELOAD DATA" << id;


    beginResetModel();
    zh_clearData();

    QString queryString = QString("SELECT measured_samples.name, "
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
            vData = query.value(1);
            if(vData.isValid() && vData.canConvert<int>())
            {
                task = zh_instanceSampleTask(vData.toInt());
            }

            vData = query.value(0);
            if(vData.isValid() && vData.canConvert<QString>())
            {
                zh_createSample(vData.toString(), task);
            }
        }


    }
    else
    {
        // report error
    }



    // setQuery(queryString);


    //    SELECT measured_samples.name,
    //    sample_tasks.name,
    //    measured_spectra.id,
    //    measured_spectra.spectrum_data
    //    FROM measured_samples
    //    JOIN sample_tasks
    //    ON measured_samples.sample_tasks_id=sample_tasks.id
    //    JOIN measured_spectra
    //    ON measured_spectra.measured_samples_id=measured_samples.id
    //    WHERE series_id=





    //    if (lastError().isValid())
    //    {
    //          qDebug() << lastError();
    //    }
    zh_recalcColumnCount();

    endResetModel();
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
