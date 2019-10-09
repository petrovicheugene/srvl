//======================================================
#ifndef ZSERIESTABLEMODEL_H
#define ZSERIESTABLEMODEL_H
//======================================================
#include "ZDependentModel.h"
#include "ZSpectrumCommonProperties.h"
//======================================================
class ZSample;
class ZSampleTask;
//======================================================
class ZSeriesTableModel : public ZDependentModel
{
    Q_OBJECT
public:
    explicit ZSeriesTableModel(QObject *parent = nullptr);
    ~ ZSeriesTableModel() override;

    virtual int	columnCount(const QModelIndex & parent = QModelIndex()) const override;
    virtual int	rowCount(const QModelIndex & parent = QModelIndex()) const override;
    virtual QVariant	data(const QModelIndex & index, int role = Qt::DisplayRole) const override;
    virtual QVariant	headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const;


signals:

public slots:

    void zp_reload(qint64 id) override;


private:

    //VARS
    QList<ZSample*> zv_sampleList;
    QList<ZSampleTask*> zv_sampleTaskList;
    QList<ZSpectrumCommonProperties> zv_spectrumCommonPropertiesList;

    QStringList zv_chemicalStringList;
    QStringList zv_measuringConditionsStringList;
    QList<QPair<quint8, int> > zv_measuringConditionsList;


    // FUNCS
    void zh_clearData();
    void zh_recalcColumnCount();
    QStringList zh_chemicalListForSample(int sampleIndex) const;
    QStringList zh_measuringConditionsStringListForSample(int sampleIndex) const;
    QList<QPair<quint8, int> > zh_measuringConditionsListForSample(int sampleIndex) const;

    int zh_createSample(const QString& sampleName,
                                           ZSampleTask* sampleTask);
    ZSampleTask* zh_instanceSampleTask(int sampleTaskId);

};
//======================================================
#endif // ZSERIESTABLEMODEL_H
