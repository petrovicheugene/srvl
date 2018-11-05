//==============================================
#ifndef ZHELPBROWSER_H
#define ZHELPBROWSER_H
//==============================================
#include <QDialog>
//==============================================
class QTextBrowser;
class QPushButton;
class QAction;
//==============================================
class ZHelpBrowser : public QDialog
{
    Q_OBJECT
public:

    void zp_restoreGeometry();
    static ZHelpBrowser* zp_instance(const QStringList& pathList, const QUrl& doc, QWidget *parent);
    void zp_freeInstance();

private:

    // VARS
    QAction* zv_homeAction;
    QAction* zv_backwardAction;
    QAction* zv_forwardAction;

    QTextBrowser* zv_browser;

    QByteArray zv_geometry;

    static ZHelpBrowser* zv_this;
    static int zv_refCount;

    // FUNCS
    explicit ZHelpBrowser(const QStringList& pathList, const QUrl& doc, QWidget *parent  = 0);
    virtual ~ZHelpBrowser();

    void closeEvent(QCloseEvent *e);
    void zp_createComponents(const QStringList& pathList, const QUrl& doc);
    void zp_createConnections();
    void zp_dialogControl();

signals:

public slots:

};
//==============================================
#endif // ZHELPBROWSER_H
