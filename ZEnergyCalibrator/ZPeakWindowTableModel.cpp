//======================================================
#include "ZPeakWindowTableModel.h"
#include "ZGeneral.h"
#include "ZControlAction.h"
#include "ZPeakWindow.h"
//======================================================
ZPeakWindowTableModel::ZPeakWindowTableModel(QObject *parent)
    : QAbstractTableModel(parent)
{
    zh_createActions();
    zh_createConnections();

}
//======================================================
ZPeakWindowTableModel::~ZPeakWindowTableModel()
{

}
//======================================================
Qt::ItemFlags	ZPeakWindowTableModel::flags(const QModelIndex & index) const
{
    Qt::ItemFlags flags;
    return flags;
}
//======================================================
QModelIndex ZPeakWindowTableModel::index(int row, int column, const QModelIndex &parent) const
{
    return QAbstractTableModel::index(row, column, parent);
}
//======================================================
int	ZPeakWindowTableModel::columnCount(const QModelIndex & parent) const
{
    return 3;
}
//======================================================
int	ZPeakWindowTableModel::rowCount(const QModelIndex & parent) const
{
    return zv_peakWindowList.count();
}
//======================================================
QVariant	ZPeakWindowTableModel::data(const QModelIndex & index, int role) const
{
    if(!index.isValid() || index.column() < 0 || index.column() >= columnCount()
            || index.row() < 0 || index.row() >= rowCount())
    {
        return QVariant();
    }

    if(role == Qt::DisplayRole)
    {
        if(index.column() == 0)
        {
            return QVariant(zv_peakWindowList.at(index.row())->zp_peakName());
        }
    }

    return QVariant();
}
//======================================================
bool ZPeakWindowTableModel::setData(const QModelIndex & index, const QVariant & value, int role)
{
    return false;
}
//======================================================
QVariant	ZPeakWindowTableModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    return QVariant();
}
//======================================================
QList<ZControlAction*> ZPeakWindowTableModel::zp_buttonActions() const
{
    QList<ZControlAction*> actionList;
    actionList.append(zv_addPeakWindowAction);
    actionList.append(zv_removePeakWindowAction);

    return actionList;
}
//======================================================
QList<ZControlAction*> ZPeakWindowTableModel::zp_contextActions() const
{
    QList<ZControlAction*> actionList;
    actionList.append(zv_addPeakWindowAction);
    actionList.append(zv_removePeakWindowAction);

    return actionList;
}
//======================================================
void ZPeakWindowTableModel::zh_createActions()
{
    zv_addPeakWindowAction = new ZControlAction(this);
    zv_addPeakWindowAction->setIcon(QIcon());
    zv_addPeakWindowAction->setText("Add");
    zv_addPeakWindowAction->setToolTip(tr("Append peak window"));

    zv_removePeakWindowAction = new ZControlAction(this);
    zv_removePeakWindowAction->setIcon(QIcon());
    zv_removePeakWindowAction->setText("Remove");
    zv_removePeakWindowAction->setToolTip(tr("Remove peak window"));

}
//===================================================
void ZPeakWindowTableModel::zh_createConnections()
{
    connect(zv_addPeakWindowAction, &ZControlAction::triggered,
            this, &ZPeakWindowTableModel::zh_onAddPeakWindowAction);
    connect(zv_removePeakWindowAction, &ZControlAction::triggered,
            this, &ZPeakWindowTableModel::zh_onRemovePeakWindowAction);

}
//===================================================
void ZPeakWindowTableModel::zh_onAddPeakWindowAction()
{
    // define peak window margins
    // ask plotter for visible rect
    QRectF currentVisibleSceneRect;
    emit zg_inquiryCurrentVisibleSceneRect(currentVisibleSceneRect);

    int windowWidth = qRound(currentVisibleSceneRect.width() / 20);
    int offset = qRound((currentVisibleSceneRect.width() - windowWidth) / 2);
    int firstChannel;
    int lastChannel;
    if(currentVisibleSceneRect.width() > 0)
    {
        firstChannel = qRound(currentVisibleSceneRect.left()) + offset;
        lastChannel = qRound(currentVisibleSceneRect.right()) - offset;
    }
    else
    {
        firstChannel = 0;
        lastChannel = 1;
    }

    qDebug() << "FIRST CHANEL" << firstChannel << "LAST CHANNEL" << lastChannel;

    // create a new Peak Window
    ZPeakWindow* peakWindow = new ZPeakWindow(this);

    int row = zv_peakWindowList.count();
    beginInsertRows(QModelIndex(), row, row);
    zv_peakWindowList.append(peakWindow);
    endInsertRows();

    //


}
//===================================================
void ZPeakWindowTableModel::zh_onRemovePeakWindowAction()
{
    QModelIndex currentIndex;
    emit zg_inquiryCurrentIndex(currentIndex);
    if(!currentIndex.isValid() || currentIndex.row() < 0
            || currentIndex.row() >=  zv_peakWindowList.count())
    {
        return;
    }

     beginRemoveRows(QModelIndex(), currentIndex.row(), currentIndex.row());
     delete zv_peakWindowList.takeAt(currentIndex.row());
     endRemoveRows();
}
//===================================================
