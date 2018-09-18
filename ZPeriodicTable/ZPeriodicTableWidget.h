//======================================================
#ifndef ZPERIODICTABLEWIDGET_H
#define ZPERIODICTABLEWIDGET_H
//======================================================
#include <QWidget>

#include "ZChemicalElementButton.h"
#include <QModelIndex>
//======================================================
class QAbstractItemModel;
class QResizeEvent;
class QVBoxLayout;

//======================================================
class ZPeriodicTableWidget : public QWidget
{
    Q_OBJECT
public:
    explicit ZPeriodicTableWidget(QWidget *parent = nullptr);

    enum SelectionMode {SM_NO_SELECTION,
                        SM_SINGLE_SELECTION,
                        SM_MULTIPLE_SELECTION};

    void zp_setSelectionMode(SelectionMode selectionMode);
    SelectionMode zp_selectionMode();
    void zp_setMainLayoutMargin(int margin);
    void zp_setModel(QAbstractItemModel* model);

    QList<int> zp_selectedChemicalElementList() const;


signals:

    void zg_selectedChemicalElementChanged(int zNumber, bool selected);

public slots:

    void zp_fillSelectedChemicalElementList(QList<int>& ZNumberList) const;

protected:

    void resizeEvent(QResizeEvent* event);

private slots:

    void zh_updatePeriodicTable();
    void zh_updateButtonCaptions(const QModelIndex& topLeft,
                                 const QModelIndex& bottomRight);
    void zh_recalcAndApplyMinButtonSize();

    void zh_onButtonToggle(bool toggled);

private:

    // VARS
    QAbstractItemModel* zv_model;
    QVBoxLayout* zv_mainLayout;
    QList<ZChemicalElementButton*> zv_elementButtonList;
    SelectionMode zv_selectionMode;

    // FUNCS
    void zh_createComponents();
    void zh_createConnections();

    bool zh_buttonForZNumber(ZNumber zNumber,  ZChemicalElementButton *&button);
    bool zh_setChemicalElementSymbol(ZNumber zNumber, const QString& symbol);
    void zh_recalcAndApplyPropertyRectSizes();
    void zh_recalcPropertyRectSizes(QSize &symbolRectSize,
                                    QSize &zNumberRectSize,
                                    QSize &nameRectSize) const;
    void zh_applyPropertyRectSizes(const QSize& symbolRectSize,
                                   const QSize& zNumberRectSize,
                                   const QSize& nameRectSize);
    void zh_buttonRectSizes(QSize &minButtonSize, QSize &maxWidthSymbolRectSize, QSize &maxWidthZNumberRectSize, QSize &maxWidthNameRectSize) const;
    void zh_maxSymbolRectSize(QSize& symbolSize) const;


};
//======================================================
#endif // ZPERIODICTABLEWIDGET_H
