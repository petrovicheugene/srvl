//=============================================================
#ifndef ZSELECTEDENERGYLINETABLEMODEL_H
#define ZSELECTEDENERGYLINETABLEMODEL_H
//=============================================================
#include <QAbstractTableModel>
#include <QMap>

#include "ZEnergyLineSetItem.h"

//=============================================================
typedef QList<QPair<QString, QString> > PropertyList;
//=============================================================
class ZSelectedEnergyLineTableModel : public QAbstractTableModel
{
    Q_OBJECT
public:
    explicit ZSelectedEnergyLineTableModel(QObject *parent = nullptr);

    // reimplemented FUNCS
    Qt::ItemFlags	flags(const QModelIndex & index) const override;
    int	columnCount(const QModelIndex & parent = QModelIndex()) const override;
    int	rowCount(const QModelIndex & parent = QModelIndex()) const override;
    QVariant	data(const QModelIndex & index, int role = Qt::DisplayRole) const override;
    bool setData(const QModelIndex & index, const QVariant & value, int role = Qt::EditRole) override;
    QVariant	headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;
    bool setHeaderData(int section, Qt::Orientation orientation, const QVariant & data, int role) override;

signals:

    void zg_requestSelectedChemicalElements(QList<int>& ZNumberList) const;
    void zg_requestEnergyLinesForZNumber(int ZNumber, PropertyList&) const;
    void zg_requestChemicalElementSymbol(int ZNumber, QString& symbol) const;

public slots:

    void zp_onSelectedChemicalElementChange(int ZNumber, bool selected);


private:

    // VARS
    QMap<int, ZEnergyLineSetItem*> zv_itemMap;
    QStringList zv_columnHederList;

    // FUNCS
    int zh_findRowToInsert(int ZNumber) const;
    void zh_updateColumns();


};
//=============================================================
#endif // ZSELECTEDENERGYLINETABLEMODEL_H
