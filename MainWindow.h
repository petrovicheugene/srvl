//============================================================
#ifndef MAINWINDOW_H
#define MAINWINDOW_H
//============================================================
#include <QMainWindow>
#include <QSqlDatabase>

#include "ZAppSettings.h"
//============================================================
class ZResultTableWidget;
class ZDashboard;
class ZCentralWidget;

class QAction;
class QMenu;
class QDockWidget;
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
    QList<QDockWidget*> zv_dockList; /*!< TODO: describe */

    // actions
    QAction* zv_settingsAction;       /*!< TODO: describe */
    QAction* zv_exitAction;       /*!< TODO: describe */
    QAction* zv_aboutAction;    /*!< TODO: describe */
    QAction* zv_helpAction;      /*!< TODO: describe */

    // widgets
    ZCentralWidget* zv_centralWidget;
    ZResultTableWidget* zv_resultTableWidget;
    ZDashboard* zv_dashBoard; /*!< TODO: describe */




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
    void zh_saveAppSettingsToSettings(const ZAppSettings &);
    void zh_getAppSettingsFromSettings(ZAppSettings& appSettings);
    void zh_appendActionsToMenu(QMenu* menu);

};
//============================================================
#endif // MAINWINDOW_H
