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
//=================================================
class ZSampleTaskTreeChemicalTaskItemOptions : public ZSampleTaskTreeItemOptions
{
public:

    ZSampleTaskTreeChemicalTaskItemOptions(int chemicalTaskId,
                                           QString chemicalTaskName,
                                           QString chemical)
        : ZSampleTaskTreeItemOptions(IT_CHEMICAL_TASK)
    {
        zv_chemicalTaskId = chemicalTaskId;
        zv_chemicalTaskName = chemicalTaskName;
        zv_chemical = chemical;
    }

    virtual ~ZSampleTaskTreeChemicalTaskItemOptions(){}
    int zp_chemicalTaskId() const
    {
        return zv_chemicalTaskId;
    }
    QString zp_chemicalTaskName() const
    {
        return zv_chemicalTaskName;
    }
    QString zp_chemical() const
    {
        return zv_chemical;
    }

protected:

    // VARS
    int zv_chemicalTaskId;
    QString zv_chemicalTaskName;
    QString zv_chemical;
};
//=================================================
class ZSampleTaskTreeCalibrationItemOptions : public ZSampleTaskTreeItemOptions
{
public:

    ZSampleTaskTreeCalibrationItemOptions(QString calibrationName,
                                          double minConcentration,
                                          double maxConcentration)
        : ZSampleTaskTreeItemOptions(IT_CALIBRATION)
    {
         zv_calibrationName = calibrationName;
         zv_minConcentration = minConcentration;
         zv_maxConcentration = maxConcentration;
    }

    virtual ~ZSampleTaskTreeCalibrationItemOptions(){}

    QString zp_calibrationName() const
    {
        return zv_calibrationName;
    }
    double zp_minConcentration() const
    {
        return zv_minConcentration;
    }
    double zp_maxConcentration() const
    {
        return zv_maxConcentration;
    }

protected:

    // VARS
    QString zv_calibrationName;
    double zv_minConcentration;
    double zv_maxConcentration;

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
    
    virtual ZSampleTaskTreeBaseItem* zp_createChild(ZSampleTaskTreeItemOptions* options) = 0;
    
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
    
    virtual ZSampleTaskTreeBaseItem* zp_createChild(ZSampleTaskTreeItemOptions* options);
    void zp_clear();
};
//=================================================
class ZSampleTaskTreeMeasuringConditionsItem : public ZSampleTaskTreeBaseItem
{
    Q_OBJECT
public:
    explicit ZSampleTaskTreeMeasuringConditionsItem(ZSampleTaskTreeMeasuringConditionsItemOptions* options,
                                                    ZSampleTaskTreeBaseItem* parent);
    virtual ZSampleTaskTreeBaseItem* zp_createChild(ZSampleTaskTreeItemOptions* options);
    
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
class ZSampleTaskTreeChemicalTaskItem : public ZSampleTaskTreeBaseItem
{
    Q_OBJECT
public:
    explicit ZSampleTaskTreeChemicalTaskItem(ZSampleTaskTreeChemicalTaskItemOptions* options,
                                             ZSampleTaskTreeBaseItem* parent);
    virtual ZSampleTaskTreeBaseItem* zp_createChild(ZSampleTaskTreeItemOptions* options);

    int zp_chemicalTaskId() const;
    QString zp_name() const;
    QString zp_chemical() const;

protected:

    // VARS
    int zv_chemicalTaskId;
    QString zv_name;
    QString zv_chemical;

    // FUNCS
    void zh_createItemName();

};
//=================================================
class ZSampleTaskTreeCalibrationItem : public ZSampleTaskTreeBaseItem
{
    Q_OBJECT
public:
    explicit ZSampleTaskTreeCalibrationItem(ZSampleTaskTreeCalibrationItemOptions* options,
                                            ZSampleTaskTreeBaseItem* parent);
    virtual ZSampleTaskTreeBaseItem* zp_createChild(ZSampleTaskTreeItemOptions* options);

    QString zp_calibrationName() const;
    double zp_minConcentration() const;
    double zp_maxConcentration() const;

protected:

    // VARS
    QString zv_calibrationName;
    double zv_minConcentration;
    double zv_maxConcentration;

    // FUNCS

};
//=================================================
#endif // ZSAMPLETASKTREEITEM_H
