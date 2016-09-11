//===================================================================
#ifndef ZSIMPLETERM_H
#define ZSIMPLETERM_H
//===================================================================
#include "ZAbstractTerm.h"
#include "ZCalibrationWindow.h"
//===================================================================

//===================================================================
/*!
 \brief

*/
class ZSimpleTerm : public ZAbstractTerm
{
    Q_OBJECT
public:
    /*!
     \brief

     \param window
     \param parent
    */
    explicit ZSimpleTerm(const ZCalibrationWindow* window,
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
     \param useBuffer
     \param ok
    */
    void zg_requestWindowIntensity(const QObject* spectrum, qreal& intensityValue, bool useBuffer,  bool* ok = 0);

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
    void zh_onWindowTypeChange(ZCalibrationWindow::WindowType previousType,
                               ZCalibrationWindow::WindowType currentType) override;
private:

    // VARS
    const ZCalibrationWindow* zv_window; /*!< TODO: describe */


    // FUNCS

    /*!
     \brief

     \param window
    */
    void zh_connectToWindow(const ZCalibrationWindow* window);

};
//===================================================================
#endif // ZSIMPLETERM_H
