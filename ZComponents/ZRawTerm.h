//================================================
#ifndef ZRAWTERM_H
#define ZRAWTERM_H
//================================================
#include "ZAbstractTerm.h"
#include <QString>
//================================================
class ZRawTerm
{
public:
    ZRawTerm();
    // VARS
    QString name;
    ZAbstractTerm::TermType termType;
    ZAbstractTerm::TermState termState;
    QStringList windowList;
    QString customString;
    QString factor;
};
//================================================
#endif // ZRAWTERM_H
