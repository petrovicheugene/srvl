//============================================================
#include "ZMeasurementParametersHandler.h"
#include <QVariant>
//============================================================

QMap<ZMeasuringParametersHandler::MeasurementParameter, QVariantList> ZMeasuringParametersHandler::zv_measurementParameterPropertyMap
= ZMeasuringParametersHandler::zh_initMeasurementParameterPropertyMap();
//============================================================
QMap<ZMeasuringParametersHandler::MeasurementParameter, QVariantList> ZMeasuringParametersHandler::zh_initMeasurementParameterPropertyMap()
{
    QMap<ZMeasuringParametersHandler::MeasurementParameter, QVariantList> measurementParameterPropertyMap;

    QVariantList parameterList;
    parameterList.append(QVariant());
    parameterList.append(QVariant());
    parameterList.append(QVariant());

    parameterList[0] = QVariant(OF_OUTPUT);
    parameterList[1] = QVariant(QObject::tr("Exposition passed:"));
    parameterList[2] = QVariant(QColor(Qt::darkYellow));

    measurementParameterPropertyMap.insert(MP_PASSED_EXPOSITION, parameterList);

    parameterList[0] = QVariant(OF_OUTPUT);
    parameterList[1] = QVariant(QObject::tr("Alive time:"));
    parameterList[2] = QVariant(QColor(Qt::darkGreen));
    measurementParameterPropertyMap.insert(MS_ALIVE_TIME, parameterList);

    parameterList[0] = QVariant(OF_OUTPUT);
    parameterList[1] = QVariant(QObject::tr("Dead time:"));
    parameterList[2] = QVariant(QColor(Qt::darkRed));
    measurementParameterPropertyMap.insert(MS_DEAD_TIME, parameterList);

    parameterList[0] = QVariant(OF_OUTPUT);
    parameterList[1] = QVariant(QObject::tr("Dead time %:"));
    parameterList[2] = QVariant(QColor(Qt::red));
    measurementParameterPropertyMap.insert(MS_DEAD_TIME_PERCENT, parameterList);

    parameterList[0] = QVariant(OF_OUTPUT);
    parameterList[1] = QVariant(QObject::tr("Workload in:"));
    parameterList[2] = QVariant(QColor(Qt::magenta));
    measurementParameterPropertyMap.insert(MS_INPUT_WORKLOAD, parameterList);

    parameterList[0] = QVariant(OF_OUTPUT);
    parameterList[1] = QVariant(QObject::tr("Workload out:"));
    parameterList[2] = QVariant(QColor(Qt::darkCyan));
    measurementParameterPropertyMap.insert(MS_OUTPUT_WORKLOAD, parameterList);

    parameterList[0] = QVariant(OF_OUTPUT);
    parameterList[1] = QVariant(QObject::tr("Intensity integral:"));
    parameterList[2] = QVariant(QColor(Qt::darkGray));
    measurementParameterPropertyMap.insert(MS_SPECTRUM_INTENSITY_INTEGRAL, parameterList);

    return measurementParameterPropertyMap;
}
//============================================================
ZMeasuringParametersHandler::ZMeasuringParametersHandler()
{

}
//============================================================
// data has been changed
// calc average income and outcome workload
// dead - alive time
//quint32 deadTime = static_cast<quint32>((static_cast<double>(zv_DeadTime) * (static_cast<double>(zv_ticDuration) / 1000.0)));
//quint32 aliveTime = zv_Time - (deadTime);
//double deadTimePercent = zv_Time == 0? 0.0 : static_cast<double>(deadTime) / (static_cast<double>(zv_Time) / 100.0 );
//// integral
//CHANNEL integral = 0;
//for(int i = 0; i < zv_bufferChannelSize; i++)
//{
//    integral += *(zv_buffer + i);
//}

//// workload
//quint32 averageOutLoad = zv_Time == 0?  0 : static_cast<quint32>((static_cast<double>(integral) / (static_cast<double>(zv_Time) / 1000.0)));
//quint32 averageInLoad = aliveTime == 0? 0 : static_cast<quint32>((static_cast<double>(integral)  / (static_cast<double>(aliveTime) / 1000.0)));

//qDebug() << "INTEGRAL" << integral << "pLoad" << zv_Load << "zv_DeadTime" << zv_DeadTime  << "zv_CountEvent" <<  zv_CountEvent << "L.IN" << averageInLoad << "L.OUT" << averageOutLoad << "D.T." << deadTime << "A.T"  << aliveTime << "D.T.%" << deadTimePercent << "T." << zv_Time;


//============================================================
void ZMeasuringParametersHandler::zp_setOutputFlagForParameter(MeasurementParameter parameter, OutputFlag flag)
{
    zv_measurementParameterPropertyMap[parameter][0] = QVariant(flag);
}
//============================================================
void ZMeasuringParametersHandler::zp_setColorForParameter(MeasurementParameter parameter, QColor color)
{
    zv_measurementParameterPropertyMap[parameter][2] = QVariant(color);
}
//============================================================
void ZMeasuringParametersHandler::zp_setShortStringForParameter(MeasurementParameter parameter, QString string)
{
    zv_measurementParameterPropertyMap[parameter][1] = QVariant(string);
}
//============================================================
bool ZMeasuringParametersHandler::zp_calcParametersAndConvertToHtml(quint32 expositionPassedMs,
                                                                    quint32 deadTimeMs,
                                                                    quint32 spectrumIntensityIntegral,
                                                                    QString& outHtmlString)
{
    QMap<MeasurementParameter, QVariant> parameterValueMap;
    if(!zp_calcParameters( expositionPassedMs,
                           deadTimeMs,
                           spectrumIntensityIntegral,
                           parameterValueMap))
    {
        return false;
    }

    QString outString;

    QMap<MeasurementParameter, QVariantList>::iterator it;
    QVariant vData;
    QString valueString;
    QString colorString;

    // set starting offset
    outString += "   ";
    for(it = zv_measurementParameterPropertyMap.begin(); it != zv_measurementParameterPropertyMap.end(); it++)
    {
        if(it.value().at(0).value<OutputFlag>() == OF_DONT_OUTPUT
                || !parameterValueMap.keys().contains(it.key()))
        {
            // flag - dont output parameter
            continue;
        }

        // get parameter value
        vData = parameterValueMap.value(it.key());
        if(!vData.isValid() || vData.isNull())
        {
            continue;
        }

        if(vData.canConvert<quint32>())
        {
            valueString = QString::number(vData.value<quint32>());
        }
        else if(vData.canConvert<double>())
        {
            valueString = QString::number(vData.value<double>());
        }
        else
        {
            continue;
        }

        //  get parameter color to string
        colorString = it.value().at(2).value<QColor>().name(QColor::HexArgb);
        outString += QString("<b><font color=%3> %1 %2 </font></b>").arg(it.value().at(1).toString(), valueString, colorString);
    }

    outHtmlString = outString;
    return true;
}
//============================================================
bool ZMeasuringParametersHandler::zp_calcParameters(quint32 expositionPassedMs,
                                                    quint32 deadTimeMs,
                                                    quint32 spectrumIntensityIntegral,
                                                    quint32& outAliveTimeMs,
                                                    double& outDeadTimePercent,
                                                    quint32& outInputWorkload,
                                                    quint32& outOutputWorkload)
{
    outAliveTimeMs = expositionPassedMs - (deadTimeMs);
    outDeadTimePercent = expositionPassedMs == 0? 0.0 : static_cast<double>(deadTimeMs) / (static_cast<double>(expositionPassedMs) / 100.0 );

    // workload
    outOutputWorkload = expositionPassedMs == 0?  0 : static_cast<quint32>((static_cast<double>(spectrumIntensityIntegral) / (static_cast<double>(expositionPassedMs) / 1000.0)));
    outInputWorkload = outAliveTimeMs == 0? 0 : static_cast<quint32>((static_cast<double>(spectrumIntensityIntegral)  / (static_cast<double>(outAliveTimeMs) / 1000.0)));

    return true;
}
//============================================================
bool ZMeasuringParametersHandler::zp_calcParameters(quint32 expositionPassedMs,
                                                    quint32 deadTimeMs,
                                                    quint32 spectrumIntensityIntegral,
                                                    QMap<MeasurementParameter, QVariant>& parameterValueMap)
{
    quint32 outAliveTimeMs;
    double outDeadTimePercent;
    quint32 outInputWorkload;
    quint32 outOutputWorkload;

    if(!zp_calcParameters( expositionPassedMs,
                           deadTimeMs,
                           spectrumIntensityIntegral,
                           outAliveTimeMs,
                           outDeadTimePercent,
                           outInputWorkload,
                           outOutputWorkload))
    {
        return false;
    }

    parameterValueMap[MP_PASSED_EXPOSITION] = QVariant(expositionPassedMs / 1000);
    parameterValueMap[MS_ALIVE_TIME] = QVariant(outAliveTimeMs / 1000);
    parameterValueMap[MS_DEAD_TIME] = QVariant(deadTimeMs / 1000);
    parameterValueMap[MS_DEAD_TIME_PERCENT] = QVariant(outDeadTimePercent);
    parameterValueMap[MS_SPECTRUM_INTENSITY_INTEGRAL] = QVariant(spectrumIntensityIntegral);
    parameterValueMap[MS_INPUT_WORKLOAD] = QVariant(outInputWorkload);
    parameterValueMap[MS_OUTPUT_WORKLOAD] = QVariant(outOutputWorkload);

    return true;
}
//============================================================
