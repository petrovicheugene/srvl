//==================================================================
#ifndef ZMEASUREMENTRESULTTABLEMODEL_H
#define ZMEASUREMENTRESULTTABLEMODEL_H
//==================================================================
#include <QAbstractTableModel>
//==================================================================
class ZSeriesTaskTreeModel;
//==================================================================
class ZMeasurementResultTableModel : public QAbstractTableModel
{
    Q_OBJECT
public:
    explicit ZMeasurementResultTableModel(QObject *parent = 0);

    void zp_connectToSourceModel(ZSeriesTaskTreeModel* sourceModel);

    // override FUNCS
    virtual Qt::ItemFlags	flags(const QModelIndex & index) const;
    QModelIndex index(int row, int column, const QModelIndex &parent) const;

    virtual int	columnCount(const QModelIndex & parent = QModelIndex()) const;
    virtual int	rowCount(const QModelIndex & parent = QModelIndex()) const;

    virtual QVariant	data(const QModelIndex & index, int role = Qt::DisplayRole) const;
    virtual bool	setData(const QModelIndex & index, const QVariant & value, int role = Qt::EditRole);
    virtual QVariant	headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const;


signals:



public slots:



private slots:

    void zh_onRowsAboutToBeInserted(const QModelIndex &parent, int start, int end);
    void zh_onRowsAboutToBeRemoved(const QModelIndex &parent, int start, int end);
    void zh_onRowsInserted(const QModelIndex &parent, int start, int end);
    void zh_onRowsRemoved(const QModelIndex &parent, int start, int end);

private:

    // VARS
    ZSeriesTaskTreeModel* zv_sourceModel;
};
//==================================================================
#endif // ZMEASUREMENTRESULTTABLEMODEL_H
