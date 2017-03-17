//=============================================================
#include "ZMeasuringResultTableWidget.h"
#include <QItemSelectionModel>
#include <QTableView>
//=============================================================
ZMeasuringResultTableWidget::ZMeasuringResultTableWidget(QWidget *parent) : ZBaseTableWidget(parent)
{
    connect(this, &ZBaseTableWidget::zg_currentChanged,
            this, &ZMeasuringResultTableWidget::zh_onCurrentIndexChanged);

}
//=============================================================
void ZMeasuringResultTableWidget::zh_onCurrentIndexChanged(const QModelIndex& current,
                              const QModelIndex& previous)
{
    emit zg_currentSampleIndexChanged(current.row());
}
//=============================================================
