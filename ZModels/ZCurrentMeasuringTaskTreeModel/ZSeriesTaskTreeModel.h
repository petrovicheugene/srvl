//===========================================================
#ifndef ZSERIESTASKTREEMODEL_H
#define ZSERIESTASKTREEMODEL_H
//===========================================================
#include <QAbstractItemModel>
#include <QList>
//===========================================================
class ZCurrentMeasuringManager;
class ZSampleTaskItem;
class ZControlAction;
class ZItem;
//===========================================================
class ZSeriesTaskTreeModel : public QAbstractItemModel
{
    Q_OBJECT
public:
    explicit ZSeriesTaskTreeModel(QObject *parent = 0);

    // overrided FUNCS
    virtual QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;
    virtual Qt::ItemFlags flags(const QModelIndex &index) const;
    virtual bool hasChildren(const QModelIndex &parent = QModelIndex()) const;
    virtual QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const;
    virtual QModelIndex index(int row, int column, const QModelIndex &parent = QModelIndex()) const;
    virtual int columnCount(const QModelIndex &parent = QModelIndex()) const;
    virtual int rowCount(const QModelIndex &parent = QModelIndex()) const;
    virtual QModelIndex parent(const QModelIndex &index) const;

    QList<ZControlAction*> zp_sampleActions() const;
    QList<ZControlAction*> zp_contextSampleActions() const;

    QList<ZControlAction*> zp_taskActions() const;
    QList<ZControlAction*> zp_contextTaskActions() const;


signals:

public slots:

private slots:

    void zh_onNewSampleAction();
    void zh_onRemoveSampleAction();

    void zh_onNewTaskAction();
    void zh_onAddTaskAction();
    void zh_onEditTaskAction();
    void zh_onRemoveTaskAction();

    void zh_checkSampleName(const QString& sampleName, bool& res);
    void zh_checkTaskName(int sampleTaskId, const QString& taskName, bool& res);

private:

    // VARS
    QList<ZSampleTaskItem*> zv_sampleTaskList;

    // actions
    ZControlAction* zv_newSampleAction;
    ZControlAction* zv_removeSampleAction;

    ZControlAction* zv_newTaskAction;
    ZControlAction* zv_addTaskAction;
    ZControlAction* zv_editTaskAction;
    ZControlAction* zv_removeTaskAction;

    // FUNCS

    void zh_createActions();
    void zh_createConnections();

    void zh_insertSampleTaskIntoList(const QString& sampleName,
                                     const QString &sampleTaskName);
    ZItem* zh_itemForIndex(const QModelIndex& index) const;


    bool zh_checkSampleName(const QString& sampleName) const;
    int zh_findLastSampleSerialNumber(const QString& sampleNameTemplate) const;
};
//===========================================================
#endif // ZSERIESTASKTREEMODEL_H
