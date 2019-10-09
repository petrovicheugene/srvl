//==============================================
#include "ZHelpBrowser.h"
#include <QApplication>
#include <QTextBrowser>
#include <QVBoxLayout>
#include <QAction>
#include <QToolBar>
#include <QUrl>
#include <QSettings>
//==============================================
ZHelpBrowser* ZHelpBrowser::zv_this = 0;
int ZHelpBrowser::zv_refCount = 0;

//==============================================
ZHelpBrowser::ZHelpBrowser(const QStringList& pathList, const QUrl& doc, QWidget *parent ) :
    QDialog(parent)
{
    QString winTitle = QString("%1 - %2").arg(qApp->applicationDisplayName(),
                                              tr("Help"));
    setWindowTitle(winTitle);

    zp_createComponents(pathList, doc);
    zp_createConnections();
    zp_dialogControl();
}
//==============================================
void ZHelpBrowser::zp_createComponents(const QStringList& pathList, const QUrl& doc)
{
    QVBoxLayout* mainLayout = new QVBoxLayout;
    setLayout(mainLayout);

    zv_homeAction = new QAction(this);
    zv_homeAction->setIcon(QIcon(":/images/ZImages/blue_home.ico"));

    zv_backwardAction = new QAction(this);
    zv_backwardAction->setIcon(QIcon(":/images/ZImages/blue_left.ico"));

    zv_forwardAction = new QAction(this);
    zv_forwardAction->setIcon(QIcon(":/images/ZImages/blue_right.ico"));

    QToolBar* toolBar = new QToolBar(this);
    toolBar->setIconSize(QSize(32, 32));

    toolBar->addAction(zv_homeAction);
    toolBar->addSeparator();
    toolBar->addAction(zv_backwardAction);
    toolBar->addAction(zv_forwardAction);
    mainLayout->addWidget(toolBar);

    zv_browser = new QTextBrowser(this);
    zv_browser->setOpenExternalLinks(true);
    zv_browser->setOpenLinks(true);
    zv_browser->setSearchPaths(pathList);
    zv_browser->setSource(doc);

    mainLayout->addWidget(zv_browser);
}
//==============================================
void ZHelpBrowser::zp_createConnections()
{
    //connect(mv_browser, SIGNAL((bool)), mv_homeAction, SLOT(setEnabled(bool)));
    connect(zv_browser, SIGNAL(backwardAvailable(bool)), zv_backwardAction, SLOT(setEnabled(bool)));
    connect(zv_browser, SIGNAL(forwardAvailable(bool)), zv_forwardAction, SLOT(setEnabled(bool)));

    connect(zv_homeAction, SIGNAL(triggered()), zv_browser, SLOT(home()));
    connect(zv_backwardAction, SIGNAL(triggered()), zv_browser, SLOT(backward()));
    connect(zv_forwardAction, SIGNAL(triggered()), zv_browser, SLOT(forward()));
}
//==============================================
ZHelpBrowser::~ZHelpBrowser()
{

}
//==============================================
void ZHelpBrowser::closeEvent(QCloseEvent *e)
{
    zv_geometry = saveGeometry();
    QDialog::closeEvent(e);
}
//==============================================
void ZHelpBrowser::zp_restoreGeometry()
{
    if(!zv_geometry.isEmpty())
    {
        restoreGeometry(zv_geometry);
    }
}
//==============================================
void ZHelpBrowser::zp_dialogControl()
{
    zv_backwardAction->setEnabled(zv_browser->isBackwardAvailable());
    zv_forwardAction->setEnabled(zv_browser->isForwardAvailable());
}
//==============================================
ZHelpBrowser* ZHelpBrowser::zp_instance(const QStringList &pathList, const QUrl &doc, QWidget *parent)
{
    if(! zv_this)
    {
        zv_this = new ZHelpBrowser(pathList, doc, parent);
        QSettings settings;
        settings.beginGroup(qApp->applicationVersion());

        QVariant vData = settings.value("help geometry");

        settings.endGroup();

        if(vData.isValid() && !vData.isNull() && vData.canConvert<QByteArray>()  )
        {
            zv_this->restoreGeometry(vData.toByteArray());
        }
    }
    zv_refCount++;
    return  zv_this;
}
//==============================================
void ZHelpBrowser::zp_freeInstance()
{
    zv_refCount--;
    if(zv_refCount < 1)
    {
        QSettings settings;
        settings.beginGroup(qApp->applicationVersion());
        settings.setValue("help geometry", saveGeometry());
        settings.endGroup();

        delete this;
        zv_this = nullptr;
    }
}
//==============================================
