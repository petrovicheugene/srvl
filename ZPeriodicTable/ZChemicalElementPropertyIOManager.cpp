//======================================================
#include "ZChemicalElementPropertyIOManager.h"
#include "ZAbstractChemicalElementPropertyRepository.h"

#include <QDebug>
#include <QFile>
#include <QFileInfo>
#include <QTextStream>
#include <QXmlStreamReader>
//======================================================
//// STATIC
////======================================================
//QMap<QChar, QString> ZChemicalElementPropertyIOManager::zv_escapeControlCharacterMap
//= ZChemicalElementPropertyIOManager::zh_initEscapeControlCharacterMap();
////======================================================
//QMap<QChar, QString> ZChemicalElementPropertyIOManager::zh_initEscapeControlCharacterMap()
//{
//    QMap<QChar, QString> escapeControlCharacterMap;
//    // non printing
//    escapeControlCharacterMap.insert('\t'	,	"&#09;");
//    escapeControlCharacterMap.insert('\n'	,	"&#10;");
//    escapeControlCharacterMap.insert('\r'	,	"&#13;");
//    escapeControlCharacterMap.insert(' '	,	"&#32;");
//    // printing
//    escapeControlCharacterMap.insert('!'	,	"&#33;");
//    escapeControlCharacterMap.insert('"'	,	"&#34;");
//    escapeControlCharacterMap.insert('#'	,	"&#35;");
//    escapeControlCharacterMap.insert('$'	,	"&#36;");
//    escapeControlCharacterMap.insert('%'	,	"&#37;");
//    escapeControlCharacterMap.insert('&'	,	"&#38;");
//    escapeControlCharacterMap.insert('\''	,	"&#39;");
//    escapeControlCharacterMap.insert('('	,	"&#40;");
//    escapeControlCharacterMap.insert(')'	,	"&#41;");
//    escapeControlCharacterMap.insert('*'	,	"&#42;");
//    escapeControlCharacterMap.insert('+'	,	"&#43;");
//    escapeControlCharacterMap.insert(','	,	"&#44;");
//    escapeControlCharacterMap.insert('-'	,	"&#45;");
//    escapeControlCharacterMap.insert('.'	,	"&#46;");
//    escapeControlCharacterMap.insert('/'	,	"&#47;");
//    escapeControlCharacterMap.insert(':'	,	"&#58;");
//    escapeControlCharacterMap.insert(';'	,	"&#59;");
//    escapeControlCharacterMap.insert('<'	,	"&#60;");
//    escapeControlCharacterMap.insert('='	,	"&#61;");
//    escapeControlCharacterMap.insert('>'	,	"&#62;");
//    escapeControlCharacterMap.insert('?'	,	"&#63;");
//    escapeControlCharacterMap.insert('@'	,	"&#64;");
//    escapeControlCharacterMap.insert('['	,	"&#91;");
//    escapeControlCharacterMap.insert('\\'	,	"&#92;");
//    escapeControlCharacterMap.insert(']'	,	"&#93;");
//    escapeControlCharacterMap.insert('^'	,	"&#94;");
//    escapeControlCharacterMap.insert('_'	,	"&#95;");
//    escapeControlCharacterMap.insert('`'	,	"&#96;");
//    escapeControlCharacterMap.insert('{'	,	"&#123;");
//    escapeControlCharacterMap.insert('|'	,	"&#124;");
//    escapeControlCharacterMap.insert('}'	,	"&#125;");
//    escapeControlCharacterMap.insert('~'	,	"&#126;");

//    return escapeControlCharacterMap;
//}
////======================================================
////END STATIC
//======================================================
ZChemicalElementPropertyIOManager::ZChemicalElementPropertyIOManager(QObject *parent)
    : QObject(parent)
{

}
//======================================================
//ZChemicalElementPropertyIOManager::ZChemicalElementPropertyIOManager(ZAbstractChemicalElementPropertyRepository* repository,
//                                                                     QObject *parent) : QObject(parent)
//{
//}
//======================================================
//bool ZChemicalElementPropertyIOManager::zp_extractFromCSV(const QString& filePath,
//                                                          QString ZNumberColumnName,
//                                                          QString separator)
//{
//    zv_repository->zp_clearData();
//    zv_errorStringList.clear();
//    QFile file(filePath);
//    if(!file.open(QIODevice::ReadOnly))
//    {
//        zv_errorStringList.append(file.errorString());
//        return false;
//    }

//    QTextStream textStream(&file);
//    QString line;
//    // first line should contain property names
//    if(!textStream.readLineInto(&line))
//    {
//        file.close();
//        zv_errorStringList.append(tr("Cannot read header line from file \"&1\"!").arg(filePath));
//        return false;
//    }
//    // create property name list
//    QStringList propertyNameList = line.split(separator);
//    if(propertyNameList.isEmpty())
//    {
//        file.close();
//        zv_errorStringList.append(tr("Property name list in file \"&1\" is empty!").arg(filePath));
//        return false;
//    }

//    // find Znumber column number
//    int ZColumnIndex = propertyNameList.indexOf(ZNumberColumnName);

//    // read properties
//    QStringList propertyStringList;
//    int Z = 0;
//    bool ok;
//    while(textStream.readLineInto(&line))
//    {
//        if(line.isEmpty())
//        {
//            continue;
//        }

//        // ZNumber
//        // if there is no Z column define Z number
//        if(ZColumnIndex < 0)
//        {
//            ++Z;
//        }

//        // read and split property line
//        propertyStringList = line.split(separator);
//        for(int i = 0; i < propertyNameList.count(); i++)
//        {
//            if(i > propertyStringList.count())
//            {
//                continue;
//            }

//            if(i == ZColumnIndex)
//            {
//                Z = propertyStringList.value(i).toInt(&ok);
//                if(!ok)
//                {
//                    ++Z;
//                }
//            }

//            if(!zv_repository->zp_setChemicalElementProperty(Z,
//                                                             propertyNameList.value(i),
//                                                             propertyStringList.value(i)))
//            {
//                file.close();
//                return false;
//            }
//        }
//    }

//    file.close();
//    return true;
//}
//======================================================
bool ZChemicalElementPropertyIOManager::zp_extractFromCSV(ZAbstractChemicalElementPropertyRepository* repository,
                                                          const QString& filePath,
                                                          QString ZNumberColumnName,
                                                          QString separator)
{
    zv_errorStringList.clear();
    if(!repository)
    {
        zv_errorStringList.append(tr("There is no repository to load data."));
        return false;
    }

    repository->zp_clearData();

    QFile file(filePath);
    if(!file.open(QIODevice::ReadOnly))
    {
        zv_errorStringList.append(file.errorString());
        return false;
    }

    QTextStream textStream(&file);
    QString line;
    // first line should contain property names
    if(!textStream.readLineInto(&line))
    {
        file.close();
        zv_errorStringList.append(tr("Cannot read header line from file \"&1\"!").arg(filePath));
        return false;
    }
    // create property name list
    QStringList propertyNameList = line.split(separator);
    if(propertyNameList.isEmpty())
    {
        file.close();
        zv_errorStringList.append(tr("Property name list in file \"&1\" is empty!").arg(filePath));
        return false;
    }

    // find Znumber column number
    int ZColumnIndex = propertyNameList.indexOf(ZNumberColumnName);

    // read properties
    QStringList propertyStringList;
    int Z = 0;
    bool ok;
    while(textStream.readLineInto(&line))
    {
        if(line.isEmpty())
        {
            continue;
        }

        // ZNumber
        // if there is no Z column define Z number
        if(ZColumnIndex < 0)
        {
            ++Z;
        }

        // read and split property line
        propertyStringList = line.split(separator);
        for(int i = 0; i < propertyNameList.count(); i++)
        {
            if(i > propertyStringList.count())
            {
                continue;
            }

            if(i == ZColumnIndex)
            {
                Z = propertyStringList.value(i).toInt(&ok);
                if(!ok)
                {
                    ++Z;
                }
            }

            if(!repository->zp_setChemicalElementProperty(Z,
                                                          propertyNameList.value(i),
                                                          propertyStringList.value(i)))
            {
                file.close();
                return false;
            }
        }
    }

    file.close();
    return true;

}
//======================================================
bool ZChemicalElementPropertyIOManager::zp_extractFromXML(ZAbstractChemicalElementPropertyRepository* repository,
                                                          const QString& filePath)
{
    zv_errorStringList.clear();
    if(!repository)
    {
        zv_errorStringList.append(tr("There is no repository to load data."));
        return false;
    }

    repository->zp_clearData();
    if(!repository)
    {
        zv_errorStringList.append(tr("There is no chemical element repository to which load data."));
        return false;
    }

    QFile file(filePath);
    if(!file.open(QIODevice::ReadOnly))
    {
        zv_errorStringList.append(tr("Cannot open file \"%1\" in read mode.").arg(filePath));
        return false;
    }

    //repository->zp_clearData();
    QXmlStreamReader reader(&file);

    if(!zh_detectRoot(reader))
    {
        zv_errorStringList.append(tr("Cannot recognize file \"%1\" as data source.<br>"
                                     "Root type is invalid").arg(filePath));
        file.close();
        return false;
    }

    QStringList sectionBranch;
    QString propertyName;
    QStack<QString> tokenNameStack;
    // after zh_detectRoot reader is inside the root section
    tokenNameStack.push(zv_ROOT);
    while(!reader.atEnd())
    {
        reader.readNext();
        if(!zh_parseXmlElement(repository, reader, sectionBranch, propertyName, tokenNameStack))
        {
            file.close();
            return false;
        }
    }

    file.close();
    return true;
}
//======================================================
bool ZChemicalElementPropertyIOManager::zh_parseXmlElement(ZAbstractChemicalElementPropertyRepository* repository,
                                                           QXmlStreamReader& reader,
                                                           QStringList& sectionBranch,
                                                           QString& propertyName,
                                                           QStack<QString>& tokenNameStack) const
{
    if(reader.tokenType() == QXmlStreamReader::StartElement)
    {
        tokenNameStack.push(reader.name().toString());
        if(reader.name().toString() == zv_SECTION)
        {
            sectionBranch.append(reader.attributes().value(zv_NAME).toString());

            bool ok;
            int ZNumber = sectionBranch.first().toInt(&ok);
            if(!ok)
            {
                return false;
            }

            QStringList branch = sectionBranch;
            branch.removeFirst();
            if(!branch.isEmpty())
            {
                if(!repository->zp_addChemicalElementPropertyBranch(ZNumber, branch))
                {
                    return false;
                }
            }
        }
        else if(reader.name().toString() == zv_PROPERTY)
        {
            propertyName = reader.attributes().value(zv_NAME).toString();
        }
        return true;
    }

    if(reader.tokenType() == QXmlStreamReader::EndElement)
    {
        if(tokenNameStack.isEmpty())
        {
            // nothing to close
            return false;
        }

        if(tokenNameStack.last() == zv_SECTION) // it was section
        {
            sectionBranch.removeLast();
        }
        else  if(tokenNameStack.last() == zv_PROPERTY) // it was property
        {
            // reset property name
            propertyName.clear();
        }
        else // it was smth else
        {
            // reset property name
            propertyName.clear();
        }

        tokenNameStack.pop();
        return true;
    }

    if(reader.tokenType() == QXmlStreamReader::Characters)
    {
        // if was property
        // create property
        if(!tokenNameStack.isEmpty() && tokenNameStack.last() == zv_PROPERTY && !propertyName.isEmpty())
        {
            if(reader.text().toString().simplified().isEmpty())
            {
                return true;
            }
            else
            {
                // create property
                bool ok;
                int ZNumber = sectionBranch.first().toInt(&ok);
                if(!ok)
                {
                    return false;
                }
                QStringList branch = sectionBranch;
                branch.removeFirst();
                PropertyList propertyList;
                propertyList.append(QPair<QString, QString>(propertyName, reader.text().toString()));
                if(!repository->zp_setChemicalElementProperties(ZNumber, branch, propertyList))
                {
                    return false;
                }
            }
        }

        return true;
    }

    return true;
}
//======================================================
bool ZChemicalElementPropertyIOManager::zh_detectRoot(QXmlStreamReader& reader) const
{
    while(!reader.atEnd())
    {
        reader.readNext();
        if(reader.tokenType() != QXmlStreamReader::StartElement ||
                reader.name() != zv_ROOT)
        {
            continue;
        }

        QString val = reader.attributes().value(zv_TYPE).toString();
        if(reader.attributes().value(zv_TYPE).toString() != zv_magicString)
        {
            continue;
        }

        return true;
    }

    return false;
}
//======================================================
bool ZChemicalElementPropertyIOManager::zp_saveToXML(ZAbstractChemicalElementPropertyRepository* repository,
                                                     const QString& filePath)
{
    zv_errorStringList.clear();
    if(!repository)
    {
        zv_errorStringList.append(tr("There is no chemical element repository from which save data."));
        return false;
    }

    QFile file(filePath);
    if(!file.open(QIODevice::WriteOnly))
    {
        zv_errorStringList.append(tr("Cannot open file \"%1\" in write mode.").arg(filePath));
        return false;
    }

    QXmlStreamWriter writer(&file);
    writer.setAutoFormatting(true);
    // head
    writer.writeStartDocument();
    writer.writeStartElement(zv_ROOT);
    writer.writeAttribute(zv_TYPE, zv_magicString);

    // chemical element property data
    QStringList sectionList;
    if(!repository->zp_chemicalElementPropertySectionList(-1,
                                                          QStringList(),
                                                          sectionList))
    {
        // error
        zv_errorStringList.append(repository->zp_lastError());

        writer.writeEndElement(); // root
        writer.writeEndDocument();
        file.close();

        return false;
    }

    QStringList propertySectionBranch;
    for(int element = 0; element < sectionList.count(); element++)
    {
        propertySectionBranch.clear();
        propertySectionBranch.append(sectionList.at(element));
        zh_writeXMLElement(repository, &writer, -1, propertySectionBranch);
    }

    writer.writeEndElement(); // root
    writer.writeEndDocument();

    bool res = file.error() == QFile::NoError;
    if(!res)
    {
        zv_errorStringList.append(file.errorString());
    }

    file.close();
    return res;
}
//======================================================
bool ZChemicalElementPropertyIOManager::zp_extractFromFile(ZAbstractChemicalElementPropertyRepository* repository,
                                                           const QString& filePath)
{
    zv_errorStringList.clear();
    if(!repository)
    {
        zv_errorStringList.append(tr("There is no repository to extract data."));
        return false;
    }

    if(filePath.isEmpty())
    {
        zv_errorStringList.append("Data file path id empty");
        return false;
    }

    QFileInfo fileInfo(filePath);
    if(fileInfo.suffix() == "csv")
    {
        return zp_extractFromCSV(repository, filePath);
    }
    else
    {
        return zp_extractFromXML(repository, filePath);
    }
}
//======================================================
QStringList ZChemicalElementPropertyIOManager::zp_lastErrors() const
{
    return zv_errorStringList;
}
//======================================================
void ZChemicalElementPropertyIOManager::zh_writeXMLElement(ZAbstractChemicalElementPropertyRepository* repository,
                                                           QXmlStreamWriter* writer,
                                                           int z,
                                                           QStringList &propertySectionBranch)
{
    // start section
    writer->writeStartElement(zv_SECTION);
    if(!propertySectionBranch.isEmpty())
    {
        writer->writeAttribute(zv_NAME, (propertySectionBranch.last()));
    }

    // chemical element properties
    PropertyList propertyList;
    if(!repository->zp_chemicalElementProperties(z,
                                                 propertySectionBranch,
                                                 propertyList))
    {
        // error
        zv_errorStringList.append(repository->zp_lastError());
    }
    else
    {
        foreach(auto& property, propertyList)
        {
            writer->writeStartElement(zv_PROPERTY);
            writer->writeAttribute(zv_NAME, property.first);
            writer->writeCharacters(property.second);
            writer->writeEndElement(); // property
        }
    }

    // property sections
    QStringList sectionList;
    QStringList childPropertySectionBranch;
    if(!repository->zp_chemicalElementPropertySectionList(z,
                                                          propertySectionBranch,
                                                          sectionList))
    {
        // error
        zv_errorStringList.append(repository->zp_lastError());
    }
    else
    {
        foreach(auto& section, sectionList)
        {
            childPropertySectionBranch =  propertySectionBranch;
            childPropertySectionBranch.append(section);

            zh_writeXMLElement(repository,
                               writer,
                               z,
                               childPropertySectionBranch);
        }
    }

    // end section
    writer->writeEndElement(); // chemical element
}
//======================================================
//QString ZChemicalElementPropertyIOManager::zh_escapeControlCharacters(const QString& srcString) const
//{
//    QString outString = srcString;
//    QList<QChar> controlCharacterList = zv_escapeControlCharacterMap.keys();
//    for(int i = outString.count() - 1; i >= 0; i--)
//    {
//        if(controlCharacterList.contains(outString.at(i)))
//        {
//            outString.replace(i, 1, zv_escapeControlCharacterMap.value(outString.at(i), outString.at(i)));
//        }
//    }

//    return outString;
//}
////======================================================
//QString ZChemicalElementPropertyIOManager::zh_removeWordSeparator(const QString& srcString) const
//{
//    QString outString = srcString;
//    return outString;
//}
//======================================================

