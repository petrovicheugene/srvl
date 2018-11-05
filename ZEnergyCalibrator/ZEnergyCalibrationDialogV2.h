//======================================================
#ifndef ZENERGYCALIBRATIONDIALOGV2_H
#define ZENERGYCALIBRATIONDIALOGV2_H
//======================================================
#include <QDialog>
#include <QGridLayout>
#include <QList>
#include <QMap>
#include <QModelIndex>
#include <ZSpeSpectrum.h>
//======================================================
class ZEnergyCalibrationSpectrumTableModel;
class ZEnergyCalibrationLine;
class ZPlotter;
class ZPlotterDataManager;

class QAction;
class QCheckBox;
class QComboBox;
class QGroupBox;
class QLineEdit;
class QPushButton;
class QSpinBox;
class QTableView;
class QToolButton;
//======================================================
class ZEnergyCalibrationDialogV2 : public QDialog
{
    Q_OBJECT
public:
    explicit ZEnergyCalibrationDialogV2(QMap<quint8, QList<ZSpeSpectrum *> > &spectrumMap,
                                        QWidget *parent = nullptr);
    ~ZEnergyCalibrationDialogV2();

signals:

    void zg_energyCalibrationChanged(int gainFactor, const QList<double>& energyCalibrationFactors) const;

public slots:

private slots:

    void zh_calculateAndWriteEnergyCalibration();
    void zh_onCurrentGainFactorIndexChange(int currentIndex);
    void zh_onCurrentPeackCountIndexChange(int currentIndex);

    void zh_onModelReset();
    void zh_onModelDataChange(const QModelIndex& topLeft, const QModelIndex& bottomRight);
    void zh_onMousePressedOnPlotter(QPointF scenePos);
    void zh_onChannelNumberAction();
    void zh_onSelectEnergyLineAction();
    void zh_onClearEnergyLineAction();
    void zh_onLineColorChange(QColor color);
    void zh_onChannelNumberChange(int channelValue);

private:

    // VARS
    const QString zv_settingsGroupName = "energyCalibrationDialogV2";
    const QString zv_geometrySectionName = "geometry";
    const QString zv_lineColorListSectionName = "lineColorList";
    const QString zv_lineColorValueName = "lineColor";
    const int zv_calibrationLineNumber = 3;
    ZEnergyCalibrationSpectrumTableModel* zv_spectrumModel ;

    qreal zv_boundingRectTopFactor = 1.05;
    QRectF zv_defaultSceneRect = QRectF(QPointF(0.0,-100.0), QPointF(2048.0, 0.0));

    QComboBox* zv_gainFactorComboBox;
    QComboBox* zv_peakInEnergyCalibrationCountComboBox;
    QList<ZEnergyCalibrationLine*> zv_energyCalibrationLineList;

    QList<QAction*> zv_channelNumberActionList;

    QTableView* zv_spectrumTableView;
    ZPlotter* zv_plotter;

    QPushButton* zv_calculateButton;
    QPushButton* zv_closeButton;

    QList<QList<QWidget*> > zv_elementLinesWidgetList;
    // FUNCS

    void zh_createComponents();
    void zh_createContextMenu();

    QWidget* zh_createMainWidget();
    QWidget* zh_createElementLinesWidgetAndComponents();
    void zh_createElementLineControlsAndComponents(QGridLayout *layout, int elementNumber);

    void zh_createConnections();
    void zh_loadSpectrumData(QMap<quint8, QList<ZSpeSpectrum *> > &spectrumMap);

    //void zh_restoreLineColorList();
    void zh_restoreSettings();
    void zh_saveSettings();

    bool zh_calculateEnergyFactors(QList<double> &energyCalibrationFactorList);
    void zh_setElementLineControlsAndComponentVisibility();

    void zh_updateVerticalLines();

};
//======================================================
#endif // ZENERGYCALIBRATIONDIALOGV2_H
