//======================================================
#include "ZEnergyLineTableWidget.h"

#include "ZChemicalElementPropertyTreeModel.h"
#include "ZEnergyLineDelegate.h"
#include "ZPeriodicTableWidget.h"
#include "ZSelectedEnergyLineTableModel.h"

#include <QApplication>
#include <QDebug>
#include <QSplitter>
#include <QTableView>
#include <QVBoxLayout>
//======================================================
ZEnergyLineTableWidget::ZEnergyLineTableWidget(QWidget *parent) : QWidget(parent)
{
    zh_createComponents();
    zh_createConnections();
}
//======================================================
void ZEnergyLineTableWidget::zh_createComponents()
{
    zv_chemicalElementPropertyTreeModel = new ZChemicalElementPropertyTreeModel(this);
    zv_chemicalElementPropertyTreeModel->zp_setNamePropertyName("Name ru");
    zv_selectedEnergyLineTableModel = new ZSelectedEnergyLineTableModel(this);

    // main layout
    QVBoxLayout* mainLayout = new QVBoxLayout;
    setLayout(mainLayout);

    // slitter
    zv_mainSplitter = new QSplitter(Qt::Vertical, this);
    mainLayout->addWidget(zv_mainSplitter);


    // periodic table
    zv_periodicTableWidget = new ZPeriodicTableWidget(this);
    zv_periodicTableWidget->zp_setSelectionMode(ZPeriodicTableWidget::SM_MULTIPLE_SELECTION);
    zv_mainSplitter->addWidget(zv_periodicTableWidget);

    // selected chemical element table view
    zv_selectedChemicalElementTableView = new QTableView(this);
    zv_mainSplitter->addWidget(zv_selectedChemicalElementTableView);

}
//======================================================
void ZEnergyLineTableWidget::zh_createConnections()
{
    zv_periodicTableWidget->zp_setModel(zv_chemicalElementPropertyTreeModel);

    zv_selectedChemicalElementTableView->setModel(zv_selectedEnergyLineTableModel);
    zv_selectedChemicalElementTableView->setItemDelegate(new ZEnergyLineDelegate);


    connect(zv_periodicTableWidget, &ZPeriodicTableWidget::zg_selectedChemicalElementChanged,
            zv_selectedEnergyLineTableModel, &ZSelectedEnergyLineTableModel::zp_onSelectedChemicalElementChange);

    connect(zv_selectedEnergyLineTableModel, &ZSelectedEnergyLineTableModel::zg_requestSelectedChemicalElements,
            zv_periodicTableWidget, &ZPeriodicTableWidget::zp_fillSelectedChemicalElementList);

    connect(zv_selectedEnergyLineTableModel, &ZSelectedEnergyLineTableModel::zg_requestEnergyLinesForZNumber,
            this, &ZEnergyLineTableWidget::zh_energyLinesForZNumber);

    connect(zv_selectedEnergyLineTableModel, &ZSelectedEnergyLineTableModel::zg_requestChemicalElementSymbol,
            this, &ZEnergyLineTableWidget::zh_chemicalElementSymbol);


}
//======================================================
void ZEnergyLineTableWidget::zh_energyLinesForZNumber(int ZNumber, PropertyList& propertyList)
{
    QStringList propertySectionBranch;
    propertySectionBranch << "Energy Lines";
    zv_chemicalElementPropertyTreeModel->zp_chemicalElementProperties(ZNumber, propertySectionBranch, propertyList);

    for(auto& property : propertyList)
    {
        qDebug() << property.first << property.second;
    }
}
//======================================================
void ZEnergyLineTableWidget::zh_chemicalElementSymbol(int ZNumber, QString& symbol)
{
    PropertyList propertyList;
    zv_chemicalElementPropertyTreeModel->zp_chemicalElementProperties(ZNumber, QStringList(), propertyList);

    for(auto& property : propertyList)
    {
        if(property.first.toLower() == "symbol")
        {
            symbol = property.second;
        }

        qDebug() << property.first << property.second;

    }
}
//======================================================

