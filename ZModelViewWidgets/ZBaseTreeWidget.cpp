//============================================================
#include "ZBaseTreeWidget.h"
#include "ZControlAction.h"
#include "ZSampleTaskTreeModel.h"

#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QTreeView>
//============================================================
ZBaseTreeWidget::ZBaseTreeWidget(QWidget *parent) : QWidget(parent)
{
    zv_captionLabel = nullptr;
    zh_createComponents();
    zh_createConnections();
}
//============================================================
void ZBaseTreeWidget::zp_setModel(ZSampleTaskTreeModel* model)
{
    zv_table->setModel(model);
    connect(model, &ZSampleTaskTreeModel::zg_setCurrentIndex,
            zv_table, &QTreeView::setCurrentIndex);
    zv_table->setAlternatingRowColors(true);

}
//============================================================
void ZBaseTreeWidget::zp_setCaption(const QString& caption)
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
//============================================================
void ZBaseTreeWidget::zp_appendButtonActions(const QList<ZControlAction*>& actionList)
{
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
        if(actionList.at(a)->icon().isNull())
        {
            button->setText(actionList.at(a)->text());
        }
        else
        {
            button->setIcon(actionList.at(a)->icon());
        }
        button->setToolTip(actionList.at(a)->toolTip());
        button->setEnabled(actionList[a]->isEnabled());
        connect(button, &QPushButton::clicked,
                actionList[a], &QAction::trigger);
        connect(actionList.at(a), &ZControlAction::zg_enableChanged,
                button, &QPushButton::setEnabled);

        zv_buttonLayout->addWidget(button, 0, Qt::AlignRight);
    }
}
//============================================================
void ZBaseTreeWidget::zp_appendButton(QPushButton* button)
{
    if(!button)
    {
        return;
    }

    button->setParent(this);
    zv_buttonLayout->addWidget(button, 0, Qt::AlignRight);
}
//============================================================
void ZBaseTreeWidget::zp_appendContextActions(const QList<ZControlAction*>& actionList)
{

}
//============================================================
void ZBaseTreeWidget::zp_setSectionResizeMode(int section, QHeaderView::ResizeMode mode)
{
    zv_table->header()->setSectionResizeMode(section, mode);
}
//============================================================
void ZBaseTreeWidget::zp_setStretchLastSection(bool stretch)
{
    zv_table->header()->setStretchLastSection(stretch);
}
//============================================================
void ZBaseTreeWidget::zp_setHeaderVisible(bool visible)
{
    zv_table->header()->setVisible(visible);
}
//============================================================
void ZBaseTreeWidget::zp_setSelectionBehavior(QAbstractItemView::SelectionBehavior behavior)
{
    zv_table->setSelectionBehavior(behavior);
}
//============================================================
void ZBaseTreeWidget::zp_setSelectionMode(QAbstractItemView::SelectionMode mode)
{
     zv_table->setSelectionMode(mode);
}
//============================================================
QTreeView* ZBaseTreeWidget::zp_treeView() const
{
    return zv_table;
}
//============================================================
void ZBaseTreeWidget::zh_createComponents()
{
    zv_mainLayout = new QVBoxLayout;
    setLayout(zv_mainLayout);
    zv_mainLayout->setMargin(0);

    setLayout(zv_mainLayout);

    zv_table = new QTreeView(this);
    zv_table->setContextMenuPolicy(Qt::CustomContextMenu);

    zv_mainLayout->addWidget(zv_table, INT_MAX);

    zv_buttonLayout = new QHBoxLayout;
    zv_mainLayout->addLayout(zv_buttonLayout);
    zv_buttonLayout->addStretch();
}
//============================================================
void ZBaseTreeWidget::zh_createConnections()
{
    //    connect(zv_table, &QTableView::customContextMenuRequested,
    //            this, &ZJointSpectrumTableWidget::zh_onContextMenuRequest);
}
//============================================================
