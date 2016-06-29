//============================================================
#ifndef MAINWINDOW_H
#define MAINWINDOW_H
//============================================================
#include <QMainWindow>
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


    MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:

    // VARS
    // actions
    QAction* zv_exitAction;       /*!< TODO: describe */
    QAction* zv_aboutAction;    /*!< TODO: describe */
    QAction* zv_helpAction;      /*!< TODO: describe */


    // FUNCS

    void closeEvent(QCloseEvent* e);
    void zh_createActions();
    void zh_createComponents();
    void zh_createWidgets();
    void zh_createMenu();
    void zh_createToolbar();
    void zh_createConnections();
    void zh_restoreSettings();
    void zh_saveSettings();

private slots:

    void zh_onAboutAction();
    void zh_onHelpAction();


};
//============================================================
#endif // MAINWINDOW_H
