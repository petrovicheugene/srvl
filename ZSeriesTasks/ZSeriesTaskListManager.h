//======================================================
#ifndef ZSERIESTASKLISTMANAGER_H
#define ZSERIESTASKLISTMANAGER_H
//======================================================
#include <QObject>
#include <QModelIndex>
//======================================================
class QSqlTableModel;
class QAbstractItemView;
class ZControlAction;
//======================================================
class ZSeriesTaskListManager : public QObject
{
    Q_OBJECT
public:
    explicit ZSeriesTaskListManager(QObject *parent = 0);

    QAbstractTableModel* zp_model();

    bool zp_loadSeriesTask();
    bool zp_saveSeriesTask(const QString& taskName, const QString& description);
    qint64 zp_newSeriesTaskId() const;
    int zp_currentSeriesTaskId();

    QString zp_lastError() const;

signals:

    void zg_currentTaskProperties(const QString& name, const QString& description);
    void zg_inquirySampleTaskIdList(QList<int>& idList);

public slots:

    void zp_onCurrentChange(const QModelIndex& current, const QModelIndex &previous);

private:

    // VARS
    QSqlTableModel* zv_seriesSqlTableModel;
    QModelIndex zv_currentIndex;
    QString zv_errorMsg;
    qint64 zv_newSeriesTaskId;

    // FUNCS
    void zh_createComponents();
    void zh_createActions();
    void zh_createConnections();
    bool zh_checkTaskNameAndFindNewId(const QString& name, int &newId);

};
//======================================================
#endif // ZSERIESTASKLISTMANAGER_H
