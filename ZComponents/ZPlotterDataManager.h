//======================================================
#ifndef ZPLOTTERDATAMANAGER_H
#define ZPLOTTERDATAMANAGER_H
//======================================================
#include <QObject>
#include "ZMeasuringManager.h"

//======================================================
class QAction;
class ZPlotter;
class ZDefaultRectGraphicsItem;
class ZEnergyLineGraphicsItem;

typedef int EnergyLineOperationType;
//======================================================
class ZPlotterDataManager : public QObject
{
    Q_OBJECT
public:
    explicit ZPlotterDataManager(QObject *parent = 0);

    void zp_connectToMeasuringManager(ZMeasuringManager* measuringManager);
    void zp_connectToPlotter(ZPlotter* plotter);

signals:

    void zg_requestCurrentMeasuringConditions(quint8 gainFactor, int exposition) const;

    void zg_requestEnergyLineEnergyValue(const QString& elementSymbol,
                                         const QString& lineName, double& energyValue) const;
    void zg_requestEnergyLineRelativeIntensity(const QString& elementSymbol,
                                               const QString& lineName, int& reletiveIntensity) const;

    void zg_requestEnergyLineVisibility(QString elementSymbol, QString lineName, bool& visibility) const;
    void zg_requestEnergyLineColor(QString elementSymbol, QString lineName, QColor& color) const;

public slots:


    void zp_onEnergyLineOperation(QString elementSymbol, QString lineName,
                                  EnergyLineOperationType operationType);
    void zp_onPlotterViewPortRectChange(QRectF rect);


private slots:

    void zh_onMeasuringManagerSampleOperation(ZMeasuringManager::SampleOperationType type,
                                              int first, int last);
    void zh_onCurrentEnergyCalibrationChange(QList<double> calibrationFactors);
    void zh_updateEnergyLines();

    void zh_switchRuleMetrix(bool toggled);

    void zh_setSpectrumCurrent(qint64 spectrumId);

private:

    //VARS
    QAction* zv_switchRuleMetrixAction;

    ZMeasuringManager* zv_measuringManager;

    ZPlotter* zv_plotter;
    ZDefaultRectGraphicsItem* zv_defaultItem;
    QString zv_verticalRuleLabel;
    QString zv_horizontalRuleLabel;
    QString zv_horizontalRecalcedRuleLabel;

    qreal zv_boundingRectTopFactor;
    QRectF zv_defaultSceneRect = QRectF(QPointF(0.0,-100.0), QPointF(2048.0, 0.0));
    QList<double> zv_calibrationFactors;

    // FUNCS
    void zh_createComponents();
    void zh_createConnections();
    bool zh_convertEnergyToChannel(double energyValue, double& channel);


};
//======================================================
#endif // ZPLOTTERDATAMANAGER_H
