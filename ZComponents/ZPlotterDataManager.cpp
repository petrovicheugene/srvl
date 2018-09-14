//======================================================
#include "ZPlotterDataManager.h"
#include "ZGeneral.h"
#include "ZDefaultRectGraphicsItem.h"
#include "ZPlotter.h"
#include "ZSpeSpectrum.h"
#include "ZSpectrumGraphicsItem.h"

#include <QAction>
//======================================================
ZPlotterDataManager::ZPlotterDataManager(QObject *parent)
    : QObject(parent)
{
    zv_measuringManager = 0;
    zv_plotter = 0;
    zv_boundingRectTopFactor = 1.05;
    zv_defaultItem = 0;
    zv_verticalRuleLabel = tr("Intensity");
    zv_horizontalRuleLabel = tr("Channels");
    zv_horizontalRecalcedRuleLabel = tr("Energy");

    zh_createComponents();
    zh_createConnections();
}
//===========================================================
void ZPlotterDataManager::zh_createComponents()
{
    zv_switchRuleMetrixAction = new QAction(this);
    zv_switchRuleMetrixAction->setIcon(QIcon(NS_Icons::glAxisToEnergyIconString));
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
    zv_plotter->zp_setAutoDefineVerticalAbsMax(false);

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

                if(spectrumItem != nullptr)
                {
                    qDebug() << "SAMPLE INSERTED" << first << last << spectrumItem->zp_spectrumId();
                }
            }
            else if(type == ZMeasuringManager::SOT_SAMPLE_REMOVED)
            {
                if(spectrumItem != nullptr)
                {
                    qDebug() << "SAMPLE REMOVED" << first << last << spectrumItem->zp_spectrumId();
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
    qDebug() << "CURRENT CALIBRATION:" << calibrationFactors;
}
//======================================================
void ZPlotterDataManager::zh_switchRuleMetrix(bool toggled)
{
    //    if(zv_currentArrayIndex < 0 || !zv_spectrumArrayRepositiry || zv_plotter == 0 || !toggled)
    //    {
    //        zv_plotter->zp_setBottomMarkRecalcFlag(false);
    //        zv_plotter->zp_setTopMarkRecalcFlag(false);
    //        zv_plotter->zp_setHorizontalMarkRecalcFactors(zv_horizontalRuleLabel,
    //                                                      0, 1, 0);
    //        return;
    //    }

    //    qreal K0;
    //    qreal K1;
    //    qreal K2;
    //    QString energyUnit;
    //    if(!zv_spectrumArrayRepositiry->zp_energyCalibrationForArrayId(zv_currentArrayId,
    //                                                                   K0, K1, K2, energyUnit))
    //    {
    //        zv_plotter->zp_setBottomMarkRecalcFlag(false);
    //        zv_plotter->zp_setTopMarkRecalcFlag(false);
    //        zv_plotter->zp_setHorizontalMarkRecalcFactors(zv_horizontalRuleLabel,
    //                                                      0, 1, 0);
    //    }
    //    else
    //    {
    //        QString labelString = zv_horizontalRecalcedRuleLabel + " " + energyUnit;
    //        zv_plotter->zp_setBottomMarkRecalcFlag(true);
    //        zv_plotter->zp_setTopMarkRecalcFlag(true);
    //        zv_plotter->zp_setHorizontalMarkRecalcFactors(labelString,
    //                                                      K0, K1, K2);
    //    }
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

    qDebug() << "CURRENT SPE SET - ID:" <<  spectrumId;
}
//======================================================
