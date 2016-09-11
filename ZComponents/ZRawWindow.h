//===============================================
#ifndef ZRAWWINDOW_H
#define ZRAWWINDOW_H
//===============================================
#include <QString>
#include <ZCalibrationWindow.h>
//===============================================
/*!
 \brief

*/
class ZRawWindow
{
public:
    /*!
     \brief

    */
    ZRawWindow();

    QString name; /*!< TODO: describe */
    ZCalibrationWindow::WindowType windowType; /*!< TODO: describe */
    QString firstChannel; /*!< TODO: describe */
    QString lastChannel; /*!< TODO: describe */

};
//===============================================
#endif // ZRAWWINDOW_H
