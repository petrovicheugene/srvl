//=======================================================
#ifndef ZCALIBRATIONINTASKTABLEMODEL_H
#define ZCALIBRATIONINTASKTABLEMODEL_H
//=======================================================
#include <QAbstractTableModel>
#include <QList>
//=======================================================
//=======================================================
class ZCalibrationInTaskTableModel : public QAbstractTableModel
{
    Q_OBJECT
public:
    explicit ZCalibrationInTaskTableModel(QObject *parent = 0);

    // override FUNCS
    Qt::ItemFlags	flags(const QModelIndex & index) const;
    QModelIndex index(int row, int column, const QModelIndex &parent) const;

    int	columnCount(const QModelIndex & parent = QModelIndex()) const;
    int	rowCount(const QModelIndex & parent = QModelIndex()) const;

    QVariant	data(const QModelIndex & index, int role = Qt::DisplayRole) const;
    bool	setData(const QModelIndex & index, const QVariant & value, int role = Qt::EditRole);
    QVariant	headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const;


signals:

public slots:


private:

    // VARS

    // FUNCS

};
//=======================================================
#endif // ZCALIBRATIONINTASKTABLEMODEL_H
