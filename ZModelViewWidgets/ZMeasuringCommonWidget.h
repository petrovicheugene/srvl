//==========================================================
#ifndef ZMEASURINGCOMMONWIDGET_H
#define ZMEASURINGCOMMONWIDGET_H
//==========================================================
#include <QWidget>
#include "ZAppSettings.h"
#include "ZMeasuringManager.h"
//==========================================================
class QLayout;
class QAbstractItemModel;
class ZControlAction;
class ZDashboard;
class ZMeasuringManager;
class ZWidgetWithSidebar;
class ZMeasuringResultTableWidget;
class ZMeasuringSeriesTaskTreeWidget;
class ZSpectrumTableDelegate;
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
    void zp_connectToMeasuringManager(ZMeasuringManager* measuringManager);

signals:

    void zg_currentIndexChanged(const QModelIndex& current, const QModelIndex& previous) const;

public slots:

    void zp_saveSettings() const;
    void zp_selectedSampleList(QList<int>& selectedSampleList) const;
    void zp_currentIndex(QModelIndex& index) const;

private slots:

    void zp_setMeasuringState(ZMeasuringState measuringState);

private:

    // VARS
    //ZWidgetWithSidebar* zv_sideBarTableWidget;
    ZMeasuringResultTableWidget* zv_measuringResultTableWidget;
    ZMeasuringManager* zv_measuringManager;
    //ZMeasuringSeriesTaskTreeWidget* zv_measuringSeriesTaskWidget;
    ZDashboard* zv_dashboard;
    // FUNCS

    void zh_createComponents();
    void zh_createConnections();
    void zh_rebuildLayout();

};
//==========================================================
#endif // ZMEASURINGCOMMONWIDGET_H
