//============================================================
#ifndef ZMEASUREMENTPARAMETERSHANDLER_H
#define ZMEASUREMENTPARAMETERSHANDLER_H
//============================================================
#include <QObject>
#include <QColor>
#include <QMap>
//============================================================

//============================================================
class ZMeasuringParametersHandler
{
public:
    ZMeasuringParametersHandler();
    // initial data for parameters calculation: deadTimeMs, expositionPassedTimeMs, spectrumIntensityIntegral
    enum MeasurementParameter {MP_PASSED_EXPOSITION,
                               MS_ALIVE_TIME,
                               MS_DEAD_TIME,
                               MS_DEAD_TIME_PERCENT,
                               MS_SPECTRUM_INTENSITY_INTEGRAL,
                               MS_INPUT_WORKLOAD,
                               MS_OUTPUT_WORKLOAD
                              };

    enum OutputFlag {OF_DONT_OUTPUT,
                     OF_OUTPUT
                    };

    static void zp_setOutputFlagForParameter(MeasurementParameter parameter, OutputFlag flag);
    static void zp_setColorForParameter(MeasurementParameter parameter, QColor color);
    static void zp_setShortStringForParameter(MeasurementParameter parameter, QString string);

    static bool zp_calcParametersAndConvertToHtml(quint32 expositionPassedMs,
                                                  quint32 deadTimeMs,
                                                  quint32 spectrumIntensityIntegral,
                                                  QString& outHtmlString);

    static bool zp_calcParameters(quint32 expositionPassedMs,
                                  quint32 deadTimeMs,
                                  quint32 spectrumIntensityIntegral,
                                  quint32& outAliveTimeMs,
                                  double &outdeadTimePercent,
                                  quint32& outInputWorkload,
                                  quint32& outOutputWorkload);

    static bool zp_calcParameters(quint32 expositionPassedMs,
                                  quint32 deadTimeMs,
                                  quint32 spectrumIntensityIntegral,
                                  QMap<MeasurementParameter, QVariant>& parameterValueMap);


private:

    // VARS
    static QMap<MeasurementParameter, QVariantList> zv_measurementParameterPropertyMap;

    // FUNCS
    static QMap<MeasurementParameter, QVariantList> zh_initMeasurementParameterPropertyMap();

};
Q_DECLARE_METATYPE(ZMeasuringParametersHandler::OutputFlag)
Q_DECLARE_METATYPE(ZMeasuringParametersHandler::MeasurementParameter)

//============================================================
#endif // ZMEASUREMENTPARAMETERSHANDLER_H
