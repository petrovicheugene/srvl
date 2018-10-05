//============================================================
#ifndef MAINWINDOW_H
#define MAINWINDOW_H
//============================================================
#include <QMainWindow>
#include <QMessageBox>
#include <QSqlDatabase>
#include <QTextDocument>
#include "ZAppSettings.h"
//============================================================
// Qt
class QAction;
class QDockWidget;
class QFrame;
class QLabel;
class QMenu;
class QSqlRelationalTableModel;

// models
class ZMeasuringManager;
class ZEnergyLineManager;
class ZMeasuringResultTableModel;
class ZHelpBrowser;
//class ZSeriesTaskTreeModel;
//class ZCurrentMeasuringManager;

// components


// widgets
class ZMeasuringCommonWidget;
class ZEnergyLineTableWidget;
class ZDashboard;
class ZPlotter;
class QPrinter;
class ZWidgetWithSidebar;
class ZPlotterDataManager;
//============================================================
/*!
 \brief the MainWindow is an application main class.

    Creates all components and connects them alltogether.

*/
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:


    /*!
     \brief

     \param parent
    */
    MainWindow(const QString& dbName,
               const QString& dbPath,
               QWidget *parent = 0);
    /*!
     \brief

    */
    ~MainWindow();
    bool zp_isDatabaseOpen() const;

signals:

    void zg_saveSettings();

private slots:

    /*!
     \brief

    */
    void zh_onAboutAction() const;

    /*!
     \brief

    */
    void zh_onHelpAction();
    void zh_onExitAction();
    void zh_onCalibrationsAction();
    void zh_onChemicalElementTasksAction();
    void zh_onSampleTasksAction();

    void zh_onRunSQLCommandAction() const;
    void zh_onSettingsAction();

    void zh_applyAppSettingsToComponents(const ZAppSettings &appSettings);
    void zh_applyAppSettings(const ZAppSettings &appSettings);

    void zh_setConnectionStatusToStatusbar(QString connectionState, QMessageBox::Icon icon);
    void zh_setMeasurementParametersToStatusbar(quint32 expoPassedMs,
                                                quint32 deadTimeMs,
                                                quint32 spectrumIntensityIntegral);

    void zh_processMessage(QString msg, QMessageBox::Icon);
    void zh_printResults() const;
    void zh_previewAndPrintResults() const;
    void zh_printResultDoc(QPrinter* printer) const;
    void zh_createOperatorMenu();
    void zh_assignOperator();

private:

    // VARS
    QSqlDatabase zv_database; /*!< TODO: describe */

    // docks
    QList<QDockWidget*> zv_dockList; /*!< TODO: describe */
    QDockWidget* zv_plotterDock;
    QDockWidget* zv_energyLineTableDock;

    // actions
    QAction* zv_settingsAction;       /*!< TODO: describe */
    QAction* zv_runSQLCommandAction;       /*!< TODO: describe */
    QAction* zv_calibrationsAction;
    QAction* zv_chemicalElementTasksAction;
    QAction* zv_sampleTasksAction;

    QAction* zv_exitAction;       /*!< TODO: describe */
//    QAction* zv_previewAndPrintAction;
    QAction* zv_aboutAction;    /*!< TODO: describe */
    QAction* zv_aboutQtAction;
    QAction* zv_helpAction;      /*!< TODO: describe */
    ZHelpBrowser* zv_helpBrowser;

    // widgets
    ZPlotter* zv_plotter;
    ZPlotterDataManager* zv_plotterDataManager;
    QMenu* zv_operatorMenu;
    const QString zv_unknownOperatorString = tr("Unknown operator");

    QLabel* zv_connectionStatusLabel;
    QLabel* zv_spectrumMeasurementParametersLabel;
    bool zv_showMeasuringParameters;
    // views
    ZMeasuringCommonWidget* zv_measuringCommonWidget;
    ZEnergyLineTableWidget* zv_energyLineTableWidget;
    // models
    ZMeasuringManager* zv_measuringManager;

    ZMeasuringResultTableModel* zv_measuringResultTableModel;
    //    ZSeriesTaskTreeModel* zv_currentMeasuringTaskTreeModel;

    // FUNCS

    /*!
     \brief

     \param e
    */
    void closeEvent(QCloseEvent* e);
    /*!
     \brief

    */
    void zh_createActions();
    /*!
     \brief

    */
    void zh_createComponents();
    /*!
     \brief

    */
    void zh_createMenu();
    /*!
     \brief

    */
    void zh_createToolbar();
    /*!
     \brief

    */
    void zh_createConnections();
    /*!
     \brief

    */
    void zh_restoreSettings();
    /*!
     \brief

    */
    void zh_saveSettings();
    QFrame* zh_setWidgetToFrame(QWidget* widget);


    void zh_saveAppSettingsToSettings(const ZAppSettings &);
    void zh_getAppSettingsFromSettings(ZAppSettings& appSettings) const;
    void zh_appendActionsToMenu(QMenu* menu);

    void zh_createResultTextDocument(QTextDocument& textDoc) const;

};
//============================================================
#endif // MAINWINDOW_H
