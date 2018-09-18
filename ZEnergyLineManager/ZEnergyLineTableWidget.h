//======================================================
#ifndef ZCHEMICALELEMENTENERGYLINEWIDGET_H
#define ZCHEMICALELEMENTENERGYLINEWIDGET_H
//======================================================
#include <QWidget>

//======================================================
class ZPeriodicTableWidget;
class ZChemicalElementPropertyTreeModel;
class ZSelectedEnergyLineTableModel;

class QSplitter;
class QTableView;

//======================================================
typedef QList<QPair<QString, QString> > PropertyList;
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
    ZSelectedEnergyLineTableModel* zv_selectedEnergyLineTableModel;

    // FUNCS
    void zh_createComponents();
    void zh_createConnections();
    void zh_energyLinesForZNumber(int ZNumber, PropertyList &propertyList);
    void zh_chemicalElementSymbol(int ZNumber, QString& symbol);

};
//======================================================
#endif // ZCHEMICALELEMENTENERGYLINEWIDGET_H
