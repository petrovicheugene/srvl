//============================================================
#include "MainWindow.h"
#include "ZGLConstantsAndDefines.h"

// components
#include "ZControlAction.h"
#include "ZCurrentMeasurementCommonWidget.h"
#include "ZDatabaseInspector.h"
#include "ZSettingsDialog.h"
#include "ZWidgetWithSidebar.h"

// views
#include "ZPlotter.h"
#include "ZCurrentMeasurementSampleTableWidget.h"
#include "ZCurrentMeasurementTaskTreeWidget.h"

// models
#include "ZMeasurementResultTableModel.h"
#include "ZSeriesTaskTreeModel.h"

//#include "ZCur.h"

// Qt
#include <QCloseEvent>
#include <QAction>
#include <QDockWidget>
#include <QFrame>
#include <QMessageBox>
#include <QMenuBar>
#include <QSettings>
#include <QStatusBar>
#include <QSqlRelationalTableModel>
#include <QVBoxLayout>

// Qt controls

// TEMP
#include "ZAddCalibrationDialog.h"

//============================================================
MainWindow::MainWindow(const QString &dbName, const QString &dbPath, QWidget *parent)
    : QMainWindow(parent)
{
    // connect to database
    QString msg;
    if(!ZDatabaseInspector::zp_connectToDatabase(dbPath, zv_database, msg))
    {
        QMessageBox::critical(this, NS_CommonStrings::glError, msg, QMessageBox::Ok);
        return;
    }

    // register ZAppSettings class for reading and writting to settings
    qRegisterMetaType<ZDashboardSettings>("ZDashboardSettings");
    qRegisterMetaTypeStreamOperators<ZDashboardSettings>("ZDashboardSettings");
    qRegisterMetaType<ZAppSettings>("ZAppSettings");
    qRegisterMetaTypeStreamOperators<ZAppSettings>("ZAppSettings");

    setWindowTitle(glAppProduct);

    zv_exitAction = 0;
    zv_aboutAction = 0;
    zv_helpAction = 0;

    zv_currentMeasurementResultTableModel = 0;
    zv_currentMeasurementTaskTreeModel = 0;
    // zv_sqlSeriesModel = 0;

    zv_plotter = 0;

    zh_createActions();
    zh_createComponents();
    zh_createMenu();
    zh_createToolbar();
    zh_createConnections();
    zh_restoreSettings();

    // plotter starting settings
    if(zv_plotter != 0)
    {
        QMetaObject::invokeMethod(zv_plotter, "zp_fitInBoundingRect",
                                  Qt::QueuedConnection);
    }

    //    ZAddCalibrationDialog dialog;
    //    dialog.exec();
}
//============================================================
MainWindow::~MainWindow()
{
    ZDatabaseInspector::zp_disconnectFromDatabase(zv_database);
}
//============================================================
bool MainWindow::zp_isDatabaseOpen() const
{
    return zv_database.isOpen();
}
//============================================================
void MainWindow::closeEvent(QCloseEvent* e)
{
    //    QString questionString = tr("Quit the application?");
    //    if(QMessageBox::question(this, tr("Quit the application"), questionString,
    //                             QMessageBox::Yes, QMessageBox::No) == QMessageBox::No)
    //    {
    //        e->ignore();
    //        return;
    //    }

    zh_saveSettings();
}
//============================================================
void MainWindow::zh_createActions()
{
    zv_settingsAction = new QAction(this);
    zv_settingsAction->setIcon(QIcon(NS_Icons::glIconStringSettings));
    zv_settingsAction->setText(NS_CommonStrings::glSettings);
    zv_settingsAction->setToolTip(NS_CommonStrings::glSettingsToolTip);

    zv_exitAction = new QAction(this);
    zv_exitAction->setIcon(QIcon(NS_Icons::glIconStringExitApp));
    zv_exitAction->setText(NS_CommonStrings::glExit);
    zv_exitAction->setToolTip(NS_CommonStrings::glExitToolTip);

    zv_aboutAction = new QAction(this);
    zv_aboutAction->setIcon(QIcon(NS_Icons::glIconStringAbout));
    zv_aboutAction->setText(NS_CommonStrings::glAbout);
    zv_aboutAction->setToolTip(NS_CommonStrings::glAboutToolTip);

    zv_helpAction = new QAction(this);
    zv_helpAction->setIcon(QIcon(NS_Icons::glIconStringHelp));
    zv_helpAction->setText(NS_CommonStrings::glHelp);
    zv_helpAction->setToolTip(NS_CommonStrings::glHelpToolTip);

}
//============================================================
void MainWindow::zh_createComponents()
{
    // Measurement Widget
    zv_currentMeasurementCommonWidget = new ZCurrentMeasurementCommonWidget(this);
    setCentralWidget(zv_currentMeasurementCommonWidget);

    // Plotter
    // create plotter dock
    zv_plotterDock = new QDockWidget(this);
    zv_plotterDock->setObjectName("PLOTTER_DOCK");
    zv_plotterDock->setWindowTitle(tr("Spectra"));
    zv_dockList << zv_plotterDock;
    addDockWidget(Qt::BottomDockWidgetArea, zv_plotterDock);

    // create plotter
    zv_plotter = new ZPlotter(this);
    QFrame* frame = zh_setWidgetToFrame(zv_plotter);

    // setting to plotter dock
    zv_plotterDock->setWidget(frame);

    // DATA MODELS
    // measurement models
    zv_currentMeasurementTaskTreeModel = new ZSeriesTaskTreeModel(this);
    zv_currentMeasurementResultTableModel = new ZMeasurementResultTableModel(this);


    statusBar();
}
//============================================================
void MainWindow::zh_createMenu()
{
    QMenuBar* menubar = menuBar();
    menubar->setCursor(Qt::PointingHandCursor);
    // File
    QMenu* menu = menubar->addMenu(NS_Menus::glMenuFile);
    menu->setCursor(Qt::PointingHandCursor);
    menu->setObjectName(NS_ObjectNames::glObjectNameMenuFile);
    zh_appendActionsToMenu(menu);

    // Edit
    menu = menubar->addMenu(NS_Menus::glMenuEdit);
    menu->setCursor(Qt::PointingHandCursor);
    menu->setObjectName(NS_ObjectNames::glObjectNameMenuEdit);
    zh_appendActionsToMenu(menu);

    // View
    menu = menubar->addMenu(NS_Menus::glMenuView);
    menu->setCursor(Qt::PointingHandCursor);
    menu->setObjectName(NS_ObjectNames::glObjectNameMenuView);
    zh_appendActionsToMenu(menu);

    // Actions
    menu = menubar->addMenu(NS_Menus::glMenuActions);
    menu->setCursor(Qt::PointingHandCursor);
    menu->setObjectName(NS_ObjectNames::glObjectNameMenuActions);
    zh_appendActionsToMenu(menu);

    // Help
    menu = menubar->addMenu(NS_Menus::glMenuHelp);
    menu->setCursor(Qt::PointingHandCursor);
    menu->setObjectName(NS_ObjectNames::glObjectNameMenuHelp);
    zh_appendActionsToMenu(menu);

}
//============================================================
void MainWindow::zh_createToolbar()
{

}
//============================================================
void MainWindow::zh_createConnections()
{
    //
    connect(zv_settingsAction, &QAction::triggered,
            this, &MainWindow::zh_onSettingsAction);
    connect(this, &MainWindow::zg_saveSettings,
            zv_currentMeasurementCommonWidget, &ZCurrentMeasurementCommonWidget::zp_saveSettings);

    zv_currentMeasurementCommonWidget->zp_appendTaskButtonActions(zv_currentMeasurementTaskTreeModel->zp_taskActions());
    zv_currentMeasurementCommonWidget->zp_appendTaskContextMenuActions(zv_currentMeasurementTaskTreeModel->zp_contextTaskActions());
    zv_currentMeasurementCommonWidget->zp_appendSampleButtonActions(zv_currentMeasurementTaskTreeModel->zp_sampleActions());
    zv_currentMeasurementCommonWidget->zp_appendSampleContextMenuActions(zv_currentMeasurementTaskTreeModel->zp_contextSampleActions());

    // table model <-> source tree model connection
    zv_currentMeasurementResultTableModel->zp_connectToSourceModel(zv_currentMeasurementTaskTreeModel);
    //    zv_currentMeasurementTaskTreeModel->zp_connectToMeasurementManager(zv_currentMeasurementManager);

    // widget <-> model connections
    zv_currentMeasurementCommonWidget->zp_setMeasurementSampleTableModel(zv_currentMeasurementResultTableModel);
    zv_currentMeasurementCommonWidget->zp_setMeasurementTaskTreeModel(zv_currentMeasurementTaskTreeModel);

}
//============================================================
void MainWindow::zh_restoreSettings()
{
    QSettings settings;
    settings.beginGroup(glAppVersion);
    QVariant vData;

    // geometry
    vData = settings.value(glAppGeometryKeyName);
    if(vData.isValid() && !vData.isNull() && vData.canConvert<QByteArray>())
    {
        restoreGeometry(vData.toByteArray());
    }

    // state
    vData = settings.value(glAppStateKeyName);
    if(vData.isValid() && !vData.isNull() && vData.canConvert<QByteArray>())
    {
        restoreState(vData.toByteArray());
    }

    // close settings
    settings.endGroup();

    // appSettings
    ZAppSettings appSettings;
    zh_getAppSettingsFromSettings(appSettings);
    zh_applyAppSettingsToComponents(appSettings);
}
//============================================================
void MainWindow::zh_saveSettings()
{
    // emit signal to another components
    emit zg_saveSettings();

    QSettings settings;
    settings.beginGroup(glAppVersion);
    // geometry
    settings.setValue(glAppGeometryKeyName, QVariant(saveGeometry()));
    // state
    settings.setValue(glAppStateKeyName, QVariant(saveState()));

    settings.endGroup();
}
//============================================================
QFrame* MainWindow::zh_setWidgetToFrame(QWidget* widget)
{
    QFrame* frame = new QFrame();
    QVBoxLayout* frameLayout = new QVBoxLayout(frame);
    frame->setLayout(frameLayout);
    frame->setFrameStyle(QFrame::StyledPanel | QFrame::Plain);
    frame->setLineWidth(1);

    frameLayout->addWidget(widget);
    return frame;
}
//============================================================
void MainWindow::zh_saveAppSettingsToSettings(const ZAppSettings& appSettings)
{
    QSettings settings;
    settings.beginGroup(glAppVersion);
    settings.setValue(glAppSettingsKeyName, QVariant::fromValue<ZAppSettings>(appSettings));
    settings.endGroup();
}
//============================================================
void MainWindow::zh_getAppSettingsFromSettings(ZAppSettings& appSettings)
{
    QSettings settings;
    settings.beginGroup(glAppVersion);

    QVariant vData = settings.value(glAppSettingsKeyName);
    if(vData.isValid() && !vData.isNull() && vData.canConvert<ZAppSettings>())
    {
        appSettings = vData.value<ZAppSettings>();
    }

    settings.endGroup();
}
//============================================================
void MainWindow::zh_appendActionsToMenu(QMenu* menu)
{
    if(menu->objectName() == NS_ObjectNames::glObjectNameMenuFile)
    {
        menu->addSeparator();
        menu->addAction(zv_exitAction);
        menu->addSeparator();
        return;
    }

    if(menu->objectName() == NS_ObjectNames::glObjectNameMenuActions)
    {
        menu->addSeparator();
        menu->addAction(zv_settingsAction);
        return;
    }

    if(menu->objectName() == NS_ObjectNames::glObjectNameMenuView)
    {
        foreach(QDockWidget* dock, zv_dockList)
        {
            //            QAction* viewAction = new QAction(this);
            //            viewAction->setText(dock->windowTitle());
            //            viewAction->setCheckable(true);
            //            QAction* viewAction = new QAction(this);
            //            viewAction->setText(dock->windowTitle());
            //            viewAction->setCheckable(true);

            //            connect(viewAction, &QAction::toggled,
            //                    dock, &QDockWidget::setVisible);
            //            connect(dock, &QDockWidget::visibilityChanged,
            //                    viewAction, &QAction::setChecked);

            menu->addAction(dock->toggleViewAction());
        }
        menu->addSeparator();
        return;
    }

    if(menu->objectName() == NS_ObjectNames::glObjectNameMenuHelp)
    {
        menu->addAction(zv_helpAction);
        menu->addAction(zv_aboutAction);
        menu->addSeparator();
        return;
    }
}
//============================================================
void MainWindow::zh_onAboutAction() const
{

}
//============================================================
void MainWindow::zh_onHelpAction() const
{

}
//============================================================
void MainWindow::zh_onSettingsAction()
{
    ZSettingsDialog dialog;
    ZAppSettings appSettings;
    zh_getAppSettingsFromSettings(appSettings);
    dialog.zh_setAppSettings(appSettings);
    connect(&dialog, &ZSettingsDialog::zg_applySettings,
            this, &MainWindow::zh_applyAppSettingsToComponents);

    if(dialog.exec())
    {
        // get settings from dialog and save to
        dialog.zh_appSettings(appSettings);
        zh_applyAppSettingsToComponents(appSettings);
        zh_saveAppSettingsToSettings(appSettings);
    }
    else
    {
        // restore settings from QSettings
        ZAppSettings appSettings;
        zh_getAppSettingsFromSettings(appSettings);
        zh_applyAppSettingsToComponents(appSettings);
    }

}
//============================================================
void MainWindow::zh_applyAppSettingsToComponents(const ZAppSettings& appSettings)
{
    zv_currentMeasurementCommonWidget->zp_applyAppSettings(appSettings);
}
//============================================================
