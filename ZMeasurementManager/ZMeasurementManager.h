//======================================================
#ifndef ZMEASUREMENTMANAGER_H
#define ZMEASUREMENTMANAGER_H
//======================================================
#include <QObject>
#include <QList>
//======================================================
class ZControlAction;
class ZSample;
//======================================================
class ZMeasurementManager : public QObject
{
    Q_OBJECT
public:
    explicit ZMeasurementManager(QObject *parent = 0);

    // FUNCS
    QList<ZControlAction*> zp_sampleActions() const;
    QList<ZControlAction*> zp_sampleContextActions() const;

    int zp_sampleCount() const;
    QString zp_sampleName(int sampleIndex) const;
    bool zp_setSampleName(int sampleIndex, const QString& name);

signals:


public slots:

    void zg_setCurrentSampleIndex(int currentSampleIndex);

private slots:

    // action slots:
    void zh_onAddSamplesToSeriesAction();
    void zh_onRemoveSamplesFromSeriesAction();

private:

    // VARS
    ZControlAction* zv_addSamplesToSeriesAction;
    ZControlAction* zv_removeSamplesFronSeriesAction;

    QList<ZSample*> zv_sampleList;
    int zv_currentSampleIndex;

    // FUNCS
    void zh_createActions();
    void zh_createComponents();
    void zh_createConnections();

    void zh_appendSampleToList(const QString& sampleName);
    int zh_findLastSampleSerialNumber(const QString& sampleName) const;
    bool zh_checkSampleName(const QString& sampleName) const;

};
//======================================================
#endif // ZMEASUREMENTMANAGER_H
