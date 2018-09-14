//======================================================
#ifndef ZCHEMICALELEMENTPROPRTYEDITOR_H
#define ZCHEMICALELEMENTPROPRTYEDITOR_H
//======================================================
#include "ZBaseDialog.h"
#include "ZChemicalElementPropertyTreeModel.h"

#include <QItemSelection>
//======================================================
class QAbstractItemModel;
class QAction;
class QClipboard;
class QLabel;
class QPushButton;
class QSplitter;
class QTableView;
class QTreeView;

class ZChemicalPropertyProxyTableModel;
//======================================================
class ZChemicalElementProprtyEditor : public ZBaseDialog
{
    Q_OBJECT
public:
    explicit ZChemicalElementProprtyEditor(QWidget *parent = nullptr);
    ~ZChemicalElementProprtyEditor();

    void zp_setModel(QAbstractItemModel* model);

signals:

public slots:


protected:

    void zh_saveSettings() override;
    void zh_restoreSettings() override;
    bool eventFilter(QObject *watched, QEvent *event) override;
    void closeEvent(QCloseEvent* event) override;

private slots:

    void zh_addNewChemicalElementPropertySection();
    void zh_removeSelectedChemicalElementPropertySections();
    void zh_addNewChemicalElementProperty();
    void zh_removeSelectedChemicalElementProperties();
    void zh_copyPropertyToClipboard();
    void zh_pastePropertyFromClipboard();

    void zh_onTreeElementSelectionChange(const QItemSelection& selected,
                                         const QItemSelection& deselected);
    void zh_onPropertyTableElementSelectionChange(const QItemSelection& selected,
                                                  const QItemSelection& deselected);
    void zh_setPropertiesSourceFilePath(const QString& fileName);
    void zh_onClipboardDataChange();
    void zh_treeColumnVisibilityControl();
    void zh_onModelColumnsInserted(const QModelIndex& parent, int first, int last);

    void zh_onLoadButtonClick();
    void zh_onCloseButtonClick();
    void zh_onSaveButtonClick();

private:

    // VARS
    static QString zv_splitterStateName;

    const int zv_maxStretchValue = 99999999;
    QLabel* zv_sourceFileNameLabel;
    QClipboard* zv_clipboard;
    QAction* zv_addChemicalElementPropertySectionsAction;
    QAction* zv_removeChemicalElementPropertySectionsAction;
    QAction* zv_addChemicalElementPropertyAction;
    QAction* zv_removeChemicalElementPropertiesAction;
    QAction* zv_copyChemicalElementPropertiesAction;
    QAction* zv_pasteChemicalElementPropertiesAction;

    QAction* zv_expandAllAction;
    QAction* zv_collapseAllAction;

    QSplitter* zv_mainSplitter;
    QTreeView* zv_propertySectionTreeView;
    QTableView* zv_propertyTableView;
    ZChemicalPropertyProxyTableModel* zv_propertyFilterProxyModel;

    QPushButton* zv_addChemicalElementPropertySectionButton;
    QPushButton* zv_removeChemicalElementPropertySectionButton;
    QPushButton* zv_addChemicalElementPropertyButton;
    QPushButton* zv_removeChemicalElementPropertiesButton;

    QPushButton* zv_loadButton;
    QPushButton* zv_saveButton;
    QPushButton* zv_closeButton;

    // FUNCS
    void zh_createActions();
    void zh_createComponents();
    void zh_createMenu();
    void zh_createConnections();

    QWidget* zh_createPropertySectionTreeWidget();
    QWidget* zh_createPropertyTableWidget();

    bool zh_addChemicalElementPropertySectionBranch(const QList<int>& zNumberList,
                                                    const QStringList& sectionBranch);
    bool zh_currentChemicalElementSectionBrunch(QStringList &branch) const;

    bool zh_insertChemicalElementPropertiesToCurrentBranch(const PropertyList& propertyList);

    bool zh_selectedZNumberList(QList<int>& zNumberList) const;
    bool zh_currentZNumber(int& currentZNumber) const;
    void zh_adjustRemoveSectionAbility(const QItemSelection& selected);
    void zh_adjustRemoveAndCopyPropertyAbility(const QItemSelection& selected);
    void zh_adjustPastePropertyAbility();
    void zh_savePropertyDataToFile();
    QString zh_dataFileFolder() const;
    QString zh_standardLocation() const;

};
//======================================================
#endif // ZCHEMICALELEMENTPROPRTYEDITOR_H
