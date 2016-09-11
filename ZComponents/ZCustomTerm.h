//===================================================================
#ifndef ZCUSTOMTERM_H
#define ZCUSTOMTERM_H
//===================================================================
#include "ZAbstractTerm.h"
//===================================================================
/*!
 \brief

*/
class ZCustomTerm : public ZAbstractTerm
{
   Q_OBJECT
public:
   /*!
    \brief

    \param parent
   */
   explicit ZCustomTerm(ZCalibration* parent);

   // FUNCS
    /*!
     \brief

     \param
     \param
     \return bool
    */
    bool zp_calcValue(const ZAbstractSpectrum*, qreal&) override;
    /*!
     \brief

     \param
     \param value
     \return bool
    */
    bool zp_calcTermVariablePart(const ZAbstractSpectrum*, qreal& value) override; // w/o factor
    /*!
     \brief

     \param
     \return bool
    */
    bool zp_termBelongsToWindow(const ZCalibrationWindow*) const override;
    /*!
     \brief

     \return QList<qint64>
    */
    QList<qint64> zp_termWindowIdList() const;


};
//===================================================================
#endif // ZCUSTOMTERM_H
