//=========================================================
#ifndef ZSTARTDIALOG_H
#define ZSTARTDIALOG_H
//=========================================================
#include <QDialog>
#include <QItemSelection>
#include <QSqlDatabase>
//=========================================================
class QPushButton;
class QListWidget;
class QTableWidgetItem;
class QLabel;
//=========================================================
/*!
 \brief

*/
class ZStartDialog : public QDialog
{
   Q_OBJECT
public:
   /*!
     \brief Constructor

     \param parent - parent widget
    */
   explicit ZStartDialog(QWidget* parent = 0);

   void zp_databaseProperties(QString& name, QString& path) const;

private slots:

   /*!
     \brief

    */
   void zh_onNewDatabase();
   /*!
     \brief

    */
   void zh_onEditDatabase();
   /*!
     \brief

    */
   void zh_onRemoveDatabase();
   /*!
     \brief

    */
   void zh_onConnectToDatabase();
   /*!
     \brief

     \param current
     \param previous
    */
   void zh_onCurrentRowChange(const QModelIndex& current, const QModelIndex& previous);
   /*!
     \brief

     \param selected
     \param deselected
    */
   void zh_onSelectionChange(const QItemSelection& selected, const QItemSelection& deselected);
   /*!
     \brief

     \param name
     \param path
     \param res
    */
   void zh_checkAndStoreDatabase(const QString &name,
                                 const QString &path, bool &res);

private:

   // VARS
   const QString zv_defaultDatabaseName = tr("Database #"); /*!< TODO: describe */
   const QString zv_databaseArrayCapture = "databases";
   const QString zv_databaseNameValueCaption = "name";
   const QString zv_databasePathValueCaption = "path";

   QString zv_dbName;
   QString zv_dbPath;

   QListWidget* zv_dbListWidget; /*!< TODO: describe */
   QLabel* zv_currentPathLabel; /*!< TODO: describe */
   QPushButton* zv_newButton; /*!< TODO: describe */
   QPushButton* zv_editButton; /*!< TODO: describe */
   QPushButton* zv_removeButton; /*!< TODO: describe */
   QPushButton* zv_connectButton; /*!< TODO: describe */
   QPushButton* zv_cancelButton; /*!< TODO: describe */
   int zv_editedRow; /*!< TODO: describe */

   // FUNCS
   /*!
     \brief

    */
   void zh_createComponents();
   /*!
     \brief

    */
   void zh_createConnections();
   /*!
     \brief

    */
   void zh_loadDatabaseList();
   void zh_saveDatabaseList() const;


   /*!
     \brief

     \param name
     \param path
    */
   void zh_editAndRecordDatabase(QString name, QString path);
   //    void zh_checkDatabaseProperties(const QString &name,
   //                          const QString& path, QString& comment, bool &res);


   /*!
     \brief

     \param name
     \param path
     \param msg
     \return bool
    */
   bool zh_emptynessCheck(const QString &name,
                          const QString& path, QString& msg) const;
   /*!
     \brief

     \param path
     \return QString
    */
   QString zh_suffixCheck(const QString& path) const;

   /*!
     \brief

     \param name
     \param path
     \param msg
     \param samePathRowList
     \return bool
    */
   bool  zh_existanceOfRecordCheck(const QString &name,
                                   const QString& path, QString& msg, QList<int>& samePathRowList) const;


   /*!
     \brief

     \param name
     \param path
     \param comment
     \param row
     \return int
    */
   int zh_recordRow(const QString& name, const QString& path,
                    const QString& comment, int row) ;
   /*!
     \brief

     \param currentRow
    */
   void zh_interfaceEnablingControl(int currentRow);
   void zh_findDublicateRows(const QString &dbPath, QList<int> &rowList);

};
//=========================================================
#endif // ZSTARTDIALOG_H
