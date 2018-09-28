//=================================================
#ifndef ZSAMPLETASK_H
#define ZSAMPLETASK_H
//=================================================
#include <QObject>
#include <QList>
#include <QSqlQuery>
#include "ZCalibration.h"
#include "ZSpeSpectrum.h"
#include "ZSample.h"
//=================================================
class ZMeasuringController;
class ZMeasuringTask;
class ZChemicalTask;
//=================================================
class ZZonedCalibration : public ZCalibration
{
    Q_OBJECT
public:
    explicit ZZonedCalibration(const QString& name,
                               int databaseCalibrationId,
                               QObject *parent = 0)
        : ZCalibration(name, parent)
    {
        zv_minConcentration = 0.0;
        zv_maxConcentration = 0.0;
        zv_databaseCalibrationId = databaseCalibrationId;
    }

    explicit ZZonedCalibration(const ZCalibration* calibration,
                               const QString &name,
                               int databaseCalibrationId,
                               QObject *parent = 0)
        : ZCalibration(calibration, name, parent)
    {
        zv_minConcentration = 0.0;
        zv_maxConcentration = 0.0;
        zv_databaseCalibrationId = databaseCalibrationId;
    }

    double zp_minConcentration() const
    {
        return zv_minConcentration;
    }
    double zp_maxConcentration() const
    {
        return zv_maxConcentration;
    }

    void zp_setMinMaConcentrations(double min, double max)
    {
        if(min > max)
        {
            qSwap(min, max);
        }
        zv_minConcentration = min;
        zv_maxConcentration = max;
    }

    bool zp_isConcentrationInBounds(qreal concentration) const
    {
        return concentration >= zv_minConcentration
                && concentration <= zv_maxConcentration;
    }

    int zp_databaseCalibrationId() const
    {
        return zv_databaseCalibrationId;
    }

private:

    // VARS
    double zv_minConcentration;
    double zv_maxConcentration;
    int zv_databaseCalibrationId;

};
//=================================================
class ZSampleTask : public QObject
{
    Q_OBJECT
public:

    ~ZSampleTask();
    int zp_id() const;
    QString zp_name() const;

    // STATIC FUNCS
    static bool zp_instanceSampleTaskObject(int sampleTaskId,
                                            ZSampleTask*& task,
                                            QString& msg,
                                            ZMeasuringController* measuringController,
                                            QObject *parent);

    void zp_appendClient(QObject *client);
    void zp_removeClient(QObject *client);

    QStringList zp_chemicalStringList() const;
    QStringList zp_measuringConditionsStringlist() const;

    QList<QPair<quint8,int> > zp_measuringConditionsList() const;

    int zp_totalMeasuringDuration() const;

    bool zp_startMeasuring(ZSample* sample);
    bool zp_stopMeasuring(ZSample* sample);

    void zp_measuringFinished();
    void zp_handleSpectrumData(QList<quint32> speDataList,
                               quint8 gainFactor,
                               int exposition, quint32 time, quint32 deadTime,
                               bool finished);

    void zp_calcConcentrations(quint8 gainFactor,
                               int exposition,
                               const ZSpeSpectrum* spectrum,
                               QList<ZChemicalConcentration> &concentrationList) const;

    qint64 zp_spectrumIdForConditions(quint8 gainFactor,
                             int exposition) const;

    QList<double> zp_spectrumEnergyCalibrationForConditions(quint8 gainFactor,
    int exposition) const;

    QList<double> zp_energyCalibrationForGainFactor(quint8 gainFactor) const;
    void zp_setEnergyCalibration(quint8 gainFactor, const QList<double>& energyCalibrationFactorList);


signals:

    void zg_inquiryToDelete();

public slots:



private:

    explicit ZSampleTask(int id,
                         const QString& name,
                         ZMeasuringController* measuringController,
                         QObject *parent);

    // VARS
    ZMeasuringController* zv_measuringController;
    int zv_sampleTaskId;
    QString zv_name;
    QList<QObject*> zv_clientList;
    QList<ZMeasuringTask*> zv_measuringTaskList;
    ZSample* zv_currentSample;

    // FUNCS
    void zh_initMeasuringTasks();

};
//=================================================
class ZMeasuringTask : public QObject
{
    Q_OBJECT
public:
    explicit ZMeasuringTask(int id, QObject* parent);

    QStringList zp_chemicalList() const;
    QString zp_measuringConditionsString() const;
    QPair<quint8,int> zp_measuringConditions() const;
    int zp_exposition() const;

    void zp_calcConcentrations(const ZSpeSpectrum *spectrum,
                               QList<ZChemicalConcentration> &chemicalConcentrationList);
    QList<double> zp_energyCalibrationFactorList() const;
    void zp_setEnergyCalibrationFactorList(const QList<double>& energyCalibrationFactorList);

private:

    // VARS
    int zv_id;
    quint8 zv_gainFactor;
    int zv_exposition;
    QList<double> zv_energyCalibrationFactorList;

    QList<ZChemicalTask*> zv_chemicalTaskList;

    // FUNCS

    void zh_initCalculationTasks(int measurementConditionsHasSampleTaskId);
    void zh_initEnergyCalibration();

};
//=================================================
class ZChemicalTask : public QObject
{
    Q_OBJECT
public:

    QString zp_chemical() const;

    // STATIC FUNCS
    static bool zp_instanceChemicalTaskObject(int chemicalTaskId,
                                              ZChemicalTask*& task,
                                              QString& msg,
                                              QObject *parent);

    bool zp_calculateConcentration(const ZSpeSpectrum* spectrum, ZChemicalConcentration &chemicalConcentration);
private:

    // Constructor
    explicit ZChemicalTask(QString& msg, int chemicalTaskId, QSqlQuery& query, QObject* parent);

    // VARS
    int zv_chemicalTaskId;
    QString zv_taskName;
    QString zv_taskDescription;
    QString zv_chemical;
    QString zv_errorMsg;

    QList<ZZonedCalibration*> zv_calibrationList;


    // FUNCS
    void zh_getChemical(int chemicalId);


};
//=================================================
#endif // ZSAMPLETASK_H
