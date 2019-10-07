//================================================
#include "ZTranslatorManager.h"

#include <QAction>
#include <QDebug>
#include <QDir>
#include <QFileInfo>
#include <QRegExp>
#include <QTextCodec>
#include <QTranslator>
//================================================
ZTranslatorManager::ZTranslatorManager(QObject* parent,
                                       QString resoucesTranslatorDirName,
                                       QString internalTranslatorDir,
                                       QString prefix)
    : QObject(parent)
{
    zv_resoucesTranslatorDirName = resoucesTranslatorDirName;
    zv_internalTranslatorDir = internalTranslatorDir;
    zv_prefix = prefix;
    zh_createAvailableLocaleList();
}
//================================================
bool ZTranslatorManager::zp_installTranslatorsToApplication()
{
    zv_errorMessage.clear();

    if(!qApp )
    {
        zh_setErrorMessage(tr("Cannot install application translator. "
                              "The pointer of the application is invalid!"));
        return false;
    }

    // get language name
    QLocale appLocale;
    zh_getApplicationLocale(appLocale);

    QTranslator* appTranslator = new QTranslator(qApp);
    QDir appDir(QApplication::applicationDirPath());

    bool res = appTranslator->load(appLocale, qApp->applicationName(), zv_prefix, zv_internalTranslatorDir);
    if(!res)
    {
        res = appTranslator->load(appLocale, qApp->applicationName(), zv_prefix, zv_resoucesTranslatorDirName);
    }

    if(res)
    {
        res = qApp->installTranslator(appTranslator);
    }

    if(!res)
    {
        zh_setErrorMessage(tr("Cannot install application translator. Unknown error."));
        delete appTranslator;
        return false;
    }

    QTranslator* qtTranslator = new QTranslator(qApp);;

    res = qtTranslator->load(appLocale, "qtbase", zv_prefix, zv_internalTranslatorDir);
    if(!res)
    {
        res = qtTranslator->load(appLocale, "qtbase", zv_prefix, zv_resoucesTranslatorDirName);
    }

    if(res)
    {
        res = qApp->installTranslator(qtTranslator);
    }

    if(!res)
    {
        zh_setErrorMessage(tr("Cannot install Qt translator. Unknown error."));
        delete qtTranslator;
        return false;
    }

    return true;
}
//================================================
QString ZTranslatorManager::zp_resourcesTranslatorDirName() const
{
    return zv_resoucesTranslatorDirName;
}
//================================================
QString ZTranslatorManager::internalTranslatorDir() const
{
    return zv_internalTranslatorDir;
}
//================================================
QString ZTranslatorManager::prefix() const
{
    return zv_prefix;
}
//================================================
QString ZTranslatorManager::zp_lastError() const
{
    return zv_errorMessage;
}
//================================================
void ZTranslatorManager::zh_createAvailableLocaleList()
{
    // application base name
    QString appBaseName = qApp->applicationName().isEmpty()?
                "*" : qApp-> applicationName();

    // get all qm file list
    QDir dir(qApp->applicationDirPath());
    QStringList translatorFileNameStringList;
    translatorFileNameStringList = dir.entryList(QStringList(QString("%1.qm").arg(appBaseName+"*")));

    QDir resDir(zv_resoucesTranslatorDirName);
    QStringList resFiles;
    translatorFileNameStringList.append(resDir.entryList());

    QString localeName;
    foreach(QString translatorFileName, translatorFileNameStringList)
    {
        QLocale locale(QLocale::c());
        localeName = zh_localeNameForTranslator(translatorFileName);
        locale = QLocale(localeName);

        if(locale == QLocale::c())
        {
            continue;
        }

        if(!zv_availableLocaleList.contains(locale))
        {
            zv_availableLocaleList.append(locale);
        }
    }
}
//================================================
bool ZTranslatorManager::zh_nativeLanguageNameForTranslatorFile(const QString& translatorFileName,
                                                                QString& languageName) const
{
    QString localeName = zh_localeNameForTranslator(translatorFileName);
    if(localeName.isEmpty())
    {
        return false;
    }

    languageName = QLocale(localeName).nativeLanguageName();
    if(languageName.isEmpty())
    {
        return false;
    }

    languageName[0] = languageName[0].toUpper();
    return true;
}
//================================================
void ZTranslatorManager::zp_availableLanguageNameList(QStringList& languageList) const
{
    QString languageName;
    foreach(QLocale locale, zv_availableLocaleList)
    {
        languageName = locale.nativeLanguageName();
        languageName[0] = languageName[0].toUpper();
        languageList.append(languageName);
    }

    std::sort(languageList.begin(), languageList.end());
}
//================================================
void ZTranslatorManager::zp_setApplicationLanguage (const QString& languageName,  bool* ok)
{
    foreach(QLocale locale, zv_availableLocaleList)
    {
        if(locale.nativeLanguageName().compare(languageName, Qt::CaseInsensitive) != 0)
        {
            continue;
        }

        QLocale currentLocale;
        zh_getApplicationLocale(currentLocale);

        if(currentLocale == locale)
        {
            // locale for languageName is already set
            if(ok)
            {
                *ok = false;
            }
            return;
        }

        QSettings settings;
        zh_openSettings(settings);
        settings.setValue(zv_appLanguageSettingsName, QVariant(locale.name()));
        zh_closeSettings(settings);
        if(ok)
        {
            *ok = true;
        }
        return;
    }

    zh_setErrorMessage(tr("Cannot set the selected langusge. Translator not found!"));
    if(ok)
    {
        *ok = false;
    }
}
//================================================
void ZTranslatorManager::zp_currentLanguageName(QString& languageName, bool *ok)
{
    QLocale locale;
    zh_getApplicationLocale(locale);

    if(locale == QLocale::c())
    {
        if(ok)
        {
            *ok = false;
        }
        return;
    }

    languageName = locale.nativeLanguageName();
    languageName[0] = languageName[0].toUpper();

    if(ok)
    {
        *ok = true;
    }
}
//================================================
void ZTranslatorManager::zp_currentLocaleName(QString& localeName, bool *ok)
{
    QLocale locale;
    zh_getApplicationLocale(locale);

    if(locale == QLocale::c())
    {
        if(ok)
        {
            *ok = false;
        }
        return;
    }

    localeName = locale.name();

    if(ok)
    {
        *ok = true;
    }
}
//================================================
void ZTranslatorManager::zh_getApplicationLocale(QLocale& locale)
{
    if(!zh_getApplicationLocaleFromSettings(locale))
    {
        locale = QLocale::system();
    }
}
//================================================
bool ZTranslatorManager::zh_getApplicationLocaleFromSettings(QLocale &locale)
{
    QSettings settings;
    zh_openSettings(settings);
    if(!settings.contains(zv_appLanguageSettingsName))
    {
        zh_setErrorMessage(tr("Language for application is not set!"));
        return false;
    }

    QVariant vData = settings.value(zv_appLanguageSettingsName);
    zh_closeSettings(settings);

    if(!vData.isValid() || vData.isNull() || !vData.canConvert<QString>())
    {
        zh_setErrorMessage(tr("Cannot get translator file name from settings!"));
        return false;
    }

    QString localeName = vData.toString();
    locale = QLocale(localeName);

    return true;
}
//================================================
void ZTranslatorManager::zh_openSettings(QSettings &settings) const
{
    QString version = qApp->applicationVersion();
    if(!version.isEmpty())
    {
        settings.beginGroup(version);
    }
}
//================================================
void ZTranslatorManager::zh_closeSettings(QSettings& settings) const
{
    while (!settings.group().isEmpty())
    {
        settings.endGroup();
    }
}
//================================================
void ZTranslatorManager::zh_setErrorMessage(const QString& msg)
{
    zv_errorMessage = msg;
    qWarning().noquote() << zv_errorMessage;
}
//================================================
QString ZTranslatorManager::zh_localeNameForTranslator(const QString& translatorFileName) const
{
    QString localeName = translatorFileName;
    localeName.truncate(localeName.lastIndexOf('.')); // "TranslationExample_de"
    localeName.remove(0, localeName.lastIndexOf(zv_prefix) + 1); // "de"
    return localeName;
}
//================================================
