//===========================================================
#ifndef ZCONSTANTS_H
#define ZCONSTANTS_H
//===========================================================
#include <QString>
#include <QIcon>
#define DBG
#ifdef DBG
#include <QDebug>
#endif

//==========================================================

extern const QString glAppExeBaseName; /*!< Exe file name without version */
extern const QString glAppProduct; /*!< Application representation name that appears in window title*/
extern const QString glAppVersion; /*!< Application version like 1.2.3.b or 1.2.3 if release version*/
extern const QString glAppCompany; /*!< Application company name like TechnoAnalyt Ltd.*/
extern const QString glAppCopyright; /*!< Copyright string */
extern const QString glAppCompanyURL; /*!< Company URL */

extern const QString glExitAppIconString; /*!< TODO: describe */
extern const QString glAboutIconString; /*!< TODO: describe */
extern const QString glHelpIconString; /*!< TODO: describe */

// Common strings
extern const QString glDefaultDBSuffixString;
extern const QString glErrorString; /*!< TODO: describe */
extern const QString glWarningString; /*!< TODO: describe */
extern const QString glButtonOkString; /*!< TODO: describe */
extern const QString glButtonCancelString; /*!< TODO: describe */
//===========================================================
#endif // ZCONSTANTS_H
