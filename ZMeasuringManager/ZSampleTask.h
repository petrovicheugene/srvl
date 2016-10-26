//=================================================
#ifndef ZSAMPLETASK_H
#define ZSAMPLETASK_H
//=================================================
#include <QObject>
#include <QList>
//=================================================

//=================================================
class ZSampleTask : public QObject
{
    Q_OBJECT
public:

    ~ZSampleTask();
    int zp_id() const;
    QString zp_name() const;


    // STATIC FUNCS
    static bool zp_instanceSampleTaskObject(int sampleTaskId,
                                              ZSampleTask*& task,
                                              QString& msg,
                                              QObject *parent);

    void zp_appendClient(QObject *client);
    void zp_removeClient(QObject *client);

signals:

    void zg_requestToDelete();

public slots:



private:

    explicit ZSampleTask(QObject *parent = 0);

    // VARS
    int zv_id;
    QString zv_name;
    QList<QObject*> zv_clientList;

};
//=================================================
#endif // ZSAMPLETASK_H
