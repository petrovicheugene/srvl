//======================================================
#include "ZEnergyLineTableWidget.h"

#include "ZPeriodicTableWidget.h"
#include "ZChemicalElementPropertyTreeModel.h"

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

}
//======================================================

