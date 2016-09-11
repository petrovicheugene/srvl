//=======================================================
#ifndef ZPROGRESSBAROPTIONS_H
#define ZPROGRESSBAROPTIONS_H
//=======================================================
#include <QColor>
//=======================================================
class ZProgressBarOptions
{
public:
    ZProgressBarOptions();

    QColor zv_progressBarColor;
    QColor zv_textColor;
    int zv_progressBarSiteSize;
    int zv_fontPixelSize;
    enum ProgressBarStyle {PS_REGULAR, PS_SLIM} zv_progressBarStyle;

};
//=======================================================
#endif // ZPROGRESSBAROPTIONS_H
