//==========================================================
#ifndef ZCENTRALWIDGET_H
#define ZCENTRALWIDGET_H
//==========================================================
#include <QWidget>
#include "ZAppSettings.h"
//==========================================================
class QLayout;
class ZDashboard;
//==========================================================
class ZCentralWidget : public QWidget
{
    Q_OBJECT
public:
    explicit ZCentralWidget(QWidget* tableWidget, QWidget *parent = 0);
    void zp_applyAppSettings(const ZAppSettings &settings);
    void zp_setTableWidget(QWidget* tableWidget, QWidget *&previousTableWidget);

signals:

public slots:


private:

    // VARS
    QWidget * zv_tableWidget;
    ZDashboard* zv_dashboard;
    // FUNCS

    void zh_createComponents();
    void zh_createConnections();
    void zh_rebuildLayout();

};
//==========================================================
#endif // ZCENTRALWIDGET_H
