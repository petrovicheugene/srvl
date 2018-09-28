//======================================================
#include "ZEnergyLineManager.h"
#include <QDebug>
//======================================================
ZEnergyLineManager::ZEnergyLineManager(QObject *parent) : QObject(parent)
{
    zt_createTestWidget();
}
//======================================================
void ZEnergyLineManager::zt_createTestWidget()
{
    QWidget* w = new QWidget;

    QVBoxLayout* mainLayout = new QVBoxLayout;
    w->setLayout(mainLayout);

    zt_channelSpinBox = new QDoubleSpinBox(w);
    mainLayout->addWidget(zt_channelSpinBox);

    zt_symbolLineEdit = new QLineEdit(w);
    mainLayout->addWidget(zt_symbolLineEdit);

    zt_setLineButton = new QPushButton(w);
    mainLayout->addWidget(zt_setLineButton);

    w->show();

    connect(zt_setLineButton, &QPushButton::clicked,
            this, &ZEnergyLineManager::zt_setLine);


}
//======================================================
void ZEnergyLineManager::zt_setLine()
{

}
//======================================================
