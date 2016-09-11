//==========================================================
#ifndef ZNEWDATABASEDIALOG_H
#define ZNEWDATABASEDIALOG_H
//==========================================================
#include <QDialog>
//==========================================================
class QPushButton;
class QLineEdit;
//==========================================================
/*!
 \brief

*/
class ZDatabasePropertiesDialog : public QDialog
{
    Q_OBJECT
public:
    /*!
     \brief

     \param name
     \param path
     \param parent
    */
    explicit ZDatabasePropertiesDialog(const QString& name,
                                       const QString& path,
                                       QWidget *parent = 0);

    /*!
     \brief

    */
    virtual ~ZDatabasePropertiesDialog();

    /*!
     \brief

     \return QString
    */
    QString zp_name() const;
    /*!
     \brief

     \return QString
    */
    QString zp_path() const;

signals:

    /*!
     \brief

     \param name
     \param path
     \param res
    */
    void zg_requestRecordDatabase(const QString& name,
                                 QString& path,
                                 bool& res);

private slots:

    /*!
     \brief

    */
    void zh_onBrowseButtonClick();
    /*!
     \brief

    */
    void zh_onOkButtonClick();

private:

    // VARS
    const QString zv_defaultDatabaseFolderSectionName = "defaultDatabaseFolder"; /*!< TODO: describe */
    const QString zv_settingsGroupName = "dbPropertiesDialog";

    QString zv_databaseFolderPath; /*!< TODO: describe */
    QLineEdit* zv_nameLineEdit; /*!< TODO: describe */
    QLineEdit* zv_pathLineEdit; /*!< TODO: describe */

    QPushButton* zv_browseButton; /*!< TODO: describe */
    QPushButton* zv_okButton; /*!< TODO: describe */
    QPushButton* zv_cancelButton; /*!< TODO: describe */


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
    void zh_restoreSettings();
    /*!
     \brief

    */
    void zh_saveSettings() const;

};
//==========================================================
#endif // ZNEWDATABASEDIALOG_H
