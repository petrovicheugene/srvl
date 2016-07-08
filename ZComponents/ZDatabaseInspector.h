//=========================================================
#ifndef ZDATABASEINSPECTOR_H
#define ZDATABASEINSPECTOR_H
//=========================================================
#include <QObject>
#include <QSqlDatabase>
#include <QString>
//=========================================================

//=========================================================
class ZDatabaseInspector : public QObject
{
    Q_OBJECT
public:
    explicit ZDatabaseInspector(QObject *parent = 0);

    // FUNCS
    static bool zp_inspectDataBase(const QString &name, const QString& path, QString& msg);

    static bool zp_checkDatabasePath(const QString& path, QString& msg);
    static bool zp_checkDatabaseFileExistance(const QString& path, QString& msg);
    static bool zp_createDatabase(const QString &name, const QString& path, QString& msg);
    static bool zp_checkFileEmptyness(const QString& name, const QString& path, QString& msg);
    static bool zp_checkDatabaseSuitability(const QString& name, const QString& path, QString& msg);
    static bool zp_connectToDatabase(const QString& name,
                                             const QString& path,
                                             QSqlDatabase& db,
                                             QString& msg);
    static void zp_disconnectFromDatabase(QSqlDatabase &db);
    static void zp_disconnectFromDatabase(const QString& connectionName);

    // VARS
    static const int zv_userVersion;

};
//=========================================================
#endif // ZDATABASEINSPECTOR_H
