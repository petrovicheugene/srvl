//============================================================
#ifndef ZCURRENTMEASUREMENTSAMPLETABLEWIDGET_H
#define ZCURRENTMEASUREMENTSAMPLETABLEWIDGET_H
//============================================================
#include "ZBaseTableWidget.h"
#include <QItemSelection>
//============================================================
class QTableView;
class QAction;
class QHBoxLayout;
class QVBoxLayout;
class ZMeasuringResultTableModel;
//============================================================
/*!
 \brief

*/
class ZMeasuringResultTableWidget : public ZBaseTableWidget
{
    Q_OBJECT
public:
    /*!
     \brief

     \param parent
    */
    explicit ZMeasuringResultTableWidget(QWidget *parent = 0);
    void zp_setModel(QAbstractItemModel *model, bool alternatingRowColorFlag = true) override;

signals:

    void zg_currentSampleIndexChanged(int currentSampleIndex) const;
    void zg_currentIndexChanged(const QModelIndex& current, const QModelIndex& previous) const;


public slots:

private slots:

    void zh_onCurrentIndexChanged(const QModelIndex& current,
                                  const QModelIndex& previous);
    void zh_onModelReset();
    void zh_onModelAboutToBeReset();
private:

    // VARS
    QItemSelection zv_itemSelection;

    // FUNCS

};
//============================================================
#endif // ZCURRENTMEASUREMENTSAMPLETABLEWIDGET_H
