//==========================================================
#include "ZXMLCalibrationIOHandler.h"
#include "ZCalibration.h"
#include <QFileInfo>
#include <QFileDialog>
#include <QStandardPaths>
#include <QXmlStreamReader>
#include <QDateTime>
//==========================================================
ZXMLCalibrationIOHandler::ZXMLCalibrationIOHandler(QObject *parent)
    : QObject(parent)
{
    // zv_calibrationParent = calibrationParent;
}
//==========================================================
ZXMLCalibrationIOHandler::~ZXMLCalibrationIOHandler()
{

}
//==========================================================
QString ZXMLCalibrationIOHandler::zp_getCalibrationOpenFile(const QString& calibrationFolderPath)
{
    QString locationDirString = zp_checkDirPath(calibrationFolderPath);
    QString fileName = QFileDialog::getOpenFileName(0, tr("Select file to open"),
                                                    locationDirString,
                                                    tr("XML Calibration files(*.clbx);;XML files(*.xml);;All files(*.*)"));
    return fileName;
}
//==========================================================
QStringList ZXMLCalibrationIOHandler::zp_getCalibrationOpenFiles(const QString& calibrationFolderPath)
{
    QString locationDirString = zp_checkDirPath(calibrationFolderPath);
    QStringList fileNames = QFileDialog::getOpenFileNames(0, tr("Open file"),
                                                          locationDirString,
                                                          tr("XML Calibration files(*.clbx);;XML files(*.xml);;All files(*.*)"));
    return fileNames;
}
//==========================================================
QString ZXMLCalibrationIOHandler::zp_getCalibrationSaveFile(const QString& calibrationFilePath)
{
    QString fileName = QFileDialog::getSaveFileName(0, tr("Save file"),
                                                    calibrationFilePath,
                                                    tr("XML Calibration files(*.clbx);;XML files(*.xml);;All files(*.*)"));
    return fileName;
}
//==========================================================
QString ZXMLCalibrationIOHandler::zp_checkDirPath(const QString& calibrationFolderPath)
{
    QString locationDirString = calibrationFolderPath;
    QFileInfo fileInfo(locationDirString);
    if(!fileInfo.exists() || !fileInfo.isDir())
    {
        QStringList docLocations = QStandardPaths::standardLocations(QStandardPaths::DocumentsLocation);
        if(docLocations.isEmpty())
        {
            locationDirString = QDir::currentPath();
        }
        else
        {
            locationDirString = docLocations.value(0);
        }
    }

    return locationDirString;
}
//==========================================================
bool ZXMLCalibrationIOHandler::zp_writeCalibrationToFile(QFile& file, const ZCalibration* calibration)
{
    if(!(file.openMode() & QIODevice::WriteOnly))
    {
        zv_message = tr("File \"%1\" is not open in write mode!").arg(file.fileName());
        emit zg_message(zv_message);
        return false;
    }

    // at start position
    file.seek(0);

    QXmlStreamWriter writer(&file);
    writer.setAutoFormatting(true);
    // head
    writer.writeStartDocument();
    writer.writeStartElement(zv_ROOT);
    writer.writeAttribute(zv_TYPE, zv_magicString);

    // date
    writer.writeStartElement(zv_DATE_TIME);
    writer.writeCharacters(calibration->zp_dateTime().toString(zv_dateTimeFormat));
    writer.writeEndElement(); // date-time

    // chem element
    writer.writeStartElement(zv_CHEMELEMENT);
    writer.writeCharacters(calibration->zp_chemElement());
    writer.writeEndElement(); // chem element

//    // Energy calibration
//    writer.writeStartElement(zv_ENERGY_K0);
//    writer.writeCharacters(QString::number(calibration->zp_energyCalibrationK0()));
//    writer.writeEndElement(); // k0

//    writer.writeStartElement(zv_ENERGY_K1);
//    writer.writeCharacters(QString::number(calibration->zp_energyCalibrationK1()));
//    writer.writeEndElement(); // k1

//    writer.writeStartElement(zv_ENERGY_K2);
//    writer.writeCharacters(QString::number(calibration->zp_energyCalibrationK2()));
//    writer.writeEndElement(); // k2

    // gain factor
    writer.writeStartElement(zv_GAIN_FACTOR);
    writer.writeCharacters(QString::number(calibration->zp_gainFactor()));
    writer.writeEndElement(); // gainFactor

//    // energy unit
//    writer.writeStartElement(zv_ENERGY_UNIT);
//    writer.writeCharacters(calibration->zp_energyUnit());
//    writer.writeEndElement(); // energy unit

    // exposition
    writer.writeStartElement(zv_EXPOSITION);
    writer.writeCharacters(QString::number(calibration->zp_exposition()));
    writer.writeEndElement(); // exposition

    // Quality data
    // R2
    writer.writeStartElement(zv_DETERMINATION_R2);
    writer.writeCharacters(calibration->zp_calibrationQualityData().determination);
    writer.writeEndElement(); // R2

    // R2adj
    writer.writeStartElement(zv_ADJUSTED_DETERMINATION_R2ADJ);
    writer.writeCharacters(calibration->zp_calibrationQualityData().adj_determination);
    writer.writeEndElement(); // R2adj

    // standard deviation s
    writer.writeStartElement(zv_STANDARD_DEVIATION);
    writer.writeCharacters(calibration->zp_calibrationQualityData().standardDeviation);
    writer.writeEndElement(); // s

    // EQUATION DATA
    // windows
    writer.writeStartElement(zv_WINDOW_LIST);
    for(int w = 0; w < calibration->zp_calibrationWindowCount(); w++)
    {
        // start window + name
        writer.writeStartElement(zv_WINDOW);
        writer.writeAttribute(zv_NAME, calibration->zp_calibrationWindowName(w));
        // window type
        writer.writeStartElement(zv_TYPE);
        writer.writeCharacters(ZCalibrationWindow::zp_typeName(calibration->zp_calibrationWindowType(w)));
        writer.writeEndElement(); // type
        // first channel
        writer.writeStartElement(zv_FIRST_CHANNEL);
        writer.writeCharacters(QString::number(calibration->zp_calibrationWindowFirstChannel(w)));
        writer.writeEndElement(); // first channel
        // last channel
        writer.writeStartElement(zv_LAST_CHANNEL);
        writer.writeCharacters(QString::number(calibration->zp_calibrationWindowLastChannel(w)));
        writer.writeEndElement(); // last channel

        writer.writeEndElement(); // window
    }
    writer.writeEndElement(); // windows

    // terms
    writer.writeStartElement(zv_TERM_LIST);
    for(int t = 0; t < calibration->zp_termCount(); t++)
    {
        // start term + name
        writer.writeStartElement(zv_TERM);
        writer.writeAttribute(zv_NAME, calibration->zp_termName(t));
        // term type
        writer.writeStartElement(zv_TYPE);
        writer.writeCharacters(ZAbstractTerm::zp_termTypeName(calibration->zp_termType(t)));
        writer.writeEndElement(); // type
        // term state
        writer.writeStartElement(zv_STATE);
        writer.writeCharacters(ZAbstractTerm::zp_termStateName(calibration->zp_termState(t)));
        writer.writeEndElement(); // term state

        // term custom string
        if(calibration->zp_termType(t) == ZAbstractTerm::TT_CUSTOM)
        {
            // TODO write Term Custom string
        }

        // term windows
        QStringList windowList = calibration->zp_termWindowsNameList(t);
        foreach(QString window, windowList)
        {
            writer.writeStartElement(zv_TERM_WINDOW);
            writer.writeCharacters(window);
            writer.writeEndElement(); // term window
        }

        // term factor
        writer.writeStartElement(zv_FACTOR);
        writer.writeCharacters(calibration->zp_termFactorString(t));
        writer.writeEndElement(); // term factor

        // end term
        writer.writeEndElement(); // term
    }
    writer.writeEndElement(); // terms

    // common term normalizer
    writer.writeStartElement(zv_NORMALIZER);
    // type
    writer.writeStartElement(zv_TYPE);
    writer.writeCharacters(ZTermNormalizer::zp_normaTypeString(calibration->zp_normaType()));
    writer.writeEndElement(); // type
    // custom string
    writer.writeStartElement(zv_CUSTOM_STRING);
    writer.writeCharacters(calibration->zp_normaCustomString());
    writer.writeEndElement(); // custom string
    writer.writeEndElement(); // common normalizer

    // equation
    writer.writeStartElement(zv_EQUATION);
    // type
    writer.writeStartElement(zv_TYPE);
    writer.writeCharacters(ZCalibration::zp_equationTypeString(calibration->zp_equationType()));
    writer.writeEndElement(); // type
    // base term
    writer.writeStartElement(zv_BASE_TERM);
    writer.writeCharacters(calibration->zp_baseTermString());
    writer.writeEndElement(); // base term

    // base term normalizer
    writer.writeStartElement(zv_FRACTIONAL_BASE_NORMALIZER);
    // type
    writer.writeStartElement(zv_TYPE);
    writer.writeCharacters(ZTermNormalizer::zp_normaTypeString(calibration->zp_baseTermNormaType()));
    writer.writeEndElement(); // type
    // custom string
    writer.writeStartElement(zv_CUSTOM_STRING);
    writer.writeCharacters(calibration->zp_baseTermNormaCustomString());
    writer.writeEndElement(); // custom string

    writer.writeEndElement(); // base term normalizer

    // intercept
    writer.writeStartElement(zv_INTERCEPT);
    writer.writeCharacters(calibration->zp_equationInterceptString());
    writer.writeEndElement(); // intercept

    writer.writeEndElement(); // equation

    writer.writeEndElement(); // root
    writer.writeEndDocument();

    if(file.error() != QFile::NoError)
    {
        zv_message = tr("Cannot write to file \"%1\"! %2").arg(file.fileName(), file.errorString());
        emit zg_message(zv_message);
        return false;
    }

    return true;
}
//==========================================================
QString ZXMLCalibrationIOHandler::zp_message() const
{
    return zv_message;
}
//==========================================================
bool ZXMLCalibrationIOHandler::zp_getCalibrationXMLByteArrayFromFile(QFile& file,
                                                                     QByteArray& calibrationXMLByteArray)
{
    if(!(file.openMode() & QIODevice::ReadOnly))
    {
        zv_message = tr("File \"%1\" is not open in read mode!").arg(file.fileName());
        emit zg_message(zv_message);
        return false;
    }

    // at start position
    file.seek(0);
    calibrationXMLByteArray = file.readAll();

    return true;
}
//==========================================================
bool ZXMLCalibrationIOHandler::zp_getCalibrationFromByteArray(QByteArray& byteArray,
                                                              ZCalibration* calibration)
{
    return false;
}
//==========================================================
bool ZXMLCalibrationIOHandler::zp_getCalibrationFromString(QString& calibrationString,
                                                           ZCalibration* calibration)
{
    if(calibration == 0 || calibrationString.isEmpty())
    {
        return false;
    }

    QXmlStreamReader reader(calibrationString);

    // root checking
    bool rootDetectedFlag = false;
    bool magicStringDetectionFlag = false;
    //bool parsingErrorFalg = false;
    parentTagStack.clear();
    // handling root level
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
                zv_message = tr("Calibration data is not recognized!");
                emit zg_message(zv_message);
                return false;
            }

            rootDetectedFlag = true;
            //calibration = new ZCalibration(file.fileName(), zv_calibrationParent);
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
            // handling inserted levels
            parentTagStack.push(reader.name().toString());
            zh_parseXMLElement(calibration, reader);
        }
    }

    if(reader.hasError())
    {
        zv_message = tr("Calibration data parsing failed! %1").arg(reader.errorString());
        emit zg_message(zv_message);
        return false;
    }

    calibration->zp_setDirty(false);
    return true;
}
//==========================================================
bool ZXMLCalibrationIOHandler::zp_getCalibrationFromFile(QFile& file,
                                                         ZCalibration* calibration)
{
    if(calibration == nullptr)
    {
        return false;
    }

    if(!(file.openMode() & QIODevice::ReadOnly))
    {
        zv_message = tr("File \"%1\" is not open in read mode!").arg(file.fileName());
        emit zg_message(zv_message);
        return false;
    }

    QXmlStreamReader reader(&file);

    // root checking
    bool rootDetectedFlag = false;
    bool magicStringDetectionFlag = false;
    //bool parsingErrorFalg = false;
    parentTagStack.clear();
    // handling root level
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
                zv_message = tr("File \"%1\" is not recognized!").arg(file.fileName());
                emit zg_message(zv_message);
                return false;
            }

            rootDetectedFlag = true;
            //calibration = new ZCalibration(file.fileName(), zv_calibrationParent);
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
            // handling inserted levels
            parentTagStack.push(reader.name().toString());
            zh_parseXMLElement(calibration, reader);
        }
    }

    if(reader.hasError())
    {
        zv_message = tr("File \"%1\" parsing failed! %2").arg(file.fileName(), reader.errorString());
        emit zg_message(zv_message);
        return false;
    }
    else if(file.error() != QFile::NoError)
    {
        zv_message = tr("Cannot read file \"%1\"! %2").arg(file.fileName(), file.errorString());
        emit zg_message(zv_message);
        return false;
    }

    calibration->zp_setDirty(false);
    return true;
}
//============================================================
//bool ZXMLCalibrationIOHandler::zh_getCalibration(QXmlStreamReader& reader, ZCalibration* calibration)
//{
//    // root checking
//    bool rootDetectedFlag = false;
//    bool magicStringDetectionFlag = false;
//    //bool parsingErrorFalg = false;
//    parentTagStack.clear();
//    // handling root level
//    while(!reader.atEnd())
//    {
//        reader.readNext();
//        // property root element detection section
//        if(!rootDetectedFlag)
//        {
//            if(!zh_detectRoot(reader, magicStringDetectionFlag))
//            {
//                continue;
//            }

//            if(!magicStringDetectionFlag)
//            {
//                zv_message = tr("File \"%1\" is not recognized!").arg(file.fileName());
//                emit zg_message(zv_message);
//                return false;
//            }

//            rootDetectedFlag = true;
//            //calibration = new ZCalibration(file.fileName(), zv_calibrationParent);
//        }

//        // root text data handling section
//        if(reader.isCharacters())
//        {
//            if(reader.text().toString().simplified().isEmpty())
//            {
//                continue;
//            }
//        }
//        else if(reader.isStartElement())
//        {
//            // handling inserted levels
//            parentTagStack.push(reader.name().toString());
//            zh_parseXMLElement(calibration, reader);
//        }
//    }

//    if(reader.hasError())
//    {
//        zv_message = tr("File \"%1\" parsing failed! %2").arg(file.fileName(), reader.errorString());
//        emit zg_message(zv_message);
//        return false;
//    }
//    else if(file.error() != QFile::NoError)
//    {
//        zv_message = tr("Cannot read file \"%1\"! %2").arg(file.fileName(), file.errorString());
//        emit zg_message(zv_message);
//        return false;
//    }

//    calibration->zp_setDirty(false);
//    return true;
//}
//============================================================
// returns true if closing tag that was opened in calling function is closed
void ZXMLCalibrationIOHandler::zh_parseXMLElement(ZCalibration* calibration,
                                                  QXmlStreamReader& reader)
{
    // current start tag handling
    QString currentTagName = reader.name().toString();
    //    parentTagStack.push(currentTagName);
    //    // attributes handling
    if(currentTagName == zv_WINDOW)
    {
        // create new raw window
        zv_rawWindow = ZRawWindow();
        zv_rawWindow.name = reader.attributes().value(zv_NAME).toString();
    }
    else if(currentTagName == zv_TERM)
    {
        // create new raw term
        zv_rawTerm = ZRawTerm();
        zv_rawTerm.name = reader.attributes().value(zv_NAME).toString();
    }

    QString readerText;
    while(!reader.atEnd())
    {
        reader.readNext();

        if(reader.isStartElement())
        {
            parentTagStack.push(currentTagName);
            zh_parseXMLElement(calibration, reader);
            continue;
        }
        else if(reader.isCharacters())
        {
            if(reader.text().toString().simplified().isEmpty())
            {
                continue;
            }

            readerText = reader.text().toString().simplified();
            if(currentTagName == zv_DATE_TIME)
            {
                QString dateTimeString = readerText;
                QDateTime dateTime = QDateTime::fromString(dateTimeString, zv_dateTimeFormat);
                calibration->zp_setDateTime(dateTime);
            }
            else if(currentTagName == zv_CHEMELEMENT)
            {
                calibration->zp_setChemElement(readerText);
            }
//            else if(currentTagName == zv_ENERGY_K0)
//            {
//                bool ok;
//                qreal realValue = readerText.toDouble(&ok);
//                if(!ok)
//                {
//                    realValue = 0;
//                }

//                calibration->zp_setEnergyCalibrationK0(realValue);
//            }
//            else if(currentTagName == zv_ENERGY_K1)
//            {
//                bool ok;
//                qreal realValue = readerText.toDouble(&ok);
//                if(!ok)
//                {
//                    realValue = 0;
//                }

//                calibration->zp_setEnergyCalibrationK1(realValue);
//            }
//            else if(currentTagName == zv_ENERGY_K2)
//            {
//                bool ok;
//                qreal realValue = readerText.toDouble(&ok);
//                if(!ok)
//                {
//                    realValue = 0;
//                }

//                calibration->zp_setEnergyCalibrationK2(realValue);
//            }
            else if(currentTagName == zv_GAIN_FACTOR)
            {
                bool ok;
                int intValue = readerText.toInt(&ok);
                if(!ok)
                {
                    intValue = -1;
                }

                calibration->zp_setGainFactor(intValue);
            }

//            else if(currentTagName == zv_ENERGY_UNIT)
//            {
//                calibration->zp_setEnergyUnit(readerText);
//            }
            else if(currentTagName == zv_EXPOSITION)
            {
                bool ok;
                int intValue = readerText.toInt(&ok);
                if(!ok)
                {
                    intValue = -1;
                }

                calibration->zp_setExposition(intValue);
            }
            else if(currentTagName == zv_DETERMINATION_R2)
            {
                calibration->zp_setDetermination(readerText);
            }
            else if(currentTagName == zv_ADJUSTED_DETERMINATION_R2ADJ)
            {
                calibration->zp_setAdjustedDetermination(readerText);
            }
            else if(currentTagName == zv_STANDARD_DEVIATION)
            {
                calibration->zp_setStandardDeviation(readerText);
            }
            else if(currentTagName == zv_TYPE)
            {

                if(parentTagStack.last() == zv_WINDOW)
                {
                    zv_rawWindow.windowType = ZCalibrationWindow::zp_typeFromString(readerText);
                }
                else if(parentTagStack.last() == zv_TERM)
                {
                    zv_rawTerm.termType = ZAbstractTerm::zp_termTypeFromString(readerText);
                }
                else if(parentTagStack.last() == zv_NORMALIZER)
                {
                    calibration->zp_setNormaType(ZTermNormalizer::zp_normaTypeForString(readerText));
                }
                else if(parentTagStack.last() == zv_FRACTIONAL_BASE_NORMALIZER)
                {
                    calibration->zp_setBaseTermNormaType(ZTermNormalizer::zp_normaTypeForString(readerText));
                }
                else if(parentTagStack.last() == zv_EQUATION)
                {
                    calibration->zp_setEquationType(ZCalibration::zp_equationTypeFromString(readerText));
                }
            }
            else if(currentTagName == zv_FIRST_CHANNEL)
            {
                zv_rawWindow.firstChannel = readerText;
            }
            else if(currentTagName == zv_LAST_CHANNEL)
            {
                zv_rawWindow.lastChannel = readerText;
            }
            else if(currentTagName == zv_STATE)
            {
                zv_rawTerm.termState = ZAbstractTerm::zp_termStateFromString(readerText);
            }
            else if(currentTagName == zv_TERM_WINDOW)
            {
                zv_rawTerm.windowList.append(readerText);
            }
            else if(currentTagName == zv_FACTOR)
            {
                zv_rawTerm.factor = readerText;
            }
            else if(currentTagName == zv_CUSTOM_STRING)
            {
                if(parentTagStack.last() == zv_NORMALIZER)
                {
                    calibration->zp_setNormaCustomString(readerText);
                }
                else if(parentTagStack.last() == zv_FRACTIONAL_BASE_NORMALIZER)
                {
                    calibration->zp_setBaseTermNormaCustomString(readerText);
                }
                else if(parentTagStack.last() == zv_TERM)
                {
                    zv_rawTerm.customString = readerText;
                }
            }
            else if(currentTagName == zv_BASE_TERM)
            {
                calibration->zp_setBaseTermFromName(readerText);
            }
            else if(currentTagName == zv_INTERCEPT)
            {
                calibration->zp_setEquationInterceptString(readerText);
            }
        }
        else  if(reader.isEndElement())
        {
            if(currentTagName == zv_WINDOW)
            {
                calibration->zp_createNewCalibrationWindow(zv_rawWindow);
            }
            else if(currentTagName == zv_TERM)
            {
                calibration->zp_createTerm(zv_rawTerm);
            }
            parentTagStack.pop();
            return; // parent tag is closed
        }
    }

    //    if(reader.hasError())
    //    {
    //        return;
    //    }
}
//============================================================
bool ZXMLCalibrationIOHandler::zh_detectRoot(const QXmlStreamReader& reader, bool& magicStringDetectionFlag) const
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
bool ZXMLCalibrationIOHandler::zh_checkfilePath(const QString& path) const
{
    QFileInfo fileInfo(path);
    if((!fileInfo.exists()) || (!fileInfo.isFile()))
    {
        return false;
    }

    return true;
}
//============================================================
