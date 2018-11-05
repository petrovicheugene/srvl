//====================================================
#include "ZPlotGraphicsScene.h"
#include "ZGraphicsItemUserTypes.h"
#include "ZWindowGraphicsItem.h"
#include "ZSpectrumGraphicsItem.h"
#include "ZGeneral.h"
#include "QPushButton"

#include <QGraphicsItem>
//====================================================
ZPlotGraphicsScene::ZPlotGraphicsScene(QObject * parent)
    : QGraphicsScene(parent)
{
    zh_createConnections();
}
//====================================================
ZPlotGraphicsScene::ZPlotGraphicsScene(const QRectF & sceneRect, QObject * parent)
    : QGraphicsScene(sceneRect,  parent)
{
    zh_createConnections();
}
//====================================================
ZPlotGraphicsScene::ZPlotGraphicsScene(qreal x, qreal y, qreal width, qreal height, QObject * parent)
    : QGraphicsScene(x,  y,  width,  height, parent)
{
    zh_createConnections();
}
//====================================================
ZPlotGraphicsScene::~ZPlotGraphicsScene()
{
    //    QList<QGraphicsItem*> itemList = items();
    //    qDeleteAll(itemList);
}
//====================================================
void ZPlotGraphicsScene::zp_addItem(QGraphicsItem * item)
{
    //    if(item->type() == WindowItemType)
    //    {
    //        ZWindowGraphicsItem* windowItem = qgraphicsitem_cast<ZWindowGraphicsItem*>(item);
    //        windowItem->zp_fitInSceneRect(this->sceneRect());
    //        zv_windowItemList << windowItem;
    //    }

    QGraphicsScene::addItem(item);
}
//====================================================
void ZPlotGraphicsScene::zp_removeItem(QGraphicsItem * item)
{
//    if(item->type() == WindowItemType)
//    {
//        ZWindowGraphicsItem* windowItem = qgraphicsitem_cast<ZWindowGraphicsItem*>(item);
//        zv_windowItemList.removeAt(zv_windowItemList.indexOf(windowItem));
//    }
    if(!items().contains(item))
    {
        return;
    }

    QGraphicsScene::removeItem(item);
    delete item;
}
//====================================================
void ZPlotGraphicsScene::zh_createConnections()
{
    connect(this, &QGraphicsScene::sceneRectChanged,
            this, &ZPlotGraphicsScene::zh_onSceneRectChange);
}
//====================================================
void ZPlotGraphicsScene::zh_onSceneRectChange(const QRectF& rect)
{
    // fitting window items in scene trect
    //    foreach(ZWindowGraphicsItem* windowItem, zv_windowItemList)
    //    {
    //        windowItem->zp_fitInSceneRect(rect);
    //    }
}
//====================================================
