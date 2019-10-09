//======================================================
#include "ZSeriesMeasurementDialog.h"

#include "ZPlotter.h"
#include "ZSeriesTableModel.h"
#include "ZDependentModelController.h"

#include <QApplication>
#include <QDebug>
#include <QDialogButtonBox>
#include <QHeaderView>
#include <QLabel>
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

    zv_seriesResultView->setModel(zv_seriesResultModel);


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
