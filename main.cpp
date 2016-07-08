//===============================================================
#include "MainWindow.h"
#include "ZStartDialog.h"
#include "ZConstants.h"

#include <QApplication>
#include <QTextCodec>
#include <QSplashScreen>
#include <QTranslator>
#include <QDir>
//===============================================================
extern const QString glAppExeBaseName =  APP_EXE_BASE_NAME;
extern const QString glAppProduct =  APP_PRODUCT;
extern const QString glAppVersion = APP_VERSION;
extern const QString glAppCompany = APP_COMPANY;
extern const QString glAppCopyright = APP_COPYRIGHT;
extern const QString glAppCompanyURL = APP_COMPANY_URL;

// Icons
extern const QString glExitAppIconString = "";
extern const QString glAboutIconString = "";
extern const QString glHelpIconString = "";

// Common strings
extern const QString glDefaultDBSuffixString = "sqlite";

extern const QString glErrorString = QObject::tr("Error");
extern const QString glWarningString = QObject::tr("Warning");
extern const QString glButtonOkString = QObject::tr("OK");
extern const QString glButtonCancelString = QObject::tr("Cancel");
//===============================================================
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QPixmap pixmap(":/images/ZImages/chemUtensil-1.jpg");
    QSplashScreen splash(pixmap);
    splash.show();
    splash.showMessage("Loading codecs...", Qt::AlignBottom | Qt::AlignRight, Qt::white );
    a.processEvents();

    QTextCodec* codec = QTextCodec::codecForName("windows-1251");
    QTextCodec::setCodecForLocale(codec);

    QApplication::setOrganizationName(glAppCompany);
    QApplication::setApplicationName(glAppExeBaseName);
    QApplication::setApplicationVersion(glAppVersion);

    splash.showMessage("Loading translations...", Qt::AlignBottom | Qt::AlignRight, Qt::white );
    a.processEvents();

    QTranslator appTranslator;
    QDir appDir(QApplication::applicationDirPath());

    bool res = appTranslator.load(appDir.absoluteFilePath(QString("%1_%2").arg(glAppExeBaseName, QLocale::system().name())));
    if(!res)
    {
        res = appTranslator.load(QString(":/trans/%1_%2").arg(glAppExeBaseName, QLocale::system().name()));
    }

    if(res)
    {
        a.installTranslator(&appTranslator);
    }

    QTranslator qtTranslator;
    res = qtTranslator.load(appDir.absoluteFilePath(QString("%1_%2").arg("qtbase", QLocale::system().name())));

    if(!res)
    {
        res = qtTranslator.load(QString(":/trans/qtbase_%2").arg(QLocale::system().name()));
    }

    if(res)
    {
        a.installTranslator(&qtTranslator);
    }

    splash.showMessage("Loading styles...", Qt::AlignBottom | Qt::AlignRight, Qt::white );
    a.processEvents();

    qApp->setStyleSheet(
                "QSplitter::handle:vertical   {height: 6px; image: url(:/images/ZImages/vSplitterHandle.png);}"
                "QSplitter::handle:horizontal {width:  6px; image: url(:/images/ZImages/hSplitterHandle.png);}"
                );

    splash.showMessage("Loading modules...", Qt::AlignBottom | Qt::AlignRight, Qt::white );
    a.processEvents();

    // start
    ZStartDialog* dialog = new ZStartDialog();
    splash.hide();

    if(dialog->exec() == QDialog::Rejected)
    {
        delete dialog;
        return 0;
    }

    splash.show();
    splash.showMessage("Connecting to database...", Qt::AlignBottom | Qt::AlignRight, Qt::white );
    a.processEvents();

    // main window
    MainWindow w;

    // transfer db to main window

    delete dialog;

    w.show();
    splash.finish(&w);

    return a.exec();
}
//===============================================================
