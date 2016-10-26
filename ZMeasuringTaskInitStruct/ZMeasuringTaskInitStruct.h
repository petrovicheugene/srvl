//=============================================================
#ifndef ZMEASURINGTASKINITSTRUCT_H
#define ZMEASURINGTASKINITSTRUCT_H
//=============================================================
#include <QList>
//=============================================================
class ZMeasuringTaskInitStruct
{
public:
    ZMeasuringTaskInitStruct(int gainFactor = -1,
                             int exposition = -1,
                             QList<int> calibrationStackIdList = QList<int>());

    int gainFactor;
    int exposition;
    QList<int> calibrationStackIdList;

};
//=============================================================
#endif // ZMEASURINGTASKINITSTRUCT_H
