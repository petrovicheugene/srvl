//===========================================================
#ifndef ZDASHBOARD_H
#define ZDASHBOARD_H
//===========================================================
#include <QFrame>
//===========================================================
class QToolButton;
class ZPlotGraphicsView;
class ZPlotGraphicsScene;
class QHBoxLayout;
class QSlider;
//===========================================================
class ZHorizontalDashBoard : public QFrame
{
    Q_OBJECT
public:

    enum AuxWidgetPositon {AWP_LEFT_OF_BUTTONS,
                          AWP_RIGHT_OF_BUTTONS};

    explicit ZHorizontalDashBoard(QWidget *parent = 0);

    void zp_setPlotGraphicsView(ZPlotGraphicsView*);
    void zp_setPlotGraphicsScene(ZPlotGraphicsScene*);
    void zp_appendButtonsToDashboard(QList<QAction*>& actionList,
                                     AuxWidgetPositon position,
                                     Qt::AlignmentFlag alignent,
                                     int extraSperatorSpacing = 0);
    void zp_appendWidgetToDashboard(QWidget* widget,
                                    AuxWidgetPositon position,
                                    Qt::AlignmentFlag alignment,
                                    int extraSeparatorSpacing = 0);
    int zp_distortionValue();
    void zp_setDistortionSliderVisible(bool visible);


signals:

    void zg_distortionFactorChanged(int);

public slots:

protected:
    // FUNCS
    void timerEvent(QTimerEvent* event);

private:

    // VARS
    QHBoxLayout* zv_mainLayout;
    QHBoxLayout* zv_leftLayout;

    QToolButton* zv_hideGridButton;
    QToolButton* zv_increaseVerticalButton;
    QToolButton* zv_increaseHorizontalButton;
    QToolButton* zv_decreaseVerticalButton;
    QToolButton* zv_decreaseHorizontalButton;
    QToolButton* zv_fitViewInSceneButton;

    QSlider* zv_distortionSlider;


    ZPlotGraphicsView* zv_plotGraphicsView;
    ZPlotGraphicsScene* zv_plotGraphicsScene;
    int zv_timer;
    Qt::Orientation zv_orientation;
    bool zv_increase;
    // FUNCS
    void zh_createComponents();
    void zh_createConnections();
    void zh_startTimer();

private slots:

    void zh_increaseVertical();
    void zh_increaseHorizontal();
    void zh_decreaseVertical();
    void zh_decreaseHorizontal();
    void zh_fitViewInScene() const;
    void zh_hideGrid(bool) const;
    void zh_killTimer();


};
//===========================================================
#endif // ZDASHBOARD_H
