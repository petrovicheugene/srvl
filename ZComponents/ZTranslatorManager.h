//================================================
#ifndef ZLANGUAGEMANAGER_H
#define ZLANGUAGEMANAGER_H
//================================================
#include <QObject>

#include <QApplication>
#include <QMap>
#include <QSettings>
//================================================
class ZTranslatorManager : public QObject
{
    Q_OBJECT
public:
    explicit ZTranslatorManager(QObject* parent = nullptr,
            QString  resoucesTranslatorDirName = ":translators",
                              QString internalTranslatorDir = qApp->applicationDirPath(),
                              QString prefix = "_");

    bool zp_installTranslatorsToApplication();
    QString zp_lastError() const;

    QString zp_resourcesTranslatorDirName() const;
    QString internalTranslatorDir() const;
    QString prefix() const;


public slots:

    void zp_availableLanguageNameList(QStringList& languageList) const;
    void zp_setApplicationLanguage(const QString& languageName, bool *ok = nullptr);
    void zp_currentLanguageName(QString& languageName, bool *ok = nullptr);
    void zp_currentLocaleName(QString& localeName, bool *ok = nullptr);

private:
    // VARS
    QList<QLocale> zv_availableLocaleList;
    const QString zv_appLanguageSettingsName = QString("appLanguage");
    QString zv_resoucesTranslatorDirName;
    QString zv_internalTranslatorDir;
    QString zv_prefix;
    QString zv_errorMessage;

    // FUNCS
    void zh_createAvailableLocaleList();
    bool zh_nativeLanguageNameForTranslatorFile(const QString& translatorFileName,
                                                QString& languageName) const;
    void zh_getApplicationLocale(QLocale &locale);
    bool zh_getApplicationLocaleFromSettings(QLocale &locale);

    void zh_openSettings(QSettings& settings) const;
    void zh_closeSettings(QSettings& settings) const;
    void zh_setErrorMessage(const QString &msg);
    QString zh_localeNameForTranslator(const QString& translatorFileName) const;

};
//================================================
#endif // ZLANGUAGEMANAGER_H
