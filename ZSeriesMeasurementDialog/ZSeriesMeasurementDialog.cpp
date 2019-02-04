//======================================================
#include "ZSeriesMeasurementDialog.h"

#include "ZPlotter.h"

#include <QApplication>
#include <QLabel>
#include <QPushButton>
#include <QSettings>
#include <QSplitter>
#include <QSqlTableModel>
#include <QTableView>
//======================================================
ZSeriesMeasurementDialog::ZSeriesMeasurementDialog(QWidget *parent)
    : ZBaseDialog("ZSeriesMeasurementDialog", parent)
{
    setWindowTitle(qApp->property("glAppProduct").toString());

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
    zv_seriesNameTableModel = new QSqlTableModel(this);
    zv_seriesTableModel = new QSqlTableModel(this);

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
QWidget* ZSeriesMeasurementDialog::zh_createTablesWidget()
{
    zv_tableSplitter = new QSplitter(this);
    zv_seriesListView = new QTableView(this);
    zv_seriesResultView = new QTableView(this);
    zv_tableSplitter->addWidget(zv_seriesListView);
    zv_tableSplitter->addWidget(zv_seriesResultView);

    return  zv_tableSplitter;
}
//======================================================
