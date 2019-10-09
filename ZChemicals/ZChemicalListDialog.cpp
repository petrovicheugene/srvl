//===============================================================
#include "ZChemicalListDialog.h"
#include "ZGeneral.h"
#include "ZChemicalTableWidget.h"
#include "ZReadOnlyStyledItemDelegate.h"
#include "ZControlAction.h"
#include "ZChemicalDialog.h"

#include <QApplication>
#include <QDialogButtonBox>
#include <QEvent>
#include <QHBoxLayout>
#include <QLabel>
#include <QMessageBox>
#include <QMouseEvent>
#include <QPushButton>
#include <QSettings>
#include <QSqlTableModel>
#include <QSqlRecord>
#include <QSqlError>
#include <QSqlField>
#include <QTableView>
#include <QVBoxLayout>
//===============================================================
ZChemicalListDialog::ZChemicalListDialog(QSqlTableModel* chemicalModel, QWidget *parent) : QDialog(parent)
{
    setWindowTitle(tr("Chemicals"));
    setWindowFlags(Qt::Tool);

    if(chemicalModel != 0)
    {
        zv_chemicalTableModel = chemicalModel;
    }
    else
    {
        zv_chemicalTableModel = new QSqlTableModel(this);
        zv_chemicalTableModel->setTable("chemicals");
        zv_chemicalTableModel->select();
        zv_chemicalTableModel->setEditStrategy(QSqlTableModel::OnManualSubmit);
    }

    zh_createActions();
    zh_createComponents();
    zh_createConnections();
    zh_restoreSettings();

}
//===============================================================
ZChemicalListDialog::~ZChemicalListDialog()
{
    zh_saveSettings();
}
//===============================================================
void ZChemicalListDialog::zh_createActions()
{
    zv_newChemicalAction = new ZControlAction(this);
    zv_newChemicalAction->setText(tr("New"));
    zv_newChemicalAction->setIcon(QIcon(":/images/ZImages/new-8"));
    zv_newChemicalAction->setToolTip(tr("Create new chemical"));

    zv_copyChemicalAction = new ZControlAction(this);
    zv_copyChemicalAction->setEnabled(false);
    zv_copyChemicalAction->setText(tr("Copy"));
    zv_copyChemicalAction->setIcon(QIcon(":/images/ZImages/copy-8"));
    zv_copyChemicalAction->setToolTip(tr("Copy current chemical"));

}
//===============================================================
void ZChemicalListDialog::zh_createComponents()
{
    QVBoxLayout* mainLayout = new QVBoxLayout;
    setLayout(mainLayout);

    // chemical table
    zv_chemicalTableWidget = new ZChemicalTableWidget(this);
    zv_chemicalTableWidget->zp_setCaption(glCreateCaption(tr("Chemicals:")));
    mainLayout->addWidget(zv_chemicalTableWidget);

    // message
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
void ZChemicalListDialog::zh_createConnections()
{
    zv_chemicalTableWidget->zp_setModel(zv_chemicalTableModel);
    zv_chemicalTableWidget->zp_setColumnHidden(0, true);
    zv_chemicalTableWidget->zp_setVerticalHeaderHidden(true);
    zv_chemicalTableWidget->zp_setStretchLastSection(true);
    if(zv_chemicalTableWidget->zp_tableView()->itemDelegate())
    {
        delete zv_chemicalTableWidget->zp_tableView()->itemDelegate();
    }
    ZReadOnlyStyledItemDelegate* delegate = new ZReadOnlyStyledItemDelegate;
    zv_chemicalTableWidget->zp_tableView()->setItemDelegate(delegate);
    zv_chemicalTableWidget->zp_tableView()->setSelectionMode(QAbstractItemView::SingleSelection);
    zv_chemicalTableWidget->zp_tableView()->setSelectionBehavior(QAbstractItemView::SelectRows);

    // add actions to widgets
    QList<ZControlAction*> actionList;
    actionList.append(zv_newChemicalAction);
    actionList.append(zv_copyChemicalAction);
    zv_chemicalTableWidget->zp_appendButtonActions(actionList);
    zv_chemicalTableWidget->zp_tableView()->viewport()->installEventFilter(this);


    connect(zv_okButton, &QPushButton::clicked,
            this, &ZChemicalListDialog::zh_onOkButtonClick);
    connect(zv_cancelButton, &QPushButton::clicked,
            this, &ZChemicalListDialog::reject);

    connect(zv_chemicalTableWidget, &ZChemicalTableWidget::zg_currentChanged,
            this, &ZChemicalListDialog::zh_onChemicalChange);

    connect(zv_newChemicalAction, &ZControlAction::triggered,
            this, &ZChemicalListDialog::zh_onNewChemicalAction);
    connect(zv_copyChemicalAction, &ZControlAction::triggered,
            this, &ZChemicalListDialog::zh_onEditChemicalAction);

}
//===============================================================
bool ZChemicalListDialog::eventFilter(QObject* object, QEvent* event)
{
    if(object == zv_chemicalTableWidget->zp_tableView()->viewport() && event->type() == QEvent::MouseButtonDblClick)
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
//===============================================================
void ZChemicalListDialog::zh_restoreSettings()
{
    QSettings settings;
    QVariant vData;
    settings.beginGroup(qApp->applicationVersion());
    settings.beginGroup("ChemicalListDialog");

    vData = settings.value("dialogGeometry");
    if(vData.isValid() && !vData.isNull() && vData.canConvert<QByteArray>())
    {
        this->restoreGeometry(vData.value<QByteArray>());
    }

    settings.endGroup();
    settings.endGroup();
}
//===============================================================
void ZChemicalListDialog::zh_saveSettings() const
{
    QSettings settings;
    settings.beginGroup(qApp->applicationVersion());
    settings.beginGroup("ChemicalListDialog");

    settings.setValue("dialogGeometry", QVariant::fromValue<QByteArray>(this->saveGeometry()));
    //settings.setValue("splitterGeometry", QVariant::fromValue<QByteArray>(zv_tableSplitter->saveState()));

    settings.endGroup();
    settings.endGroup();
}
//===============================================================
int ZChemicalListDialog::zp_chemicalId() const
{
    QModelIndex index = zv_chemicalTableWidget->zp_tableView()->currentIndex();
    if(!index.isValid())
    {
        return -1;
    }

    int row = index.row();
    index = zv_chemicalTableModel->index(row, 0);
    if(!index.isValid())
    {
        return -1;
    }

    QVariant vData = index.data(Qt::DisplayRole);
    if(!vData.isValid() || !vData.canConvert<int>())
    {
        return -1;
    }

    bool ok;
    int id = vData.toInt(&ok);
    if(!ok)
    {
        return -1;
    }

    return id;
}
//===============================================================
QString ZChemicalListDialog::zp_chemical() const
{
    QModelIndex index = zv_chemicalTableWidget->zp_tableView()->currentIndex();
    if(!index.isValid())
    {
        return QString();
    }

    int row = index.row();
    index = zv_chemicalTableModel->index(row, 1);
    if(!index.isValid())
    {
        return QString();
    }

    QVariant vData = index.data(Qt::DisplayRole);
    if(!vData.isValid() || !vData.canConvert<QString>())
    {
        return QString();
    }

    return vData.toString();;
}
//===============================================================
void ZChemicalListDialog::zh_onOkButtonClick()
{
    if(!zh_checkData())
    {
        return;
    }
    accept();
}
//===============================================================void ZChemicalTaskListDialog::zh_onNewChemicalAction()
void ZChemicalListDialog::zh_onNewChemicalAction()
{
    ZChemicalDialog dialog;
    connect(&dialog, &ZChemicalDialog::zg_checkChemical,
            this, &ZChemicalListDialog::zh_checkChemical);

    if(!dialog.exec())
    {
        return;
    }

    QString newChemical = dialog.zp_chemical();
    if(newChemical.isEmpty())
    {
        return;
    }

    // find new chemical id
    int newChemicalId = 0;
    int currentChemicalId;
    QModelIndex index;
    QVariant vData;
    bool ok;
    for(int row = 0; row < zv_chemicalTableModel->rowCount(); row++)
    {
        index = zv_chemicalTableModel->index(row, 0);
        if(!index.isValid())
        {
            continue;
        }
        vData = index.data(Qt::DisplayRole);
        if(!vData.isValid() || !vData.canConvert<int>())
        {
            continue;
        }

        currentChemicalId = vData.toInt(&ok);
        if(!ok)
        {
            continue;
        }
        if(newChemicalId < currentChemicalId)
        {
            newChemicalId = currentChemicalId;
        }
    }
    newChemicalId++;
    // write to model
    QSqlRecord record;
    record.append(QSqlField("id", QVariant::Int));
    record.append(QSqlField("name", QVariant::String));

    record.setValue(0, QVariant(newChemicalId));
    record.setValue(1, QVariant(newChemical));

    if(!zv_chemicalTableModel->insertRecord(-1, record))
    {
        QString msg = zv_chemicalTableModel->lastError().text();
        QMessageBox::critical(this, tr("Error"), tr("Model data record error: %1").arg(msg), QMessageBox::Ok);
        return;
    }

    if(!zv_chemicalTableModel->submitAll())
    {
        QString msg = zv_chemicalTableModel->lastError().text();
        QMessageBox::critical(this, tr("Error"), tr("Database record error: %1").arg(msg), QMessageBox::Ok);
        zv_chemicalTableModel->revertAll();
        return;
    }

    int row = zv_chemicalTableModel->rowCount() - 1;
    QModelIndex insertedIndex = zv_chemicalTableModel->index(row, 1);
    zv_chemicalTableWidget->zp_tableView()->setCurrentIndex(insertedIndex);
    zv_chemicalTableWidget->zp_tableView()->scrollTo(insertedIndex);
}
//===============================================================
void ZChemicalListDialog::zh_onEditChemicalAction()
{
    // get current chemical data
    QModelIndex index = zv_chemicalTableWidget->zp_tableView()->currentIndex();
    if(!index.isValid())
    {
        QString msg = tr("Cannot get model current index.");
        QMessageBox::critical(this, tr("Error"), tr("Model error: %1").arg(msg), QMessageBox::Ok);
        return;
    }

    int row = index.row();
    // id
    index = zv_chemicalTableModel->index(row, 0);
    if(!index.isValid())
    {
        QString msg = tr("Cannot get model index for id of current chemical.");
        QMessageBox::critical(this, tr("Error"), tr("Model error: %1").arg(msg), QMessageBox::Ok);
        return;
    }

    QVariant vData = index.data(Qt::DisplayRole);
    if(!vData.isValid() || !vData.canConvert<int>())
    {
        QString msg = tr("Cannot get model data for id of current chemical.");
        QMessageBox::critical(this, tr("Error"), tr("Model error: %1").arg(msg), QMessageBox::Ok);
        return;
    }

    bool ok;
    int chemicalId = vData.toInt(&ok);
    if(!ok)
    {
        QString msg = tr("Cannot cast id value from QVariant.");
        QMessageBox::critical(this, tr("Error"), tr("Model error: %1").arg(msg), QMessageBox::Ok);
        return;
    }

    // chemical string
    index = zv_chemicalTableModel->index(row, 1);
    if(!index.isValid())
    {
        QString msg = tr("Cannot get model index for current chemical name.");
        QMessageBox::critical(this, tr("Error"), tr("Model error: %1").arg(msg), QMessageBox::Ok);
        return;
    }

    vData = index.data(Qt::DisplayRole);
    if(!vData.isValid() || !vData.canConvert<QString>())
    {
        QString msg = tr("Cannot get model data for current chemical name.");
        QMessageBox::critical(this, tr("Error"), tr("Model error: %1").arg(msg), QMessageBox::Ok);
        return;
    }

    QString chemical = vData.toString();

    ZChemicalDialog dialog;
    connect(&dialog, &ZChemicalDialog::zg_checkChemical,
            this, &ZChemicalListDialog::zh_checkChemical);

    dialog.zp_setChemical(chemicalId, chemical);

    if(!dialog.exec())
    {
        return;
    }

    // get data from dialog
    chemical = dialog.zp_chemical();

    // write chemical
    index = zv_chemicalTableModel->index(row, 1);
    if(!index.isValid())
    {
        QString msg = tr("Cannot get model index for current chemical name.");
        QMessageBox::critical(this, tr("Error"), tr("Model error: %1").arg(msg), QMessageBox::Ok);
        return;
    }

    if(!zv_chemicalTableModel->setData(index, QVariant(chemical)))
    {
        QString msg = zv_chemicalTableModel->lastError().text();
        QMessageBox::critical(this, tr("Error"), tr("Model error: %1").arg(msg), QMessageBox::Ok);
        return;
    }

    if(!zv_chemicalTableModel->submitAll())
    {
        QString msg = zv_chemicalTableModel->lastError().text();
        QMessageBox::critical(this, tr("Error"), tr("Database record error: %1").arg(msg), QMessageBox::Ok);
        zv_chemicalTableModel->revertAll();
        return;
    }
}
//===============================================================
void ZChemicalListDialog::zh_checkChemical(int id, const QString& chemical, bool& res) const
{
    QModelIndex index;
    QVariant vData;
    for(int row = 0; row < zv_chemicalTableModel->rowCount(); row++)
    {
        index = zv_chemicalTableModel->index(row, 1);
        if(!index.isValid())
        {
            continue;
        }

        vData = index.data(Qt::DisplayRole);
        if(!vData.isValid() || !vData.canConvert<QString>())
        {
            continue;
        }

        if(chemical == vData.toString())
        {
            // check Id
            index = zv_chemicalTableModel->index(row, 0);
            if(!index.isValid())
            {
                continue;
            }
            vData = index.data(Qt::DisplayRole);
            if(!vData.isValid() || !vData.canConvert<int>())
            {
                continue;
            }
            bool ok;
            int currentId = vData.toInt(&ok);
            if(!ok)
            {
                continue;
            }

            if(id ==  currentId)
            {
                continue;
            }
            res = false;
            return;
        }
    }

    res = true;
}
//===============================================================
void ZChemicalListDialog::zh_onChemicalChange(const QModelIndex& current, const QModelIndex& previous)
{
    // get current chemical Id
    int currentChemicalId = 0;
    if(current.isValid())
    {
        int currentRow = current.row();
        QModelIndex idIndex = zv_chemicalTableModel->index(currentRow, 0);
        if(idIndex.isValid())
        {
            QVariant vData = idIndex.data(Qt::DisplayRole);
            if(vData.isValid() && vData.canConvert<int>())
            {
                currentChemicalId = vData.toInt();
            }
        }
    }

    zv_copyChemicalAction->setEnabled(currentChemicalId > 0);
}
//===============================================================
bool ZChemicalListDialog::zh_checkData()
{
    if(zp_chemicalId() < 1 || zp_chemical().isEmpty())
    {
        QString msg = tr("Chemical is not selected.");
        zv_messageLabel->setText(QString("<font color=red>%1</font>").arg(msg));
        return false;
    }

    return true;
}
//===============================================================
