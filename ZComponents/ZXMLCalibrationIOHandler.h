//==========================================================
#ifndef ZXMLCALIBRATIONARRAYIOHANDLER_H
#define ZXMLCALIBRATIONARRAYIOHANDLER_H
//==========================================================
#include <QObject>
#include <QApplication>
#include <QFile>
#include <QStack>
#include "ZCalibration.h"
#include "ZGeneral.h"
//==========================================================
class QXmlStreamReader;
//==========================================================
class ZXMLCalibrationIOHandler : public QObject
{
    Q_OBJECT
public:
    explicit ZXMLCalibrationIOHandler(/*QObject* calibrationParent,*/ QObject *parent = 0);
    virtual ~ZXMLCalibrationIOHandler();

    static QString zp_getCalibrationOpenFile(const QString& calibrationFolderPath);
    static QStringList zp_getCalibrationOpenFiles(const QString& calibrationFolderPath);
    static QString zp_getCalibrationSaveFile(const QString& calibrationFolderPath);
    static QString zp_checkDirPath(const QString& calibrationFolderPath);

    bool zp_getCalibrationFromFile(QFile&, ZCalibration*);
    bool zp_writeCalibrationToFile(QFile&file, const ZCalibration*calibration);

    bool zp_getCalibrationXMLByteArrayFromFile(
        QFile& file, QByteArray& calibrationXMLByteArray);
    bool zp_getCalibrationFromByteArray(QByteArray& byteArray,
                                        ZCalibration* calibration);
    bool zp_getCalibrationFromString(QString&, ZCalibration*);

    QString zp_message() const;

public slots:


signals:

    void zg_message(const QString&);

private:

    // VARS
    // QObject* zv_calibrationParent;
    // QString parentTagName;
    QStack<QString> parentTagStack;
    ZRawTerm zv_rawTerm;
    ZRawWindow zv_rawWindow;
    QString zv_message;

    const QString zv_magicString = qApp->organizationName()  + " 7D385RTNOW9SH31JZQL";
    // elment names
    const QString zv_ROOT = "root";
    const QString zv_TYPE = "type";
    const QString zv_STATE = "state";

    const QString zv_DATE_TIME = "date-time";
//    const QString zv_ENERGY_K0 = "energy_K0";
//    const QString zv_ENERGY_K1 = "energy_K1";
//    const QString zv_ENERGY_K2 = "energy_K2";
//    const QString zv_ENERGY_UNIT = "energy_unit";
    const QString zv_EXPOSITION = "exposition";
    const QString zv_GAIN_FACTOR = "gainFactor";

    const QString zv_CHEMELEMENT = "chem_element";
    const QString zv_DETERMINATION_R2 = "R2";
    const QString zv_ADJUSTED_DETERMINATION_R2ADJ = "R2adj";
    const QString zv_STANDARD_DEVIATION = "s";

    const QString zv_WINDOW_LIST = "window_list";
    const QString zv_WINDOW = "window";
    const QString zv_NAME = "name";
    const QString zv_FIRST_CHANNEL = "first_channel";
    const QString zv_LAST_CHANNEL = "last_channel";

    const QString zv_TERM_LIST = "term_list";
    const QString zv_TERM = "term";
    const QString zv_TERM_WINDOW = "term_window";
    const QString zv_FACTOR = "factor";

    const QString zv_EQUATION = "equation";
    const QString zv_POLYNOMIAL = "polynomial";
    const QString zv_FRACTIONAL = "fractional";
    const QString zv_BASE_TERM = "base_term";
    const QString zv_NORMALIZER = "normalizer";
    const QString zv_FRACTIONAL_BASE_NORMALIZER = "fractional_base_normalizer";

    const QString zv_CUSTOM_STRING = "custom_string";
    const QString zv_DESCRIPTION = "description";
    const QString zv_INTERCEPT = "intercept";

    const QString zv_YES = "yes";
    const QString zv_NO = "no";

    const QString zv_dateTimeFormat = "dd.MM.yyyy hh:mm:ss";

    // FUNCS
    bool zh_detectRoot(const QXmlStreamReader& reader, bool& magicStringDetectionFlag) const;
    void zh_parseXMLElement(ZCalibration*,
                            QXmlStreamReader&);
    bool zh_checkfilePath(const QString& path) const;


};
//==========================================================
#endif // ZXMLCALIBRATIONARRAYIOHANDLER_H
