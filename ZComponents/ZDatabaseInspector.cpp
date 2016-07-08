//=========================================================
#include "ZDatabaseInspector.h"
#include "ZConstants.h"

#include <QMessageBox>
#include <QFile>
#include <QDir>
#include <QFileInfo>
#include <QDataStream>
#include <QIODevice>
#include <QSqlQuery>
//=========================================================
const int ZDatabaseInspector::zv_userVersion = 41086;
//=========================================================
ZDatabaseInspector::ZDatabaseInspector(QObject *parent) : QObject(parent)
{

}
//=========================================================
bool ZDatabaseInspector::zp_inspectDataBase(const QString &name, const QString& path, QString& msg)
{
    // check path to db file
    if(!zp_checkDatabasePath(path, msg))
    {
        return false;
    }

    // check the db file existance
    if(zp_checkDatabaseFileExistance(path, msg))
    {
        // file exists
        // check db content
        if(zp_checkFileEmptyness(name, path, msg))
        {
            // file is empty
            // create database
            return zp_createDatabase(name, path, msg);
        }
        else
        {
            // file is not empty
            return zp_checkDatabaseSuitability(name, path, msg);
        }
    }
    else
    {
        // file doesn't exist
        // create a new db file and all tables
        return zp_createDatabase(name, path, msg);
    }
}
//=========================================================
bool ZDatabaseInspector::zp_checkDatabasePath(const QString& path, QString& msg)
{
    QFileInfo fileInfo(path);
    bool error = false;
    if(!QDir::isAbsolutePath(path))
    {
        msg = tr("The database file path \"%1\" is not an absolute path!").arg(path);
        error = true;
    }
    else if(fileInfo.isDir())
    {
        msg = tr("\"%1\" is not a file!").arg(path);
        error = true;
    }

    return !error;
}
//=========================================================
bool ZDatabaseInspector::zp_checkDatabaseFileExistance(const QString& path, QString& msg)
{
    QFileInfo fileInfo(path);
    bool res = false;
    if(fileInfo.exists())
    {
        msg = tr("File \"%1\" is already exist!").arg(path);
        res = true;
    }
    else
    {
        msg = tr("File \"%1\" is not exist!").arg(path);
        res =  false;
    }

    return res;
}
//=========================================================
bool ZDatabaseInspector::zp_createDatabase(const QString& name, const QString& path, QString& msg)
{
    // connect to new db
    QSqlDatabase db;
    bool res = zp_connectToDatabase(name, path, db, msg);

    if(!res)
    {
        return false;
    }


    // create tables


    // set database version
    QString sqlPragma = "PRAGMA user_version=" + QString::number(zv_userVersion);
    QSqlQuery query(db);
    query.setForwardOnly(true);
    query.exec(sqlPragma);
    db.commit();

    // close database
    zp_disconnectFromDatabase(db);
    return true;
}
//=========================================================
bool ZDatabaseInspector::zp_checkFileEmptyness(const QString& name, const QString& path, QString& msg)
{
    // check if database file is empty
    QFile file(path);
    if(!file.open(QIODevice::ReadOnly))
    {
        msg = tr("Cannot open database file \"%1\"").arg(path);
        return false;
    }

    QDataStream dataStream(&file);
    bool fileIsEmpty = dataStream.atEnd();
    file.close();

    return fileIsEmpty;
}
//=========================================================
bool ZDatabaseInspector::zp_checkDatabaseSuitability(const QString& name, const QString& path, QString& msg)
{
    // check database version
    QSqlDatabase db;
    if(!zp_connectToDatabase(name, path, db, msg))
    {
        return false;
    }

    int userVersion = -1;
    QSqlQuery query(db);
    query.setForwardOnly(true);
    query.exec("PRAGMA user_version");
    if (!query.isActive())
    {
        // error
        msg = tr("Cannot fetching DB Version from database \"%1\"!").arg(path);
        zp_disconnectFromDatabase(db);
        return false;
    }

    if(query.next())
    {
        userVersion = query.value(0).toInt();
    }

    zp_disconnectFromDatabase(db);

    if(userVersion != zv_userVersion)
    {
        msg = tr("The database \"%1\" is valid but have  inappropriate version!").arg(path);
        return false;
    }

    return true;
}
//=========================================================
bool ZDatabaseInspector::zp_connectToDatabase(const QString& name, const QString& path, QSqlDatabase& db,QString& msg)
{
    db = QSqlDatabase::addDatabase("QSQLITE", name);
    if(!db.isValid())
    {
        msg = tr("Cannot establish %1 driver connection.").arg("QSQLITE");
        return false;
    }
    //db.setHostName("localhost");
    db.setDatabaseName(path);

    if(!db.open())
    {
        msg = tr("Cannot open database. \n"
                 "Unable to establish a \"%1\" database connection.").arg(path);

        return false;
    }

    return true;
}
//=========================================================
void ZDatabaseInspector::zp_disconnectFromDatabase(QSqlDatabase& db)
{
    if(!db.isValid())
    {
        return;
    }

    if(db.isOpen())
    {
        db.close();
    }

    QSqlDatabase::removeDatabase(db.connectionName());
}
//=========================================================
void ZDatabaseInspector::zp_disconnectFromDatabase(const QString& connectionName)
{
    QSqlDatabase db = QSqlDatabase::database(connectionName);
    zp_disconnectFromDatabase(db);
}

//=========================================================
