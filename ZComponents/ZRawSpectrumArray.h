//========================================================
#ifndef ZRAWARRAY
#define ZRAWARRAY
//========================================================
#include <QStringList>
#include <QMap>
//========================================================
struct ZRawSpectrum
{
    ZRawSpectrum()
    {
        path = QString();
        concentrationMap = QMap<QString, QString>();
    }

    QString path;
    QMap<QString, QString> concentrationMap; // QMap<Chem element, Concentration value>
};

//========================================================
struct ZRawSpectrumArray
{
    ZRawSpectrumArray()
    {
        name = QString();
        gainFactor = 0;
        spectrumList = QList<ZRawSpectrum>();
    }

    QString name;
    int gainFactor;
    QList<ZRawSpectrum> spectrumList;
};
//========================================================
#endif // ZRAWARRAY

