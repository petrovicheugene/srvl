//============================================================
#include "ZXMLSpectrumArrayIOHandler.h"
#include <QXmlStreamReader>
#include <QFileInfo>
//============================================================
ZXMLSpectrumArrayIOHandler::ZXMLSpectrumArrayIOHandler(QObject *parent) : QObject(parent)
{

}
//============================================================
ZXMLSpectrumArrayIOHandler::~ZXMLSpectrumArrayIOHandler()
{

}
//============================================================
bool ZXMLSpectrumArrayIOHandler::zp_readSpectrumArray (QFile& file, QList<ZRawSpectrumArray>& rawArrayList) const
{
    if(!(file.openMode() & QIODevice::ReadOnly))
    {
        QString errorMsg = tr("File \"%1\" is not open in read mode!").arg(file.fileName());
        emit zg_message(errorMsg);
        return false;
    }

    // root checking
    QXmlStreamReader reader(&file);
    bool rootDetectedFlag = false;
    bool magicStringDetectionFlag = false;
    //bool parsingErrorFalg = false;

    while(!reader.atEnd())
    {
        reader.readNext();
        // property root element detection section
        if(!rootDetectedFlag)
        {
            if(!zh_detectRoot(reader, magicStringDetectionFlag))
            {
                continue;
            }

            if(!magicStringDetectionFlag)
            {
                QString errorMsg = tr("File \"%1\" is not recognized!").arg(file.fileName());
                emit zg_message(errorMsg);
                return false;
            }
            rootDetectedFlag = true;
        }

        // root text data handling section

        if(reader.isCharacters())
        {
            if(reader.text().toString().simplified().isEmpty())
            {
                continue;
            }
        }
        else if(reader.isStartElement())
        {
            zh_parseXMLElement(rawArrayList, -1, -1, reader);
        }
        else if(reader.isEndElement())
        {
            // end of root level
        }
    }

    if(reader.hasError())
    {
        QString errorMsg = tr("File \"%1\" parsing failed! %2").arg(file.fileName(), reader.errorString());
        emit zg_message(errorMsg);
        return false;
    }
    else if(file.error() != QFile::NoError)
    {
        QString errorMsg = tr("Cannot read file \"%1\"! %2").arg(file.fileName(), file.errorString());
        emit zg_message(errorMsg);
        return false;
    }

    return true;
}
//============================================================
// returns true if closing tag opened in calling function is readed
void ZXMLSpectrumArrayIOHandler::zh_parseXMLElement(QList<ZRawSpectrumArray> &array,
                                                    int currentArrayIndex,
                                                    int currentSpectrumIndex,
                                                    QXmlStreamReader& reader) const
{
    QString currentChemElement = QString();
    // bool insertedElementOpened = false;
    // current start tag handling
    // attributes handling
    QString currentTagName = reader.name().toString();
#ifdef DBG
    qDebug() << "OPEN TAG" << currentTagName;
#endif

    if(currentTagName == zv_ARRAY)
    {
        ZRawSpectrumArray rawArray;
        rawArray.name = reader.attributes().value(zv_NAME).toString();
        array.append(rawArray);
        currentArrayIndex = array.count() - 1;
        //   array.zp_setArrayName(reader.attributes().value(zv_NAME).toString());
    }
    else if(currentTagName == zv_GAIN_FACTOR)
    {
        if(currentArrayIndex >= 0 && currentArrayIndex < array.count())
        {
            // read gainfactor value
            if(!reader.atEnd())
            {
                reader.readNext();
            }
            QString gainFactorString = reader.text().toString();
            bool ok;
            int gainFactor = gainFactorString.toInt(&ok);
            if(!ok)
            {
                gainFactor = 0;
            }
            array[currentArrayIndex].gainFactor = gainFactor;
        }
    }
    else if(currentTagName == zv_SPECTRUM)
    {
        if(currentArrayIndex >= 0 && currentArrayIndex < array.count())
        {
            ZRawSpectrum rawSpectrum;
            rawSpectrum.path = reader.attributes().value(zv_PATH).toString();
            array[currentArrayIndex].spectrumList.append(rawSpectrum);
            currentSpectrumIndex = array[currentArrayIndex].spectrumList.count() - 1;
        }
    }
    else if(currentTagName == zv_CONCENTRATION)
    {
        if(currentArrayIndex >= 0 && currentArrayIndex < array.count() &&
                currentSpectrumIndex >= 0 && currentSpectrumIndex < array.value(currentArrayIndex).spectrumList.count())
        {
            currentChemElement = reader.attributes().value(zv_NAME).toString();
            array[currentArrayIndex].spectrumList[currentSpectrumIndex].
                    concentrationMap.insert(currentChemElement, QString());
        }
    }

    while(!reader.atEnd())
    {
        reader.readNext();
        if(reader.isStartElement())
        {
            // insertedElementOpened = true;
            zh_parseXMLElement(array, currentArrayIndex, currentSpectrumIndex, reader);
            continue;
//            if(closingTagReadedFlag)
//            {

//                // continue;
//                return false;
//            }
        }
        else if(reader.isCharacters())
        {
            if(reader.text().toString().simplified().isEmpty())
            {
                continue;
            }

            if(currentTagName == zv_CONCENTRATION)
            {
                QString concentration = reader.text().toString();
                array[currentArrayIndex].spectrumList[currentSpectrumIndex].
                        concentrationMap[currentChemElement] = concentration;
            }
        }
        else if(reader.isEndElement())
        {
            return;
            // return insertedElementOpened;
        }
    }
}
//============================================================
bool ZXMLSpectrumArrayIOHandler::zp_writeSpectrumArray(QFile& file, const QList<ZRawSpectrumArray>& rawArrayList) const
{
    if(!(file.openMode() & QIODevice::WriteOnly))
    {
        QString errorMsg = tr("File \"%1\" is not open in write mode!").arg(file.fileName());
        emit zg_message(errorMsg);
        return false;
    }

    QXmlStreamWriter writer(&file);
    writer.setAutoFormatting(true);
    // head
    writer.writeStartDocument();
    // root
    writer.writeStartElement(zv_ROOT);
    writer.writeAttribute(zv_TYPE, zv_magicString);
    // data

    for(int a = 0; a < rawArrayList.count(); a++)
    {
        writer.writeStartElement(zv_ARRAY);
        writer.writeAttribute(zv_NAME, rawArrayList.at(a).name);

        writer.writeStartElement(zv_GAIN_FACTOR);
        writer.writeCharacters(QString::number(rawArrayList.at(a).gainFactor));
        writer.writeEndElement(); // gain factor

        for(int s = 0; s < rawArrayList.at(a).spectrumList.count(); s++)
        {
            writer.writeStartElement(zv_SPECTRUM);
            writer.writeAttribute(zv_PATH, rawArrayList.value(a).spectrumList.value(s).path);

            QMap<QString, QString>::const_iterator it;

            for(it = rawArrayList[a].spectrumList[s].concentrationMap.begin();
                it != rawArrayList[a].spectrumList[s].concentrationMap.end(); it++ )
            {
                writer.writeStartElement(zv_CONCENTRATION);
                writer.writeAttribute(zv_NAME, it.key());
                writer.writeCharacters(it.value());
                writer.writeEndElement(); // concentration
            }
             writer.writeEndElement(); // spectrum
        }
          writer.writeEndElement(); // array
    }

    writer.writeEndElement(); // root
    writer.writeEndDocument();

    if(file.error() != QFile::NoError)
    {
        QString errorMsg = tr("Cannot write to file \"%1\"! %2").arg(file.fileName(), file.errorString());
        emit zg_message(errorMsg);
        return false;
    }

    return true;
}
//============================================================
bool ZXMLSpectrumArrayIOHandler::zh_detectRoot(const QXmlStreamReader& reader, bool& magicStringDetectionFlag) const
{
    // true - element name == root, error - trnado magic string existing flag
    if(reader.tokenType() != QXmlStreamReader::StartElement ||
            reader.name() != zv_ROOT)
    {
        return false;
    }

    if(reader.attributes().value(zv_TYPE).toString() == zv_magicString)
    {
        magicStringDetectionFlag = true;
    }
    else
    {
        magicStringDetectionFlag = false;
    }

    return true;
}

//============================================================
bool ZXMLSpectrumArrayIOHandler::zh_checkfilePath(const QString& path) const
{
    QFileInfo fileInfo(path);
    if((!fileInfo.exists()) || (!fileInfo.isFile()))
    {
        return false;
    }

    return true;
}
//============================================================
