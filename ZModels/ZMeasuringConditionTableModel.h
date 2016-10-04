//======================================================
#ifndef ZMEASURINGCONDITIONTABLEMODEL_H
#define ZMEASURINGCONDITIONTABLEMODEL_H
//======================================================
#include <QAbstractTableModel>
//======================================================

//======================================================
class ZMeasuringConditionTableModel : public QAbstractTableModel
{
    Q_OBJECT
public:
    explicit ZMeasuringConditionTableModel(QObject *parent = 0);

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


};
//======================================================
#endif // ZMEASURINGCONDITIONTABLEMODEL_H
