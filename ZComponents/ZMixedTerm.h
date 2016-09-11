//===================================================================
#ifndef ZCROSSPRODUCTTERM_H
#define ZCROSSPRODUCTTERM_H
//===================================================================
#include "ZAbstractTerm.h"
//===================================================================
class ZCalibrationWindow;
//===================================================================
/*!
 \brief

*/
class ZMixedTerm : public ZAbstractTerm
{
    Q_OBJECT
public:
    /*!
     \brief

     \param window1
     \param window2
     \param parent
    */
    explicit ZMixedTerm(const ZCalibrationWindow *window1,
                        const ZCalibrationWindow *window2,
                        ZCalibration *parent);

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
    bool zp_calcTermVariablePart(const ZAbstractSpectrum*, qreal &value) override; // w/o factor
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

    //QString zp_termName() const override;

signals:

    /*!
     \brief

     \param spectrum
     \param intensityValue
     \param useBuffer
     \param ok
    */
    void zg_requestWindow1Intensity(const QObject* spectrum, qreal& intensityValue, bool useBuffer,  bool* ok = 0);
    /*!
     \brief

     \param spectrum
     \param intensityValue
     \param useBuffer
     \param ok
    */
    void zg_requestWindow2Intensity(const QObject* spectrum, qreal& intensityValue, bool useBuffer,  bool* ok = 0);

protected slots:

    /*!
     \brief

    */
    virtual void zh_updateTermNameForWindowName() override;
    /*!
     \brief

     \param previousType
     \param currentType
    */
    virtual void zh_onWindowTypeChange(ZCalibrationWindow::WindowType previousType,
                                       ZCalibrationWindow::WindowType currentType) override;

private:

    // VARS
    const ZCalibrationWindow* zv_window1; /*!< TODO: describe */
    const ZCalibrationWindow* zv_window2; /*!< TODO: describe */

    // FUNCS
    /*!
     \brief

    */
    void zh_setName();
    /*!
     \brief

    */
    void zh_connectToWindows();

};
//===================================================================
#endif // ZCROSSPRODUCTTERM_H
