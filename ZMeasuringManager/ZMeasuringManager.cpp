//======================================================
#include "ZMeasuringManager.h"
#include "ZGeneral.h"

#include "ZAddSampleDialog.h"
#include "ZAppSettings.h"
#include "ZControlAction.h"
#include "ZDeviceSettings.h"
#include "ZEnergyCalibrationDialog.h"
#include "ZEnergyCalibrationDialogV2.h"
#include "ZMeasuringController.h"
#include "ZSaveSpectraToFilesDialog.h"
#include "ZSetGainFactorToCalibrationDialog.h"
#include "ZSample.h"
#include "ZSampleTask.h"
#include "ZSeriesTaskListDialog.h"
#include "ZSeriesTaskListManager.h"
#include "ZSpeIOHandler.h"
#include "ZSpeSpectrum.h"
#include "ZUralAdcDeviceConnector.h"

#include <QApplication>
#include <QColor>
#include <QDataStream>
#include <QDebug>
#include <QDir>
#include <QDateTime>
#include <QFile>
#include <QFileDialog>
#include <QMap>
#include <QMenu>
#include <QMessageBox>
#include <QFileInfo>
#include <QSettings>
#include <QSqlError>
#include <QSqlQuery>
#include <QTimer>
#include <QTimerEvent>
//======================================================
ZMeasuringManager::ZMeasuringManager(QObject *parent)
    : QObject(parent)
{
    //qRegisterMetaTypeStreamOperators<ZSpeSpectrum>("ZSpeSpectrum");

    zv_UralAdcDeviceConnector = nullptr;
    zv_measuringController = nullptr;
    zv_lastColorIndex = 0;
    zv_deviceSampleQuantity = 1;
    zv_expositionDelayDuration = 0;
    zv_seriesTimePassed = 0;
    zv_expositionDelayTimer = 0;
    zv_seriesId = -1;
    zv_startDateTime = QDateTime();

    zh_createColorList();

    zh_createActions();
    zh_createComponents();
    zh_createConnections();
    zh_restoreSettings();

    zh_manageControlEnable();


}
//======================================================
ZMeasuringManager::~ZMeasuringManager()
{
    qDeleteAll(zv_sampleList);
    zh_saveSettings();
}
//======================================================
void ZMeasuringManager::zh_createColorList()
{
    int red;
    int green;
    int blue;
    QColor color;

    // QList<QColor> colorList;

    while(zv_colorList.count() < 1500)
    {
        qsrand(static_cast<uint>(QDateTime::currentMSecsSinceEpoch()));
        red = ((qrand() % 24) * 10) + 15;
        green = ((qrand() % 24) * 10) + 15;
        blue = ((qrand() % 24) * 10) +15;

        color = QColor(red, green, blue);
        if(zh_checkColor(color))
        {
            zv_colorList << color;
        }
    }
}
//======================================================
bool ZMeasuringManager::zh_checkColor(QColor color)
{
    if(!color.isValid() || zv_colorList.contains(color))
    {
        return false;
    }

    //    if(color.red() == 255 &&  color.green() < 10 && color.blue() < 10  ||
    //            color.red() < 10 &&  color.green() == 255 && color.blue() < 10  ||
    //            color.red() < 10 &&  color.green() < 10 && color.blue() == 255  )
    //    {
    //        return false;
    //    }

    int colorSumm = color.red() + color.green() + color.blue();
    int averageVol = colorSumm / 3;

    if(colorSumm > 450)
    {
        return false;
    }

    if(qAbs(averageVol - color.red()) + qAbs(averageVol - color.green()) + qAbs(averageVol - color.blue()) < 90)
    {
        return false;
    }

    return true;
}
//======================================================
void ZMeasuringManager::zh_createActions()
{
    zv_saveSeriesAction = new ZControlAction(this);
    zv_saveSeriesAction->setIcon(QIcon(":images/ZImages/save-8.png"));
    zv_saveSeriesAction->setText(tr("Save series"));
    zv_saveSeriesAction->setToolTip(tr("Save current series"));
    zv_saveSeriesAction->setEnabled(false);

    zv_loadSeriesAction = new ZControlAction(this);
    zv_loadSeriesAction->setIcon(QIcon(":images/ZImages/load-8.png"));
    zv_loadSeriesAction->setText(tr("Load series"));
    zv_loadSeriesAction->setToolTip(tr("Load series"));

    zv_addSamplesToSeriesAction = new ZControlAction(this);
    zv_addSamplesToSeriesAction->setIcon(QIcon(":images/ZImages/add-8.png"));
    zv_addSamplesToSeriesAction->setText(tr("Add samples"));
    zv_addSamplesToSeriesAction->setToolTip(tr("Add sample to series"));

    zv_removeSamplesFromSeriesAction = new ZControlAction(this);
    zv_removeSamplesFromSeriesAction->setIcon(QIcon(":images/ZImages/remove-8.png"));
    zv_removeSamplesFromSeriesAction->setText(tr("Remove samples"));
    zv_removeSamplesFromSeriesAction->setEnabled(false);
    zv_removeSamplesFromSeriesAction->setToolTip(tr("Remove sample from series"));

    zv_loadSpectraFromFilesAction = new ZControlAction(this);
    zv_loadSpectraFromFilesAction->setText(tr("Load spectra from files"));

    zv_saveSpectraToFilesAction = new ZControlAction(this);
    zv_saveSpectraToFilesAction->setIcon(QIcon(":images/ZImages/saveToFile-8.png"));
    zv_saveSpectraToFilesAction->setText(tr("Save spectra to files"));
    zv_saveSpectraToFilesAction->setEnabled(false);
    zv_saveSpectraToFilesAction->setToolTip(tr("Save selected spectra to files"));

    zv_printAction = new ZControlAction(this);
    zv_printAction->setIcon(QIcon(":images/ZImages/print-8.png"));
    zv_printAction->setText(tr("Print"));
    zv_printAction->setToolTip(tr("Print measurement results"));

    zv_previewAndPrintAction = new ZControlAction(this);
    //zv_previewAndPrintAction->setIcon(QIcon(":images/ZImages/print-preview-2.png"));
    zv_previewAndPrintAction->setText(tr("Preview and print"));

    // connection actions
    QStringList deviceNameList = ZUralAdcDeviceConnector::zp_deviceNameList();
    ZControlAction* action;
    foreach(QString name,  deviceNameList)
    {
        action = new ZControlAction(this);
        action->setText(name);
        action->setData(QVariant(name));
        zv_connectionActionList.append(action);
    }

    //    zv_addSamplesToSeriesAction = new ZControlAction(this);
    //    zv_addSamplesToSeriesAction->setText(tr("Add samples"));
    zv_energyCalibrationAction = new ZControlAction(this);
    zv_energyCalibrationAction->setText(tr("Energy calibration"));

    zv_spectrumInfoAction = new ZControlAction(this);
    zv_spectrumInfoAction->setText(tr("Spectrum info"));


}
//======================================================
void ZMeasuringManager::zh_createComponents()
{
    zv_measuringController = new ZMeasuringController(this);
}
//======================================================
void ZMeasuringManager::zh_createConnections()
{
    connect(zv_saveSeriesAction, &ZControlAction::triggered,
            this, &ZMeasuringManager::zh_onSaveSeriesAction);
    connect(zv_loadSeriesAction, &ZControlAction::triggered,
            this, &ZMeasuringManager::zh_onLoadSeriesAction);

    connect(zv_addSamplesToSeriesAction, &ZControlAction::triggered,
            this, &ZMeasuringManager::zh_onAddSamplesToSeriesAction);
    connect(zv_removeSamplesFromSeriesAction, &ZControlAction::triggered,
            this, &ZMeasuringManager::zh_onRemoveSamplesFromSeriesAction);

    connect(zv_loadSpectraFromFilesAction, &ZControlAction::triggered,
            this, &ZMeasuringManager::zh_onLoadSpectraFromFilesAction);
    connect(zv_saveSpectraToFilesAction, &ZControlAction::triggered,
            this, &ZMeasuringManager::zh_onSaveSpectraToFilesAction);

    connect(zv_energyCalibrationAction, &ZControlAction::triggered,
            this, &ZMeasuringManager::zh_onEnergyCalibrationAction);
    connect(zv_printAction, &ZControlAction::triggered,
            this, &ZMeasuringManager::zh_onPrintAction);
    connect(zv_previewAndPrintAction, &ZControlAction::triggered,
            this, &ZMeasuringManager::zh_onPreviewAndPrintAction);


    connect(zv_measuringController, &ZMeasuringController::zg_connectionState,
            this, &ZMeasuringManager::zg_connectionState);
    connect(zv_measuringController, &ZMeasuringController::zg_message,
            this, &ZMeasuringManager::zg_message);
    connect(zv_measuringController, &ZMeasuringController::zg_expositionPassed,
            this, &ZMeasuringManager::zh_onExpositionPassedMSec);
    connect(zv_measuringController, &ZMeasuringController::zg_currentMeasuringParameters,
            this, &ZMeasuringManager::zg_measurementParameters);
    connect(zv_measuringController, &ZMeasuringController::zg_currentSpectrumId,
            this, &ZMeasuringManager::zh_onCurrentSpectrumMeasuring);
    connect(zv_measuringController, &ZMeasuringController::zg_currentEnergyCalibration,
            this, &ZMeasuringManager::zh_onCurrentEnergyCalibration);


    foreach(ZControlAction* action, zv_connectionActionList)
    {
        connect(action, &ZControlAction::triggered,
                this, &ZMeasuringManager::zh_onConnectToDeviceAction);
    }

}
//======================================================
void ZMeasuringManager::zh_restoreSettings()
{
    QSettings settings;
    QVariant vData;
    settings.beginGroup(glAppVersion);
    settings.beginGroup("MeasuringManager");

    vData = settings.value("spectrumFolderPath");
    if(vData.isValid() && !vData.isNull() && vData.canConvert<QString>())
    {
        zv_spectrumFolderPath = vData.toString();
    }

    settings.endGroup();
    settings.endGroup();
}
//======================================================
void ZMeasuringManager::zh_saveSettings() const
{
    QSettings settings;
    settings.beginGroup(glAppVersion);
    settings.beginGroup("MeasuringManager");

    settings.setValue("spectrumFolderPath", QVariant(zv_spectrumFolderPath));

    settings.endGroup();
    settings.endGroup();
}
//======================================================
void ZMeasuringManager::zp_applyAppSettings(const ZAppSettings& appSettings)
{
    QString libraryFileName = appSettings.zv_deviceSettings.zv_libraryFileName;
    QString defaultConnectionADC = appSettings.zv_deviceSettings.zv_defaultConnectionADC;
    zv_deviceSampleQuantity = appSettings.zv_deviceSettings.zv_deviceSampleQuantity;
    zv_expositionDelayDuration = appSettings.zv_deviceSettings.zv_expositionDelayTime;
    zv_measuringController->zp_setSampleShiftImpulseDuration(appSettings.zv_deviceSettings.zv_sampleShiftImpulseDuration);

    QString errorMsg;
    // Library
    if(libraryFileName.isEmpty())
    {
        libraryFileName = QDir(qApp->applicationDirPath()).absoluteFilePath(zv_deviceLibraryDefaultFileName);
        if(!QFile(libraryFileName).exists())
        {
            if(!zh_createLibraryFromResources(libraryFileName, errorMsg))
            {
                QMessageBox::critical(nullptr, tr("Library error"), errorMsg, QMessageBox::Ok);
                emit zg_message(errorMsg, QMessageBox::Critical);
                QString connectionState = tr("ADC library absent.");
                emit zg_connectionState(connectionState, QMessageBox::Critical );
                return;
            }
        }
    }

    // try to load library
    bool ok;

    if(!zv_UralAdcDeviceConnector)
    {
        zv_UralAdcDeviceConnector = new ZUralAdcDeviceConnector(libraryFileName,
                                                                ok, errorMsg, this);

        if(!ok)
        {
            // roll out error message
            QMessageBox::critical(nullptr, tr("Library load error"), errorMsg, QMessageBox::Ok);
            emit zg_message(errorMsg, QMessageBox::Critical);

            QString connectionState = tr("ADC library loading error.");
            emit zg_connectionState(connectionState, QMessageBox::Critical );

            delete zv_UralAdcDeviceConnector;
            zv_measuringController->zp_setConnector(nullptr);
            zv_UralAdcDeviceConnector = nullptr;
            zh_setConnectionActionsEnable(false);
            return;
        }
        else
        {
            QString connectionState = tr("ADC library loaded.");
            emit zg_connectionState(connectionState, QMessageBox::Question );
        }

        // try to connect to ADC
        if(!defaultConnectionADC.isEmpty())
        {
            zh_connectToDevice(defaultConnectionADC);
        }
    }
}
//======================================================
void ZMeasuringManager::zh_onConnectToDeviceAction()
{
    // get device name from action
    ZControlAction* action = qobject_cast<ZControlAction*>(sender());
    if(!action )
    {
        return;
    }

    QString deviceName;
    QVariant vData = action->data();
    if(!vData.isValid() || vData.isNull() || !vData.canConvert<QString>())
    {
        // if action has not internal data, try to get device name from action text
        deviceName = action->text();
    }
    else
    {
        deviceName = vData.toString();
    }

    zh_connectToDevice(deviceName);

}
//======================================================
void ZMeasuringManager::zh_onExpositionPassedMSec(qint64 expoPassedMs)
{
    //#ifdef DBG
    //        qDebug() << "EXPO PASSED" << expoPassedMs;
    //#endif

    // calc passed sample measuring time
    if(zv_currentMeasuringState.zp_currentSampleRow() < 0
            || zv_currentMeasuringState.zp_currentSampleRow() >= zv_sampleList.count())
    {
        return;
    }

    int sampleTimePassedSec = static_cast<int>(expoPassedMs / 1000);
    //QString dbgStr = QString("DASHBOARD SAMPLE TIME - %1").arg(QString::number(sampleTimePassedSec));
    //qDebug() << dbgStr;

    zv_currentMeasuringState.zp_setSampleTimePassed(sampleTimePassedSec);

    // calc series time passed

    zv_currentMeasuringState.zp_setSeriesTimePassed(zv_seriesTimePassed + sampleTimePassedSec);
    zh_notifyMeasuringStateChanged();

}
//======================================================
bool ZMeasuringManager::zh_connectToDevice(QString deviceName )
{
    if(!zv_UralAdcDeviceConnector)
    {
        return false;
    }

    ZUralAdcDeviceConnector::PID pid;
    ZUralAdcDeviceConnector::VID vid;
    ZUralAdcDeviceConnector::zp_pidVidForDeviceName(deviceName, pid, vid);
    ZUralAdcDeviceConnector::SlotResult res = ZUralAdcDeviceConnector::SR_FALSE;

    zv_UralAdcDeviceConnector->zp_connectToDevice(res, pid, vid);

    if(res == ZUralAdcDeviceConnector::SR_TRUE)
    {
        zv_measuringController->zp_setConnector(zv_UralAdcDeviceConnector);

        QString connectionState = tr("Connected to %1.").arg(deviceName);
        emit zg_connectionState(connectionState, QMessageBox::Information);

        zh_setConnectionActionsEnable(false);
    }
    else if(res == ZUralAdcDeviceConnector::SR_FUNCTION_UNRESOLVED)
    {
        QString connectionState = tr("Connectin function is unresolved.");
        emit zg_connectionState(connectionState, QMessageBox::Critical);

        zh_setConnectionActionsEnable(true);
    }
    else if(res == ZUralAdcDeviceConnector::SR_FALSE)
    {
        QString connectionState = tr("Connection to %1 failed.").arg(deviceName);
        emit zg_connectionState(connectionState, QMessageBox::Critical);

        zh_setConnectionActionsEnable(true);
    }
    else if(res == ZUralAdcDeviceConnector::SR_ERROR)
    {
        QString connectionState = tr("Unknown connection error.");
        emit zg_connectionState(connectionState, QMessageBox::Critical);

        zh_setConnectionActionsEnable(true);
    }

    return res == ZUralAdcDeviceConnector::SR_TRUE;

}
//======================================================
void ZMeasuringManager::zh_setConnectionActionsEnable(bool enabling)
{
    foreach(ZControlAction* action, zv_connectionActionList)
    {
        action->setEnabled(enabling);
    }
}
//======================================================
void ZMeasuringManager::zh_recalcSeriesMeasuringTotalDuration()
{
    int totalSeriesDuration = 0;
    if(zv_sampleList.isEmpty())
    {
        return;
    }

    for(int i = 0; i < zv_sampleList.count(); i++)
    {
        totalSeriesDuration += zv_sampleList.at(i)->zp_totalMeasuringDuration();
    }

    // sample set count
    int sampleFullSetCount = zv_sampleList.count() / zv_deviceSampleQuantity;
    int sampleLastSetCount = zv_sampleList.count() - sampleFullSetCount * zv_deviceSampleQuantity;

    int dalayTotalDuration = (sampleFullSetCount * (zv_deviceSampleQuantity - 1) + (sampleLastSetCount - 1) )
            * zv_expositionDelayDuration;
    totalSeriesDuration += dalayTotalDuration;

    zv_currentMeasuringState.zp_setSeriesDuration(totalSeriesDuration);

}
//======================================================
bool ZMeasuringManager::zp_libraryState() const
{
    return zv_UralAdcDeviceConnector != nullptr;
}
//======================================================
bool ZMeasuringManager::zp_spectrumVisibility(qint64 spectrumId, bool& visibility) const
{
    foreach(ZSample* sample, zv_sampleList)
    {
        if(sample->zp_spectrumVisibilityForSpectrumId(spectrumId, visibility))
        {
            return true;
        }
    }

    return false;
}
//======================================================
//bool ZMeasuringManager::zp_connectionState() const
//{
//    if(zv_UralAdcDeviceConnector == nullptr)
//    {
//        return false;
//    }

//    ZUralAdcDeviceConnector::SlotResult res;
//    zv_UralAdcDeviceConnector->zp_isDeviceConnected(res);

//    if(res == ZUralAdcDeviceConnector::SR_TRUE)
//    {
//        return true;
//    }

//    return false;
//}
//======================================================
ZSpeSpectrum* ZMeasuringManager::zp_spectrumForId(qint64 id) const
{
    ZSpeSpectrum* spectrum = nullptr;
    foreach(ZSample* sample, zv_sampleList)
    {
        spectrum = sample->zp_spectrumForId(id);
        if(spectrum)
        {
            return spectrum;
        }
    }

    return nullptr;
}
//======================================================
QDateTime ZMeasuringManager::zp_currentMeasuringStartDateTime() const
{
    return zv_startDateTime;
}
//======================================================
QDateTime ZMeasuringManager::zp_currentMeasuringFinishDateTime() const
{
    return zv_finishDateTime;
}
//======================================================
QString  ZMeasuringManager::zp_currentSeriesName() const
{
    return zv_currentMeasuringState.zp_currentSeriesName();
}
//======================================================
bool ZMeasuringManager::zh_createLibraryFromResources(const QString& libraryFileName, QString &errorMsg)
{
    // check library file in resources
    QString resourceLibFileName = ":/library/ComDV.dll";
    QFile resorceFile(resourceLibFileName);
    if(!resorceFile.exists())
    {
        errorMsg = tr("The required library file \"%1\" has not found in the application folder \"%2\". "
                      "The application cannot create library file because the source is absent in resources.")
                .arg(QFileInfo(libraryFileName).fileName(), qApp->applicationDirPath());
        return false;
    }

    //  if the library file exists remove it from app folder
    QFile::remove(libraryFileName);

    // create the new library file from resources in the folder of the application
    if(!QFile::copy(resourceLibFileName, libraryFileName))
    {
        errorMsg = tr("The required library file \"%1\" in the application folder \"%2\" has not found. "
                      "The application unsuccessfully attempted to create the library file from its own resources. "
                      "It seems you have not enough permission. "
                      "Restart the application with permission to write into the application folder.")
                .arg(QFileInfo(libraryFileName).fileName(), qApp->applicationDirPath());

        // creation of the file is impossible for some reason
        return false;
    }

    return true;
}
//======================================================
QList<ZControlAction*> ZMeasuringManager::zp_sampleActions() const
{
    QList<ZControlAction*> actionList;
    actionList.append(zv_saveSeriesAction);
    actionList.append(zv_loadSeriesAction);
    actionList.append(nullptr);
    actionList.append(zv_addSamplesToSeriesAction);
    actionList.append(zv_removeSamplesFromSeriesAction);
    //    actionList.append(nullptr);
    //    actionList.append(zv_saveSpectraToFilesAction);
    actionList.append(nullptr);
    //    actionList.append(zv_previewAndPrintAction);
    actionList.append(zv_printAction);

    return actionList;
}
//======================================================
QList<ZControlAction*> ZMeasuringManager::zp_sampleContextActions() const
{
    QList<ZControlAction*> actionList;
    actionList.append(zv_saveSeriesAction);
    actionList.append(zv_loadSeriesAction);
    actionList.append(nullptr);
    actionList.append(zv_addSamplesToSeriesAction);
    actionList.append(zv_removeSamplesFromSeriesAction);
    actionList.append(nullptr);
    actionList.append(zv_saveSpectraToFilesAction);
    actionList.append(nullptr);
    actionList.append(zv_previewAndPrintAction);
    actionList.append(zv_printAction);

    return actionList;
}
//======================================================
QList<ZControlAction*> ZMeasuringManager::zp_connectionActions() const
{
    return zv_connectionActionList;
}
//======================================================
QList<ZControlAction*> ZMeasuringManager::zp_toolActions() const
{
    QList<ZControlAction*> actionList;
    actionList.append(zv_energyCalibrationAction);
    //actionList.append(zv_spectrumInfoAction);

    return actionList;
}
//======================================================
int ZMeasuringManager::zp_sampleCount() const
{
    return zv_sampleList.count();
}
//======================================================
QString ZMeasuringManager::zp_sampleName(int sampleIndex) const
{
    if(sampleIndex < 0 || sampleIndex >= zv_sampleList.count())
    {
        return QString();
    }

    return zv_sampleList.at(sampleIndex)->zp_sampleName();
}
//======================================================
int ZMeasuringManager::zp_indexForSampleName(const QString& sampleName) const
{
    if(sampleName.isEmpty() || zv_sampleList.isEmpty())
    {
        return -1;
    }

    for(int i = 0; i < zv_sampleList.count(); i++)
    {
        if(zv_sampleList.at(i)->zp_sampleName() == sampleName)
        {
            return i;
        }
    }

    return -1;
}
//======================================================
bool ZMeasuringManager::zp_setSampleName(int sampleIndex, const QString& name)
{
    if(sampleIndex < 0 || sampleIndex >= zv_sampleList.count())
    {
        return false;
    }

    return zv_sampleList.at(sampleIndex)->zp_setSampleName(name);
}
//======================================================
void ZMeasuringManager::zp_appendActionsToMenu(QMenu* menu) const
{
    if(menu->objectName() == NS_ObjectNames::glObjectNameMenuFile)
    {
        menu->addAction(zv_saveSeriesAction);
        menu->addAction(zv_loadSeriesAction);
        menu->addSeparator();

        menu->addAction(zv_addSamplesToSeriesAction);
        menu->addAction(zv_removeSamplesFromSeriesAction);
        menu->addSeparator();

        menu->addAction(zv_saveSpectraToFilesAction);
        menu->addSeparator();

        menu->addAction(zv_printAction);
        menu->addAction(zv_previewAndPrintAction);

    }

    if(menu->objectName() == NS_ObjectNames::glObjectNameMenuActions)
    {
        QMenu* connectMenu = new QMenu(tr("Connect to..."));
        QList<ZControlAction*> connectActionList = zp_connectionActions();
        foreach(ZControlAction* action, connectActionList)
        {
            connectMenu->addAction(action);
        }

        menu->addMenu(connectMenu);
        menu->addSeparator();
        QList<ZControlAction*> toolActionList = zp_toolActions();
        foreach(ZControlAction* action, toolActionList)
        {
            menu->addAction(action);
        }

    }
}
//======================================================
QString ZMeasuringManager::zp_sampleTaskName(int sampleIndex) const
{
    if(sampleIndex < 0 || sampleIndex >= zv_sampleList.count())
    {
        return QString();
    }

    return zv_sampleList.at(sampleIndex)->zp_sampleTaskName();
}
//======================================================
int ZMeasuringManager::zp_sampleTaskId(int sampleIndex) const
{
    if(sampleIndex < 0 || sampleIndex >= zv_sampleList.count())
    {
        return -1;
    }

    return zv_sampleList.at(sampleIndex)->zp_sampleTaskId();
}
//======================================================
QStringList ZMeasuringManager::zp_chemicalListForSample(int sampleIndex) const
{
    if(sampleIndex < 0 || sampleIndex >= zv_sampleList.count())
    {
        return QStringList();
    }

    return zv_sampleList.at(sampleIndex)->zp_sampleChemicalList();
}
//======================================================
QStringList ZMeasuringManager::zp_measuringConditionsStringListForSample(int sampleIndex) const
{
    if(sampleIndex < 0 || sampleIndex >= zv_sampleList.count())
    {
        return QStringList();
    }

    return zv_sampleList.at(sampleIndex)->zp_sampleMeasuringConditionsStringList();
}
//======================================================
QList<QPair<quint8, int> > ZMeasuringManager::zp_measuringConditionsListForSample(int sampleIndex) const
{
    if(sampleIndex < 0 || sampleIndex >= zv_sampleList.count())
    {
        return QList<QPair<quint8, int> >();
    }

    return zv_sampleList.at(sampleIndex)->zp_sampleMeasuringConditionsList();
}
//======================================================
QString ZMeasuringManager::zp_seriesTaskName() const
{
    return zv_currentMeasuringState.zp_currentSeriesName();
}
//======================================================
bool  ZMeasuringManager::zp_concentration(int row,
                                          const QString& chemical,
                                          double& concentration)
{
    if(row < 0 || row >= zv_sampleList.count())
    {
        return false;
    }

    return zv_sampleList.at(row)->zp_concentration(chemical, concentration);
}
//======================================================
QList<quint32> ZMeasuringManager::zp_spectrumData(int row, int gainFactor, int exposition)
{
    ZSpeSpectrum* spectrum = zv_sampleList.at(row)->zp_spectrumForMeasuringConditions(static_cast<quint8>(gainFactor), exposition);
    if(!spectrum)
    {
        return QList<quint32>();
    }

    return spectrum->zp_spectrumData();

}
//======================================================
ZSpeSpectrum* ZMeasuringManager::zp_spectrum(int row, quint8 gainFactor, int exposition)
{
    ZSpeSpectrum* spectrum = zv_sampleList.at(row)->zp_spectrumForMeasuringConditions(gainFactor, exposition);
    return spectrum;
}
//======================================================
int ZMeasuringManager::zp_arrayChannelCount(int gainFactor, int exposition) const
{
    foreach(SpectrumCommonProperties properties,  zv_spectrumCommonPropertiesList)
    {
        if(properties.gainFactor == gainFactor && properties.exposition == exposition)
        {
            return properties.channelCount;
        }
    }

    return 0;
}
//======================================================
int ZMeasuringManager::zp_arrayMaxIntensity(int gainFactor, int exposition) const
{
    foreach(SpectrumCommonProperties properties,  zv_spectrumCommonPropertiesList)
    {
        if(properties.gainFactor == gainFactor && properties.exposition == exposition)
        {
            return properties.maxValue;
        }
    }

    return 0;
}
//======================================================
QColor ZMeasuringManager::zp_spectrumColor(int sampleRow, int gainFactor, int exposition) const
{
    if(sampleRow < 0 || sampleRow >= zv_sampleList.count())
    {
        return QColor();
    }

    ZSpeSpectrum* spectrum = zv_sampleList.at(sampleRow)->zp_spectrumForMeasuringConditions(static_cast<quint8>(gainFactor), exposition);
    if(!spectrum)
    {
        return QColor();
    }

    return spectrum->zp_color();
}
//======================================================
bool ZMeasuringManager::zp_spectrumVisibility(int sampleRow, int gainFactor, int exposition, bool& visibility) const
{
    if(sampleRow < 0 || sampleRow >= zv_sampleList.count())
    {
        return false;
    }

    ZSpeSpectrum* spectrum = zv_sampleList.at(sampleRow)->zp_spectrumForMeasuringConditions(static_cast<quint8>(gainFactor), exposition);
    if(!spectrum)
    {
        return false;
    }

    visibility = spectrum->zp_isSpectrumVisible();
    return true;
}
//======================================================
bool ZMeasuringManager::zp_setSpectrumVisibility(int sampleRow, int gainFactor, int exposition, bool visibility)
{
    if(sampleRow < 0 || sampleRow >= zv_sampleList.count())
    {
        return false;
    }

    ZSpeSpectrum* spectrum = zv_sampleList.at(sampleRow)->zp_spectrumForMeasuringConditions(static_cast<quint8>(gainFactor), exposition);
    if(!spectrum)
    {
        return false;
    }

    spectrum->zp_setSpectrumVisible(visibility);
    emit zg_sampleOperation(SOT_SPECTRUM_VISIBILITY_CHANGED, sampleRow, sampleRow);
    return true;
}
//======================================================
void ZMeasuringManager::zh_calcSpectrumCommonProperties(quint8 gainFactor, int exposition)
{
    ZSpeSpectrum* spectrum;
    int channelCount = 0;
    int maxValue = 0;
    int currentChannelCount;
    int currentMaxValue;

    // spectrum for measuring conditions cycle
    for(int i = 0; i < zv_sampleList.count(); i++)
    {
        spectrum = zv_sampleList.at(i)->zp_spectrumForMeasuringConditions(gainFactor, exposition);
        if(!spectrum)
        {
            continue;
        }

        currentChannelCount = spectrum->zp_channelCount();
        if(channelCount < currentChannelCount)
        {
            channelCount = currentChannelCount;
        }

        currentMaxValue = spectrum->zp_maxIntensity();
        if(maxValue < currentMaxValue)
        {
            maxValue = currentMaxValue;
        }
    }

    // find spectrum properties  and  redefine
    SpectrumCommonProperties properties;
    for(int i = 0; i < zv_spectrumCommonPropertiesList.count(); i++)
    {
        properties = zv_spectrumCommonPropertiesList.at(i);
        if(properties.gainFactor == gainFactor
                && properties.exposition == exposition)
        {
            zv_spectrumCommonPropertiesList[i].channelCount = channelCount;
            zv_spectrumCommonPropertiesList[i].maxValue = maxValue;
            return;
        }
    }

    // if spectrum properties doesn't exist create it
    properties.gainFactor = gainFactor;
    properties.exposition = exposition;
    properties.channelCount = channelCount;
    properties.maxValue = maxValue;

    zv_spectrumCommonPropertiesList.append(properties);
    emit zg_sampleOperation(SOT_SPECTRUM_PROPERTIES_CHANGED, -1, -1);
}
//======================================================
void ZMeasuringManager::zh_notifyMeasuringStateChanged()
{
    emit zg_measuringStateChanged(zv_currentMeasuringState);
}
//======================================================
void ZMeasuringManager::zh_onCurrentSpectrumMeasuring(qint64 currentMeasuredSpectrumId)
{
    emit zg_currentSpectrumChanged(currentMeasuredSpectrumId);
}
//======================================================
void ZMeasuringManager::zh_onCurrentEnergyCalibration(QList<double> energyCalibrationFactorList)
{
    emit zg_currentEnergyCalibrationChanged(energyCalibrationFactorList);
}
//======================================================
void ZMeasuringManager::zp_onSelectionChange()
{
    zh_manageControlEnable();
}
//======================================================
void ZMeasuringManager::zp_notifyOfCurrentStatus()
{
    zh_notifyMeasuringStateChanged();
}
//======================================================
//void ZMeasuringManager::zp_setCurrentSampleIndex(int currentSampleIndex)
//{
//    if(currentSampleIndex >= zv_sampleList.count())
//    {
//        zv_currentSampleIndex = -1;
//        return;
//    }

//    zv_currentSampleIndex = currentSampleIndex;
//}
//======================================================
void ZMeasuringManager::zp_startSeries()
{
    if(zv_expositionDelayTimer)
    {
        return;
    }

    if(zv_sampleList.isEmpty())
    {
        //zv_currentMeasuringState = ZMeasuringState();
        // if sample list is empty - stop
        //if(zv_currentMeasuringState.zp_measuringAction() != ZMeasuringState::MA_STOPPED)
        //{
        zp_stopSeries();
        //}
        //zh_notifyMeasuringStateChanged();
        return;
    }
    // if suspended - check sample list for emptiness and continue or stop
    if(zv_currentMeasuringState.zp_measuringAction() == ZMeasuringState::MA_SUSPENDED)
    {
        if(zv_currentMeasuringState.zp_currentSampleRow() >= zv_sampleList.count()
                || zv_currentMeasuringState.zp_currentSampleRow() < 0)
        {
            zp_stopSeries();
            return;
        }

        // continue measurement series
        int sampleDuration = zv_sampleList.at(zv_currentMeasuringState.zp_currentSampleRow())->zp_totalMeasuringDuration();
        zv_currentMeasuringState.zp_setSampleDuration(sampleDuration);
        zv_currentMeasuringState.zp_setSampleTimePassed(0);

        zv_currentMeasuringState.zp_setCurrentSampleName(zv_sampleList.at(zv_currentMeasuringState.zp_currentSampleRow())->zp_sampleName());
        zv_sampleList.at(zv_currentMeasuringState.zp_currentSampleRow())->zp_startMeasuring();
        zv_currentMeasuringState.zp_setMeasuringAction(ZMeasuringState::MA_RUNNING);
        zh_notifyMeasuringStateChanged();
        return;
    }
    else if(zv_currentMeasuringState.zp_measuringAction() == ZMeasuringState::MA_STOPPED)
    {
        zv_currentMeasuringState.zp_incrementSampleRow();
        if(zv_currentMeasuringState.zp_currentSampleRow() >= zv_sampleList.count()
                || zv_currentMeasuringState.zp_currentSampleRow() < 0)
        {
            zp_stopSeries();
            return;
        }

        // if stopped - launch new measurement
        // if spe and results exist save savethem to database

        // clear spe and results from samples
        zh_resetMeasuringResults();

        // reset first
        // zv_currentMeasuringState.zp_setSeriesName(zv_currentSeriesTaskName);
        zv_startDateTime = QDateTime::currentDateTime();
        zv_currentMeasuringState.zp_setCurrentSampleName(zv_sampleList.at(zv_currentMeasuringState.zp_currentSampleRow())->zp_sampleName());
        int sampleDuration = zv_sampleList.at(zv_currentMeasuringState.zp_currentSampleRow())->zp_totalMeasuringDuration();
        zv_currentMeasuringState.zp_setSampleDuration(sampleDuration);
        zv_currentMeasuringState.zp_setSampleTimePassed(0);
        zv_currentMeasuringState.zp_setSeriesTimePassed(0);

        // measure series
        if(!zv_sampleList.at(zv_currentMeasuringState.zp_currentSampleRow())->zp_startMeasuring())
        {
            zp_stopSeries();
            return;
        }

        zh_assignNewSeriesId();
        zv_currentMeasuringState.zp_setMeasuringAction(ZMeasuringState::MA_RUNNING);
        zh_notifyMeasuringStateChanged();
    }

    // QMessageBox::information(0, "MM", "START SERIES", QMessageBox::Ok);
}
//======================================================
void ZMeasuringManager::zh_assignNewSeriesId()
{
    int newId = 0;
    zv_seriesId = 0;
    if(zh_findNewIdInTable("series", newId))
    {
        zv_seriesId = newId;
    }
}
//======================================================
void ZMeasuringManager::zh_saveSampleMeasurementResult()
{

    if(!zh_recordSeriesId())
    {
        // id record error
        return;
    }

    int newSampleId = 0;
    if(!zh_findNewIdInTable("measured_samples", newSampleId))
    {
        // cannot obtain new sample id
        return;
    }

    // record sample
    QSqlQuery query;
    QString queryString = QString("INSERT INTO measured_samples (id, name, series_id, sample_tasks_id) "
                                  "VALUES (:id, :name, :series_id, :sample_tasks_id)");

    if(!query.prepare(queryString))
    {
        qDebug() << query.lastError().text();
        return;
    }

    query.bindValue(":id", newSampleId);
    query.bindValue(":name", zv_currentMeasuringState.zp_currentSampleName());
    query.bindValue(":series_id", zv_seriesId);
    query.bindValue(":sample_tasks_id", zv_measuringController->zp_currentSampleTaskId());

    if(!query.exec())
    {
        return ;
    }

    // record spectra
    // current sample
    ZSample* currentSample = zv_sampleList.at(zv_currentMeasuringState.zp_currentSampleRow());

    // zv_measuring conditions map
    QMap<int, QPair<quint8,int> > measuringConditionsMap
            = zv_measuringController->zp_currentSampleTaskMeasuringConditions();

    QMap<int, QPair<quint8,int> >::const_iterator it;
    for(it = measuringConditionsMap.begin(); it != measuringConditionsMap.end(); it++)
    {
        int newSpectrumId = 0;
        if(!zh_findNewIdInTable("measured_spectra", newSpectrumId))
        {
            // cannot obtain new spectrum id
            continue;
        }

        ZSpeSpectrum* spectrum = currentSample->zp_spectrumForMeasuringConditions(it.value().first, it.value().second);
        QByteArray spectrumByteArray;
        QDataStream dataStream(&spectrumByteArray, QIODevice::ReadWrite);

        dataStream << *spectrum;

        query.clear();
        queryString = QString("INSERT INTO measured_spectra (id, spectrum_data, measured_samples_id, measuring_conditions_id) "
                              "VALUES (:id, :spectrum_data, :measured_samples_id, :measuring_conditions_id)");

        if(!query.prepare(queryString))
        {
            qDebug() << query.lastError().text();
            return;
        }

        query.bindValue(":id", newSpectrumId);
        query.bindValue(":spectrum_data", spectrumByteArray);
        query.bindValue(":measured_samples_id", newSampleId);
        query.bindValue(":measuring_conditions_id", it.key());

        if(!query.exec())
        {
            qDebug() << query.lastError().text();
            return ;
        }
    }

    QMap<int, QString> chemicalMap = currentSample->zp_sampleChemicalMap();

    QString testStr = "Chemicals: ";
    QMap<int, QString>::iterator chemIt;
    for(chemIt = chemicalMap.begin(); chemIt != chemicalMap.end(); chemIt++)
    {
        // test output
        double concentration = 0;
        currentSample->zp_concentration(chemIt.key(), concentration);

        // end test output
        int newChemicalId = 0;
        if(!zh_findNewIdInTable("measured_chemicals", newChemicalId))
        {
            // cannot obtain new spectrum id
            continue;
        }

        query.clear();
        queryString = QString("INSERT INTO measured_chemicals (id, chemicals_id, measured_samples_id, value) "
                              "VALUES (:id, :chemicals_id, :measured_samples_id, :value)");

        if(!query.prepare(queryString))
        {
            qDebug() << query.lastError().text();
            return;
        }

        query.bindValue(":id", newChemicalId);
        query.bindValue(":chemicals_id", chemIt.key());
        query.bindValue(":measured_samples_id", newSampleId);
        query.bindValue(":value", concentration);

        if(!query.exec())
        {
            qDebug() << query.lastError().text();
            return ;
        }
    }
}
//======================================================
bool ZMeasuringManager::zh_recordSeriesId()
{
    // check series id
    QSqlQuery query;
    QString queryString = QString("SELECT id FROM series "
                                  "WHERE id=%1").arg(QString::number(zv_seriesId));

    if(!query.prepare(queryString))
    {
        return false;
    }

    if(!query.exec())
    {
        return false;
    }

    if(query.next())
    {
        QVariant vData = query.value(0);
        if(vData.isValid() && vData.canConvert<int>() && vData.toInt() == zv_seriesId)
        {
            qDebug() << "Series Id is already recorded: " << zv_seriesId;
            return true;
        }
    }

    // record series id
    query.clear();
    queryString = QString("INSERT INTO series (id, date, time, operators_id, series_tasks_id) "
                          "VALUES (:id, :date, :time, :operators_id, :series_tasks_id)");

    if(!query.prepare(queryString))
    {
        return false;
    }

    query.bindValue(":id", zv_seriesId);
    query.bindValue(":date", zv_startDateTime.date());
    query.bindValue(":time", zv_startDateTime.time());

    int operatorId = 0;
    emit zg_inquiryCurrentOperatorId(operatorId);
    query.bindValue(":operators_id", operatorId);
    query.bindValue(":series_tasks_id", zv_currentMeasuringState.zp_currentSeriesTaskId());

    if(!query.exec())
    {
        return false;
    }

    qDebug() << "New Series Id: " << zv_seriesId ;

    return true;
}
//======================================================
void ZMeasuringManager::zp_stopSeries()
{
    zv_seriesTimePassed = 0;
    zv_finishDateTime = QDateTime::currentDateTime();

    if(zv_expositionDelayTimer)
    {
        killTimer(zv_expositionDelayTimer);
        zv_expositionDelayTimer = 0;
    }

    //    if(zv_currentMeasuringState.zp_currentSampleRow() >= 0
    //            && zv_currentMeasuringState.zp_currentSampleRow() < zv_sampleList.count() )
    //    {
    //        zv_sampleList.at(zv_currentMeasuringState.zp_currentSampleRow())->zp_stopMeasuring();
    //    }

    zv_measuringController->zp_stopMeasuring();
    zv_currentMeasuringState.zp_setMeasuringAction(ZMeasuringState::MA_STOPPED);
    zh_notifyMeasuringStateChanged();

    emit zg_invokeNotifyCurrent();

    // QMessageBox::information(0, "MM", "STOP SERIES", QMessageBox::Ok);
}
//======================================================
void ZMeasuringManager::zh_onSampleMeasuringFinish()
{
    zh_saveSampleMeasurementResult();

    if(zv_deviceSampleQuantity < 1)
    {
        zp_stopSeries();
        return;
    }

    // check end of series
    if(zv_currentMeasuringState.zp_currentSampleRow() + 1 >= zv_sampleList.count()
            || zv_currentMeasuringState.zp_currentSampleRow() + 1 < 0)
    {
        // series has been mesured
        zp_stopSeries();
        return;
    }

    zv_seriesTimePassed += zv_sampleList.at(zv_currentMeasuringState.zp_currentSampleRow())->zp_totalMeasuringDuration();

    // next sampleIndex
    zv_currentMeasuringState.zp_incrementSampleRow();
    // sample name

    // check sample set is already measured
    if(zv_currentMeasuringState.zp_currentSampleRow()  % zv_deviceSampleQuantity == 0
            && zv_currentMeasuringState.zp_currentSampleRow() != 0)
    {
        // change samples
        zv_currentMeasuringState.zp_setMeasuringAction(ZMeasuringState::MA_SUSPENDED);
        zh_notifyMeasuringStateChanged();

        if(zv_deviceSampleQuantity > 1)
        {
            QString msg = tr("Replace sample set and press the \"Start\" button for continue measuring.");
            QMessageBox::information(nullptr, qApp->property("glAppProduct").toString(), msg, QMessageBox::Ok);
        }
        return;
    }

    // series measurement in progress: start next
    // QApplication::processEvents(QEventLoop::AllEvents);
    zv_currentMeasuringState.zp_setSeriesTimePassed(zv_seriesTimePassed);
    zh_notifyMeasuringStateChanged();
    zv_currentMeasuringState.zp_setCurrentSampleName(zv_sampleList.at(zv_currentMeasuringState.zp_currentSampleRow())->zp_sampleName());

    int sampleDuration = zv_sampleList.at(zv_currentMeasuringState.zp_currentSampleRow())->zp_totalMeasuringDuration();
    zv_currentMeasuringState.zp_setSampleDuration(sampleDuration);
    zv_currentMeasuringState.zp_setSampleTimePassed(0);

    //    zh_notifyMeasuringStateChanged();

    // sample mesauring delay
    if(zv_expositionDelayDuration <= 0)
    {
        zh_notifyMeasuringStateChanged();
        //        qDebug() << "STARTING SAMPLE" << zv_currentMeasuringState.zp_currentSampleRow();

        zv_sampleList.at(zv_currentMeasuringState.zp_currentSampleRow())->zp_startMeasuring();

        //        qDebug() << "SAMPLE LIST COUNT" << zv_sampleList.count();
        //        qDebug() << "CUR SAMPLE ROW" << zv_currentMeasuringState.zp_currentSampleRow();
    }
    else
    {
        zv_expositionDelayTimer = startTimer(zv_expositionDelayDuration * 1000);
    }
}
//======================================================
void ZMeasuringManager::timerEvent(QTimerEvent* event)
{
    if(event->timerId() == zv_expositionDelayTimer)
    {
        killTimer(zv_expositionDelayTimer);
        zv_expositionDelayTimer = 0;

        zv_seriesTimePassed += zv_expositionDelayDuration;
        zh_notifyMeasuringStateChanged();
        //        qDebug() << "STARTING SAMPLE (TIMER EVENT)" << zv_currentMeasuringState.zp_currentSampleRow();

        zv_sampleList.at(zv_currentMeasuringState.zp_currentSampleRow())->zp_startMeasuring();
    }
}
//======================================================
void ZMeasuringManager::zh_onSpectrumDataChange(quint8 gainFactor, int exposition)
{
    // qDebug() << "MM: ON SPECTRUM DATA CHANGE ";
    ZSample* sample = qobject_cast<ZSample*>(sender());
    if(!sample)
    {
        return;
    }
    int row = 0;
    bool res = false;
    for(; row < zv_sampleList.count(); row++)
    {
        if(zv_sampleList.at(row) == sample)
        {
            res = true;
            break;
        }
    }

    if(!res)
    {
        // sample is not found
        return;
    }

    zh_calcSpectrumCommonProperties(gainFactor, exposition);
    emit zg_sampleOperation(SOT_SPECTRUM_CHANGED, row, row);
}
//======================================================
void ZMeasuringManager::zh_onConcentrationChange()
{
    ZSample* sample = qobject_cast<ZSample*>(sender());
    if(!sample)
    {
        return;
    }
    int row = 0;
    bool res = false;
    for(; row < zv_sampleList.count(); row++)
    {
        if(zv_sampleList.at(row) == sample)
        {
            res = true;
            break;
        }
    }

    if(!res)
    {
        // sample is not found
        return;
    }

    emit zg_sampleOperation(SOT_CONCENTRATIONS_CHANGED, row, row);

}
//======================================================
void ZMeasuringManager::zh_onCurrentSpectrumChange(qint64 spectrumId)
{
    if(zv_currentMeasuringState.zp_measuringAction() == ZMeasuringState::MA_RUNNING)
    {
        return;
    }

    emit zg_currentSpectrumChanged(spectrumId);
    zh_manageControlEnable();
    //    QMap<QPair<quint8, int>, QList<ZSpeSpectrum*> > selectedSpectrumList;
    //    emit zg_inquirySelectedSpectrumMap(selectedSpectrumList);
    //    zv_saveSpectraToFilesAction->setEnabled(!selectedSpectrumList.isEmpty());
}
//======================================================
void ZMeasuringManager::zh_currentEnergyCalibrationChanged(QList<double> energyCalibrationFactorList)
{
    if(zv_currentMeasuringState.zp_measuringAction() == ZMeasuringState::MA_RUNNING)
    {
        return;
    }

    emit zg_currentEnergyCalibrationChanged(energyCalibrationFactorList);
}
//======================================================
void ZMeasuringManager::zh_onSaveSeriesAction()
{
    ZSeriesTaskListManager seriesManager;
    ZSeriesTaskListDialog dialog(false);
    dialog.zp_connectToManager(&seriesManager);
    connect(&seriesManager, &ZSeriesTaskListManager::zg_inquirySampleTaskIdList,
            this, &ZMeasuringManager::zh_sampleTaskIdList);

    if(!dialog.exec())
    {
        return;
    }

    zv_currentMeasuringState.zp_setSeriesName(dialog.zp_newSeriesTaskName());
    // emit zg_seriesTaskNameChanged(zv_currentSeriesTaskName);

    zv_currentMeasuringState.zp_setSeriesTaskDirty(false);

    qint64 seriesTaskId = dialog.zp_newSeriesTaskId();
    zv_currentMeasuringState.zp_setSeriesTaskId(seriesTaskId);

    //emit zg_seriesTaskNameDirtyChanged(zv_currentSeriesTaskDirty);
    zh_notifyMeasuringStateChanged();


}
//======================================================
void ZMeasuringManager::zh_onLoadSeriesAction()
{
    if(zv_currentMeasuringState.zp_seriesTaskDirty())
    {
        QString questionString = tr("Current series task has been changed. "
                                    "Do you want to save it?");
        if(QMessageBox::question(nullptr, tr("Series task loading"), questionString,
                                 QMessageBox::Yes, QMessageBox::No) == QMessageBox::Yes)
        {
            zh_onSaveSeriesAction();
        }
    }

    ZSeriesTaskListManager seriesManager;
    ZSeriesTaskListDialog dialog(true);
    dialog.zp_connectToManager(&seriesManager);

    if(!dialog.exec())
    {
        return;
    }

    zv_currentMeasuringState = ZMeasuringState();
    int seriesTaskId = seriesManager.zp_currentSeriesTaskId();
    zh_clearSeriesTask();
    zh_loadSeriesTask(seriesTaskId);
    zv_currentMeasuringState.zp_setSeriesTaskDirty(false);

    // set current measuring state
    // reset first
    if(!zv_sampleList.isEmpty())
    {
        zv_currentMeasuringState.zp_setCurrentSampleName(zv_sampleList.at(0)->zp_sampleName());
        zv_currentMeasuringState.zp_setMeasuringAction(ZMeasuringState::MA_STOPPED);
    }

    zh_recalcSeriesMeasuringTotalDuration();
    zh_manageControlEnable();
    zh_notifyMeasuringStateChanged();
}
//======================================================
void ZMeasuringManager::zh_clearSeriesTask()
{
    for(int sampleIndex = zv_sampleList.count() - 1; sampleIndex >= 0; sampleIndex--)
    {
        emit zg_sampleOperation(SOT_SAMPLE_ABOUT_TO_BE_REMOVED, sampleIndex, sampleIndex);
        ZSample* sample = zv_sampleList.takeAt(sampleIndex);
        delete sample;
        emit zg_sampleOperation(SOT_SAMPLE_REMOVED, sampleIndex, sampleIndex);
    }

}
//======================================================
bool ZMeasuringManager::zh_loadSeriesTask(int seriesTaskId)
{
    QSqlQuery query;
    QString queryString = QString("SELECT * FROM series_tasks "
                                  "WHERE id=%1").arg(QString::number(seriesTaskId));

    if(!query.prepare(queryString))
    {
        return false;
    }

    if(!query.exec())
    {
        return false;
    }

    if(!query.next())
    {
        return false;
    }

    // series task name
    QVariant vData = query.value(1);
    if(!vData.isValid() || !vData.canConvert<QString>())
    {
        return false;
    }

    QString seriesTaskName = vData.toString();
    // emit zg_seriesTaskNameChanged(zv_currentSeriesTaskName);

    // description
    vData = query.value(2);
    if(!vData.isValid() || !vData.canConvert<QString>())
    {
        return false;
    }

    // samples
    query.clear();
    queryString = QString("SELECT sample_tasks_id FROM sample_tasks_has_series_tasks "
                          "WHERE series_tasks_id=%1").arg(QString::number(seriesTaskId));

    if(!query.prepare(queryString))
    {
        return false;
    }

    if(!query.exec())
    {
        return false;
    }

    int sampleTaskId;
    while(query.next())
    {
        vData = query.value(0);
        if(!vData.isValid() || !vData.canConvert<int>())
        {
            continue;
        }

        sampleTaskId = vData.toInt();

        zh_addSamplesToSeries(sampleTaskId, 1, QString());
    }

    zv_currentMeasuringState.zp_setSeriesName(seriesTaskName);
    zv_currentMeasuringState.zp_setSeriesTaskId(seriesTaskId);
    // emit zg_seriesTaskNameChanged(zv_currentSeriesTaskName);

    return true;
}
//======================================================
void ZMeasuringManager::zh_onAddSamplesToSeriesAction()
{
    ZAddSampleDialog dialog;
    if(!dialog.exec())
    {
        return;
    }

    // get data from dialog
    int sampleTaskId = dialog.zp_sampleTaskId();
    QString defaultSampleName = dialog.zp_sampleName();
    int sampleQuantity = dialog.zp_sampleQuantity();

    zh_addSamplesToSeries(sampleTaskId, sampleQuantity, defaultSampleName);

    zh_manageControlEnable();
    zv_currentMeasuringState.zp_setSeriesTaskDirty(true);
    zv_currentMeasuringState.zp_setSeriesTaskId(0);
    zh_recalcSeriesMeasuringTotalDuration();
    zh_notifyMeasuringStateChanged();

}
//======================================================
bool ZMeasuringManager::zh_addSamplesToSeries(int sampleTaskId,
                                              int sampleQuantity,
                                              QString defaultSampleName)
{
    if(defaultSampleName.isEmpty())
    {
        QSqlQuery query;
        QString queryString = QString("SELECT sample_name_template FROM sample_tasks "
                                      "WHERE id=%1").arg(QString::number(sampleTaskId));
        if(!query.prepare(queryString))
        {
            return false;
        }

        if(!query.exec())
        {
            return false;
        }

        if(!query.next())
        {
            return false;
        }

        QVariant vData = query.value(0);
        if(!vData.isValid() || !vData.canConvert<QString>())
        {
            return false;
        }

        defaultSampleName = vData.toString();
    }

    // check data
    if(sampleTaskId < 0 || sampleQuantity < 1)
    {
        return false;
    }

    // get sample task
    ZSampleTask* task = zh_instanceSampleTask(sampleTaskId);

    if(task == nullptr)
    {
        return false;
    }

    // create samples
    int lastSampleSerialNumber = zh_findLastSampleSerialNumber(defaultSampleName);
    QString newSampleName;
    QString serialNumberString;
    // serial index correction
    if(lastSampleSerialNumber < 0 && sampleQuantity > 1)
    {
        lastSampleSerialNumber = 0;
    }

    for(int i = 0; i < sampleQuantity; i++)
    {
        for(int c = 0; c < 999999999; c++)
        {
            if(++lastSampleSerialNumber == 0)
            {
                serialNumberString = QString();
            }
            else
            {
                serialNumberString = "-" + QString::number(lastSampleSerialNumber);
            }
            newSampleName = defaultSampleName + serialNumberString;
            if(zh_checkSampleName(newSampleName))
            {
                break;
            }
        }

        zh_createSample(newSampleName, task);
    }

    return true;
}
//======================================================
void ZMeasuringManager::zh_onRemoveSamplesFromSeriesAction()
{
    QList<int> selectedSampleList;
    emit zg_inquirySelectedSampleList(selectedSampleList);

    qSort(selectedSampleList);

    int sampleIndex;
    QList<QPair<quint8, int> > sampleMeasuringConditionsList;
    for(int i = selectedSampleList.count() - 1; i >= 0; i--)
    {
        sampleIndex = selectedSampleList.at(i);
        emit zg_sampleOperation(SOT_SAMPLE_ABOUT_TO_BE_REMOVED, sampleIndex, sampleIndex);
        ZSample* sample = zv_sampleList.takeAt(sampleIndex);
        sampleMeasuringConditionsList = sample->zp_sampleMeasuringConditionsList();
        delete sample;

        emit zg_sampleOperation(SOT_SAMPLE_REMOVED, sampleIndex, sampleIndex);

        for(int i = 0; i < sampleMeasuringConditionsList.count(); i++)
        {
            zh_calcSpectrumCommonProperties(sampleMeasuringConditionsList.value(i).first, sampleMeasuringConditionsList.value(i).second);
        }
    }

    if(zv_sampleList.isEmpty())
    {
        zv_currentMeasuringState.zp_setSeriesName(QString());
    }

    zv_currentMeasuringState.zp_setSeriesTaskDirty(!zv_sampleList.isEmpty());
    zv_currentMeasuringState.zp_setSeriesTaskId(0);

    zh_recalcSeriesMeasuringTotalDuration();
    zh_manageControlEnable();
    zh_notifyMeasuringStateChanged();

}
//======================================================
void ZMeasuringManager::zh_onLoadSpectraFromFilesAction()
{
    // get spe file list
    QStringList fileNameList = QFileDialog::getOpenFileNames(nullptr, tr("Select spectrum file"),
                                                             zv_spectrumFolderPath,
                                                             tr("SRV spectrum files(*.spe);;All files(*.*)"));
    if(fileNameList.isEmpty())
    {
        return;
    }

    // save spe files path
    QFileInfo fileInfo(fileNameList.first()) ;
    zv_spectrumFolderPath = fileInfo.absolutePath();

    // create ZSpeSpectrum list
    QList<ZSpeSpectrum*> spectrumList;
    ZAbstractSpectrum* abstractSpectrum;
    ZSpeSpectrum* speSpectrum;
    ZSpeIOHandler* speHandler = new ZSpeIOHandler(this, this);
    QColor color;

    foreach(QString fileName, fileNameList)
    {
        zh_nextSpectrumColor(color);
        if(!speHandler->zp_getSpectrumFromFile(fileName, color, abstractSpectrum))
        {
            QString msg = tr("Cannot load spectrum from file \"%1\"!").arg(fileName);
            QMessageBox::critical(nullptr, tr("Spectra loading"), msg, QMessageBox::Ok);
            return;
        }

        speSpectrum = qobject_cast<ZSpeSpectrum*>(abstractSpectrum);
        spectrumList.append(speSpectrum);
    }
    delete speHandler;

    // get gain factor from user
    ZSetGainFactorToCalibrationDialog dialog;
    if(!dialog.exec())
    {
        return;
    }
    quint8 gainFactor = static_cast<quint8>(dialog.zp_gainFactor());

    // define starting sample and column for insertion
    int startSampleIndex = 0;
    QModelIndex currentIndex;
    emit zg_inquiryCurrentIndex(currentIndex);
    if(currentIndex.isValid())
    {
        startSampleIndex = currentIndex.row();
    }

    // remove existing samples if necessary
    if(!zv_sampleList.isEmpty())
    {
        QString msg = tr("Do you want to clear current sample list?");
        int res = QMessageBox::question(nullptr, tr("Spectra loading"), msg,
                                        QMessageBox::Yes, QMessageBox::No, QMessageBox::Cancel);
        if(res == QMessageBox::Cancel)
        {
            return;
        }
        else if(res == QMessageBox::Yes)
        {
            zh_clearSampleList();
            // reset start sample index
            startSampleIndex = 0;
        }
    }

    // if sample list is empty, create new samples
    if(zv_sampleList.isEmpty())
    {
        QString sampleName;
        for(int i = 0; i < fileNameList.count(); i++)
        {
            sampleName = tr("Sample - %1").arg(QString::number(i + 1));
            zh_createSample(sampleName, nullptr);

        }
    }

    // check spectra count and sample count for insertion equality
    if(zv_sampleList.count() - startSampleIndex != spectrumList.count())
    {
        QString msg = tr("The loading spectrum count is not equal to the sample count.\n"
                         "Do you want to load them anyway?");
        if(QMessageBox::warning(nullptr, tr("Spectra loading"), msg,
                                QMessageBox::Yes | QMessageBox::No) == QMessageBox::No)
        {
            return;
        }
    }

    // define exposition
    int exposition = 0;
    for(int s = 0; s < spectrumList.count(); s++)
    {
        if(exposition == 0)
        {
            exposition = spectrumList.at(s)->zp_speAuxData()->zp_exposition();
            continue;
        }

        if(exposition != spectrumList.at(s)->zp_speAuxData()->zp_exposition())
        {
            QString msg = tr("Some of the loaded spectra have different exposition.\n"
                             "Do you want to load them anyway?");
            if(QMessageBox::warning(nullptr, tr("Spectra loading"), msg,
                                    QMessageBox::Yes | QMessageBox::No) == QMessageBox::No)
            {
                return;
            }
            else
            {
                break;
            }
        }
    }

    // if spectra for defined measuring conditions exist, ask for further action
    for(int sm = startSampleIndex; sm < zv_sampleList.count(); sm++)
    {
        if(zv_sampleList.at(sm)->zp_spectrumForMeasuringConditions(gainFactor, exposition) != nullptr)
        {
            QString msg = tr("The samples already have spectra with such measuring conditions.\n"
                             "Do you want to replace them?");
            if(QMessageBox::warning(nullptr, tr("Spectra loading"), msg,
                                    QMessageBox::Yes | QMessageBox::No) == QMessageBox::No)
            {
                return;
            }
            else
            {
                break;
            }
        }
    }

    // put spectra into samples
    int speExposition;
    for(int sm = startSampleIndex, sp = 0;
        sm < zv_sampleList.count() && sp < spectrumList.count();
        sm++, sp++)
    {
        speExposition = spectrumList.at(sp)->zp_speAuxData()->zp_exposition();
        emit zg_sampleOperation(SOT_CONDITIONS_ABOUT_TO_BE_INSERTED, sm, sm);
        zv_sampleList.at(sm)->zp_addMeasuringConditions(gainFactor, speExposition);
        emit zg_sampleOperation(SOT_CONDITIONS_INSERTED, sm, sm);

        zv_sampleList.at(sm)->zp_setSpectrum(spectrumList.at(sp), gainFactor, speExposition);
        emit zg_sampleOperation(SOT_SPECTRUM_CHANGED, sm, sm);
        zh_calcSpectrumCommonProperties(gainFactor, speExposition);
    }

    if(!spectrumList.isEmpty())
    {
        // delete remaining spectra
        for(int sp = spectrumList.count() - 1; sp >= 0;  sp--)
        {
            if(spectrumList.at(sp)->parent() == this)
            {
                delete spectrumList.at(sp);
            }
        }
    }

    // change measuring manager state
    zh_recalcSeriesMeasuringTotalDuration();
    zv_currentMeasuringState.zp_setSeriesTaskDirty(false);
    zh_manageControlEnable();
    zh_notifyMeasuringStateChanged();
}
//======================================================
void ZMeasuringManager::zh_onSaveSpectraToFilesAction() const
{
    QMap< QPair<quint8, int>, QList<ZSpeSpectrum*> > spectrumMap;
    emit zg_inquirySelectedSpectrumMap(spectrumMap);

    if(spectrumMap.isEmpty())
    {
        return;
    }

    ZSaveSpectraToFilesDialog dialog(spectrumMap);
    dialog.setWindowFlags(Qt::Tool);

    dialog.exec();

}
//======================================================
void ZMeasuringManager::zh_onPrintAction() const
{
    emit zg_inquiryResultsPrinting();
}
//======================================================
void ZMeasuringManager::zh_onPreviewAndPrintAction() const
{
    emit zg_inquiryResultsPreviewAndPrinting();
}
//======================================================
//void ZMeasuringManager::zh_getSpectraFromIndexes(const QModelIndexList& selectedIndexes,
//                                                 QMap< QPair<quint8, int>, QList<ZSpeSpectrum*> >& spectrumMap) const
//{
//    foreach(QModelIndex index, selectedIndexes)
//    {
//        if(!index.isValid() || index.column() )
//        {
//            continue;
//        }
//    }
//}
//======================================================
void ZMeasuringManager::zh_onEnergyCalibrationAction()
{
    // get selected spectra
    QMap< QPair<quint8, int>, QList<ZSpeSpectrum*> > selectedSpectrumMap;
    emit zg_inquirySelectedSpectrumMap(selectedSpectrumMap);

    if(selectedSpectrumMap.isEmpty())
    {
        QString msg = tr("Select spectra for wich energy calibration is calculated!");
        QMessageBox::critical(nullptr, tr("Error"), msg, QMessageBox::Ok);
        return;
    }

    // create spectrum map for energyCalibrationDialog
    QMap<quint8, QList<ZSpeSpectrum*> > spectrumMap;
    for(auto & conditions : selectedSpectrumMap.keys())
    {
        for(auto& spectrum : selectedSpectrumMap.value(conditions))
        {
            spectrumMap[conditions.first].append(spectrum);
        }
    }

    ZEnergyCalibrationDialogV2 energyCalibrationDialogV2(spectrumMap);
    connect(&energyCalibrationDialogV2, &ZEnergyCalibrationDialogV2::zg_energyCalibrationChanged,
            this, &ZMeasuringManager::zh_onEnergyCalibrationChange);
    if(energyCalibrationDialogV2.exec())
    {

    }

    return;

    //    // create energy calibration widget
    //    ZEnergyCalibrationDialog * energyCalibrationDialog = new ZEnergyCalibrationDialog;
    //    connect(this, &ZMeasuringManager::zg_currentMeasuringConditions,
    //            energyCalibrationDialog, &ZEnergyCalibrationDialog::zp_setMeasuringConditionsAndSpectrum);
    //    connect(energyCalibrationDialog, &ZEnergyCalibrationDialog::zg_inquiryCurrentVisibleSceneRect,
    //            this, &ZMeasuringManager::zg_inquiryCurrentVisibleSceneRect);
    //    // current Index
    //    QModelIndex currentIndex;
    //    emit zg_inquiryCurrentIndex(currentIndex);
    //    qDebug() << "CURRENT INDEX VALIDITY" << currentIndex.isValid();
    //    // measuring conditions and spectrum for current index
    //    quint8 gainFactor = 0;
    //    int exposition = -1;
    //    const ZSpeSpectrum* spectrum = 0;
    //    emit zg_inquiryMeasuringConditionsAndSpectrumForIndex(currentIndex, gainFactor, exposition, spectrum);
    //    qDebug() << "CURRENT G F " << gainFactor << "CURRENT EXPO " << exposition << "CURRENT SPE " << spectrum;
    //    energyCalibrationDialog->zp_setMeasuringConditionsAndSpectrum(gainFactor, exposition, spectrum);
    //    energyCalibrationDialog->show();
    //    // create connections

}
//======================================================
void ZMeasuringManager::zh_onEnergyCalibrationChange(int gainFactor,
                                                     const QList<double>& energyCalibrationFactorList)
{
    foreach(ZSample* sample, zv_sampleList)
    {
        sample->zp_setEnergyCalibration(gainFactor, energyCalibrationFactorList);
    }

    foreach(ZSampleTask* sampleTask, zv_sampleTaskList)
    {
        sampleTask->zp_setEnergyCalibration(static_cast<quint8>(gainFactor), energyCalibrationFactorList);
    }

    // notify current spectrum
    emit zg_invokeNotifyCurrent();
}
//======================================================
void ZMeasuringManager::zh_clearSampleList()
{
    for(int sampleIndex = zv_sampleList.count() - 1; sampleIndex >= 0; sampleIndex--)
    {
        emit zg_sampleOperation(SOT_SAMPLE_ABOUT_TO_BE_REMOVED, sampleIndex, sampleIndex);
        ZSample* sample = zv_sampleList.takeAt(sampleIndex);
        delete sample;
        emit zg_sampleOperation(SOT_SAMPLE_REMOVED, sampleIndex, sampleIndex);
    }

    zv_currentMeasuringState.zp_setSeriesName(QString());
    zv_currentMeasuringState.zp_setSeriesTaskDirty(!zv_sampleList.isEmpty());
    zv_spectrumCommonPropertiesList.clear();

    zh_recalcSeriesMeasuringTotalDuration();
    zh_manageControlEnable();
    zh_notifyMeasuringStateChanged();
}
//======================================================
void ZMeasuringManager::zh_resetMeasuringResults()
{
    for(int s = 0; s < zv_sampleList.count(); s++)
    {
        zv_sampleList.at(s)->zp_resetMeasuringResults();
    }

    zv_finishDateTime = QDateTime();
    zv_startDateTime = QDateTime();

    zv_seriesId = -1;

    emit zg_sampleOperation(SOT_CONCENTRATIONS_CHANGED, 0, zv_sampleList.count());
    emit zg_sampleOperation(SOT_SPECTRUM_CHANGED, 0, zv_sampleList.count());

}
//======================================================
void ZMeasuringManager::zh_deleteSampleTask()
{
    ZSampleTask* sampleTask = qobject_cast<ZSampleTask*>(sender());
    if(!zv_sampleTaskList.contains(sampleTask))
    {
        return;
    }

    delete zv_sampleTaskList.takeAt(zv_sampleTaskList.indexOf(sampleTask));

    zv_currentMeasuringState.zp_setSeriesTaskDirty(true && !zv_sampleList.isEmpty());
    //    zh_recalcSeriesMeasuringTotalDuration();
    zh_manageControlEnable();
    zh_notifyMeasuringStateChanged();
}
//======================================================
void ZMeasuringManager::zh_sampleTaskIdList(QList<int>& idList) const
{
    for(int i = 0; i < zv_sampleList.count(); i++)
    {
        idList.append(zv_sampleList.at(i)->zp_sampleTaskId());
    }
}
//======================================================
ZSampleTask* ZMeasuringManager::zh_instanceSampleTask(int sampleTaskId)
{
    if(sampleTaskId < 0)
    {
        return nullptr;
    }

    // search in already created
    foreach(ZSampleTask* task, zv_sampleTaskList)
    {
        if(task->zp_id() == sampleTaskId)
        {
            return task;
        }
    }

    // create new sampleTask
    ZSampleTask* task;
    QString msg;
    if(ZSampleTask::zp_instanceSampleTaskObject(sampleTaskId,
                                                task,
                                                msg,
                                                zv_measuringController,
                                                this))
    {
        zv_sampleTaskList.append(task);

        connect(task, &ZSampleTask::zg_inquiryToDelete,
                this, &ZMeasuringManager::zh_deleteSampleTask);
        connect(task, &ZSampleTask::zg_invokeToStopMeasurenent,
                this, &ZMeasuringManager::zp_stopSeries);


        return task;
    }

    return nullptr;
}
//======================================================
int ZMeasuringManager::zh_createSample(const QString& sampleName, ZSampleTask* sampleTask)
{
    ZSample* sample = new ZSample(sampleName, nullptr, this);
    connect(sample, &ZSample::zg_inquirySpectrumColor,
            this, &ZMeasuringManager::zh_nextSpectrumColor);
    connect(sample, &ZSample::zg_measuringFinished,
            this, &ZMeasuringManager::zh_onSampleMeasuringFinish);
    connect(sample, &ZSample::zg_spectrumDataChanged,
            this, &ZMeasuringManager::zh_onSpectrumDataChange);
    connect(sample, &ZSample::zg_concentrationChanged,
            this, &ZMeasuringManager::zh_onConcentrationChange);


    sample->zp_setSampleTask(sampleTask, ZSample::STSF_CLEAR_SPE_LIST);
    int sampleIndex = zv_sampleList.count();
    emit zg_sampleOperation(SOT_SAMPLE_ABOUT_TO_BE_INSERTED, sampleIndex, sampleIndex);
    zv_sampleList.append(sample);
    emit zg_sampleOperation(SOT_SAMPLE_INSERTED, sampleIndex, sampleIndex);
    return sampleIndex;
}
//======================================================
void ZMeasuringManager::zh_nextSpectrumColor(QColor& color)
{
    color = zv_colorList.at(zv_lastColorIndex);
    // color index increment
    if(++zv_lastColorIndex >= zv_colorList.count())
    {
        zv_lastColorIndex = 0;
    }
}
//======================================================
int ZMeasuringManager::zh_findLastSampleSerialNumber(const QString &sampleName) const
{
    // return value : -1 - no smple with tne name exists, 0 - sample w/o serial number , other - serial number
    QString currentSampleName;
    int lastSerialNumber = -1;
    // QString extraPart;
    bool ok;
    int currentSerialNumber;
    int postFixLength;

    foreach(ZSample* sample, zv_sampleList)
    {
        currentSampleName = sample->zp_sampleName();
        if(!currentSampleName.startsWith(sampleName))
        {
            continue;
        }

        // try to convert right part to Int
        postFixLength = currentSampleName.length() - sampleName.length();
        if(postFixLength == 0)
        {
            if(lastSerialNumber < 0)
            {
                lastSerialNumber = 0;
            }
            continue;
        }

        currentSerialNumber = currentSampleName.right(postFixLength).toInt(&ok);
        if(!ok)
        {
            continue;
        }

        if(lastSerialNumber < currentSerialNumber)
        {
            lastSerialNumber = currentSerialNumber;
        }
    }

    return lastSerialNumber;
}
//======================================================
bool ZMeasuringManager::zh_checkSampleName(const QString& sampleName) const
{
    foreach(ZSample* item, zv_sampleList)
    {
        if(item->zp_sampleName() == sampleName)
        {
            return false;
        }
    }
    return true;
}
//======================================================
void ZMeasuringManager::zh_manageControlEnable()
{
    zv_saveSeriesAction->setDisabled(zv_sampleList.isEmpty() && zv_sampleTaskList.isEmpty());

    QMap<QPair<quint8, int>, QList<ZSpeSpectrum*> > selectedSpectrumList;
    emit zg_inquirySelectedSpectrumMap(selectedSpectrumList);
    zv_saveSpectraToFilesAction->setEnabled(!selectedSpectrumList.isEmpty());

    QList<int> selectedSampleList;
    emit zg_inquirySelectedSampleList(selectedSampleList);
    zv_removeSamplesFromSeriesAction->setEnabled(!selectedSampleList.isEmpty());
}
//======================================================
bool ZMeasuringManager::zh_findNewIdInTable(const QString& tableName, int& newId)
{
    QSqlQuery query;
    QString queryString = QString("SELECT MAX(id) FROM %1").arg(tableName);
    if(!query.prepare(queryString))
    {
        return false;
    }
    if(!query.exec())
    {
        return false;
    }

    if(!query.next())
    {
        newId = 1;
        return true;
    }

    QVariant vData = query.value(0);
    if(!vData.isValid() || !vData.canConvert<int>())
    {
        return false;
    }

    newId = vData.toInt() + 1;

    return true;
}
//======================================================
