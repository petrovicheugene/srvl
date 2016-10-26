#-------------------------------------------------
#
# Project created by QtCreator 2016-06-23T14:56:28
#
#-------------------------------------------------

QT       += core gui
QT       += sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

#PRO VARS
#Application version
RC_ICONS = "ZImages\gnome4.ico"

VER_MAJ=0
VER_MIN=0
VER_PAT=0

VER_RELEASE=b

EXE_BASE_NAME=SDAnalyzer
QMAKE_TARGET_PRODUCT="SD Analyzer"
QMAKE_TARGET_DESCRIPTION="Chemical analysis of X-ray spectra"
QMAKE_TARGET_COMPANY="TechnoAnalyt"
QMAKE_TARGET_COPYRIGHT="Copyright © $${QMAKE_TARGET_COMPANY} Ltd. 2016.  All rights reserved."
COMPANY_URL=tehnoanalit.com

CONFIG += $$VER_RELEASE
CONFIG += c++11

VERSION=$${VER_MAJ}.$${VER_MIN}.$${VER_PAT}

#Target version
r{
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
    ZSampleTask \
    ZMeasuringConditions \
    ZChemicalTask \
    ZMeasuringTaskInitStruct

SOURCES += main.cpp\
        MainWindow.cpp \
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
    ZWidgets/ZProcessTimeRoundIndicator.cpp \
    ZWidgets/QRoundProgressBar.cpp \
    ZGLConstantsAndDefines.cpp \
    ZDialogs/ZSettingsDialog.cpp \
    ZWidgets/ZDashboardSettingsWidget.cpp \
    ZWidgets/ZDashboard.cpp \
    ZWidgets/ZColorLabel.cpp \
    ZWidgets/ZStartStopButton.cpp \
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
    ZModelViewWidgets/ZChemicalTaskTableWidget.cpp \
    ZModels/ZCurrentMeasuringTaskTreeModel/ZSeriesTaskTreeModel.cpp \
    ZModels/ZCurrentMeasuringTaskTreeModel/ZItem.cpp \
    ZModels/ZCurrentMeasuringTaskTreeModel/ZSampleTaskItem.cpp \
    ZMeasuringManager/ZSample.cpp \
    ZMeasuringManager/ZAddSampleDialog.cpp \
    ZSampleTask/ZSampleTaskListDialog.cpp \
    ZSampleTask/ZSampleTaskDialog.cpp \
    ZSampleTask/ZSampleTaskTreeWidget.cpp \
    ZSampleTask/ZSampleTaskTreeModel.cpp \
    ZSampleTask/ZSampleTaskTreeItem.cpp \
    ZMeasuringConditions/ZMeasuringConditionTableModel.cpp \
    ZMeasuringConditions/ZMeasuringConditionsListDialog.cpp \
    ZChemicalTask/ZChemicalTaskListDialog.cpp \
    ZMeasuringConditions/ZMeasuringConditionsDialog.cpp \
    ZComponents/ZReadOnlyStyledItemDelegate.cpp \
    ZMeasuringManager/ZMeasuringManager.cpp \
    ZMeasuringManager/ZMeasuringResultTableModel.cpp \
    ZModelViewWidgets/ZMeasuringCommonWidget.cpp \
    ZModelViewWidgets/ZMeasuringSeriesTaskTreeWidget.cpp \
    ZModelViewWidgets/ZMeasuringResultTableWidget.cpp \
    ZMeasuringManager/ZSampleTask.cpp \
    ZMeasuringTaskInitStruct/ZMeasuringTaskInitStruct.cpp

HEADERS  += MainWindow.h \
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
    ZWidgets/ZProcessTimeRoundIndicator.h \
    ZWidgets/QRoundProgressBar.h \
    ZGLConstantsAndDefines.h \
    ZDialogs/ZSettingsDialog.h \
    ZWidgets/ZDashboardSettingsWidget.h \
    ZWidgets/ZDashboard.h \
    ZWidgets/ZColorLabel.h \
    ZWidgets/ZStartStopButton.h \
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
    ZModelViewWidgets/ZChemicalTaskTableWidget.h \
    ZModels/ZCurrentMeasuringTaskTreeModel/ZSeriesTaskTreeModel.h \
    ZModels/ZCurrentMeasuringTaskTreeModel/ZItem.h \
    ZModels/ZCurrentMeasuringTaskTreeModel/ZSampleTaskItem.h \
    ZMeasuringManager/ZSample.h \
    ZMeasuringManager/ZAddSampleDialog.h \
    ZSampleTask/ZSampleTaskListDialog.h \
    ZSampleTask/ZSampleTaskDialog.h \
    ZSampleTask/ZSampleTaskTreeWidget.h \
    ZSampleTask/ZSampleTaskTreeModel.h \
    ZSampleTask/ZSampleTaskTreeItem.h \
    ZMeasuringConditions/ZMeasuringConditionsListDialog.h \
    ZMeasuringConditions/ZMeasuringConditionTableModel.h \
    ZChemicalTask/ZChemicalTaskListDialog.h \
    ZMeasuringConditions/ZMeasuringConditionsDialog.h \
    ZComponents/ZReadOnlyStyledItemDelegate.h \
    ZMeasuringManager/ZMeasuringManager.h \
    ZMeasuringManager/ZMeasuringResultTableModel.h \
    ZModelViewWidgets/ZMeasuringCommonWidget.h \
    ZModelViewWidgets/ZMeasuringSeriesTaskTreeWidget.h \
    ZModelViewWidgets/ZMeasuringResultTableWidget.h \
    ZMeasuringManager/ZSampleTask.h \
    ZMeasuringTaskInitStruct/ZMeasuringTaskInitStruct.h

RESOURCES += \
    resources.qrc


