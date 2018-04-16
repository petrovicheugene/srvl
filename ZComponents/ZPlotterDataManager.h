//======================================================
#ifndef ZPLOTTERDATAMANAGER_H
#define ZPLOTTERDATAMANAGER_H
//======================================================
#include <QObject>
#include "ZMeasuringManager.h"

//======================================================
class ZPlotter;
class ZDefaultRectGraphicsItem;
//======================================================
class ZPlotterDataManager : public QObject
{
    Q_OBJECT
public:
    explicit ZPlotterDataManager(QObject *parent = 0);

    void zp_connectToMeasuringManager(ZMeasuringManager* measuringManager);
    void zp_connectToPlotter(ZPlotter* plotter);

signals:



public slots:



private slots:

    void zh_onMeasuringManagerSampleOperation(ZMeasuringManager::SampleOperationType type,
                                              int first, int last);

private:

    //VARS
    ZMeasuringManager* zv_measuringManager;
    ZPlotter* zv_plotter;
    ZDefaultRectGraphicsItem* zv_defaultItem;
    QString zv_verticalRuleLabel;
    QString zv_horizontalRuleLabel;
    QString zv_horizontalRecalcedRuleLabel;

    qreal zv_boundingRectTopFactor;
    QRectF zv_defaultSceneRect = QRectF(QPointF(0.0,-100.0), QPointF(2048.0, 0.0));


    // FUNCS


};
//======================================================
#endif // ZPLOTTERDATAMANAGER_H
