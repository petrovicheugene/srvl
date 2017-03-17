//=====================================================
#include "ZSample.h"
#include "ZSampleTask.h"
#include "ZSpeSpectrum.h"
//=====================================================
ZSample::ZSample(const QString &sampleName,
                 ZSampleTask* sampleTask,
                 QObject *parent)
    : QObject(parent)
{
    zv_sampleTask = 0;
    zv_sampleName = sampleName;
    zp_setSampleTask(sampleTask, STSF_CLEAR_SPE_LIST);
}
//=====================================================
ZSample::~ZSample()
{
    if(zv_sampleTask != 0)
    {
        zv_sampleTask->zp_removeClient(this);
    }
}
//=====================================================
QString ZSample::zp_sampleName() const
{
    return zv_sampleName;
}
//=====================================================
bool ZSample::zp_setSampleName(const QString& sampleName)
{
    if(sampleName.isEmpty() || zv_sampleName == sampleName)
    {
        return false;
    }

    zv_sampleName = sampleName;
    return true;
}
//=====================================================
void ZSample::zp_setSampleTask(ZSampleTask* sampleTask,
                               SampleTaskSetFlag flag)
{
    if(sampleTask == zv_sampleTask)
    {
        return;
    }

    if(zv_sampleTask != 0)
    {
        zv_sampleTask->zp_removeClient(this);
    }

    zv_sampleTask = sampleTask;
    zv_sampleTask->zp_appendClient(this);

    // chemical-concentration list
    QStringList chemicalStringList = zv_sampleTask->zp_chemicalStringList();

    // replace without cheking
    zv_chemicalConcentrationList.clear();
    QPair<QString, double> chemicalPair;
    foreach(QString chemical, chemicalStringList)
    {
        chemicalPair.first = chemical;
        chemicalPair.second = 0.0;
        zv_chemicalConcentrationList.append(chemicalPair);
    }

    // spectrum list
    QList<QPair<int,int> >  measuringConditionsList = zv_sampleTask->zp_measuringConditionsList();
    QPair<int, int> currentMeasuringConditions;
    int existingMeasuringConditionsQuantity;
    int insertedMeasuringConditionsQuantity;

    if(zv_spectrumList.isEmpty())
    {
        if(!measuringConditionsList.isEmpty())
        {
            // create spectrum element
            QPair<QPair<int, int>, ZSpeSpectrum*> speElement;
            for(int i = 0; i < measuringConditionsList.count(); i++)
            {
                speElement.first = measuringConditionsList.at(i);
                speElement.second = 0;
                zv_spectrumList.append(speElement);
            }
        }
        return;
    }
    else if(measuringConditionsList.isEmpty())
    {
        if(flag == STSF_ASK)
        {
            emit zg_requestSpeListClear(flag);
        }

        if(flag == STSF_CLEAR_SPE_LIST)
        {
            // clear spectra
            for(int e = zv_spectrumList.count() - 1; e >= 0; e--)
            {
                if(zv_spectrumList.at(e).second != 0)
                {
                    delete zv_spectrumList.at(e).second;
                }
                zv_spectrumList.removeAt(e);
            }
        }
        return;
    }
    else
    {
        for(int i = measuringConditionsList.count() - 1; i >= 0; i--)
        {

            currentMeasuringConditions = measuringConditionsList.at(i);
            // calc and check quantity
            existingMeasuringConditionsQuantity = 0;
            for(int e = 0; e < zv_spectrumList.count(); e++)
            {
                if(zv_spectrumList.at(e).first == currentMeasuringConditions)
                {
                    existingMeasuringConditionsQuantity++;
                }
            }
            insertedMeasuringConditionsQuantity = 0;
            for(int i = 0; i < measuringConditionsList.count(); i++)
            {
                if(measuringConditionsList.at(i) == currentMeasuringConditions)
                {
                    insertedMeasuringConditionsQuantity++;
                }
            }

            if(existingMeasuringConditionsQuantity == insertedMeasuringConditionsQuantity)
            {
                continue;
            }

            if(existingMeasuringConditionsQuantity > insertedMeasuringConditionsQuantity)
            {
                if(flag == STSF_ASK)
                {
                    emit zg_requestSpeListClear(flag);
                }

                if(flag == STSF_NOT_CLEAR_SPE_LIST)
                {
                    continue;
                }

                if(flag == STSF_CLEAR_SPE_LIST)
                {
                    for(int e = zv_spectrumList.count() - 1; e >= 0; e--)
                    {
                        if(zv_spectrumList.at(e).first != currentMeasuringConditions)
                        {
                            continue;
                        }

                        if(zv_spectrumList.at(e).second != 0)
                        {
                            delete zv_spectrumList.at(e).second;
                        }
                        zv_spectrumList.removeAt(e);

                        if(--existingMeasuringConditionsQuantity <= insertedMeasuringConditionsQuantity)
                        {
                            break;
                        }
                    }
                    continue;
                }
            }

            if(existingMeasuringConditionsQuantity < insertedMeasuringConditionsQuantity)
            {
                QPair<QPair<int, int>, ZSpeSpectrum*> speElement;
                for(int i = 0; i < insertedMeasuringConditionsQuantity - existingMeasuringConditionsQuantity; i++)
                {
                    speElement.first = currentMeasuringConditions;
                    speElement.second = 0;
                    zv_spectrumList.append(speElement);
                }
            }
        }
    }
}
//=====================================================
QString ZSample::zp_sampleTaskName() const
{
    if(zv_sampleTask == 0)
    {
        return QString();
    }

    return zv_sampleTask->zp_name();
}
//=====================================================
bool ZSample::zp_addMeasuringConditions(int gainFactor, int exposition)
{
    for(int s = 0; s < zv_spectrumList.count(); s++)
    {
        if(zv_spectrumList.at(s).first.first == gainFactor && zv_spectrumList.at(s).first.second == exposition)
        {
             return false;
        }
    }

    // create new spectrum list element
    QPair<QPair<int, int>, ZSpeSpectrum*>  spectrumElement;
    spectrumElement.first = QPair<int, int>(gainFactor, exposition);
    spectrumElement.second = 0;
    zv_spectrumList.append(spectrumElement);
    return true;
}
//=====================================================
bool ZSample::zp_setSpectrum(ZSpeSpectrum *spectrum, int gainFactor, int exposition)
{
    // replace if spectrum with such measuring conditions exists
    for(int s = 0; s < zv_spectrumList.count(); s++)
    {
        if(zv_spectrumList.at(s).first.first == gainFactor && zv_spectrumList.at(s).first.second == exposition)
        {
            // delete the previous spectrum
            if(zv_spectrumList[s].second != 0)
            {
                delete zv_spectrumList[s].second;
            }

            // assign the new spectrum
            zv_spectrumList[s].second = spectrum;
            spectrum->setParent(this);
            return true;
        }
    }

    // create new spectrum list element
    QPair<QPair<int, int>, ZSpeSpectrum*>  spectrumElement;
    spectrumElement.first = QPair<int, int>(gainFactor, exposition);
    spectrumElement.second = spectrum;
    spectrum->setParent(this);
    zv_spectrumList.append(spectrumElement);
    return true;
}
//=====================================================
ZSpeSpectrum* ZSample::zp_spectrumForMeasuringConditions(int gainFactor, int exposition) const
{
    for(int s = 0; s < zv_spectrumList.count(); s++)
    {
        if(zv_spectrumList.at(s).first.first == gainFactor && zv_spectrumList.at(s).first.second == exposition)
        {
            return zv_spectrumList.at(s).second;
        }
    }

    return 0;
}
//=====================================================
QStringList ZSample::zp_sampleChemicalList() const
{
    QStringList chemicalList;
    for(int i = 0; i < zv_chemicalConcentrationList.count(); i++)
    {
        chemicalList.append(zv_chemicalConcentrationList.at(i).first);
    }

    return chemicalList;
}
//=====================================================
QStringList ZSample::zp_sampleMeasuringConditionsStringList() const
{
    QStringList measuringConditionsList;
    int gainFactor;
    int exposition;
    for(int i = 0; i < zv_spectrumList.count(); i++)
    {
        gainFactor = zv_spectrumList.at(i).first.first;
        exposition = zv_spectrumList.at(i).first.second;
        measuringConditionsList.append(tr("G.F. - %1, Exp. - %2").arg(QString::number(gainFactor),
                                                                      QString::number(exposition)));
    }

    return measuringConditionsList;
}
//=====================================================
QList<QPair<int, int> > ZSample::zp_sampleMeasuringConditionsList() const
{
    QList<QPair<int, int> > measuringConditionsList;
    for(int i = 0; i < zv_spectrumList.count(); i++)
    {
        measuringConditionsList.append(zv_spectrumList.at(i).first);
    }

    return measuringConditionsList;
}
//=====================================================
QStringList ZSample::zp_sampleTaskChemicalList() const
{
    if(zv_sampleTask == 0)
    {
        return QStringList();
    }

    return zv_sampleTask->zp_chemicalStringList();
}
//=====================================================
QStringList ZSample::zp_sampleTaskMeasuringConditionsList() const
{
    if(zv_sampleTask == 0)
    {
        return QStringList();
    }

    return zv_sampleTask->zp_measuringConditionsStringlist();
}
//=====================================================
int ZSample::zp_sampleTaskId() const
{
    if(zv_sampleTask == 0)
    {
        return -1;
    }

    return zv_sampleTask->zp_id();
}
//=====================================================
int ZSample::zp_totalMeasuringDuration() const
{
    if(zv_sampleTask == 0)
    {
        return 0;
    }

    return zv_sampleTask->zp_totalMeasuringDuration();
}
//=====================================================
