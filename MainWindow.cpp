//============================================================
#include "MainWindow.h"
#include "ZGeneral.h"

// components
#include "ZControlAction.h"
#include "ZMeasuringCommonWidget.h"
#include "ZDatabaseInspector.h"
#include "ZSettingsDialog.h"
#include "ZWidgetWithSidebar.h"
#include "ZSQLCommanderDialog.h"
#include "ZPlotterDataManager.h"
#include "ZMeasurementParametersHandler.h"
#include "ZPasswordDialog.h"
#include "ZTranslatorManager.h"

// views
#include "ZPlotter.h"
#include "ZMeasuringResultTableWidget.h"
#include "ZMeasuringSeriesTaskTreeWidget.h"
#include "ZEnergyLineTableWidget.h"
#include "ZHelpBrowser.h"
#include "ZCalibrationListDialog.h"
#include "ZSampleTaskListDialog.h"
#include "ZChemicalTaskListDialog.h"
#include "ZSeriesMeasurementDialog.h"
// models
//#include "ZMeasuringResultTableModel.h"
//#include "ZSeriesTaskTreeModel.h"
#include "ZMeasuringManager.h"
#include "ZEnergyLineManager.h"
#include "ZMeasuringResultTableModel.h"

//#include "ZCur.h"

// Qt
#include <QApplication>
#include <QCloseEvent>
#include <QAction>
#include <QDesktopWidget>
#include <QDir>
#include <QDockWidget>
#include <QFrame>
#include <QLabel>
#include <QMenu>
#include <QMessageBox>
#include <QMenuBar>
#include <QPageSetupDialog>
#include <QPixmap>
#include <QPrintDialog>
#include <QPrinter>
#include <QPrintPreviewDialog>
#include <QSettings>
#include <QStatusBar>
#include <QSqlError>
#include <QSqlRelationalTableModel>
#include <QStyle>
#include <QTableView>
#include <QTextDocumentFragment>
#include <QTextTable>
#include <QTextTableCell>
#include <QTextTableCellFormat>
#include <QVBoxLayout>

// Qt controls

// TEMP
#include <QSqlQuery>
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
    qRegisterMetaType<ZDeviceSettings>("ZDeviceSettings");
    qRegisterMetaTypeStreamOperators<ZDeviceSettings>("ZDeviceSettings");
    qRegisterMetaType<ZAppSettings>("ZAppSettings");
    qRegisterMetaTypeStreamOperators<ZAppSettings>("ZAppSettings");

    setWindowTitle(qApp->applicationDisplayName());

    zv_exitAction = nullptr;
    zv_aboutAction = nullptr;
    zv_helpAction = nullptr;
    zv_helpBrowser = nullptr;
    //    zv_currentMeasuringTaskTreeModel = 0;
    // zv_sqlSeriesModel = 0;
    zv_measuringManager = nullptr;
    zv_energyLineTableWidget = nullptr;
    zv_measuringResultTableModel = nullptr;

    zv_plotter = nullptr;

    zh_createActions();
    zh_createComponents();
    zh_createMenu();
    zh_createToolbar();
    zh_createConnections();
    zh_restoreSettings();

    // plotter starting settings
    if(zv_plotter != nullptr)
    {
        QMetaObject::invokeMethod(zv_plotter, "zp_fitInBoundingRect",
                                  Qt::QueuedConnection);

    }

    zv_measuringManager->zp_stopSeries();
    //    ZAddCalibrationDialog dialog;
    //    dialog.exec();
}
//============================================================
MainWindow::~MainWindow()
{
    zh_saveSettings();
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

    // zh_saveSettings();
}
//============================================================
void MainWindow::zh_createActions()
{
    zv_settingsAction = new QAction(this);
    zv_settingsAction->setIcon(QIcon(NS_Icons::glIconSettings));
    zv_settingsAction->setText(tr("Settings"));
    zv_settingsAction->setToolTip(tr("Run settings of the application"));

    zv_exitAction = new QAction(this);
    zv_exitAction->setIcon(QIcon(NS_Icons::glIconExitApp));
    zv_exitAction->setText(tr("Exit"));
    zv_exitAction->setToolTip(tr("Exit the application"));

    zv_calibrationsAction = new QAction(this);
    zv_calibrationsAction->setIcon(QIcon());
    zv_calibrationsAction->setText(tr("Calibrations"));
    zv_calibrationsAction->setToolTip(tr("Manage calibration list"));

    zv_chemicalElementTasksAction = new QAction(this);
    zv_chemicalElementTasksAction->setIcon(QIcon());
    zv_chemicalElementTasksAction->setText(tr("Chemical element tasks"));
    zv_chemicalElementTasksAction->setToolTip(tr("Manage chemical element task list"));

    zv_sampleTasksAction = new QAction(this);
    zv_sampleTasksAction->setIcon(QIcon());
    zv_sampleTasksAction->setText(tr("Sample tasks"));
    zv_sampleTasksAction->setToolTip(tr("Manage sample task list"));

    zv_seriesMeasurementAction = new QAction(this);
    zv_seriesMeasurementAction->setIcon(QIcon());
    zv_seriesMeasurementAction->setText(tr("Measured series"));
    zv_seriesMeasurementAction->setToolTip(tr("Show measured series list"));

    //    zv_printAction  = new QAction(this);
    //    zv_printAction->setIcon(QIcon(NS_Icons::glIconStringExitApp));
    //    zv_printAction->setText(tr("Print"));
    //    zv_printAction->setToolTip(tr("Print results"));

    //    zv_previewAndPrintAction = new QAction(this);
    //    zv_previewAndPrintAction->setIcon(QIcon(NS_Icons::glIconStringExitApp));
    //    zv_previewAndPrintAction->setText(tr("Preview and print"));
    //    zv_previewAndPrintAction->setToolTip(tr("Preview and print results"));

    zv_aboutAction = new QAction(this);
    zv_aboutAction->setIcon(QIcon(NS_Icons::glIconAbout));
    zv_aboutAction->setText(tr("About"));
    zv_aboutAction->setToolTip(tr("About the application"));

    zv_aboutQtAction = new QAction(this);
    zv_aboutQtAction->setIcon(QIcon(NS_Icons::glIconQt));
    zv_aboutQtAction->setText(tr("About Qt"));
    zv_aboutQtAction->setToolTip(tr("About Qt framework"));

    zv_helpAction = new QAction(this);
    zv_helpAction->setIcon(QIcon(NS_Icons::glIconHelp));
    zv_helpAction->setText(tr("Help"));
    zv_helpAction->setToolTip(tr("Show user guide"));

    zv_runSQLCommandAction = new QAction(this);
    zv_runSQLCommandAction->setIcon(QIcon());
    zv_runSQLCommandAction->setText("SQL commander");
    zv_runSQLCommandAction->setToolTip("Run SQL commander");

}
//============================================================
void MainWindow::zh_createComponents()
{
    // Measuring Widget
    zv_measuringCommonWidget = new ZMeasuringCommonWidget(this);
    setCentralWidget(zv_measuringCommonWidget);


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

    zv_plotterDataManager = new ZPlotterDataManager(this);

    // setting to plotter dock
    zv_plotterDock->setWidget(frame);


    // Chemical element energy line widget
    // create dock
    zv_energyLineTableDock = new QDockWidget(this);
    zv_energyLineTableDock->setObjectName("ENERGY_LINE_TABLE_DOCK");
    zv_energyLineTableDock->setWindowTitle(tr("Energy lines"));
    zv_dockList << zv_energyLineTableDock;
    addDockWidget(Qt::RightDockWidgetArea, zv_energyLineTableDock);

    // create widget
    zv_energyLineTableWidget = new ZEnergyLineTableWidget(this);
    frame = zh_setWidgetToFrame(zv_energyLineTableWidget);

    // setting to widget dock
    zv_energyLineTableDock->setWidget(frame);


    // DATA MODELS
    zv_measuringManager = new ZMeasuringManager(this);
    QList<ZControlAction*> contextActionList = zv_measuringManager->zp_sampleContextActions();
    zv_measuringCommonWidget->zp_appendSampleContextMenuActions(contextActionList);

    // measuring models
    zv_measuringResultTableModel = new ZMeasuringResultTableModel(this);

    statusBar()->setSizeGripEnabled(true);

    // Connection status label
    zv_connectionStatusLabel = new QLabel(this);
    statusBar()->addWidget(zv_connectionStatusLabel);

    zv_spectrumMeasurementParametersLabel = new QLabel(this);
    statusBar()->addWidget(zv_spectrumMeasurementParametersLabel);

}
//============================================================
void MainWindow::zh_createMenu()
{
    QMenuBar* menubar = menuBar();
    menubar->setCursor(Qt::PointingHandCursor);
    // File
    QMenu* menu = menubar->addMenu(tr("File"));
    menu->setCursor(Qt::PointingHandCursor);
    menu->setObjectName(NS_ObjectNames::glObjectNameMenuFile);
    zv_measuringManager->zp_appendActionsToMenu(menu);
    zh_appendActionsToMenu(menu);

    //    // Edit
    //    menu = menubar->addMenu(NS_Menus::glMenuEdit);
    //    menu->setCursor(Qt::PointingHandCursor);
    //    menu->setObjectName(NS_ObjectNames::glObjectNameMenuEdit);
    //    zh_appendActionsToMenu(menu);

    // View
    menu = menubar->addMenu(tr("View"));
    menu->setCursor(Qt::PointingHandCursor);
    menu->setObjectName(NS_ObjectNames::glObjectNameMenuView);
    zh_appendActionsToMenu(menu);

    // Actions
    menu = menubar->addMenu(tr("Actions"));
    menu->setCursor(Qt::PointingHandCursor);
    menu->setObjectName(NS_ObjectNames::glObjectNameMenuActions);
    zv_measuringManager->zp_appendActionsToMenu(menu);
    zh_appendActionsToMenu(menu);

    // Help
    menu = menubar->addMenu(tr("Help"));
    menu->setCursor(Qt::PointingHandCursor);
    menu->setObjectName(NS_ObjectNames::glObjectNameMenuHelp);
    zh_appendActionsToMenu(menu);

    QMenuBar* rightMenu = new QMenuBar(this);
    zv_operatorMenu = rightMenu->addMenu(tr("Operator"));
    connect(zv_operatorMenu, &QMenu::aboutToShow,
            this, &MainWindow::zh_createOperatorMenu);
    menubar->setCornerWidget(rightMenu);
}
//============================================================
void MainWindow::zh_createOperatorMenu()
{
    zv_operatorMenu->clear();

    QSqlQuery query;
    QString queryString = QString("SELECT * FROM operators");

    if(!query.prepare(queryString))
    {
        QString msg = query.lastError().text();
        QMessageBox::critical(this, tr("Operator loading error"), msg, QMessageBox::Ok);
        return ;
    }

    if(!query.exec())
    {
        QString msg = query.lastError().text();
        QMessageBox::critical(this, tr("Operator loading error"), msg, QMessageBox::Ok);
        return ;
    }

    QVariant vData;
    //    int id;
    QString login;
    QByteArray password;
    QAction* action;
    while(query.next())
    {
        //        vData = query.value(0);
        //        if(!vData.isValid() || !vData.canConvert<int>())
        //        {
        //            continue;
        //        }

        //        id = vData.toInt();

        vData = query.value(1);
        if(!vData.isValid() || !vData.canConvert<QString>())
        {
            continue;
        }

        login = vData.toString();

        vData = query.value(2);
        if(!vData.isValid() || !vData.canConvert<int>())
        {
            continue;
        }

        password = vData.toByteArray();

        action = new QAction(zv_operatorMenu);
        action->setText(login);
        action->setData(password);
        connect(action, &QAction::triggered,
                this, &MainWindow::zh_assignOperator);
        zv_operatorMenu->addAction(action);
    }

    if(!zv_operatorMenu->actions().isEmpty())
    {
        action = new QAction(zv_operatorMenu);
        action->setText(tr("Unknown"));
        action->setData(QByteArray());
        connect(action, &QAction::triggered,
                this, &MainWindow::zh_assignOperator);
        zv_operatorMenu->insertAction(zv_operatorMenu->actions().first(), action);
    }
}
//============================================================
void MainWindow::zh_assignOperator()
{
    if(!sender())
    {
        return;
    }

    QAction* action = qobject_cast<QAction*>(sender());
    if(!action)
    {
        return;
    }

    // password request if needed
    QByteArray password = action->data().toByteArray();
    if(!password.isEmpty())
    {
        QString passwordString;
        if(!ZPasswordDialog::mf_decryptByteArrayToString(passwordString, password))
        {
            QString msg = tr("Cannot decrypt password for operator %1.").arg(action->text());
            QMessageBox::critical(this, tr("Operator selection error"), msg, QMessageBox::Ok);
            return;
        }

        ZPasswordDialog dialog(passwordString);
        if(!dialog.exec())
        {
            return;
        }
    }

    QString operatorText;
    if(action->text() == zv_unknownOperatorString)
    {
        operatorText = tr("Operator");
    }
    else
    {
        zv_operatorMenu->setTitle(action->text());
    }
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
            zv_measuringCommonWidget, &ZMeasuringCommonWidget::zp_saveSettings);
    connect(zv_runSQLCommandAction, &QAction::triggered,
            this, &MainWindow::zh_onRunSQLCommandAction);
    connect(zv_measuringManager, &ZMeasuringManager::zg_connectionState,
            this, &MainWindow::zh_setConnectionStatusToStatusbar);
    connect(zv_measuringManager, &ZMeasuringManager::zg_message,
            this, &MainWindow::zh_processMessage);
    connect(zv_measuringManager, &ZMeasuringManager::zg_measurementParameters,
            this, &MainWindow::zh_setMeasurementParametersToStatusbar);

    connect(zv_aboutAction, &QAction::triggered,
            this, &MainWindow::zh_onAboutAction);
    connect(zv_aboutQtAction, &QAction::triggered,
            qApp, &QApplication::aboutQt);

    connect(zv_helpAction, &QAction::triggered,
            this, &MainWindow::zh_onHelpAction);

    connect(zv_exitAction, &QAction::triggered,
            this, &MainWindow::zh_onExitAction);

    connect(zv_languageMenu, &QMenu::aboutToShow,
            this, &MainWindow::zh_fillLanguageMenu);


    connect(zv_calibrationsAction, &QAction::triggered,
            this, &MainWindow::zh_onCalibrationsAction);
    connect(zv_chemicalElementTasksAction, &QAction::triggered,
            this, &MainWindow::zh_onChemicalElementTasksAction);
    connect(zv_sampleTasksAction, &QAction::triggered,
            this, &MainWindow::zh_onSampleTasksAction);

    connect(zv_seriesMeasurementAction, &QAction::triggered,
            this, &MainWindow::zh_onSeriesMeasurementAction);



    connect(zv_measuringManager, &ZMeasuringManager::zg_inquiryCurrentIndex,
            zv_measuringCommonWidget, &ZMeasuringCommonWidget::zp_currentIndex);
    connect(zv_measuringManager, &ZMeasuringManager::zg_invokeNotifyCurrent,
            zv_measuringCommonWidget, &ZMeasuringCommonWidget::zp_notifyCurrent);
    connect(zv_measuringCommonWidget, &ZMeasuringCommonWidget::zg_selectionChanged,
            zv_measuringManager, &ZMeasuringManager::zp_onSelectionChange);

    connect(zv_measuringManager, &ZMeasuringManager::zg_inquiryResultsPrinting,
            this, &MainWindow::zh_printResults);
    connect(zv_measuringManager, &ZMeasuringManager::zg_inquiryResultsPreviewAndPrinting,
            this, &MainWindow::zh_previewAndPrintResults);

    connect(zv_measuringManager, &ZMeasuringManager::zg_inquiryMeasuringConditionsAndSpectrumForIndex,
            zv_measuringResultTableModel, &ZMeasuringResultTableModel::zp_measuringConditionsAndSpectrumForIndex);
    connect(zv_measuringManager, &ZMeasuringManager::zg_inquirySelectedSpectrumMap,
            zv_measuringResultTableModel, &ZMeasuringResultTableModel::zp_selectedSpectrumMap);

    connect(zv_measuringCommonWidget, &ZMeasuringCommonWidget::zg_currentIndexChanged,
            zv_measuringResultTableModel, &ZMeasuringResultTableModel::zp_onCurrentIndexChanged);

    zv_plotterDataManager->zp_connectToMeasuringManager(zv_measuringManager);
    // connect to zv_energyLineTableWidget

    connect(zv_energyLineTableWidget, &ZEnergyLineTableWidget::zg_energyLineOperation,
            zv_plotterDataManager, &ZPlotterDataManager::zp_onEnergyLineOperation);
    connect(zv_plotterDataManager, &ZPlotterDataManager::zg_requestEnergyLineEnergyValue,
            zv_energyLineTableWidget, &ZEnergyLineTableWidget::zp_energyLineEnergyValue);
    connect(zv_plotterDataManager, &ZPlotterDataManager::zg_requestEnergyLineRelativeIntensity,
            zv_energyLineTableWidget, &ZEnergyLineTableWidget::zp_energyLineRelativeIntensity);

    connect(zv_plotterDataManager, &ZPlotterDataManager::zg_requestEnergyLineVisibility,
            zv_energyLineTableWidget, &ZEnergyLineTableWidget::zp_energyLineVisibility);
    connect(zv_plotterDataManager, &ZPlotterDataManager::zg_requestEnergyLineColor,
            zv_energyLineTableWidget, &ZEnergyLineTableWidget::zp_energyLineColor);


    zv_plotterDataManager->zp_connectToPlotter(zv_plotter);

    connect(zv_measuringManager, &ZMeasuringManager::zg_inquiryCurrentVisibleSceneRect,
            zv_plotter, &ZPlotter::zp_currentVisibleSceneRect);

    // all about sample table
    zv_measuringCommonWidget->zp_connectToMeasuringManager(zv_measuringManager);
    zv_measuringResultTableModel->zp_connectToMeasuringManager(zv_measuringManager);
    zv_measuringCommonWidget->zp_setMeasuringResultTableModel(zv_measuringResultTableModel);

}
//============================================================
void MainWindow::zh_restoreSettings()
{
    QSettings settings;
    settings.beginGroup(qApp->applicationVersion());
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
    settings.beginGroup(qApp->applicationVersion());
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
    QVBoxLayout* frameLayout = new QVBoxLayout;
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
    settings.beginGroup(qApp->applicationVersion());
    settings.setValue(glAppSettingsKeyName, QVariant::fromValue<ZAppSettings>(appSettings));
    settings.endGroup();
}
//============================================================
void MainWindow::zh_getAppSettingsFromSettings(ZAppSettings& appSettings) const
{
    QSettings settings;
    settings.beginGroup(qApp->applicationVersion());

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
        //        menu->addAction(zv_printAction);
        //        menu->addAction(zv_previewAndPrintAction);
        //        menu->addSeparator();
        // language
        zv_languageMenu = new QMenu;
        zv_languageMenu->setIcon(QIcon(":/images/ZImages/earthGlobe-16.png"));
        ZTranslatorManager translatorManager;
        QString currentLanguage;
        bool ok = false;
        translatorManager.zp_currentLanguageName(currentLanguage, &ok);
        if(ok)
        {
            zv_languageMenu->setTitle(currentLanguage);
        }

        zv_languageMenu->setToolTip(tr("Application language"));

        menu->addMenu(zv_languageMenu);



        menu->addAction(zv_exitAction);
        menu->addSeparator();
        return;
    }

    if(menu->objectName() == NS_ObjectNames::glObjectNameMenuActions)
    {
        menu->addSeparator();
        menu->addAction(zv_calibrationsAction);
        menu->addAction(zv_chemicalElementTasksAction);
        menu->addAction(zv_sampleTasksAction);
        menu->addSeparator();
        menu->addAction(zv_seriesMeasurementAction);

        menu->addSeparator();
        menu->addAction(zv_runSQLCommandAction);
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
        menu->addAction(zv_aboutQtAction);

        menu->addSeparator();
        return;
    }
}
void MainWindow::zh_appLanguageControl()
{
    if(!sender())
    {
        return;
    }

    QAction* action = dynamic_cast<QAction*>(sender());
    if(!action)
    {
        return;
    }

    ZTranslatorManager translatorManager;
    QString currentLanguageName;
    bool ok = false;
    translatorManager.zp_currentLanguageName(currentLanguageName, &ok);

    qDebug() << "Current lang" << currentLanguageName << "Installied lang" << action->text();

    if(ok && action->text() == currentLanguageName)
    {
        return;
    }

    ok = false;
    translatorManager.zp_setApplicationLanguage(action->text(), &ok);
    QString msg;
    if(ok)
    {
        msg = tr("The language of application has been changed.\n"
                 "In order for the changes to take effect, please restart the application.");
    }
    else
    {
        msg = translatorManager.zp_lastError().isEmpty() ?
                    tr("Unknown language settings error.") : translatorManager.zp_lastError();

    }

    QString title = tr("Language settings");
    QMessageBox::information(this, title, msg, QMessageBox::Ok);
}
//==========================================================
void MainWindow::zh_fillLanguageMenu()
{
    zv_languageMenu->clear();
    ZTranslatorManager translatorManager;
    QStringList availableLanguageNameList;
    translatorManager.zp_availableLanguageNameList(availableLanguageNameList);

    foreach(QString languageName, availableLanguageNameList)
    {
        QAction* action = zv_languageMenu->addAction(languageName);
        connect(action, &QAction::triggered,
                this, &MainWindow::zh_appLanguageControl);
    }
}
//============================================================
void MainWindow::zh_onAboutAction() const
{
    QString title = tr("About %1").arg(qApp->applicationDisplayName());
    //    QString text = tr("The application is a supplement to a SRV spectrometer software. It provides to make extra calculation of chemical concentration that cannot be directly  measured."
    //                      "");
    //    QString htmlText = QString(
    //                "<table border=0 cellspacing = 15>"
    //                "<tr>"
    //                "<td align = left><img src=:/images/ZImages/CR3_64.png></td>"
    //                "<td align = left><h2 align = center>CRecalc 2.3</h2>"
    //                "<p>Copyright &copy; TechnoAnalyt Ltd., 2014.  All rights reserved.</p>"
    //                "<p>%1</p>"
    //                "</td>"
    //                "</tr>"
    //                "</table>").arg(text);

    QString text = QString();//tr("%1 is an application that controls other console application.").arg(glAppProduct);

    QString htmlText = QString(
                "<table border=0 cellspacing = 15>"
                "<tr>"
                "<td align = left><img src=:/images/appIcon.png></td>"
                "<td align = left><h1 align = center>%1</h1>"
                "</td>"
                "</tr>"
                "</table>"
                "<p>%6 - %2</p>"
                "<p>%3</p>"
                "<p>%7<br> "
                "Company website: <a href=\"http://%8/\">%8</a><br>"
                "%5: %4.<br>"
                "Author's email: <a href=mailto:petrovich.eugene@gmail.com?Subject=My%20Subject>petrovich.eugene@gmail.com</a></p>"
                ).arg(qApp->applicationDisplayName(),
                      qApp->applicationVersion(),
                      text,
                      tr("Eugene Petrovich"),
                      tr("Author"),
                      tr("Version"),
                      qApp->property("appCopyright").toString(), /*copyright*/
                      qApp->organizationDomain()
                      );

    QMessageBox::about(centralWidget(), title, htmlText);
}
//============================================================
void MainWindow::zh_onHelpAction()
{
    if(zv_helpBrowser == nullptr)
    {
        QUrl source = QUrl(".htm");
        QStringList searchList;

        QDir dir = QApplication::applicationDirPath();
        dir.cd("Doc");
        searchList << dir.absolutePath(); // << ":/docs"; Streams.files
        dir.cd("StreamControl.files");
        searchList << dir.absolutePath() << ":/docs";

        zv_helpBrowser = ZHelpBrowser::zp_instance(searchList, source, centralWidget());
        zv_helpBrowser->setAttribute(Qt::WA_GroupLeader);
        QSettings settings;
        settings.beginGroup(qApp->applicationVersion());

        settings.beginGroup("AppState");
        QVariant vData = settings.value("help browser geometry");
        settings.endGroup();
        settings.endGroup();

        if(vData.isValid() && !vData.isNull() && vData.canConvert<QByteArray>())
        {
            zv_helpBrowser->restoreGeometry(vData.toByteArray());
        }
        zv_helpBrowser->show();
    }
    else if(!zv_helpBrowser->isVisible())
    {
        zv_helpBrowser->setVisible(true);
        zv_helpBrowser->zp_restoreGeometry();
    }
}
//============================================================
void MainWindow::zh_onExitAction()
{
    close();
}
//============================================================
void MainWindow::zh_onCalibrationsAction()
{
    QSqlTableModel chemicalSQLTableModel;
    chemicalSQLTableModel.setTable("chemicals");
    chemicalSQLTableModel.select();
    chemicalSQLTableModel.setEditStrategy(QSqlTableModel::OnManualSubmit);
    chemicalSQLTableModel.setHeaderData(1, Qt::Horizontal, QVariant(tr("Name")));

    QSqlTableModel calibrationSQLTableModel;
    calibrationSQLTableModel.setTable("calibrations");
    calibrationSQLTableModel.select();
    calibrationSQLTableModel.setEditStrategy(QSqlTableModel::OnManualSubmit);
    calibrationSQLTableModel.setHeaderData(1, Qt::Horizontal, QVariant(tr("Name")));
    calibrationSQLTableModel.setHeaderData(2, Qt::Horizontal, QVariant(tr("Description")));

    ZCalibrationListDialog dialog(&chemicalSQLTableModel, nullptr, &calibrationSQLTableModel);

    dialog.exec();
}
//============================================================
void MainWindow::zh_onChemicalElementTasksAction()
{
    ZChemicalTaskListDialog dialog;
    dialog.exec();
}
//============================================================
void MainWindow::zh_onSampleTasksAction()
{
    ZSampleTaskListDialog dialog;
    dialog.exec();
}
//============================================================
void MainWindow::zh_onSeriesMeasurementAction()
{
    ZSeriesMeasurementDialog dialog;
    dialog.exec();
}
//============================================================
void MainWindow::zh_onRunSQLCommandAction() const
{
    ZSQLCommanderDialog dialog;
    dialog.setWindowFlags(Qt::Tool);

    dialog.exec();

}
//============================================================
void MainWindow::zh_onSettingsAction()
{
    ZSettingsDialog dialog;
    dialog.setWindowFlags(Qt::Tool);

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
    zh_applyAppSettings(appSettings);
    zv_measuringCommonWidget->zp_applyAppSettings(appSettings);
    zv_measuringManager->zp_applyAppSettings(appSettings);
}
//============================================================
void MainWindow::zh_applyAppSettings(const ZAppSettings &appSettings)
{
    zv_showMeasuringParameters = appSettings.zv_showMeasuringParameters;
}
//============================================================
void MainWindow::zh_setConnectionStatusToStatusbar(QString connectionState,
                                                   QMessageBox::Icon icon)
{
    QString connectionStateString;

    if(icon == QMessageBox::Information)
    {
        connectionStateString = tr("<font color=green><b>%1</b></font>").arg(connectionState);
    }
    else if(icon == QMessageBox::Critical)
    {
        connectionStateString = tr("<font color=red><b>%1</b></font>").arg(connectionState);
    }
    else if(icon == QMessageBox::Warning)
    {
        connectionStateString = tr("<font color=yellow><b>%1</b></font>").arg(connectionState);
    }
    else if(icon == QMessageBox::Question)
    {
        connectionStateString = tr("<font color=blue><b>%1</b></font>").arg(connectionState);
    }

    zv_connectionStatusLabel->setText(connectionStateString);
}
//============================================================
void MainWindow::zh_setMeasurementParametersToStatusbar(quint32 expoPassedMs,
                                                        quint32 deadTimeMs,
                                                        quint32 spectrumIntensityIntegral)
{
    if(!zv_showMeasuringParameters)
    {
        return;
    }

    QString paramHTMLString;
    ZMeasuringParametersHandler::zp_calcParametersAndConvertToHtml(expoPassedMs,
                                                                   deadTimeMs,
                                                                   spectrumIntensityIntegral,
                                                                   paramHTMLString);

    zv_spectrumMeasurementParametersLabel->setText(paramHTMLString);
}
//============================================================
void MainWindow::zh_processMessage(QString msg, QMessageBox::Icon icon)
{
    if(icon == QMessageBox::Information)
    {
        QMessageBox::information(this, qApp->applicationDisplayName(), msg, QMessageBox::Ok);
    }
    else if(icon == QMessageBox::Critical)
    {
        QMessageBox::critical(this, qApp->applicationDisplayName(), msg, QMessageBox::Ok);
    }
    else if(icon == QMessageBox::Warning)
    {
        QMessageBox::warning(this, qApp->applicationDisplayName(), msg, QMessageBox::Ok);
    }
    else if(icon == QMessageBox::Question)
    {
        // connectionStateString = tr("<font color=blue><b>%1</b></font>").arg(connectionState);
    }

}
//============================================================
void MainWindow::zh_printResults() const
{
    QPrinter printer;
    if(!printer.isValid())
    {
        QPrintDialog dialog(&printer);
        if(!dialog.exec())
        {
            return;
        }
    }

    zh_printResultDoc(&printer);
}
//============================================================
void  MainWindow::zh_previewAndPrintResults() const
{
    QPrinter printer;
    QPrintPreviewDialog dialog(&printer);
    connect(&dialog, &QPrintPreviewDialog::paintRequested,
            this, &MainWindow::zh_printResultDoc);

    dialog.exec();

}
//============================================================
void MainWindow::zh_printResultDoc(QPrinter* printer) const
{
    // create text doc
    QTextDocument textDoc;
    zh_createResultTextDocument(textDoc);

    textDoc.print(printer);
}
//============================================================
void MainWindow::zh_createResultTextDocument(QTextDocument& textDoc) const
{
    ZAppSettings appSettings;
    zh_getAppSettingsFromSettings(appSettings);

    QString deviceSerialNumber = appSettings.zv_deviceSettings.zv_deviceSerial;

    QDateTime startDateTime = zv_measuringManager->zp_currentMeasuringStartDateTime();
    QDateTime finishDateTime = zv_measuringManager->zp_currentMeasuringFinishDateTime();
    QString currentSeriesName = zv_measuringManager->zp_currentSeriesName();


    if(currentSeriesName.isEmpty())
    {
        currentSeriesName = tr("No name");
    }
    QString formatString = "dd.MM.yyyy hh:mm:ss";
    QString startTimeString = startDateTime.isValid()? startDateTime.toString(formatString) : tr("Unknown");
    QString finishTimeString = finishDateTime.isValid()? finishDateTime.toString(formatString) : tr("Unknown");

    QString infoString = tr("<h6>SRV %1<br>").arg(deviceSerialNumber);
    infoString += tr("Sample series: %1</h6><br>").arg(currentSeriesName);
    infoString += tr("Start: %1<br>").arg(startTimeString, finishTimeString);
    infoString += tr("Finish: %1<br>").arg(finishTimeString);

    QTextCursor cursor(&textDoc);
    cursor.insertHtml(infoString);

    // insert table
    QTextDocument tableTextDoc;
    QTextCursor tableCursor(&tableTextDoc);
    // row and column count
    int rowCount = zv_measuringResultTableModel->rowCount();
    int columnCount = zv_measuringResultTableModel->columnCount();
    int spectrumCount = zv_measuringResultTableModel->zp_spectrumColumnCount();
    if(rowCount > 0 && (columnCount - spectrumCount) > 0)
    {
        // tableFormat
        QTextTableFormat tableFormat;
        tableFormat.setCellPadding(5);
        tableFormat.setCellSpacing(0);
        tableFormat.setPadding(0);
        tableFormat.setBorder(1);

        //        tableFormat.setCellPadding(10);
        tableFormat.setTopMargin(5);
        tableFormat.setBottomMargin(5);
        tableFormat.setBorderBrush(QBrush(QColor(Qt::black), Qt::SolidPattern));


        tableFormat.setHeaderRowCount(1);
        tableFormat.setBorderStyle(QTextFrameFormat::BorderStyle_Solid);

        int tableColumnCount = columnCount - spectrumCount + 1;
        QTextTable* table = tableCursor.insertTable(rowCount + 1, tableColumnCount, tableFormat);
        QVariant vData;
        int modelColumn;
        for(int row = 0; row < rowCount + 1; row++)
        {
            for(int col = 0; col < tableColumnCount; col++)
            {
                QTextTableCell headerCell = table->cellAt(row, col);
                QTextCursor headerCellCursor = headerCell.firstCursorPosition();

                if(col == 0 && row > 0)
                {
                    // row number
                    headerCellCursor.insertText(QString::number(row));
                    continue;
                }

                modelColumn = col - 1;

                if(col > 2)
                {
                    modelColumn += spectrumCount;
                }

                if(row == 0)
                {
                    // header
                    vData = zv_measuringResultTableModel->headerData(modelColumn, Qt::Horizontal, Qt::DisplayRole);
                }
                else
                {
                    QModelIndex index = zv_measuringResultTableModel->index(row - 1, modelColumn, QModelIndex());
                    if(!index.isValid())
                    {
                        continue;
                    }

                    vData = index.data(Qt::DisplayRole);
                }

                if(!vData.isValid() || !vData.canConvert<QString>())
                {
                    continue;
                }

                headerCellCursor.insertText(vData.toString());
            }
        }
    }

    cursor.insertFragment(QTextDocumentFragment(&tableTextDoc));


    // insert offset
    cursor.insertHtml("<p><br><br><br></p>");

    // basement
    infoString = tr("Operator: _______________________________   %1").arg(zv_operatorMenu->title());
    cursor.insertHtml(infoString);

}
//============================================================
