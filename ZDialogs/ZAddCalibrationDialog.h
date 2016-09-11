//=========================================================
#ifndef ZADDCALIBRATIONDIALOG_H
#define ZADDCALIBRATIONDIALOG_H
//=========================================================
#include <QDialog>
//=========================================================
class QPushButton;
class QLineEdit;
class QDoubleSpinBox;
class ZCalibration;
//=========================================================
/*!
 \brief

*/
class ZAddCalibrationDialog : public QDialog
{
    Q_OBJECT
public:
    /*!
     \brief

     \param parent
    */
    explicit ZAddCalibrationDialog(QWidget *parent = 0);
    /*!
     \brief

    */
    virtual ~ZAddCalibrationDialog();

    /*!
     \brief

     \return QString
    */
    QString zp_calibrationFilePath() const;
    /*!
     \brief

     \return QString
    */
    QString zp_calibrationName() const;
    /*!
     \brief

     \return QString
    */
    QString zp_chemicalElement() const;
    /*!
     \brief

     \return qreal
    */
    qreal zp_rangeMinMargin() const;
    /*!
     \brief

     \return qreal
    */
    qreal zp_rangeMaxMargin() const;

signals:

public slots:

private slots:

    /*!
     \brief

    */
    void zh_onBrowseClick();
    /*!
     \brief

     \param double
    */
    void zh_onRangeChange(double);
    /*!
     \brief

    */
    void zh_onOkClick();

private:

    // VARS
    // settings names
    const QString zv_settingsGroupName = "addCalibrationDialog"; /*!< TODO: describe */
    const QString zv_fileLacationDirSectionName = "fileLocationDir"; /*!< TODO: describe */

    ZCalibration* zv_calibration; /*!< TODO: describe */
    QString zv_fileLocationDirString; /*!< TODO: describe */
    QLineEdit* zv_filePathLineEdit; /*!< TODO: describe */
    QLineEdit* zv_calibrationNameLineEdit; /*!< TODO: describe */

    QLineEdit* zv_chemElementLineEdit; /*!< TODO: describe */
    QDoubleSpinBox* zv_rangeMinMarginSpinBox; /*!< TODO: describe */
    QDoubleSpinBox* zv_rangeMaxMarginSpinBox; /*!< TODO: describe */

    QPushButton* zv_okButton; /*!< TODO: describe */
    QPushButton* zv_cancelButton; /*!< TODO: describe */
    QPushButton* zv_browseButton; /*!< TODO: describe */

    double zv_rangePreviousMinValue; /*!< TODO: describe */
    double zv_rangePreviousMaxValue; /*!< TODO: describe */

    // FUNCS
    /*!
     \brief

    */
    void zh_createComponents();
    /*!
     \brief

    */
    void zh_createConnections();
    /*!
     \brief

    */
    void zh_restoreSettings();
    /*!
     \brief

    */
    void zh_saveSettings();

    /*!
     \brief

     \param fileName
     \param msg
     \return bool
    */
    bool zh_createCalibrationFromFile(const QString& fileName, QString& msg);
    /*!
     \brief

    */
    void zh_displayCalibrationProperties();
};
//=========================================================
#endif // ZADDCALIBRATIONDIALOG_H
