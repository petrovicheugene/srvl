//===============================================================
#include "ZSampleTaskDialog.h"
#include "ZGLConstantsAndDefines.h"
//#include "ZTaskMeasuringConditionTableWidget.h"
//#include "ZChemicalTaskTableWidget.h"
//#include "ZTaskCalibrationStackTableWidget.h"
#include "ZSampleTaskTreeWidget.h"
#include "ZSampleTaskTreeModel.h"

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QDialogButtonBox>

#include <QEvent>
#include <QFrame>
#include <QLineEdit>
#include <QLabel>
#include <QPushButton>
#include <QSpinBox>
#include <QSettings>
#include <QSplitter>
#include <QTableView>
#include <QTextEdit>
//===============================================================
ZSampleTaskDialog::ZSampleTaskDialog(QWidget *parent) : QDialog(parent)
{
    setWindowTitle(tr("Sample task"));
    zv_sampleTaskTreeModel = 0;
    zv_sampleTaskId = -1;
    zh_createComponents();
    zh_createConnections();
    zh_restoreSettings();
}
//===============================================================
QString ZSampleTaskDialog::zp_sampleTaskName() const
{
    return zv_sampleTaskNameLineEdit->text();
}
//===============================================================
QString ZSampleTaskDialog::zp_sampleNameTemplate() const
{
    return zv_sampleNameTemplateLineEdit->text();
}
//===============================================================
QString ZSampleTaskDialog::zp_description() const
{
    return zv_descriptionTextEdit->toPlainText();
}
//===============================================================
QList<ZMeasuringTaskInitStruct> ZSampleTaskDialog::zp_measuringTaskInitList() const
{
    QList<ZMeasuringTaskInitStruct> measuringTaskInitList;

    return measuringTaskInitList;
}
//===============================================================
bool ZSampleTaskDialog::zp_loadSampleTask(int sampleTaskId,
                                          const QString& sampleTaskName,
                                          const QString& sampleNameTemplate,
                                          const QString& description,
                                          const QList<ZMeasuringTaskInitStruct>& measuringTaskInitStructList)
{
    zv_sampleTaskId = sampleTaskId;
    zv_sampleTaskNameLineEdit->setText(sampleTaskName);
    zv_sampleNameTemplateLineEdit->setText(sampleNameTemplate);
    zv_descriptionTextEdit->setText(description);


    return true;
}
//===============================================================
void ZSampleTaskDialog::closeEvent(QCloseEvent* event)
{
    zh_saveSettings();
}
//===============================================================
void ZSampleTaskDialog::zh_createComponents()
{
    // model
    zv_sampleTaskTreeModel = new ZSampleTaskTreeModel(this);

    // Controls & widgets
    QVBoxLayout* mainLayout = new QVBoxLayout(this);
    setLayout(mainLayout);

    // name lineEdit
    QLabel* label = new QLabel(this);
    label->setText(glCreateCaption(tr("Task name:")));
    mainLayout->addWidget(label);

    QHBoxLayout* levelLayout = new QHBoxLayout(this);
    mainLayout->addLayout(levelLayout);
    zv_sampleTaskNameLineEdit = new QLineEdit(this);
    levelLayout->addWidget(zv_sampleTaskNameLineEdit);
    levelLayout->addStretch();

    // sample name template
    label = new QLabel(this);
    label->setText(glCreateCaption(tr("Sample name template:")));
    mainLayout->addWidget(label);

    levelLayout = new QHBoxLayout(this);
    mainLayout->addLayout(levelLayout);
    zv_sampleNameTemplateLineEdit = new QLineEdit(this);
    levelLayout->addWidget(zv_sampleNameTemplateLineEdit);
    label = new QLabel(this);
    label->setText(glCreateCaption(tr(" + sample serial number")));
    levelLayout->addWidget(label);
    levelLayout->addStretch();

    // description
    label = new QLabel(this);
    label->setText(glCreateCaption(tr("Description:")));
    mainLayout->addWidget(label);

    zv_descriptionTextEdit = new QTextEdit(this);
    int descriptionTextEditMaxHeight = zv_sampleTaskNameLineEdit->height() * 2;
    zv_descriptionTextEdit->setMaximumHeight(descriptionTextEditMaxHeight);
    zv_descriptionTextEdit->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    mainLayout->addWidget(zv_descriptionTextEdit);

    // tree view widget
    label = new QLabel(this);
    label->setText(glCreateCaption(tr("Task stages:")));
    mainLayout->addWidget(label);

    zv_sampleTaskTreeWidget = new ZSampleTaskTreeWidget(this);
    mainLayout->addWidget(zv_sampleTaskTreeWidget);

    zv_messageLabel = new QLabel(this);
    zv_messageLabel->setWordWrap(true);
    mainLayout->addWidget(zv_messageLabel);

    // Basement
    // mainLayout->addStretch();
    // sparator line
    QFrame* line = new QFrame(this);
    line->setFrameStyle(QFrame::HLine | QFrame::Sunken);
    line->setLineWidth(1);
    mainLayout->addWidget(line);

    // button box
    QDialogButtonBox* buttonBox = new QDialogButtonBox(this);
    mainLayout->addWidget(buttonBox);

    // buttons
    zv_okButton = new QPushButton(this);
    zv_okButton->setText(tr("OK"));
    buttonBox->addButton(zv_okButton, QDialogButtonBox::ActionRole);

    zv_cancelButton = new QPushButton(this);
    zv_cancelButton->setText(tr("Cancel"));
    buttonBox->addButton(zv_cancelButton, QDialogButtonBox::ActionRole);
}
//===============================================================
void ZSampleTaskDialog::zh_createConnections()
{
    zv_sampleTaskTreeWidget->zp_setModel(zv_sampleTaskTreeModel);
    zv_sampleTaskTreeWidget->zp_setSectionResizeMode(0, QHeaderView::ResizeToContents);
    zv_sampleTaskTreeWidget->zp_setStretchLastSection(true);
    zv_sampleTaskTreeWidget->zp_setHeaderVisible(false);

    zv_sampleTaskTreeWidget->zp_appendButtonActions(zv_sampleTaskTreeModel->zp_buttonTreeActions());
    zv_sampleTaskTreeWidget->zp_appendContextActions(zv_sampleTaskTreeModel->zp_contextTreeActions());

    connect(zv_okButton, &QPushButton::clicked,
            this, &ZSampleTaskDialog::zh_onOkButtonClick);
    connect(zv_cancelButton, &QPushButton::clicked,
            this, &ZSampleTaskDialog::reject);
    connect(zv_sampleTaskNameLineEdit, &QLineEdit::textChanged,
            this, &ZSampleTaskDialog::zh_onLineEditDataChange);
    connect(zv_sampleNameTemplateLineEdit, &QLineEdit::textChanged,
            this, &ZSampleTaskDialog::zh_onLineEditDataChange);

    connect(zv_sampleTaskTreeModel, &QAbstractItemModel::rowsInserted,
            this, &ZSampleTaskDialog::zh_onModelRowCountChange);
    connect(zv_sampleTaskTreeModel, &QAbstractItemModel::rowsRemoved,
            this, &ZSampleTaskDialog::zh_onModelRowCountChange);

}
//===============================================================
void ZSampleTaskDialog::zh_restoreSettings()
{
    QSettings settings;
    QVariant vData;
    settings.beginGroup(glAppVersion);
    settings.beginGroup("SampleTaskDialog");

    vData = settings.value("dialogGeometry");
    if(vData.isValid() && !vData.isNull() && vData.canConvert<QByteArray>())
    {
        this->restoreGeometry(vData.value<QByteArray>());
    }

    settings.endGroup();
    settings.endGroup();
}
//===============================================================
void ZSampleTaskDialog::zh_saveSettings() const
{
    QSettings settings;
    settings.beginGroup(glAppVersion);
    settings.beginGroup("SampleTaskDialog");

    settings.setValue("dialogGeometry", QVariant::fromValue<QByteArray>(this->saveGeometry()));
    //settings.setValue("splitterGeometry", QVariant::fromValue<QByteArray>(zv_tableSplitter->saveState()));

    settings.endGroup();
    settings.endGroup();
}
//===============================================================
bool ZSampleTaskDialog::zh_checkDataEmptiness()
{
    QString msg;
    if(zv_sampleTaskNameLineEdit->text().isEmpty())
    {
        msg = tr("Sample task name ");
    }

    if(zv_sampleNameTemplateLineEdit->text().isEmpty())
    {
        if(msg.isEmpty())
        {
            msg = tr("Sample name template");
        }
        else
        {
             msg += tr("and sample name template");
        }

        msg += tr("cannot be empty.");
    }

    if(zv_sampleTaskTreeModel->zp_childCount(QModelIndex()) < 1)
    {
        if(!msg.isEmpty())
        {
            msg += "<br>";
        }
        msg += tr("Sample task must contain at least 1 measuring task.");
    }

    if(!msg.isEmpty())
    {
        zv_messageLabel->setText(QString("<font color=red>%1</font>").arg(msg));
    }

    return msg.isEmpty();
}
//===============================================================
void ZSampleTaskDialog::zh_onOkButtonClick()
{
    // Check data
    // eptiness check
    if(!zh_checkDataEmptiness())
    {
        return;
    }

    bool res;
    emit zg_checkTaskName(zv_sampleTaskId, zv_sampleTaskNameLineEdit->text(), res);
    if(!res)
    {
        QString msg = tr("Sample task name is not unique.");
        zv_messageLabel->setText(QString("<font color=red>%1</font>").arg(msg));
        return;
    }

    accept();

    //    bool res = false;
    //    // check sample name
    //    emit zg_checkTaskName(zv_sampleTaskNameLineEdit->text(), res);

    //    if(!res)
    //    {
    //        return;
    //    }

    //    accept();
}
//===============================================================
void ZSampleTaskDialog::zh_onLineEditDataChange(const QString& text)
{
    zv_messageLabel->clear();
}
//===============================================================
void ZSampleTaskDialog::zh_onModelRowCountChange(const QModelIndex& parent, int first, int last)
{
    zv_messageLabel->clear();
}
//===============================================================
