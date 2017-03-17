//===========================================================
#ifndef ZSTARTSTOPBUTTON_H
#define ZSTARTSTOPBUTTON_H
//===========================================================
#include <QWidget>

//===========================================================
class QButtonGroup;
class QPushButton;
class QLayout;
class QAbstractButton;
//===========================================================
class ZStartStopButtonWidget : public QWidget
{
    Q_OBJECT
public:
    explicit ZStartStopButtonWidget(Qt::Orientation orientation = Qt::Horizontal,
                                    QWidget *parent = 0);

    //void zp_setSizeHint(QSize);

protected:
//    void paintEvent(QPaintEvent* event);
//    QSize sizeHint() const;
//    void mouseReleaseEvent(QMouseEvent *event);
//    void mousePressEvent(QMouseEvent *event);

signals:

    void zg_start() const;
    void zg_stop() const;

public slots:

    void zp_setOrientation(Qt::Orientation orientation);

private slots:

    void zh_onButtonToggle(QAbstractButton* button, bool checked);

private:

    // VARS
    QButtonGroup* zv_buttonGroup;

    QPushButton* zv_startButton;
    QPushButton* zv_stopButton;

    // FUNS
    void zh_createComponents();
    void zh_createConnections();

//    enum ButtonState {BS_STOP,
//                      BS_STOP_DOWN,
//                      BS_START,
//                      BS_START_DOWN};

//    ButtonState zv_buttonState;
//    QSize zv_sizeHint;
};
//===========================================================
#endif // ZSTARTSTOPBUTTON_H
