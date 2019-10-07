//======================================================
#ifndef ZSERIESTABLEMODEL_H
#define ZSERIESTABLEMODEL_H
//======================================================
#include "ZDependentModel.h"
//======================================================

//======================================================
class ZSeriesTableModel : public ZDependentModel
{
    Q_OBJECT
public:
    explicit ZSeriesTableModel(QObject *parent = nullptr);

    virtual int	columnCount(const QModelIndex & parent = QModelIndex()) const override;
    virtual int	rowCount(const QModelIndex & parent = QModelIndex()) const override;
    virtual QVariant	data(const QModelIndex & index, int role = Qt::DisplayRole) const override;


signals:

public slots:

    void zp_reload(qint64 id) override;

};
//======================================================
#endif // ZSERIESTABLEMODEL_H
