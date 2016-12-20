//============================================================
#include "ZBaseTreeWidget.h"
#include "ZControlAction.h"

#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QTreeView>
//============================================================
ZBaseTreeWidget::ZBaseTreeWidget(QWidget *parent) : QWidget(parent)
{
    zv_captionLabel = 0;
    zh_createComponents();
    zh_createConnections();
}
//============================================================
void ZBaseTreeWidget::zp_setModel(QAbstractItemModel* model)
{
    zv_table->setModel(model);
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
void ZBaseTreeWidget::zh_createComponents()
{
    zv_mainLayout = new QVBoxLayout(this);
    zv_mainLayout->setMargin(0);

    setLayout(zv_mainLayout);

    zv_table = new QTreeView(this);
    zv_table->setContextMenuPolicy(Qt::CustomContextMenu);

    zv_mainLayout->addWidget(zv_table, INT_MAX);

    zv_buttonLayout = new QHBoxLayout(this);
    zv_mainLayout->addLayout(zv_buttonLayout);
}
//============================================================
void ZBaseTreeWidget::zh_createConnections()
{
//    connect(zv_table, &QTableView::customContextMenuRequested,
//            this, &ZJointSpectrumTableWidget::zh_onContextMenuRequest);
}
//============================================================
