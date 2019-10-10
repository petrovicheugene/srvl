//=====================================================
#include "ZSample.h"

#include <QDateTime>
#include <QDebug>

#include "ZSampleTask.h"
#include "ZSpeSpectrum.h"
//=====================================================
ZSample::ZSample(const QString &sampleName,
                 ZSampleTask* sampleTask,
                 QObject *parent)
    : QObject(parent)
{
    timerId = -1;
    zv_sampleTask = nullptr;
    zv_sampleName = sampleName;
    zp_setSampleTask(sampleTask, STSF_CLEAR_SPE_LIST);
}
//=====================================================
ZSample::~ZSample()
{
    if(zv_sampleTask != nullptr)
    {
        zv_sampleTask->zp_removeClient(dynamic_cast<QObject*>(this));
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

    if(zv_sampleTask != nullptr)
    {
        zv_sampleTask->zp_removeClient(dynamic_cast<QObject*>(this));
    }

    zv_sampleTask = sampleTask;
    zv_sampleTask->zp_appendClient(dynamic_cast<QObject*>(this));

    // chemical-concentration list
    //    QStringList chemicalStringList = zv_sampleTask->zp_chemicalStringList();
    QMap<int, QString> chemicalMap = zv_sampleTask->zp_chemicalMap();

    // replace without cheking
    zv_chemicalConcentrationList.clear();
    //    foreach(QString chemical, chemicalStringList)
    //    {
    //        ZChemicalConcentration chemicalConcentration;
    //        chemicalConcentration.zv_chemical = chemical;
    //        chemicalConcentration.zv_chemicalId =
    //        zv_chemicalConcentrationList.append(chemicalConcentration);
    //    }
    QMap<int, QString>::iterator it;
    for(it = chemicalMap.begin(); it != chemicalMap.end(); it++)
    {
        ZChemicalConcentration chemicalConcentration;
        chemicalConcentration.zv_chemical = it.value();
        chemicalConcentration.zv_chemicalId = it.key();
        zv_chemicalConcentrationList.append(chemicalConcentration);
    }

    // spectrum list
    QList<QPair<quint8,int> >  measuringConditionsList = zv_sampleTask->zp_measuringConditionsList();
    QPair<quint8, int> currentMeasuringConditions;
    int existingMeasuringConditionsQuantity;
    int insertedMeasuringConditionsQuantity;
    ZSpeSpectrum* spectrum;
    ZSpeAuxData speAuxdata;
    QColor color;
    QList<quint32> intensityList;

    if(zv_spectrumList.isEmpty())
    {
        if(!measuringConditionsList.isEmpty())
        {
            // create spectrum element
            QPair<QPair<int, int>, ZSpeSpectrum*> speElement;
            for(int i = 0; i < measuringConditionsList.count(); i++)
            {
                speElement.first = measuringConditionsList.at(i);

                speAuxdata.zp_setEnergyUnit("kEv");
                speAuxdata.zp_setGainFactor(measuringConditionsList.at(i).first);

                QList<double> energyCalibrationFactorList =
                        zv_sampleTask->zp_energyCalibrationForGainFactor(measuringConditionsList.at(i).first);

                speAuxdata.zp_setEnergyCalibrationFactors(energyCalibrationFactorList);

                emit zg_inquirySpectrumColor(color);
                spectrum = new ZSpeSpectrum(intensityList, speAuxdata,
                                            QString(), color, false, this);

                spectrum->zp_setSpectrumVisible(false);
                speElement.second = spectrum;
                zv_spectrumList.append(speElement);
            }
        }
        return;
    }
    else if(measuringConditionsList.isEmpty())
    {
        if(flag == STSF_ASK)
        {
            emit zg_inquirySpeListClear(flag);
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
                    emit zg_inquirySpeListClear(flag);
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
                    speElement.second = nullptr;
                    zv_spectrumList.append(speElement);
                }
            }
        }
    }
}
//=====================================================
QString ZSample::zp_sampleTaskName() const
{
    if(zv_sampleTask == nullptr)
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
    spectrumElement.second = nullptr;
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
bool ZSample::zp_setSpectrumData(QList<quint32> speDataList,
                                 quint8 gainFactor,
                                 int exposition,
                                 quint32 time,
                                 quint32 deadTime,
                                 bool finished)
{
    for(int s = 0; s < zv_spectrumList.count(); s++)
    {
        if(zv_spectrumList.at(s).first.first == gainFactor && zv_spectrumList.at(s).first.second == exposition)
        {
            // delete the previous spectrum
            if(zv_spectrumList[s].second == nullptr)
            {
                // create new SpeSpectrum
                // spectrum->setParent(this);
            }

            // assign the new spectrum
            ZSpeSpectrum* spectrum = zv_spectrumList[s].second;
            spectrum->zp_setSpectrumData(speDataList);
            spectrum->zp_setCompleted(finished);

            emit zg_spectrumDataChanged(gainFactor, exposition);
            if(finished)
            {
                // set date, time, exposition, aliveTime, gainfactor
                spectrum->zp_setSpectrumDateTime(QDateTime::currentDateTime());
                spectrum->zp_setExposition(exposition);
                spectrum->zp_setGainFactor(gainFactor);
                spectrum->zp_setAliveTime(static_cast<quint32>(exposition) - (deadTime / 1000));
                //spectrum->zp_setEnergyUnit("kEv");

                QList<ZChemicalConcentration> chemicalConcentrationList;
                zv_sampleTask->zp_calcConcentrations(gainFactor,
                                                     exposition,
                                                     spectrum,
                                                     chemicalConcentrationList);
                // update concentration list
                QString currentChemical;
                for(int i = 0; i < zv_chemicalConcentrationList.count(); i++ )
                {
                    currentChemical = zv_chemicalConcentrationList.at(i).zv_chemical;
                    foreach(ZChemicalConcentration chemicalConcentration, chemicalConcentrationList)
                    {
                        if(chemicalConcentration.zv_chemical == currentChemical)
                        {
                            zv_chemicalConcentrationList[i].zv_concentration = chemicalConcentration.zv_concentration;
                            zv_chemicalConcentrationList[i].zv_calibrationId = chemicalConcentration.zv_calibrationId;
                            break;
                        }
                    }
                }

                // notify measuring manager
                emit zg_concentrationChanged();
            }
            return true;
        }
    }

    return false;
}
//=====================================================
bool ZSample::zp_setConcentration(const QString& chemical, double concentration)
{
    for(int i = 0; i < zv_chemicalConcentrationList.count(); i++)
    {
        if( zv_chemicalConcentrationList[i].zv_chemical == chemical)
        {
            zv_chemicalConcentrationList[i].zv_concentration = concentration;
            //zv_chemicalConcentrationList[i].zv_calibrationId = chemicalConcentration.zv_calibrationId;
            // notify measuring manager
            emit zg_concentrationChanged();
            return true;
        }
    }

    return false;
}
//=====================================================
ZSpeSpectrum* ZSample::zp_spectrumForMeasuringConditions(quint8 gainFactor, int exposition) const
{
    for(int s = 0; s < zv_spectrumList.count(); s++)
    {
        if(zv_spectrumList.at(s).first.first == gainFactor && zv_spectrumList.at(s).first.second == exposition)
        {
            return zv_spectrumList.at(s).second;
        }
    }

    return nullptr;
}
//=====================================================
bool ZSample::zp_spectrumVisibilityForSpectrumId(qint64 id, bool& visibility) const
{
    foreach(auto &spectrumData, zv_spectrumList)
    {
        if(spectrumData.second->zp_spectrumId() == id)
        {
            visibility = spectrumData.second->zp_isSpectrumVisible();
            return true;
        }
    }

    return false;
}
//=====================================================
QStringList ZSample::zp_sampleChemicalList() const
{
    QStringList chemicalList;
    for(int i = 0; i < zv_chemicalConcentrationList.count(); i++)
    {
        chemicalList.append(zv_chemicalConcentrationList.at(i).zv_chemical);
    }

    return chemicalList;
}
//=====================================================
QMap<int, QString> ZSample::zp_sampleChemicalMap() const
{
    QMap<int, QString> chemicalMap;

    for(int i = 0; i < zv_chemicalConcentrationList.count(); i++)
    {
        chemicalMap.insert(zv_chemicalConcentrationList.at(i).zv_chemicalId,
                           zv_chemicalConcentrationList.at(i).zv_chemical);
    }

    return chemicalMap;
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
QList<QPair<quint8, int> > ZSample::zp_sampleMeasuringConditionsList() const
{
    QList<QPair<quint8, int> > measuringConditionsList;
    for(int i = 0; i < zv_spectrumList.count(); i++)
    {
        measuringConditionsList.append(zv_spectrumList.at(i).first);
    }

    return measuringConditionsList;
}
//=====================================================
QStringList ZSample::zp_sampleTaskChemicalList() const
{
    if(zv_sampleTask == nullptr)
    {
        return QStringList();
    }

    return zv_sampleTask->zp_chemicalStringList();
}
//=====================================================
QStringList ZSample::zp_sampleTaskMeasuringConditionsList() const
{
    if(zv_sampleTask == nullptr)
    {
        return QStringList();
    }

    return zv_sampleTask->zp_measuringConditionsStringlist();
}
//=====================================================
int ZSample::zp_sampleTaskId() const
{
    if(zv_sampleTask == nullptr)
    {
        return -1;
    }

    return zv_sampleTask->zp_id();
}
//=====================================================
int ZSample::zp_totalMeasuringDuration() const
{
    if(zv_sampleTask == nullptr)
    {
        return 0;
    }

    return zv_sampleTask->zp_totalMeasuringDuration();
}
//=====================================================
bool ZSample::zp_startMeasuring()
{
    if(!zv_sampleTask)
    {
        return false;
    }

    return zv_sampleTask->zp_startMeasuring(this);
}
//=====================================================
bool ZSample::zp_stopMeasuring()
{
    if(!zv_sampleTask)
    {
        return false;
    }

    return zv_sampleTask->zp_stopMeasuring(this);
}
//=====================================================
void ZSample::zp_measuringFinished()
{
    emit zg_measuringFinished();
}
//=====================================================
bool ZSample::zp_concentration(const QString& chemical, double& concentration) const
{
    foreach(ZChemicalConcentration chemicalConcentration, zv_chemicalConcentrationList)
    {
        if(chemicalConcentration.zv_chemical == chemical)
        {
            concentration = chemicalConcentration.zv_concentration;
            return true;
        }
    }
    return false;
}
//=====================================================
bool ZSample::zp_concentration(int chemicalId, double& concentration) const
{
    foreach(ZChemicalConcentration chemicalConcentration, zv_chemicalConcentrationList)
    {
        if(chemicalConcentration.zv_chemicalId == chemicalId)
        {
            concentration = chemicalConcentration.zv_concentration;
            return true;
        }
    }
    return false;
}
//=====================================================
void ZSample::zp_resetMeasuringResults()
{
    // clear spectra
    QList<quint32> zeroSpectrumData;
    for(int s = 0; s < zv_spectrumList.count(); s++)
    {
        zv_spectrumList.at(s).second->zp_setSpectrumData(zeroSpectrumData);
    }

    // clear calc results
    for(int c = 0; c < zv_chemicalConcentrationList.count(); c++)
    {
        zv_chemicalConcentrationList[c].zv_concentration = 0.0;
    }
}
//=====================================================
void ZSample::zp_setEnergyCalibration(int gainFactor, const QList<double>& energyCalibrationFactorList)
{
    for(auto& spectrumData : zv_spectrumList)
    {
        if(spectrumData.first.first == gainFactor)
        {
            spectrumData.second->zp_setEnergyCalibration(energyCalibrationFactorList);
        }
    }
}
//=====================================================
ZSpeSpectrum* ZSample::zp_spectrumForId(qint64 id)
{
    for(auto& spectrumData : zv_spectrumList)
    {
        if(spectrumData.second->zp_spectrumId() == id)
        {
            return spectrumData.second;
        }
    }

    return nullptr;
}
//=====================================================

