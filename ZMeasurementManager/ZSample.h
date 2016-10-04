//=====================================================
#ifndef ZSAMPLE_H
#define ZSAMPLE_H
//=====================================================
#include <QObject>
//=====================================================


//=====================================================
class ZSample : public QObject
{
    Q_OBJECT
public:
    explicit ZSample(const QString& sampleName,
                     QObject *parent);

    QString zp_sampleName() const;
    bool zp_setSampleName(const QString& name);

signals:

public slots:

private:

    // VARS
    QString zv_sampleName;

};
//=====================================================
#endif // ZSAMPLE_H
