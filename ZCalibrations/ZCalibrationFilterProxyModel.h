//========================================================
#ifndef ZCALIBRATIONFILTERPROXYMODEL_H
#define ZCALIBRATIONFILTERPROXYMODEL_H
//========================================================
#include <QSortFilterProxyModel>
//========================================================
class ZCalibrationFilterProxyModel : public QSortFilterProxyModel
{
    Q_OBJECT
public:
    explicit ZCalibrationFilterProxyModel(QObject *parent = 0);

    void zp_setMeasuringConditionsFilter(int gainFactor, int exposition);

signals:

public slots:

protected:

    bool filterAcceptsRow(int sourceRow, const QModelIndex &sourceParent) const;

private:

    // VARS
    int zv_gainFactorFilter;
    int zv_expositionFilter;


};
//========================================================
#endif // ZCALIBRATIONFILTERPROXYMODEL_H
