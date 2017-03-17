//=============================================================
#include "ZAbstractSpectrumIOHandler.h"
//=============================================================
ZAbstractSpectrumIOHandler::ZAbstractSpectrumIOHandler(QObject *spectrumParent, QObject *parent) : QObject(parent)
{
    zv_spectrumParent = spectrumParent;
}
//=============================================================
ZAbstractSpectrumIOHandler::~ZAbstractSpectrumIOHandler()
{

}
//=============================================================

