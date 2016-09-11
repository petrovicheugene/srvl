//=======================================================
#ifndef ZABSTRACTPROCESSTIMEINDICATOR_H
#define ZABSTRACTPROCESSTIMEINDICATOR_H
//=======================================================
#include <QWidget>
#include <QColor>
#include <ZProgressBarOptions.h>
//=======================================================

//=======================================================
class ZAbstractProcessTimeIndicator : public QWidget
{
    Q_OBJECT
public:
    explicit ZAbstractProcessTimeIndicator(QWidget *parent = 0);

    virtual void zp_setMainLayoutMargin(int margin);
    virtual void zp_setMainLayoutSpacing(int spacing);
    /*!
     \brief

     \param bool
    */
    virtual void zp_setPercentTextOnProgressBar(bool) = 0;
    virtual void zp_applyProgressBarOptions(const ZProgressBarOptions& options) = 0;


signals:

public slots:

protected:


};
//=======================================================
#endif // ZABSTRACTPROCESSTIMEINDICATOR_H
