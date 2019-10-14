//======================================================
#ifndef ZENERGYLINETABLEWIDGET_H
#define ZENERGYLINETABLEWIDGET_H
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
typedef int EnergyLineOperationType;
//======================================================
class ZEnergyLineTableWidget : public QWidget
{
    Q_OBJECT
public:
    explicit ZEnergyLineTableWidget(QWidget *parent = nullptr);

signals:

    void zg_energyLineVisibilityChanged(QString elementSymbol, QString lineName,
                                        double energy,
                                        bool visible) const;

    void zg_energyLineOperation(const QString& elementSymbol,
                                const QString& lineName,
                                EnergyLineOperationType operationType);

public slots:


    void zp_energyLineEnergyValue(const QString& elementSymbol,
                                  const QString& lineName,
                                  double& energyValue) const;

    void zp_energyLineVisibility(const QString& elementSymbol,
                                 const QString& lineName,
                                 bool& visible) const;

    void zp_energyLineColor(const QString& elementSymbol,
                            const QString& lineName,
                            QColor& visible) const;

    void zp_energyLineRelativeIntensity(const QString &elementSymbol,
                                        const QString& lineName,
                                        int& reletiveIntensity) const;
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
    void zh_energyLinesRelativeIntensityForZNumber(int ZNumber, PropertyList &propertyList);
    void zh_chemicalElementSymbol(int ZNumber, QString& symbol);

};
//======================================================
#endif // ZENERGYLINETABLEWIDGET_H
