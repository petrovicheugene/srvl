//=========================================================
#include "ZDatabaseInspector.h"
#include "ZGLConstantsAndDefines.h"
#include "ZGLConstantsAndDefines.h"

#include <QMessageBox>
#include <QFile>
#include <QFileDialog>
#include <QDir>
#include <QFileInfo>
#include <QDataStream>
#include <QIODevice>
#include <QSqlQuery>
#include <QSqlError>
#include <QStandardPaths>
//=========================================================
const int ZDatabaseInspector::zv_userVersion = 41086;
//=========================================================
ZDatabaseInspector::ZDatabaseInspector(QObject *parent) : QObject(parent)
{

}
//=========================================================
bool ZDatabaseInspector::zp_inspectNewDataBase(const QString &name, const QString& path, QString& msg)
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
            // file is empty msg is not necessary here
            msg.clear();
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
bool ZDatabaseInspector::zp_inspectExistingDataBase(const QString &name,
                                                    const QString& path,
                                                    QString& msg)
{
    // check path to db file
    if(!zp_checkDatabasePath(path, msg))
    {
        return false;
    }

    // check the db file existance
    if(!zp_checkDatabaseFileExistance(path, msg))
    {
        // file does not exist
        return false;
    }

    // file exists
    // check db content
    if(zp_checkFileEmptyness(name, path, msg))
    {
        // file is empty
        return false;
    }

    if(!zp_checkDatabaseSuitability(name, path, msg))
    {
        return false;
    }

    return true;
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
    res = zp_createTables(db, msg);

    if(res)
    {
        // set database version
        QString sqlPragma = "PRAGMA user_version=" + QString::number(zv_userVersion);
        QSqlQuery query(db);
        query.setForwardOnly(true);
        query.exec(sqlPragma);
        db.commit();
    }

    // close database
    zp_disconnectFromDatabase(db);
    return res;
}
//=========================================================
bool ZDatabaseInspector::zp_createTables(QSqlDatabase& db, QString& msg)
{
    // ask for SQL file source
    QString sqlFilePath;

    QMessageBox msgBox;
    msgBox.setText("The database can be initalized by a script from an external SQL file as well as from an internal SQL script.");
    msgBox.setInformativeText("Do you want to initialize database from an external SQL file?");
    // msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::Discard | QMessageBox::Cancel);
    msgBox.setIcon(QMessageBox::Question);
    msgBox.addButton(QMessageBox::Yes);
    msgBox.addButton(QMessageBox::No);

    int res = msgBox.exec();

    if(res == QMessageBox::Yes)
    {
        QString standardLocation = QStandardPaths::standardLocations(QStandardPaths::DocumentsLocation).value(0);
        sqlFilePath = QFileDialog::getOpenFileName(0, tr("Select SQL file"), standardLocation,
                                                   tr("SQL files(*.sql);;All files(*.*)"));
    }

    if(sqlFilePath.isEmpty())
    {
        sqlFilePath = ":/sql/SQL/sdanalyzer.sql";
    }

    // get SQL file
    QFile file(sqlFilePath);
    if(!file.open(QIODevice::ReadOnly))
    {
        msg = tr("Cannot open file with error: \"%1\".").arg(file.errorString());
        return false;
    }

    // parse sql statements
    QTextStream textStream(&file);
    QString line;
    QString sqlStatement;
    // query
    QSqlQuery query(db);
    query.setForwardOnly(true);
    while(!textStream.atEnd())
    {
        line = textStream.readLine();
        // comments
        if(line.isEmpty() || line.startsWith("--") || line.startsWith("//") || line.startsWith("/*"))
        {
            continue;
        }

        sqlStatement += line;

        // end of statement
        if(!sqlStatement.endsWith(";"))
        {
            continue;
        }

        if(!query.exec(sqlStatement))
        {
            msg = tr("The SQL statement \"%1\" has been executed with error: \"%2\"!").arg(sqlStatement, query.lastError().text());
            file.close();
            return false;
        }

        sqlStatement.clear();
    }

    file.close();
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
    if(name.isEmpty())
    {
        db = QSqlDatabase::addDatabase("QSQLITE");
    }
    else
    {
        db = QSqlDatabase::addDatabase("QSQLITE", name);
    }

    if(!db.isValid())
    {
        msg = tr("Cannot establish %1 driver connection.").arg("QSQLITE");
        return false;
    }
    //db.setHostName("localhost");
    db.setDatabaseName(path);

    if(!db.open())
    {
        QSqlError error = db.lastError();
        msg = tr("Cannot open database \"%1\" with following error:\n"
                 "%2").arg(path, error.text());

        return false;
    }

    return true;
}
//=========================================================
bool ZDatabaseInspector::zp_connectToDatabase(const QString& path,
                                              QSqlDatabase& db,
                                              QString& msg)
{
    return zp_connectToDatabase(QString(), path, db, msg);
}
//=========================================================
void ZDatabaseInspector::zp_disconnectFromDatabase()
{
    QSqlDatabase db = QSqlDatabase::database();
    zp_disconnectFromDatabase(db);
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
