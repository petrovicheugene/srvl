//======================================================
#ifndef ZABSTRACTCHEMICALELEMENTPROPERTYREPOSITORY_H
#define ZABSTRACTCHEMICALELEMENTPROPERTYREPOSITORY_H
//======================================================
#include <QList>
#include <QPair>
#include <QString>
//======================================================
typedef QPair<QString, QString> PropertyPair;
typedef QList<QPair<QString, QString> > PropertyList;
//======================================================
class ZAbstractChemicalElementPropertyRepository
{
public:
    // friend class ZChemicalElementPropertyIOManager;
    ZAbstractChemicalElementPropertyRepository();
    virtual ~ZAbstractChemicalElementPropertyRepository();

    virtual bool zp_setChemicalElementProperty(int ZNumber,
                                               const QString& key,
                                               const QString& value) = 0;

    virtual bool zp_setChemicalElementProperties(int ZNumber,
                                                 const QStringList& propertySectionBranch,
                                                 const PropertyList& propertyList) = 0;


    virtual bool zp_addChemicalElementPropertyBranch(int ZNumber,
                                                     const QStringList& propertySectionBranch) = 0;


    virtual bool zp_chemicalElementProperties(int ZNumber,
                                              const QStringList& propertySectionBranch,
                                              PropertyList& propertyList) = 0;

    virtual bool zp_chemicalElementPropertySectionList(int ZNumber,
                                                       const QStringList& propertySectionBranch,
                                                       QStringList& sectionList) = 0;

    int zp_chemicalElementCount() const;
    QString zp_lastError() const;

    virtual void zp_clearData() = 0;

protected:

    // VARS
    int zv_chemicalElementCount;
    QString zv_errorMsg;
};
//======================================================
#endif // ZABSTRACTCHEMICALELEMENTPROPERTYREPOSITORY_H
