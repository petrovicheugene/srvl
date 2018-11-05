//===================================================================
#ifndef ZCUSTOMTERM_H
#define ZCUSTOMTERM_H
//===================================================================
#include "ZAbstractTerm.h"
//===================================================================
class ZCustomTerm : public ZAbstractTerm
{
   Q_OBJECT
public:
   explicit ZCustomTerm(ZCalibration* parent);

   // FUNCS
    bool zp_calcValue(const ZAbstractSpectrum*, qreal&) override;
    bool zp_calcTermVariablePart(const ZAbstractSpectrum*, qreal& value) override; // w/o factor
    bool zp_termBelongsToWindow(const ZCalibrationWindow*) const override;
    QList<qint64> zp_termWindowIdList() const;


};
//===================================================================
#endif // ZCUSTOMTERM_H
