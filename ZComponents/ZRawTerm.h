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
    ZRawTerm(ZAbstractTerm::TermType termType = ZAbstractTerm::TT_NOT_DEFINED);
    virtual ~ZRawTerm() {};
    // VARS
    QString name;
    ZAbstractTerm::TermType termType;
    ZAbstractTerm::TermState termState;
    QStringList windowList;
    QString customString;
    QString factor;

    qint64 termId;
    QString descriptionString;
};

class ZRawCustomTerm : public ZRawTerm
{
public:
    ZRawCustomTerm(ZAbstractTerm::TermType termType = ZAbstractTerm::TT_NOT_DEFINED);
    virtual ~ZRawCustomTerm() {};
    // VARS
//    qint64 termId;
//    QString descriptionString;
};

//================================================
#endif // ZRAWTERM_H
