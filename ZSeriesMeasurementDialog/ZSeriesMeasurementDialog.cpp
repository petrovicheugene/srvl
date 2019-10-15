//======================================================
#include "ZSeriesMeasurementDialog.h"
#include "ZGeneral.h"

#include "ZDependentModelController.h"
#include "ZDefaultRectGraphicsItem.h"
#include "ZEnergyLineTableWidget.h"
#include "ZEnergyLineGraphicsItem.h"
#include "ZPlotter.h"
#include "ZRulerToolGraphicsItem.h"
#include "ZSeriesTableModel.h"
#include "ZSpectrumTableDelegate.h"
#include "ZSpectrumGraphicsItem.h"
#include "ZSpeSpectrum.h"


#include <QApplication>
#include <QDebug>
#include <QDialogButtonBox>
#include <QHeaderView>
#include <QLabel>
#include <QMetaObject>
#include <QPushButton>
#include <QSettings>
#include <QSplitter>
#include <QSqlTableModel>
#include <QSqlRelationalTableModel>
#include <QTableView>
#include <QVBoxLayout>

//======================================================
ZSeriesMeasurementDialog::ZSeriesMeasurementDialog(QWidget *parent)
    : ZBaseDialog("ZSeriesMeasurementDialog", parent)
{
    setWindowTitle(qApp->applicationDisplayName());

    zv_boundingRectTopFactor = 1.05;
    zv_plotter = nullptr;

    zh_createComponents();
    zh_createConnections();
    zh_restoreSettings();

    if(zv_plotter != nullptr)
    {
        QMetaObject::invokeMethod(zv_plotter, "zp_fitInBoundingRect",
                                  Qt::QueuedConnection);
    }
}
//======================================================
ZSeriesMeasurementDialog::~ZSeriesMeasurementDialog()
{
    zh_saveSettings();
}
//======================================================
void ZSeriesMeasurementDialog::zh_createComponents()
{
    zv_seriesListTableModel = new QSqlRelationalTableModel(this);
    zv_seriesListTableModel->setTable("series");
    zv_seriesListTableModel->setJoinMode(QSqlRelationalTableModel::LeftJoin);
    zv_seriesListTableModel->setRelation(3, QSqlRelation("operators", "id", "name"));
    zv_seriesListTableModel->setRelation(4, QSqlRelation("series_tasks", "id", "name"));

    zv_seriesListTableModel->select();
    zv_seriesListTableModel->setHeaderData(1, Qt::Horizontal, QVariant(tr("Date")));
    zv_seriesListTableModel->setHeaderData(2, Qt::Horizontal, QVariant(tr("Time")));
    zv_seriesListTableModel->setHeaderData(3, Qt::Horizontal, QVariant(tr("Operator")));
    zv_seriesListTableModel->setHeaderData(4, Qt::Horizontal, QVariant(tr("Task")));

    zv_seriesResultModel = new ZSeriesTableModel(this);
    zv_seriesTableModelController = new ZDependentModelController(this);

    // create main splitter
    zv_mainSplitter = new  QSplitter(Qt::Vertical, this);
    zh_insertWidgetToMainLayout(0, zv_mainSplitter);

    zv_mainSplitter->addWidget(zh_createTablesWidget());

    zv_plotter = new ZPlotter;
    zv_mainSplitter->addWidget(zv_plotter);

    //  zv_treeView = new QTreeView(this);
    //  zh_insertWidgetToMainLayout(0, zv_treeView);

    zv_closeButton = zh_createBasementButton(tr("Close"),
                                             tr("Close dialog"),
                                             QIcon());
}
//======================================================
void ZSeriesMeasurementDialog::zh_createConnections()
{
    connect(zv_closeButton, &QPushButton::clicked,
            this, &ZSeriesMeasurementDialog::zh_onCloseButtonClick);

    connect(zv_sortButton, &QPushButton::clicked,
            this, &ZSeriesMeasurementDialog::zh_onResetSortButtonClick);

    zv_seriesTableModelController->zp_setView(zv_seriesListView);
    zv_seriesTableModelController->zp_setModel(zv_seriesResultModel);

    zv_seriesListView->setModel(zv_seriesListTableModel);
    zv_seriesListView->setItemDelegateForColumn(2, new ZTimeDelegate);

    zv_seriesListView->setColumnHidden(0, true);
    zv_seriesListView->setSortingEnabled(true);
    zv_seriesListView->setAlternatingRowColors(true);

    zv_seriesResultView->setModel(zv_seriesResultModel);

    ZSpectrumTableDelegate* spectrumTableDelegate = new ZSpectrumTableDelegate(zv_seriesResultView);
    zv_seriesResultView->viewport()->installEventFilter(spectrumTableDelegate);
    zv_seriesResultView->setItemDelegate(spectrumTableDelegate);

    zv_seriesResultView->setAlternatingRowColors(true);

    connect(zv_seriesResultModel, &QAbstractTableModel::modelReset,
            this, &ZSeriesMeasurementDialog::zh_onModelReset);
    connect(zv_seriesResultView->selectionModel(), &QItemSelectionModel::currentChanged,
            this, &ZSeriesMeasurementDialog::zh_onCurrentChange);

    if(zv_plotter->zp_itemCount() <= 0)
    {
        zv_defaultItem = new ZDefaultRectGraphicsItem(zv_defaultSceneRect, false, false, false);
        zv_plotter->zp_addItem(zv_defaultItem);
    }

    connect(zv_plotter, &ZPlotter::zg_viewportRectChanged,
            this, &ZSeriesMeasurementDialog::zp_onPlotterViewPortRectChange);
    connect(zv_plotter, &ZPlotter::zg_rulerToolChanged,
            this, &ZSeriesMeasurementDialog::zh_updateRulerTool);


    connect(zv_energyLineTableWidget, &ZEnergyLineTableWidget::zg_energyLineOperation,
            this, &ZSeriesMeasurementDialog::zh_onEnergyLineOperation);
    connect(this, &ZSeriesMeasurementDialog::zg_requestEnergyLineEnergyValue,
            zv_energyLineTableWidget, &ZEnergyLineTableWidget::zp_energyLineEnergyValue);
    connect(this, &ZSeriesMeasurementDialog::zg_requestEnergyLineRelativeIntensity,
            zv_energyLineTableWidget, &ZEnergyLineTableWidget::zp_energyLineRelativeIntensity);

    connect(this, &ZSeriesMeasurementDialog::zg_requestEnergyLineVisibility,
            zv_energyLineTableWidget, &ZEnergyLineTableWidget::zp_energyLineVisibility);
    connect(this, &ZSeriesMeasurementDialog::zg_requestEnergyLineColor,
            zv_energyLineTableWidget, &ZEnergyLineTableWidget::zp_energyLineColor);



//    connect(zv_seriesResultModel, &ZSeriesTableModel::zg_sampleOperation,
//            this, &ZSeriesMeasurementDialog::zh_onMeasuringManagerSampleOperation);
//    connect(zv_seriesResultModel, &ZSeriesTableModel::zg_currentEnergyCalibrationChanged,
//            this, &ZSeriesMeasurementDialog::zh_onCurrentEnergyCalibrationChange);
//    connect(zv_seriesResultModel, &ZSeriesTableModel::zg_currentSpectrumChanged,
//            this, &ZSeriesMeasurementDialog::zh_setSpectrumCurrent);


}
//======================================================
void ZSeriesMeasurementDialog::zp_onPlotterViewPortRectChange(QRectF rect)
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
void ZSeriesMeasurementDialog::zh_updateRulerTool(QPointF startPoint, QPointF endPoint, bool visibility)
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
        ZSpeSpectrum* currentSpectrum = zv_seriesResultModel->zp_spectrumForId(id);
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
void ZSeriesMeasurementDialog::zh_saveSettings()
{
    QSettings settings;
    if(!zh_openDialogSettingsGroup(&settings))
    {
        return;
    }

    settings.setValue(zv_mainSplitterStateName, zv_mainSplitter->saveState());
    settings.setValue(zv_tableSplitterStateName, zv_tableSplitter->saveState());

    // close dialog group
    settings.endGroup();

    zh_closeDialogSettingsGroup(&settings);
}
//======================================================
void ZSeriesMeasurementDialog::zh_restoreSettings()
{
    QSettings settings;
    if(!zh_openDialogSettingsGroup(&settings))
    {
        return;
    }

    if(settings.value(zv_mainSplitterStateName).canConvert<QByteArray>())
    {
        zv_mainSplitter->restoreState(settings.value(zv_mainSplitterStateName).toByteArray());
    }

    if(settings.value(zv_tableSplitterStateName).canConvert<QByteArray>())
    {
        zv_tableSplitter->restoreState(settings.value(zv_tableSplitterStateName).toByteArray());
    }

    zh_closeDialogSettingsGroup(&settings);
}
//======================================================
void ZSeriesMeasurementDialog::zh_onCloseButtonClick()
{
    reject();
}
//======================================================
void ZSeriesMeasurementDialog::zh_onResetSortButtonClick()
{
    zv_seriesListView->sortByColumn(0, Qt::DescendingOrder);
}
//======================================================
void ZSeriesMeasurementDialog::zh_onModelReset()
{
    // current plotter state
    qreal distortionFactor;
    qreal distortionCorrectionFactor;
    zv_plotter->zp_verticalDistortionFactors(distortionFactor, distortionCorrectionFactor);
    bool isPlotScaled = zv_plotter->zp_isPlotScaled();

    // clear all previous spectra
    zv_plotter->zp_clearItemsForType(SpectrumItemType);

    //
    ZSpectrumGraphicsItem* spectrumItem;
    ZSpeSpectrum* spectrum;

    for(int row = 0; row < zv_seriesResultModel->rowCount(); row++)
    {
        QList<QPair<quint8, int> > measuringConditions = zv_seriesResultModel->zp_sampleMeasuringConditionsListForRow(row);
        for(int m = 0; m < measuringConditions.count(); m++)
        {
            spectrum =
                    zv_seriesResultModel->zp_spectrumForMeasuringConditionsForRow(row, measuringConditions.at(m).first,
                                                                                  measuringConditions.at(m).second);

            spectrumItem = new ZSpectrumGraphicsItem(spectrum,
                                                     zv_boundingRectTopFactor,
                                                     distortionFactor,
                                                     distortionCorrectionFactor);
            connect(spectrum, &ZSpeSpectrum::zg_visibleChanged,
                    this, &ZSeriesMeasurementDialog::zh_onVisibleSpectrumChange);
            zv_plotter->zp_addItem(spectrumItem);
        }

        //        for(int s = 0; s < zv_seriesResultModel->zp_spectrumCountForRow(row); s++)
        //        {
        ////            spectrum = zv_spectrumModel->zp_spectrumForRow(row);
        ////            spectrumItem = new ZSpectrumGraphicsItem(spectrum,
        ////                                                     zv_boundingRectTopFactor,
        ////                                                     distortionFactor,
        ////                                                     distortionCorrectionFactor);
        ////            zv_plotter->zp_addItem(spectrumItem);

        //        }
    }

    if(!isPlotScaled)
    {
        zv_plotter->zp_fitInBoundingRect();
        //            QMetaObject::invokeMethod(zv_plotter, "zp_fitInBoundingRect",
        //                                      Qt::QueuedConnection);
    }

    //    QRectF boundingRect = zv_plotter->zp_boundingRect();
    //    ZVerticalLineGraphicsItem::zp_setTopAndButtonMargins(boundingRect.top(),boundingRect.bottom());
    //    zh_updateVerticalLines();
}
//======================================================
void ZSeriesMeasurementDialog::zh_onVisibleSpectrumChange(bool visible)
{
    ZSpeSpectrum* spectrum = dynamic_cast<ZSpeSpectrum*>(sender());
    if(!spectrum)
    {
        return;
    }

    // get spectrum graphic item if it exists
    ZSpectrumGraphicsItem* spectrumItem = nullptr;
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

    if(spectrumItem != nullptr)
    {
        if(!spectrumItem->zp_isSpectrumCurrent())
        {
            spectrumItem->setVisible(spectrum->zp_isSpectrumVisible());
        }
    }
}
//======================================================
void ZSeriesMeasurementDialog::zh_onCurrentChange(const QModelIndex& current,
                                                  const QModelIndex& previous)
{
    ZSpeSpectrum* spectrum = zv_seriesResultModel->zp_spectrumForIndex(current);
    qint64 spectrumId = -1;
    bool spectrumVisibility = false;
    QList<double> calibrationFactors;

    if(spectrum)
    {
        spectrumId = spectrum->zp_spectrumId();
        calibrationFactors = spectrum->zp_energyCalibration();
    }

    ZSpectrumGraphicsItem::zp_setCurrentSpectrumId(spectrumId);

    ZSpectrumGraphicsItem* spectrumItem = nullptr;
    QList<QGraphicsItem*> spectrumList = zv_plotter->zp_itemListForType(SpectrumItemType);

    for(int sp = 0; sp < spectrumList.count(); sp++)
    {
        spectrumItem = qgraphicsitem_cast<ZSpectrumGraphicsItem*>(spectrumList.at(sp));
        if(!spectrumItem)
        {
            continue;
        }

        if(!zv_seriesResultModel->zp_spectrumVisibility(spectrumItem->zp_spectrumId(), spectrumVisibility))
        {
            spectrumVisibility = false;
        }

        spectrumItem->zp_updateCurrentSpectrum(spectrumVisibility);
    }

    zh_onCurrentEnergyCalibrationChange(calibrationFactors);

}
//======================================================
void ZSeriesMeasurementDialog::zh_onEnergyLineOperation(QString elementSymbol, QString lineName,
                              EnergyLineOperationType operationType)
{
    //    enum EnergyLineOperation{EL_REMOVED,
    //                            EL_INSERTED,
    //                            EL_VISIBILITY_CHANGED,
    //                            EL_COLOR_CHANGED};
    // find item
    qDebug() << elementSymbol << lineName << operationType;

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
bool ZSeriesMeasurementDialog::zh_convertEnergyToChannel(double energyValue,
                                                         double& channel)
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
void ZSeriesMeasurementDialog::zh_updateRuleMetrix()
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
void ZSeriesMeasurementDialog::zh_onCurrentEnergyCalibrationChange(QList<double> calibrationFactors)
{
    zv_calibrationFactors = calibrationFactors;
    zv_plotter->zp_setEnergyCalibration(zv_calibrationFactors);
    zh_updateEnergyLines();
    zh_updateRuleMetrix();
}
//======================================================
void ZSeriesMeasurementDialog::zh_updateEnergyLines()
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
QWidget* ZSeriesMeasurementDialog::zh_createTablesWidget()
{
    zv_tableSplitter = new QSplitter(this);

    zv_tableSplitter->addWidget(zh_createSeriesListViewWidget());

    zv_seriesResultView = new QTableView(this);
    zv_tableSplitter->addWidget(zv_seriesResultView);

    zv_energyLineTableWidget = new ZEnergyLineTableWidget(this);
    zv_tableSplitter->addWidget(zv_energyLineTableWidget);

    return  zv_tableSplitter;
}
//======================================================
QWidget* ZSeriesMeasurementDialog::zh_createSeriesListViewWidget()
{
    QWidget* widget = new QWidget(this);
    QVBoxLayout* mainLayout = new QVBoxLayout;
    mainLayout->setMargin(0);
    widget->setLayout(mainLayout);

    zv_seriesListView = new QTableView(this);
    zv_seriesListView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    zv_seriesListView->setSelectionBehavior(QAbstractItemView::SelectRows);
    mainLayout->addWidget(zv_seriesListView);


    QDialogButtonBox* buttonBox = new QDialogButtonBox(this);
    mainLayout->addWidget(buttonBox);

    //    zv_filterButton = new QPushButton(this);
    //    zv_filterButton->setText("F");
    //    zv_filterButton->setFlat(true);
    //    zv_filterButton->setCheckable(true);
    //    zv_filterButton->setFocusPolicy(Qt::NoFocus);
    //    buttonBox->addButton(zv_filterButton, QDialogButtonBox::ActionRole);

    zv_sortButton = new QPushButton(this);
    zv_sortButton->setIcon(QIcon(":images/ZImages/sorting_256-green.png"));
    zv_sortButton->setToolTip(tr("Reset sorting"));
    // ZImages/sorting_256-green.png
    zv_sortButton->setFlat(true);
    // zv_sortButton->setCheckable(true);
    zv_sortButton->setFocusPolicy(Qt::NoFocus);
    buttonBox->addButton(zv_sortButton, QDialogButtonBox::ActionRole);

    return widget;
}
//======================================================
