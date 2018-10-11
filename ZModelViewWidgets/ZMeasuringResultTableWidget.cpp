//=============================================================
#include "ZMeasuringResultTableWidget.h"
#include "ZMeasuringResultTableModel.h"
#include "ZSampleTaskDelegate.h"


#include <QHeaderView>
#include <QItemSelectionModel>
#include <QMetaObject>
#include <QTableView>
#include <QItemSelection>
//=============================================================
ZMeasuringResultTableWidget::ZMeasuringResultTableWidget(QWidget *parent) : ZBaseTableWidget(parent)
{
    connect(this, &ZBaseTableWidget::zg_currentChanged,
            this, &ZMeasuringResultTableWidget::zh_onCurrentIndexChanged);

}
//=============================================================
void ZMeasuringResultTableWidget::zp_setModel(QAbstractItemModel* model, bool alternatingRowColorFlag)
{
    ZBaseTableWidget::zp_setModel(model, alternatingRowColorFlag);

    ZSampleTaskDelegate* sampleTaskDelegate = new ZSampleTaskDelegate;
    zv_table->setItemDelegateForColumn(1, sampleTaskDelegate);
    connect(model, &QAbstractItemModel::modelReset,
            this, &ZMeasuringResultTableWidget::zh_onModelReset);

    connect(model, &QAbstractItemModel::modelAboutToBeReset,
            this, &ZMeasuringResultTableWidget::zh_onModelAboutToBeReset);


}
//=============================================================
void ZMeasuringResultTableWidget::zh_onCurrentIndexChanged(const QModelIndex& current,
                              const QModelIndex& previous)
{
    emit zg_currentSampleIndexChanged(current.row());
    emit zg_currentIndexChanged(current, previous);
}
//=============================================================
void ZMeasuringResultTableWidget::zh_onModelReset()
{
    zh_manageColumnWidth();

//    QMetaObject::invokeMethod(zv_table->selectionModel(),
//                              "select",
//                              Qt::QueuedConnection,
//                              QGenericReturnArgument("void"),
//                              Q_ARG(QItemSelection, zv_itemSelection),
//                              Q_ARG(QItemSelectionModel::SelectionFlag, QItemSelectionModel::Select));


//    Q_RETURN_ARG(QString, retVal),
//                              Q_ARG(QString, "sqrt"),
//                              Q_ARG(int, 42),
//                              Q_ARG(double, 9.7));

    // zv_table->selectionModel()->select(zv_itemSelection, QItemSelectionModel::Select);
}
//=============================================================
void ZMeasuringResultTableWidget::zh_onModelAboutToBeReset()
{
    // zv_itemSelection = zv_table->selectionModel()->selection();
}
//=============================================================
void ZMeasuringResultTableWidget::zh_manageColumnWidth()
{
    for(int column = 0; column < zv_table->model()->columnCount(); column++)
    {
        if(zv_table->columnWidth(column) < zv_table->horizontalHeader()->sectionSizeHint(column))
        {
            zv_table->resizeColumnToContents(column);
        }
    }
}
//=============================================================
