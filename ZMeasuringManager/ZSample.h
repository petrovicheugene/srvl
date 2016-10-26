//=====================================================
#ifndef ZSAMPLE_H
#define ZSAMPLE_H
//=====================================================
#include <QObject>
//=====================================================
class ZSampleTask;
//=====================================================
class ZSample : public QObject
{
    Q_OBJECT
public:
    explicit ZSample(const QString& sampleName,
                     ZSampleTask* sampleTask,
                     QObject *parent);
    ~ZSample();

    QString zp_sampleName() const;
    bool zp_setSampleName(const QString& name);

    QString zp_sampleTaskName() const;


signals:

public slots:

private:

    // VARS
    QString zv_sampleName;
    ZSampleTask* zv_sampleTask;



};
//=====================================================
#endif // ZSAMPLE_H
