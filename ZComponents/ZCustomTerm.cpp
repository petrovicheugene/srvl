//===================================================================
#include "ZCustomTerm.h"
#include "ZCalibrationWindow.h"
#include "ZMathExpressionVariableListMaker.h"

//===================================================================
ZCustomTerm::ZCustomTerm(ZCalibration* parent) : ZAbstractTerm(parent)
{
    zv_type = TT_CUSTOM;
    zh_createConnections();
}
//===================================================================
void ZCustomTerm::zh_createConnections()
{
    connect(&zv_mathExpressionHandler,
            &ZMathExpressionHandler::zs_requestVariableValue,
            this,
            &ZCustomTerm::zh_handleWindowIntensityRequest);
    connect(&zv_mathExpressionHandler,
            &ZMathExpressionHandler::zs_errorReport,
            this,
            &ZCustomTerm::zh_handleErrorReport);
}
//===================================================================
void ZCustomTerm::zh_handleWindowIntensityRequest(const QString& windowName,
                                                  double& value,
                                                  bool& bRes)
{
    emit zs_calcWindowIntensity(windowName, (const QObject*) zv_spectrum, value, true, &bRes);
}
//===================================================================
void ZCustomTerm::zh_handleErrorReport(const QString& errorString,
                                       int errorTokenStartPosition,
                                       int errorTokenEndPosition) const
{
    emit zs_errorReport(QString("%1 (%2)!").arg(errorString, QString::number(errorTokenStartPosition)));

}
//===================================================================
bool ZCustomTerm::zp_calcValue(const ZAbstractSpectrum* spectrum, qreal& value)
{
    value = 0.0;
    if(zv_termState != TS_CONST_INCLUDED && zv_termState != TS_INCLUDED)
    {
        return true;
    }

    zv_spectrum = spectrum;
    bool res = zv_mathExpressionHandler.zp_calculateExpression(zv_expression, value);
    zv_spectrum = nullptr;
    value *= zv_termFactor;

    return res;
}
//===================================================================
bool ZCustomTerm::zp_calcTermVariablePart(const ZAbstractSpectrum* spectrum,
                                          qreal& value) // w/o factor
{
    zv_spectrum = spectrum;
    bool res = zv_mathExpressionHandler.zp_calculateExpression(zv_expression, value);
    zv_spectrum = nullptr;

    return res;
}
//===================================================================
bool ZCustomTerm::zp_termBelongsToWindow(const ZCalibrationWindow* window) const
{
    return zv_windowList.contains(window->zp_windowName());
}
//===================================================================
QList<qint64> ZCustomTerm::zp_termWindowIdList() const
{
    QList<qint64> windowIdList;
    return windowIdList;
}
//===================================================================
void ZCustomTerm::zh_updateTermNameForWindowName()
{
    if (!sender())
    {
        return;
    }
    //    else if(sender() != zv_window1 && sender() != zv_window2)
    //    {
    //        return;
    //    }

    //    zh_setName();
    //emit zg_termNameChanged();
}
//===================================================================
void ZCustomTerm::zh_onWindowTypeChange(ZCalibrationWindow::WindowType previousType,
                                        ZCalibrationWindow::WindowType currentType)
{
    //    if(!sender() || (sender() != zv_window1 && sender() != zv_window2))
    //    {
    //        return;
    //    }

    //    if(currentType != ZCalibrationWindow::WT_PEAK)
    //    {
    //        emit zg_requestForDelete(this);
    //    }
}
//===================================================================
QString ZCustomTerm::zp_expression() const
{
    return zv_expression;
}
//===================================================================
QString ZCustomTerm::zp_description() const
{
    return zv_description;
}
//===================================================================
void ZCustomTerm::zp_setName(const QString& name)
{
    zv_name = name;
    emit zg_termNameChanged();
}
//===================================================================
bool ZCustomTerm::zp_setExpression(const QString& expression)
{
    zv_expression = expression;
    return zh_checkExpression(zv_expression);
}
//===================================================================
void ZCustomTerm::zp_setDescription(const QString& description)
{
    zv_description = description;
}
//===================================================================
bool ZCustomTerm::zh_checkExpression(const QString& expression)
{
    ZMathExpressionHandler mathExpressionHandler;
    ZMathExpressionVariableListMaker mathExpressionVariableListMaker;
    connect(&mathExpressionHandler,
            &ZMathExpressionHandler::zs_requestVariableValue,
            &mathExpressionVariableListMaker,
            &ZMathExpressionVariableListMaker::zp_insertVariableValue);
    connect(&mathExpressionHandler,
            &ZMathExpressionHandler::zs_errorReport,
            &mathExpressionVariableListMaker,
            &ZMathExpressionVariableListMaker::zp_setError);
    connect(&mathExpressionVariableListMaker,
            &ZMathExpressionVariableListMaker::zs_variableCheckRequest,
            this,
            &ZCustomTerm::zg_windowIsExist);

    double res;
    if (!mathExpressionHandler.zp_calculateExpression(expression, res))
    {
        QString error;
        int errorStartPos;
        int errorEndPos;
        mathExpressionVariableListMaker.zp_error(error, errorStartPos, errorEndPos);
        QString msg = tr("Error in the term \"%1\" expression: %2").arg(zv_name, error);
        emit zs_errorReport(msg);
        return false;
    }
    zv_windowList = mathExpressionVariableListMaker.zp_variableList();
    return true;
}
//===================================================================

