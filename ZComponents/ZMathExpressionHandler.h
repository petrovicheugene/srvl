//===================================================
#ifndef ZMATHEXPRESSIONHANDLER_H
#define ZMATHEXPRESSIONHANDLER_H
//===================================================
#include <QChar>
#include <QObject>
#include <QPair>
#include <QSet>
#include <QVector>
//===================================================
class ZMathExpressionHandler : public QObject
{
    Q_OBJECT
public:
    explicit ZMathExpressionHandler(QObject* parent = 0);


signals:

    void zs_requestVariableValue(const QString& varName, double& value, bool& bRes) const;
    void zs_errorReport(const QString& errorString,
                        int errorTokenStartPosition,
                        int errorTokenEndPosition) const;

public slots:

    void zp_defineHighLights(const QString& expression,
                             int cursorPos,
                             QVector<QPair<int, bool>>& highlightedSymbolVector);

    bool zp_checkExpression(const QString& expression,
                            int cursorPos,
                            QVector<QPair<int, bool>>& highlightedSymbolVector);
    bool zp_calculateExpression(const QString& expression, double& result);

protected slots:


protected:
    // VARS

    enum TokenType
    {
        TT_NULL,
        TT_ERROR,
        TT_DOUBLE,
        TT_VARIABLE,
        TT_DELIMITER,
        TT_OPERATOR,
        TT_INTEGER,
        TT_CELL
    } zv_tokenType;

    QVector<QPair<QChar, QChar>> zv_pairSymbolVector;
    QSet<QChar> zv_delimiterSet;
    QSet<QString> zv_operatorSet;
    QSet<QString> zv_extraSymbolSet;

    int zv_charPosition;       // parsing pos in expression
    int zv_tokenStartPosition; // for error report
    int zv_tokenEndPosition;   //  for error report

    QString zv_token;
    QString zv_expression;


    // FUNCS

    void zh_findBracketForward(QChar bracketSymbol,
                               const QString& expression,
                               int pos,
                               QPair<int, bool>& highlight);
    void zh_findBracketBackward(QChar bracketSymbol,
                                const QString& expression,
                                int pos,
                                QPair<int, bool>& highlight);

    bool zh_getToken();
    bool zh_addOrSubtract(double& result);
    bool zh_multiplyOrDevide(double& result);
    bool zh_power(double& result);
    bool zh_unaryOperation(double& result);
    bool zh_recognizeBrackets(double& result);
    bool zh_variableValue(double& result);
    void zh_handleError(int error);
};
//===================================================
#endif // ZMATHEXPRESSIONHANDLER_H
