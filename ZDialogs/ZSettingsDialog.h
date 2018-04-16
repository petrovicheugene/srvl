//===============================================================
#ifndef ZSETTINGSDIALOG_H
#define ZSETTINGSDIALOG_H
//===============================================================
#include <QDialog>
#include "ZAppSettings.h"
//===============================================================
class QPushButton;
class QComboBox;
class QLabel;

class ZDashboardSettingsWidget;
class ZDeviceSettingsWidget;
class ZCommonSettingsWidget;
//===============================================================
/*!
 \brief

*/
class ZSettingsDialog : public QDialog
{
    Q_OBJECT
public:
    /*!
     \brief

     \param parent
    */
    explicit ZSettingsDialog(QWidget *parent = 0);
    ~ZSettingsDialog();
    void zh_appSettings(ZAppSettings& appSettings) const;
    void zh_setAppSettings(const ZAppSettings &appSettings);

signals:

    /*!
     \brief

    */
    void zg_applySettings(const ZAppSettings&);

public slots:


private slots:

    /*!
     \brief

    */
    void zh_onOkClick();
    /*!
     \brief

    */
    void zh_onApplyClick();

private:

    // VARS
    QPushButton* zv_okButton; /*!< TODO: describe */
    QPushButton* zv_applyButton; /*!< TODO: describe */
    QPushButton* zv_cancelButton; /*!< TODO: describe */

    ZDashboardSettingsWidget* zv_dashboardSettingsWidget;
    ZDeviceSettingsWidget* zv_deviceSettingsWidget;
    ZCommonSettingsWidget* zv_commonSettingsWidget;
    // FUNCS

    /*!
     \brief

    */
    void zh_createComponents();

    /*!
     \brief

    */
    void zh_createConnections();
    void zh_saveSettings();
    void zh_restoreSettings();

};
//===============================================================
#endif // ZSETTINGSDIALOG_H
