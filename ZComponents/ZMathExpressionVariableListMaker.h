//===================================================
#ifndef ZMATHEXPRESSIONVARIABLELISTMAKER_H
#define ZMATHEXPRESSIONVARIABLELISTMAKER_H
//===================================================
#include <QObject>
#include <QMap>
//===================================================

//===================================================
class ZMathExpressionVariableListMaker : public QObject
{
    Q_OBJECT
public:
    explicit ZMathExpressionVariableListMaker(QObject* parent = nullptr);

    QStringList zp_variableList() const;
    void zp_error(QString& errorString,
                     int& errorTokenStartPosition,
                     int& errorTokenEndPosition) const;
    void zp_clear();

signals:

    void zs_variableCheckRequest(const QString& name, bool& res);

public slots:

    void zp_insertVariableValue(const QString& varName, double& value, bool& bRes);
    void zp_setError(const QString& errorString,
                     int errorTokenStartPosition,
                     int errorTokenEndPosition);

private:
    // VARS
    QMap<QString, qreal> zv_variableNameMap;
    QString zv_errorString;
    int zv_errorTokenStartPosition;
    int zv_errorTokenEndPosition;
    //FUNCS


};
//===================================================
#endif // ZMATHEXPRESSIONVARIABLELISTMAKER_H
