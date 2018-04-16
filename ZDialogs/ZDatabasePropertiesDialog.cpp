//==========================================================
#include "ZDatabasePropertiesDialog.h"
#include "ZGeneral.h"

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QDialogButtonBox>

#include <QPushButton>
#include <QLineEdit>
#include <QLabel>

#include <QFileDialog>
#include <QDir>
#include <QSettings>
#include <QStandardPaths>
#include <QMessageBox>

//==========================================================
ZDatabasePropertiesDialog::ZDatabasePropertiesDialog(const QString& name,
                                                     const QString& path,
                                                     QWidget *parent) : QDialog(parent)
{
    setWindowTitle(glAppProduct);
    setWindowFlags(Qt::Tool);

    zh_createComponents();
    zh_createConnections();
    zh_restoreSettings();

    zv_nameLineEdit->setText(name);
    zv_pathLineEdit->setText(path);
}
//==========================================================
ZDatabasePropertiesDialog::~ZDatabasePropertiesDialog()
{
    zh_saveSettings();
}
//==========================================================
QString ZDatabasePropertiesDialog::zp_name() const
{
    return zv_nameLineEdit->text();
}
//==========================================================
QString ZDatabasePropertiesDialog::zp_path() const
{
    return zv_pathLineEdit->text();
}
//==========================================================
void ZDatabasePropertiesDialog::zh_createComponents()
{
    // main layout
    QVBoxLayout* mainLayout = new QVBoxLayout(this);
    setLayout(mainLayout);

    QGridLayout* controlLayout = new QGridLayout(this);
    mainLayout->addLayout(controlLayout);

    // name level
    QLabel* label = new QLabel(this);
    label->setText(QString("<b>%1</b>").arg(tr("Name:")));
    controlLayout->addWidget(label, 0, 0, 1, 2);

    zv_nameLineEdit = new QLineEdit(this);
    controlLayout->addWidget(zv_nameLineEdit, 1, 0, 1, 1);

    // path level
    label = new QLabel(this);
    label->setText(QString("<b>%1</b>").arg(tr("Path:")));
    controlLayout->addWidget(label, 3, 0, 1, 2);

    zv_pathLineEdit = new QLineEdit(this);
    controlLayout->addWidget(zv_pathLineEdit, 4, 0, 1, 1);

    zv_browseButton = new QPushButton(this);
    zv_browseButton->setText(tr("Browse..."));
    controlLayout->addWidget(zv_browseButton, 4, 1, 1, 1);

    // buttons
    QFrame* line = new QFrame(this);
    line->setFrameStyle(QFrame::HLine | QFrame::Sunken);
    line->setLineWidth(1);
    mainLayout->addWidget(line);

    QDialogButtonBox* bottomButtonBox = new QDialogButtonBox(Qt::Horizontal, this);
    mainLayout->addWidget(bottomButtonBox);

    zv_okButton = new QPushButton(this);
    zv_okButton->setText(tr("OK"));
    bottomButtonBox->addButton(zv_okButton, QDialogButtonBox::ActionRole);

    zv_cancelButton = new QPushButton(this);
    zv_cancelButton->setText(tr("Cancel"));
    bottomButtonBox->addButton(zv_cancelButton, QDialogButtonBox::ActionRole);

    zv_nameLineEdit->setFocus();
}
//==========================================================
void ZDatabasePropertiesDialog::zh_createConnections()
{
    connect(zv_cancelButton, &QPushButton::clicked,
            this, &ZDatabasePropertiesDialog::reject);
    connect(zv_browseButton, &QPushButton::clicked,
            this, &ZDatabasePropertiesDialog::zh_onBrowseButtonClick);
    connect(zv_okButton, &QPushButton::clicked,
            this, &ZDatabasePropertiesDialog::zh_onOkButtonClick);
}
//==========================================================
void ZDatabasePropertiesDialog::zh_restoreSettings()
{
    QSettings settings;
    settings.beginGroup(glAppVersion);
    settings.beginGroup(zv_settingsGroupName);

    QVariant vData = settings.value(zv_defaultDatabaseFolderSectionName);

    if(vData.isValid() && !vData.isNull() && vData.canConvert<QString>())
    {
        zv_databaseFolderPath = vData.toString();
    }
    else
    {
        QStringList standardLocations =
                QStandardPaths::standardLocations(QStandardPaths::DocumentsLocation);
        if(!standardLocations.isEmpty())
        {
            zv_databaseFolderPath = standardLocations.value(0);
        }
        else
        {
            zv_databaseFolderPath = QDir::currentPath();
        }

    }
    settings.endGroup();
    settings.endGroup();

}
//==========================================================
void ZDatabasePropertiesDialog::zh_saveSettings() const
{
    QSettings settings;
    settings.beginGroup(glAppVersion);
    settings.beginGroup(zv_settingsGroupName);
    settings.setValue(zv_defaultDatabaseFolderSectionName, QVariant(zv_databaseFolderPath));
    settings.endGroup();
    settings.endGroup();
}
//==========================================================
void ZDatabasePropertiesDialog::zh_onBrowseButtonClick()
{
    // run file dialog
    QFileDialog fileDialog(this, tr("Select database file"),
                                         zv_databaseFolderPath,
                                         tr("sqlite database files(*.sqlite);;All files(*.*)"));

    fileDialog.setFileMode(QFileDialog::AnyFile);

    if(!fileDialog.exec())
    {
        return;
    }

    // get names (should be the one only)
    QStringList fileNames = fileDialog.selectedFiles();
    if(fileNames.isEmpty())
    {
        return;
    }

    if(fileNames.value(0).isEmpty())
    {
        return;
    }

    // save folder path
    QFileInfo fileInfo(fileNames.value(0));
    zv_databaseFolderPath = fileInfo.absolutePath();

    // set db file
    zv_pathLineEdit->setText(fileNames.value(0));
}
//==========================================================
void ZDatabasePropertiesDialog::zh_onOkButtonClick()
{
    // check database properties
    bool res;
    QString name = zv_nameLineEdit->text();
    QString path = zv_pathLineEdit->text();

    emit zg_inquiryRecordDatabase(name, path, res);
    if(res)
    {
        accept();
    }

//    // if a suffix has been appended to path repalace zv_pathLineEdit text
//    if(zv_pathLineEdit->text() != path)
//    {
//        zv_pathLineEdit->setText(path);
//    }

}
//==========================================================
