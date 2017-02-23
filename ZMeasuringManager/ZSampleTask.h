//=================================================
#ifndef ZSAMPLETASK_H
#define ZSAMPLETASK_H
//=================================================
#include <QObject>
#include <QList>
#include <QSqlQuery>
#include "ZCalibration.h"
//=================================================
class ZMeasuringTask;
class ZChemicalTask;
//=================================================
class ZZonedCalibration : public ZCalibration
{
    Q_OBJECT
public:
    explicit ZZonedCalibration(const QString& name, QObject *parent = 0)
        : ZCalibration(name, parent)
    {
        zv_minConcentration = 0.0;
        zv_maxConcentration = 0.0;
    }

    explicit ZZonedCalibration(const ZCalibration* calibration,
                               const QString &name, QObject *parent = 0)
        : ZCalibration(calibration, name, parent)
    {
        zv_minConcentration = 0.0;
        zv_maxConcentration = 0.0;
    }

    // VARS
    double zv_minConcentration;
    double zv_maxConcentration;

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
                                            QObject *parent);

    void zp_appendClient(QObject *client);
    void zp_removeClient(QObject *client);

    QStringList zp_chemicalList() const;
    QStringList zp_measuringConditionsList() const;

signals:

    void zg_requestToDelete();

public slots:



private:

    explicit ZSampleTask(int id, const QString& name, QObject *parent);

    // VARS
    int zv_id;
    QString zv_name;
    QList<QObject*> zv_clientList;
    QList<ZMeasuringTask*> zv_measuringTaskList;

    // FUNCS
    void zh_initMeasuringTasks();


};
//=================================================
class ZMeasuringTask : public QObject
{
    Q_OBJECT
public:
    explicit ZMeasuringTask(int id, int gainFactor, int exposition, QObject* parent);

    QStringList zp_chemicalList() const;
    QString zp_measuringConditions() const;

private:

    // VARS
    int zv_id;
    int zv_gainFactor;
    int zv_exposition;

    QList<ZChemicalTask*> zv_chemicalTaskList;

    // FUNCS

    void zh_initCalculationTasks();

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
