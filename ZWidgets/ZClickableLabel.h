//=================================================
#ifndef ZCLICKABLELABEL_H
#define ZCLICKABLELABEL_H
//=================================================
#include <QLabel>
//=================================================

//=================================================
class ZClickableLabel : public QLabel
{
    Q_OBJECT
public:
    explicit ZClickableLabel(QWidget *parent);


signals:

    void clicked();

protected:

    void mouseReleaseEvent(QMouseEvent*);

};
//=================================================
#endif // ZCLICKABLELABEL_H
