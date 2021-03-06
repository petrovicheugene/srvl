//===================================================
#ifndef ZSAMPLETASKTREEMODEL_H
#define ZSAMPLETASKTREEMODEL_H
//===================================================
#include <QAbstractItemModel>
#include <QList>
#include <QMap>
#include "ZSampleTaskTreeItem.h"
//===================================================
class ZControlAction;
//===================================================
class ZSampleTaskTreeModel : public QAbstractItemModel
{
    Q_OBJECT
public:
    explicit ZSampleTaskTreeModel(QObject *parent = 0);
    ~ZSampleTaskTreeModel();
    // overrided FUNCS
    virtual QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;
    virtual Qt::ItemFlags flags(const QModelIndex &index) const;
    virtual bool hasChildren(const QModelIndex &parent = QModelIndex()) const;
    virtual QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const;
    virtual QModelIndex index(int row, int column, const QModelIndex &parent = QModelIndex()) const;
    virtual int columnCount(const QModelIndex &parent = QModelIndex()) const;
    virtual int rowCount(const QModelIndex &parent = QModelIndex()) const;
    virtual QModelIndex parent(const QModelIndex &index) const;

    int zp_childCount(QModelIndex parent) const;
    QList<ZControlAction*> zp_buttonTreeActions() const;


    //QMap<QString, QList<ZControlAction*> > zp_buttonTreeActions() const;
    QList<ZControlAction*> zp_contextTreeActions() const;

    ZSampleTaskTreeBaseItem* zp_itemForIndex(const QModelIndex& index) const;

    void zp_appendChemicalTask(int chemicalTaskId);
    void zp_appendMeasuringConditions(int id, int gainFactor, int exposition, int quantity);

signals:

    void zg_setCurrentIndex(const QModelIndex& index) const;

public slots:


private slots:

    void zh_onAppendMeasuringConditionsAction();
    void zh_onAppendChemicalTaskAction();
    void zh_onRemoveCurrentTaskElementAction();

    void zh_indexForItem(ZSampleTaskTreeBaseItem* item,
                         QModelIndex& index);
    void zh_onItemOperation(ZSampleTaskTreeBaseItem::ItemOperationType type,
                            QModelIndex parent,
                            int first,
                            int last);
private:

    // VARS
    ZSampleTaskTreeRootItem* zv_rootItem;
    const int zv_columnCount = 3;

    // actions
    ZControlAction* zv_appendMeasuringConditionsAction;
    ZControlAction* zv_appendChemicalTaskAction;
    ZControlAction* zv_removeCurrentTaskElementAction;

    // FUNCS

    void zh_createActions();
    void zh_createConnections();

};
//===================================================
#endif // ZSAMPLETASKTREEMODEL_H
