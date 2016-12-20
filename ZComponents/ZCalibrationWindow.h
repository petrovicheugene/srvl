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
class ZCalibrationWindow : public QObject
{
    Q_OBJECT
public:
    enum WindowType{WT_NOT_DEFINED,
                    //WT_BASE_PEAK,
                    WT_PEAK,
                    WT_COHERENT,
                    WT_INCOHERENT};

    explicit ZCalibrationWindow(QObject* parent);
    // explicit ZCalibrationWindow(const QString&,  WindowType = WT_NOT_DEFINED );
    explicit ZCalibrationWindow(const QString&,
                                WindowType,
                                int firstChannel,
                                int lastChannel,
                                QObject* parent);

    bool zp_isWindowVisible() const;
    bool zp_setWindowVisible(bool);

    QString zp_windowName() const;
    bool zp_setWindowName(const QString&);

    int zp_firstChannel() const;
    int zp_lastChannel() const;

    bool zp_setWindowFirstChannel(int channel);
    bool zp_setWindowLastChannel(int channel);

    bool  zp_setWindowMarginChannels(int, int);

    WindowType zp_windowType() const;
    bool zp_setWindowType(WindowType);
    qint64 zp_windowId() const;

    // STATIC FUNCS
    static QString zp_displayTypeName(WindowType);
    static QString zp_typeName(WindowType);
    static WindowType zp_typeFromString(const QString& );

public slots:

    void zp_calcWindowIntensity(const QObject*, qreal &, bool keepBufferClean, bool *ok);

signals:

    void zg_windowNameChanged() const;
    void zg_windowMarginsChanged() const;
    void zg_windowTypeChanged(ZCalibrationWindow::WindowType previousType, ZCalibrationWindow::WindowType currentType);

private:

    QString zv_windowName;
    int zv_firstChannel;
    int zv_lastChannel;
    WindowType zv_type;
    bool zv_visible;
    qint64 zv_windowId;

    // buffer
    qint64 zv_spectrumId;
    qreal zv_windowIntensityValue;

    // STATIC
    // VARS
    static qint64 zv_lastWindowId;
    static QMap<WindowType, QPair<QString, QString>> zv_typeNameMap;


    // FUNCS
    static QMap<WindowType, QPair<QString, QString> > zh_intTypeNameMap();

};
//====================================================
#endif // ZCALIBRATIONWINDOW_H
