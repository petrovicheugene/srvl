//======================================================
#ifndef ZCHEMICALELEMENTPROPERTYITEM_H
#define ZCHEMICALELEMENTPROPERTYITEM_H
//======================================================
#include <QObject>
#include <QList>
#include <QPair>
//======================================================
typedef QList<QStringList> PropertyNames;
//======================================================
class ZChemicalElementPropertyItem : public QObject
{
    Q_OBJECT
public:
    explicit ZChemicalElementPropertyItem(QObject* parent = nullptr);
    explicit ZChemicalElementPropertyItem(const QString& name, QObject* parent = nullptr);

    QString zp_name() const;
    void zp_setName(const QString& name);
    bool zp_createName(const QString& nameTemplate,
                       const QString &separator = " ");

    void zp_addChild(ZChemicalElementPropertyItem* child);
    QStringList zp_chemicalElementPropertySectionList() const;

    void zp_setChemicalElementProperty(const QString& key, const QString& value);
    QList<QPair<QString, QString> > zp_chemicalElementPropertyList() const;
    bool zp_setChemicalElementPropertyName(int index, const QString& name);
    bool zp_setChemicalElementPropertyValue(int index, const QString& value);
    QString zp_chemicalElementPropertyValue(const QString& key) const;
    QPair<QString, QString>zp_chemicalElementProperty(int index) const;
    void zp_setChemicalElementPropertyList(const QList<QPair<QString, QString> >& propertyList);

    ZChemicalElementPropertyItem* zp_parent() const;
    ZChemicalElementPropertyItem* zp_childAtRow(int row) const;
    ZChemicalElementPropertyItem* zp_childNamed(const QStringList &nameBranch);
    ZChemicalElementPropertyItem* zp_childNamed(const QString& name);
    void zp_chemicalElementPropertySectionBranch(QStringList& branch);

    int zp_childCount() const;
    bool zp_hasCildren() const;
    void zp_removeChildren();
    int zp_rowOfChildItem(const ZChemicalElementPropertyItem *child) const;
    void zp_completeChemicalElementPropertyNameList(PropertyNames &propertyNames) const;
    int zp_chemicalElementPropertyCount() const;
    int zh_elementLevelNumber() const;
    int zp_zNumber(const ZChemicalElementPropertyItem *callingItem) const;
    void zp_totalDescendantList(QList<ZChemicalElementPropertyItem *> &descendantList) const;

    bool zp_initRemoving();
    void zp_removeChild(ZChemicalElementPropertyItem* child);
    bool zp_removeChemicalElementProperty(int propertyIndex);
    void zp_removeChemicalElementProperties();
    void zp_clear();

signals:

    void zg_itemAboutToBeInserted(int row, const ZChemicalElementPropertyItem* parentItem) const;
    void zg_itemInserted(const ZChemicalElementPropertyItem* item) const;

    void zg_requestItemRemovingPermission(const ZChemicalElementPropertyItem* item,
                                bool& permission);
    void zg_itemAboutToBeRemoved(const ZChemicalElementPropertyItem* item);
    void zg_itemRemoved(const ZChemicalElementPropertyItem* item);

    void zg_itemIsChanged(const ZChemicalElementPropertyItem* item);
    void zg_itemPropertyValueIsChanged(const ZChemicalElementPropertyItem* item);
    void zg_itemPropertyNameIsChanged(const ZChemicalElementPropertyItem* item);

private:

    //VARS
    QString zv_name;
    ZChemicalElementPropertyItem* zv_parentItem;
    QList<ZChemicalElementPropertyItem*> zv_childItemList;
    QList<QPair<QString, QString> > zv_chemicalElementPropertyList;

    // FUNCS
    ZChemicalElementPropertyItem* zp_createChild(const QString& name);

};
typedef const ZChemicalElementPropertyItem* ZChemicalElementPropertyItemCPtr;
Q_DECLARE_METATYPE(ZChemicalElementPropertyItemCPtr)
//======================================================
#endif // ZCHEMICALELEMENTPROPERTYITEM_H
