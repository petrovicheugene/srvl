//=========================================================================
#include "ZBaseTableWidget.h"
#include "ZGeneral.h"
#include "ZControlAction.h"

#include <QAbstractItemModel>
#include <QHeaderView>
#include <QHBoxLayout>
#include <QList>
#include <QLabel>
#include <QPushButton>
#include <QTableView>
#include <QVBoxLayout>
//=========================================================================
ZBaseTableWidget::ZBaseTableWidget(QWidget *parent) : QWidget(parent)
{
    zv_captionLabel = 0;
    zh_createComponents();
    zh_createConnections();
}
//=========================================================================
void ZBaseTableWidget::zp_setModel(QAbstractItemModel* model, bool alternatingRowColorFlag)
{
    zv_table->setModel(model);
    connect(zv_table->selectionModel(), &QItemSelectionModel::currentChanged,
            this, &ZBaseTableWidget::zg_currentChanged);
    zv_table->setAlternatingRowColors(alternatingRowColorFlag);
}
//=========================================================================
QTableView* ZBaseTableWidget::zp_tableView() const
{
    return zv_table;
}
//=========================================================================
void ZBaseTableWidget::zp_setCaption(const QString& caption)
{
    if(zv_captionLabel != 0)
    {
        if(caption.isEmpty())
        {
            delete zv_captionLabel;
            zv_captionLabel = 0;
            return;
        }
        else
        {
            zv_captionLabel->setText(caption);
            return;
        }
    }
    else
    {
        if(!caption.isEmpty())
        {
            zv_captionLabel = new QLabel(this);
            zv_captionLabel->setText(caption);
            zv_mainLayout->insertWidget(0, zv_captionLabel);
            return;
        }
    }
}
//=========================================================================
void ZBaseTableWidget::zp_setColumnHidden(int column, bool hidden)
{
    zv_table->setColumnHidden(column, hidden);
}
//=========================================================================
void ZBaseTableWidget::zp_setHorizontalHeaderHidden(bool hidden)
{
    zv_table->horizontalHeader()->setHidden(hidden);
}
//=========================================================================
void ZBaseTableWidget::zp_setVerticalHeaderHidden(bool hidden)
{
    zv_table->verticalHeader()->setHidden(hidden);
}
//=========================================================================
void ZBaseTableWidget::zp_setStretchLastSection(bool stretch)
{
    zv_table->horizontalHeader()->setStretchLastSection(stretch);
}
//=========================================================================
void ZBaseTableWidget::zp_appendButtonActions(const QList<ZControlAction*>& actionList)
{
    zv_buttonLayout->addStretch();
    // zv_table->addActions(actionList);

    for(int a = 0; a < actionList.count(); a++)
    {
        if(actionList.at(a) == 0)
        {
            // Separator for context menu
            continue;
        }
        QPushButton* button = new QPushButton(this);
        button->setFlat(true);
        button->setIcon(actionList.at(a)->icon());
        button->setText(actionList.at(a)->text());
        button->setToolTip(actionList.at(a)->toolTip());
        button->setEnabled(actionList[a]->isEnabled());
        connect(button, &QPushButton::clicked,
                actionList[a], &QAction::trigger);
        connect(actionList.at(a), &ZControlAction::zg_enableChanged,
                button, &QPushButton::setEnabled);

        zv_buttonLayout->addWidget(button, 0, Qt::AlignRight);
    }
}
//=========================================================================
void ZBaseTableWidget::zp_appendContextActions(const QList<ZControlAction*>& actionList)
{

}
//=========================================================================
void ZBaseTableWidget::zh_createComponents()
{
    zv_mainLayout = new QVBoxLayout;
    zv_mainLayout->setMargin(0);
    setLayout(zv_mainLayout);

    zv_table = new QTableView(this);
    zv_table->setContextMenuPolicy(Qt::CustomContextMenu);

    zv_mainLayout->addWidget(zv_table, INT_MAX);

    zv_buttonLayout = new QHBoxLayout;
    zv_mainLayout->addLayout(zv_buttonLayout);
}
//=========================================================================
void ZBaseTableWidget::zh_createConnections()
{
    //    connect(zv_table, &QTableView::customContextMenuRequested,
    //            this, &ZJointSpectrumTableWidget::zh_onContextMenuRequest);
}
//=========================================================================
QList<int> ZBaseTableWidget::zp_selectedRowList()
{
    QList<int> selectedRowList;
    QModelIndexList indexList = zv_table->selectionModel()->selectedIndexes();
    foreach(QModelIndex index, indexList)
    {
        if(!selectedRowList.contains(index.row()) )
        {
            selectedRowList.append(index.row());
        }
    }

    return selectedRowList;
}
//=========================================================================
QModelIndexList ZBaseTableWidget::zp_selectedModelIndexList() const
{
    QModelIndexList indexList = zv_table->selectionModel()->selectedIndexes();
    return indexList;
}
//=========================================================================
void ZBaseTableWidget::zp_setCurrentRow(int row)
{
    QModelIndex currentIndex;
    QAbstractItemModel* model = zv_table->model();
    if(model)
    {
        currentIndex = model->index(row, 0);
    }

    zv_table->selectionModel()->setCurrentIndex(currentIndex, QItemSelectionModel::Clear);
    zv_table->selectionModel()->select(currentIndex, QItemSelectionModel::Clear);
}
//=========================================================================
void ZBaseTableWidget::zp_currentIndex(QModelIndex& index)
{
    index = QModelIndex();
    if(!zv_table)
    {
        return;
    }

    index = zv_table->currentIndex();
}
//=========================================================================
