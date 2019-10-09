//=======================================================
#include "ZSampleTaskListDialog.h"
#include "ZGeneral.h"
#include "ZSampleTaskDialog2.h"
#include "ZMeasuringTaskInitStruct.h"

#include <QApplication>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QDialogButtonBox>

#include <QEvent>
#include <QHeaderView>
#include <QLabel>
#include <QMouseEvent>
#include <QMessageBox>
#include <QPushButton>
#include <QTableView>
#include <QSettings>
#include <QSqlError>
#include <QSqlTableModel>
#include <QSqlRecord>
#include <QSqlField>
//=======================================================
ZSampleTaskListDialog::ZSampleTaskListDialog(QWidget *parent)
    : QDialog(parent)
{
    setWindowTitle(tr("Sample task"));
    setWindowFlags(Qt::Tool);

    zh_createComponents();
    zh_createConnections();
    zh_restoreSettings();
}
//=======================================================
ZSampleTaskListDialog::~ZSampleTaskListDialog()
{
    zh_saveSettings();
}
//=======================================================
bool ZSampleTaskListDialog::eventFilter(QObject* object, QEvent* event)
{
    if(object == zv_sampleTaskTable->viewport() && event->type() == QEvent::MouseButtonDblClick)
    {
        QMouseEvent* mouseEvent = static_cast<QMouseEvent*>(event);
        if(mouseEvent && mouseEvent->button() == Qt::LeftButton)
        {
            zh_onOkButtonClick();
            return true;
        }
    }

    return QDialog::eventFilter(object, event);
}
//=======================================================
void ZSampleTaskListDialog::zh_createComponents()
{
    // model
    zv_sampleTaskTableModel = new QSqlTableModel(this);
    zv_sampleTaskTableModel->setTable("sample_tasks");
    zv_sampleTaskTableModel->setEditStrategy(QSqlTableModel::OnManualSubmit);
    zv_sampleTaskTableModel->select();

    zv_sampleTaskTableModel->setHeaderData(0, Qt::Horizontal, QVariant(tr("Id")));
    zv_sampleTaskTableModel->setHeaderData(1, Qt::Horizontal, QVariant(tr("Name")));
    zv_sampleTaskTableModel->setHeaderData(3, Qt::Horizontal, QVariant(tr("Description")));

    QVBoxLayout* mainLayout = new QVBoxLayout;
    setLayout(mainLayout);

            // name
            //    QLabel* label = new QLabel(this);
            //    label->setText(glCreateCaption(tr("Sample tasks:")));
            //    mainLayout->addWidget(label);

            zv_sampleTaskTable = new QTableView(this);
    mainLayout->addWidget(zv_sampleTaskTable);

    zv_messageLabel = new QLabel(this);
    zv_messageLabel->setWordWrap(true);
    mainLayout->addWidget(zv_messageLabel);

    // basement
    // Separator
    QFrame* line = new QFrame(this);
    line->setFrameStyle(QFrame::HLine | QFrame::Sunken);
    line->setLineWidth(1);
    mainLayout->addWidget(line);

    QDialogButtonBox* buttonBox = new QDialogButtonBox(this);
    mainLayout->addWidget(buttonBox);

    // buttons
    zv_newTaskButton = new QPushButton(NS_Buttons::glButtonNew, this);
    buttonBox->addButton(zv_newTaskButton, QDialogButtonBox::ActionRole);

    zv_okButton = new QPushButton(NS_Buttons::glButtonOk, this);
    buttonBox->addButton(zv_okButton, QDialogButtonBox::ActionRole);

    zv_cancelButton = new QPushButton(NS_Buttons::glButtonCancel, this);
    buttonBox->addButton(zv_cancelButton, QDialogButtonBox::ActionRole);
}
//======================================================
void ZSampleTaskListDialog::zh_createConnections()
{
    zv_sampleTaskTable->setModel(zv_sampleTaskTableModel);
    zv_sampleTaskTable->setColumnHidden(0, true);
    zv_sampleTaskTable->setColumnHidden(2, true);
    zv_sampleTaskTable->horizontalHeader()->setStretchLastSection(true);

    connect(zv_okButton, &QPushButton::clicked,
            this, &ZSampleTaskListDialog::zh_onOkButtonClick);
    connect(zv_cancelButton, &QPushButton::clicked,
            this, &ZSampleTaskListDialog::reject);
    connect(zv_newTaskButton, &QPushButton::clicked,
            this, &ZSampleTaskListDialog::zh_newSampleTask);

}
//======================================================
void ZSampleTaskListDialog::zh_restoreSettings()
{
    QSettings settings;
    QVariant vData;
    settings.beginGroup(qApp->applicationVersion());
    settings.beginGroup("SampleTaskListDialog");

    vData = settings.value("dialogGeometry");
    if(vData.isValid() && !vData.isNull() && vData.canConvert<QByteArray>())
    {
        this->restoreGeometry(vData.value<QByteArray>());
    }

    settings.endGroup();
    settings.endGroup();
}
//======================================================
void ZSampleTaskListDialog::zh_saveSettings() const
{
    QSettings settings;
    settings.beginGroup(qApp->applicationVersion());
    settings.beginGroup("SampleTaskListDialog");

    settings.setValue("dialogGeometry", QVariant::fromValue<QByteArray>(this->saveGeometry()));
    //settings.setValue("splitterGeometry", QVariant::fromValue<QByteArray>(zv_tableSplitter->saveState()));

    settings.endGroup();
    settings.endGroup();
}
//======================================================
void ZSampleTaskListDialog::zh_onOkButtonClick() const
{

}
//======================================================
void ZSampleTaskListDialog::zh_newSampleTask()
{
    ZSampleTaskDialog2 dialog;
    //    connect(&dialog, &ZSampleTaskDialog2::zg_checkTaskName,
    //            this, &ZSampleTaskListDialog::zh_checkTaskName);

    if(!dialog.exec())
    {
        return;
    }

}
//======================================================
bool ZSampleTaskListDialog::zh_removeSampleTaskFromTable(int row)
{
    if(row >= zv_sampleTaskTableModel->rowCount())
    {
        return false;
    }

    if(row < 0)
    {
        // last row
        row = zv_sampleTaskTableModel->rowCount() - 1;
    }

    if(!zv_sampleTaskTableModel->removeRow(zv_sampleTaskTableModel->rowCount()-1))
    {
        return false;
    }

    //zv_sampleTaskTableModel->submitAll();
    return true;
}
//======================================================
void ZSampleTaskListDialog::zh_checkTaskName(int sampleTaskId, const QString& taskName, bool& res) const
{
    QModelIndex index;
    QVariant vData;
    for(int row = 0; row < zv_sampleTaskTableModel->rowCount(); row++)
    {
        index = zv_sampleTaskTableModel->index(row, 1);
        if(!index.isValid())
        {
            continue;
        }

        vData = index.data(Qt::DisplayRole);
        if(!vData.isValid() || !vData.canConvert<QString>())
        {
            continue;
        }

        if(vData.toString() == taskName)
        {
            // check id
            index = zv_sampleTaskTableModel->index(row, 0);
            if(!index.isValid())
            {
                res = false;
                return;
            }

            vData = index.data(Qt::DisplayRole);
            if(!vData.isValid() || !vData.canConvert<int>())
            {
                res = false;
                return;
            }
            if(vData.toInt() == sampleTaskId)
            {
                continue;
            }

            res = false;
            return;
        }
    }

    res = true;
}
//======================================================
