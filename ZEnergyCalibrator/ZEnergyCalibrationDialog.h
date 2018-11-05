//======================================================
#ifndef ZENERGYCALIBRATIONDIALOG_H
#define ZENERGYCALIBRATIONDIALOG_H
//======================================================
#include <QWidget>
//======================================================
class QSpinBox;
class ZBaseTableWidget;
class QPushButton;
class ZSpeSpectrum;
class ZPeakWindowTableModel;
//======================================================
class ZEnergyCalibrationDialog : public QWidget
{
    Q_OBJECT
public:
    explicit ZEnergyCalibrationDialog(QWidget *parent = 0);
    ~ZEnergyCalibrationDialog();

signals:

    void zg_inquiryCurrentVisibleSceneRect(QRectF& sceneRect) const;

public slots:

    void zp_setMeasuringConditionsAndSpectrum(quint8 gainFactor,
                                              int exposition,
                                              const ZSpeSpectrum *spectrum);

private slots:

    void zh_calculateSaveAndClose();

private:

    // VARS
    QSpinBox* zv_gainFactorSpinBox;
    ZBaseTableWidget* zv_peakTableWidget;
//    QPushButton* zv_addPeakWindowButton;
//    QPushButton* zv_removePeakWindowButton;

    QPushButton* zv_okButton;
    QPushButton* zv_cancelButton;

    ZPeakWindowTableModel* zv_peakWindowTableModel;

    // FUNCS
    void zh_createActions();
    void zh_createComponents();
    QWidget* zh_createControlWidget();
    void zh_createConnections();
    void zh_restoreSettings();
    void zh_saveSettings() const;

};
//======================================================
#endif // ZENERGYCALIBRATIONDIALOG_H
