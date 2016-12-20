//===============================================
#ifndef ZRAWWINDOW_H
#define ZRAWWINDOW_H
//===============================================
#include <QString>
#include <ZCalibrationWindow.h>
//===============================================
class ZRawWindow
{
public:
    ZRawWindow();

    QString name;
    ZCalibrationWindow::WindowType windowType;
    QString firstChannel;
    QString lastChannel;

};
//===============================================
#endif // ZRAWWINDOW_H
