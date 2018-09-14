//===============================================================
#include "MainWindow.h"
#include "ZStartDialog.h"
#include "ZGeneral.h"

#include <QApplication>
#include <QTextCodec>
#include <QSplashScreen>
#include <QTranslator>
#include <QDir>
//===============================================================
extern const QString glAppExeBaseName = APP_EXE_BASE_NAME;
extern const QString glAppProduct = APP_PRODUCT;
extern const QString glAppVersion = APP_VERSION;
extern const QString glAppCompany = APP_COMPANY;
extern const QString glAppCopyright = APP_COPYRIGHT;
extern const QString glAppCompanyURL = APP_COMPANY_URL;

//// Icons
//extern const QString glSettingsIconString = "";
//extern const QString glExitAppIconString = "";
//extern const QString glAboutIconString = "";
//extern const QString glHelpIconString = "";

//// Common non translatable strings
//extern const QString glDefaultDBSuffixString = "sqlite";
//extern const QString glDefaultChemElementString = "Not defined";
//// object Names
//extern const QString glMenuFileObjectNameString = "File";
//extern const QString glMenuEditObjectNameString = "Edit";
//extern const QString glMenuViewObjectNameString = "View";
//extern const QString glMenuActionsObjectNameString = "Actions";
//extern const QString glMenuHelpObjectNameString = "Help";

//// Common strings
//extern const QString glSettingsString = QObject::tr("Settings");
//extern const QString glSettingsToolTipString = QObject::tr("Run settings of the application");
//extern const QString glExitString = QObject::tr("Exit");
//extern const QString glExitToolTipString = QObject::tr("Exit the application");
//extern const QString glAboutString = QObject::tr("About");
//extern const QString glAboutToolTipString = QObject::tr("About the application");
//extern const QString glHelpString = QObject::tr("Help");
//extern const QString glHelpToolTipString = QObject::tr("Show user guide");

//extern const QString NS_NS_CommonStrings::glError = QObject::tr("Error");
//extern const QString NS_CommonStrings::glWarning = QObject::tr("Warning");
//extern const QString glCommonString = QObject::tr("Common");

//// Menu strings
//extern const QString glMenuFileString = QObject::tr("File");
//extern const QString glMenuEditString = QObject::tr("Edit");
//extern const QString glMenuViewString = QObject::tr("View");
//extern const QString glMenuActionsString = QObject::tr("Actions");
//extern const QString glMenuHelpString = QObject::tr("Help");

//// button strings
//extern const QString glButtonOkString = QObject::tr("OK");
//extern const QString glButtonApplyString = QObject::tr("Apply");
//extern const QString glButtonCancelString = QObject::tr("Cancel");
//extern const QString glButtonBrowseString = QObject::tr("Browse");

//// window captions
//extern const QString glOpenFileString = QObject::tr("Open file");

//===============================================================
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QPixmap pixmap(":/images/ZImages/Chemistry.png");
    QSplashScreen splash(pixmap);
    splash.show();
    splash.showMessage("Loading codecs...", Qt::AlignBottom | Qt::AlignRight, Qt::white );
    a.processEvents();

    QTextCodec* codec = QTextCodec::codecForName("windows-1251");
    QTextCodec::setCodecForLocale(codec);

    QApplication::setOrganizationName(glAppCompany);
    QApplication::setApplicationName(glAppExeBaseName);
    QApplication::setApplicationVersion(glAppVersion);

    // create qApp properties and set .pro defines into them
#ifdef APP_EXE_BASE_NAME
    qApp->setProperty("glAppExeBaseName", QString(APP_EXE_BASE_NAME));
#endif

#ifdef APP_PRODUCT
    qApp->setProperty("glAppProduct", QString(APP_PRODUCT));
#endif

#ifdef APP_VERSION
    qApp->setProperty("glAppVersion", QString(APP_VERSION));
#endif

#ifdef APP_COMPANY
    qApp->setProperty("glAppCompany", QString(APP_COMPANY));
#endif

#ifdef APP_COPYRIGHT
    qApp->setProperty("glAppCopyright", QString(APP_COPYRIGHT));
#endif

#ifdef APP_COMPANY_URL
    qApp->setProperty("glAppCompanyURL", QString(APP_COMPANY_URL));
#endif

#ifdef APP_ICON
    qApp->setProperty("glAppIcon", QString(APP_ICON));
#endif

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
                "QSplitter::handle:vertical {height: 6px; image: url(:/images/ZImages/vSplitterHandle.png);}"
                "QSplitter::handle:horizontal {width:  6px; image: url(:/images/ZImages/hSplitterHandle.png);}"
                );

    splash.showMessage("Loading modules...", Qt::AlignBottom | Qt::AlignRight, Qt::white );
    a.processEvents();

    // set path for sqlite plugin
    QStringList paths = QCoreApplication::libraryPaths();
    paths.append(".");
    paths.append("platforms");
    paths.append("sqldrivers");
    QCoreApplication::setLibraryPaths(paths);

    // start
    ZStartDialog* dialog = new ZStartDialog();
    splash.finish(dialog);

    if(dialog->exec() == QDialog::Rejected)
    {
        delete dialog;
        return 0;
    }

    //  pixmap = QPixmap(":/images/ZImages/Chemistry.png");
    //  QSplashScreen dbSplash(pixmap);
    //  dbSplash.show();
    //  dbSplash.showMessage("Connecting to database...", Qt::AlignBottom | Qt::AlignRight, Qt::white );
    //  a.processEvents();

    QString dbName;
    QString dbPath;
    dialog->zp_databaseProperties(dbName, dbPath);
    delete dialog;

    // main window
    MainWindow w(dbName, dbPath);

    // transfer db to main window
    if(!w.zp_isDatabaseOpen())
    {
        return 0;
    }

    w.show();
    // dbSplash.finish(&w);

    return a.exec();
}
//===============================================================
