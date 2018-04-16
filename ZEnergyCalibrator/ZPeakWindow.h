//==================================================
#ifndef ZPEAKWINDOW_H
#define ZPEAKWINDOW_H
//==================================================
#include <QObject>
//==================================================

//==================================================
class ZPeakWindow : public QObject
{
    Q_OBJECT
public:
    explicit ZPeakWindow(QObject *parent = 0);

    bool zp_setPeakName(const QString& peakName);
    QString zp_peakName() const;

signals:

public slots:


protected:
    // VARS
    QString zv_peakName;

    // FUNCS

};
//==================================================
#endif // ZPEAKWINDOW_H
