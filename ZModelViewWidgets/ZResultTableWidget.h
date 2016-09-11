//============================================================
#ifndef ZRESULTTABLEWIDGET_H
#define ZRESULTTABLEWIDGET_H
//============================================================
#include <QWidget>
//============================================================
class QTableView;
class QAction;
class QHBoxLayout;
class QVBoxLayout;

//============================================================
/*!
 \brief

*/
class ZResultTableWidget : public QWidget
{
    Q_OBJECT
public:
    /*!
     \brief

     \param parent
    */
    explicit ZResultTableWidget(QWidget *parent = 0);

signals:

public slots:

private:

    // VARS
    QTableView* zv_table; /*!< TODO: describe */
    QHBoxLayout* zv_buttonLayout; /*!< TODO: describe */
    QVBoxLayout* zv_mainLayout; /*!< TODO: describe */

    QList<QAction*> zv_contextMenuActionList; /*!< TODO: describe */

    // FUNCS
    /*!
     \brief

    */
    void zh_createComponents();
    /*!
     \brief

    */
    void zh_createConnections();


};
//============================================================
#endif // ZRESULTTABLEWIDGET_H
