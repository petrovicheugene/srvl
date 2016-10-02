//====================================================
#ifndef ZPLOTGRAPHICSSCENE_H
#define ZPLOTGRAPHICSSCENE_H
//====================================================
#include <QGraphicsScene>
//====================================================
class ZWindowGraphicsItem;
//====================================================
class ZPlotGraphicsScene : public QGraphicsScene
{
    Q_OBJECT
public:
    explicit ZPlotGraphicsScene(QObject *parent = 0);
    explicit ZPlotGraphicsScene(const QRectF & sceneRect, QObject * parent = 0);
    explicit ZPlotGraphicsScene(qreal x, qreal y, qreal width, qreal height, QObject * parent = 0);
    ~ZPlotGraphicsScene();
    void zp_addItem(QGraphicsItem * item);
    void 	zp_removeItem(QGraphicsItem * item);

signals:


protected:

    // VARS
    QList<ZWindowGraphicsItem*> zv_windowItemList;
    QList<QGraphicsItem*> zv_itemToRemoveList;
    // FUNCS
    // resectioned
    void addItem(QGraphicsItem * item);
    void 	removeItem(QGraphicsItem * item);

    // my
    void zh_createConnections();

protected slots:

    void zh_onSceneRectChange(const QRectF&);

};
//====================================================
#endif // ZPLOTGRAPHICSSCENE_H
