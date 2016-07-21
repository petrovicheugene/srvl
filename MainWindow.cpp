//============================================================
#include "MainWindow.h"
#include "ZConstants.h"
#include "ZDatabaseInspector.h"

// Qt
#include <QCloseEvent>
#include <QAction>
#include <QSettings>
#include <QMessageBox>
#include <QDockWidget>
// Qt controls

//============================================================
/*!
 \brief TODO: describe main function

 Detailed description

 \param parent TODO: describe
*/
MainWindow::MainWindow(const QString &dbName, const QString &dbPath, QWidget *parent)
    : QMainWindow(parent)
{
    // connect to database
    QString msg;
    if(!ZDatabaseInspector::zp_connectToDatabase(dbName, dbPath, zv_database, msg))
    {
        QMessageBox::critical(this, glErrorString, msg, QMessageBox::Ok);
        return;
    }
    
    
    setWindowTitle(glAppProduct);

    zv_exitAction = 0;
    zv_aboutAction = 0;
    zv_helpAction = 0;


    zh_createActions();
    zh_createComponents();
    zh_createWidgets();
    zh_createMenu();
    zh_createToolbar();
    zh_createConnections();
    zh_restoreSettings();

    // plotter starting settings
    //    if(zv_plotter != 0)
    //    {
    //        QMetaObject::invokeMethod(zv_plotter, "zp_fitInBoundingRect",
    //                                  Qt::QueuedConnection);
    //    }


}
//============================================================
/*!
 \brief

*/
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
/*!
 \brief

 \param e
*/
void MainWindow::closeEvent(QCloseEvent* e)
{
    QString questionString = tr("Quit the application?");
    if(QMessageBox::question(this, tr("Quit the application"), questionString,
                             QMessageBox::Yes, QMessageBox::No) == QMessageBox::No)
    {
        e->ignore();
        return;
    }

    zh_saveSettings();
}
//============================================================
/*!
 \brief

*/
void MainWindow::zh_createActions()
{
    zv_exitAction = new QAction(this);
    zv_exitAction->setIcon(QIcon(glExitAppIconString));
    zv_exitAction->setText(tr("Exit"));
    zv_exitAction->setToolTip(tr("Exit the application"));

    zv_aboutAction = new QAction(this);
    zv_aboutAction->setIcon(QIcon(glAboutIconString));
    zv_aboutAction->setText(tr("About"));
    zv_aboutAction->setToolTip(tr("About the application"));

    zv_helpAction = new QAction(this);
    zv_helpAction->setIcon(QIcon(glHelpIconString));
    zv_helpAction->setText(tr("Help"));
    zv_helpAction->setToolTip(tr("Show user guide"));

}
//============================================================
/*!
 \brief TODO: describe zh_createComponents function

 Detailed description zh_createComponents

 \param parent TODO: describe
*/
void MainWindow::zh_createComponents()
{

}
//============================================================
/*!
 \brief TODO: describe zh_createWidgets function

 Detailed description zh_createWidgets

 \param parent TODO: describe
*/
void MainWindow::zh_createWidgets()
{

}
//============================================================
/*!
 \brief TODO: describe main function

 Detailed description

 \param parent TODO: describe
*/
void MainWindow::zh_createMenu()
{

}
//============================================================
/*!
 \brief

*/
void MainWindow::zh_createToolbar()
{

}
//============================================================
/*!
 \brief

*/
void MainWindow::zh_createConnections()
{

}
//============================================================
/*!
 \brief

*/
void MainWindow::zh_restoreSettings()
{

}
//============================================================
/*!
 \brief

*/
void MainWindow::zh_saveSettings()
{

}
//============================================================
/*!
 \brief

*/
void MainWindow::zh_onAboutAction()
{

}
//============================================================
/*!
 \brief

*/
void MainWindow::zh_onHelpAction()
{

}
//============================================================
