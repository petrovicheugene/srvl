//======================================================
#include "ZPlotterDataManager.h"
#include "ZGeneral.h"
#include "ZDefaultRectGraphicsItem.h"
#include "ZEnergyLineGraphicsItem.h"
#include "ZPlotter.h"
#include "ZRulerToolGraphicsItem.h"
#include "ZSpeSpectrum.h"
#include "ZSpectrumGraphicsItem.h"

#include <QAction>
//======================================================
ZPlotterDataManager::ZPlotterDataManager(QObject *parent)
    : QObject(parent)
{
    zv_measuringManager = nullptr;
    zv_plotter = nullptr;
    zv_boundingRectTopFactor = 1.05;
    zv_defaultItem = nullptr;
    zv_verticalRuleLabel = tr("Intensity");
    zv_horizontalRuleLabel = tr("Channels");
    zv_horizontalRecalcedRuleLabel = tr("Energy");
    zv_energyRuleMetrixFlag = false;

    zh_createComponents();
    zh_createConnections();
}
//===========================================================
void ZPlotterDataManager::zh_createComponents()
{
    zv_switchRuleMetrixAction = new QAction(this);
    zv_switchRuleMetrixAction->setIcon(QIcon(NS_Icons::glIconAxisToEnergy));
    zv_switchRuleMetrixAction->setCheckable(true);
}
//===========================================================
void ZPlotterDataManager::zh_createConnections()
{
    connect(zv_switchRuleMetrixAction, &QAction::toggled,
            this, &ZPlotterDataManager::zh_switchRuleMetrix);
}
//======================================================
void ZPlotterDataManager::zp_connectToMeasuringManager(ZMeasuringManager *measuringManager)
{
    zv_measuringManager = measuringManager;

    connect(zv_measuringManager, &ZMeasuringManager::zg_sampleOperation,
            this, &ZPlotterDataManager::zh_onMeasuringManagerSampleOperation);
    connect(zv_measuringManager, &ZMeasuringManager::zg_currentEnergyCalibrationChanged,
            this, &ZPlotterDataManager::zh_onCurrentEnergyCalibrationChange);
    connect(zv_measuringManager, &ZMeasuringManager::zg_currentSpectrumChanged,
            this, &ZPlotterDataManager::zh_setSpectrumCurrent);

}
//======================================================
void ZPlotterDataManager::zp_connectToPlotter(ZPlotter *plotter)
{
    zv_plotter = plotter;
    zv_plotter->zp_setAutoDefineVerticalAbsMax(true);

    QList<QAction*> actionList;
    actionList << zv_switchRuleMetrixAction;
    zv_plotter->zp_appendButtonsToDashboard(actionList,
                                            ZHorizontalDashBoard::AWP_LEFT_OF_BUTTONS,
                                            Qt::AlignLeft, 0);


    zv_plotter->zp_setLeftMarkRecalcFlag(false);
    zv_plotter->zp_setLeftRuleLabel(zv_verticalRuleLabel);
    zv_plotter->zp_setLeftRuleLabelVisible(true);

    zv_plotter->zp_setRightMarkRecalcFlag(false);
    zv_plotter->zp_setRightRuleLabelVisible(false);

    zv_plotter->zp_setTopMarkRecalcFlag(false);
    zv_plotter->zp_setTopRuleLabelVisible(false);

    zv_plotter->zp_setBottomMarkRecalcFlag(false);
    zv_plotter->zp_setBottomRuleLabel(zv_horizontalRuleLabel);
    zv_plotter->zp_setBottomRuleLabelVisible(true);

    if(zv_plotter->zp_itemCount() <= 0)
    {
        zv_defaultItem = new ZDefaultRectGraphicsItem(zv_defaultSceneRect, false, false, false);
        zv_plotter->zp_addItem(zv_defaultItem);
    }

    connect(zv_plotter, &ZPlotter::zg_viewportRectChanged,
            this, &ZPlotterDataManager::zp_onPlotterViewPortRectChange);
    connect(zv_plotter, &ZPlotter::zg_rulerToolChanged,
            this, &ZPlotterDataManager::zh_updateRulerTool);

}
//======================================================
void ZPlotterDataManager::zp_onEnergyLineOperation(QString elementSymbol, QString lineName,
                                                   EnergyLineOperationType operationType)
{
    //    enum EnergyLineOperation{EL_REMOVED,
    //                            EL_INSERTED,
    //                            EL_VISIBILITY_CHANGED,
    //                            EL_COLOR_CHANGED};
    // find item
    ZEnergyLineGraphicsItem* energyLineItem = nullptr;
    QList<QGraphicsItem*> energyLineList = zv_plotter->zp_itemListForType(EnergyLineItemType);
    for(int i = 0; i < energyLineList.count(); i++)
    {
        energyLineItem = qgraphicsitem_cast<ZEnergyLineGraphicsItem*>(energyLineList.at(i));
        if(energyLineItem != nullptr && energyLineItem->zp_chemicalElementSymbol() == elementSymbol
                && energyLineItem->zp_lineName() == lineName)
        {
            // item found
            break;
        }

        // reset item pointer and go on
        energyLineItem = nullptr;
        continue;
    }

    if(operationType == 0)
    {
        //"EL_REMOVED";
        if(energyLineItem)
        {
            zv_plotter->zp_removeItem(energyLineItem);
        }
    }
    else if(operationType == 1)
    {
        //"EL_INSERTED";

        if(energyLineItem)
        {
            zv_plotter->zp_removeItem(energyLineItem);
        }

        // request energy line properties
        double energyVal;
        emit zg_requestEnergyLineEnergyValue(elementSymbol, lineName, energyVal);

        bool visibility;
        emit zg_requestEnergyLineVisibility(elementSymbol, lineName, visibility);

        QColor color;
        emit zg_requestEnergyLineColor(elementSymbol, lineName, color);

        int relativeIntensity = 100;
        emit zg_requestEnergyLineRelativeIntensity(elementSymbol, lineName, relativeIntensity);

        energyLineItem = new ZEnergyLineGraphicsItem(elementSymbol, lineName, relativeIntensity, energyVal);
        zv_plotter->zp_addItem(energyLineItem);

        double channel;
        if(zh_convertEnergyToChannel(energyVal, channel))
        {
            energyLineItem->zp_setXPosition(channel);
            energyLineItem->setVisible(visibility);
        }
        else
        {
            energyLineItem->zp_setXPosition(0.0);
            energyLineItem->setVisible(false);
        }

        energyLineItem->zp_setColor(color);
    }
    else if(operationType == 2)
    {
        // "EL_VISIBILITY_CHANGED";
        if(energyLineItem)
        {
            bool visibility;
            emit zg_requestEnergyLineVisibility(elementSymbol, lineName, visibility);
            energyLineItem->setVisible(visibility);
        }
    }
    else if(operationType == 3)
    {
        // "EL_COLOR_CHANGED";
        if(energyLineItem)
        {
            QColor color;
            emit zg_requestEnergyLineColor(elementSymbol, lineName, color);
            energyLineItem->zp_setColor(color);
        }
    }
    else if(operationType == 4)
    {
        // "EL_CHANGED";
        if(energyLineItem)
        {
            int relativeIntensity;
            emit zg_requestEnergyLineRelativeIntensity(elementSymbol, lineName, relativeIntensity);
            energyLineItem->zp_setHeightPercent(relativeIntensity);
        }
    }
}
//======================================================
bool ZPlotterDataManager::zh_convertEnergyToChannel(double energyValue, double& channel)
{
    if(zv_calibrationFactors.count() < 2)
    {
        return false;
    }

    if(zv_calibrationFactors.count() == 2 || (zv_calibrationFactors.count() > 2 && zv_calibrationFactors.at(2) == 0.0))
    {
        // linear equation
        if(zv_calibrationFactors.at(1) == 0.0)
        {
            return false;
        }

        channel = (energyValue - zv_calibrationFactors.at(0)) / zv_calibrationFactors.at(1);
        return true;
    }

    // square equation
    // D = b^2 - 4*(ac)
    double D = pow(zv_calibrationFactors.at(1), 2.0) - (4*zv_calibrationFactors.at(2) * (zv_calibrationFactors.at(0) - energyValue));

    if(D < 0.0)
    {
        return false;
    }

    if(D == 0.0)
    {
        channel = (-zv_calibrationFactors.at(1)) / (2.0 * zv_calibrationFactors.at(2));
    }
    else // D > 0
    {
        double ch1 = (-zv_calibrationFactors.at(1) + sqrt(D)) / (2.0 * zv_calibrationFactors.at(2));
        double ch2 = (-zv_calibrationFactors.at(1) - sqrt(D)) / (2.0 * zv_calibrationFactors.at(2));

        channel = qMax(ch1, ch2);
    }

    return true;
}
//======================================================
void ZPlotterDataManager::zp_onPlotterViewPortRectChange(QRectF rect)
{
    ZEnergyLineGraphicsItem::zp_setTopAndButtonMargins(rect.top(), rect.bottom());
    ZEnergyLineGraphicsItem* energyLineItem = nullptr;
    QList<QGraphicsItem*> energyLineList = zv_plotter->zp_itemListForType(EnergyLineItemType);
    for(int i = 0; i < energyLineList.count(); i++)
    {
        energyLineItem = qgraphicsitem_cast<ZEnergyLineGraphicsItem*>(energyLineList.at(i));
        if(energyLineItem != nullptr)
        {
            energyLineItem->zp_updateItem();

        }
    }
}
//======================================================
void ZPlotterDataManager::zh_onMeasuringManagerSampleOperation(ZMeasuringManager::SampleOperationType type,
                                                               int first, int last)
{
    if(!zv_plotter)
    {
        return;
    }

    // current plotter state
    qreal distortionFactor;
    qreal distortionCorrectionFactor;
    zv_plotter->zp_verticalDistortionFactors(distortionFactor, distortionCorrectionFactor);
    bool isPlotScaled = zv_plotter->zp_isPlotScaled();

    // cycle vars declaration
    QList<QPair<quint8, int> > mesuringConditionsList;
    ZSpeSpectrum* spectrum;
    ZSpectrumGraphicsItem* spectrumItem;
    // circle for samples
    for(int row = first; row <= last; row++)
    {
        // get measuring conditions list for sample
        mesuringConditionsList = zv_measuringManager->zp_measuringConditionsListForSample(row);

        if(mesuringConditionsList.isEmpty())
        {
            continue;
        }

        // circle for measuring conditions
        for(int mc = 0; mc < mesuringConditionsList.count(); mc++)
        {
            // get original spectrum
            quint8 gainFactor = mesuringConditionsList.at(mc).first;
            int exposition = mesuringConditionsList.at(mc).second;
            spectrum = zv_measuringManager->zp_spectrum(row, gainFactor, exposition);

            // get spectrum graphic item if it exists
            spectrumItem = nullptr;
            QList<QGraphicsItem*> spectrumList = zv_plotter->zp_itemListForType(SpectrumItemType);
            for(int sp = 0; sp < spectrumList.count(); sp++)
            {
                spectrumItem = qgraphicsitem_cast<ZSpectrumGraphicsItem*>(spectrumList.at(sp));
                if(spectrumItem != nullptr && spectrumItem->zp_spectrumId() == spectrum->zp_spectrumId())
                {
                    // spectrumItem found
                    break;
                }

                // reset spectrum item pointer and go on
                spectrumItem = nullptr;
                continue;
            }

            // do approprite action
            if(type == ZMeasuringManager::SOT_SPECTRUM_CHANGED)
            {
                // check spectrum item existance
                if(spectrumItem != nullptr)
                {
                    // set spectrum data
                    spectrumItem->zp_setSpectrumData(spectrum->zp_spectrumData());
                }
            }

            if(type == ZMeasuringManager::SOT_SPECTRUM_VISIBILITY_CHANGED)
            {
                if(spectrumItem != nullptr)
                {
                    if(!spectrumItem->zp_isSpectrumCurrent())
                    {
                        spectrumItem->setVisible(spectrum->zp_isSpectrumVisible());
                    }
                }
            }
            else if(type == ZMeasuringManager::SOT_SAMPLE_INSERTED)
            {
                // check spectrum item existance
                if(spectrumItem == nullptr)
                {
                    spectrumItem = new ZSpectrumGraphicsItem(spectrum,
                                                             zv_boundingRectTopFactor,
                                                             distortionFactor,
                                                             distortionCorrectionFactor);
                    zv_plotter->zp_addItem(spectrumItem);
                }
            }
            else if(type == ZMeasuringManager::SOT_SAMPLE_REMOVED)
            {
                if(spectrumItem != nullptr)
                {

                }
            }
            else if(type == ZMeasuringManager::SOT_SAMPLE_ABOUT_TO_BE_REMOVED)
            {
                // check spectrum item existance
                if(spectrumItem != nullptr)
                {
                    zv_plotter->zp_removeItem(spectrumItem);
                }
            }
        }
    }

    if(!isPlotScaled)
    {
        zv_plotter->zp_fitInBoundingRect();
        //            QMetaObject::invokeMethod(zv_plotter, "zp_fitInBoundingRect",
        //                                      Qt::QueuedConnection);
    }

}
//======================================================
void ZPlotterDataManager::zh_onCurrentEnergyCalibrationChange(QList<double> calibrationFactors)
{
    zv_calibrationFactors = calibrationFactors;
    zv_plotter->zp_setEnergyCalibration(zv_calibrationFactors);
    zh_updateEnergyLines();
    zh_updateRuleMetrix();
}
//======================================================
void ZPlotterDataManager::zh_updateEnergyLines()
{
    ZEnergyLineGraphicsItem* energyLineItem = nullptr;
    QList<QGraphicsItem*> energyLineList = zv_plotter->zp_itemListForType(EnergyLineItemType);
    for(int i = 0; i < energyLineList.count(); i++)
    {
        energyLineItem = qgraphicsitem_cast<ZEnergyLineGraphicsItem*>(energyLineList.at(i));
        if(energyLineItem == nullptr)
        {
            continue;
        }

        double channel = 0.0;
        if(!zh_convertEnergyToChannel(energyLineItem->zp_energyValue(), channel))
        {
            channel = 0.0;
        }

        energyLineItem->zp_setXPosition(channel);
    }
}
//======================================================
void ZPlotterDataManager::zh_switchRuleMetrix(bool toggled)
{
    zv_energyRuleMetrixFlag = toggled;
    zh_updateRuleMetrix();
}
//======================================================
void ZPlotterDataManager::zh_updateRuleMetrix()
{
    if(!zv_energyRuleMetrixFlag || zv_calibrationFactors.isEmpty() ||
            (!(zv_calibrationFactors.value(1, 0.0) == 0.0 ||
               zv_calibrationFactors.value(2, 0.0) == 0.0 )))
    {
        zv_plotter->zp_setBottomMarkRecalcFlag(false);
        zv_plotter->zp_setTopMarkRecalcFlag(false);
        zv_plotter->zp_setHorizontalMarkRecalcFactors(zv_horizontalRuleLabel,
                                                      0, 1, 0);
        return;
    }

    qreal K0 = zv_calibrationFactors.value(0, 0.0);
    qreal K1 = zv_calibrationFactors.value(1, 1.0);
    qreal K2 = zv_calibrationFactors.value(2, 0.0);
    QString energyUnit = tr("kEv");

    QString labelString = zv_horizontalRecalcedRuleLabel + " " + energyUnit;
    zv_plotter->zp_setBottomMarkRecalcFlag(true);
    zv_plotter->zp_setTopMarkRecalcFlag(true);
    zv_plotter->zp_setHorizontalMarkRecalcFactors(labelString,
                                                  K0, K1, K2);

}
//======================================================
void ZPlotterDataManager::zh_setSpectrumCurrent(qint64 spectrumId)
{
    ZSpectrumGraphicsItem::zp_setCurrentSpectrumId(spectrumId);

    ZSpectrumGraphicsItem* spectrumItem = nullptr;
    QList<QGraphicsItem*> spectrumList = zv_plotter->zp_itemListForType(SpectrumItemType);
    bool spectrumVisibility;
    for(int sp = 0; sp < spectrumList.count(); sp++)
    {
        spectrumItem = qgraphicsitem_cast<ZSpectrumGraphicsItem*>(spectrumList.at(sp));
        if(!spectrumItem)
        {
            continue;
        }

        if(!zv_measuringManager->zp_spectrumVisibility(spectrumItem->zp_spectrumId(), spectrumVisibility))
        {
            spectrumVisibility = false;
        }

        spectrumItem->zp_updateCurrentSpectrum(spectrumVisibility);
    }

}
//======================================================
void ZPlotterDataManager::zh_updateRulerTool(QPointF startPoint, QPointF endPoint, bool visibility)
{
    // qDebug() << "UPDATE RT" << startPoint.x() << endPoint.x() << visibility;
    QRectF plotterRect = zv_plotter->zp_viewportSceneRect();
    QSize plotterPixelSize = zv_plotter->zp_viewportPixelSize();

    if(!visibility || !plotterRect.isValid() || plotterRect.isNull())
    {
        zv_plotter->zp_removeItemsForType(RulerToolItemType);
        return;
    }

    ZRulerToolGraphicsItem* rulerItem = nullptr;
    QList<QGraphicsItem*> itemList = zv_plotter->zp_itemListForType(RulerToolItemType);

    for(int i = 0; i < itemList.count(); i++)
    {
        rulerItem = qgraphicsitem_cast<ZRulerToolGraphicsItem*>(itemList.at(i));
        if(rulerItem)
        {
            break;
        }
    }

    if(!rulerItem)
    {
        rulerItem = new ZRulerToolGraphicsItem;
        zv_plotter->zp_addItem(rulerItem);
    }

    // first channel
    QString infoString = tr("Start: %1<br>End: %2").arg(QString::number(static_cast<int>(startPoint.x()) + 1),
                                                                     QString::number(static_cast<int>(endPoint.x()) + 1));

    QList<QGraphicsItem*> spectrumList = zv_plotter->zp_itemListForType(SpectrumItemType);
    if(!spectrumList.isEmpty())
    {
        qint64 id = ZSpectrumGraphicsItem::zp_currentSpectrumId();
        ZSpeSpectrum* currentSpectrum = zv_measuringManager->zp_spectrumForId(id);
        if(currentSpectrum)
        {
            double intensity;
            if(currentSpectrum->zp_intensityInWindow(static_cast<int>(startPoint.x()),
                                                     static_cast<int>(endPoint.x()),
                                                     intensity))
            {
                infoString += tr("<br>Integral: %1").arg(QString::number(intensity, 'f', 0));
            }
        }
    }

    rulerItem->zp_setProperties(startPoint,
                                endPoint,
                                plotterRect,
                                plotterPixelSize,
                                infoString);

}
//======================================================
