//===================================================
#include "ZCaptionFormatter.h"
//===================================================
QString ZCaptionFormatter::zv_htmlFormat = QString("<b>%1</b>");
//===================================================
ZCaptionFormatter::ZCaptionFormatter()
{

}
//===================================================
QString ZCaptionFormatter::zp_format(QString& title)
{
    return title = QString(zv_htmlFormat).arg(title);
}
//===================================================
void ZCaptionFormatter::zp_setHtmlFomat(const QString& htmlFormat)
{
    zv_htmlFormat = htmlFormat;
}
//===================================================
