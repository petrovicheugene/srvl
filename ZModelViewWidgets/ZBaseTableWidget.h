//=========================================================================
#ifndef ZBASETABLEWIDGET_H
#define ZBASETABLEWIDGET_H
//=========================================================================
#include <QWidget>
//=========================================================================
class QAbstractItemModel;
class QTableView;
class ZControlAction;
class QLabel;

class QHBoxLayout;
class QVBoxLayout;

//=========================================================================
class ZBaseTableWidget : public QWidget
{
    Q_OBJECT
public:
    explicit ZBaseTableWidget(QWidget *parent = 0);

    virtual void zp_setModel(QAbstractItemModel* model);
    void zp_setCaption(const QString& caption);

    void zp_appendButtonActions(const QList<ZControlAction*>& actionList);
    void zp_appendContextActions(const QList<ZControlAction*>& actionList);

    QList<int> zp_selectedRowList();


signals:

public slots:


protected:

    // VARS
    QTableView* zv_table; /*!< TODO: describe */
    QHBoxLayout* zv_buttonLayout; /*!< TODO: describe */
    QVBoxLayout* zv_mainLayout; /*!< TODO: describe */
    QLabel* zv_captionLabel;
    QList<ZControlAction*> zv_contextMenuActionList; /*!< TODO: describe */

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
//=========================================================================
#endif // ZBASETABLEWIDGET_H
