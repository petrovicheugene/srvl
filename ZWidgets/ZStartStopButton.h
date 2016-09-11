//===========================================================
#ifndef ZSTARTSTOPBUTTON_H
#define ZSTARTSTOPBUTTON_H
//===========================================================
#include <QWidget>

//===========================================================


//===========================================================
class ZStartStopButton : public QWidget
{
    Q_OBJECT
public:
    explicit ZStartStopButton(QWidget *parent = 0);

    void zp_setSizeHint(QSize);

protected:
    void paintEvent(QPaintEvent* event);
    QSize sizeHint() const;
    void mouseReleaseEvent(QMouseEvent *event);
    void mousePressEvent(QMouseEvent *event);

signals:

public slots:




private:

    // VARS
    enum ButtonState {BS_STOP,
                      BS_STOP_DOWN,
                      BS_START,
                      BS_START_DOWN};

    ButtonState zv_buttonState;
    QSize zv_sizeHint;
};
//===========================================================
#endif // ZSTARTSTOPBUTTON_H
