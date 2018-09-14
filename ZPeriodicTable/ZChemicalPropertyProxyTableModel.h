//====================================================
#ifndef ZCHEMPROPERTYPROXYMODEL_H
#define ZCHEMPROPERTYPROXYMODEL_H
//====================================================
#include <QAbstractTableModel>
//====================================================
class QAbstractItemModel;
//====================================================
class ZChemicalPropertyProxyTableModel : public QAbstractTableModel
{
    Q_OBJECT
public:
    explicit ZChemicalPropertyProxyTableModel(QObject *parent = nullptr);

    // reimplemented FUNCS
    Qt::ItemFlags	flags(const QModelIndex & index) const override;
    int	columnCount(const QModelIndex & parent = QModelIndex()) const override;
    int	rowCount(const QModelIndex & parent = QModelIndex()) const override;
    QVariant	data(const QModelIndex & index, int role = Qt::DisplayRole) const override;
    bool setData(const QModelIndex & index, const QVariant & value, int role = Qt::EditRole) override;
    QVariant	headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;
    bool setHeaderData(int section, Qt::Orientation orientation, const QVariant & data, int role) override;

    void zp_setItemIsEditable(bool editable);

signals:


public slots:

    void zp_onCurrentTreeElementChanged(const QModelIndex& current,
                                        const QModelIndex& previous);
    void zp_onSourceModelDataChanged(const QModelIndex& topLeft, const QModelIndex& bottomRight);
private:

    // VARS
    QModelIndex zv_currentSourceModelIndex;
    QStringList zv_headerList;
    bool zv_itemIsEditable;

    // FUNCS

};
//====================================================
#endif // ZCHEMPROPERTYPROXYMODEL_H
