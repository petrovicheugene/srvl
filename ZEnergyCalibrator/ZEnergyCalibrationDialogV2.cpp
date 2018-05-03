//======================================================
#include "ZEnergyCalibrationDialogV2.h"
#include "ZGeneral.h"
#include "ZPlotter.h"
#include "ZPlotterDataManager.h"

#include <QComboBox>
#include <QDialogButtonBox>
#include <QGroupBox>
#include <QGridLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QSettings>
#include <QSpinBox>
#include <QVBoxLayout>

//======================================================
ZEnergyCalibrationDialogV2::ZEnergyCalibrationDialogV2(QMap<quint8, QList<ZSpeSpectrum*> >& spectrumMap,
                                                       QWidget *parent) :
    QDialog(parent)
{
    setWindowTitle(tr("Energy calibration"));
    setWindowFlags(Qt::Tool);

    zv_spectrumMap = spectrumMap;

    zh_createComponents();
    zh_createConnections();
    zh_restoreSettings();

}
//======================================================
ZEnergyCalibrationDialogV2::~ZEnergyCalibrationDialogV2()
{
    zh_saveSettings();
}
//======================================================
void ZEnergyCalibrationDialogV2::zh_createComponents()
{
    zv_plotterDataManager = new ZPlotterDataManager(this);

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

    zv_okButton = new QPushButton(NS_Buttons::glButtonOk, this);
    buttonBox->addButton(zv_okButton, QDialogButtonBox::ActionRole);

    zv_cancelButton = new QPushButton(NS_Buttons::glButtonCancel, this);
    buttonBox->addButton(zv_cancelButton, QDialogButtonBox::ActionRole);
}
//======================================================
QWidget* ZEnergyCalibrationDialogV2::zh_createMainWidget()
{
     zv_mainGroupBox = new QGroupBox;
     QVBoxLayout* mainGroupBoxlayout = new QVBoxLayout;
     zv_mainGroupBox->setLayout(mainGroupBoxlayout);
     mainGroupBoxlayout->addWidget(zh_createElementLinesWidget());

     zv_plotter = new ZPlotter;
     mainGroupBoxlayout->addWidget(zv_plotter);



     return zv_mainGroupBox;
}
//======================================================
QWidget* ZEnergyCalibrationDialogV2::zh_createElementLinesWidget()
{
    QWidget* mainControlWidget = new QWidget;

    // Element lines
    QGridLayout* dataLayout = new QGridLayout;
    dataLayout->setMargin(0);
    mainControlWidget->setLayout(dataLayout);

    //    // Element #1
    QLabel* label = new QLabel;
    label->setText(glCreateCaption("Chemical element #1"));
    dataLayout->addWidget(label, 0, 0, 1, 3);

    label = new QLabel;
    label->setText("Peak center");
    dataLayout->addWidget(label, 1, 0, 1, 1);

    zv_channel1 = new QSpinBox;
    zv_channel1->setRange(0, 2048);
    dataLayout->addWidget(zv_channel1, 1, 1, 1, 1);

    label = new QLabel;
    label->setText("Chemical element line");
    dataLayout->addWidget(label, 1, 2, 1, 1);

    zv_elementLine1 = new QLineEdit;
    dataLayout->addWidget(zv_elementLine1, 1, 3, 1, 1);

    zv_selectLineButton1 = new QPushButton;
    zv_selectLineButton1->setText("...");
    dataLayout->addWidget(zv_selectLineButton1, 1, 4, 1, 1);


    // Element #2
    label = new QLabel;
    label->setText(glCreateCaption("Chemical element #2"));
    dataLayout->addWidget(label, 2, 0, 1, 3);

    label = new QLabel;
    label->setText("Peak center");
    dataLayout->addWidget(label, 3, 0, 1, 1);

    zv_channel2 = new QSpinBox;
    zv_channel2->setRange(0, 2048);
    dataLayout->addWidget(zv_channel2, 3, 1, 1, 1);

    label = new QLabel;
    label->setText("Chemical element line");
    dataLayout->addWidget(label, 3, 2, 1, 1);

    zv_elementLine2 = new QLineEdit;
    dataLayout->addWidget(zv_elementLine2, 3, 3, 1, 1);

    zv_selectLineButton2 = new QPushButton;
    zv_selectLineButton2->setText("...");
    dataLayout->addWidget(zv_selectLineButton2, 3, 4, 1, 1);








    return mainControlWidget;
}
//======================================================
void ZEnergyCalibrationDialogV2::zh_createConnections()
{
    zv_plotterDataManager->zp_connectToPlotter(zv_plotter);


    connect(zv_okButton, &QPushButton::clicked,
            this, &ZEnergyCalibrationDialogV2::zh_onOkClick);
    connect(zv_cancelButton, &QPushButton::clicked,
            this, &ZEnergyCalibrationDialogV2::reject);

}
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
    settings.endGroup();
    settings.endGroup();
}
//======================================================
void ZEnergyCalibrationDialogV2::zh_onOkClick()
{
    accept();
}
//======================================================
