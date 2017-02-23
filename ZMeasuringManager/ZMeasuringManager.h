//======================================================
#ifndef ZMEASURINGMANAGER_H
#define ZMEASURINGMANAGER_H
//======================================================
#include <QObject>
#include <QList>
//======================================================
class ZControlAction;
class ZSample;
class ZSampleTask;
//======================================================
class ZMeasuringManager : public QObject
{
    Q_OBJECT
public:
    explicit ZMeasuringManager(QObject *parent = 0);
    enum SampleOperationType {SOT_SAMPLE_ABOUT_TO_BE_INSERTED,
                              SOT_SAMPLE_INSERTED,
                              SOT_SAMPLE_ABOUT_TO_BE_REMOVED,
                              SOT_SAMPLE_REMOVED};
    // FUNCS
    QList<ZControlAction*> zp_sampleActions() const;
    QList<ZControlAction*> zp_sampleContextActions() const;

    int zp_sampleCount() const;
    QString zp_sampleName(int sampleIndex) const;
    bool zp_setSampleName(int sampleIndex, const QString& name);

    QString zp_sampleTaskName(int sampleIndex) const;
    QStringList zp_chemicalListForSample(int sampleIndex) const;
    QStringList zp_measuringConditionsListForSample(int sampleIndex) const;

    QString zp_seriesTaskName() const;

signals:

    void zg_sampleOperation(SampleOperationType, int first = -1, int last = -1) const;
    void zg_requestSelectedSampleList(QList<int>&);
    void zg_seriesTaskNameChanged(const QString& taskName) const;
    void zg_seriesTaskNameDirtyChanged(bool dirty) const;

public slots:

    void zg_setCurrentSampleIndex(int currentSampleIndex);

private slots:

    // action slots:
    void zh_onSaveSeriesAction();
    void zh_onLoadSeriesAction();

    void zh_onAddSamplesToSeriesAction();
    void zh_onRemoveSamplesFromSeriesAction();
    void zh_deleteSampleTask();

    void zh_sampleTaskIdList(QList<int>& idList) const;

private:

    // VARS
    ZControlAction* zv_saveSeriesAction;
    ZControlAction* zv_loadSeriesAction;

    ZControlAction* zv_addSamplesToSeriesAction;
    ZControlAction* zv_removeSamplesFromSeriesAction;

    QList<ZSample*> zv_sampleList;
    QList<ZSampleTask*> zv_sampleTaskList;
    int zv_currentSampleIndex;
    bool zv_currentSeriesTaskDirty;
    QString zv_currentSeriesTaskName;

    // FUNCS
    void zh_createActions();
    void zh_createComponents();
    void zh_createConnections();

    void zh_createSample(const QString& sampleName, ZSampleTask *sampleTask);
    ZSampleTask* zh_instanceSampleTask(int sampleTaskId);

    int zh_findLastSampleSerialNumber(const QString& sampleName) const;
    bool zh_checkSampleName(const QString& sampleName) const;

    void zh_manageControlEnable();
    bool zh_addSamplesToSeries(int sampleTaskId,
                               int sampleQuantity,
                               QString defaultSampleName = QString());

    void zh_clearSeriesTask();
    bool zh_loadSeriesTask(int seriesTaskId);

};
//======================================================
#endif // ZMEASURINGMANAGER_H
