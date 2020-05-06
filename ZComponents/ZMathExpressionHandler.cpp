//===================================================
#include "ZMathExpressionHandler.h"
#include <math.h>
#include <QDebug>
#include <QRegExp>
#include <QStack>
//===================================================
ZMathExpressionHandler::ZMathExpressionHandler(QObject* parent) : QObject(parent)
{
    zv_tokenType = TT_NULL;
    zv_pairSymbolVector << QPair<QChar, QChar>('[', ']') << QPair<QChar, QChar>('(', ')')
                        << QPair<QChar, QChar>('{', '}');
    zv_delimiterSet << '(' << ')' << '+' << '-' << '*' << '/' << '^' << '[' << ']' << '\0';
    zv_operatorSet << "ln"
                   << "lg"
                   << "exp";
    zv_extraSymbolSet << "#" << "№" << "@" << "`" << "$";
}
//===================================================
void ZMathExpressionHandler::zh_findBracketForward(QChar bracketSymbol,
                                                   const QString& expression,
                                                   int pos,
                                                   QPair<int, bool>& highlight)
{
    int openBracketCount = 0;
    for (int i = pos + 1; i < expression.count(); i++)
    {
        QChar currentSymbol = expression.at(i);

        if (currentSymbol == bracketSymbol)
        {
            if (openBracketCount == 0)
            {
                highlight.first = i;
                highlight.second = true;
                return;
            }
            else
            {
                openBracketCount--;
                continue;
            }
        }
        else
        {
            bool quitCircle = false;
            for (int s = 0; s < zv_pairSymbolVector.count(); s++)
            {
                if (quitCircle)
                {
                    continue;
                }

                if (currentSymbol == zv_pairSymbolVector.value(s).first) // open bracket, not to find
                {
                    openBracketCount++;
                    quitCircle = true;
                    continue;
                }

                if (currentSymbol == zv_pairSymbolVector.value(s).second)
                {
                    if (openBracketCount < 1)
                    {
                        highlight.first = i;
                        highlight.second = false;
                        return;
                    }
                    else
                    {
                        openBracketCount--;
                        quitCircle = true;
                        continue;
                    }
                }
            }
        }
    } // end of for(int i = pos+1; i < equation.count(); i++)

    highlight.first = expression.count() - 1;
    highlight.second = false;
}
//===================================================
void ZMathExpressionHandler::zh_findBracketBackward(QChar bracketSymbol,
                                                    const QString& expression,
                                                    int pos,
                                                    QPair<int, bool>& highlight)
{
    int closeBracketCount = 0;
    for (int i = pos - 2; i >= 0; i--)
    {
        QChar currentSymbol = expression.at(i);

        if (currentSymbol == bracketSymbol)
        {
            if (closeBracketCount == 0)
            {
                highlight.first = i;
                highlight.second = true;
                return;
            }
            else
            {
                closeBracketCount--;
                continue;
            }
        }
        else
        {
            bool quitCircle = false;
            for (int s = 0; s < zv_pairSymbolVector.count(); s++)
            {
                if (quitCircle)
                {
                    continue;
                }

                if (currentSymbol
                    == zv_pairSymbolVector.value(s).second) // open bracket, not to find
                {
                    closeBracketCount++;
                    quitCircle = true;
                    continue;
                }

                if (currentSymbol == zv_pairSymbolVector.value(s).first)
                {
                    if (closeBracketCount < 1)
                    {
                        highlight.first = i;
                        highlight.second = false;
                        return;
                    }
                    else
                    {
                        closeBracketCount--;
                        quitCircle = true;
                        continue;
                    }
                }
            }
        }
    } // end of for(int i = pos+1; i < equation.count(); i++)

    highlight.first = 0;
    highlight.second = false;
}
//===================================================
bool ZMathExpressionHandler::zh_getToken()
{
    zv_token = QString();
    zv_tokenType = TT_NULL;
    int nError = -1;

    QChar currentChar;
    //QString token;

    zv_tokenStartPosition = zv_charPosition;
    QRegExp cellRegExp("^\\s{0,}\\[\\s{0,}$");

    // QRegExp("^~\\w{0,}$");

    for (; zv_charPosition < zv_expression.count(); zv_charPosition++)
    {
        currentChar = zv_expression.at(zv_charPosition);

        if (currentChar.isSpace())
        {
            if (zv_tokenType == TT_VARIABLE)
            {
                zv_token.append(currentChar);
            }
            continue;
        }

        if (zv_delimiterSet.contains(currentChar))
        {
            switch (zv_tokenType)
            {
            case TT_CELL:
                if (currentChar == ']')
                {
                    zv_token.append(currentChar);
                    zv_tokenEndPosition = zv_charPosition;
                    zv_charPosition++;

                    return true;
                }

                if ((currentChar == '-' || currentChar == '+')
                    && (!zv_token.contains('-') && !zv_token.contains('+')))
                {
                    zv_token.append(currentChar);
                    continue;
                }

                // mf_handleError(2);
                zv_tokenType = TT_ERROR;
                //return false;

                nError = 2;
                continue;

                break;
            case TT_NULL:
                zv_token.append(currentChar);
                if (currentChar == '[')
                {
                    zv_tokenType = TT_CELL;
                    continue;
                }

                if (currentChar == ']')
                {
                    // mf_handleError(3);
                    zv_tokenType = TT_ERROR;

                    //return false;
                    nError = 3;
                    continue;
                }

                zv_tokenType = TT_DELIMITER;
                zv_tokenEndPosition = zv_charPosition;
                zv_charPosition++;
                return true;

                break;
//            case TT_OPERATOR:
//                qDebug() << "TOK" << zv_token;
//                break;
            case TT_VARIABLE:
                if (zv_operatorSet.contains(zv_token.simplified()))
                {
                    zv_tokenType = TT_OPERATOR;
                }
                zv_tokenEndPosition = zv_charPosition - 1;
                return true;
                break;

            case TT_ERROR:
                zv_tokenEndPosition = zv_charPosition;
                zh_handleError(nError);
                return false;

                break;
            default:
                zv_tokenEndPosition = zv_charPosition - 1;
                return true;

                //return true;
            }
        }
        else if (currentChar.isLetter() || zv_extraSymbolSet.contains(currentChar))
        {
            switch (zv_tokenType)
            {
            case TT_NULL:
            case TT_VARIABLE:
                zv_tokenType = TT_VARIABLE;
                zv_token.append(currentChar);

                continue;
                // break;
            case TT_CELL:
                if (!zv_token.contains('-') && !zv_token.contains('+'))
                {
                    zv_token.append(currentChar);
                    continue;
                }

            default:
                //mf_handleError(2);
                nError = 2;
                zv_tokenType = TT_ERROR;
                continue;
                // return false;
            }
        }
        else if (currentChar == '~')
        {
            switch (zv_tokenType)
            {
            case TT_NULL:
                zv_tokenType = TT_VARIABLE;
                zv_token.append(currentChar);
                continue;

            case TT_CELL:

                if (cellRegExp.exactMatch(zv_token))
                {
                    zv_token.append(currentChar);
                }
                continue;

                //break;
                //            case TT_VARIABLE:

                //                mv_token.append(currentChar);
                //                continue;
                //                break;
                //            case TT_CELL:
                //                if(!mv_token.contains('-') && !mv_token.contains('+'))
                //                {
                //                    mv_token.append(currentChar);
                //                    continue;
                //                }

            default:
                //mf_handleError(2);
                nError = 2;
                zv_tokenType = TT_ERROR;
                continue;
                // return false;
            }
        }
        else if (currentChar.isDigit())
        {
            switch (zv_tokenType)
            {
            case TT_NULL:
                zv_tokenType = TT_INTEGER;
            case TT_VARIABLE:
            case TT_INTEGER:
            case TT_DOUBLE:
            case TT_CELL:
                zv_token.append(currentChar);

                continue;
                break;

            default:
                // mf_handleError(2);
                nError = 2;
                zv_tokenType = TT_ERROR;
                //return false;
                continue;
            }
        }
        else if (currentChar == '.' || currentChar == ',')
        {
            switch (zv_tokenType)
            {
            case TT_INTEGER:

                zv_tokenType = TT_DOUBLE;
                zv_token.append(currentChar);
                continue;
                break;

            default:
                // mf_handleError(2);
                nError = 2;
                zv_tokenType = TT_ERROR;
                //return false;
                continue;
            }
        }
        else
        {
            // mf_handleError(2);
            nError = 2;
            zv_tokenType = TT_ERROR;
            //return false;
            continue;
        }
    }

    zv_tokenEndPosition = zv_expression.count() - 1;

    if (zv_tokenType == TT_CELL)
    {
        zh_handleError(2);
        zv_tokenType = TT_ERROR;
        return false;
    }

    if (zv_tokenType == TT_ERROR)
    {
        zh_handleError(nError);
        return false;
    }

    return true;

    //return true;
}
//===================================================
bool ZMathExpressionHandler::zh_addOrSubtract(double& result)
{
    QString op;
    double temp;
    if (!zh_multiplyOrDevide(result))
    {
        return false;
    }
    while ((op = zv_token) == "+" || op == "-")
    {
        if (!zh_getToken())
        {
            return false;
        }
        if (!zh_multiplyOrDevide(temp))
        {
            return false;
        }

        if (op == "-")
        {
            result = result - temp;
        }
        else if (op == "+")
        {
            result = result + temp;
        }
    }

    return true;
}
//===================================================
bool ZMathExpressionHandler::zh_multiplyOrDevide(double& result)
{
    QString op;
    double temp;
    if (!zh_power(result))
    {
        return false;
    }

    while ((op = zv_token) == "*" || op == "/")
    {
        if (!zh_getToken())
        {
            return false;
        }
        if (!zh_power(temp))
        {
            return false;
        }

        if (op == "*")
        {
            result = result * temp;
        }
        else if (op == "/")
        {
            if (temp == 0)
            {
                zh_handleError(1);
                return false;
            }
            result = result / temp;
        }
    }

    return true;
}
//===================================================
bool ZMathExpressionHandler::zh_power(double& result)
{
    if (!zh_unaryOperation(result))
    {
        return false;
    }

    if (zv_token == "^")
    {
        if (!zh_getToken())
        {
            return false;
        }

        double temp = 0.0;
        if (!zh_unaryOperation(temp))
        {
            return false;
        }

        double ex = result;
        if (temp == 0.0)
        {
            result = 1.0;
            return true;
        }

        int t;
        for (t = (int) temp - 1; t > 0; --t)
        {
            result = result * (double) ex;
        }
        return true;
    }

    return true;
}
//===================================================
bool ZMathExpressionHandler::zh_unaryOperation(double& result)
{
    QString op;
    if ((zv_tokenType == TT_DELIMITER || zv_tokenType == TT_OPERATOR)
        && (zv_token == "+" || zv_token == "-" || zv_token == "ln" || zv_token == "lg"
            || zv_token == "exp"))
    {
        op = zv_token;
        if (!zh_getToken())
        {
            return false;
        }
    }

    if (zh_recognizeBrackets(result))
    {
        if (op == "-")
        {
            result = -result;
        }
        else if (op == "ln")
        {
            result = log(result);
        }
        else if (op == "lg")
        {
            result = log10(result);
        }
        else if (op == "exp")
        {
            result = exp(result);
        }

        return true;
    }
    else
    {
        return false;
    }
}
//===================================================
bool ZMathExpressionHandler::zh_recognizeBrackets(double& result)
{
    if (zv_token == "(")
    {
        if (!zh_getToken())
        {
            return false;
        }
        if (!zh_addOrSubtract(result))
        {
            return false;
        }

        if (zv_token != ")")
        {
            zh_handleError(3);
            return false;
        }
        return zh_getToken();
    }
    else
    {
        return zh_variableValue(result);
    }
}
//===================================================
bool ZMathExpressionHandler::zh_variableValue(double& result)
{
    bool res;
    QString simplifiedToken = zv_token.simplified();

    if (zv_tokenType == TT_DOUBLE || zv_tokenType == TT_INTEGER)
    {
        result = simplifiedToken.toDouble(&res);
        if (!res)
        {
            zh_handleError(4);
            return false;
        }
    }
    else if (zv_tokenType == TT_VARIABLE || zv_tokenType == TT_CELL)
    {
        emit zs_requestVariableValue(simplifiedToken, result, res);
        if (!res)
        {
            zh_handleError(5);
            return false;
        }
    }

    return zh_getToken();
}
//===================================================
void ZMathExpressionHandler::zh_handleError(int error)
{
    QString errorString;
    switch (error)
    {
    case 0:
        errorString = tr("Empty expression");
        break;
    case 1:
        errorString = tr("Division by zero");
        break;
    case 2:
        errorString = tr("Wrong token");
        break;
    case 3:
        errorString = tr("Bracket error");
        break;
    case 4:
        errorString = tr("Cannot convert to double");
        break;
    case 5:
        errorString = tr("Wrong variable");
        break;
    }

    emit zs_errorReport(errorString, zv_tokenStartPosition, zv_tokenEndPosition);
}
//===================================================
//bool ZEquationHandler::mf_checkSpacesInToken()
//{
//    mv_token = mv_token.trimmed();
//    QRegExp regExp("[' ',\t,\n,\v,\f,\r]");
//    if(regExp.exactMatch(mv_token))
//    {
//        return false;
//    }

//    return true;
//}
//===================================================
void ZMathExpressionHandler::zp_defineHighLights(const QString& expression,
                                                 int cursorPos,
                                                 QVector<QPair<int, bool>>& highlightedSymbolVector)
{
    QChar symbol;
    highlightedSymbolVector.clear();
    QPair<int, bool> forwardHighlight;
    QPair<int, bool> backwardHighlight;
    forwardHighlight.first = -1;
    backwardHighlight.first = -1;

    if (cursorPos < expression.count() && cursorPos >= 0)
    {
        symbol = expression.at(cursorPos);
        for (int i = 0; i < zv_pairSymbolVector.count(); i++)
        {
            if (zv_pairSymbolVector.value(i).first == symbol)
            {
                zh_findBracketForward(zv_pairSymbolVector.value(i).second,
                                      expression,
                                      cursorPos,
                                      forwardHighlight);
                highlightedSymbolVector << QPair<int, bool>(cursorPos, forwardHighlight.second);
                highlightedSymbolVector << forwardHighlight;
            }
        }
    }

    if (cursorPos > 0 && cursorPos <= expression.count())
    {
        symbol = expression.at(cursorPos - 1);
        for (int i = 0; i < zv_pairSymbolVector.count(); i++)
        {
            if (zv_pairSymbolVector.value(i).second == symbol)
            {
                zh_findBracketBackward(zv_pairSymbolVector.value(i).first,
                                       expression,
                                       cursorPos,
                                       backwardHighlight);
                highlightedSymbolVector
                    << QPair<int, bool>(cursorPos - 1, backwardHighlight.second);
                highlightedSymbolVector << backwardHighlight;
            }
        }
    }
}
//===================================================
bool ZMathExpressionHandler::zp_checkExpression(const QString& expression,
                                              int cursorPos,
                                              QVector<QPair<int, bool>>& highlightedSymbolVector)
{
    zp_defineHighLights(expression, cursorPos, highlightedSymbolVector);
    return true;
}
//===================================================
bool ZMathExpressionHandler::zp_calculateExpression(const QString& expression, double& dResult)
{
    zv_expression = expression;
    zv_charPosition = 0;

    zh_getToken();

    if (zv_tokenType == TT_NULL)
    {
        zh_handleError(0); //  Выражение пусто.
        dResult = 0.0;
        return false;
    }

    bool res = zh_addOrSubtract(dResult);

    //    if(mv_tokenType == TT_NULL)
    //    {
    //        mf_handleError(0);  //  Последней  лексемой  должен  быть
    //        //  нулевой  символ.
    //    }

    return res;
}
//===================================================
