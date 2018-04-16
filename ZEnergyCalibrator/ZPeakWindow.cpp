//==================================================
#include "ZPeakWindow.h"
//==================================================
ZPeakWindow::ZPeakWindow(QObject *parent) : QObject(parent)
{
    zv_peakName = "Unknown";
}
//==================================================
bool ZPeakWindow::zp_setPeakName(const QString& peakName)
{
    if(peakName.isEmpty())
    {
        return false;
    }

    zv_peakName = peakName;
    return true;
}
//==================================================
QString ZPeakWindow::zp_peakName() const
{
    return zv_peakName;
}
//==================================================
