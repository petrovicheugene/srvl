//======================================================
#ifndef ZDEPENDENDMODEL_H
#define ZDEPENDENDMODEL_H
//======================================================
#include <QAbstractTableModel>
//======================================================

//======================================================
class ZDependentModel : public QAbstractTableModel
{
    Q_OBJECT
public:
    explicit ZDependentModel(QObject *parent = nullptr);

signals:

public slots:

    virtual void zp_reload(qint64 id) = 0;


};
//======================================================
#endif // ZDEPENDENDMODEL_H
