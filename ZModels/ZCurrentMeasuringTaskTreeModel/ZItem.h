//=================================================================
#ifndef ZITEM_H
#define ZITEM_H
//=================================================================
#include <QObject>
#include <QList>
//=================================================================
class ZItem : public QObject
{
    Q_OBJECT
public:
    enum ItemType {IT_SAMPLE};
    explicit ZItem(ItemType type, QObject* parent);

    ItemType zp_itemType() const;
    ZItem* zp_parentItem() const;
    ZItem* zp_childAt(int index) const;

    int zp_childCount() const;
    bool zp_hasChildren() const;
    int zp_childItemIndex(ZItem* item);

    virtual QString zp_displayString() const = 0;

protected:

    ItemType zv_type;
    QList<ZItem*> zv_childList;

};
//=================================================================
#endif // ZITEM_H
