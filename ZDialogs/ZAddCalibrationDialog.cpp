//=========================================================
#include "ZAddCalibrationDialog.h"
#include "ZGeneral.h"
#include "ZXMLCalibrationIOHandler.h"
#include "ZCalibration.h"

#include <QSettings>
#include <QFileInfo>
#include <QStandardPaths>
#include <QDir>
#include <QMessageBox>

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QDialogButtonBox>
#include <QPushButton>
#include <QLineEdit>
#include <QDoubleSpinBox>
#include <QLabel>
//=========================================================
ZAddCalibrationDialog::ZAddCalibrationDialog(QWidget *parent) : QDialog(parent)
{
    setWindowTitle(tr("Add calibration"));
    setWindowFlags(Qt::Tool);

    zv_calibration = 0;
    zh_createComponents();
    zh_createConnections();
    zv_rangePreviousMinValue = zv_rangeMinMarginSpinBox->value();
    zv_rangePreviousMaxValue = zv_rangeMaxMarginSpinBox->value();
    zh_restoreSettings();
}
//=========================================================
ZAddCalibrationDialog::~ZAddCalibrationDialog()
{
    zh_saveSettings();
}
//=========================================================
QString ZAddCalibrationDialog::zp_calibrationFilePath() const
{
    return zv_filePathLineEdit->text();
}
//=========================================================
QString ZAddCalibrationDialog::zp_calibrationName() const
{
    return zv_calibrationNameLineEdit->text();
}
//=========================================================
QString ZAddCalibrationDialog::zp_chemicalElement() const
{
    return zv_chemElementLineEdit->text();
}
//=========================================================
qreal ZAddCalibrationDialog::zp_rangeMinMargin() const
{
    return zv_rangeMinMarginSpinBox->value();
}
//=========================================================
qreal ZAddCalibrationDialog::zp_rangeMaxMargin() const
{
    return zv_rangeMaxMarginSpinBox->value();
}
//=========================================================
void ZAddCalibrationDialog::zh_createComponents()
{
    // main layout
    QVBoxLayout* mainLayout = new QVBoxLayout(this);
    setLayout(mainLayout);

    // file path
    QLabel* label = new QLabel(this);
    label->setText(tr("Calibration file:"));
    mainLayout->addWidget(label);

    QHBoxLayout* pathLayout = new QHBoxLayout(this);
    mainLayout->addLayout(pathLayout);
    zv_filePathLineEdit = new QLineEdit(this);
    zv_filePathLineEdit->setReadOnly(true);
    pathLayout->addWidget(zv_filePathLineEdit);
    zv_browseButton = new QPushButton(NS_Buttons::glButtonBrowse, this);
    pathLayout->addWidget(zv_browseButton);

    // central layout
    QHBoxLayout* centralLayout = new QHBoxLayout(this);
    mainLayout->addLayout(centralLayout);
    // left layout
    QVBoxLayout* leftLayout = new QVBoxLayout(this);
    centralLayout->addLayout(leftLayout);
    centralLayout->addSpacing(20);
    // right layout
    QVBoxLayout* rightLayout = new QVBoxLayout(this);
    centralLayout->addLayout(rightLayout);

    // name
    label = new QLabel(this);
    label->setText(tr("Calibration name:"));
    leftLayout->addWidget(label);

    zv_calibrationNameLineEdit = new QLineEdit(this);
    leftLayout->addWidget(zv_calibrationNameLineEdit);

    // chem elment
    label = new QLabel(this);
    label->setText(tr("Chemical element:"));
    leftLayout->addWidget(label);

    zv_chemElementLineEdit = new QLineEdit(this);
    leftLayout->addWidget(zv_chemElementLineEdit);

    // range
    label = new QLabel(this);
    label->setText(tr("Concentration range:"));
    rightLayout->addWidget(label);

    // max
    QHBoxLayout* rangeMaxLayout = new QHBoxLayout(this);
    rightLayout->addLayout(rangeMaxLayout);

    label = new QLabel(this);
    label->setText(tr("max:"));
    rangeMaxLayout->addWidget(label);
    zv_rangeMaxMarginSpinBox = new QDoubleSpinBox(this);
    zv_rangeMaxMarginSpinBox->setRange(0.0, 1.0e+99);
    label->setBuddy(zv_rangeMaxMarginSpinBox);
    rangeMaxLayout->addWidget(zv_rangeMaxMarginSpinBox, 999999);

    // spacing between min and max
    rightLayout->addSpacing(label->sizeHint().width());

    // min
    QHBoxLayout* rangeMinLayout = new QHBoxLayout(this);
    rightLayout->addLayout(rangeMinLayout);

    label = new QLabel(this);
    label->setText(tr("min:"));
    rangeMinLayout->addWidget(label);
    zv_rangeMinMarginSpinBox = new QDoubleSpinBox(this);
    label->setBuddy(zv_rangeMinMarginSpinBox);
    zv_rangeMinMarginSpinBox->setRange(0.0, 1.0e+99);
    rangeMinLayout->addWidget(zv_rangeMinMarginSpinBox, 999999);

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
//=========================================================
void ZAddCalibrationDialog::zh_createConnections()
{
    connect(zv_browseButton, &QPushButton::clicked,
            this, &ZAddCalibrationDialog::zh_onBrowseClick);
    connect(zv_okButton, &QPushButton::clicked,
            this, &ZAddCalibrationDialog::zh_onOkClick);
    connect(zv_cancelButton, &QPushButton::clicked,
            this, &ZAddCalibrationDialog::reject);

    connect(zv_rangeMinMarginSpinBox, SIGNAL(valueChanged(double)),
            this, SLOT(zh_onRangeChange(double)));
    connect(zv_rangeMaxMarginSpinBox, SIGNAL(valueChanged(double)),
            this, SLOT(zh_onRangeChange(double)));
}
//=========================================================
void ZAddCalibrationDialog::zh_restoreSettings()
{
    QSettings settings;
    settings.beginGroup(glAppVersion);
    settings.beginGroup(zv_settingsGroupName);

    QVariant vData = settings.value(zv_fileLacationDirSectionName);

    if(vData.isValid() && !vData.isNull() && vData.canConvert<QString>())
    {
        zv_fileLocationDirString = vData.toString();
    }
    else
    {
        QStringList standardLocations =
                QStandardPaths::standardLocations(QStandardPaths::DocumentsLocation);
        if(!standardLocations.isEmpty())
        {
            zv_fileLocationDirString = standardLocations.value(0);
        }
        else
        {
            zv_fileLocationDirString = QDir::currentPath();
        }

    }

    settings.endGroup();
    settings.endGroup();
}
//=========================================================
void ZAddCalibrationDialog::zh_saveSettings()
{
    QSettings settings;
    settings.beginGroup(glAppVersion);
    settings.beginGroup(zv_settingsGroupName);
    settings.setValue(zv_fileLacationDirSectionName, QVariant(zv_fileLocationDirString));
    settings.endGroup();
    settings.endGroup();
}
//=========================================================
bool ZAddCalibrationDialog::zh_createCalibrationFromFile(const QString& fileName, QString& msg)
{
    QFileInfo fileInfo(fileName);
    QString suffix = fileInfo.suffix();

    // check file
    if(!fileInfo.exists() || !fileInfo.isFile())
    {
        msg = tr("Error: \"%1\" is not a file!").arg(fileName);
        return false;
    }
    else if(suffix != "clbx" && suffix != "xml")
    {
        msg = QObject::tr("Cannot handle file of type \"%1\"!").arg(suffix);
        return false;
    }

    // open file
    QFile file(fileName);
    if(!file.open(QIODevice::ReadOnly))
    {
        if(file.error() != QFile::NoError)
        {
            msg = tr("Cannot read file \"%1\"! %2").arg(file.fileName(), file.errorString());
        }
        else
        {
            msg = tr("Cannot read file \"%1\"! %2").arg(file.fileName(), tr("Unknown error"));
        }
        return false;
    }

    //ZXMLCalibrationIOHandler* ioHandler = new ZXMLCalibrationIOHandler(this);
    // create a new calibration
    ZCalibration* calibration = new ZCalibration(fileName, this);

    // extract calibration from file
    ZXMLCalibrationIOHandler ioHandler;
    bool res = ioHandler.zp_getCalibrationFromFile(file, calibration);

    if(file.isOpen())
    {
        file.close();
    }

    if(res)
    {
        // replace old calibration
        if(zv_calibration != 0)
        {
            delete zv_calibration;
        }

        zv_calibration = calibration;
    }
    else
    {
        msg = ioHandler.zp_message();
    }

    return res;
}
//=========================================================
void ZAddCalibrationDialog::zh_displayCalibrationProperties()
{
    if(zv_calibration == 0)
    {
        return;
    }

    // calibration name
    zv_calibrationNameLineEdit->setText(zv_calibration->zp_baseName());

    // calibration chem element
    zv_chemElementLineEdit->setText(zv_calibration->zp_chemElement());

    // R

    // R2

    // s


}
//=========================================================
void ZAddCalibrationDialog::zh_onBrowseClick()
{
    QString fileName = ZXMLCalibrationIOHandler::zp_getCalibrationOpenFile(zv_fileLocationDirString);

    if(fileName.isEmpty())
    {
        return;
    }

    // try to get calibration
    QString msg;
    if(!zh_createCalibrationFromFile(fileName, msg))
    {
        QMessageBox::critical(this, NS_CommonStrings::glError, msg, QMessageBox::Ok);
        return;
    }

    // set properties of the calibration to the display controls
    zh_displayCalibrationProperties();

    // set current file name and location
    QFileInfo fileInfo(fileName);
    zv_fileLocationDirString = fileInfo.absolutePath();
    zv_filePathLineEdit->setText(fileName);
}
//=========================================================
void ZAddCalibrationDialog::zh_onRangeChange(double value)
{
    QDoubleSpinBox* senderSpinBox = qobject_cast<QDoubleSpinBox*>(sender());
    if(senderSpinBox == 0)
    {
        return;
    }

    if(senderSpinBox == zv_rangeMinMarginSpinBox)
    {
        if(zv_rangeMinMarginSpinBox->value() > zv_rangeMaxMarginSpinBox->value())
        {
            zv_rangeMinMarginSpinBox->setValue(zv_rangePreviousMinValue);
        }
        else
        {
            zv_rangePreviousMinValue = zv_rangeMinMarginSpinBox->value();
        }
    }
    else if(senderSpinBox == zv_rangeMaxMarginSpinBox)
    {
        if(zv_rangeMinMarginSpinBox->value() > zv_rangeMaxMarginSpinBox->value())
        {
            zv_rangeMaxMarginSpinBox->setValue(zv_rangePreviousMaxValue);
        }
        else
        {
            zv_rangePreviousMaxValue = zv_rangeMaxMarginSpinBox->value();
        }
    }
}
//=========================================================
void ZAddCalibrationDialog::zh_onOkClick()
{
    QFileInfo fileInfo(zv_filePathLineEdit->text());
    QString msg;
    if(!fileInfo.exists())
    {
        msg = tr("File \"%1\" does not exist!");
        QMessageBox::critical(this, NS_CommonStrings::glError, msg, QMessageBox::Ok);
        return;
    }

    if(zv_calibrationNameLineEdit->text().isEmpty() || zv_chemElementLineEdit->text().isEmpty())
    {
        msg = tr("Not all fields are filled!");
        QMessageBox::critical(this, NS_CommonStrings::glError, msg, QMessageBox::Ok);
        return;
    }

    accept();
}
//=========================================================
