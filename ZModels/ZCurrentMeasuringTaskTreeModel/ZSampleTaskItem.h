//=================================================================
#ifndef ZSAMPLEITEM_H
#define ZSAMPLEITEM_H
//=================================================================
#include "ZItem.h"
#include "ZSeriesTaskTreeModel.h"
//=================================================================

//=================================================================
class ZSampleTaskItem : public ZItem
{
    Q_OBJECT
public:
    ZSampleTaskItem(const QString& name,
                    const QString& sampleTaskName,
                    ZSeriesTaskTreeModel* parent);

    QString zp_displayString() const;

    QString zp_sampleName() const;
    bool zp_setSampleName(const QString& name);

    bool zp_setSampleTaskName(const QString& sampleTaskName);
    QString zp_sampleTaskName();

private:

    // VARS
    QString zv_sampleName;
    QString zv_sampleTaskName;


};
//=================================================================
#endif // ZSAMPLEITEM_H
