//=============================================================
#include "ZMeasuringTaskInitStruct.h"
//=============================================================
ZMeasuringTaskInitStruct::ZMeasuringTaskInitStruct(int gainFactor, int exposition, QList<int> calibrationStackIdList)
{
    this->gainFactor = gainFactor;
    this->exposition = exposition;
    this->calibrationStackIdList = calibrationStackIdList;
}
//=============================================================
