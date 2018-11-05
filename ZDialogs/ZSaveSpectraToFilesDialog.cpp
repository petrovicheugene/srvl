//======================================================
#include "ZSaveSpectraToFilesDialog.h"
#include "ZGeneral.h"
#include "ZSpeSpectrum.h"
#include "ZSpeIOHandler.h"

#include <QApplication>
#include <QDebug>
#include <QDialogButtonBox>
#include <QFileDialog>
#include <QGridLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QStandardPaths>
#include <QVBoxLayout>



//======================================================
// ZSpectrumArraySettingsWidget
//======================================================
ZSpectrumArraySettingsWidget::ZSpectrumArraySettingsWidget(QPair<quint8, int>* conditions,
                                                           QWidget *parent) : QWidget(parent)
{
    QStringList standardPathList = QStandardPaths::standardLocations(QStandardPaths::DocumentsLocation);
    if(!standardPathList.isEmpty())
    {
        zv_folderPath = standardPathList.value(0);
    }
    else
    {
        zv_folderPath = QDir::currentPath();
    }

    zv_conditions = conditions;
    zh_createComponents();
    zh_createConnections();
    zh_restoreSettings();

}
//======================================================
ZSpectrumArraySettingsWidget::~ZSpectrumArraySettingsWidget()
{
    zh_saveSettings();
}
//======================================================
void ZSpectrumArraySettingsWidget::zh_createComponents()
{
    QGridLayout* mainLayout = new QGridLayout;
    mainLayout->setMargin(0);
    setLayout(mainLayout);

    QString text = tr("<b><u>G.F. - %1 Exposition: %2</u></b>").arg(QString::number(zv_conditions->first),
                                                                    QString::number(zv_conditions->second));
    QLabel* label = new QLabel(this);
    label->setText(text);
    mainLayout->addWidget(label, 0, 0, 1, 1);

    text = tr("Spectrum file name template:");
    label = new QLabel(this);
    label->setText(text);
    mainLayout->addWidget(label, 1, 0, 1, 1);

    zv_spectrumFileNameTemplateLineEdit = new QLineEdit(this);
    mainLayout->addWidget(zv_spectrumFileNameTemplateLineEdit, 1, 1, 1, 1);

    text = tr("Folder:");
    label = new QLabel(this);
    label->setText(text);
    mainLayout->addWidget(label, 1, 2, 1, 1);

    zv_pathLineEdit = new QLineEdit(this);
    mainLayout->addWidget(zv_pathLineEdit, 1, 3, 1, 1);

    zv_browseButton = new QPushButton(this);
    zv_browseButton->setText(tr("Browse..."));
    mainLayout->addWidget(zv_browseButton, 1, 4, 1, 1);

}
//======================================================
void ZSpectrumArraySettingsWidget::zh_createConnections()
{
    connect(zv_browseButton, &QPushButton::clicked,
            this, &ZSpectrumArraySettingsWidget::zh_onBrowseButtonClick);

}
//======================================================
void ZSpectrumArraySettingsWidget::zh_restoreSettings()
{

}
//======================================================
void ZSpectrumArraySettingsWidget::zh_saveSettings()
{

}
//======================================================
QString ZSpectrumArraySettingsWidget::zp_folderPath() const
{
    return zv_pathLineEdit->text();
}
//======================================================
QString ZSpectrumArraySettingsWidget::zp_fileNameTemplate() const
{
    return zv_spectrumFileNameTemplateLineEdit->text();
}
//======================================================
void ZSpectrumArraySettingsWidget::zh_onBrowseButtonClick()
{
    QString directory = QFileDialog::getExistingDirectory(this, tr("Select folder"),
                                                          zv_folderPath
                                                          /*QFileDialog::ShowDirsOnly
                                                                                                                | QFileDialog::DontResolveSymlinks*/);

    if(!directory.isEmpty())
    {
        zv_pathLineEdit->setText(directory);
    }
}
//======================================================
// END OF ZSpectrumArraySettingsWidget
//======================================================
ZSaveSpectraToFilesDialog::ZSaveSpectraToFilesDialog(const QMap< QPair<quint8, int>, QList<ZSpeSpectrum*> >& spectrumMap,
                                                     QWidget *parent ) : QDialog(parent)

{
    setWindowTitle(tr("Spectrum saving settings"));
    setWindowFlags(Qt::Tool);

    zv_spectrumMap = spectrumMap;
    zh_createComponents();
    zh_createConnections();
}
//======================================================
void ZSaveSpectraToFilesDialog::zh_createComponents()
{
    // main layout
    QVBoxLayout* mainLayout = new QVBoxLayout;
    setLayout(mainLayout);

    // file path
    QLabel* label = new QLabel(this);
    label->setText(tr("Calibration file:"));
    mainLayout->addWidget(label);
    ZSpectrumArraySettingsWidget* spectrumArraySettingsWidget;
    foreach(auto conditions, zv_spectrumMap.keys())
    {
        spectrumArraySettingsWidget = new ZSpectrumArraySettingsWidget(&conditions);
        zv_spectrumArraySettingsWidgetList.insert(conditions, spectrumArraySettingsWidget);
        mainLayout->addWidget(spectrumArraySettingsWidget);
    }


    //    QHBoxLayout* pathLayout = new QHBoxLayout(this);
    //    mainLayout->addLayout(pathLayout);
    //    zv_filePathLineEdit = new QLineEdit(this);
    //    zv_filePathLineEdit->setReadOnly(true);
    //    pathLayout->addWidget(zv_filePathLineEdit);
    //    zv_browseButton = new QPushButton(NS_Buttons::glButtonBrowse, this);
    //    pathLayout->addWidget(zv_browseButton);

    //    // central layout
    //    QHBoxLayout* centralLayout = new QHBoxLayout(this);
    //    mainLayout->addLayout(centralLayout);
    //    // left layout
    //    QVBoxLayout* leftLayout = new QVBoxLayout(this);
    //    centralLayout->addLayout(leftLayout);
    //    centralLayout->addSpacing(20);
    //    // right layout
    //    QVBoxLayout* rightLayout = new QVBoxLayout(this);
    //    centralLayout->addLayout(rightLayout);

    //    // name
    //    label = new QLabel(this);
    //    label->setText(tr("Calibration name:"));
    //    leftLayout->addWidget(label);

    //    zv_calibrationNameLineEdit = new QLineEdit(this);
    //    leftLayout->addWidget(zv_calibrationNameLineEdit);

    //    // chem elment
    //    label = new QLabel(this);
    //    label->setText(tr("Chemical element:"));
    //    leftLayout->addWidget(label);

    //    zv_chemElementLineEdit = new QLineEdit(this);
    //    leftLayout->addWidget(zv_chemElementLineEdit);

    //    // range
    //    label = new QLabel(this);
    //    label->setText(tr("Concentration range:"));
    //    rightLayout->addWidget(label);

    //    // max
    //    QHBoxLayout* rangeMaxLayout = new QHBoxLayout(this);
    //    rightLayout->addLayout(rangeMaxLayout);

    //    label = new QLabel(this);
    //    label->setText(tr("max:"));
    //    rangeMaxLayout->addWidget(label);
    //    zv_rangeMaxMarginSpinBox = new QDoubleSpinBox(this);
    //    zv_rangeMaxMarginSpinBox->setRange(0.0, 1.0e+99);
    //    label->setBuddy(zv_rangeMaxMarginSpinBox);
    //    rangeMaxLayout->addWidget(zv_rangeMaxMarginSpinBox, 999999);

    //    // spacing between min and max
    //    rightLayout->addSpacing(label->sizeHint().width());

    //    // min
    //    QHBoxLayout* rangeMinLayout = new QHBoxLayout(this);
    //    rightLayout->addLayout(rangeMinLayout);

    //    label = new QLabel(this);
    //    label->setText(tr("min:"));
    //    rangeMinLayout->addWidget(label);
    //    zv_rangeMinMarginSpinBox = new QDoubleSpinBox(this);
    //    label->setBuddy(zv_rangeMinMarginSpinBox);
    //    zv_rangeMinMarginSpinBox->setRange(0.0, 1.0e+99);
    //    rangeMinLayout->addWidget(zv_rangeMinMarginSpinBox, 999999);

    // Basement
    mainLayout->addStretch();

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
void ZSaveSpectraToFilesDialog::zh_createConnections()
{
    connect(zv_okButton, &QPushButton::clicked,
            this, &ZSaveSpectraToFilesDialog::zh_onOkButtonClick);
    connect(zv_cancelButton, &QPushButton::clicked,
            this, &ZSaveSpectraToFilesDialog::zh_onCancelButtonClick);
}
//======================================================
void ZSaveSpectraToFilesDialog::zh_onOkButtonClick()
{
    ZSpeIOHandler speIOHandler(nullptr);
    QString folderPath;
    QString nameTemplate;
    QString fileName;
    int spCount = 0;
    for(auto &conditions : zv_spectrumMap.keys())
    {
        // get folder and file name
        folderPath = zv_spectrumArraySettingsWidgetList[conditions]->zp_folderPath();
        nameTemplate = zv_spectrumArraySettingsWidgetList[conditions]->zp_fileNameTemplate();
        spCount = 0;
        foreach(ZSpeSpectrum* spectrum,  zv_spectrumMap[conditions])
        {
            spectrum->zp_setGainFactor(conditions.first);
            spectrum->zp_setExposition(conditions.second);

            if(nameTemplate.isEmpty())
            {
                fileName = spectrum->zp_name();
            }
            else
            {
                fileName = nameTemplate + "-" + QString::number(++spCount);
            }

            speIOHandler.zp_saveSpectrumToFile(folderPath, fileName, spectrum, false);
        }
    }

    accept();
}
//======================================================
void ZSaveSpectraToFilesDialog::zh_onCancelButtonClick()
{
    reject();
}
//======================================================
