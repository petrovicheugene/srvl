//=========================================================
#ifndef ZMEASURINGRESULTTABLEMODEL_H
#define ZMEASURINGRESULTTABLEMODEL_H
//=========================================================
#include <QAbstractTableModel>
#include <QTextDocument>
#include "ZMeasuringManager.h"

//=========================================================
class ZSpeSpectrum;
//=========================================================
class ZMeasuringResultTableModel : public QAbstractTableModel
{
    Q_OBJECT
public:
    explicit ZMeasuringResultTableModel(QObject *parent = nullptr);
    void zp_connectToMeasuringManager(ZMeasuringManager* measuringManager);

    // override FUNCS
    virtual Qt::ItemFlags flags(const QModelIndex & index) const;
    virtual QModelIndex index(int row, int column, const QModelIndex &parent) const;
    virtual int columnCount(const QModelIndex & parent = QModelIndex()) const;
    virtual int rowCount(const QModelIndex & parent = QModelIndex()) const;
    virtual QVariant data(const QModelIndex & index, int role = Qt::DisplayRole) const;
    virtual bool setData(const QModelIndex & index, const QVariant & value, int role = Qt::EditRole);
    virtual QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const;

    // custom funcs
    int zp_spectrumColumnCount() const;
    ZSpeSpectrum* zp_spectrum(int row, quint8 gainFactor, int exposition) const;
    bool zp_measuringConditionsForColumn(int column, QPair<quint8, int> &measuringConditions) const;

    void zp_setConcentrationDisplayPrecisioin(int precision);

signals:

    void zg_currentEnergyCalibrationChanged(QList<double> energyCalibrationFactorList) const;
    void zg_selectedModelIndexList(QModelIndexList& ) const;
    void zg_currentSpectrumChanged(qint64 spectrumId) const;

public slots:

    //void zp_spectraColumnList(QList<int>& );
    void zp_onCurrentIndexChanged(const QModelIndex& current, const QModelIndex& previous);
    bool zp_measuringConditionsAndSpectrumForIndex(const QModelIndex& index,
                                        quint8& gainFactor,
                                        int& exposition,
                                        const ZSpeSpectrum*& spectrum);
    void zp_selectedSpectrumMap(QMap<QPair<quint8, int>, QList<ZSpeSpectrum *> > &spectrumMap);

protected slots:

    void zh_onSampleOperation(ZMeasuringManager::SampleOperationType type,
                              int first,
                              int last);

private:

    // VARS
    ZMeasuringManager* zv_measuringManager;
    QStringList zv_chemicalStringList;
    QStringList zv_measuringConditionsStringList;
    QList<QPair<quint8, int> > zv_measuringConditionsList;
    int zv_concentrationDisplayPrecisioin;

    // FUNCS
    void zh_recalcColumnCount();
    void zh_repaintAllSpectra();
    void zh_sortIndexesForRow(QModelIndexList& indexList, int leftMargin, int rightMargin) const;

};
//=========================================================
#endif // ZMEASURINGRESULTTABLEMODEL_H
