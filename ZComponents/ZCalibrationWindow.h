//====================================================
#ifndef ZCALIBRATIONWINDOW_H
#define ZCALIBRATIONWINDOW_H
//====================================================
#include <QObject>
#include <QString>
#include <QMap>
#include <QPair>
//====================================================
class ZAbstractTerm;
class ZAbstractSpectrum;
//====================================================
/*!
 \brief

*/
class ZCalibrationWindow : public QObject
{
    Q_OBJECT
public:
    /*!
     \brief

    */
    enum WindowType{WT_NOT_DEFINED,
                    //WT_BASE_PEAK,
                    WT_PEAK,
                    WT_COHERENT,
                    WT_INCOHERENT};

    /*!
     \brief

     \param parent
    */
    explicit ZCalibrationWindow(QObject* parent);
    // explicit ZCalibrationWindow(const QString&,  WindowType = WT_NOT_DEFINED );
    /*!
     \brief

     \param
     \param WindowType
     \param firstChannel
     \param lastChannel
     \param parent
    */
    explicit ZCalibrationWindow(const QString&,
                                WindowType,
                                int firstChannel,
                                int lastChannel,
                                QObject* parent);

    /*!
     \brief

     \return bool
    */
    bool zp_isWindowVisible() const;
    /*!
     \brief

     \param bool
     \return bool
    */
    bool zp_setWindowVisible(bool);

    /*!
     \brief

     \return QString
    */
    QString zp_windowName() const;
    /*!
     \brief

     \param
     \return bool
    */
    bool zp_setWindowName(const QString&);

    /*!
     \brief

     \return int
    */
    int zp_firstChannel() const;
    /*!
     \brief

     \return int
    */
    int zp_lastChannel() const;

    /*!
     \brief

     \param channel
     \return bool
    */
    bool zp_setWindowFirstChannel(int channel);
    /*!
     \brief

     \param channel
     \return bool
    */
    bool zp_setWindowLastChannel(int channel);

    /*!
     \brief

     \param int
     \param int
     \return bool
    */
    bool  zp_setWindowMarginChannels(int, int);

    /*!
     \brief

     \return WindowType
    */
    WindowType zp_windowType() const;
    /*!
     \brief

     \param WindowType
     \return bool
    */
    bool zp_setWindowType(WindowType);
    /*!
     \brief

     \return qint64
    */
    qint64 zp_windowId() const;

    // STATIC FUNCS
    /*!
     \brief

     \param WindowType
     \return QString
    */
    static QString zp_displayTypeName(WindowType);
    /*!
     \brief

     \param WindowType
     \return QString
    */
    static QString zp_typeName(WindowType);
    /*!
     \brief

     \param
     \return WindowType
    */
    static WindowType zp_typeFromString(const QString& );

public slots:

    /*!
     \brief

     \param
     \param
     \param keepBufferClean
     \param ok
    */
    void zp_calcWindowIntensity(const QObject*, qreal &, bool keepBufferClean, bool *ok);

signals:

    /*!
     \brief

    */
    void zg_windowNameChanged() const;
    /*!
     \brief

    */
    void zg_windowMarginsChanged() const;
    /*!
     \brief

     \param previousType
     \param currentType
    */
    void zg_windowTypeChanged(ZCalibrationWindow::WindowType previousType, ZCalibrationWindow::WindowType currentType);

private:

    QString zv_windowName; /*!< TODO: describe */
    int zv_firstChannel; /*!< TODO: describe */
    int zv_lastChannel; /*!< TODO: describe */
    WindowType zv_type; /*!< TODO: describe */
    bool zv_visible; /*!< TODO: describe */
    qint64 zv_windowId; /*!< TODO: describe */

    // buffer
    qint64 zv_spectrumId; /*!< TODO: describe */
    qreal zv_windowIntensityValue; /*!< TODO: describe */

    // STATIC
    // VARS
    static qint64 zv_lastWindowId; /*!< TODO: describe */
    static QMap<WindowType, QPair<QString, QString>> zv_typeNameMap; /*!< TODO: describe */


    // FUNCS
    /*!
     \brief

     \return QMap<WindowType, QPair<QString, QString> >
    */
    static QMap<WindowType, QPair<QString, QString> > zh_intTypeNameMap();

};
//====================================================
#endif // ZCALIBRATIONWINDOW_H
