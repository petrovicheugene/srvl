//================================================
#ifndef ZRAWTERM_H
#define ZRAWTERM_H
//================================================
#include "ZAbstractTerm.h"
#include <QString>
//================================================
/*!
 \brief

*/
class ZRawTerm
{
public:
    /*!
     \brief

    */
    ZRawTerm();
    // VARS
    QString name; /*!< TODO: describe */
    ZAbstractTerm::TermType termType; /*!< TODO: describe */
    ZAbstractTerm::TermState termState; /*!< TODO: describe */
    QStringList windowList; /*!< TODO: describe */
    QString customString; /*!< TODO: describe */
    QString factor; /*!< TODO: describe */
};
//================================================
#endif // ZRAWTERM_H
