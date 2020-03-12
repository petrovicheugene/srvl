//======================================================
#ifndef ZMEASURINGMANAGER_H
#define ZMEASURINGMANAGER_H
//======================================================
#include <QCloseEvent>
#include <QColor>
#include <QDateTime>
#include <QMessageBox>
#include <QModelIndexList>
#include <QObject>
#include <QList>
#include <QDebug>
#include "ZAppSettings.h"
#include "ZSpectrumCommonProperties.h"

//======================================================
class ZControlAction;
class ZSample;
class ZSampleTask;
class ZUralAdcDeviceConnector;
class ZMeasuringController;
class QTimerEvent;
class QMenu;
class ZSpeSpectrum;

//======================================================
class ZMeasuringState
{
public:

    enum MeasuringAction {MA_STOPPED, MA_RUNNING, MA_SUSPENDED};
    ZMeasuringState()
    {
        zv_measuringAction = MA_STOPPED;
        zv_currentSeriesName = QString();
        zv_currentSeriesTaskId = 0;
        zv_currentSampleName = QString();
        zv_seriesResultsDirty = false;
        zv_seriesTaskDirty = false;
        zv_currentSampleRow = -1;

        zv_seriesTimePassed = 0;
        zv_sampleTimePassedMs = 0;

        zv_seriesDuration = 0;
        zv_sampleDuration = 0;
    }

    MeasuringAction zp_measuringAction() {return zv_measuringAction;}
    void zp_setMeasuringAction(MeasuringAction measuringAction)
    {
        zv_measuringAction = measuringAction;
        if(zv_measuringAction == MA_STOPPED)
        {
            zv_currentSampleName = QString();
            zv_currentSampleRow = -1;
            zv_seriesTimePassed = 0;
            zv_sampleTimePassedMs = 0;
            zv_sampleDuration = 0;
        }
    }

    QString zp_currentSeriesName() const {return zv_currentSeriesName;}
    void zp_setSeriesName(const QString& name)
    {
        zv_currentSeriesName = name;
    }

    qint64 zp_currentSeriesTaskId() const {return zv_currentSeriesTaskId;}
    void zp_setSeriesTaskId(qint64 id)
    {
        zv_currentSeriesTaskId = id;
    }

    QString zp_currentSampleName() {return zv_currentSampleName;}
    void zp_setCurrentSampleName(const QString& sampleName)
    {
        zv_currentSampleName = sampleName;
    }

    bool zp_seriesResultsDirty() {return zv_seriesResultsDirty;}
    void zp_setSeriesResultsDirty(bool dirty)
    {
        zv_seriesResultsDirty = dirty;
    }
    bool zp_seriesTaskDirty() {return zv_seriesTaskDirty;}
    void zp_setSeriesTaskDirty(bool dirty)
    {
        zv_seriesTaskDirty = dirty;
    }

    int zp_currentSampleRow(){return zv_currentSampleRow;}
    void zp_incrementSampleRow()
    {
        zv_currentSampleRow++;
    }

    int zp_seriesTimePassed() {return zv_seriesTimePassed;}
    void zp_setSeriesTimePassed(int passed)
    {
        zv_seriesTimePassed = passed;
    }

    qint64 zp_sampleTimePassed() {return zv_sampleTimePassedMs;}
    void zp_setSampleTimePassed(qint64 passedMs)
    {
        zv_sampleTimePassedMs = passedMs;
    }
    void zp_applySampleTimePassed(qint64 passedMs)
    {
        zv_sampleTimePassedMs += passedMs;
    }

    int zp_seriesDuration() {return zv_seriesDuration;}
    void zp_setSeriesDuration(int duration)
    {
        zv_seriesDuration = duration;
    }

    int zp_sampleDuration() {return zv_sampleDuration;}
    void zp_setSampleDuration(int duration)
    {
        zv_sampleDuration = duration;
    }

private:

    // VARS
    MeasuringAction zv_measuringAction;

    QString zv_currentSeriesName;
    qint64 zv_currentSeriesTaskId;
    QString zv_currentSampleName;
    bool zv_seriesResultsDirty;
    bool zv_seriesTaskDirty;

    int zv_currentSampleRow;
    int zv_seriesTimePassed;
    qint64 zv_sampleTimePassedMs;

    int zv_seriesDuration;
    int zv_sampleDuration;

};
Q_DECLARE_METATYPE(ZMeasuringState)
//======================================================
class ZMeasuringManager : public QObject
{
    Q_OBJECT
public:
    explicit ZMeasuringManager(QObject *parent = nullptr);
    ~ZMeasuringManager() override;

    //    enum MeasuringState {MS_STOPPED,
    //                         MS_SUSPENDED,
    //                         MS_ACTIVE};

    enum SampleOperationType {SOT_SAMPLE_ABOUT_TO_BE_INSERTED,
                              SOT_SAMPLE_INSERTED,
                              SOT_SAMPLE_ABOUT_TO_BE_REMOVED,
                              SOT_SAMPLE_REMOVED,
                              SOT_CONDITIONS_ABOUT_TO_BE_INSERTED,
                              SOT_CONDITIONS_INSERTED,
                              SOT_CONDITIONS_ABOUT_TO_BE_REMOVED,
                              SOT_CONDITIONS_REMOVED,
                              SOT_SPECTRUM_CHANGED,
                              SOT_SPECTRUM_VISIBILITY_CHANGED,
                              SOT_CONCENTRATIONS_CHANGED,
                              SOT_SPECTRUM_PROPERTIES_CHANGED};
    // FUNCS
    void zp_applyAppSettings(const ZAppSettings& appSettings);
    QList<ZControlAction*> zp_sampleActions() const;
    QList<ZControlAction*> zp_sampleContextActions() const;
    QList<ZControlAction*> zp_connectionActions() const;
    QList<ZControlAction*> zp_toolActions() const;

    int zp_sampleCount() const;
    QString zp_sampleName(int sampleIndex) const;
    int zp_indexForSampleName(const QString& sampleName) const;
    bool zp_setSampleName(int sampleIndex, const QString& name);

    void zp_appendActionsToMenu(QMenu* menu) const;

    QString zp_sampleTaskName(int sampleIndex) const;
    int zp_sampleTaskId(int sampleIndex) const;
    QStringList zp_chemicalListForSample(int sampleIndex) const;
    QStringList zp_measuringConditionsStringListForSample(int sampleIndex) const;
    QList<QPair<quint8, int> > zp_measuringConditionsListForSample(int sampleIndex) const;
    QString zp_seriesTaskName() const;
    ZMeasuringState zp_currentMeasuringState() const;

    bool zp_concentration(int row, const QString &chemical, double &concentration);

    QList<quint32> zp_spectrumData(int row, int gainFactor, int exposition);
    ZSpeSpectrum* zp_spectrum(int row, quint8 gainFactor, int exposition);
    int zp_arrayChannelCount(int gainFactor, int exposition) const;
    int zp_arrayMaxIntensity(int gainFactor, int exposition) const;

    QColor zp_spectrumColor(int sampleRow, int gainFactor, int exposition) const;
    bool zp_spectrumVisibility(int sampleRow, int gainFactor, int exposition, bool& visibility) const;
    bool zp_setSpectrumVisibility(int sampleRow, int gainFactor, int exposition, bool visibility);
    bool zp_spectrumVisibility(qint64 spectrumId, bool& visibility) const;
    bool zp_libraryState() const;
    //bool zp_connectionState() const;

    ZSpeSpectrum* zp_spectrumForId(qint64 id) const;
    QDateTime zp_currentMeasuringStartDateTime() const;
    QDateTime zp_currentMeasuringFinishDateTime() const;
    QString zp_currentSeriesName() const;

signals:

    void zg_connectionState(QString connectionState, QMessageBox::Icon) const;
    void zg_message(const QString& msg, QMessageBox::Icon) const;
    void zg_sampleOperation(SampleOperationType, int first = -1, int last = -1) const;
    void zg_inquirySelectedSampleList(QList<int>&);
    void zg_inquirySelectedSpectrumMap(QMap< QPair<quint8, int>, QList<ZSpeSpectrum*> >& spectrumMap) const;

    void zg_inquiryMeasuringConditionsAndSpectrumForIndex(const QModelIndex& currentIndex,
                                                          quint8& gainFactor,
                                                          int& exposition,
                                                          const ZSpeSpectrum*& spectrum) const;

    //    void zg_seriesTaskNameChanged(const QString& taskName) const;
    //    void zg_seriesTaskNameDirtyChanged(bool dirty) const;

    void zg_inquiryCurrentIndex(QModelIndex& currentIndex) const;
    void zg_invokeNotifyCurrent();
    void zg_measuringStateChanged(ZMeasuringState) const;
    void zg_measurementParameters(quint32 expoPassedMs,
                                  quint32 deadTimeMs,
                                  quint32 spectrumIntensityIntegral) const;

    void zg_currentEnergyCalibrationChanged(QList<double> energyCalibrationFactorList) const;
    void zg_inquiryCurrentVisibleSceneRect(QRectF& sceneRect) const;

    //void zg_currentSpectrumChanged(int row, int conditionsIndex);
    void zg_currentSpectrumChanged(qint64 currentSpectrumId) const;

    void zg_inquiryResultsPrinting() const;
    void zg_inquiryResultsPreviewAndPrinting() const;
    void zg_inquiryCurrentOperatorId(int& opratorId) const;

public slots:

    void zp_onSelectionChange();
    void zp_notifyOfCurrentStatus();
    // void zp_setCurrentSampleIndex(int currentSampleIndex);
    void zp_startSeries();
    void zp_stopSeries();
    void zh_onCurrentSpectrumChange(qint64 spectrumId);
    void zh_currentEnergyCalibrationChanged(QList<double> energyCalibrationFactorList);

private slots:

    // action slots:
    void zh_onSaveSeriesAction();
    void zh_onLoadSeriesAction();
    void zh_onAddSamplesToSeriesAction();
    void zh_onRemoveSamplesFromSeriesAction();
    void zh_onLoadSpectraFromFilesAction();
    void zh_onSaveSpectraToFilesAction() const;
    void zh_onEnergyCalibrationAction();
    void zh_onPrintAction() const;
    void zh_onPreviewAndPrintAction() const;

    void zh_deleteSampleTask();
    void zh_sampleTaskIdList(QList<int>& idList) const;

    void zh_nextSpectrumColor(QColor& color);

    void zh_onConnectToDeviceAction();
    void zh_onExpositionPassedMSec(qint64 expoPassedMs);
    void zh_notifyMeasuringStateChanged();
    void zh_onCurrentSpectrumMeasuring(qint64 currentMeasuredSpectrumId);
    void zh_onCurrentEnergyCalibration(QList<double> energyCalibrationFactorList);
    void zh_onEnergyCalibrationChange(int gainFactor,
                                      const QList<double>& energyCalibrationFactorList);

    // sample measuring
    void zh_onSampleMeasuringFinish();
    void zh_onSpectrumDataChange(quint8 gainFactor, int exposition);
    void zh_onConcentrationChange();


protected:

    void timerEvent(QTimerEvent* event) override;

private:

    // VARS
    ZUralAdcDeviceConnector* zv_UralAdcDeviceConnector;
    ZMeasuringController* zv_measuringController;

    ZControlAction* zv_saveSeriesAction;
    ZControlAction* zv_loadSeriesAction;
    ZControlAction* zv_loadSpectraFromFilesAction;
    ZControlAction* zv_saveSpectraToFilesAction;
    ZControlAction* zv_printAction;
    ZControlAction* zv_previewAndPrintAction;

    ZControlAction* zv_addSamplesToSeriesAction;
    ZControlAction* zv_removeSamplesFromSeriesAction;

    ZControlAction* zv_energyCalibrationAction;
    ZControlAction* zv_spectrumInfoAction;

    QList<ZControlAction*> zv_connectionActionList;

    QList<ZSample*> zv_sampleList;
    QList<ZSampleTask*> zv_sampleTaskList;
    QList<ZSpectrumCommonProperties> zv_spectrumCommonPropertiesList;

    // bool zv_currentSeriesTaskDirty;
    // QString zv_currentSeriesTaskName;
    QString zv_spectrumFolderPath;
    QList<QColor> zv_colorList;

    const QString zv_deviceLibraryDefaultFileName = "ComDV.dll";
    int zv_deviceSampleQuantity;
    int zv_expositionDelayDuration;
    int zv_expositionDelayTimer;
    int zv_totalSeriesDuration;
    int zv_seriesTimePassed;
    QDateTime zv_startDateTime;
    QDateTime zv_finishDateTime;
    qint64 zv_seriesId;

    ZMeasuringState zv_currentMeasuringState;

    // FUNCS
    bool zh_checkColor(QColor color);
    void zh_createColorList();
    int zv_lastColorIndex;

    void zh_createActions();
    void zh_createComponents();
    void zh_createConnections();
    void zh_restoreSettings();
    void zh_saveSettings() const;
    bool zh_createLibraryFromResources(const QString& libraryFileName, QString &errorMsg);

    int zh_createSample(const QString& sampleName, ZSampleTask *sampleTask); // returns sample index in array
    void zh_clearSampleList();
    void zh_resetMeasuringResults();

    ZSampleTask* zh_instanceSampleTask(int sampleTaskId);

    int zh_findLastSampleSerialNumber(const QString& sampleName) const;
    bool zh_checkSampleName(const QString& sampleName) const;

    void zh_manageControlEnable();
    bool zh_addSamplesToSeries(int sampleTaskId,
                               int sampleQuantity,
                               QString defaultSampleName = QString());

    void zh_clearSeriesTask();
    bool zh_loadSeriesTask(int seriesTaskId);

    void zh_calcSpectrumCommonProperties(quint8 gainFactor, int exposition);

    bool zh_connectToDevice(QString deviceName );
    void zh_setConnectionActionsEnable(bool enabling);
    void zh_recalcSeriesMeasuringTotalDuration();

//    void zh_getSpectraFromIndexes(const QModelIndexList& selectedIndexes,
//                                  QMap< QPair<quint8, int>, QList<ZSpeSpectrum*> >& spectrumMap) const;

    void zh_assignNewSeriesId();
    void zh_saveSampleMeasurementResult();
    bool zh_recordSeriesId();
    bool zh_findNewIdInTable(const QString& tableName, int& newId);

};
//======================================================
#endif // ZMEASURINGMANAGER_H
