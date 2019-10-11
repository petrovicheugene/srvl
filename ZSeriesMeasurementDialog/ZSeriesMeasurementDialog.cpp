//======================================================
#include "ZSeriesMeasurementDialog.h"
#include "ZGeneral.h"

#include "ZDependentModelController.h"
#include "ZPlotter.h"
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

    if(spectrum)
    {
        spectrumId = spectrum->zp_spectrumId();
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
}
//======================================================
QWidget* ZSeriesMeasurementDialog::zh_createTablesWidget()
{
    zv_tableSplitter = new QSplitter(this);
    zv_seriesResultView = new QTableView(this);
    zv_tableSplitter->addWidget(zh_createSeriesListViewWidget());
    zv_tableSplitter->addWidget(zv_seriesResultView);

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
    zv_sortButton->setText("S");
    zv_sortButton->setFlat(true);
    // zv_sortButton->setCheckable(true);
    zv_sortButton->setFocusPolicy(Qt::NoFocus);
    buttonBox->addButton(zv_sortButton, QDialogButtonBox::ActionRole);

    return widget;
}
//======================================================
