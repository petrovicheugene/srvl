//================================================
#include "ZRawTerm.h"
//================================================
ZRawTerm::ZRawTerm(ZAbstractTerm::TermType termType)
{
    name = QString();
    this->termType = termType;
    termState = ZAbstractTerm::TS_NOT_DEFINED;
    windowList = QStringList();
    customString = QString();
    factor = 0.0;
}
//================================================
ZRawCustomTerm::ZRawCustomTerm(ZAbstractTerm::TermType termType)
    : ZRawTerm(termType)
{
    termId = -1;
    descriptionString = QString();
}
//================================================
