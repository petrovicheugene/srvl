//======================================================
#ifndef ZCHEMICALELEMENTPROPERTYIOMANAGER_H
#define ZCHEMICALELEMENTPROPERTYIOMANAGER_H
//======================================================
#include <QApplication>
#include <QObject>
#include <QList>
#include <QMap>
#include <QPair>
#include <QStack>
#include <QXmlStreamReader>
#include <QXmlStreamWriter>
//======================================================
class ZAbstractChemicalElementPropertyRepository;
//======================================================
class ZChemicalElementPropertyIOManager : public QObject
{
    Q_OBJECT
public:
    explicit ZChemicalElementPropertyIOManager(QObject *parent = nullptr);
//    explicit ZChemicalElementPropertyIOManager(ZAbstractChemicalElementPropertyRepository* repository,
//                                               QObject *parent = nullptr);

//    bool zp_extractFromCSV(const QString& filePath,
//                           QString ZNumberColumnName = "Z",
//                           QString separator = ";");

    bool zp_extractFromCSV(ZAbstractChemicalElementPropertyRepository* repository,
                           const QString& filePath,
                           QString ZNumberColumnName = "Z",
                           QString separator = ";");

    bool zp_extractFromXML(ZAbstractChemicalElementPropertyRepository* repository,
                           const QString& filePath);


    bool zp_saveToXML(ZAbstractChemicalElementPropertyRepository* repository,
                      const QString& filePath);

    bool zp_extractFromFile(ZAbstractChemicalElementPropertyRepository* repository,
                            const QString& filePath);

    QStringList zp_lastErrors() const;

signals:

public slots:

private:

    // VARS
    // static QMap<QChar, QString> zv_escapeControlCharacterMap;

    QStringList zv_errorStringList;

    const QString zv_magicString = qApp->property("glAppCompany").toString() + " GWSOT76D429YWFP";
    const QString zv_ROOT = "root";
    const QString zv_TYPE = "type";
    const QString zv_SECTION = "section";
    const QString zv_PROPERTY = "property";
    const QString zv_NAME = "name";

    // FUNCS
    // static QMap<QChar, QString> zh_initEscapeControlCharacterMap();
    void zh_writeXMLElement(ZAbstractChemicalElementPropertyRepository* repository,
                            QXmlStreamWriter *writer,
                            int z,
                            QStringList& propertySectionBranch);

    bool zh_detectRoot(QXmlStreamReader &reader) const;
    bool zh_parseXmlElement(ZAbstractChemicalElementPropertyRepository* repository,
                            QXmlStreamReader& reader,
                            QStringList& sectionBranch,
                            QString &propertyName,
                            QStack<QString>& tokenNameStack) const;

//    QString zh_escapeControlCharacters(const QString& srcString) const;
//    QString zh_removeWordSeparator(const QString& srcString) const;

};
//======================================================
#endif // ZCHEMICALELEMENTPROPERTYIOMANAGER_H
