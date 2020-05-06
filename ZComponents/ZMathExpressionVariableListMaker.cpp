//===================================================
#include "ZMathExpressionVariableListMaker.h"
#include <QRandomGenerator>
//===================================================
ZMathExpressionVariableListMaker::ZMathExpressionVariableListMaker(QObject* parent)
    : QObject(parent)
{}
//===================================================
QStringList ZMathExpressionVariableListMaker::zp_variableList() const
{
    return zv_variableNameMap.keys();
}
//===================================================
void ZMathExpressionVariableListMaker::zp_error(QString& errorString,
                                                int& errorTokenStartPosition,
                                                int& errorTokenEndPosition) const
{
    errorString = zv_errorString;
    errorTokenStartPosition = zv_errorTokenStartPosition;
    errorTokenEndPosition = zv_errorTokenEndPosition;
}
//===================================================
void ZMathExpressionVariableListMaker::zp_clear()
{
    zv_variableNameMap.clear();
    zv_errorString.clear();
    zv_errorTokenStartPosition = -1;
    zv_errorTokenEndPosition = -1;
}
//===================================================
void ZMathExpressionVariableListMaker::zp_insertVariableValue(const QString& varName,
                                                              double& value,
                                                              bool& bRes)
{
    emit zs_variableCheckRequest(varName, bRes);
    if (!bRes )
    {
        return;
    }

    if (!zv_variableNameMap.keys().contains(varName))
    {
        zv_variableNameMap.insert(varName, static_cast<qreal>(QRandomGenerator::global()->generate()));
    }

    value = zv_variableNameMap.value(varName);
}
//===================================================
void ZMathExpressionVariableListMaker::zp_setError(const QString& errorString,
                                                   int errorTokenStartPosition,
                                                   int errorTokenEndPosition)
{
    zv_errorString = errorString;
    zv_errorTokenStartPosition = errorTokenStartPosition;
    zv_errorTokenEndPosition = errorTokenEndPosition;
}
//===================================================
