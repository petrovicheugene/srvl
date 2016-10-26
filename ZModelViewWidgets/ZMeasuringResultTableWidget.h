//============================================================
#ifndef ZCURRENTMEASUREMENTSAMPLETABLEWIDGET_H
#define ZCURRENTMEASUREMENTSAMPLETABLEWIDGET_H
//============================================================
#include "ZBaseTableWidget.h"
//============================================================
class QTableView;
class QAction;
class QHBoxLayout;
class QVBoxLayout;

//============================================================
/*!
 \brief

*/
class ZMeasuringResultTableWidget : public ZBaseTableWidget
{
    Q_OBJECT
public:
    /*!
     \brief

     \param parent
    */
    explicit ZMeasuringResultTableWidget(QWidget *parent = 0);

signals:

public slots:

private:

};
//============================================================
#endif // ZCURRENTMEASUREMENTSAMPLETABLEWIDGET_H
