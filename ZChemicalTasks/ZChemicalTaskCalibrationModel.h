//==========================================================
#ifndef ZCHEMICALTASKCALIBRATIONMODEL_H
#define ZCHEMICALTASKCALIBRATIONMODEL_H
//==========================================================
#include <QAbstractTableModel>
//==========================================================
class QSqlTableModel;
//==========================================================
struct ZCalibratiionItem
{
    int calibrationId;
    int calibrationRow;
    double minConcentration;
    double maxConcentration;

    ZCalibratiionItem()
    {
        calibrationId = -1;
        calibrationRow = -1;
        minConcentration = 0.0;
        maxConcentration = 0.0;
    }

    ZCalibratiionItem(int id)
    {
        calibrationId = id;
        calibrationRow = -1;
        minConcentration = 0.0;
        maxConcentration = 0.0;

    }
};
//==========================================================
class ZChemicalTaskCalibrationModel : public QAbstractTableModel
{
    Q_OBJECT
public:
    explicit ZChemicalTaskCalibrationModel(QObject *parent = 0);

    void zp_connectToCalibrationSQLTableModel(QSqlTableModel *model);

    // overrided FUNCS
    virtual Qt::ItemFlags flags(const QModelIndex &index) const;
    virtual QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;
    virtual bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole);
    virtual QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const;
    virtual QModelIndex index(int row, int column, const QModelIndex &parent = QModelIndex()) const;
    virtual int columnCount(const QModelIndex &parent = QModelIndex()) const;
    virtual int rowCount(const QModelIndex &parent = QModelIndex()) const;

    virtual bool removeRows(int row, int count, const QModelIndex &parent = QModelIndex());

    void zp_addCalibrationIdToFilter(const QList<int> idList);

    bool zp_moveRowUp(int row);
    bool zp_moveRowDown(int row);

    bool zp_limitsForRow(int row, double& min, double& max) const;
    QString zp_calibrationName(int row) const;

    int zh_calibrationId(int row) const;

signals:

public slots:

private slots:

    //    void zh_columnsAboutToBeInserted(const QModelIndex &parent, int first, int last);
    //    void zh_columnsAboutToBeMoved(const QModelIndex &sourceParent, int sourceStart, int sourceEnd, const QModelIndex &destinationParent, int destinationColumn);
    //    void zh_columnsAboutToBeRemoved(const QModelIndex &parent, int first, int last);
    //    void zh_columnsInserted(const QModelIndex &parent, int first, int last);
    //    void zh_columnsMoved(const QModelIndex &parent, int start, int end, const QModelIndex &destination, int column);
    //    void zh_columnsRemoved(const QModelIndex &parent, int first, int last);
    //void zh_headerDataChanged(Qt::Orientation orientation, int first, int last);

    void zh_sourceDataChanged(const QModelIndex &topLeft, const QModelIndex &bottomRight, const QVector<int> &roles = QVector<int> ());
    //void zh_layoutAboutToBeChanged(const QList<QPersistentModelIndex> &parents = QList<QPersistentModelIndex> (), QAbstractItemModel::LayoutChangeHint hint = QAbstractItemModel::NoLayoutChangeHint);
    //void zh_layoutChanged(const QList<QPersistentModelIndex> &parents = QList<QPersistentModelIndex> (), QAbstractItemModel::LayoutChangeHint hint = QAbstractItemModel::NoLayoutChangeHint);
    void zh_sourceModelAboutToBeReset();
    void zh_sourceModelReset();
    void zh_sourceRowsAboutToBeInserted(const QModelIndex &parent, int start, int end);
    //void zh_rowsAboutToBeMoved(const QModelIndex &sourceParent, int sourceStart, int sourceEnd, const QModelIndex &destinationParent, int destinationRow);
    void zh_sourceRowsAboutToBeRemoved(const QModelIndex &parent, int first, int last);
    void zh_sourceRowsInserted(const QModelIndex &parent, int first, int last);
    //void zh_rowsMoved(const QModelIndex &parent, int start, int end, const QModelIndex &destination, int row);
    void zh_sourceRowsRemoved(const QModelIndex &parent, int first, int last);

private:

    // VARS
    QSqlTableModel* zv_calibrationSQLTableModel;
    // QMap<int, QPair<double, double> > zv_idAndLimitsMap;
    // QMap<int, int> zv_rowForIdMap;
    QList<ZCalibratiionItem> zv_calibrationItemList;

    // FUNCS
    QModelIndex zh_mapIndexFromSource(const QModelIndex& sourceIndex) const;
    bool zh_calibrationRowForItem(int calibrationItemIndex) const;
    QVariant zh_sourceModelData(int row, int column, int role = Qt::DisplayRole) const;
    int zh_calibrationIdForSourceRow(int row) const;

};
//==========================================================
#endif // ZCHEMICALTASKCALIBRATIONMODEL_H
