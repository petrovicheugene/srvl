//======================================================
#ifndef ZCHEMICALELEMENTPROPERTYTREEMODEL_H
#define ZCHEMICALELEMENTPROPERTYTREEMODEL_H
//======================================================
#include <QAbstractItemModel>
#include "ZAbstractChemicalElementPropertyRepository.h"
//======================================================
class ZChemicalElementPropertyItem;
class QSettings;
typedef QList<QStringList> PropertyNames;
//======================================================
const int ZNumberRole = Qt::UserRole + 1;
const int SymbolRole = Qt::UserRole + 2;
const int NameRole = Qt::UserRole + 3;
const int PropertyCountRole = Qt::UserRole + 4;
const int PropertyFirstColumnRole = Qt::UserRole + 5;
const int PropertyNameRole = Qt::UserRole + 6;
const int PropertyValueRole = Qt::UserRole + 7;

//======================================================
class ZChemicalElementPropertyTreeModel : public QAbstractItemModel,
        public ZAbstractChemicalElementPropertyRepository
{
    Q_OBJECT
public:
    explicit ZChemicalElementPropertyTreeModel(QObject *parent = nullptr);
    ~ZChemicalElementPropertyTreeModel();

    // reimplemented FUNCS
    Qt::ItemFlags	flags(const QModelIndex & index) const override;
    QModelIndex index(int row, int column, const QModelIndex &parent) const override;
    int	columnCount(const QModelIndex & parent = QModelIndex()) const override;
    int	rowCount(const QModelIndex & parent = QModelIndex()) const override;
    QVariant	data(const QModelIndex & index, int role = Qt::DisplayRole) const override;
    bool setData(const QModelIndex & index, const QVariant & value, int role = Qt::EditRole) override;
    QVariant	headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;
    QModelIndex parent(const QModelIndex& child) const override;
    bool hasChildren(const QModelIndex &parent = QModelIndex()) const override;


    bool zp_setChemicalElementProperty(int ZNumber,
                                      const QString& key,
                                      const QString& value) override;

    bool zp_setChemicalElementProperties(int ZNumber,
                                           const QStringList& propertySectionBranch,
                                           const PropertyList& propertyList) override;

    bool zp_addChemicalElementPropertyBranch(int ZNumber,
                                            const QStringList& propertySectionBranch) override;

    bool zp_chemicalElementPropertySectionBranch(const QModelIndex &index, QStringList &branch);

    bool zp_removeChemicalElementProperty(int ZNumber,
                                          const QStringList& propertySectionBranch,
                                          int propertyIndex);
    bool zp_chemicalElementProperties(int ZNumber,
                                              const QStringList& propertySectionBranch,
                                              PropertyList& propertyList) override;

    bool zp_chemicalElementPropertySectionList(int ZNumber,
                                                       const QStringList& propertySectionBranch,
                                                       QStringList& sectionList) override;
    void zp_clearData() override;
    QStringList zp_lastErrorStringList() const;


    bool zp_zNumberForIndex(const QModelIndex& index, int& zNumber) const;
    QString zp_zNumberPropertyName() const;
    void zp_setZNumberPropertyName(const QString& name);
    QString zp_symbolPropertyName() const;
    void zp_setSymbolPropertyName(const QString& name);
    QString zp_namePropertyName() const;
    void zp_setNamePropertyName(const QString& name);

    int zp_chemicalElementCount() const;
    bool zp_isRemovable(const QModelIndex& index) const;
    void zp_removeItems(const QModelIndexList& indexList);

    bool zp_isDirty() const;
    bool zp_saveDataToXMLFile(const QString& filePath);

    QString zp_propertiesSourceFilePath() const;
    bool zp_setPropertiesSourceFileName(const QString& propertiesFileName);

    bool zp_loadDataFromFile(const QString& filePath);

    QModelIndex zp_indexForSection(int ZNumber, const QStringList& sectionBranch) const;

signals:

    void zg_propertiesSourceFilePathChanged(const QString& fileName);

public slots:

protected:
    bool zh_setCemicalElementProperty(int ZNumber,
                                      const QString& key,
                                      const QString& value);

    bool zh_setCemicalElementProperties(int ZNumber,
                                           const QStringList& propertySectionBranch,
                                           const PropertyList& propertyList);

    bool zh_addCemicalElementPropertyBranch(int ZNumber,
                                            const QStringList& propertySectionBranch);

private slots:

    void zh_onItemAboutToBeInserted(int row, const ZChemicalElementPropertyItem* parentItem);
    void zh_onItemInsertion(const ZChemicalElementPropertyItem* item);
    void zh_notifyItemAboutToBeRemoved(const ZChemicalElementPropertyItem *item);
    void zh_notifyItemRemoved(const ZChemicalElementPropertyItem* item);
    void zh_notifyItemIsChanged(const ZChemicalElementPropertyItem *item);

    void zh_notifyItemPropertyNameIsChanged(const ZChemicalElementPropertyItem *item);
    void zh_notifyItemPropertyValueIsChanged(const ZChemicalElementPropertyItem *item);

    void zh_itemRemovingPermission(const ZChemicalElementPropertyItem* item,
                                   bool& permissionToRemove);

private:

    // VARS
    QStringList zv_errorStringList;
    ZChemicalElementPropertyItem* zv_root;
    const int zv_chemicalElementCount = 118;
    const QString zv_defaultPropertiesSourceFileName = ":/files/chemicalElementBasicProperties.xml";
    PropertyNames zv_propertyNames;

    QString zv_zNumberPropertyName;
    QString zv_symbolPropertyName;
    QString zv_namePropertyName;

    QString zv_settingsGroupName;
    const QString zv_propertyFilePathSettingsName = "propertyFilePath";
    bool zv_dirty;

    // FUNCS
    void zh_createComponents();
    void zh_createConnections();
    void zh_loadBasicChemicalElementProperties();

    ZChemicalElementPropertyItem* zh_itemForIndex(const QModelIndex &index) const;
    QModelIndex zh_indexForItem(const ZChemicalElementPropertyItem* item) const;
    ZChemicalElementPropertyItem* zh_itemForZNumber(int ZNumber) const;
    void zh_redefinePropertyNameList();
    bool zh_firstPropertyColumnForItem(ZChemicalElementPropertyItem* item,
                                      int& columnNumber) const;

    void zh_defineChemicalElementItemNames(const QString &nameTemplate, QString separator = " ");
    int zh_propertyNameCount(const PropertyNames& propertyNames) const;

    bool zh_openDialogSettingsGroup(QSettings* settings) const;
    bool zh_closeDialogSettingsGroup(QSettings* settings) const;

};
//======================================================
#endif // ZCHEMICALELEMENTPROPERTYTREEMODEL_H
