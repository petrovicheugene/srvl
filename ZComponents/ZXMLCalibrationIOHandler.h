//==========================================================
#ifndef ZXMLCALIBRATIONARRAYIOHANDLER_H
#define ZXMLCALIBRATIONARRAYIOHANDLER_H
//==========================================================
#include <QObject>
#include <QFile>
#include <QStack>
#include "ZCalibration.h"
#include "ZGLConstantsAndDefines.h"
//==========================================================
class QXmlStreamReader;
//==========================================================
/*!
 \brief

*/
class ZXMLCalibrationIOHandler : public QObject
{
    Q_OBJECT
public:
    explicit ZXMLCalibrationIOHandler(/*QObject* calibrationParent,*/ QObject *parent = 0);
    virtual ~ZXMLCalibrationIOHandler();

    static QString zp_getCalibrationOpenFile(const QString& calibrationFolderPath);
    /*!
     \brief

     \param calibrationFolderPath
     \return QStringList
    */
    static QStringList zp_getCalibrationOpenFiles(const QString& calibrationFolderPath);
    /*!
     \brief

     \param calibrationFolderPath
     \return QString
    */
    static QString zp_getCalibrationSaveFile(const QString& calibrationFolderPath);
    /*!
     \brief

     \param calibrationFolderPath
     \return QString
    */
    static QString zp_checkDirPath(const QString& calibrationFolderPath);

    /*!
     \brief

     \param
     \param
     \return bool
    */
    bool zp_getCalibrationFromFile(QFile&, ZCalibration*&);
    /*!
     \brief

     \param file
     \param calibration
     \return bool
    */
    bool zp_writeCalibrationToFile(QFile&file, const ZCalibration*calibration);

    /*!
     \brief

     \return QString
    */
    QString zp_message() const;

public slots:


signals:

    /*!
     \brief

     \param
    */
    void zg_message(const QString&);

private:

    // VARS
    // QObject* zv_calibrationParent;
    // QString parentTagName;
    QStack<QString> parentTagStack; /*!< TODO: describe */
    ZRawTerm zv_rawTerm; /*!< TODO: describe */
    ZRawWindow zv_rawWindow; /*!< TODO: describe */
    QString zv_message; /*!< TODO: describe */

    const QString zv_magicString = glAppCompany + " 7D385RTNOW9SH31JZQL"; /*!< TODO: describe */
    // elment names
    const QString zv_ROOT = "root"; /*!< TODO: describe */
    const QString zv_TYPE = "type"; /*!< TODO: describe */
    const QString zv_STATE = "state"; /*!< TODO: describe */

    const QString zv_DATE_TIME = "date-time"; /*!< TODO: describe */
    const QString zv_CHEMELEMENT = "chem_element"; /*!< TODO: describe */
    const QString zv_DETERMINATION_R2 = "R2"; /*!< TODO: describe */
    const QString zv_ADJUSTED_DETERMINATION_R2ADJ = "R2adj"; /*!< TODO: describe */
    const QString zv_STANDARD_DEVIATION = "s"; /*!< TODO: describe */

    const QString zv_WINDOW_LIST = "window_list"; /*!< TODO: describe */
    const QString zv_WINDOW = "window"; /*!< TODO: describe */
    const QString zv_NAME = "name"; /*!< TODO: describe */
    const QString zv_FIRST_CHANNEL = "first_channel"; /*!< TODO: describe */
    const QString zv_LAST_CHANNEL = "last_channel"; /*!< TODO: describe */

    const QString zv_TERM_LIST = "term_list"; /*!< TODO: describe */
    const QString zv_TERM = "term"; /*!< TODO: describe */
    const QString zv_TERM_WINDOW = "term_window"; /*!< TODO: describe */
    const QString zv_FACTOR = "factor"; /*!< TODO: describe */

    const QString zv_EQUATION = "equation"; /*!< TODO: describe */
    const QString zv_POLYNOMIAL = "polynomial"; /*!< TODO: describe */
    const QString zv_FRACTIONAL = "fractional"; /*!< TODO: describe */
    const QString zv_BASE_TERM = "base_term"; /*!< TODO: describe */
    const QString zv_NORMALIZER = "normalizer"; /*!< TODO: describe */
    const QString zv_FRACTIONAL_BASE_NORMALIZER = "fractional_base_normalizer"; /*!< TODO: describe */

    const QString zv_CUSTOM_STRING = "custom_string"; /*!< TODO: describe */
    const QString zv_INTERCEPT = "intercept"; /*!< TODO: describe */

    const QString zv_YES = "yes"; /*!< TODO: describe */
    const QString zv_NO = "no"; /*!< TODO: describe */

    const QString zv_dateTimeFormat = "dd.MM.yyyy hh:mm:ss"; /*!< TODO: describe */

    // FUNCS
    /*!
     \brief

     \param reader
     \param magicStringDetectionFlag
     \return bool
    */
    bool zh_detectRoot(const QXmlStreamReader& reader, bool& magicStringDetectionFlag) const;
    /*!
     \brief

     \param
     \param
    */
    void zh_parseXMLElement(ZCalibration*,
                            QXmlStreamReader&);
    /*!
     \brief

     \param path
     \return bool
    */
    bool zh_checkfilePath(const QString& path) const;


};
//==========================================================
#endif // ZXMLCALIBRATIONARRAYIOHANDLER_H
