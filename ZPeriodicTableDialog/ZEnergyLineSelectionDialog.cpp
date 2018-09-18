//=============================================================
#include "ZEnergyLineSelectionDialog.h"
#include "ZGeneral.h"
#include "ZPeriodicTableWidget.h"
#include "ZChemicalElementPropertyTreeModel.h"
#include "ZChemicalPropertyProxyTableModel.h"

#include <QApplication>
#include <QDialogButtonBox>
#include <QFrame>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QSettings>
#include <QSplitter>
#include <QTableView>
#include <QVBoxLayout>

//=============================================================
ZEnergyLineSelectionDialog::ZEnergyLineSelectionDialog(QWidget *parent) : QDialog(parent)
{
    zv_chemicalElementSymbol = QString();
    zv_energyLineName = QString();
    zv_energyLineEnergyValue = 0.0;

    zh_createComponents();
    zh_createConnections();
    zh_restoreSettings();
}
//=============================================================
ZEnergyLineSelectionDialog::~ZEnergyLineSelectionDialog()
{
    zh_saveSettings();
}
//=============================================================
void ZEnergyLineSelectionDialog::zh_createComponents()
{
    // models
    zv_chemicalElementPropertyTreeModel = new ZChemicalElementPropertyTreeModel(this);
    zv_chemicalElementPropertyTreeModel->zp_setNamePropertyName("Name ru");

    zv_chemicalPropertyProxyTableModel = new ZChemicalPropertyProxyTableModel(this);
    zv_chemicalPropertyProxyTableModel->zp_setItemIsEditable(false);
    zv_chemicalPropertyProxyTableModel->setHeaderData(0, Qt::Horizontal, tr("Energy line"), Qt::EditRole);
    zv_chemicalPropertyProxyTableModel->setHeaderData(1, Qt::Horizontal, tr("Energy value"), Qt::EditRole);

    // main layout
    QVBoxLayout* mainLayout = new QVBoxLayout;
    setLayout(mainLayout);

    // slitter
    zv_mainSplitter = new QSplitter(this);
    mainLayout->addWidget(zv_mainSplitter);

    // periodic table
    zv_periodicTableWidget = new ZPeriodicTableWidget(this);
    zv_periodicTableWidget->zp_setSelectionMode(ZPeriodicTableWidget::SM_SINGLE_SELECTION);
    zv_mainSplitter->addWidget(zv_periodicTableWidget);

    // property table view
    zv_chemicalElementPropertyTableView = new QTableView(this);
    zv_chemicalElementPropertyTableView->setModel(zv_chemicalPropertyProxyTableModel);
    zv_chemicalElementPropertyTableView->setSelectionBehavior(QAbstractItemView::SelectRows);
    zv_chemicalElementPropertyTableView->setSelectionMode( QAbstractItemView::SingleSelection);
    zv_mainSplitter->addWidget(zv_chemicalElementPropertyTableView);


    QHBoxLayout* selectedLineLayout = new QHBoxLayout;
    mainLayout->addLayout(selectedLineLayout);

    QLabel* label = new QLabel(this);
    QString text = tr("Selected line:");
    label->setText(text);
    selectedLineLayout->addWidget(label);

    zv_selectedEnergyLineLineEdit = new QLineEdit(this);
    zv_selectedEnergyLineLineEdit->setReadOnly(true);
    selectedLineLayout->addWidget(zv_selectedEnergyLineLineEdit);

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
//=============================================================
void ZEnergyLineSelectionDialog::zh_createConnections()
{
    zv_periodicTableWidget->zp_setModel(zv_chemicalElementPropertyTreeModel);

    connect(zv_periodicTableWidget, &ZPeriodicTableWidget::zg_selectedChemicalElementChanged,
            this, &ZEnergyLineSelectionDialog::zh_onSelectedChemicalElementChange);

    connect(zv_okButton, &QPushButton::clicked,
            this, &ZEnergyLineSelectionDialog::zh_onOkClick);
    connect(zv_cancelButton, &QPushButton::clicked,
            this, &ZEnergyLineSelectionDialog::reject);

    connect(zv_chemicalElementPropertyTableView->selectionModel(), &QItemSelectionModel::currentChanged,
            this, &ZEnergyLineSelectionDialog::zh_onCurrentEnergyLineChange);

}
//=============================================================
double ZEnergyLineSelectionDialog::zp_energyValue() const
{
    return zv_energyLineEnergyValue;
}
//=============================================================
QString ZEnergyLineSelectionDialog::zp_chemicalElementName() const
{
    return zv_chemicalElementSymbol;
}
//=============================================================
QString ZEnergyLineSelectionDialog::zp_energyLineName() const
{
    return zv_energyLineName;
}
//=============================================================
void ZEnergyLineSelectionDialog::zh_saveSettings()
{
    QSettings settings;
    settings.beginGroup(glAppVersion);
    settings.beginGroup(zv_settingsGroupName);
    settings.setValue(zv_geometrySectionName, QVariant(saveGeometry()));
    if(zv_mainSplitter)
    {
        settings.setValue(zv_splitterStateSectionName, QVariant(zv_mainSplitter->saveState()));
    }
    settings.endGroup();
    settings.endGroup();
}
//=============================================================
void ZEnergyLineSelectionDialog::zh_restoreSettings()
{
    QSettings settings;
    settings.beginGroup(glAppVersion);
    settings.beginGroup(zv_settingsGroupName);

    QVariant vData = settings.value(zv_geometrySectionName);
    if(vData.isValid() && !vData.isNull() && vData.canConvert<QByteArray>())
    {
        restoreGeometry(vData.toByteArray());
    }

    if(zv_mainSplitter)
    {
        vData = settings.value(zv_splitterStateSectionName);
        if(vData.isValid() && !vData.isNull() && vData.canConvert<QByteArray>())
        {
            zv_mainSplitter->restoreState(vData.toByteArray());
        }
    }

    settings.endGroup();
    settings.endGroup();
}
//=============================================================
void ZEnergyLineSelectionDialog::zh_onOkClick()
{
    accept();
}
//=============================================================
void ZEnergyLineSelectionDialog::zh_onSelectedChemicalElementChange(int ZNumber, bool selected)
{
    QList<int> selectedChemicalElementList = zv_periodicTableWidget->zp_selectedChemicalElementList();

    if(selectedChemicalElementList.isEmpty())
    {
        zv_chemicalPropertyProxyTableModel->zp_onCurrentTreeElementChanged(QModelIndex(), QModelIndex());
    }

    QStringList sectionBranch;
    sectionBranch.append("Energy Lines");

    QModelIndex energyLinesSectionIndex = zv_chemicalElementPropertyTreeModel->zp_indexForSection(selectedChemicalElementList.first(), sectionBranch);
    zv_chemicalPropertyProxyTableModel->zp_onCurrentTreeElementChanged(energyLinesSectionIndex, QModelIndex());
}
//=============================================================
void ZEnergyLineSelectionDialog::zh_onCurrentEnergyLineChange(const QModelIndex& current, const QModelIndex& previous)
{
    if(!current.isValid())
    {
        return;
    }

    // change selected chem element and line
    // define current chemical elment symbol
    QList<int> selectedChemicalElementList = zv_periodicTableWidget->zp_selectedChemicalElementList();
    if(selectedChemicalElementList.isEmpty())
    {
        return;
    }

    PropertyList propertyList;
    zv_chemicalElementPropertyTreeModel->zp_chemicalElementProperties(selectedChemicalElementList.first(), QStringList(), propertyList);
    QString symbolPropertyName = zv_chemicalElementPropertyTreeModel->zp_symbolPropertyName();
    for(int i = 0; i < propertyList.count(); i++)
    {
        if(propertyList.at(i).first == symbolPropertyName)
        {
            zv_chemicalElementSymbol = propertyList.at(i).second;
            break;
        }
    }

    int propertyRow = current.row();
    // line name
    QModelIndex propertyIndex = zv_chemicalPropertyProxyTableModel->index(propertyRow, 0);
    if(!propertyIndex.isValid())
    {
        zv_energyLineName = tr("not defined");
    }
    else
    {
        QVariant vData = propertyIndex.data(Qt::DisplayRole);
        if(!vData.isValid() || !vData.canConvert<QString>())
        {
            zv_energyLineName = tr("not defined");
        }
        else
        {
            zv_energyLineName = vData.toString();
        }
    }
    // line value
    propertyIndex = zv_chemicalPropertyProxyTableModel->index(propertyRow, 1);
    if(!propertyIndex.isValid())
    {
        zv_energyLineEnergyValue = 0.0;
    }
    else
    {
        QVariant vData = propertyIndex.data(Qt::DisplayRole);
        if(!vData.isValid() || !vData.canConvert<double>())
        {
            zv_energyLineEnergyValue = 0.0;
        }
        else
        {
            zv_energyLineEnergyValue = vData.toDouble();
        }
    }

    zh_updateSelectedLineLineEdit();

}
//=============================================================
void ZEnergyLineSelectionDialog::zh_updateSelectedLineLineEdit()
{
    QString selectedLineText = tr("%1 %2 - %3 (kEv)").
            arg(zv_chemicalElementSymbol, zv_energyLineName, QString::number(zv_energyLineEnergyValue));

    zv_selectedEnergyLineLineEdit->setText(selectedLineText);
}
//=============================================================

