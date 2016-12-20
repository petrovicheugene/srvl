//======================================================
#ifndef ZADVANCEDACTION_H
#define ZADVANCEDACTION_H
//======================================================
#include <QAction>
#include <QEvent>
//======================================================

//======================================================
class ZControlAction : public QAction
{
    Q_OBJECT
public:
    explicit ZControlAction(QObject *parent = 0);

signals:
    
    void zg_enableChanged(bool);

protected:

    // VARS
    bool zv_enabled;
    // FUNCS
    bool event(QEvent *event);


};
//======================================================
#endif // ZADVANCEDACTION_H
