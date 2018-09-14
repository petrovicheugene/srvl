//=========================================================
#ifndef ZENERGYCALIBRATIONSPECTRUMMOPDEL_H
#define ZENERGYCALIBRATIONSPECTRUMMOPDEL_H
//=========================================================
#include <QAbstractTableModel>

#include <QList>
#include <QMap>
//=========================================================
class ZSpeSpectrum;
//=========================================================
class ZEnergyCalibrationSpectrumTableModel : public QAbstractTableModel
{
    Q_OBJECT
public:
    explicit ZEnergyCalibrationSpectrumTableModel(QObject *parent = nullptr);
    ~ZEnergyCalibrationSpectrumTableModel();

    // override FUNCS
    virtual Qt::ItemFlags flags(const QModelIndex & index) const;
    QModelIndex index(int row, int column, const QModelIndex &parent) const;
    virtual int	columnCount(const QModelIndex & parent = QModelIndex()) const;
    virtual int	rowCount(const QModelIndex & parent = QModelIndex()) const;
    virtual QVariant	data(const QModelIndex & index, int role = Qt::DisplayRole) const;
    virtual bool setData(const QModelIndex & index, const QVariant & value, int role = Qt::EditRole);
    virtual QVariant	headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const;

    void zp_loadSpectrumData(QMap<quint8, QList<ZSpeSpectrum *> > &spectrumMap);

    ZSpeSpectrum* zp_spectrumForRow(int row);

    void zp_writeEnergyCalibrationToCheckedSpectra(QList<double> &factorList);

signals:


public slots:

    void zp_setGainFactorFilter(quint8 currentGainFactor);

private:

    // VARS
    QMap<quint8, QList<ZSpeSpectrum*> > zv_spectrumMap;
    QMap<quint8, QList<QPair<ZSpeSpectrum*, QPair<bool, bool> > > > zv_spectrumVisibilityMap;

    quint8 zv_gainFactorFilter;
    int zv_maxChannelNumber;
    int zv_maxIntensity;

    // FUNCS
    void zh_recalcMaxSpectrumParameters();
    void zh_saveSpectrumVisibleAndCheckedState();
    void zh_restoreSpectrumVisibleAndCheckedState();

};
//=========================================================
#endif // ZENERGYCALIBRATIONSPECTRUMMOPDEL_H
