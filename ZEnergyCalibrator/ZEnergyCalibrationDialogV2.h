//======================================================
#ifndef ZENERGYCALIBRATIONDIALOGV2_H
#define ZENERGYCALIBRATIONDIALOGV2_H
//======================================================
#include <QDialog>
#include <QList>
#include <QMap>

#include <ZSpeSpectrum.h>
//======================================================
class ZPlotter;
class ZPlotterDataManager;

class QComboBox;
class QGroupBox;
class QLineEdit;
class QListWidget;
class QPushButton;
class QSpinBox;

//======================================================
class ZEnergyCalibrationDialogV2 : public QDialog
{
    Q_OBJECT
public:
    explicit ZEnergyCalibrationDialogV2(QMap<quint8, QList<ZSpeSpectrum *> > &spectrumMap,
                                        QWidget *parent = nullptr);
    ~ZEnergyCalibrationDialogV2();

signals:

public slots:


private slots:

    void zh_onOkClick();

private:

    // VARS
    const QString zv_settingsGroupName = "energyCalibrationDialogV2";
    const QString zv_geometrySectionName = "geometry";

    QMap<quint8, QList<ZSpeSpectrum*> > zv_spectrumMap;

    QComboBox* zv_gainFactorComboBox;
    QGroupBox* zv_mainGroupBox;
    QSpinBox* zv_channel1;
    QSpinBox* zv_channel2;

    QLineEdit* zv_elementLine1;
    QLineEdit* zv_elementLine2;

    QPushButton* zv_selectLineButton1;
    QPushButton* zv_selectLineButton2;

    QListWidget* zv_spectrumList;
    ZPlotterDataManager* zv_plotterDataManager;
    ZPlotter* zv_plotter;

    QPushButton* zv_okButton;
    QPushButton* zv_cancelButton;

    // FUNCS

    void zh_createComponents();
    QWidget* zh_createMainWidget();
    QWidget* zh_createElementLinesWidget();
    void zh_createConnections();

    void zh_restoreSettings();
    void zh_saveSettings();


};
//======================================================
#endif // ZENERGYCALIBRATIONDIALOGV2_H
