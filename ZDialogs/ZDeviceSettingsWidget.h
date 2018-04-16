//=======================================================
#ifndef ZDEVICESETTINGSWIDGET_H
#define ZDEVICESETTINGSWIDGET_H
//=======================================================
#include <QWidget>
#include "ZDeviceSettings.h"
//=======================================================
class QLineEdit;
class QSpinBox;
class QComboBox;
class QPushButton;
//=======================================================
class ZDeviceSettingsWidget : public QWidget
{
    Q_OBJECT
public:
    explicit ZDeviceSettingsWidget(QWidget *parent = 0);

    void zp_setAppSettings(const ZDeviceSettings&);
    void zp_appSettings(ZDeviceSettings&);

signals:

public slots:


private slots:

    void zh_onLibraryFileBrowseClick();

private:

    // VARS
    QLineEdit* zv_deviceSerialLineEdit;
    QSpinBox * zv_deviceSampleQuantitySpinBox;
    QSpinBox * zv_deviceChannelQuantitySpinBox;
    QLineEdit* zv_deviceLibraryFilePathLineEdit;
    QPushButton* zv_deviceLibraryFileBrowseButton;
    QComboBox* zv_defaultADCType;
    QComboBox* zv_expositionStarterComboBox;
    QSpinBox* zv_sampleShiftImpulseDurationSpinBox;
    QSpinBox* zv_expositionDelayTimeSpinBox;

    const QString zv_manualConnectionstring = tr("Manual connection");

    // FUNCS
    void zh_createComponents();
    void zh_createConnections();

};
//=======================================================
#endif // ZDEVICESETTINGSWIDGET_H
