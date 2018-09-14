//======================================================
#include "ZEnergyCalibrationDialogV2.h"

#include "ZColorButton.h"
#include "ZGeneral.h"
#include "ZGraphicsItemUserTypes.h"
#include "ZEnergyCalibrationLine.h"
#include "ZEnergyCalibrationSpectrumTableModel.h"
#include "ZLESGaussSolver.h"
#include "ZEnergyLineSelectionDialog.h"
#include "ZPlotter.h"
#include "ZSpectrumGraphicsItem.h"
#include "ZSpectrumTableDelegate.h"
#include "ZVerticalLineGraphicsItem.h"

#include <QAction>
#include <QCheckBox>
#include <QComboBox>
#include <QDialogButtonBox>
#include <QDoubleSpinBox>
#include <QGroupBox>
#include <QGridLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QMenu>
#include <QMessageBox>
#include <QPushButton>
#include <QSettings>
#include <QSpinBox>
#include <QTableView>
#include <QToolButton>
#include <QVBoxLayout>

//======================================================
ZEnergyCalibrationDialogV2::ZEnergyCalibrationDialogV2(QMap<quint8, QList<ZSpeSpectrum*> >& spectrumMap,
                                                       QWidget *parent) :
    QDialog(parent)
{
    setWindowTitle(tr("Energy calibration"));
    setWindowFlags(Qt::Tool);

    zh_createComponents();
    zh_createConnections();
    zh_createContextMenu();
    zh_setElementLineControlsAndComponentVisibility();
    zh_restoreSettings();
    zh_loadSpectrumData(spectrumMap);


    if(zv_plotter != 0)
    {
        QMetaObject::invokeMethod(zv_plotter, "zp_fitInBoundingRect",
                                  Qt::QueuedConnection);
    }

}
//======================================================
ZEnergyCalibrationDialogV2::~ZEnergyCalibrationDialogV2()
{
    zh_saveSettings();
}
//======================================================
void ZEnergyCalibrationDialogV2::zh_createComponents()
{
    zv_spectrumModel = new ZEnergyCalibrationSpectrumTableModel(this);

    // main layout
    QVBoxLayout* mainLayout = new QVBoxLayout;
    setLayout(mainLayout);

    // gain factor
    QHBoxLayout* gainFactorLayout = new QHBoxLayout;
    gainFactorLayout->setMargin(0);
    mainLayout->addLayout(gainFactorLayout);

    QLabel* label = new QLabel;
    label->setText(glCreateCaption("Gain factor"));
    gainFactorLayout->addWidget(label);

    zv_gainFactorComboBox = new QComboBox(this);
    gainFactorLayout->addWidget(zv_gainFactorComboBox);

    gainFactorLayout->addStretch();

    // main widget
    mainLayout->addWidget(zh_createMainWidget());

    // Basement

    // sparator line
    QFrame* line = new QFrame(this);
    line->setFrameStyle(QFrame::HLine | QFrame::Sunken);
    line->setLineWidth(1);
    mainLayout->addWidget(line);

    // dialog buttons
    QDialogButtonBox* buttonBox = new QDialogButtonBox(this);
    mainLayout->addWidget(buttonBox);

//    zv_okButton = new QPushButton(NS_Buttons::glButtonOk, this);
//    buttonBox->addButton(zv_okButton, QDialogButtonBox::ActionRole);

    zv_closeButton = new QPushButton(tr("Close"), this);
    buttonBox->addButton(zv_closeButton, QDialogButtonBox::ActionRole);
}
//======================================================
QWidget* ZEnergyCalibrationDialogV2::zh_createMainWidget()
{
    QWidget* mainWidget = new QWidget;
    QVBoxLayout* mainGroupBoxlayout = new QVBoxLayout;
    mainWidget->setLayout(mainGroupBoxlayout);

    // top  layout
    QHBoxLayout* rightLayout = new QHBoxLayout;
    rightLayout->setMargin(0);
    mainGroupBoxlayout->addLayout(rightLayout);

//    QDialogButtonBox* mainWidgetButtonBox = new QDialogButtonBox(this);
//    mainGroupBoxlayout->addWidget(mainWidgetButtonBox);

//    zv_okButton = new QPushButton(NS_Buttons::glButtonOk, this);
//    mainWidgetButtonBox->addButton(zv_okButton, QDialogButtonBox::ActionRole);

    // plotter
    zv_plotter = new ZPlotter;
    mainGroupBoxlayout->addWidget(zv_plotter, 99999999);

    // spectrum list
    zv_spectrumTableView = new QTableView(this);
    rightLayout->addWidget(zv_spectrumTableView);

    // element lines
    rightLayout->addWidget(zh_createElementLinesWidgetAndComponents());
    rightLayout->addStretch();

    return mainWidget;
}
//======================================================
QWidget* ZEnergyCalibrationDialogV2::zh_createElementLinesWidgetAndComponents()
{
    QGroupBox* mainControlWidget = new QGroupBox;
    QVBoxLayout* mainLayout = new QVBoxLayout;
    // mainLayout->setMargin(0);
    mainControlWidget->setLayout(mainLayout);

    QHBoxLayout* peakCountLayout = new QHBoxLayout(this);
    mainLayout->addLayout(peakCountLayout);

    QLabel* label = new QLabel(this);
    label->setText(glCreateCaption(tr("Peak count in energy calibration:")));
    peakCountLayout->addWidget(label);
    zv_peakInEnergyCalibrationCountComboBox = new QComboBox(this);
    zv_peakInEnergyCalibrationCountComboBox->addItem(tr("Two"), QVariant(2));
    zv_peakInEnergyCalibrationCountComboBox->addItem(tr("Three"), QVariant(3));

    peakCountLayout->addWidget(zv_peakInEnergyCalibrationCountComboBox);
    peakCountLayout->addStretch();

    // Element lines
    QGridLayout* dataLayout = new QGridLayout;
    // dataLayout->setMargin(0);
    mainLayout->addLayout(dataLayout);

    for(int i = 0; i < zv_calibrationLineNumber; i++)
    {
        zh_createElementLineControlsAndComponents(dataLayout, i);
    }

    mainLayout->addStretch();
    // Calculate buttonBox
    QDialogButtonBox* buttonBox = new QDialogButtonBox(this);
    mainLayout->addWidget(buttonBox);

    zv_calculateButton = new QPushButton(tr("Calculate"), this);
    zv_calculateButton->setToolTip(tr("Calculate energy calibration"));
    zv_calculateButton->setFocusPolicy(Qt::NoFocus);
    buttonBox->addButton(zv_calculateButton, QDialogButtonBox::ActionRole);

    return mainControlWidget;
}
//======================================================
void ZEnergyCalibrationDialogV2::zh_createElementLineControlsAndComponents(QGridLayout* layout, int elementNumber)
{
    // calibration line
    ZEnergyCalibrationLine* calibrationLine = new ZEnergyCalibrationLine(this);
    connect(calibrationLine, &ZEnergyCalibrationLine::zg_lineColor,
            this, &ZEnergyCalibrationDialogV2::zh_onLineColorChange);
    zv_energyCalibrationLineList.append(calibrationLine);

    // claibration line graphic item
    ZVerticalLineGraphicsItem* verticalLineGraphicsItem = new ZVerticalLineGraphicsItem(calibrationLine);
    zv_plotter->zp_addItem(verticalLineGraphicsItem);

    // channel set up action
    QAction* channelNumberAction = new QAction(calibrationLine);
    channelNumberAction->setText(tr("Energy line #%1").arg(QString::number(elementNumber + 1)));
    connect(channelNumberAction, &QAction::triggered,
            this, &ZEnergyCalibrationDialogV2::zh_onChannelNumberAction);
    zv_channelNumberActionList.append(channelNumberAction);

    QList<QWidget*> widgetList;

    int startRow = elementNumber * 2;
    QLabel* label = new QLabel(this);
    label->setText(glCreateCaption(QString("Energy line #%1").arg(QString::number(elementNumber + 1))));
    widgetList.append(label);
    layout->addWidget(label, startRow, 0, 1, 3);

    ZColorButton* colorBtn = new ZColorButton(this);
    widgetList.append(colorBtn);
    layout->addWidget(colorBtn, startRow + 1, 0, 1, 1);
    connect(colorBtn, &ZColorButton::zg_colorChanged,
            calibrationLine, &ZEnergyCalibrationLine::zp_setLineColor);
    connect(calibrationLine, &ZEnergyCalibrationLine::zg_lineColor,
            colorBtn, &ZColorButton::zp_setColor);

    label = new QLabel(this);
    label->setText("Peak center");
    widgetList.append(label);
    layout->addWidget(label, startRow + 1, 1, 1, 1);

    QSpinBox* channelNumberSpinBox = new QSpinBox(this);
    channelNumberSpinBox->setRange(0, 2048);
    widgetList.append(channelNumberSpinBox);
    connect(calibrationLine, &ZEnergyCalibrationLine::zg_channelNumber,
            this, &ZEnergyCalibrationDialogV2::zh_onChannelNumberChange);
    connect(calibrationLine, &ZEnergyCalibrationLine::zg_channelNumber,
            channelNumberSpinBox, &QSpinBox::setValue);
    connect(channelNumberSpinBox, SIGNAL(valueChanged(int)),
            calibrationLine, SLOT(zp_setChannelNumber(int)));
    //    connect(channelNumberSpinBox, &QSpinBox::hasFo
    //            this, &ZEnergyCalibrationDialogV2::onChannelspinBoxHasFocus);
    layout->addWidget(channelNumberSpinBox, startRow + 1, 2, 1, 1);

    label = new QLabel(this);
    label->setText("Energy value");
    widgetList.append(label);
    layout->addWidget(label, startRow + 1, 3, 1, 1);

    QDoubleSpinBox* energyValueSpinBox = new QDoubleSpinBox(this);
    energyValueSpinBox->setRange(0, 9999.99);
    energyValueSpinBox->setDecimals(3);
    energyValueSpinBox->setSuffix(tr("kEv"));
    //energyValueSpinBox->setReadOnly(true);
    energyValueSpinBox->setButtonSymbols(QAbstractSpinBox::NoButtons);
    widgetList.append(energyValueSpinBox);

    connect(calibrationLine, &ZEnergyCalibrationLine::zg_energyValue,
            energyValueSpinBox, &QDoubleSpinBox::setValue);
    connect(energyValueSpinBox, SIGNAL(valueChanged(double)),
            calibrationLine, SLOT(zp_setEnergyValue(double)));
    layout->addWidget(energyValueSpinBox, startRow + 1, 4, 1, 1);


    label = new QLabel(this);
    label->setText("Chemical element line");
    widgetList.append(label);
    layout->addWidget(label, startRow + 1, 5, 1, 1);

    QLineEdit* elementLineEdit = new QLineEdit(this);
    widgetList.append(elementLineEdit);

    //elementLineEdit->setReadOnly(true);
    connect(calibrationLine, &ZEnergyCalibrationLine::zg_lineName,
            elementLineEdit, &QLineEdit::setText);
    connect(elementLineEdit, &QLineEdit::textChanged,
            calibrationLine, &ZEnergyCalibrationLine::zp_setLineName);
    layout->addWidget(elementLineEdit, startRow + 1, 6, 1, 1);


    QToolButton* selectLineButton = new QToolButton(this);
    selectLineButton->setText("...");
    selectLineButton->setFocusPolicy(Qt::NoFocus);
    widgetList.append(selectLineButton);
    layout->addWidget(selectLineButton, startRow + 1, 7, 1, 1);

    // energy line set up dialog
    QAction* selectEnergyLineAction = new QAction(calibrationLine);
    connect(selectLineButton, &QPushButton::clicked,
            selectEnergyLineAction, &QAction::trigger);
    connect(selectEnergyLineAction, &QAction::triggered,
            this, &ZEnergyCalibrationDialogV2::zh_onSelectEnergyLineAction);

    QToolButton* clearLineButton = new QToolButton(this);
    clearLineButton->setText("X");
    clearLineButton->setFocusPolicy(Qt::NoFocus);
    widgetList.append(clearLineButton);
    layout->addWidget(clearLineButton, startRow + 1, 8, 1, 1);

    QAction* clearEnergyLineAction = new QAction(calibrationLine);
    connect(clearLineButton, &QPushButton::clicked,
            clearEnergyLineAction, &QAction::trigger);
    connect(clearEnergyLineAction, &QAction::triggered,
            this, &ZEnergyCalibrationDialogV2::zh_onClearEnergyLineAction);

    zv_elementLinesWidgetList.append(widgetList);

}
//======================================================
void ZEnergyCalibrationDialogV2::zh_createContextMenu()
{
    zv_plotter->zp_setContextMenu(zv_channelNumberActionList);
}
//======================================================
void ZEnergyCalibrationDialogV2::zh_createConnections()
{
    zv_spectrumTableView->setModel(zv_spectrumModel);
    ZSpectrumTableDelegate* spectrumTableDelegate = new ZSpectrumTableDelegate(zv_spectrumTableView);
    zv_spectrumTableView->viewport()->installEventFilter(spectrumTableDelegate);
    zv_spectrumTableView->setItemDelegate(spectrumTableDelegate);

    connect(zv_calculateButton, &QPushButton::clicked,
            this, &ZEnergyCalibrationDialogV2::zh_calculateAndWriteEnergyCalibration);
    connect(zv_closeButton, &QPushButton::clicked,
            this, &ZEnergyCalibrationDialogV2::close);
    connect(zv_gainFactorComboBox, SIGNAL(currentIndexChanged(int)),
            this, SLOT(zh_onCurrentGainFactorIndexChange(int)));

    connect(zv_spectrumModel, &ZEnergyCalibrationSpectrumTableModel::modelReset,
            this, &ZEnergyCalibrationDialogV2::zh_onModelReset);
    connect(zv_spectrumModel, &ZEnergyCalibrationSpectrumTableModel::dataChanged,
            this, &ZEnergyCalibrationDialogV2::zh_onModelDataChange);

    connect(zv_plotter, &ZPlotter::zg_mousePressedAt,
            this, &ZEnergyCalibrationDialogV2::zh_onMousePressedOnPlotter);

    connect(zv_peakInEnergyCalibrationCountComboBox, SIGNAL(currentIndexChanged(int)),
            this, SLOT(zh_onCurrentPeackCountIndexChange(int)));
}
//======================================================
void ZEnergyCalibrationDialogV2::zh_loadSpectrumData(QMap<quint8, QList<ZSpeSpectrum *> > &spectrumMap)
{
    // gain factor
    QString itemString;
    for(auto &gainFactor : spectrumMap.keys())
    {
        itemString = tr("Gain Factor: %1").arg(QString::number(gainFactor));
        zv_gainFactorComboBox->addItem(itemString, QVariant(gainFactor));
    }
    zv_gainFactorComboBox->setCurrentIndex(0);

    // spectrum model data
    zv_spectrumModel->zp_loadSpectrumData(spectrumMap);
}
//======================================================
//void ZEnergyCalibrationDialogV2::zh_restoreLineColorList()
//{
//    QSettings settings;
//    settings.beginGroup(glAppVersion);
//    settings.beginGroup(zv_settingsGroupName);

//    int size = settings.beginReadArray(zv_lineColorListSectionName);
//    if(size > 0)
//    {
//        for (int i = 0; i < size; ++i)
//        {
//            settings.setArrayIndex(i);
//            QColor color = settings.value(zv_lineColorValueName).value<QColor>();
//            zv_lineColorList.append(color);
//        }
//    }
//    else
//    {
//        zv_lineColorList.append(QColor(Qt::red));
//        zv_lineColorList.append(QColor(Qt::green));
//        zv_lineColorList.append(QColor(Qt::blue));
//    }

//    settings.endArray();

//    settings.endGroup();
//    settings.endGroup();
//}
//======================================================
void ZEnergyCalibrationDialogV2::zh_restoreSettings()
{
    QSettings settings;
    settings.beginGroup(glAppVersion);
    settings.beginGroup(zv_settingsGroupName);

    QVariant vData = settings.value(zv_geometrySectionName);
    if(vData.isValid() && !vData.isNull() && vData.canConvert<QByteArray>())
    {
        restoreGeometry(vData.toByteArray());
    }

    int size = settings.beginReadArray(zv_lineColorListSectionName);
    if(size > 0)
    {
        for (int i = 0; i < size; ++i)
        {
            settings.setArrayIndex(i);
            QColor color = settings.value(zv_lineColorValueName).value<QColor>();
            ZEnergyCalibrationLine* line = zv_energyCalibrationLineList.value(i, nullptr);
            if(line)
            {
                line->zp_setLineColor(color);
            }
        }
    }
    else
    {
        QList<QColor> lineColorList;
        lineColorList.append(QColor(Qt::red));
        lineColorList.append(QColor(Qt::green));
        lineColorList.append(QColor(Qt::blue));

        QColor color;
        for(int i = 0; i < zv_energyCalibrationLineList.count(); i++)
        {
            zv_energyCalibrationLineList.at(i)->zp_setLineColor(lineColorList.value(i, QColor()));
        }
    }

    settings.endArray();


    settings.endGroup();
    settings.endGroup();
}
//======================================================
void ZEnergyCalibrationDialogV2::zh_saveSettings()
{
    QSettings settings;
    settings.beginGroup(glAppVersion);
    settings.beginGroup(zv_settingsGroupName);
    settings.setValue(zv_geometrySectionName, QVariant(saveGeometry()));

    settings.beginWriteArray(zv_lineColorListSectionName);
    for (int i = 0; i < zv_energyCalibrationLineList.count(); ++i)
    {
        settings.setArrayIndex(i);
        settings.setValue(zv_lineColorValueName, QVariant(zv_energyCalibrationLineList.at(i)->zp_lineColor()));
    }
    settings.endArray();

    settings.endGroup();
    settings.endGroup();
}
//======================================================
void ZEnergyCalibrationDialogV2::zh_calculateAndWriteEnergyCalibration()
{
    // calculate energy scale factors
    QList<double> energyCalibrationFactorList;
    if(!zh_calculateEnergyFactors(energyCalibrationFactorList))
    {
        return;
    }

    // write energy calibration to checked files
    zv_spectrumModel->zp_writeEnergyCalibrationToCheckedSpectra(energyCalibrationFactorList);

    // write energy calibration to gain factor
    QString msg = tr("Do you want to bind the energy calibration to the current gain factor equals %1?").arg(zv_gainFactorComboBox->currentText());
    if(QMessageBox::question(this, tr(""), msg, QMessageBox::Yes | QMessageBox::No) == QMessageBox::No)
    {
        return;
    }






}
//======================================================
void ZEnergyCalibrationDialogV2::zh_onCurrentGainFactorIndexChange(int currentIndex)
{
    if(currentIndex < 0 || currentIndex >= zv_gainFactorComboBox->count())
    {
        zv_spectrumModel->zp_setGainFactorFilter(0);
        return;
    }

    QVariant vData = zv_gainFactorComboBox->itemData(currentIndex);
    if(!vData.isValid() | !vData.canConvert<quint8>())
    {
        zv_spectrumModel->zp_setGainFactorFilter(0);
        return;
    }

    quint8 currentGainFactor = static_cast<quint8>(vData.toUInt());
    zv_spectrumModel->zp_setGainFactorFilter(currentGainFactor);

}
//======================================================
void ZEnergyCalibrationDialogV2::zh_onCurrentPeackCountIndexChange(int currentIndex)
{
    QComboBox* comboBox = qobject_cast<QComboBox*>(sender());
    if(!comboBox)
    {
        return;
    }

    zh_setElementLineControlsAndComponentVisibility();
    zh_updateVerticalLines();
}
//======================================================
void ZEnergyCalibrationDialogV2::zh_onModelReset()
{
    // current plotter state
    qreal distortionFactor;
    qreal distortionCorrectionFactor;
    zv_plotter->zp_verticalDistortionFactors(distortionFactor, distortionCorrectionFactor);
    bool isPlotScaled = zv_plotter->zp_isPlotScaled();

    // clear all previous spectra
    zv_plotter->zp_clearItemsForType(SpectrumItemType);

    //
    ZSpectrumGraphicsItem* spectrumItem;
    ZSpeSpectrum* spectrum;

    for(int row = 0; row < zv_spectrumModel->rowCount(); row++)
    {
        spectrum = zv_spectrumModel->zp_spectrumForRow(row);
        spectrumItem = new ZSpectrumGraphicsItem(spectrum,
                                                 zv_boundingRectTopFactor,
                                                 distortionFactor,
                                                 distortionCorrectionFactor);
        zv_plotter->zp_addItem(spectrumItem);
    }

    if(!isPlotScaled)
    {
        zv_plotter->zp_fitInBoundingRect();
        //            QMetaObject::invokeMethod(zv_plotter, "zp_fitInBoundingRect",
        //                                      Qt::QueuedConnection);
    }

    QRectF boundingRect = zv_plotter->zp_boundingRect();
    ZVerticalLineGraphicsItem::zp_setTopAndButtonMargins(boundingRect.top(),boundingRect.bottom());
    zh_updateVerticalLines();
}
//======================================================
void ZEnergyCalibrationDialogV2::zh_onModelDataChange(const QModelIndex& topLeft,
                                                      const QModelIndex& bottomRight)
{
    // update spectrum Visibility
    QList<QGraphicsItem*> spectrumList = zv_plotter->zp_itemListForType(SpectrumItemType);
    ZSpectrumGraphicsItem* spectrumItem;
    ZSpeSpectrum* spectrum;

    for(int row = 0; row < zv_spectrumModel->rowCount(); row++)
    {
        spectrum = zv_spectrumModel->zp_spectrumForRow(row);
        for(int sp = 0; sp < spectrumList.count(); sp++)
        {
            spectrumItem = qgraphicsitem_cast<ZSpectrumGraphicsItem*>(spectrumList.at(sp));
            if(spectrumItem != nullptr && spectrumItem->zp_spectrumId() == spectrum->zp_spectrumId())
            {
                // spectrumItem found
                // set visibility
                spectrumItem->setVisible(spectrum->zp_isSpectrumVisible());
                // remove spectrum graphic item from the list
                spectrumList.removeAt(sp);
                break;
            }
        }
    }
}
//======================================================
void ZEnergyCalibrationDialogV2::zh_onMousePressedOnPlotter(QPointF scenePos)
{
    qDebug() << "mouse press" << scenePos;


}
//======================================================
void ZEnergyCalibrationDialogV2::zh_onChannelNumberAction()
{
    QAction* action = qobject_cast<QAction*>(sender());
    if(!action)
    {
        return;
    }

    QVariant vData = action->data();
    if(!vData.isValid() || !vData.canConvert<QPointF>())
    {
        return;
    }

    ZEnergyCalibrationLine* energyCalibrationLine = qobject_cast<ZEnergyCalibrationLine*>(action->parent());
    if(!energyCalibrationLine)
    {
        return;
    }

    energyCalibrationLine->zp_setChannelNumber(static_cast<int>(vData.toPointF().x()));
}
//======================================================
void ZEnergyCalibrationDialogV2::zh_onLineColorChange(QColor color)
{
    zh_updateVerticalLines();
}
//======================================================
void ZEnergyCalibrationDialogV2::zh_onChannelNumberChange(int channelValue)
{
    zh_updateVerticalLines();
}
//======================================================
void ZEnergyCalibrationDialogV2::zh_updateVerticalLines()
{
    QList<QGraphicsItem*>lineList = zv_plotter->zp_itemListForType(VerticalLineItemType);
    foreach(QGraphicsItem* graphicsItem, lineList)
    {
        ZVerticalLineGraphicsItem* lineItem = qgraphicsitem_cast<ZVerticalLineGraphicsItem*>(graphicsItem);
        if(lineItem)
        {
            lineItem->zp_updateItem();
        }
    }
}
//======================================================
void ZEnergyCalibrationDialogV2::zh_onSelectEnergyLineAction()
{
    QAction* action = qobject_cast<QAction*>(sender());
    if(!action)
    {
        return;
    }

    ZEnergyCalibrationLine* energyCalibrationLine = qobject_cast<ZEnergyCalibrationLine*>(action->parent());
    if(!energyCalibrationLine)
    {
        return;
    }

    ZEnergyLineSelectionDialog dialog;
    if(!dialog.exec())
    {
        return;
    }

    energyCalibrationLine->zp_setEnergyValue(dialog.zp_energyValue());
    QString chemicalElementEnergyLineName = dialog.zp_chemicalElementName() + " - " +
            dialog.zp_energyLineName();
    energyCalibrationLine->zp_setLineName(chemicalElementEnergyLineName);


}
//======================================================
void ZEnergyCalibrationDialogV2::zh_onClearEnergyLineAction()
{
    QAction* action = qobject_cast<QAction*>(sender());
    if(!action)
    {
        return;
    }

    ZEnergyCalibrationLine* energyCalibrationLine = qobject_cast<ZEnergyCalibrationLine*>(action->parent());
    if(!energyCalibrationLine)
    {
        return;
    }

    energyCalibrationLine->zp_setChannelNumber(0);
    energyCalibrationLine->zp_setEnergyValue(0.0);
    energyCalibrationLine->zp_setLineName(QString());

}
//======================================================
bool ZEnergyCalibrationDialogV2::zh_calculateEnergyFactors(QList<double>& energyCalibrationFactorList)
{
    QList<QPair<int, double> > calibrationLineSrcDataList;
    bool skipLine;
    for(int k = 0; k < zv_energyCalibrationLineList.count(); k++)
    {
        skipLine = false;
        for(int e = 0; e < k; e++)
        {
            if(zv_energyCalibrationLineList.at(e)->zp_channelNumber() ==
                    zv_energyCalibrationLineList.at(k)->zp_channelNumber() &&
                    zv_energyCalibrationLineList.at(e)->zp_energyValue() -
                    zv_energyCalibrationLineList.at(k)->zp_energyValue() == 0.0)
            {

                skipLine = true;
                break;
            }
            else if(!(zv_energyCalibrationLineList.at(e)->zp_channelNumber() !=
                      zv_energyCalibrationLineList.at(k)->zp_channelNumber() &&
                      zv_energyCalibrationLineList.at(e)->zp_energyValue() -
                      zv_energyCalibrationLineList.at(k)->zp_energyValue() != 0.0))
            {
                QString msg = tr("Ambiguity in source data of energy lines.");
                QMessageBox::critical(this, tr("Error"), msg, QMessageBox::Ok);
                return false;
            }
        }

        if(skipLine)
        {
            continue;
        }

        if(!zv_energyCalibrationLineList.at(k)->zp_isEnabled())
        {
            continue;
        }

        calibrationLineSrcDataList.append(QPair<int, double>(zv_energyCalibrationLineList.at(k)->zp_channelNumber(),
                                                             zv_energyCalibrationLineList.at(k)->zp_energyValue()));
    }

    if(calibrationLineSrcDataList.count() < 2)
    {
        QString msg = tr("Insufficient source data of energy lines.");
        QMessageBox::critical(this, tr("Error"), msg, QMessageBox::Ok);
        return false;
    }


    // create free member list, column list and fill all lists
    //
    QList<double> freeMemberList;
    QList<QPair<double, QList<double> > > matrixColumnList;
    for(int i = 0; i < calibrationLineSrcDataList.count(); i++)
    {
        matrixColumnList.append(QPair<double, QList<double> >(0.0, QList<double>()));
    }

    double memberValue;
    for(int cl = 0; cl < calibrationLineSrcDataList.count(); cl++)
    {
        // fill column lists
        for(int mc = 0; mc < matrixColumnList.count(); mc++)
        {
            memberValue = static_cast<double>(pow(calibrationLineSrcDataList.at(cl).first, mc));
            matrixColumnList[mc].second.append(memberValue);
        }

        // fill free member list
        freeMemberList.append(calibrationLineSrcDataList.at(cl).second);
    }

    ZLESGaussSolver solver;
    for(int mc = 0; mc < matrixColumnList.count(); mc++)
    {
        solver.zp_appendTermColumn(&matrixColumnList[mc].first,  matrixColumnList.at(mc).second);
    }

    solver.zp_appendFreeTermList(freeMemberList);

    if(!solver.zp_solve())
    {
        QString msg = tr("Calculation of energy calibration factors fails. Error: %1.").arg(solver.zp_lastError());
        QMessageBox::critical(this, tr("Error"), msg, QMessageBox::Ok);
        return false;
    }

    qDebug() << "FACTORS";
    for(int mc = 0; mc <  matrixColumnList.count(); mc++)
    {
        energyCalibrationFactorList.append(matrixColumnList[mc].first);
        qDebug() << "FACTOR" <<  mc << matrixColumnList[mc].first;
    }

    return true;
}
//======================================================
void ZEnergyCalibrationDialogV2::zh_setElementLineControlsAndComponentVisibility()
{
    QVariant vData = zv_peakInEnergyCalibrationCountComboBox->currentData();
    if(!vData.isValid() || !vData.canConvert<int>())
    {
        return;
    }

    int peackCount = vData.toInt();
    ZEnergyCalibrationLine* line;
    for(int i = 0; i < zv_elementLinesWidgetList.count(); i++)
    {
        line = zv_energyCalibrationLineList.value(i, nullptr);
        if(line)
        {
            line->zp_setEnabled(i < peackCount);
        }

        for(auto& widget : zv_elementLinesWidgetList.at(i))
        {
            widget->setVisible(i < peackCount);
        }

        QAction* action = zv_channelNumberActionList.value(i, nullptr);
        if(action)
        {
            action->setEnabled(i < peackCount);
            action->setVisible(i < peackCount);
        }
    }
}
//======================================================
