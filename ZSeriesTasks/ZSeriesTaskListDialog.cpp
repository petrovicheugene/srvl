//=====================================================
#include "ZSeriesTaskListDialog.h"
#include "ZGeneral.h"
#include "ZSeriesTaskListManager.h"
#include "ZSeriesTaskTableWidget.h"

#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QDialogButtonBox>

#include <QAbstractItemView>
#include <QFrame>
#include <QHeaderView>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QSettings>
#include <QTableView>
#include <QTextEdit>
//=====================================================
ZSeriesTaskListDialog::ZSeriesTaskListDialog(bool forLoad, QWidget *parent)
    : QDialog(parent)
{
    setWindowFlags(Qt::Tool);

    zv_manager = 0;
    zv_forLoad = forLoad;
    zv_seriesNameLineEdit = 0;
    zv_descriptionTextEdit = 0;

    zh_createComponents();
    zh_createConnections();
    zh_restoreSettings();

}
//=====================================================
ZSeriesTaskListDialog::~ZSeriesTaskListDialog()
{
    zh_saveSettings();
}
//=====================================================
void ZSeriesTaskListDialog::zh_createComponents()
{
    QVBoxLayout* mainLayout = new QVBoxLayout;
    setLayout(mainLayout);

    zv_seriesTaskTableWidget = new ZSeriesTaskTableWidget();
    zv_seriesTaskTableWidget->zp_setCaption(glCreateCaption(tr("Series tasks:")));
    mainLayout->addWidget(zv_seriesTaskTableWidget);

    QLabel* label;
    if(!zv_forLoad)
    {
        // name
        label = new QLabel(this);
        label->setText(glCreateCaption(tr("Series name:")));
        mainLayout->addWidget(label);

        zv_seriesNameLineEdit = new QLineEdit(this);
        // label->setBuddy(zv_seriesNamelineEdit);
        mainLayout->addWidget(zv_seriesNameLineEdit);

        // description
        label = new QLabel(this);
        label->setText(glCreateCaption(tr("Description:")));
        mainLayout->addWidget(label);

        zv_descriptionTextEdit = new QTextEdit(this);
        int descriptionTextEditMaxHeight = zv_seriesNameLineEdit->height() * 2;
        zv_descriptionTextEdit->setMaximumHeight(descriptionTextEditMaxHeight);
        zv_descriptionTextEdit->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
        mainLayout->addWidget(zv_descriptionTextEdit);
    }

    // basement
    // Separator
    QFrame* line = new QFrame(this);
    line->setFrameStyle(QFrame::HLine | QFrame::Sunken);
    line->setLineWidth(1);
    mainLayout->addWidget(line);

    QDialogButtonBox* buttonBox = new QDialogButtonBox(this);
    mainLayout->addWidget(buttonBox);

    // buttons
    //    zv_newTaskButton = new QPushButton(NS_Buttons::glButtonNew, this);
    //    buttonBox->addButton(zv_newTaskButton, QDialogButtonBox::ActionRole);

    QString okButtonCaption;
    QString okToolTip;
    if(zv_forLoad)
    {
        okButtonCaption = tr("Load");
        okToolTip = tr("Load series task");
    }
    else
    {
        okButtonCaption = zv_forLoad ? tr("Load") : tr("Save");
        okToolTip = tr("Save series task");
    }

    zv_okButton = new QPushButton(okButtonCaption, this);
    zv_okButton->setToolTip(okToolTip);
    buttonBox->addButton(zv_okButton, QDialogButtonBox::ActionRole);

    zv_cancelButton = new QPushButton(tr("Cancel"), this);
    zv_cancelButton->setToolTip(tr("Cancel and close dialog"));
    buttonBox->addButton(zv_cancelButton, QDialogButtonBox::ActionRole);

}
//=====================================================
void ZSeriesTaskListDialog::zh_createConnections()
{
    connect(zv_cancelButton, &QPushButton::clicked,
            this, &ZSeriesTaskListDialog::reject);

    connect(zv_okButton, &QPushButton::clicked,
            this, &ZSeriesTaskListDialog::zh_onOkButtonClick);

}
//=====================================================
void ZSeriesTaskListDialog::zh_restoreSettings()
{
    QSettings settings;
    QVariant vData;
    settings.beginGroup(glAppVersion);
    settings.beginGroup("SeriesTaskListDialog");

    vData = settings.value("dialogGeometry");
    if(vData.isValid() && !vData.isNull() && vData.canConvert<QByteArray>())
    {
        this->restoreGeometry(vData.value<QByteArray>());
    }

    settings.endGroup();
    settings.endGroup();
}
//=====================================================
void ZSeriesTaskListDialog::zh_saveSettings() const
{
    QSettings settings;
    settings.beginGroup(glAppVersion);
    settings.beginGroup("SeriesTaskListDialog");

    settings.setValue("dialogGeometry", QVariant::fromValue<QByteArray>(this->saveGeometry()));
    //settings.setValue("splitterGeometry", QVariant::fromValue<QByteArray>(zv_tableSplitter->saveState()));

    settings.endGroup();
    settings.endGroup();
}
//=====================================================
bool ZSeriesTaskListDialog::eventFilter(QObject* object, QEvent* event)
{
    return QDialog::eventFilter(object, event);
}
//=====================================================
void ZSeriesTaskListDialog::zp_connectToManager(ZSeriesTaskListManager* manager)
{
    zv_manager = manager;
    // zv_manager->zp_connectViewToModel(zv_seriesTaskTableWidget->zp_tableView());
    zv_seriesTaskTableWidget->zp_setModel(zv_manager->zp_model());

    zv_seriesTaskTableWidget->zp_tableView()->setColumnHidden(0, true);
    zv_seriesTaskTableWidget->zp_tableView()->horizontalHeader()->stretchLastSection();

    connect(zv_seriesTaskTableWidget, &ZSeriesTaskTableWidget::zg_currentChanged,
            zv_manager, &ZSeriesTaskListManager::zp_onCurrentChange);
    connect(zv_manager, &ZSeriesTaskListManager::zg_currentTaskProperties,
            this, &ZSeriesTaskListDialog::zh_setCurrentTaskProperties);

    if(zv_forLoad)
    {

    }
    else
    {

    }
}
//=====================================================
QString ZSeriesTaskListDialog::zp_newSeriesTaskName() const
{
    if(!zv_seriesNameLineEdit)
    {
        return QString();
    }

    return zv_seriesNameLineEdit->text();
}
//=====================================================
void ZSeriesTaskListDialog::zh_setCurrentTaskProperties(const QString& name,
                                                        const QString& description)
{
    if(zv_seriesNameLineEdit)
    {
        zv_seriesNameLineEdit->setText(name);
    }

    if(zv_descriptionTextEdit)
    {
        zv_descriptionTextEdit->setText(description);
    }
}
//=====================================================
void ZSeriesTaskListDialog::zh_onOkButtonClick()
{
    if(!zv_manager)
    {
        return;
    }

    bool res;
    QString msg;
    if(zv_forLoad)
    {
        int currentId = zv_manager->zp_currentSeriesTaskId();
        res =currentId > 0;
    }
    else
    {
        res = zv_manager->zp_saveSeriesTask(zv_seriesNameLineEdit->text(), zv_descriptionTextEdit->toPlainText());
    }

    if(!res)
    {
        msg = zv_manager->zp_lastError();
        QMessageBox::critical(this, tr("Error"),msg, QMessageBox::Ok);
        return;
    }

    accept();
}
//=====================================================
