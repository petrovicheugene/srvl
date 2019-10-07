//=========================================================
#include "ZDependentModelController.h"
#include "ZDependentModel.h"

#include <QAbstractItemView>
#include <QDebug>
//=========================================================
ZDependentModelController::ZDependentModelController(QObject *parent) : QObject(parent)
{
    zv_id = -1;
}
//=========================================================
void ZDependentModelController::zp_setView(QAbstractItemView* view)
{
    connect(view, &QAbstractItemView::clicked,
            this, &ZDependentModelController::zh_onCurrentIndexChange);
}
//=========================================================
void ZDependentModelController::zp_setModel(ZDependentModel* model)
{
    connect(this, &ZDependentModelController::zg_currentId,
            model, &ZDependentModel::zp_reload);
}
//=========================================================
void ZDependentModelController::zh_onCurrentIndexChange(const QModelIndex& index)
{
    qint64 id = -1;
    QModelIndex idIndex = index.model()->index(index.row(), 0, QModelIndex());
    if(idIndex.isValid())
    {
        QVariant vData = idIndex.data();
        if(vData.isValid() && vData.canConvert<qint64>())
        {
            bool ok = false;
            id = vData.toInt(&ok);
            if(!ok)
            {
                id = -1;
            }
        }
    }

    if(zv_id != id)
    {
        zv_id = id;
        emit zg_currentId(id);
    }
}
//=========================================================
