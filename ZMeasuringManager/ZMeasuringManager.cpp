//======================================================
#include "ZMeasuringManager.h"
#include "ZGeneral.h"

#include "ZAddSampleDialog.h"
#include "ZControlAction.h"
#include "ZSetGainFactorToCalibrationDialog.h"
#include "ZSample.h"
#include "ZSampleTask.h"
#include "ZSeriesTaskListDialog.h"
#include "ZSeriesTaskListManager.h"
#include "ZSpeIOHandler.h"
#include "ZSpeSpectrum.h"

#include <QColor>
#include <QDateTime>
#include <QFileDialog>
#include <QMessageBox>
#include <QFileInfo>
#include <QSettings>

//======================================================
ZMeasuringManager::ZMeasuringManager(QObject *parent)
    : QObject(parent)
{
    zv_currentSampleIndex = -1;
    zv_currentSeriesTaskDirty = false;
    zv_lastColorIndex = 0;
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

    if(abs(averageVol - color.red()) + abs(averageVol - color.green()) + abs(averageVol - color.blue()) < 90)
    {
        return false;
    }

    return true;
}
//======================================================
void ZMeasuringManager::zh_createActions()
{
    zv_saveSeriesAction  = new ZControlAction(this);
    zv_saveSeriesAction->setText(tr("Save series"));

    zv_loadSeriesAction  = new ZControlAction(this);
    zv_loadSeriesAction->setText(tr("Load series"));

    zv_addSamplesToSeriesAction = new ZControlAction(this);
    zv_addSamplesToSeriesAction->setText(tr("Add samples"));

    zv_removeSamplesFromSeriesAction = new ZControlAction(this);
    zv_removeSamplesFromSeriesAction->setText(tr("Remove samples"));

    zv_loadSpectrumFromFileAction   = new ZControlAction(this);
    zv_loadSpectrumFromFileAction->setText(tr("Load spectra from files"));
}
//======================================================
void ZMeasuringManager::zh_createComponents()
{

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

    connect(zv_loadSpectrumFromFileAction, &ZControlAction::triggered,
            this, &ZMeasuringManager::zh_onLoadSpectraFromFileAction);

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
QList<ZControlAction*> ZMeasuringManager::zp_sampleActions() const
{
    QList<ZControlAction*> actionList;
    actionList.append(zv_saveSeriesAction);
    actionList.append(zv_loadSeriesAction);

    actionList.append(zv_addSamplesToSeriesAction);
    actionList.append(zv_removeSamplesFromSeriesAction);
    actionList.append(zv_loadSpectrumFromFileAction);

    return actionList;
}
//======================================================
QList<ZControlAction*> ZMeasuringManager::zp_sampleContextActions() const
{
    QList<ZControlAction*> actionList;
    actionList.append(zv_saveSeriesAction);
    actionList.append(zv_loadSeriesAction);

    actionList.append(zv_addSamplesToSeriesAction);
    actionList.append(zv_removeSamplesFromSeriesAction);
    actionList.append(zv_loadSpectrumFromFileAction);

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
bool ZMeasuringManager::zp_setSampleName(int sampleIndex, const QString& name)
{
    if(sampleIndex < 0 || sampleIndex >= zv_sampleList.count())
    {
        return false;
    }

    return zv_sampleList.at(sampleIndex)->zp_setSampleName(name);
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
QList<QPair<int, int> > ZMeasuringManager::zp_measuringConditionsListForSample(int sampleIndex) const
{
    if(sampleIndex < 0 || sampleIndex >= zv_sampleList.count())
    {
        return QList<QPair<int, int> >();
    }

    return zv_sampleList.at(sampleIndex)->zp_sampleMeasuringConditionsList();
}
//======================================================
QString ZMeasuringManager::zp_seriesTaskName() const
{
    return zv_currentSeriesTaskName;
}
//======================================================
ZMeasuringState ZMeasuringManager::zp_currentMeasuringState() const
{
    ZMeasuringState measuringState;
    zh_createCurrentMeasuringState(measuringState);
    return measuringState;
}
//======================================================
QList<int> ZMeasuringManager::zp_spectrumData(int row, int gainFactor, int exposition)
{
    QList<int> data;
    data << 1000 << 2000 << 3000 << 2000 << 1000;
    return data;
}
//======================================================
int ZMeasuringManager::zp_arrayChannelCount(int gainFactor, int exposition) const
{
    return 5;
}
//======================================================
int ZMeasuringManager::zp_arrayMaxIntensity(int gainFactor, int exposition) const
{
    return 5000;
}
//======================================================
void ZMeasuringManager::zh_createCurrentMeasuringState(ZMeasuringState& measuringState) const
{
    measuringState.currentSeriesName = zv_currentSeriesTaskName;
    measuringState.currentSampleName = "Nothing yet";
    measuringState.seriesDirty = zv_currentSeriesTaskDirty;
}
//======================================================
void ZMeasuringManager::zh_notifyMeasuringStateChanged()
{
    ZMeasuringState measuringState;
    zh_createCurrentMeasuringState(measuringState);
    emit zg_measuringStateChanged(measuringState);
}
//======================================================
int ZMeasuringManager::zh_seriesMeasuringTotalDuration() const
{
    int seriesMeasuringTotalDuration = 0;
    for(int i = 0; i < zv_sampleList.count(); i++)
    {

    }

    return seriesMeasuringTotalDuration;
}
//======================================================
void ZMeasuringManager::zp_notifyOfCurrentStatus()
{
    zh_notifyMeasuringStateChanged();
}
//======================================================
void ZMeasuringManager::zp_setCurrentSampleIndex(int currentSampleIndex)
{
    if(currentSampleIndex >= zv_sampleList.count())
    {
        zv_currentSampleIndex = -1;
        return;
    }

    zv_currentSampleIndex = currentSampleIndex;
}
//======================================================
void ZMeasuringManager::zp_startSeries()
{
#ifdef DBG
    qDebug() << "START SERIES";
#endif

    zh_notifyMeasuringStateChanged();
    // QMessageBox::information(0, "MM", "START SERIES", QMessageBox::Ok);
}
//======================================================
void ZMeasuringManager::zp_stopSeries()
{
#ifdef DBG
    qDebug() << "STOP SERIES";
#endif

    zh_notifyMeasuringStateChanged();

    // QMessageBox::information(0, "MM", "STOP SERIES", QMessageBox::Ok);
}
//======================================================
void ZMeasuringManager::zh_onSaveSeriesAction()
{
    ZSeriesTaskListManager seriesManager;
    ZSeriesTaskListDialog dialog(false);
    dialog.zp_connectToManager(&seriesManager);
    connect(&seriesManager, &ZSeriesTaskListManager::zg_requestSampleTaskIdList,
            this, &ZMeasuringManager::zh_sampleTaskIdList);

    if(!dialog.exec())
    {
        return;
    }

    zv_currentSeriesTaskName = dialog.zp_newSeriesTaskName();
    // emit zg_seriesTaskNameChanged(zv_currentSeriesTaskName);

    zv_currentSeriesTaskDirty = false;
    //emit zg_seriesTaskNameDirtyChanged(zv_currentSeriesTaskDirty);
    zh_notifyMeasuringStateChanged();
}
//======================================================
void ZMeasuringManager::zh_onLoadSeriesAction()
{
    if(zv_currentSeriesTaskDirty)
    {
        QString questionString = tr("Current series task has been changed. "
                                    "Do you want to save it?");
        if(QMessageBox::question(0, tr("Series task loading"), questionString,
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

    int seriesTaskId = seriesManager.zp_currentSeriesTaskId();
    zh_clearSeriesTask();
    zh_loadSeriesTask(seriesTaskId);
    zv_currentSeriesTaskDirty = false;

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

    zv_currentSeriesTaskName = seriesTaskName;
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
    zv_currentSeriesTaskDirty = true;
    // emit zg_seriesTaskNameDirtyChanged(zv_currentSeriesTaskDirty);
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

    if(task == 0)
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
    emit zg_requestSelectedSampleList(selectedSampleList);

    qSort(selectedSampleList);

    int sampleIndex;
    for(int i = selectedSampleList.count() - 1; i >= 0; i--)
    {
        sampleIndex = selectedSampleList.at(i);
        emit zg_sampleOperation(SOT_SAMPLE_ABOUT_TO_BE_REMOVED, sampleIndex, sampleIndex);
        ZSample* sample = zv_sampleList.takeAt(sampleIndex);
        delete sample;
        emit zg_sampleOperation(SOT_SAMPLE_REMOVED, sampleIndex, sampleIndex);
    }

    if(zv_sampleList.isEmpty())
    {
        zv_currentSeriesTaskName.clear();
    }

    zv_currentSeriesTaskDirty = !zv_sampleList.isEmpty();

    zh_manageControlEnable();
    zh_notifyMeasuringStateChanged();
}
//======================================================
void ZMeasuringManager::zh_onLoadSpectraFromFileAction()
{
    // get spe file list
    QStringList fileNameList = QFileDialog::getOpenFileNames(0, tr("Select spectrum file"),
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

    foreach(QString fileName, fileNameList)
    {
        if(!speHandler->zp_getSpectrumFromFile(fileName, zv_colorList.value(zv_lastColorIndex), abstractSpectrum))
        {
            QString msg = tr("Cannot load spectrum from file \"%1\"!").arg(fileName);
            QMessageBox::critical(0, tr("Spectra loading"), msg, QMessageBox::Ok);
            return;
        }

        speSpectrum = qobject_cast<ZSpeSpectrum*>(abstractSpectrum);
        spectrumList.append(speSpectrum);

        // color index increment
        if(++zv_lastColorIndex >= zv_colorList.count())
        {
            zv_lastColorIndex = 0;
        }
    }
    delete speHandler;

    // get gain factor from user
    ZSetGainFactorToCalibrationDialog dialog;
    if(!dialog.exec())
    {
        return;
    }
    int gainFactor = dialog.zp_gainFactor();

    // define starting sample and column for insertion
    int startSampleIndex = 0;
    QModelIndex currentIndex;
    emit zg_requestCurrentIndex(currentIndex);
    if(currentIndex.isValid())
    {
        startSampleIndex = currentIndex.row();
    }

     // remove existing samples if necessary
    if(!zv_sampleList.isEmpty())
    {
        QString msg = tr("Do you want to clear current sample list?");
        int res = QMessageBox::question(0, tr("Spectra loading"), msg,
                                        QMessageBox::Yes, QMessageBox::No, QMessageBox::Cancel);
        if(res == QMessageBox::Cancel)
        {
            return;
        }
        else if(res == QMessageBox::Yes)
        {
            zh_clearSamples();
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
            zh_createSample(sampleName, 0);

        }
    }

    // check spectra count and sample count for insertion equality
    if(zv_sampleList.count() - startSampleIndex != spectrumList.count())
    {
        QString msg = tr("The loading spectrum count is not equal to the sample count.\n"
                         "Do you want to load them anyway?");
        if(QMessageBox::warning(0, tr("Spectra loading"), msg,
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
            if(QMessageBox::warning(0, tr("Spectra loading"), msg,
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
        if(zv_sampleList.at(sm)->zp_spectrumForMeasuringConditions(gainFactor, exposition) != 0)
        {
            QString msg = tr("The samples already have spectra with such measuring conditions.\n"
                             "Do you want to replace them?");
            if(QMessageBox::warning(0, tr("Spectra loading"), msg,
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

    }

    // delete remaining spectra
    for(int sp = spectrumList.count() - 1; sp>= 0;  sp++)
    {
         if(spectrumList.at(sp)->parent() == this)
         {
             delete spectrumList.at(sp);
         }
    }

    // change measuring manager state
    zv_currentSeriesTaskDirty = false;
    zh_manageControlEnable();
    zh_notifyMeasuringStateChanged();

}
//======================================================
void ZMeasuringManager::zh_clearSamples()
{
    for(int sampleIndex = zv_sampleList.count() - 1; sampleIndex >= 0; sampleIndex--)
    {
        emit zg_sampleOperation(SOT_SAMPLE_ABOUT_TO_BE_REMOVED, sampleIndex, sampleIndex);
        ZSample* sample = zv_sampleList.takeAt(sampleIndex);
        delete sample;
        emit zg_sampleOperation(SOT_SAMPLE_REMOVED, sampleIndex, sampleIndex);
    }

    zv_currentSeriesTaskName.clear();
    zv_currentSeriesTaskDirty = !zv_sampleList.isEmpty();
    zh_manageControlEnable();
    zh_notifyMeasuringStateChanged();
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

    zv_currentSeriesTaskDirty = true && !zv_sampleList.isEmpty();
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
        return 0;
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
    if(ZSampleTask::zp_instanceSampleTaskObject(sampleTaskId, task, msg, this))
    {
        zv_sampleTaskList.append(task);
        connect(task, &ZSampleTask::zg_requestToDelete,
                this, &ZMeasuringManager::zh_deleteSampleTask);

        return task;
    }

    return 0;
}
//======================================================
int ZMeasuringManager::zh_createSample(const QString& sampleName, ZSampleTask* sampleTask)
{
    ZSample* sample = new ZSample(sampleName, sampleTask, this);
    int sampleIndex = zv_sampleList.count();
    emit zg_sampleOperation(SOT_SAMPLE_ABOUT_TO_BE_INSERTED, sampleIndex, sampleIndex);
    zv_sampleList.append(sample);
    emit zg_sampleOperation(SOT_SAMPLE_INSERTED, sampleIndex, sampleIndex);
    return sampleIndex;
}
//======================================================
int ZMeasuringManager::zh_findLastSampleSerialNumber(const QString &sampleName) const
{
    // return value : -1 - no smple with tne name exists, 0 - sample w/o serial number , other - serial number
    QString currentSampleName;
    int lastSerialNumber = -1;
    QString extraPart;
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
}
//======================================================
