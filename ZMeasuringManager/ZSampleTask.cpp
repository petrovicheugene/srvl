//=================================================
#include "ZSampleTask.h"
#include "ZGLConstantsAndDefines.h"
//=================================================
bool ZSampleTask::zp_instanceSampleTaskObject(int sampleTaskId,
                                              ZSampleTask*& task,
                                              QString& msg,
                                              QObject* parent)
{
    msg.clear();
    if(sampleTaskId < 0 || parent == 0)
    {
        task = 0;
        msg = "Invalid sample task id.";
        return false;
    }

    task = new ZSampleTask(parent);
    task->zv_id = sampleTaskId;
    task->zv_name = QString("TASK %1").arg(QString::number(sampleTaskId));
    return true;
}
//=================================================
ZSampleTask::ZSampleTask(QObject *parent) : QObject(parent)
{

}
//=================================================
ZSampleTask::~ZSampleTask()
{
    qDebug() << QString("Deleted: %1").arg(zv_name);
}
//=================================================
int ZSampleTask::zp_id() const
{
    return zv_id;
}
//=================================================
QString ZSampleTask::zp_name() const
{
    return zv_name;
}
//=================================================
void ZSampleTask::zp_appendClient(QObject *client)
{
    if(zv_clientList.contains(client))
    {
        return;
    }

    zv_clientList.append(client);
    qDebug() << QString("Appended client to %1: ").arg(zv_name);

}
//=================================================
void ZSampleTask::zp_removeClient(QObject* client )
{
    if(!zv_clientList.contains(client))
    {
        return;
    }

    zv_clientList.removeAt(zv_clientList.indexOf(client));
    qDebug() << QString("Removed client from %1: ").arg(zv_name);

    if(zv_clientList.count() < 1)
    {
        emit zg_requestToDelete();
    }
}
//=================================================
