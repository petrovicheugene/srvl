//===================================================================
#ifndef ZCUSTOMTERM_H
#define ZCUSTOMTERM_H
//===================================================================
#include "ZAbstractTerm.h"
#include "ZMathExpressionHandler.h"
#include <QString>
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
    QList<qint64> zp_termWindowIdList() const override;

    QString zp_expression() const;
    QString zp_description() const;

    void zp_setName(const QString& name);
    bool zp_setExpression(const QString& equation);
    void zp_setDescription(const QString& description);

signals:

    void zs_calcWindowIntensity(
        const QString& windowName, const QObject*, qreal&, bool keepBufferClean, bool* ok);
    void zs_errorReport(const QString& errorMsg) const;
    void zg_windowIsExist(const QString& name, bool& res) const;

protected slots:

    virtual void zh_updateTermNameForWindowName() override;
    virtual void zh_onWindowTypeChange(ZCalibrationWindow::WindowType previousType,
                                       ZCalibrationWindow::WindowType currentType) override;
    void zh_handleWindowIntensityRequest(const QString& varName, double& value, bool& bRes);
    void zh_handleErrorReport(const QString& errorString,
                        int errorTokenStartPosition,
                        int errorTokenEndPosition) const;

private:
    // VARS
    ZMathExpressionHandler zv_mathExpressionHandler;
    QString zv_expression;
    QString zv_description;
    QStringList zv_windowList;
    const ZAbstractSpectrum* zv_spectrum;

    // FUNCS
    void zh_createConnections();
    bool zh_checkExpression(const QString &expression);

};
//===================================================================
#endif // ZCUSTOMTERM_H
