//==========================================================
#ifndef ZSPECTRUMPAINTDATA
#define ZSPECTRUMPAINTDATA
//==========================================================
#include <QVector>
#include <QPointF>
#include <QMetaType>
#include <QColor>
//==========================================================
struct ZSpectrumPaintData
{
    QList<int> spectrumData;
    int maxChannel;
    int maxIntensity;
    QColor color;

};
Q_DECLARE_METATYPE(ZSpectrumPaintData)
//==========================================================
#endif // ZSPECTRUMPAINTDATA

