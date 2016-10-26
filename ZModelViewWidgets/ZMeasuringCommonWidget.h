//==========================================================
#ifndef ZMEASURINGCOMMONWIDGET_H
#define ZMEASURINGCOMMONWIDGET_H
//==========================================================
#include <QWidget>
#include "ZAppSettings.h"
//==========================================================
class QLayout;
class QAbstractItemModel;
class ZControlAction;
class ZDashboard;
class ZWidgetWithSidebar;
class ZMeasuringResultTableWidget;
class ZMeasuringSeriesTaskTreeWidget;
//==========================================================
class ZMeasuringCommonWidget : public QWidget
{
    Q_OBJECT
public:
    explicit ZMeasuringCommonWidget(QWidget *parent = 0);

    // FUNCS
    void zp_applyAppSettings(const ZAppSettings &settings);
    void zp_appendTaskButtonActions(const QList<ZControlAction *> &actionList);
    void zp_appendTaskContextMenuActions(const QList<ZControlAction*>& actionList);

    void zp_appendSampleButtonActions(const QList<ZControlAction *> &actionList);
    void zp_appendSampleContextMenuActions(const QList<ZControlAction *> &actionList);

    void zp_setMeasuringResultTableModel(QAbstractItemModel* model);
    void zp_setMeasuringSeriesTaskTreeModel(QAbstractItemModel* model);

signals:

public slots:

    void zp_saveSettings() const;
    void zp_selectedSampleList(QList<int>& selectedSampleList) const;

private:

    // VARS
    ZWidgetWithSidebar* zv_sideBarTableWidget;
    ZMeasuringResultTableWidget* zv_measuringResultTableWidget;
    ZMeasuringSeriesTaskTreeWidget* zv_measuringSeriesTaskWidget;
    ZDashboard* zv_dashboard;
    // FUNCS

    void zh_createComponents();
    void zh_createConnections();
    void zh_rebuildLayout();

};
//==========================================================
#endif // ZMEASURINGCOMMONWIDGET_H
