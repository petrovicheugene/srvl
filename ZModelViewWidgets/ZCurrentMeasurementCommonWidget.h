//==========================================================
#ifndef ZCURRENTMEASUREMENTCOMMONWIDGET_H
#define ZCURRENTMEASUREMENTCOMMONWIDGET_H
//==========================================================
#include <QWidget>
#include "ZAppSettings.h"
//==========================================================
class QLayout;
class QAbstractItemModel;
class ZControlAction;
class ZDashboard;
class ZWidgetWithSidebar;
class ZCurrentMeasurementSampleTableWidget;
class ZCurrentMeasurementTaskTreeWidget;
//==========================================================
class ZCurrentMeasurementCommonWidget : public QWidget
{
    Q_OBJECT
public:
    explicit ZCurrentMeasurementCommonWidget(QWidget *parent = 0);

    // FUNCS
    void zp_applyAppSettings(const ZAppSettings &settings);
    void zp_appendTaskButtonActions(const QList<ZControlAction *> &actionList);
    void zp_appendTaskContextMenuActions(const QList<ZControlAction*>& actionList);

    void zp_appendSampleButtonActions(const QList<ZControlAction *> &actionList);
    void zp_appendSampleContextMenuActions(const QList<ZControlAction *> &actionList);

    void zp_setMeasurementSampleTableModel(QAbstractItemModel* model);
    void zp_setMeasurementTaskTreeModel(QAbstractItemModel* model);

signals:

public slots:

    /*!
     \brief

    */
    void zp_saveSettings() const;

private:

    // VARS
    ZWidgetWithSidebar* zv_sideBarTableWidget;
    ZCurrentMeasurementSampleTableWidget* zv_currentMeasurementSampleTableWidget;
    ZCurrentMeasurementTaskTreeWidget* zv_currentMeasurementTaskWidget;
    ZDashboard* zv_dashboard;
    // FUNCS

    void zh_createComponents();
    void zh_createConnections();
    void zh_rebuildLayout();

};
//==========================================================
#endif // ZCURRENTMEASUREMENTCOMMONWIDGET_H
