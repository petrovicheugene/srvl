//============================================================
#ifndef ZXMLARRAYIOHANDLER_H
#define ZXMLARRAYIOHANDLER_H
//============================================================
#include <QObject>
#include <QFile>
#include "ZRawSpectrumArray.h"
#include "ZGeneral.h"
//============================================================
class QXmlStreamReader;
//============================================================
class ZXMLSpectrumArrayIOHandler : public QObject
{
    Q_OBJECT
public:
    explicit ZXMLSpectrumArrayIOHandler(QObject* parent = 0);
    virtual ~ZXMLSpectrumArrayIOHandler();

    bool zp_readSpectrumArray (QFile&, QList<ZRawSpectrumArray>&) const;
    bool zp_writeSpectrumArray(QFile&, const QList<ZRawSpectrumArray>&) const;

signals:

    void zg_message(QString) const;

private:

    // VARS
    const QString zv_magicString = glAppCompany + " 5FG3P72BNW8HS1L9FGX";
    // elment names
    const QString zv_ROOT = "root";
    const QString zv_TYPE = "type";
    const QString zv_NAME = "name";
    const QString zv_GAIN_FACTOR = "gainFactor";

    const QString zv_ARRAY = "array";
    const QString zv_SPECTRUM = "spectrum";
    const QString zv_PATH = "path";
    const QString zv_CONCENTRATION = "concentration";

    const QString zv_YES = "yes";
    const QString zv_NO = "no";


    // FUNCS
    bool zh_detectRoot(const QXmlStreamReader& reader, bool& magicStringDetectionFlag) const;
    void zh_parseXMLElement(QList<ZRawSpectrumArray>&,
                            int currentArrayIndex,
                            int currentSpectrumIndex,
                            QXmlStreamReader&) const;
    bool zh_checkfilePath(const QString& path) const;
};
//============================================================
#endif // ZXMLARRAYIOHANDLER_H
