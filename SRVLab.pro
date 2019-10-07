#-------------------------------------------------
#
# Project created by QtCreator 2016-06-23T14:56:28
#
#-------------------------------------------------

QT       += core gui
QT       += sql
QT       += printsupport

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

#PRO VARS
#Application version
RC_ICONS = "ZImages/SRVLab-8.ico"

VER_MAJ=1
VER_MIN=0
VER_PAT=3

VER_RELEASE=b

CONFIG += $$VER_RELEASE
CONFIG += c++11

#DEBUG SETTINGS
CONFIG(release, debug|release):DEFINES += QT_NO_DEBUG_OUTPUT
#by default defined: in Debug mode QT_DEBUG, in Release mode QT_NO_DEBUG

EXE_BASE_NAME=SRVLab
QMAKE_TARGET_PRODUCT="SRV Lab"
QMAKE_TARGET_DESCRIPTION="Chemical analysis of X-ray spectra"
QMAKE_TARGET_COMPANY="TechnoAnalyt"
QMAKE_TARGET_COPYRIGHT="Copyright © $${QMAKE_TARGET_COMPANY} Ltd. 2017, 2018.  All rights reserved."
COMPANY_URL=tehnoanalit.com

VERSION=$${VER_MAJ}.$${VER_MIN}.$${VER_PAT}

#Target version
CONFIG(debug, debug|release):{
TARGET=$${EXE_BASE_NAME}-$${VERSION}
}else{
TARGET=$${EXE_BASE_NAME}-$${VERSION}.$${VER_RELEASE}
}

#Define the preprocessor macro to get the application version in the application.
DEFINES += APP_PRODUCT=\"\\\"$${QMAKE_TARGET_PRODUCT}\\\"\"
DEFINES += APP_EXE_BASE_NAME=\"\\\"$${EXE_BASE_NAME}\\\"\"
DEFINES += APP_VERSION=\"\\\"$${VERSION}.$${VER_RELEASE}\\\"\"
DEFINES += APP_COMPANY=\"\\\"$${QMAKE_TARGET_COMPANY}\\\"\"
DEFINES += APP_COMPANY_URL=\"\\\"$${COMPANY_URL}\\\"\"
DEFINES += APP_COPYRIGHT=\"\\\"$${QMAKE_TARGET_COPYRIGHT}\\\"\"

TRANSLATIONS = $${EXE_BASE_NAME}_ru.ts
QMAKE_CXXFLAGS_WARN_ON += -Wno-unused-parameter

#END OF CHIEF SETTINGS

#TARGET = SDAnalyzer
TEMPLATE = app

INCLUDEPATH += ZDialogs \
    ZComponents \
    ZModelViewWidgets \
    ZWidgets \
    ZPlotter \
    ZModels \
    ZModels/ZCurrentMeasuringTaskTreeModel \
    ZMeasuringManager \
    ZSampleTasks \
    ZMeasuringConditions \
    ZChemicalTasks \
    ZMeasuringTaskInitStruct \
    ZCalibrations \
    ZChemicals \
    ZModelViewWidgets/ZNumericDelegate \
    ZSeriesTasks \
    ZMeasuringManager/ZSpectrumTableDelegate \
    ZEnergyCalibrator \
    ZPeriodicTableDialog \
    ZPeriodicTable \
    ZEnergyLineManager \
    ZSeriesMeasurementDialog

SOURCES += main.cpp\
        MainWindow.cpp \
    ZComponents/ZTranslatorManager.cpp \
    ZDialogs/ZStartDialog.cpp \
    ZDialogs/ZDatabasePropertiesDialog.cpp \
    ZComponents/ZDatabaseInspector.cpp \
    ZComponents/ZXMLCalibrationIOHandler.cpp \
    ZComponents/ZCalibration.cpp \
    ZComponents/ZCalibrationWindow.cpp \
    ZComponents/ZTermNormalizer.cpp \
    ZComponents/ZCalibrationQualityData.cpp \
    ZComponents/ZRawTerm.cpp \
    ZComponents/ZRawWindow.cpp \
    ZComponents/ZAbstractSpectrum.cpp \
    ZComponents/ZAbstractSpectrumAuxData.cpp \
    ZComponents/ZCustomTerm.cpp \
    ZComponents/ZMixedTerm.cpp \
    ZComponents/ZQuadraticTerm.cpp \
    ZComponents/ZSimpleTerm.cpp \
    ZComponents/ZEquationSettingsData.cpp \
    ZComponents/ZNormaSettingsData.cpp \
    ZComponents/ZQrealToStringConverter.cpp \
    ZSeriesMeasurementDialog/ZDependentModel.cpp \
    ZSeriesMeasurementDialog/ZDependentModelController.cpp \
    ZSeriesMeasurementDialog/ZSeriesTableModel.cpp \
    ZWidgets/ZProcessTimeRoundIndicator.cpp \
    ZWidgets/QRoundProgressBar.cpp \
    ZDialogs/ZSettingsDialog.cpp \
    ZWidgets/ZDashboard.cpp \
    ZComponents/ZAbstractTerm.cpp \
    ZComponents/ZDashboardSettings.cpp \
    ZComponents/ZAppSettings.cpp \
    ZWidgets/ZAbstractProcessTimeIndicator.cpp \
    ZWidgets/ZProcessTimeClassicIndicator.cpp \
    ZComponents/ZProgressBarOptions.cpp \
    ZWidgets/ZLabeledSliderWidget.cpp \
    ZModelViewWidgets/ZBaseTableWidget.cpp \
    ZWidgets/ZWidgetWithSidebar.cpp \
    ZWidgets/ZClickableLabel.cpp \
    ZPlotter/ZDefaultRectGraphicsItem.cpp \
    ZPlotter/ZHorizontalDashBoard.cpp \
    ZPlotter/ZPlotGraphicsScene.cpp \
    ZPlotter/ZPlotGraphicsView.cpp \
    ZPlotter/ZPlotter.cpp \
    ZPlotter/ZRulersAndGridManager.cpp \
    ZPlotter/ZRulerWidget.cpp \
    ZPlotter/ZSpectrumGraphicsItem.cpp \
    ZPlotter/ZWindowGraphicsItem.cpp \
    ZComponents/ZVisibilityPointF.cpp \
    ZModelViewWidgets/ZBaseTreeWidget.cpp \
    ZComponents/ZControlAction.cpp \
    ZDialogs/ZAddCalibrationDialog.cpp \
    ZModels/ZCurrentMeasuringTaskTreeModel/ZSeriesTaskTreeModel.cpp \
    ZModels/ZCurrentMeasuringTaskTreeModel/ZItem.cpp \
    ZModels/ZCurrentMeasuringTaskTreeModel/ZSampleTaskItem.cpp \
    ZMeasuringManager/ZSample.cpp \
    ZMeasuringManager/ZAddSampleDialog.cpp \
    ZSampleTasks/ZSampleTaskListDialog.cpp \
    ZSampleTasks/ZSampleTaskTreeWidget.cpp \
    ZSampleTasks/ZSampleTaskTreeModel.cpp \
    ZSampleTasks/ZSampleTaskTreeItem.cpp \
    ZMeasuringConditions/ZMeasuringConditionTableModel.cpp \
    ZMeasuringConditions/ZMeasuringConditionsListDialog.cpp \
    ZChemicalTasks/ZChemicalTaskListDialog.cpp \
    ZMeasuringConditions/ZMeasuringConditionsDialog.cpp \
    ZComponents/ZReadOnlyStyledItemDelegate.cpp \
    ZMeasuringManager/ZMeasuringManager.cpp \
    ZMeasuringManager/ZMeasuringResultTableModel.cpp \
    ZModelViewWidgets/ZMeasuringCommonWidget.cpp \
    ZModelViewWidgets/ZMeasuringSeriesTaskTreeWidget.cpp \
    ZModelViewWidgets/ZMeasuringResultTableWidget.cpp \
    ZMeasuringManager/ZSampleTask.cpp \
    ZMeasuringTaskInitStruct/ZMeasuringTaskInitStruct.cpp \
    ZSampleTasks/ZSampleTaskDialog2.cpp \
    ZDialogs/ZSQLCommanderDialog.cpp \
    ZChemicalTasks/ZChemicalTaskTableWidget.cpp \
    ZChemicalTasks/ZChemicalTaskDialog.cpp \
    ZSampleTasks/ZSampleTaskTableWidget.cpp \
    ZCalibrations/ZCalibrationListDialog.cpp \
    ZChemicals/ZChemicalDialog.cpp \
    ZChemicals/ZChemicalListDialog.cpp \
    ZChemicals/ZChemicalTableWidget.cpp \
    ZCalibrations/ZCalibrationTableWidget.cpp \
    ZChemicalTasks/ZCalibrationInTaskTableModel.cpp \
    ZCalibrations/ZSetGainFactorToCalibrationDialog.cpp \
    ZCalibrations/ZCalibrationFilterProxyModel.cpp \
    ZChemicalTasks/ZChemicalTaskCalibrationModel.cpp \
    ZModelViewWidgets/ZNumericDelegate/ZNumericDelegate.cpp \
    ZModelViewWidgets/ZNumericDelegate/ZNumericEditor.cpp \
    ZGeneral.cpp \
    ZSeriesTasks/ZSeriesTaskListDialog.cpp \
    ZSeriesTasks/ZSeriesTaskListManager.cpp \
    ZSeriesTasks/ZSeriesTaskTableWidget.cpp \
    ZWidgets/ZStartStopButtonWidget.cpp \
    ZWidgets/ZSeriesLabelWidget.cpp \
    ZComponents/ZSpeIOHandler.cpp \
    ZComponents/ZSpeSpectrum.cpp \
    ZComponents/ZSpeAuxData.cpp \
    ZComponents/ZAbstractSpectrumIOHandler.cpp \
    ZMeasuringManager/ZSpectrumTableDelegate/ZPrimitivePlot.cpp \
    ZMeasuringManager/ZSpectrumTableDelegate/ZSpectrumTableDelegate.cpp \
    ZComponents/ZXMLSpectrumArrayIOHandler.cpp \
    ZDialogs/ZDashboardSettingsWidget.cpp \
    ZDialogs/ZDeviceSettingsWidget.cpp \
    ZComponents/ZDeviceSettings.cpp \
    ZComponents/ZUralAdcDeviceConnector.cpp \
    ZComponents/ZMeasuringController.cpp \
    ZComponents/ZPlotterDataManager.cpp \
    ZDialogs/ZCommonSettingsWidget.cpp \
    ZComponents/ZMeasurementParametersHandler.cpp \
    ZEnergyCalibrator/ZEnergyCalibrationDialog.cpp \
    ZEnergyCalibrator/ZEnergyCalibrator.cpp \
    ZEnergyCalibrator/ZPeakWindowTableModel.cpp \
    ZEnergyCalibrator/ZPeakWindow.cpp \
    ZDialogs/ZSaveSpectraToFilesDialog.cpp \
    ZEnergyCalibrator/ZEnergyCalibrationDialogV2.cpp \
    ZEnergyCalibrator/ZEnergyCalibrationSpectrumTableModel.cpp \
    ZComponents/ZLESGaussSolver.cpp \
    ZPlotter/ZVerticalLineGraphicsItem.cpp \
    ZComponents/ZEnergyCalibrationLine.cpp \
    ZWidgets/ZColorButton.cpp \
    ZPeriodicTableDialog/ZEnergyLineSelectionDialog.cpp \
    ZPeriodicTable/ZAbstractChemicalElementPropertyRepository.cpp \
    ZPeriodicTable/ZBaseDialog.cpp \
    ZPeriodicTable/ZCaptionFormatter.cpp \
    ZPeriodicTable/ZChemicalElementButton.cpp \
    ZPeriodicTable/ZChemicalElementPropertyIOManager.cpp \
    ZPeriodicTable/ZChemicalElementPropertyItem.cpp \
    ZPeriodicTable/ZChemicalElementPropertyTreeModel.cpp \
    ZPeriodicTable/ZChemicalElementProprtyEditor.cpp \
    ZPeriodicTable/ZChemicalPropertyProxyTableModel.cpp \
    ZPeriodicTable/ZPeriodicTableWidget.cpp \
    ZPeriodicTable/ZPropertyEditDialog.cpp \
    ZPeriodicTable/ZPropertySectionEditDialog.cpp \
    ZPlotter/ZEnergyLineGraphicsItem.cpp \
    ZEnergyLineManager/ZEnergyLineManager.cpp \
    ZEnergyLineManager/ZEnergyLineTableWidget.cpp \
    ZEnergyLineManager/ZSelectedEnergyLineTableModel.cpp \
    ZEnergyLineManager/ZEnergyLineSetItem.cpp \
    ZEnergyLineManager/ZEnergyLineDelegate.cpp \
    ZPlotter/ZTextGraphicsItem.cpp \
    ZPlotter/ZRulerToolGraphicsItem.cpp \
    ZWidgets/ZHelpBrowser.cpp \
    ZDialogs/ZOperatorDialog.cpp \
    ZDialogs/ZPasswordChangeDialog.cpp \
    ZDialogs/ZPasswordDialog.cpp \
    ZComponents/simplecrypt.cpp \
    ZModelViewWidgets/ZSampleTaskDelegate.cpp \
    ZSeriesMeasurementDialog/ZSeriesMeasurementDialog.cpp

HEADERS  += MainWindow.h \
    ZComponents/ZTranslatorManager.h \
    ZDialogs/ZStartDialog.h \
    ZDialogs/ZDatabasePropertiesDialog.h \
    ZComponents/ZDatabaseInspector.h \
    ZDialogs/ZAddCalibrationDialog.h \
    ZComponents/ZXMLCalibrationIOHandler.h \
    ZComponents/ZCalibration.h \
    ZComponents/ZCalibrationWindow.h \
    ZComponents/ZAbstractTerm.h \
    ZComponents/ZTermNormalizer.h \
    ZComponents/ZCalibrationQualityData.h \
    ZComponents/ZRawTerm.h \
    ZComponents/ZRawWindow.h \
    ZComponents/ZAbstractSpectrum.h \
    ZComponents/ZAbstractSpectrumAuxData.h \
    ZComponents/ZCustomTerm.h \
    ZComponents/ZMixedTerm.h \
    ZComponents/ZQuadraticTerm.h \
    ZComponents/ZSimpleTerm.h \
    ZComponents/ZEquationSettingsData.h \
    ZComponents/ZNormaSettingsData.h \
    ZComponents/ZQrealToStringConverter.h \
    ZSeriesMeasurementDialog/ZDependentModel.h \
    ZSeriesMeasurementDialog/ZDependentModelController.h \
    ZSeriesMeasurementDialog/ZSeriesTableModel.h \
    ZWidgets/ZProcessTimeRoundIndicator.h \
    ZWidgets/QRoundProgressBar.h \
    ZDialogs/ZSettingsDialog.h \
    ZWidgets/ZDashboard.h \
    ZComponents/ZDashboardSettings.h \
    ZComponents/ZAppSettings.h \
    ZWidgets/ZAbstractProcessTimeIndicator.h \
    ZWidgets/ZProcessTimeClassicIndicator.h \
    ZComponents/ZProgressBarOptions.h \
    ZWidgets/ZLabeledSliderWidget.h \
    ZModelViewWidgets/ZBaseTableWidget.h \
    ZWidgets/ZWidgetWithSidebar.h \
    ZWidgets/ZClickableLabel.h \
    ZPlotter/ZDefaultRectGraphicsItem.h \
    ZPlotter/ZGraphicsItemUserTypes.h \
    ZPlotter/ZHorizontalDashBoard.h \
    ZPlotter/ZPlotGraphicsScene.h \
    ZPlotter/ZPlotGraphicsView.h \
    ZPlotter/ZPlotter.h \
    ZPlotter/ZPlotterDefaulVariables.h \
    ZPlotter/ZRulersAndGridManager.h \
    ZPlotter/ZRulerWidget.h \
    ZPlotter/ZSpectrumGraphicsItem.h \
    ZPlotter/ZWindowGraphicsItem.h \
    ZComponents/ZVisibilityPointF.h \
    ZModelViewWidgets/ZBaseTreeWidget.h \
    ZComponents/ZControlAction.h \
    ZModels/ZCurrentMeasuringTaskTreeModel/ZSeriesTaskTreeModel.h \
    ZModels/ZCurrentMeasuringTaskTreeModel/ZItem.h \
    ZModels/ZCurrentMeasuringTaskTreeModel/ZSampleTaskItem.h \
    ZMeasuringManager/ZSample.h \
    ZMeasuringManager/ZAddSampleDialog.h \
    ZSampleTasks/ZSampleTaskListDialog.h \
    ZSampleTasks/ZSampleTaskTreeWidget.h \
    ZSampleTasks/ZSampleTaskTreeModel.h \
    ZSampleTasks/ZSampleTaskTreeItem.h \
    ZMeasuringConditions/ZMeasuringConditionsListDialog.h \
    ZMeasuringConditions/ZMeasuringConditionTableModel.h \
    ZChemicalTasks/ZChemicalTaskListDialog.h \
    ZMeasuringConditions/ZMeasuringConditionsDialog.h \
    ZComponents/ZReadOnlyStyledItemDelegate.h \
    ZMeasuringManager/ZMeasuringManager.h \
    ZMeasuringManager/ZMeasuringResultTableModel.h \
    ZModelViewWidgets/ZMeasuringCommonWidget.h \
    ZModelViewWidgets/ZMeasuringSeriesTaskTreeWidget.h \
    ZModelViewWidgets/ZMeasuringResultTableWidget.h \
    ZMeasuringManager/ZSampleTask.h \
    ZMeasuringTaskInitStruct/ZMeasuringTaskInitStruct.h \
    ZSampleTasks/ZSampleTaskDialog2.h \
    ZDialogs/ZSQLCommanderDialog.h \
    ZChemicalTasks/ZChemicalTaskTableWidget.h \
    ZChemicalTasks/ZChemicalTaskDialog.h \
    ZSampleTasks/ZSampleTaskTableWidget.h \
    ZCalibrations/ZCalibrationListDialog.h \
    ZChemicals/ZChemicalDialog.h \
    ZChemicals/ZChemicalListDialog.h \
    ZChemicals/ZChemicalTableWidget.h \
    ZCalibrations/ZCalibrationTableWidget.h \
    ZChemicalTasks/ZCalibrationInTaskTableModel.h \
    ZCalibrations/ZSetGainFactorToCalibrationDialog.h \
    ZCalibrations/ZCalibrationFilterProxyModel.h \
    ZChemicalTasks/ZChemicalTaskCalibrationModel.h \
    ZModelViewWidgets/ZNumericDelegate/ZNumericDelegate.h \
    ZModelViewWidgets/ZNumericDelegate/ZNumericEditor.h \
    ZGeneral.h \
    ZSeriesTasks/ZSeriesTaskListDialog.h \
    ZSeriesTasks/ZSeriesTaskListManager.h \
    ZSeriesTasks/ZSeriesTaskTableWidget.h \
    ZWidgets/ZStartStopButtonWidget.h \
    ZWidgets/ZSeriesLabelWidget.h \
    ZComponents/ZSpeIOHandler.h \
    ZComponents/ZSpeSpectrum.h \
    ZComponents/ZSpeAuxData.h \
    ZComponents/ZAbstractSpectrumIOHandler.h \
    ZMeasuringManager/ZSpectrumTableDelegate/ZPrimitivePlot.h \
    ZMeasuringManager/ZSpectrumTableDelegate/ZSpectrumTableDelegate.h \
    ZMeasuringManager/ZSpectrumTableDelegate/ZSpectrumPaintData.h \
    ZComponents/ZXMLSpectrumArrayIOHandler.h \
    ZDialogs/ZDashboardSettingsWidget.h \
    ZDialogs/ZDeviceSettingsWidget.h \
    ZComponents/ZDeviceSettings.h \
    ZComponents/ZUralAdcDeviceConnector.h \
    ZComponents/ZMeasuringController.h \
    ZComponents/ZPlotterDataManager.h \
    ZDialogs/ZCommonSettingsWidget.h \
    ZComponents/ZMeasurementParametersHandler.h \
    ZEnergyCalibrator/ZEnergyCalibrationDialog.h \
    ZEnergyCalibrator/ZEnergyCalibrator.h \
    ZEnergyCalibrator/ZPeakWindowTableModel.h \
    ZEnergyCalibrator/ZPeakWindow.h \
    ZDialogs/ZSaveSpectraToFilesDialog.h \
    ZEnergyCalibrator/ZEnergyCalibrationDialogV2.h \
    ZEnergyCalibrator/ZEnergyCalibrationSpectrumTableModel.h \
    ZComponents/ZLESGaussSolver.h \
    ZPlotter/ZVerticalLineGraphicsItem.h \
    ZComponents/ZEnergyCalibrationLine.h \
    ZWidgets/ZColorButton.h \
    ZPeriodicTableDialog/ZEnergyLineSelectionDialog.h \
    ZPeriodicTable/ZAbstractChemicalElementPropertyRepository.h \
    ZPeriodicTable/ZBaseDialog.h \
    ZPeriodicTable/ZCaptionFormatter.h \
    ZPeriodicTable/ZChemicalElementButton.h \
    ZPeriodicTable/ZChemicalElementPropertyIOManager.h \
    ZPeriodicTable/ZChemicalElementPropertyItem.h \
    ZPeriodicTable/ZChemicalElementPropertyTreeModel.h \
    ZPeriodicTable/ZChemicalElementProprtyEditor.h \
    ZPeriodicTable/ZChemicalPropertyProxyTableModel.h \
    ZPeriodicTable/ZPeriodicTableWidget.h \
    ZPeriodicTable/ZPropertyEditDialog.h \
    ZPeriodicTable/ZPropertySectionEditDialog.h \
    ZPlotter/ZEnergyLineGraphicsItem.h \
    ZEnergyLineManager/ZEnergyLineManager.h \
    ZEnergyLineManager/ZEnergyLineTableWidget.h \
    ZEnergyLineManager/ZSelectedEnergyLineTableModel.h \
    ZEnergyLineManager/ZEnergyLineSetItem.h \
    ZEnergyLineManager/ZEnergyLineDelegate.h \
    ZPlotter/ZTextGraphicsItem.h \
    ZPlotter/ZRulerToolGraphicsItem.h \
    ZWidgets/ZHelpBrowser.h \
    ZDialogs/ZOperatorDialog.h \
    ZDialogs/ZPasswordChangeDialog.h \
    ZDialogs/ZPasswordDialog.h \
    ZComponents/simplecrypt.h \
    ZModelViewWidgets/ZSampleTaskDelegate.h \
    ZSeriesMeasurementDialog/ZSeriesMeasurementDialog.h

RESOURCES += \
    resources.qrc \
    ZPeriodicTable/ChemicalElementModelResources.qrc

DISTFILES += \
    ZPeriodicTable/chemicalElementBasicProperties.csv


