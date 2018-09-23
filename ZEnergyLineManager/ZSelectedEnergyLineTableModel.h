//=============================================================
#ifndef ZSELECTEDENERGYLINETABLEMODEL_H
#define ZSELECTEDENERGYLINETABLEMODEL_H
//=============================================================
#include <QAbstractTableModel>
#include <QMap>

#include "ZEnergyLineSetItem.h"
//=============================================================
typedef QList<QPair<QString, QString> > PropertyList;
typedef int EnergyLineOperation;
//=============================================================
class ZSelectedEnergyLineTableModel : public QAbstractTableModel
{
    Q_OBJECT
public:
    explicit ZSelectedEnergyLineTableModel(QObject *parent = nullptr);
    ~ZSelectedEnergyLineTableModel();
    // reimplemented FUNCS
    Qt::ItemFlags	flags(const QModelIndex & index) const override;
    int	columnCount(const QModelIndex & parent = QModelIndex()) const override;
    int	rowCount(const QModelIndex & parent = QModelIndex()) const override;
    QVariant	data(const QModelIndex & index, int role = Qt::DisplayRole) const override;
    bool setData(const QModelIndex & index, const QVariant & value, int role = Qt::EditRole) override;
    QVariant	headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;
    bool setHeaderData(int section, Qt::Orientation orientation, const QVariant & data, int role) override;

    //
    bool zp_energyLineEnergyValue(const QString& elementSymbol,
                           const QString& lineName,
                           double& energyValue) const;
    bool zp_energyLineVisibility(const QString& elementSymbol,
                           const QString& lineName,
                           bool& visible) const;
    bool zp_energyLineColor(const QString& elementSymbol,
                           const QString& lineName,
                           QColor& color) const;
signals:

    void zg_requestSelectedChemicalElements(QList<int>& ZNumberList) const;
    void zg_requestEnergyLinesForZNumber(int ZNumber, PropertyList&) const;
    void zg_requestChemicalElementSymbol(int ZNumber, QString& symbol) const;

    void zg_energyLineVisibilityChanged(QString elementSymbol, QString lineName,
                                        double energy, bool visible) const;

    void zg_energyLineOperation(QString elementSymbol, QString lineName,
                                EnergyLineOperation operationType) const;

public slots:

    void zp_onSelectedChemicalElementChange(int ZNumber, bool selected);


private slots:



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
