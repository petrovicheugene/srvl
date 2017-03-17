//=========================================================
#ifndef ZMEASURINGRESULTTABLEMODEL_H
#define ZMEASURINGRESULTTABLEMODEL_H
//=========================================================
#include <QAbstractTableModel>
#include "ZMeasuringManager.h"
//=========================================================

//=========================================================
class ZMeasuringResultTableModel : public QAbstractTableModel
{
    Q_OBJECT
public:
    explicit ZMeasuringResultTableModel(QObject *parent = 0);
    void zp_connectToMeasuringManager(ZMeasuringManager* measuringManager);

    // override FUNCS
    virtual Qt::ItemFlags	flags(const QModelIndex & index) const;
    QModelIndex index(int row, int column, const QModelIndex &parent) const;

    virtual int	columnCount(const QModelIndex & parent = QModelIndex()) const;
    virtual int	rowCount(const QModelIndex & parent = QModelIndex()) const;

    virtual QVariant	data(const QModelIndex & index, int role = Qt::DisplayRole) const;
    virtual bool	setData(const QModelIndex & index, const QVariant & value, int role = Qt::EditRole);
    virtual QVariant	headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const;

    int zp_spectrumColumnCount() const;

signals:

public slots:

protected slots:
    void zh_onSampleOperation(ZMeasuringManager::SampleOperationType type,
                              int first,
                              int last);

private:

    // VARS
    ZMeasuringManager* zv_measuringManager;
    QStringList zv_chemicalStringList;
    QStringList zv_measuringConditionsStringList;
    QList<QPair<int, int> > zv_measuringConditionsList;

    // FUNCS
    void zh_recalcColumnCount();

};
//=========================================================
#endif // ZMEASURINGRESULTTABLEMODEL_H
