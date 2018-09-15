//======================================================
#ifndef ZCHEMICALELEMENTENERGYLINEWIDGET_H
#define ZCHEMICALELEMENTENERGYLINEWIDGET_H
//======================================================
#include <QWidget>
//======================================================
class ZPeriodicTableWidget;
class ZChemicalElementPropertyTreeModel;

class QSplitter;
class QTableView;
//======================================================
class ZEnergyLineTableWidget : public QWidget
{
    Q_OBJECT
public:
    explicit ZEnergyLineTableWidget(QWidget *parent = nullptr);

signals:

public slots:


private:

    // VARS
    ZPeriodicTableWidget* zv_periodicTableWidget;
    QTableView* zv_selectedChemicalElementTableView;
    QSplitter* zv_mainSplitter;

    ZChemicalElementPropertyTreeModel* zv_chemicalElementPropertyTreeModel;


    // FUNCS
    void zh_createComponents();
    void zh_createConnections();


};
//======================================================
#endif // ZCHEMICALELEMENTENERGYLINEWIDGET_H
