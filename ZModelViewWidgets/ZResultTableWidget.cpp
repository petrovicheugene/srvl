//=============================================================
#include "ZResultTableWidget.h"
#include "ZGLConstantsAndDefines.h"

#include <QTableView>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QPushButton>
#include <QAction>

//=============================================================
ZResultTableWidget::ZResultTableWidget(QWidget *parent) : QWidget(parent)
{
    zh_createComponents();
    zh_createConnections();
}
//=============================================================
void ZResultTableWidget::zh_createComponents()
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
//=============================================================
void ZResultTableWidget::zh_createConnections()
{
//    connect(zv_table, &QTableView::customContextMenuRequested,
//            this, &ZJointSpectrumTableWidget::zh_onContextMenuRequest);
}
//=============================================================
