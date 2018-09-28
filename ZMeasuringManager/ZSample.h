//=====================================================
#ifndef ZSAMPLE_H
#define ZSAMPLE_H
//=====================================================
#include <QObject>
//=====================================================
class ZSampleTask;
class ZSpeSpectrum;
class ZAbstractSpectrum;
class QTimerEvent;
//=====================================================
class ZChemicalConcentration
{
public:
    ZChemicalConcentration()
    {
        zv_chemical = QString();
        zv_calibrationId = -1;
        zv_concentration = 0.0;
    }

    // VARS
    QString zv_chemical;
    int zv_calibrationId;
    double zv_concentration;

};
//=====================================================
class ZSample : public QObject
{
    Q_OBJECT
public:
    enum SampleTaskSetFlag{STSF_ASK,
                           STSF_CLEAR_SPE_LIST,
                           STSF_NOT_CLEAR_SPE_LIST};

    explicit ZSample(const QString& sampleName,
                     ZSampleTask* sampleTask,
                     QObject *parent);
    ~ZSample();

    QString zp_sampleName() const;
    bool zp_setSampleName(const QString& name);
    void zp_setSampleTask(ZSampleTask *sampleTask, SampleTaskSetFlag flag);
    QString zp_sampleTaskName() const;
    bool zp_addMeasuringConditions(int gainFactor, int exposition);
    bool zp_setSpectrum(ZSpeSpectrum*spectrum, int gainFactor, int exposition);
    bool zp_setSpectrumData(QList<quint32> speDataList,
                            quint8 gainFactor,
                            int exposition, quint32 time, quint32 deadTime,
                            bool finished);
    ZSpeSpectrum* zp_spectrumForMeasuringConditions(quint8 gainFactor, int exposition) const;
    bool zp_spectrumVisibilityForSpectrumId(qint64 id, bool& visibility) const;


    QStringList zp_sampleChemicalList() const;
    QStringList zp_sampleMeasuringConditionsStringList() const;
    QList<QPair<quint8, int> > zp_sampleMeasuringConditionsList() const;
    QStringList zp_sampleTaskChemicalList() const;
    QStringList zp_sampleTaskMeasuringConditionsList() const;

    int zp_sampleTaskId() const;
    int zp_totalMeasuringDuration() const;

    bool zp_startMeasuring();
    bool zp_stopMeasuring();
    void zp_measuringFinished();

    bool zp_concentration(const QString& chemical, double& concentration);
    void zp_resetMeasuringResults();

    void zp_setEnergyCalibration(int gainFactor, const QList<double>& energyCalibrationFactorList);

    ZSpeSpectrum* zp_spectrumForId(qint64 id);

signals:

    void zg_inquirySpeListClear(SampleTaskSetFlag& flag) const;
    void zg_inquirySpectrumColor(QColor& color) const;
    void zg_measuringFinished() const;
    void zg_spectrumDataChanged(quint8 gainfactor, int exposition);
    void zg_concentrationChanged();


public slots:

protected:

private:

    // VARS
    QString zv_sampleName;
    ZSampleTask* zv_sampleTask;
    int timerId;

    QList<QPair<QPair<quint8, int>, ZSpeSpectrum*> > zv_spectrumList;
    QList<ZChemicalConcentration> zv_chemicalConcentrationList;

    // FUNCS



};
//=====================================================
#endif // ZSAMPLE_H
