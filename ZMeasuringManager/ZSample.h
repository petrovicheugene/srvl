//=====================================================
#ifndef ZSAMPLE_H
#define ZSAMPLE_H
//=====================================================
#include <QObject>
//=====================================================
class ZSampleTask;
class ZSpeSpectrum;
class ZAbstractSpectrum;
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

    ZSpeSpectrum* zp_spectrumForMeasuringConditions(int gainFactor, int exposition) const;


    QStringList zp_sampleChemicalList() const;
    QStringList zp_sampleMeasuringConditionsStringList() const;
    QList<QPair<int, int> > zp_sampleMeasuringConditionsList() const;
    QStringList zp_sampleTaskChemicalList() const;
    QStringList zp_sampleTaskMeasuringConditionsList() const;

    int zp_sampleTaskId() const;
    int zp_totalMeasuringDuration() const;

signals:

    void  zg_requestSpeListClear(SampleTaskSetFlag& flag);

public slots:

private:

    // VARS
    QString zv_sampleName;
    ZSampleTask* zv_sampleTask;

    QList<QPair<QPair<int, int>, ZSpeSpectrum*> > zv_spectrumList;
    QList<QPair<QString, double> > zv_chemicalConcentrationList;

    // FUNCS



};
//=====================================================
#endif // ZSAMPLE_H
