//======================================================
#include "ZPeriodicTableWidget.h"
#include "ZChemicalElementPropertyTreeModel.h"

#include <QAbstractItemModel>
#include <QApplication>
#include <QDebug>
#include <QGridLayout>
#include <QLabel>
#include <QResizeEvent>
#include <QVBoxLayout>
//======================================================
ZPeriodicTableWidget::ZPeriodicTableWidget(QWidget *parent)
    : QWidget(parent)
{
    zv_model = nullptr;
    zh_createComponents();
    zh_createConnections();
    zh_recalcAndApplyMinButtonSize();
    zp_setSelectionMode(SM_NO_SELECTION);
}
//======================================================
void ZPeriodicTableWidget::zh_createComponents()
{
    zv_mainLayout = new QVBoxLayout(this);
    // main button layout
    QGridLayout* buttonGridLayout = new QGridLayout;
    buttonGridLayout->setSpacing(0);
    zv_mainLayout->addLayout(buttonGridLayout);

    // button placing
    ZChemicalElementButton* chemicalElementButton;
    QLabel* label;
    uint currentZNumber = 0;
    for(int row = 0; row < 10; row++)
    {
        if(row == 7)
        {
            label = new QLabel;
            buttonGridLayout->addWidget(label, row, 0, 1, 17, Qt::AlignHCenter | Qt::AlignBottom);
            continue;
        }

        // buttons
        for(int col = 0; col < 18; col++)
        {
            // I period  (H and He)
            if(row == 0)
            {
                if(col != 0 && col != 17)
                {
                    continue;
                }
            }

            //  II III periods
            if(row > 0 && row < 3)
            {
                if(col > 1 && col < 12)
                {
                    continue;
                }
            }

            // Lanthanide // Actinide
            if(row > 7)
            {
                if(col < 2 || col > 15)
                {
                    continue;
                }
            }

            // Adjust element number
            if(currentZNumber == 57)
            {
                // skip Lanthanide
                currentZNumber = 71;
            }
            else if(currentZNumber == 89)
            {
                // skip Actinide
                currentZNumber = 103;
            }
            else if(currentZNumber == 118)
            {
                // return to lanthanide
                currentZNumber = 57;
            }
            else if(currentZNumber == 71)
            {
                // return to actinide
                currentZNumber = 89;
            }

            // create and place button
            chemicalElementButton = new ZChemicalElementButton(++currentZNumber);
            chemicalElementButton->setCheckable(true);
            chemicalElementButton->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
            buttonGridLayout->addWidget(chemicalElementButton, row, col, 1, 1);
            // apppend the button to the list
            zv_elementButtonList.insert(static_cast<int>(currentZNumber - 1), chemicalElementButton);
        }
    }
}
//======================================================
void ZPeriodicTableWidget::zh_createConnections()
{
    foreach(ZChemicalElementButton* button, zv_elementButtonList)
    {
        connect(button, &ZChemicalElementButton::zg_symbolChanged,
                this, &ZPeriodicTableWidget::zh_recalcAndApplyMinButtonSize);
        connect(button, &ZChemicalElementButton::toggled,
                this, &ZPeriodicTableWidget::zh_onButtonToggle);


    }
}
//======================================================
void ZPeriodicTableWidget::zp_setMainLayoutMargin(int margin)
{
    if(margin < 0)
    {
        return;
    }

    zv_mainLayout->setMargin(margin);
}
//======================================================
void ZPeriodicTableWidget::zp_setModel(QAbstractItemModel* model)
{
    if(model == nullptr)
    {
        return;
    }

    // disconnect from
    if(zv_model != nullptr)
    {
        disconnect(model, &QAbstractItemModel::modelReset,
                   this, &ZPeriodicTableWidget::zh_updatePeriodicTable);
    }

    // connect to model
    zv_model = model;
    connect(model, &QAbstractItemModel::modelReset,
            this, &ZPeriodicTableWidget::zh_updatePeriodicTable);

    connect(model, &QAbstractItemModel::dataChanged,
            this, &ZPeriodicTableWidget::zh_updateButtonCaptions);

    zh_updatePeriodicTable();
}
//======================================================
QList<int> ZPeriodicTableWidget::zp_selectedChemicalElementList() const
{
    QList<int> selectedChemicalElementList;
    foreach(ZChemicalElementButton* button, zv_elementButtonList)
    {
        if(button->isChecked())
        {
            selectedChemicalElementList.append(static_cast<int>(button->zp_zNumber()));
        }
    }

    return selectedChemicalElementList;
}
//======================================================
void ZPeriodicTableWidget::zp_fillSelectedChemicalElementList(QList<int>& ZNumberList) const
{
    ZNumberList = zp_selectedChemicalElementList();
}
//======================================================
void ZPeriodicTableWidget::zp_setSelectionMode(SelectionMode selectionMode)
{
    zv_selectionMode = selectionMode;
    foreach(ZChemicalElementButton* button, zv_elementButtonList)
    {
        button->setCheckable(zv_selectionMode != SM_NO_SELECTION);
    }

}
//======================================================
ZPeriodicTableWidget::SelectionMode ZPeriodicTableWidget::zp_selectionMode()
{
    return zv_selectionMode;
}
//======================================================
void ZPeriodicTableWidget::resizeEvent(QResizeEvent* event)
{
    zh_recalcAndApplyPropertyRectSizes();
    QWidget::resizeEvent(event);
}
//======================================================
void ZPeriodicTableWidget::zh_updatePeriodicTable()
{
    QModelIndex index;
    QVariant vData;
    QString dataString;
    QString toolTipString;
    for(int row = 0; row < zv_elementButtonList.count(); row++)
    {
        index = zv_model->index(row, 0, QModelIndex());
        if(!index.isValid())
        {
            continue;
        }

        // symbol
        vData = zv_model->data(index, SymbolRole);
        if(!vData.isValid() || !vData.canConvert<QString>())
        {
            continue;
        }
        zv_elementButtonList[row]->zp_setChemicalElementSymbol(vData.toString());

        // name
        vData = zv_model->data(index, NameRole);
        if(!vData.isValid() || !vData.canConvert<QString>())
        {
            continue;
        }
        zv_elementButtonList[row]->zp_setChemicalElementName(vData.toString());
        toolTipString = QString("%1 - %2").arg(QString::number(zv_elementButtonList[row]->zp_zNumber()), zv_elementButtonList[row]->zp_chemicalElementName());
        zv_elementButtonList[row]->setToolTip(toolTipString);
    }
}
//======================================================
void ZPeriodicTableWidget::zh_updateButtonCaptions(const QModelIndex& topLeft,
                                                   const QModelIndex& bottomRight)
{
    zh_updatePeriodicTable();
}
//======================================================
void ZPeriodicTableWidget::zh_onButtonToggle(bool toggled)
{
    ZChemicalElementButton* toggledButton = qobject_cast<ZChemicalElementButton*>(sender());
    if(toggledButton->isChecked() && zv_selectionMode == SM_SINGLE_SELECTION)
    {
        // make other button unchecked
        foreach(ZChemicalElementButton* button, zv_elementButtonList)
        {
            if(button == toggledButton)
            {
                continue;
            }
            button->setChecked(false);
        }
    }

    emit zg_selectedChemicalElementChanged(toggledButton->zp_zNumber(), toggledButton->isChecked());
}
//======================================================
void ZPeriodicTableWidget::zh_recalcAndApplyMinButtonSize()
{
    QSize maxSymbolSize;
    zh_maxSymbolRectSize(maxSymbolSize);

    foreach(ZChemicalElementButton* button, zv_elementButtonList)
    {
        button->zp_updateMinimumSizeForSymbolRectSize(maxSymbolSize);
    }
}
//======================================================
bool ZPeriodicTableWidget::zh_setChemicalElementSymbol(ZNumber zNumber,
                                                       const QString& symbol)
{
    ZChemicalElementButton* button = nullptr;
    if(!zh_buttonForZNumber(zNumber,  button))
    {
        return false;
    }

    button->zp_setChemicalElementSymbol(symbol);
    return true;
}
//======================================================
void ZPeriodicTableWidget::zh_recalcAndApplyPropertyRectSizes()
{
    QSize symbolRectSize;
    QSize zNumberRectSize;
    QSize nameRectSize;
    zh_recalcPropertyRectSizes(symbolRectSize,
                               zNumberRectSize,
                               nameRectSize);

    zh_applyPropertyRectSizes(symbolRectSize,
                              zNumberRectSize,
                              nameRectSize);


}
//======================================================
void ZPeriodicTableWidget::zh_recalcPropertyRectSizes(QSize& symbolRectSize,
                                                      QSize& zNumberRectSize,
                                                      QSize& nameRectSize) const
{
    // reset rects
    symbolRectSize = QSize();
    zNumberRectSize = QSize();
    nameRectSize = QSize();

    // get Button rect sizes
    QSize minButtonSize;
    QSize symbolWindowRectSize;
    QSize zNumberWindowRectSize;
    QSize nameWindowRectSize;

    zh_buttonRectSizes(minButtonSize,
                       symbolWindowRectSize,
                       zNumberWindowRectSize,
                       nameWindowRectSize);

    if(minButtonSize.isEmpty() || symbolWindowRectSize.isEmpty())
    {
        return;
    }

    // define included property rect sizes
    if(minButtonSize.height() - symbolWindowRectSize.height() < zNumberWindowRectSize.height())
    {
        // only symbol
        zNumberWindowRectSize = QSize();
        nameWindowRectSize = QSize();
    }
    else if(minButtonSize.height() - symbolWindowRectSize.height() - zNumberWindowRectSize.height()
            < nameWindowRectSize.height() || minButtonSize.width() < nameWindowRectSize.width())
    {
        // symbol and Z
        nameWindowRectSize = QSize();
    }

    // Define increase factor
    //
    int commonWindowHeight = symbolWindowRectSize.height()
            + zNumberWindowRectSize.height() + nameWindowRectSize.height();

    int commonWindowWidth = qMax(qMax(symbolWindowRectSize.width(), zNumberWindowRectSize.width()),
                                 nameWindowRectSize.width());

    if(commonWindowHeight <= 0 || commonWindowWidth <= 0)
    {
        // no a caption
        return;
    }

    double windowSideRatio = static_cast<double>(commonWindowWidth)
            / static_cast<double>(commonWindowHeight);

    double buttonSideRatio = static_cast<double>(minButtonSize.width())
            / static_cast<double>(minButtonSize.height());

    double scaleFactor;
    if(buttonSideRatio > windowSideRatio)
    {
        // fit by height
        scaleFactor = static_cast<double>(minButtonSize.height())
                / static_cast<double>(commonWindowHeight);
    }
    else
    {
        // fit by width
        scaleFactor = static_cast<double>(minButtonSize.width())
                / static_cast<double>(commonWindowWidth);

        // calc height of button rect reduced to commonWindowWidth
        int reducedButtonHeight = qRound(static_cast<double>(commonWindowWidth) / buttonSideRatio);
        int freeHeightForSymbol = reducedButtonHeight - zNumberWindowRectSize.height() - nameWindowRectSize.height();

        double freeWindowForSymbolSideRatio = static_cast<double>(commonWindowWidth)
                / static_cast<double>(freeHeightForSymbol);

        double symbolWindowSideRatio = static_cast<double>(symbolWindowRectSize.width())
                / static_cast<double>(symbolWindowRectSize.height());

        double symbolScaleFactor; // symbol rect size can be increased if it is possible
        if(freeWindowForSymbolSideRatio > symbolWindowSideRatio)
        {
            // fit by height
            symbolScaleFactor = static_cast<double>(freeHeightForSymbol)
                    / static_cast<double>(symbolWindowRectSize.height());
        }
        else
        {
            // fit by width
            symbolScaleFactor = static_cast<double>(commonWindowWidth)
                    / static_cast<double>(symbolWindowRectSize.width());
        }
        // recalc symbol window rect size
        symbolWindowRectSize.setHeight(qRound(symbolWindowRectSize.height() * symbolScaleFactor));
        symbolWindowRectSize.setWidth(qRound(symbolWindowRectSize.width() * symbolScaleFactor));
    }

    // recalc every rect size
    if(!zNumberWindowRectSize.isEmpty())
    {
        zNumberRectSize.setHeight(qRound(zNumberWindowRectSize.height() * scaleFactor));
        zNumberRectSize.setWidth(qRound(zNumberWindowRectSize.width() * scaleFactor));
    }

    if(!nameWindowRectSize.isEmpty())
    {
        nameRectSize.setHeight(qRound(nameWindowRectSize.height() * scaleFactor));
        nameRectSize.setWidth(qRound(nameWindowRectSize.width() * scaleFactor));
    }

    if(!symbolWindowRectSize.isEmpty())
    {
        symbolRectSize.setHeight(qRound(symbolWindowRectSize.height() * scaleFactor));
        symbolRectSize.setWidth(qRound(symbolWindowRectSize.width() * scaleFactor));
    }
}
//======================================================
void ZPeriodicTableWidget::zh_applyPropertyRectSizes(const QSize& symbolRectSize,
                                                     const QSize& zNumberRectSize,
                                                     const QSize& nameRectSize)
{
    // set rects to chemical element button
    ZChemicalElementButton::zp_setSymbolBoundingRectSize(symbolRectSize);
    ZChemicalElementButton::zp_setZNumberBoundingRectSize(zNumberRectSize);
    ZChemicalElementButton::zp_setNameBoundingRectSize(nameRectSize);

    // apply rects to buttons
    foreach(ZChemicalElementButton* button, zv_elementButtonList)
    {
        button->zp_updatePropertyCaptions();
    }
}
//======================================================
void ZPeriodicTableWidget::zh_buttonRectSizes(QSize& minButtonSize,
                                              QSize& maxWidthSymbolRectSize,
                                              QSize& maxWidthZNumberRectSize,
                                              QSize& maxWidthNameRectSize) const
{
    if(zv_elementButtonList.isEmpty())
    {
        return;
    }
    // min button size
    int width = this->width();
    int height = this->height();

    // max width symbol rect
    int symbolWidth = 0;
    int symbolHeight = 0;
    // max width symbol rect
    int zNumberWidth = 0;
    int zNumberHeight = 0;
    // max width symbol rect
    int nameWidth = 0;
    int nameHeight = 0;

    QSize size;
    foreach(ZChemicalElementButton* button, zv_elementButtonList)
    {
        // minButtonSize
        size = button->zp_buttonViewPortRect().size();
        if(height > size.height())
        {
            height = size.height();
        }
        if(width > size.width())
        {
            width = size.width();
        }

        // symbol rect
        size = button->zp_symbolWindowRectSize();
        if(symbolWidth < size.width())
        {
            symbolWidth = size.width();
        }
        if(symbolHeight < size.height())
        {
            symbolHeight = size.height();
        }

        // Z rect
        size = button->zp_zNumberWindowRectSize();
        if(zNumberWidth < size.width())
        {
            zNumberWidth = size.width();
        }
        if(zNumberHeight < size.height())
        {
            zNumberHeight = size.height();
        }

        // name rect
        size = button->zp_nameWindowRectSize();
        if(nameWidth < size.width())
        {
            nameWidth = size.width();
        }
        if(nameHeight < size.height())
        {
            nameHeight = size.height();
        }
    }

    minButtonSize = QSize(width, height);
    maxWidthSymbolRectSize = QSize(symbolWidth, symbolHeight);
    maxWidthZNumberRectSize = QSize(zNumberWidth, zNumberHeight);
    maxWidthNameRectSize = QSize(nameWidth, nameHeight);
}
//======================================================
void ZPeriodicTableWidget::zh_maxSymbolRectSize(QSize& symbolSize) const
{
    int symbolWidth = 0;
    int symbolHeight = 0;

    QSize size;
    foreach(ZChemicalElementButton* button, zv_elementButtonList)
    {
        // symbol rect
        size = button->zp_symbolWindowRectSize();
        if(symbolWidth < size.width())
        {
            symbolWidth = size.width();
        }
        if(symbolHeight < size.height())
        {
            symbolHeight = size.height();
        }
    }

    symbolSize = QSize(symbolWidth, symbolHeight);
}
//======================================================
bool ZPeriodicTableWidget::zh_buttonForZNumber(ZNumber zNumber,
                                               ZChemicalElementButton*& button)
{
    foreach(ZChemicalElementButton* currentButton, zv_elementButtonList)
    {
        if(currentButton->zp_zNumber() == zNumber)
        {
            button = currentButton;
            return true;
        }
    }

    button = nullptr;
    return false;
}
//======================================================

