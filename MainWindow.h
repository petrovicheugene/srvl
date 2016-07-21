//============================================================
#ifndef MAINWINDOW_H
#define MAINWINDOW_H
//============================================================
#include <QMainWindow>
#include <QSqlDatabase>
//============================================================
class QAction;
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

private:

    // VARS
    QSqlDatabase zv_database;

    // actions
    QAction* zv_exitAction;       /*!< TODO: describe */
    QAction* zv_aboutAction;    /*!< TODO: describe */
    QAction* zv_helpAction;      /*!< TODO: describe */


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
    void zh_createWidgets();
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

private slots:

    /*!
     \brief

    */
    void zh_onAboutAction();
    /*!
     \brief

    */
    void zh_onHelpAction();


};
//============================================================
#endif // MAINWINDOW_H
