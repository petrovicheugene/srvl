//===========================================================
#ifndef ZCOLORLABEL_H
#define ZCOLORLABEL_H
//===========================================================
#include <QWidget>
#include <QColor>
//===========================================================

//===========================================================
class ZColorLabel : public QWidget
{
    Q_OBJECT
public:
    explicit ZColorLabel(QWidget *parent = 0);

    void zp_setSizeHint(int);
    void zp_setColor(QColor);
    QColor zp_color() const;

public slots:

    void zp_runColorSelectionDialog();

protected:

    // FUNCS
    void paintEvent(QPaintEvent* event);
    QSize sizeHint() const;
    void mouseReleaseEvent(QMouseEvent *event);


    // VARS
    QSize zv_sizeHint;
    QColor zv_color;

signals:

    void zg_colorChanged();

public slots:
};
//===========================================================
#endif // ZCOLORLABEL_H
