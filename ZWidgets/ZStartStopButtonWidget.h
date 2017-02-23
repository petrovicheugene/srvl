//===========================================================
#ifndef ZSTARTSTOPBUTTON_H
#define ZSTARTSTOPBUTTON_H
//===========================================================
#include <QWidget>

//===========================================================
class QPushButton;
class QLayout;
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

public slots:

    void zp_setOrientation(Qt::Orientation orientation);

private:

    // VARS
    QPushButton* zv_startButton;
    QPushButton* zv_stopButton;

    // FUNS
    void zh_createComponents();


//    enum ButtonState {BS_STOP,
//                      BS_STOP_DOWN,
//                      BS_START,
//                      BS_START_DOWN};

//    ButtonState zv_buttonState;
//    QSize zv_sizeHint;
};
//===========================================================
#endif // ZSTARTSTOPBUTTON_H
