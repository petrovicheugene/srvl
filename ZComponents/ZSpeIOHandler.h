//===========================================================
#ifndef ZSPEIOHANDLER_H
#define ZSPEIOHANDLER_H
//===========================================================
#include "ZAbstractSpectrumIOHandler.h"
#include "ZSpeSpectrum.h"

//===========================================================

//===========================================================
class ZSpeIOHandler : public ZAbstractSpectrumIOHandler
{
    Q_OBJECT
public:
    explicit ZSpeIOHandler(QObject* spectrumParent, QObject* parent = 0);
    virtual ~ZSpeIOHandler();

    bool zp_getSpectrumFromFile(const QString& path, QColor, ZAbstractSpectrum*&) const;
    bool zp_getSpectrumFromFile(QFile& file, QColor, ZAbstractSpectrum *&) const;
    //virtual bool zp_getSpectrumFromFile(QFile& file, ZSpectrumArray&) const override;

    bool zp_saveSpectrumToFile(const QString& path,
                               const QString& fileName,
                               ZSpeSpectrum *&spectrum);

    bool zp_saveSpectrumToFile(QFile& file, ZSpeSpectrum*& spectrum);

signals:

    void zg_message(QString) const;

private:

    // VARS
    const int zv_intensityStartLine = 20;
    int zv_whatToDoAnswer;
    // FUNCS

};
//===========================================================
#endif // ZSPEIOHANDLER_H
