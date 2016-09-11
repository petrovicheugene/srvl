//=========================================================
#ifndef ZDATABASEINSPECTOR_H
#define ZDATABASEINSPECTOR_H
//=========================================================
#include <QObject>
#include <QSqlDatabase>
#include <QString>
//=========================================================

//=========================================================
/*!
 \brief

*/
class ZDatabaseInspector : public QObject
{
    Q_OBJECT
public:
    /*!
     \brief

     \param parent
    */
    explicit ZDatabaseInspector(QObject *parent = 0);

    // FUNCS
    /*!
     \brief

     \param name
     \param path
     \param msg
     \return bool
    */
    static bool zp_inspectNewDataBase(const QString &name, const QString& path, QString& msg);
    /*!
     \brief

     \param name
     \param path
     \param msg
     \return bool
    */
    static bool zp_inspectExistingDataBase(const QString &name, const QString& path, QString& msg);


    /*!
     \brief

     \param path
     \param msg
     \return bool
    */
    static bool zp_checkDatabasePath(const QString& path, QString& msg);
    /*!
     \brief

     \param path
     \param msg
     \return bool
    */
    static bool zp_checkDatabaseFileExistance(const QString& path, QString& msg);
    /*!
     \brief

     \param name
     \param path
     \param msg
     \return bool
    */
    static bool zp_createDatabase(const QString &name, const QString& path, QString& msg);
    static bool zp_createTables(QSqlDatabase &db, QString &msg);

    /*!
     \brief

     \param name
     \param path
     \param msg
     \return bool
    */
    static bool zp_checkFileEmptyness(const QString& name, const QString& path, QString& msg);
    /*!
     \brief

     \param name
     \param path
     \param msg
     \return bool
    */
    static bool zp_checkDatabaseSuitability(const QString& name, const QString& path, QString& msg);
    /*!
     \brief

     \param name
     \param path
     \param db
     \param msg
     \return bool
    */
    static bool zp_connectToDatabase(const QString& name,
                                             const QString& path,
                                             QSqlDatabase& db,
                                             QString& msg);
    /*!
     \brief

     \param db
    */
    static void zp_disconnectFromDatabase(QSqlDatabase &db);
    /*!
     \brief

     \param connectionName
    */
    static void zp_disconnectFromDatabase(const QString& connectionName);

    // VARS
    static const int zv_userVersion; /*!< TODO: describe */

};
//=========================================================
#endif // ZDATABASEINSPECTOR_H
