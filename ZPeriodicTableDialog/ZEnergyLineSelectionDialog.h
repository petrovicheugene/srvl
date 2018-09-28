//=============================================================
#ifndef ZENERGYLINEDIALOG_H
#define ZENERGYLINEDIALOG_H
//=============================================================
#include <QDialog>
//=============================================================
class QLineEdit;
class QPushButton;
class QSplitter;
class QTableView;

class ZPeriodicTableWidget;
class ZChemicalElementPropertyTreeModel;
class ZChemicalPropertyProxyTableModel;

//=============================================================
class ZEnergyLineSelectionDialog : public QDialog
{
    Q_OBJECT
public:
    explicit ZEnergyLineSelectionDialog(QWidget *parent = nullptr);
    ~ZEnergyLineSelectionDialog();

    double zp_energyValue() const;
    QString zp_chemicalElementName() const;
    QString zp_energyLineName() const;

signals:

public slots:

private slots:

    void zh_onOkClick();
    void zh_onSelectedChemicalElementChange(int ZNumber, bool selected);
    void zh_onCurrentEnergyLineChange(const QModelIndex& current, const QModelIndex& previous);

private:

    // VARS
    const QString zv_settingsGroupName = "energyLineSelectionDialog";
    const QString zv_splitterStateSectionName = "mainSplitterState";
    const QString zv_geometrySectionName = "geometry";

    QString zv_chemicalElementSymbol;
    QString zv_energyLineName;
    double zv_energyLineEnergyValue;

    QSplitter* zv_mainSplitter;

    ZPeriodicTableWidget* zv_periodicTableWidget;
    QTableView* zv_chemicalElementPropertyTableView;
    QLineEdit* zv_selectedEnergyLineLineEdit;

    ZChemicalElementPropertyTreeModel* zv_chemicalElementPropertyTreeModel;
    ZChemicalPropertyProxyTableModel* zv_chemicalPropertyProxyTableModel;

    QPushButton* zv_okButton;
    QPushButton* zv_cancelButton;

    // FUNCS
    void zh_createComponents();
    void zh_createConnections();
    void zh_saveSettings();
    void zh_restoreSettings();
    void zh_updateSelectedLineLineEdit();


};
//=============================================================
#endif // ZENERGYLINEDIALOG_H
