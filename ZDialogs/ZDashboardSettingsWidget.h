//============================================================
#ifndef ZDASHBOARDSETTINGSWIDGET_H
#define ZDASHBOARDSETTINGSWIDGET_H
//============================================================
#include <QWidget>
#include <ZDashboardSettings.h>
//============================================================
class QPushButton;
class QComboBox;
class QLabel;
class ZColorButton;
class QSlider;
class ZLabeledSliderWidget;
//============================================================
class ZDashboardSettingsWidget : public QWidget
{
    Q_OBJECT
public:
    explicit ZDashboardSettingsWidget(QWidget *parent = 0);

    void zp_setAppSettings(const ZDashboardSettings&);
    void zp_appSettings(ZDashboardSettings&);

signals:

    void zg_settingsChanged();

public slots:


private slots:

    void zh_onStyleComboBoxIndexChange(int value);
    void zh_someValueChange(int value);

private:

    // VARS
    QComboBox* zv_dashBoardStyleComboBox;
    ZLabeledSliderWidget* zv_dashBoardSizeSliderWidget;
    QComboBox* zv_locationComboBox;

    ZColorButton* zv_progressBarColorLabel;
    QPushButton* zv_progressBarColorDialogButton;

    ZColorButton* zv_textColorLabel;
    QPushButton* zv_textColorDialogButton;

    // FUNCS
    void zh_createComponents();
    void zh_createConnections();

    int zh_findComboBoxIndexForData(QComboBox* comboBox, QVariant data, int defaultIndex = 0) const;
    void zh_controlEnableManagement();

};
//============================================================
#endif // ZDASHBOARDSETTINGSWIDGET_H
