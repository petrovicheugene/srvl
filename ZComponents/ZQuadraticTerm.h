//===================================================================
#ifndef ZQUADRATICTERM_H
#define ZQUADRATICTERM_H
//===================================================================
#include "ZAbstractTerm.h"
#include "ZCalibrationWindow.h"

//===================================================================

//===================================================================
/*!
 \brief

*/
class ZQuadraticTerm : public ZAbstractTerm
{
    Q_OBJECT
public:
    /*!
     \brief

     \param window
     \param parent
    */
    explicit ZQuadraticTerm(const ZCalibrationWindow* window,
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
    /*!
     \brief

     \return const ZCalibrationWindow
    */
    const ZCalibrationWindow* zp_window() const;

signals:

    /*!
     \brief

     \param spectrum
     \param intensityValue
     \param keepBufferClean
     \param ok
    */
    void zg_requestWindowIntensity(const QObject* spectrum, qreal& intensityValue, bool keepBufferClean, bool* ok = 0);

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
    virtual void zh_onWindowTypeChange(ZCalibrationWindow::WindowType previousType, ZCalibrationWindow::WindowType currentType) override;

private:

    // VARS
    const ZCalibrationWindow* zv_window; /*!< TODO: describe */

    // FUNCS
    /*!
     \brief

    */
    void zh_setName();
    /*!
     \brief

    */
    void zh_connectToWindow();

};
//===================================================================
#endif // ZQUADRATICTERM_H
