//======================================================
#ifndef ZMEASURINGMANAGER_H
#define ZMEASURINGMANAGER_H
//======================================================
#include <QCloseEvent>
#include <QColor>
#include <QObject>
#include <QList>
//======================================================
class ZControlAction;
class ZSample;
class ZSampleTask;
//======================================================
class ZMeasuringState
{
public:

    enum MeasuringAction {MA_STOP, MA_RUN, MA_SUSPEND};
    ZMeasuringState()
    {
        measuringAction = MA_STOP;
        seriesDirty = false;
    }

    // VARS
    MeasuringAction measuringAction;

    QString currentSeriesName;
    QString currentSampleName;
    bool seriesDirty;

    int currentSampleRow;
    int measuringSecondLeft;

};
Q_DECLARE_METATYPE(ZMeasuringState)
//======================================================
class ZMeasuringManager : public QObject
{
    Q_OBJECT
public:
    explicit ZMeasuringManager(QObject *parent = 0);
    ~ZMeasuringManager();

    enum SampleOperationType {SOT_SAMPLE_ABOUT_TO_BE_INSERTED,
                              SOT_SAMPLE_INSERTED,
                              SOT_SAMPLE_ABOUT_TO_BE_REMOVED,
                              SOT_SAMPLE_REMOVED,
                              SOT_CONDITIONS_ABOUT_TO_BE_INSERTED,
                              SOT_CONDITIONS_INSERTED,
                              SOT_CONDITIONS_ABOUT_TO_BE_REMOVED,
                              SOT_CONDITIONS_REMOVED,
                              SOT_SPECTRUM_CHANGED,
                              SOT_CONCENTRATIONS_CHANGED};
    // FUNCS
    QList<ZControlAction*> zp_sampleActions() const;
    QList<ZControlAction*> zp_sampleContextActions() const;

    int zp_sampleCount() const;
    QString zp_sampleName(int sampleIndex) const;
    bool zp_setSampleName(int sampleIndex, const QString& name);

    QString zp_sampleTaskName(int sampleIndex) const;
    QStringList zp_chemicalListForSample(int sampleIndex) const;
    QStringList zp_measuringConditionsStringListForSample(int sampleIndex) const;
    QList<QPair<int, int> > zp_measuringConditionsListForSample(int sampleIndex) const;
    QString zp_seriesTaskName() const;
    ZMeasuringState zp_currentMeasuringState() const;

    QList<int> zp_spectrumData(int row, int gainFactor, int exposition);
    int zp_arrayChannelCount(int gainFactor, int exposition) const;
    int zp_arrayMaxIntensity(int gainFactor, int exposition) const;

signals:

    void zg_sampleOperation(SampleOperationType, int first = -1, int last = -1) const;
    void zg_requestSelectedSampleList(QList<int>&);
//    void zg_seriesTaskNameChanged(const QString& taskName) const;
//    void zg_seriesTaskNameDirtyChanged(bool dirty) const;
    void zg_requestCurrentIndex(QModelIndex& currentIndex) const;
    void zg_measuringStateChanged(ZMeasuringState) const;

public slots:

    void zp_notifyOfCurrentStatus();
    void zp_setCurrentSampleIndex(int currentSampleIndex);
    void zp_startSeries();
    void zp_stopSeries();

private slots:

    // action slots:
    void zh_onSaveSeriesAction();
    void zh_onLoadSeriesAction();

    void zh_onAddSamplesToSeriesAction();
    void zh_onRemoveSamplesFromSeriesAction();
    void zh_onLoadSpectraFromFileAction();

    void zh_deleteSampleTask();

    void zh_sampleTaskIdList(QList<int>& idList) const;

private:

    // VARS
    ZControlAction* zv_saveSeriesAction;
    ZControlAction* zv_loadSeriesAction;
    ZControlAction* zv_loadSpectrumFromFileAction;

    ZControlAction* zv_addSamplesToSeriesAction;
    ZControlAction* zv_removeSamplesFromSeriesAction;

    QList<ZSample*> zv_sampleList;
    QList<ZSampleTask*> zv_sampleTaskList;
    int zv_currentSampleIndex;
    bool zv_currentSeriesTaskDirty;
    QString zv_currentSeriesTaskName;
    QString zv_spectrumFolderPath;
    QList<QColor> zv_colorList;


    // FUNCS
    bool zh_checkColor(QColor color);
    void zh_createColorList();
    int zv_lastColorIndex;

    void zh_createActions();
    void zh_createComponents();
    void zh_createConnections();
    void zh_restoreSettings();
    void zh_saveSettings() const;

    int zh_createSample(const QString& sampleName, ZSampleTask *sampleTask); // returns sample index in array
    void zh_clearSamples();
    ZSampleTask* zh_instanceSampleTask(int sampleTaskId);

    int zh_findLastSampleSerialNumber(const QString& sampleName) const;
    bool zh_checkSampleName(const QString& sampleName) const;

    void zh_manageControlEnable();
    bool zh_addSamplesToSeries(int sampleTaskId,
                               int sampleQuantity,
                               QString defaultSampleName = QString());

    void zh_clearSeriesTask();
    bool zh_loadSeriesTask(int seriesTaskId);
    void zh_createCurrentMeasuringState(ZMeasuringState& measuringState) const;
    void zh_notifyMeasuringStateChanged();
    int zh_seriesMeasuringTotalDuration() const;


};
//======================================================
#endif // ZMEASURINGMANAGER_H
