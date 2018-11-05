//======================================================
#ifndef ZENERGYCALIBRATIONLINE_H
#define ZENERGYCALIBRATIONLINE_H
//======================================================
#include <QObject>
#include <QColor>
#include <QString>
//======================================================
class ZEnergyCalibrationLine : public QObject
{
    Q_OBJECT
public:
    explicit ZEnergyCalibrationLine(QObject* parent = 0);

    int zp_channelNumber() const;
    double zp_energyValue() const;
    QString zp_lineName() const;
    QColor zp_lineColor() const;
    bool zp_isEnabled() const;
    void zp_setEnabled(bool enabled);

public slots:

    void zp_setChannelNumber(int channelNumber);
    void zp_setEnergyValue(double energyValue);
    void zp_setEnergyValueString(const QString& value);
    void zp_setLineName(const QString& lineName);
    void zp_setLineColor(QColor color);

signals:

    void zg_channelNumber(int channelNumber);
    void zg_energyValue(double energyValue);
    void zg_lineName(const QString& lineName);
    void zg_lineColor(QColor color);

private:

    // VARS
    int zv_channelNumber;
    double zv_energyValue;
    QString zv_lineName;
    QColor zv_lineColor;
    bool zv_enabled;

};
//======================================================
#endif // ZENERGYCALIBRATIONLINE_H
