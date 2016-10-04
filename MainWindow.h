//============================================================
#ifndef MAINWINDOW_H
#define MAINWINDOW_H
//============================================================
#include <QMainWindow>
#include <QSqlDatabase>

#include "ZAppSettings.h"
//============================================================
// Qt
class QAction;
class QDockWidget;
class QFrame;
class QMenu;
class QSqlRelationalTableModel;

// models
//class ZMeasurementResultTableModel;
//class ZSeriesTaskTreeModel;
//class ZCurrentMeasurementManager;

// components
class ZMeasurementManager;


// widgets
class ZCurrentMeasurementCommonWidget;
class ZDashboard;
class ZPlotter;
class ZCurrentMeasurementSampleTableWidget;
class ZCurrentMeasurementTaskTreeWidget;
class ZWidgetWithSidebar;

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
    void zh_onHelpAction() const;
    void zh_onSettingsAction();

    void zh_applyAppSettingsToComponents(const ZAppSettings &appSettings);

private:

    // VARS
    QSqlDatabase zv_database; /*!< TODO: describe */

    // docks
    QList<QDockWidget*> zv_dockList; /*!< TODO: describe */
    QDockWidget* zv_plotterDock;

    // actions
    QAction* zv_settingsAction;       /*!< TODO: describe */
    QAction* zv_exitAction;       /*!< TODO: describe */
    QAction* zv_aboutAction;    /*!< TODO: describe */
    QAction* zv_helpAction;      /*!< TODO: describe */

    // widgets
    ZPlotter* zv_plotter;

    // views
    ZCurrentMeasurementCommonWidget* zv_currentMeasurementCommonWidget;

    // models

    ZMeasurementManager* zv_measurementManager;

//    ZMeasurementResultTableModel* zv_currentMeasurementResultTableModel;
//    ZSeriesTaskTreeModel* zv_currentMeasurementTaskTreeModel;

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
    void zh_getAppSettingsFromSettings(ZAppSettings& appSettings);
    void zh_appendActionsToMenu(QMenu* menu);

};
//============================================================
#endif // MAINWINDOW_H
