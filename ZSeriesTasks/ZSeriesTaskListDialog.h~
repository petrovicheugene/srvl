//=====================================================
#ifndef ZSERIESTASKLISTDIALOG_H
#define ZSERIESTASKLISTDIALOG_H
//=====================================================
#include <QDialog>
#include <QModelIndex>
//=====================================================
class ZSeriesTaskListManager;
class ZSeriesTaskTableWidget;

class QLabel;
class QLineEdit;
class QPushButton;
class QTextEdit;
//=====================================================
class ZSeriesTaskListDialog : public QDialog
{
    Q_OBJECT
public:
    explicit ZSeriesTaskListDialog(bool forLoad, QWidget *parent = 0);

    void zp_connectToManager(ZSeriesTaskListManager* manager);

signals:


public slots:

protected:

    void closeEvent(QCloseEvent* event);
    bool eventFilter(QObject* object, QEvent* event);

private slots:

    void zh_setCurrentTaskProperties(const QString& name,
                                     const QString& description);

    void zh_onOkButtonClick();

private:

    // VARS
    ZSeriesTaskListManager* zv_manager;
    ZSeriesTaskTableWidget* zv_seriesTaskTableWidget;
    QLineEdit* zv_seriesNameLineEdit;
    QTextEdit* zv_descriptionTextEdit;
    QPushButton* zv_okButton;
    QPushButton* zv_cancelButton;
    bool zv_forLoad;

    // FUNCS

    void zh_createComponents();
    void zh_createConnections();
    void zh_restoreSettings();
    void zh_saveSettings() const;

};
//=====================================================
#endif // ZSERIESTASKLISTDIALOG_H
