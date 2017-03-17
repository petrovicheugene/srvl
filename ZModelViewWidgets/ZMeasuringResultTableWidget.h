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

    void zg_currentSampleIndexChanged(int currentSampleIndex) const;

public slots:

private slots:

    void zh_onCurrentIndexChanged(const QModelIndex& current,
                                  const QModelIndex& previous);

private:

    // VARS


    // FUNCS

};
//============================================================
#endif // ZCURRENTMEASUREMENTSAMPLETABLEWIDGET_H
