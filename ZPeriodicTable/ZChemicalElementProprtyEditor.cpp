//======================================================
#include "ZChemicalElementProprtyEditor.h"
#include "ZCaptionFormatter.h"
#include "ZChemicalPropertyProxyTableModel.h"
#include "ZPropertyEditDialog.h"
#include "ZPropertySectionEditDialog.h"

#include <QAbstractItemModel>
#include <QApplication>
#include <QAction>
#include <QClipboard>
#include <QDebug>
#include <QDialogButtonBox>
#include <QFileDialog>
#include <QHeaderView>
#include <QKeyEvent>
#include <QLabel>
#include <QMessageBox>
#include <QMimeData>
#include <QPushButton>
#include <QSettings>
#include <QSplitter>
#include <QStandardPaths>
#include <QStyledItemDelegate>
#include <QTableView>
#include <QTreeView>
#include <QVBoxLayout>

//======================================================
// STATIC
QString ZChemicalElementProprtyEditor::zv_splitterStateName = "splitterState";
// END STATIC
//======================================================
ZChemicalElementProprtyEditor::ZChemicalElementProprtyEditor(QWidget *parent)
    : ZBaseDialog(QString("ZChemicalElementProprtyEditor"), parent)
{
    setWindowTitle(qApp->property("glAppProduct").toString());

    zh_createActions();
    zh_createComponents();
    zh_createMenu();
    zh_createConnections();
    zh_restoreSettings();

    zh_adjustRemoveAndCopyPropertyAbility(QItemSelection());
    zh_adjustRemoveSectionAbility(QItemSelection());
    zh_adjustPastePropertyAbility();
}
//======================================================
ZChemicalElementProprtyEditor::~ZChemicalElementProprtyEditor()
{
    zh_saveSettings();
}
//======================================================
void ZChemicalElementProprtyEditor::zh_createActions()
{
    zv_addChemicalElementPropertySectionsAction = new QAction(this);
    zv_addChemicalElementPropertySectionsAction->setText(tr("Add"));

    zv_removeChemicalElementPropertySectionsAction = new QAction(this);
    zv_removeChemicalElementPropertySectionsAction->setText(tr("Remove"));

    zv_addChemicalElementPropertyAction = new QAction(this);
    zv_addChemicalElementPropertyAction->setText(tr("Add"));

    zv_removeChemicalElementPropertiesAction = new QAction(this);
    zv_removeChemicalElementPropertiesAction->setText(tr("Remove"));

    zv_copyChemicalElementPropertiesAction = new QAction(this);
    zv_copyChemicalElementPropertiesAction->setText(tr("Copy"));

    zv_pasteChemicalElementPropertiesAction = new QAction(this);
    zv_pasteChemicalElementPropertiesAction->setText(tr("Paste"));

    zv_expandAllAction = new QAction(this);
    zv_expandAllAction->setText(tr("Expand all"));

    zv_collapseAllAction = new QAction(this);
    zv_collapseAllAction->setText(tr("Collapse all"));


}
//======================================================
void ZChemicalElementProprtyEditor::zh_createComponents()
{
    zv_clipboard = QGuiApplication::clipboard();
    zv_propertyFilterProxyModel = new ZChemicalPropertyProxyTableModel(this);

    QHBoxLayout* fileNameLabelLayout = new QHBoxLayout;
    QLabel* label = new QLabel(this);
    QString caption = tr("Source file:");
    label->setText(ZCaptionFormatter::zp_format(caption));
    fileNameLabelLayout->addWidget(label);
    zv_sourceFileNameLabel = new QLabel(this);
    fileNameLabelLayout->addWidget(zv_sourceFileNameLabel, zv_maxStretchValue, Qt::AlignVCenter | Qt::AlignLeft);
    zh_addLayoutToMainLayout(fileNameLabelLayout, 0);

    zv_mainSplitter = new QSplitter(Qt::Horizontal, this);
    zh_addWidgetToMainLayout(zv_mainSplitter, zv_maxStretchValue);
    zv_mainSplitter->addWidget(zh_createPropertySectionTreeWidget());
    zv_mainSplitter->addWidget(zh_createPropertyTableWidget());

    zv_loadButton = zh_createBasementButton(tr("Load"),
                                            tr("Load chemical element properties from file"),
                                            QIcon());
    zv_saveButton = zh_createBasementButton(tr("Save"),
                                            tr("Save chemical element properties to file"),
                                            QIcon());
    zv_closeButton = zh_createBasementButton(tr("Close"),
                                             tr("Close editor"),
                                             QIcon());
}
//======================================================
void ZChemicalElementProprtyEditor::zh_createMenu()
{
    zv_propertySectionTreeView->addAction(zv_addChemicalElementPropertySectionsAction);
    zv_propertySectionTreeView->addAction(zv_removeChemicalElementPropertySectionsAction);
    zv_propertySectionTreeView->addAction(zv_expandAllAction);
    zv_propertySectionTreeView->addAction(zv_collapseAllAction);
    zv_propertySectionTreeView->setContextMenuPolicy(Qt::ActionsContextMenu);

    zv_propertyTableView->addAction(zv_addChemicalElementPropertyAction);
    zv_propertyTableView->addAction(zv_removeChemicalElementPropertiesAction);
    zv_propertyTableView->addAction(zv_copyChemicalElementPropertiesAction);
    zv_propertyTableView->addAction(zv_pasteChemicalElementPropertiesAction);
    zv_propertyTableView->setContextMenuPolicy(Qt::ActionsContextMenu);

}
//======================================================
void ZChemicalElementProprtyEditor::zh_createConnections()
{
    zv_propertyTableView->setModel(zv_propertyFilterProxyModel);

    connect(zv_addChemicalElementPropertySectionButton, &QPushButton::clicked,
            zv_addChemicalElementPropertySectionsAction, &QAction::trigger);
    connect(zv_addChemicalElementPropertySectionsAction, &QAction::triggered,
            this, &ZChemicalElementProprtyEditor::zh_addNewChemicalElementPropertySection);

    connect(zv_removeChemicalElementPropertySectionButton, &QPushButton::clicked,
            zv_removeChemicalElementPropertySectionsAction, &QAction::trigger);
    connect(zv_removeChemicalElementPropertySectionsAction, &QAction::triggered,
            this, &ZChemicalElementProprtyEditor::zh_removeSelectedChemicalElementPropertySections);

    connect(zv_addChemicalElementPropertyButton, &QPushButton::clicked,
            zv_addChemicalElementPropertyAction, &QAction::trigger);
    connect(zv_addChemicalElementPropertyAction, &QAction::triggered,
            this, &ZChemicalElementProprtyEditor::zh_addNewChemicalElementProperty);

    connect(zv_removeChemicalElementPropertiesButton, &QPushButton::clicked,
            zv_removeChemicalElementPropertiesAction, &QAction::trigger);
    connect(zv_removeChemicalElementPropertiesAction, &QAction::triggered,
            this, &ZChemicalElementProprtyEditor::zh_removeSelectedChemicalElementProperties);

    connect(zv_copyChemicalElementPropertiesAction, &QAction::triggered,
            this, &ZChemicalElementProprtyEditor::zh_copyPropertyToClipboard);
    connect(zv_pasteChemicalElementPropertiesAction, &QAction::triggered,
            this, &ZChemicalElementProprtyEditor::zh_pastePropertyFromClipboard);

    connect(zv_expandAllAction, &QAction::triggered,
            zv_propertySectionTreeView, &QTreeView::expandAll);
    connect(zv_collapseAllAction, &QAction::triggered,
            zv_propertySectionTreeView, &QTreeView::collapseAll);

    connect(zv_clipboard, &QClipboard::dataChanged,
            this, &ZChemicalElementProprtyEditor::zh_onClipboardDataChange);

    connect(zv_loadButton, &QPushButton::clicked,
            this, &ZChemicalElementProprtyEditor::zh_onLoadButtonClick);
    connect(zv_saveButton, &QPushButton::clicked,
            this, &ZChemicalElementProprtyEditor::zh_onSaveButtonClick);
    connect(zv_closeButton, &QPushButton::clicked,
            this, &ZChemicalElementProprtyEditor::zh_onCloseButtonClick);
}
//======================================================
QWidget* ZChemicalElementProprtyEditor::zh_createPropertySectionTreeWidget()
{
    QWidget* sectionTreeWidget = new QWidget(this);
    QVBoxLayout* mainLayout = new QVBoxLayout;
    sectionTreeWidget->setLayout(mainLayout);

    QLabel* label = new QLabel(sectionTreeWidget);
    QString caption = tr("Property sections:");
    label->setText(ZCaptionFormatter::zp_format(caption));
    mainLayout->addWidget(label);

    zv_propertySectionTreeView = new QTreeView;
    zv_propertySectionTreeView->installEventFilter(this);
    mainLayout->addWidget(zv_propertySectionTreeView);

    // basement
    QDialogButtonBox* buttonBox = new QDialogButtonBox(sectionTreeWidget);
    mainLayout->addWidget(buttonBox);

    zv_addChemicalElementPropertySectionButton = new QPushButton(sectionTreeWidget);
    zv_addChemicalElementPropertySectionButton->setFocusPolicy(Qt::NoFocus);
    zv_addChemicalElementPropertySectionButton->setText(zv_addChemicalElementPropertySectionsAction->text());
    zv_addChemicalElementPropertySectionButton->setIcon(zv_addChemicalElementPropertySectionsAction->icon());
    zv_addChemicalElementPropertySectionButton->setToolTip(zv_addChemicalElementPropertySectionsAction->toolTip());

    buttonBox->addButton(zv_addChemicalElementPropertySectionButton, QDialogButtonBox::ActionRole);

    zv_removeChemicalElementPropertySectionButton = new QPushButton(sectionTreeWidget);
    zv_removeChemicalElementPropertySectionButton->setFocusPolicy(Qt::NoFocus);
    zv_removeChemicalElementPropertySectionButton->setText(zv_removeChemicalElementPropertySectionsAction->text());
    zv_removeChemicalElementPropertySectionButton->setIcon(zv_removeChemicalElementPropertySectionsAction->icon());
    zv_removeChemicalElementPropertySectionButton->setToolTip(zv_removeChemicalElementPropertySectionsAction->toolTip());
    buttonBox->addButton(zv_removeChemicalElementPropertySectionButton, QDialogButtonBox::ActionRole);

    return sectionTreeWidget;
}
//======================================================
QWidget* ZChemicalElementProprtyEditor::zh_createPropertyTableWidget()
{
    QWidget* propertyTableWidget = new QWidget(this);
    QVBoxLayout* mainLayout = new QVBoxLayout;
    propertyTableWidget->setLayout(mainLayout);

    QLabel* label = new QLabel(propertyTableWidget);
    QString caption = tr("Property:");
    label->setText(ZCaptionFormatter::zp_format(caption));
    mainLayout->addWidget(label);

    zv_propertyTableView = new QTableView;
    zv_propertyTableView->setSelectionBehavior(QAbstractItemView::SelectRows);
    zv_propertyTableView->installEventFilter(this);
    mainLayout->addWidget(zv_propertyTableView);


    // basement
    QDialogButtonBox* buttonBox = new QDialogButtonBox(propertyTableWidget);
    mainLayout->addWidget(buttonBox);

    zv_addChemicalElementPropertyButton = new QPushButton(propertyTableWidget);
    zv_addChemicalElementPropertyButton->setFocusPolicy(Qt::NoFocus);
    zv_addChemicalElementPropertyButton->setText(zv_addChemicalElementPropertySectionsAction->text());
    zv_addChemicalElementPropertyButton->setIcon(zv_addChemicalElementPropertySectionsAction->icon());
    zv_addChemicalElementPropertyButton->setToolTip(zv_addChemicalElementPropertySectionsAction->toolTip());

    buttonBox->addButton(zv_addChemicalElementPropertyButton, QDialogButtonBox::ActionRole);

    zv_removeChemicalElementPropertiesButton = new QPushButton(propertyTableWidget);
    zv_removeChemicalElementPropertiesButton->setFocusPolicy(Qt::NoFocus);
    zv_removeChemicalElementPropertiesButton->setText(zv_removeChemicalElementPropertySectionsAction->text());
    zv_removeChemicalElementPropertiesButton->setIcon(zv_removeChemicalElementPropertySectionsAction->icon());
    zv_removeChemicalElementPropertiesButton->setToolTip(zv_removeChemicalElementPropertySectionsAction->toolTip());
    buttonBox->addButton(zv_removeChemicalElementPropertiesButton, QDialogButtonBox::ActionRole);

    return propertyTableWidget;
}
//======================================================
void ZChemicalElementProprtyEditor::zh_saveSettings()
{
    QSettings settings;
    if(!zh_openDialogSettingsGroup(&settings))
    {
        return;
    }

    settings.setValue(zv_splitterStateName, zv_mainSplitter->saveState());

    // close dialog group
    settings.endGroup();

    zh_closeDialogSettingsGroup(&settings);
}
//======================================================
void ZChemicalElementProprtyEditor::zh_restoreSettings()
{
    QSettings settings;
    if(!zh_openDialogSettingsGroup(&settings))
    {
        return;
    }

    if(settings.value(zv_splitterStateName).canConvert<QByteArray>())
    {
        zv_mainSplitter->restoreState(settings.value(zv_splitterStateName).toByteArray());
    }

    zh_closeDialogSettingsGroup(&settings);
}
//======================================================
bool ZChemicalElementProprtyEditor::eventFilter(QObject *watched, QEvent *event)
{
    if(event->type() == QEvent::KeyRelease)
    {
        QKeyEvent* keyEvent = static_cast<QKeyEvent*>(event);
        if(!keyEvent)
        {
            return false;
        }

        if(watched == zv_propertySectionTreeView)
        {
            if(keyEvent->key() == Qt::Key_Delete
                    && zv_removeChemicalElementPropertySectionsAction->isEnabled())
            {
                zv_removeChemicalElementPropertySectionsAction->trigger();
            }
            else if(keyEvent->key() == Qt::Key_Insert)
            {
                zv_addChemicalElementPropertySectionsAction->trigger();
            }
        }
        else if(watched == zv_propertyTableView)
        {
            if(keyEvent->key() == Qt::Key_Delete
                    && zv_removeChemicalElementPropertiesAction->isEnabled())
            {
                zv_removeChemicalElementPropertiesAction->trigger();
            }
            else if(keyEvent->key() == Qt::Key_Insert)
            {
                zv_addChemicalElementPropertyAction->trigger();
            }
        }
    }

    return false;
}
//======================================================
void ZChemicalElementProprtyEditor::closeEvent(QCloseEvent* event)
{
    ZChemicalElementPropertyTreeModel* model =
            qobject_cast<ZChemicalElementPropertyTreeModel*>(zv_propertySectionTreeView->model());
    if(!model)
    {
        return;
    }

    if(model->zp_isDirty())
    {
        QMessageBox msgBox;
        msgBox.setText(tr("Chemical element properties were changed."));
        msgBox.setInformativeText(tr("Do you want to save them to file?"));
        msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
        msgBox.setDefaultButton(QMessageBox::Yes);
        int answer = msgBox.exec();

        if(answer == QMessageBox::Yes)
        {
            zh_savePropertyDataToFile();
        }
    }
}
//======================================================
void ZChemicalElementProprtyEditor::zp_setModel(QAbstractItemModel* model)
{
    // set up tree view
    zv_propertySectionTreeView->setModel(model);
    // zv_propertySectionTreeView->header()->setVisible(false);
    zv_propertySectionTreeView->setSelectionMode(QAbstractItemView::ExtendedSelection);
    zv_propertySectionTreeView->setSelectionBehavior(QAbstractItemView::SelectItems);


    // connect signal current tree element changed
    // in order to show chemical properties approprietary to current element
    connect(zv_propertySectionTreeView->selectionModel(), &QItemSelectionModel::currentChanged,
            zv_propertyFilterProxyModel, &ZChemicalPropertyProxyTableModel::zp_onCurrentTreeElementChanged);
    connect(model, &QAbstractItemModel::dataChanged,
            zv_propertyFilterProxyModel, &ZChemicalPropertyProxyTableModel::zp_onSourceModelDataChanged);

    connect(zv_propertySectionTreeView->selectionModel(), &QItemSelectionModel::selectionChanged,
            this, &ZChemicalElementProprtyEditor::zh_onTreeElementSelectionChange);

    connect(zv_propertyTableView->selectionModel(), &QItemSelectionModel::selectionChanged,
            this, &ZChemicalElementProprtyEditor::zh_onPropertyTableElementSelectionChange);

    connect(model, &QAbstractItemModel::columnsInserted,
            this, &ZChemicalElementProprtyEditor::zh_onModelColumnsInserted);

    ZChemicalElementPropertyTreeModel* treeModel = qobject_cast<ZChemicalElementPropertyTreeModel*>(model);
    if(treeModel)
    {
        connect(treeModel, &ZChemicalElementPropertyTreeModel::zg_propertiesSourceFilePathChanged,
                this, &ZChemicalElementProprtyEditor::zh_setPropertiesSourceFilePath);
        zh_setPropertiesSourceFilePath(treeModel->zp_propertiesSourceFilePath());
    }

    zh_treeColumnVisibilityControl();
}
//======================================================
void ZChemicalElementProprtyEditor::zh_addNewChemicalElementPropertySection()
{
    QStringList sectionBranch;
    zh_currentChemicalElementSectionBrunch(sectionBranch);

    // set first section chemical element symbol
    if(!sectionBranch.isEmpty())
    {
        sectionBranch.removeFirst();
    }

    int currentZNumber;
    if(zh_currentZNumber(currentZNumber))
    {
        QAbstractItemModel* model =
                qobject_cast<QAbstractItemModel*>(zv_propertySectionTreeView->model());
        if(model)
        {
            QModelIndex index = model->index(currentZNumber - 1, 0, QModelIndex());
            if(index.isValid())
            {
                QVariant vData = index.data(Qt::DisplayRole);
                if(vData.isValid() && vData.canConvert<QString>())
                {
                    sectionBranch.push_front(vData.toString());
                }
            }
        }
    }
    //

    bool currentVisible = zv_propertySectionTreeView->currentIndex().isValid();
    bool selectedVisible = !zv_propertySectionTreeView->selectionModel()->selectedIndexes().isEmpty();

    ZPropertySectionEditDialog dialog(sectionBranch, currentVisible, selectedVisible);
    if(!dialog.exec())
    {
        return;
    }

    QList<int>zNumberList;
    if(dialog.zp_additionTarget() == ZPropertySectionEditDialog::AT_ALL)
    {
        for(int z = 1; z <= 118; z++)
        {
            zNumberList.append(z);
        }
    }
    else if(dialog.zp_additionTarget() == ZPropertySectionEditDialog::AT_SELECTED)
    {
        if(!zh_selectedZNumberList(zNumberList))
        {
            return;
        }
    }
    else if(dialog.zp_additionTarget() == ZPropertySectionEditDialog::AT_CURRENT)
    {
        int currentZNumber;
        if(!zh_currentZNumber(currentZNumber))
        {
            return;
        }
        zNumberList.append(currentZNumber);
    }
    else
    {
        return;
    }

    sectionBranch = dialog.zp_newSectionBranch();
    // remove first chemical element section
    if(!sectionBranch.isEmpty())
    {
        sectionBranch.removeFirst();
    }
    zh_addChemicalElementPropertySectionBranch(zNumberList, sectionBranch);
}
//======================================================
void ZChemicalElementProprtyEditor::zh_removeSelectedChemicalElementPropertySections()
{
    ZChemicalElementPropertyTreeModel* model =
            qobject_cast<ZChemicalElementPropertyTreeModel*>(zv_propertySectionTreeView->model());

    if(!model)
    {
        return;
    }

    // get selected indexes
    QModelIndexList selectedIndexList = zv_propertySectionTreeView->selectionModel()->selectedIndexes();

    // let model remove remained indexes
    model->zp_removeItems(selectedIndexList);
}
//======================================================
void ZChemicalElementProprtyEditor::zh_addNewChemicalElementProperty()
{
    int currentZNumber;
    if(!zh_currentZNumber(currentZNumber))
    {
        QString msg = tr("Select chemical element or property section!");
        QMessageBox::critical(this, tr("Error"), msg, QMessageBox::Ok);
        return;
    }

    ZPropertyEditDialog dialog;
    if(!dialog.exec())
    {
        return;
    }

    PropertyList propertyList;
    propertyList.append(dialog.zp_property());
    zh_insertChemicalElementPropertiesToCurrentBranch(propertyList);
}
//======================================================
bool ZChemicalElementProprtyEditor::zh_insertChemicalElementPropertiesToCurrentBranch(const PropertyList& propertyList)
{
    ZChemicalElementPropertyTreeModel* model =
            qobject_cast<ZChemicalElementPropertyTreeModel*>(zv_propertySectionTreeView->model());

    if(!model)
    {
        QString msg = tr("Cannot access to chemical element property model.");
        QMessageBox::critical(this, tr("Error"), msg, QMessageBox::Ok);
        return false;
    }

    int currentZNumber;
    if(!zh_currentZNumber(currentZNumber))
    {
        QString msg = tr("Select chemical element or property section!");
        QMessageBox::critical(this, tr("Error"), msg, QMessageBox::Ok);
        return false;
    }

    QStringList sectionBranch;
    zh_currentChemicalElementSectionBrunch(sectionBranch);
    // remove first chemical element section
    if(!sectionBranch.isEmpty())
    {
        sectionBranch.removeFirst();
    }

    QString errorMsg;
    if(!model->zp_setChemicalElementProperties(currentZNumber, sectionBranch, propertyList))
    {
        QMessageBox::critical(this, tr("Error"), model->zp_lastError(), QMessageBox::Ok);
        return false;
    }

    return true;
}
//======================================================
void ZChemicalElementProprtyEditor::zh_removeSelectedChemicalElementProperties()
{
    ZChemicalElementPropertyTreeModel* model =
            qobject_cast<ZChemicalElementPropertyTreeModel*>(zv_propertySectionTreeView->model());
    if(!model)
    {
        return;
    }

    // get selected properties
    QModelIndexList selectedIndexList = zv_propertyTableView->selectionModel()->selectedIndexes();
    if(selectedIndexList.isEmpty())
    {
        return;
    }

    QList<int> rowList;
    foreach(QModelIndex index, selectedIndexList)
    {
        if(!rowList.contains(index.row()))
        {
            rowList.append(index.row());
        }
    }

    std::sort(rowList.begin(), rowList.end());

    int zNumber;
    if(!zh_currentZNumber(zNumber))
    {
        return;
    }

    QStringList branch;
    zh_currentChemicalElementSectionBrunch(branch);
    if(!branch.isEmpty())
    {
        branch.removeFirst();
    }

    for(int i = rowList.count() - 1; i >= 0;  i--)
    {
        model->zp_removeChemicalElementProperty(zNumber, branch, rowList.at(i));
    }

}
//======================================================
void ZChemicalElementProprtyEditor::zh_copyPropertyToClipboard()
{
    // TODO Copy Selected Properties
}
//======================================================
void ZChemicalElementProprtyEditor::zh_pastePropertyFromClipboard()
{
    if(!zv_clipboard->mimeData()->hasText()
            || zv_clipboard->text().isEmpty())
    {
        return;
    }

    // devide text to rows (separator - \n)
    QRegExp rowSplitterRegExp("\n");
    QString txt = zv_clipboard->text();
    QStringList rowList = zv_clipboard->text().split(rowSplitterRegExp, QString::SkipEmptyParts);

    // devide every row to columns (separator - \t or ';')
    QRegExp columnSplitterRegExp("[\t;]");
    QList<QStringList> slittedRowList;
    QStringList currentRow;
    int columnCount = 0;
    for(int i = 0; i < rowList.count(); i++)
    {
        currentRow = rowList.at(i).split(columnSplitterRegExp);
        slittedRowList.append(currentRow);
        if(columnCount < currentRow.count())
        {
            columnCount = currentRow.count();
        }
    }

    if(columnCount <= 0)
    {
        return;
    }
    else if(columnCount == 1)
    {
        // insert in selection only

    }
    else // columnCount > 1
    {
        // selections ignored
        PropertyList propertyList;

        for(int row = 0; row < slittedRowList.count(); row++)
        {
            propertyList.append(QPair<QString, QString>(slittedRowList.at(row).value(0), slittedRowList.at(row).value(1)));
        }

        zh_insertChemicalElementPropertiesToCurrentBranch(propertyList);
    }
}
//======================================================
void ZChemicalElementProprtyEditor::zh_onTreeElementSelectionChange(const QItemSelection& selected,
                                                                    const QItemSelection& deselected)
{
    zh_adjustRemoveSectionAbility(selected);
}
//======================================================
void ZChemicalElementProprtyEditor::zh_onPropertyTableElementSelectionChange(const QItemSelection& selected,
                                                                             const QItemSelection& deselected)
{
    zh_adjustRemoveAndCopyPropertyAbility(selected);
}
//======================================================
void ZChemicalElementProprtyEditor::zh_setPropertiesSourceFilePath(const QString& fileName)
{
    zv_sourceFileNameLabel->setText(fileName);
}
//======================================================
void ZChemicalElementProprtyEditor::zh_onClipboardDataChange()
{
    zh_adjustPastePropertyAbility();
}
//======================================================
void ZChemicalElementProprtyEditor::zh_onModelColumnsInserted(const QModelIndex& parent, int first, int last)
{
    zh_treeColumnVisibilityControl();
}
//======================================================
void ZChemicalElementProprtyEditor::zh_onLoadButtonClick()
{
    ZChemicalElementPropertyTreeModel* model =
            qobject_cast<ZChemicalElementPropertyTreeModel*>(zv_propertySectionTreeView->model());
    if(!model)
    {
        QString msg = tr("There is no chemical elment model for load data.");
        QMessageBox::critical(this, tr("Error"), msg, QMessageBox::Ok);
        return;
    }

    if(model->zp_isDirty())
    {
        QMessageBox msgBox;
        msgBox.setText(tr("Chemical element properties were changed."));
        msgBox.setInformativeText(tr("Do you want to save them to file?"));
        msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
        msgBox.setDefaultButton(QMessageBox::Yes);
        int answer = msgBox.exec();

        if(answer == QMessageBox::Yes)
        {
            zh_savePropertyDataToFile();
        }
    }

    QString dataFileFolder = zh_dataFileFolder();
    QString fileDataPath = QFileDialog::getOpenFileName(this, tr("Open data file"), dataFileFolder, tr("XML Files (*.xml);;All Files (*.*)"));
    if(fileDataPath.isEmpty())
    {
        return;
    }

    if(!model->zp_loadDataFromFile(fileDataPath))
    {
        QMessageBox::critical(this, tr("Error"), model->zp_lastError(), QMessageBox::Ok);
    }

}
//======================================================
void ZChemicalElementProprtyEditor::zh_onCloseButtonClick()
{
    ZChemicalElementPropertyTreeModel* model =
            qobject_cast<ZChemicalElementPropertyTreeModel*>(zv_propertySectionTreeView->model());
    if(!model)
    {
        reject();
        return;
    }

    if(model->zp_isDirty())
    {
        QMessageBox msgBox;
        msgBox.setText(tr("Chemical element properties were changed."));
        msgBox.setInformativeText(tr("Do you want to save them to file?"));
        msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
        msgBox.setDefaultButton(QMessageBox::Yes);
        int answer = msgBox.exec();

        if(answer == QMessageBox::Yes)
        {
            zh_savePropertyDataToFile();
        }
    }

    accept();
}
//======================================================
void ZChemicalElementProprtyEditor::zh_treeColumnVisibilityControl()
{
    QAbstractItemModel* model = zv_propertySectionTreeView->model();
    if(!model)
    {
        return;
    }

    for(int column = 1; column < model->columnCount(); column++)
    {
        zv_propertySectionTreeView->setColumnHidden(column, true);
    }

    zv_propertySectionTreeView->header()->setSectionResizeMode(0, QHeaderView::Stretch);
}
//======================================================
bool ZChemicalElementProprtyEditor::zh_addChemicalElementPropertySectionBranch(const QList<int>& zNumberList,
                                                                               const QStringList& sectionBranch)
{
    if(sectionBranch.isEmpty())
    {
        return false;
    }

    ZChemicalElementPropertyTreeModel* model =
            qobject_cast<ZChemicalElementPropertyTreeModel*>(zv_propertySectionTreeView->model());

    if(!model)
    {
        return false;
    }

    QString errorMsg;
    bool continueFlag = false;
    for(int i = 0; i < zNumberList.count(); i++)
    {
        if(!model->zp_addChemicalElementPropertyBranch(zNumberList.at(i),
                                                       sectionBranch) && !continueFlag)
        {
            // get chem element name
            int row = zNumberList.at(i) - 1;
            QModelIndex index = model->index(row, 0, QModelIndex());
            QVariant vData = index.data(Qt::DisplayRole);

            QString chemicalElement;
            if(!vData.isValid() || !vData.canConvert<QString>())
            {
                chemicalElement = QString::number(zNumberList.at(i));
            }
            else
            {
                chemicalElement = vData.toString();
            }

            QMessageBox msgBox;
            msgBox.setText(tr("Cannot add chemical element property branch to chemical element %1 with error \"%2\".")
                           .arg(chemicalElement, errorMsg));
            msgBox.setInformativeText("Do you want to continue anyway?");
            msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
            msgBox.setDefaultButton(QMessageBox::Yes);
            int answer = msgBox.exec();

            switch(answer)
            {
            case QMessageBox::Yes:
                continueFlag = true;
                continue;
            case QMessageBox::No:
                return false;
            }
        }
    }

    zh_adjustRemoveSectionAbility(zv_propertySectionTreeView->selectionModel()->selection());
    return true;
}
//======================================================
bool ZChemicalElementProprtyEditor::zh_currentChemicalElementSectionBrunch(QStringList& branch) const
{
    ZChemicalElementPropertyTreeModel* model =
            qobject_cast<ZChemicalElementPropertyTreeModel*>(zv_propertySectionTreeView->model());

    if(!model)
    {
        return false;
    }

    return model->zp_chemicalElementPropertySectionBranch(zv_propertySectionTreeView->currentIndex(), branch);
}
//======================================================
bool ZChemicalElementProprtyEditor::zh_selectedZNumberList(QList<int>& zNumberList) const
{
    ZChemicalElementPropertyTreeModel* model =
            qobject_cast<ZChemicalElementPropertyTreeModel*>(zv_propertySectionTreeView->model());

    if(!model)
    {
        return false;
    }

    // get selected indexes
    QModelIndexList selectedIndexList = zv_propertySectionTreeView->selectionModel()->selectedIndexes();
    // get zNumbers

    int zNumber;
    foreach(QModelIndex index, selectedIndexList)
    {
        if(!model->zp_zNumberForIndex(index, zNumber))
        {
            continue;
        }

        if(!zNumberList.contains(zNumber))
        {
            zNumberList.append(zNumber);
        }
    }

    std::sort(zNumberList.begin(), zNumberList.end());

    return !zNumberList.isEmpty();
}
//======================================================
bool ZChemicalElementProprtyEditor::zh_currentZNumber(int& currentZNumber) const
{
    ZChemicalElementPropertyTreeModel* model =
            qobject_cast<ZChemicalElementPropertyTreeModel*>(zv_propertySectionTreeView->model());

    if(!model)
    {
        return false;
    }

    return model->zp_zNumberForIndex(zv_propertySectionTreeView->currentIndex(), currentZNumber);
}
//======================================================
void ZChemicalElementProprtyEditor::zh_adjustRemoveSectionAbility(const QItemSelection& selected)
{
    ZChemicalElementPropertyTreeModel* model =
            qobject_cast<ZChemicalElementPropertyTreeModel*>(zv_propertySectionTreeView->model());

    if(!model || selected.isEmpty())
    {
        zv_removeChemicalElementPropertySectionButton->setEnabled(false);
        zv_removeChemicalElementPropertySectionsAction->setEnabled(false);
        return;
    }

    QModelIndexList selectedIndexList = selected.indexes();
    foreach(QModelIndex index, selectedIndexList)
    {
        if(index.column() == 0 &&  (model->hasChildren(index) || model->zp_isRemovable(index)))
        {
            zv_removeChemicalElementPropertySectionButton->setEnabled(true);
            zv_removeChemicalElementPropertySectionsAction->setEnabled(true);

            return;
        }
    }

    zv_removeChemicalElementPropertySectionButton->setEnabled(false);
    zv_removeChemicalElementPropertySectionsAction->setEnabled(false);
}
//======================================================
void ZChemicalElementProprtyEditor::zh_adjustRemoveAndCopyPropertyAbility(const QItemSelection& selected)
{
    ZChemicalElementPropertyTreeModel* model =
            qobject_cast<ZChemicalElementPropertyTreeModel*>(zv_propertySectionTreeView->model());

    if(!model || selected.isEmpty())
    {
        zv_removeChemicalElementPropertiesButton->setEnabled(false);
        zv_removeChemicalElementPropertiesAction->setEnabled(false);
        zv_copyChemicalElementPropertiesAction->setEnabled(false);
        return;
    }

    bool enabled = !selected.indexes().isEmpty();
    zv_removeChemicalElementPropertiesButton->setEnabled(enabled);
    zv_removeChemicalElementPropertiesAction->setEnabled(enabled);
    zv_copyChemicalElementPropertiesAction->setEnabled(enabled);

}
//======================================================
void ZChemicalElementProprtyEditor::zh_adjustPastePropertyAbility()
{
    zv_pasteChemicalElementPropertiesAction->setEnabled(zv_clipboard->mimeData()->hasText()
                                                        && !zv_clipboard->text().isEmpty());
}
//======================================================
void ZChemicalElementProprtyEditor::zh_onSaveButtonClick()
{
    zh_savePropertyDataToFile();
}
//======================================================
void ZChemicalElementProprtyEditor::zh_savePropertyDataToFile()
{
    ZChemicalElementPropertyTreeModel* model =
            qobject_cast<ZChemicalElementPropertyTreeModel*>(zv_propertySectionTreeView->model());

    if(!model)
    {
        QString msg = tr("There is no property model!");
        QMessageBox::critical(this, tr("Error"), msg, QMessageBox::Ok);
        return;
    }

    // get the file
    QString dataFileFolder = zh_dataFileFolder();
    QString fileDataPath = QFileDialog::getSaveFileName(this, tr("Save data to file"), dataFileFolder, tr("XML Files (*.xml);;All Files (*.*)"));
    if(fileDataPath.isEmpty())
    {
        return;
    }

    QString errorMsg;
    if(!model->zp_saveDataToXMLFile(fileDataPath))
    {
        QMessageBox::critical(this, tr("Error"), model->zp_lastError(), QMessageBox::Ok);
        return;
    }

    QString msg = tr("File \"%1\" have been saved successfully.").arg(fileDataPath);

    QMessageBox msgBox;
    msgBox.setWindowTitle(qApp->property("glAppProduct").toString());
    msgBox.setText(msg);
    msgBox.setInformativeText(tr("Do you want to set the file as default chemical element properties source or reset source file to default?"));
    msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No | QMessageBox::Reset);
    msgBox.setDefaultButton(QMessageBox::Yes);
    int answer = msgBox.exec();

    switch(answer)
    {
    case QMessageBox::No:
        return;
    case QMessageBox::Yes:
        model->zp_setPropertiesSourceFileName(fileDataPath);
        break;
    case QMessageBox::Reset:
        model->zp_setPropertiesSourceFileName(QString());
        break;
    }

}
//======================================================
QString ZChemicalElementProprtyEditor::zh_dataFileFolder() const
{
    ZChemicalElementPropertyTreeModel* model =
            qobject_cast<ZChemicalElementPropertyTreeModel*>(zv_propertySectionTreeView->model());

    if(!model)
    {
        return zh_standardLocation();
    }

    QString currentPropertiesSourceFileName = model->zp_propertiesSourceFilePath();
    QFileInfo fileInfo(currentPropertiesSourceFileName);

    if(fileInfo.absolutePath().startsWith(":"))
    {
        return zh_standardLocation();
    }

    return fileInfo.absolutePath();
}
//======================================================
QString ZChemicalElementProprtyEditor::zh_standardLocation() const
{
    if(!QStandardPaths::standardLocations(QStandardPaths::DocumentsLocation).isEmpty())
    {
        return QStandardPaths::standardLocations(QStandardPaths::DocumentsLocation).first();
    }

    return QDir::currentPath();
}
//======================================================

