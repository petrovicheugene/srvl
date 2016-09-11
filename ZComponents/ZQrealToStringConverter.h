//===============================================
#ifndef ZQREALTOSTRINGCONVERTER_H
#define ZQREALTOSTRINGCONVERTER_H
//===============================================
#include <QString>
//===============================================
/*!
 \brief

*/
class ZQrealToStringConverter
{
public:
    /*!
     \brief

    */
    ZQrealToStringConverter();

    /*!
     \brief

     \param char
     \return bool
    */
    static bool zp_setFormat(char);
    /*!
     \brief

     \param int
     \return bool
    */
    static bool zp_setPrecision(int);
    /*!
     \brief

     \param value
     \return QString
    */
    static QString zp_convert(qreal value);
    /*!
     \brief

     \param
    */
    static void zh_chopTailZeroesFromDoubleString(QString&);

private:

    // VARS
    static char zv_format; /*!< TODO: describe */
    static int zv_precision; /*!< TODO: describe */

};
//===============================================
#endif // ZQREALTOSTRINGCONVERTER_H
