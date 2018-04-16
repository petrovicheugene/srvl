//===========================================================
#include "ZSpeIOHandler.h"
#include "ZGeneral.h"
#include <QFileInfo>
#include <QFile>
#include <QTextStream>
#include <QFileDevice>
#include "ZSpeSpectrum.h"
//===========================================================
ZSpeIOHandler::ZSpeIOHandler(QObject *spectrumParent, QObject* parent)
    : ZAbstractSpectrumIOHandler(spectrumParent, parent)
{

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
