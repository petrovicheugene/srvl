//======================================================
#ifndef ZCHEMICALELEMENTENERGYLINEWIDGET_H
#define ZCHEMICALELEMENTENERGYLINEWIDGET_H
//======================================================
#include <QWidget>
//======================================================


//======================================================
class ZEnergyLineTableWidget : public QWidget
{
    Q_OBJECT
public:
    explicit ZEnergyLineTableWidget(QWidget *parent = nullptr);

signals:

public slots:


private:

    // VARS


    // FUNCS
    void zh_createComponents();
    void zh_createConnections();


};
//======================================================
#endif // ZCHEMICALELEMENTENERGYLINEWIDGET_H
