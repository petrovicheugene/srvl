//=================================================
#ifndef ZSAMPLETASKTREEITEM_H
#define ZSAMPLETASKTREEITEM_H
//=================================================
#include <QObject>
#include <QModelIndex>
#include <QString>
#include <QList>
#include <QMap>
//=================================================
class ZSampleTaskTreeItemOptions
{
public:
    enum ItemType {IT_ROOT,
                   IT_MEASURING_CONDITIONS,
                   IT_CHEMICAL_TASK,
                   IT_CALIBRATION};
    
    ItemType zp_type() const
    {
        return zv_type;
    }
    
protected:
    
    // Constructor
    ZSampleTaskTreeItemOptions(ItemType type)
    {
        zv_type = type;
    }
    
    // VARS
    ItemType zv_type;
    
};
//=================================================
class ZSampleTaskTreeMeasuringConditionsItemOptions : public ZSampleTaskTreeItemOptions
{
public:
    
    ZSampleTaskTreeMeasuringConditionsItemOptions(int gainFactor,
                                                  int exposition)
        : ZSampleTaskTreeItemOptions(IT_MEASURING_CONDITIONS)
    {
        zv_gainFactor = gainFactor;
        zv_exposition = exposition;
    }
    
    virtual ~ZSampleTaskTreeMeasuringConditionsItemOptions(){}
    
    int zp_gainFactor() const
    {
        return zv_gainFactor;
    }
    int zp_exposition() const
    {
        return zv_exposition;
    }
    
protected:
    
    // VARS
    int zv_gainFactor;
    int zv_exposition;
    
};
// END OPTIONS
//=================================================
class ZSampleTaskTreeBaseItem : public QObject
{
    Q_OBJECT
public:
    explicit ZSampleTaskTreeBaseItem(ZSampleTaskTreeItemOptions::ItemType type,
                                     ZSampleTaskTreeBaseItem* parent = 0);
    virtual ~ZSampleTaskTreeBaseItem();
    enum ItemState {IS_CLEAR, IS_DIRTY};
    enum ItemOperationType {IOT_ABOUT_TO_INSERT_ROW,
                            IOT_ROW_INSERTED,
                            IOT_ABOUT_TO_REMOVE_ROW,
                            IOT_ROW_REMOVED};
    
    QString zp_itemName() const;
    bool zp_setItemName(const QString& displayString);
    
    ZSampleTaskTreeBaseItem* zp_parent() const;
    ZSampleTaskTreeBaseItem* zp_childAt(int childIndex) const;
    
    ZSampleTaskTreeItemOptions::ItemType zp_itemType() const;
    int zp_id() const;
    ItemState zp_itemState() const;
    bool zp_hasChildren() const;
    int zp_childCount() const;
    int zp_rowOfChild(ZSampleTaskTreeBaseItem *item) const;
    bool zp_findItemRow(ZSampleTaskTreeBaseItem *item,
                        ZSampleTaskTreeBaseItem*& parentItem,
                        int& row);
    
    virtual bool zp_createChild(ZSampleTaskTreeItemOptions* options) = 0;
    
signals:
    
    void zg_requestIndexForItem(ZSampleTaskTreeBaseItem* item, QModelIndex& index);
    
    void zg_itemOperation(ItemOperationType type, QModelIndex parent, int first, int last);
    
protected:
    
    // VARS
    QString zv_itemName;
    ZSampleTaskTreeItemOptions::ItemType zv_itemType;
    int zv_id;
    ItemState zv_itemState;

    QList<ZSampleTaskTreeBaseItem*> zv_childList;
    ZSampleTaskTreeBaseItem* zv_parent;
    
    
    // STATIC
    // VARS
    static const QMap<ZSampleTaskTreeItemOptions::ItemType, QString> zv_tableNameForTypeMap;
    
    // FUNCS
    static QMap<ZSampleTaskTreeItemOptions::ItemType, QString> zp_initTableNameForTypeMap();
    
};
//=================================================
class ZSampleTaskTreeRootItem : public ZSampleTaskTreeBaseItem
{
    Q_OBJECT
public:
    explicit ZSampleTaskTreeRootItem();
    
    virtual bool zp_createChild(ZSampleTaskTreeItemOptions* options);
    
};
//=================================================
class ZSampleTaskTreeMeasuringConditionsItem : public ZSampleTaskTreeBaseItem
{
    Q_OBJECT
public:
    explicit ZSampleTaskTreeMeasuringConditionsItem(ZSampleTaskTreeMeasuringConditionsItemOptions* options,
                                                    ZSampleTaskTreeBaseItem* parent);
    virtual bool zp_createChild(ZSampleTaskTreeItemOptions* options);
    
    int zp_gainFactor() const;
    int zp_exposition() const;
    
protected:
    
    // VARS 
    int zv_gainFactor;
    int zv_exposition;

    // FUNCS
    void zh_createItemName();

};
//=================================================
#endif // ZSAMPLETASKTREEITEM_H
