//===================================================
#ifndef ZCAPTIONFORMATTER_H
#define ZCAPTIONFORMATTER_H
//===================================================
#include <QString>
//===================================================
class ZCaptionFormatter
{
public:

    static QString zp_format(QString& title);
    static void zp_setHtmlFomat(const QString& htmlFormat);

private:
    ZCaptionFormatter();
    ~ZCaptionFormatter();
    ZCaptionFormatter(ZCaptionFormatter const&) = delete;
    void operator=(ZCaptionFormatter const&) = delete;

    static QString zv_htmlFormat;

};
//===================================================
#endif // ZCAPTIONFORMATTER_H
