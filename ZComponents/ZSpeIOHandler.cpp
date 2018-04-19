//===========================================================
#include "ZSpeIOHandler.h"
#include "ZGeneral.h"

#include <QDir>
#include <QFileInfo>
#include <QFile>
#include <QTextStream>
#include <QFileDevice>
#include "ZSpeSpectrum.h"
//===========================================================
ZSpeIOHandler::ZSpeIOHandler(QObject *spectrumParent, QObject* parent)
    : ZAbstractSpectrumIOHandler(spectrumParent, parent)
{
    zv_whatToDoAnswer = -1;
}
//===========================================================
ZSpeIOHandler::~ZSpeIOHandler()
{

}
//===========================================================
bool ZSpeIOHandler::zp_getSpectrumFromFile(const QString& path, QColor color, ZAbstractSpectrum*& spectrum) const
{
    QFileInfo fileInfo(path);
    if(!fileInfo.exists() || !fileInfo.isFile())
    {
        QString errorString = tr("File \"%1\" doesn't exist!").arg(path);
        emit zg_message(errorString);
        return false;
    }

    QFile file(path);
    if(!file.open(QIODevice::ReadOnly))
    {
        QString errorMsg;
        if(file.error() != QFile::NoError)
        {
            errorMsg = tr("Cannot read file \"%1\"! %2").arg(file.fileName(), file.errorString());
        }
        else
        {
            errorMsg = tr("Cannot read file \"%1\"! %2").arg(file.fileName(), tr("Unknown error"));
        }
        emit zg_message(errorMsg);
        return false;
    }

    return zp_getSpectrumFromFile(file, color, spectrum);
}
//===========================================================
bool ZSpeIOHandler::zp_getSpectrumFromFile(QFile& file, QColor color, ZAbstractSpectrum*& spectrum) const
{
    if(!file.isOpen())
    {
        QString errorMsg;
        errorMsg = tr("Cannot read file \"%1\"! The file is not open!").arg(file.fileName());
        emit zg_message(errorMsg);
        return false;
    }
    if(!(file.openMode() & QIODevice::ReadOnly))
    {
        QString errorMsg;
        errorMsg = tr("Cannot read file \"%1\"! The file is not in \"Read\" mode!").arg(file.fileName());
        emit zg_message(errorMsg);
        return false;
    }

    QTextStream ts(&file);
    int lineNumber = 0;
    QList<quint32> intensityList;
    ZSpeAuxData speAuxData;
    bool ok;
    while (!ts.atEnd())
    {
        QString line = ts.readLine();
        if(lineNumber >= zv_intensityStartLine)
        {
            int intensity = line.toInt(&ok);
            if(!ok)
            {
                intensity = 0;
            }
            intensityList.append(intensity);
        }
        else // auxData
        {
            if(lineNumber == 1)
            {
                speAuxData.zp_setDate(line);
            }
            else if(lineNumber == 2)
            {
                speAuxData.zp_setTime(line);
            }
            else if(lineNumber == 3)
            {
                speAuxData.zp_setExposition(line);
            }
            else if(lineNumber == 4)
            {
                speAuxData.zp_setAliveTime(line);
            }
            else if(lineNumber == 10)
            {
                speAuxData.zp_setEnergyUnit(line);
            }
            else if(lineNumber == 11)
            {
                speAuxData.zp_setEnergyK0(line);
            }
            else if(lineNumber == 12)
            {
                speAuxData.zp_setEnergyK1(line);
            }
            else if(lineNumber == 13)
            {
                speAuxData.zp_setEnergyK2(line);
            }
            else if(lineNumber == 14)
            {
                speAuxData.zp_setPeakWidth(line);
            }
            else if(lineNumber == 15)
            {
                speAuxData.zp_setPeakWidthK0(line);
            }
            else if(lineNumber == 16)
            {
                speAuxData.zp_setPeakWidthK1(line);
            }
            else if(lineNumber == 17)
            {
                speAuxData.zp_setPeakWidthK2(line);
            }
        }
        lineNumber++;
    }

    spectrum = new ZSpeSpectrum(intensityList,
                                speAuxData,
                                file.fileName(),
                                color,
                                true,
                                zv_spectrumParent);
    return true;
}
//===========================================================
bool ZSpeIOHandler::zp_saveSpectrumToFile(const QString& path,
                                          const QString& fileName,
                                          ZSpeSpectrum*& spectrum)
{
    // if directory doesn't exist create it
    if(!QDir(path).exists())
    {
        if(!QDir().mkpath(path))
        {
            QString errorMsg;
            errorMsg = tr("Cannot create directory \"%1\"!").arg(path);
            emit zg_message(errorMsg);
            return false;
        }
    }

    QFileInfo fileInfo(QDir(path),fileName);
    if(fileInfo.exists() && zv_whatToDoAnswer != QMessageBox::YesToAll )
    {
        if(zv_whatToDoAnswer == QMessageBox::NoToAll)
        {
            return true;
        }

        QMessageBox msgBox;
        msgBox.setText(tr("The file \"%1\" is already exists.").arg(fileInfo.absoluteFilePath()));
        msgBox.setInformativeText("Do you want to update it?");
        msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::YesToAll
                                  | QMessageBox::No | QMessageBox::NoToAll);
        msgBox.setDefaultButton(QMessageBox::Yes);
        int ret = msgBox.exec();
        zv_whatToDoAnswer = ret;

        switch(ret)
        {
        case QMessageBox::Yes:
            break;
        case QMessageBox::YesToAll:
            break;
        case QMessageBox::No:
            return true;
            break;
        case QMessageBox::NoToAll:
            return true;
            break;
        }
    }

    QFile file(fileInfo.absoluteFilePath());
    if(!file.open(QIODevice::WriteOnly))
    {
        QString errorMsg;
        if(file.error() != QFile::NoError)
        {
            errorMsg = tr("Cannot write to file \"%1\"! %2").arg(fileInfo.absoluteFilePath(), file.errorString());
        }
        else
        {
            errorMsg = tr("Cannot write to file \"%1\"! %2").arg(fileInfo.absoluteFilePath(), tr("Unknown error"));
        }
        emit zg_message(errorMsg);
        return false;
    }

    return zp_saveSpectrumToFile(file, spectrum);

}
//===========================================================
bool ZSpeIOHandler::zp_saveSpectrumToFile(QFile& file, ZSpeSpectrum*& spectrum)
{
    if(!file.isOpen())
    {
        QString errorMsg;
        errorMsg = tr("Cannot write to file \"%1\"! The file is not open!").arg(file.fileName());
        emit zg_message(errorMsg);
        return false;
    }
    if(!(file.openMode() & QIODevice::WriteOnly))
    {
        QString errorMsg;
        errorMsg = tr("Cannot write to file \"%1\"! The file is not in \"Write\" mode!").arg(file.fileName());
        emit zg_message(errorMsg);
        return false;
    }

    QTextStream ts(&file);
    // spe header
    for(int i = 0; i < 20; i++)
    {
        switch(i)
        {
        case 0:

            break;
        case 1:

            break;
        case 2:

            break;
        case 3:

            break;
        case 4:

            break;
        case 5:

            break;
        case 6:

            break;
        case 7:

            break;
        case 8:

            break;
        case 9:

            break;
        case 10:

            break;
        case 11:

            break;
        case 12:

            break;
        case 13:

            break;
        case 14:

            break;
        case 15:

            break;
        case 16:

            break;
        case 17:

            break;
        case 18:

            break;
        case 19:

            break;
        }
    }

    // spectrum data
    QList<quint32> spectrumData;
    spectrum->zp_setSpectrumData(spectrumData);

    foreach(quint32 intensity, spectrumData)
    {
        ts << QString::number(intensity, 10);
    }

    file.close();
    return true;
//    int lineNumber = 0;
//    bool ok;
//    while (!ts.atEnd())
//    {
//        QString line = ts.readLine();
//        if(lineNumber >= zv_intensityStartLine)
//        {
//            int intensity = line.toInt(&ok);
//            if(!ok)
//            {
//                intensity = 0;
//            }
//            intensityList.append(intensity);
//        }
//        else // auxData



}
//===========================================================
