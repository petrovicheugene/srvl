//============================================================
#ifndef ZBASETREEWIDGET_H
#define ZBASETREEWIDGET_H
//============================================================
#include <QWidget>
#include <QHeaderView>

//============================================================
class ZControlAction;

class QTreeView;
class ZSampleTaskTreeModel;
class QLabel;
class QHBoxLayout;
class QVBoxLayout;
//============================================================
class ZBaseTreeWidget : public QWidget
{
    Q_OBJECT
public:
    explicit ZBaseTreeWidget(QWidget *parent = 0);

    // FUNCS
    virtual void zp_setModel(ZSampleTaskTreeModel *model);
    void zp_setCaption(const QString& caption);
    void zp_appendButtonActions(const QList<ZControlAction*>& actionList);
    void zp_appendContextActions(const QList<ZControlAction*>& actionList);
    void zp_setSectionResizeMode(int section, QHeaderView::ResizeMode);
    void zp_setStretchLastSection(bool stretch);
    void zp_setHeaderVisible(bool visible);
    void zp_setSelectionBehavior(QAbstractItemView::SelectionBehavior behavior);
    void zp_setSelectionMode(QAbstractItemView::SelectionMode mode);

    QTreeView* zp_treeView() const;

signals:

public slots:



protected:

    // VARS
    QTreeView* zv_table; /*!< TODO: describe */
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
//============================================================
#endif // ZBASETREEWIDGET_H
