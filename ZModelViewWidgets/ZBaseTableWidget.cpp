//=========================================================================
#include "ZBaseTableWidget.h"
#include "ZGLConstantsAndDefines.h"
#include "ZControlAction.h"

#include <QAbstractItemModel>
#include <QTableView>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QLabel>
#include <QPushButton>

//=========================================================================
ZBaseTableWidget::ZBaseTableWidget(QWidget *parent) : QWidget(parent)
{
    zv_captionLabel = 0;
    zh_createComponents();
    zh_createConnections();
}
//=========================================================================
void ZBaseTableWidget::zp_setModel(QAbstractItemModel* model)
{
    zv_table->setModel(model);
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
        connect(button, &QPushButton::clicked,
                actionList[a], &QAction::trigger);
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
    zv_mainLayout = new QVBoxLayout(this);
    zv_mainLayout->setMargin(0);

    setLayout(zv_mainLayout);

    zv_table = new QTableView(this);
    zv_table->setContextMenuPolicy(Qt::CustomContextMenu);

    zv_mainLayout->addWidget(zv_table, INT_MAX);

    zv_buttonLayout = new QHBoxLayout(this);
    zv_mainLayout->addLayout(zv_buttonLayout);
}
//=========================================================================
void ZBaseTableWidget::zh_createConnections()
{
    //    connect(zv_table, &QTableView::customContextMenuRequested,
    //            this, &ZJointSpectrumTableWidget::zh_onContextMenuRequest);
}
//=========================================================================
