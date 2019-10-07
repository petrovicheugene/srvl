//=========================================================
#ifndef ZDEPENDENTMODELCONTROLLER_H
#define ZDEPENDENTMODELCONTROLLER_H
//=========================================================
#include <QObject>
//=========================================================
class QAbstractItemView;
class ZDependentModel;
//=========================================================
class ZDependentModelController : public QObject
{
    Q_OBJECT
public:
    explicit ZDependentModelController(QObject *parent = nullptr);

    void zp_setView(QAbstractItemView *view);
    void zp_setModel(ZDependentModel *model);

signals:

    void zg_currentId(qint64 id) const;

public slots:


protected slots:

    void zh_onCurrentIndexChange(const QModelIndex& index);

private:

    // VARS
    qint64 zv_id;

};
//=========================================================
#endif // ZDEPENDENTMODELCONTROLLER_H
