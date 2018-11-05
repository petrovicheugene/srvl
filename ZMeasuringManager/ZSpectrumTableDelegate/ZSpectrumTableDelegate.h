//======================================================================
#ifndef ZSPECTRUMTABLEDELEGATE_H
#define ZSPECTRUMTABLEDELEGATE_H
//======================================================================
#include <QStyledItemDelegate>
//======================================================================
class ZPrimitivePlot;
//======================================================================
class ZSpectrumTableDelegate : public QStyledItemDelegate
{
    Q_OBJECT
public:
    explicit ZSpectrumTableDelegate(QObject *parent = 0);
    virtual ~ZSpectrumTableDelegate();

    virtual void paint ( QPainter * painter, const QStyleOptionViewItem & option, const QModelIndex & index ) const;
    virtual QSize	sizeHint ( const QStyleOptionViewItem & option, const QModelIndex & index ) const;

signals:


public slots:


private:

    // VARS
    // QWidget* zv_baseWidget;
    ZPrimitivePlot* zv_plot;

    // FUNCS
    bool editorEvent ( QEvent * event,
                       QAbstractItemModel * model,
                       const QStyleOptionViewItem & option,
                       const QModelIndex & index );

    bool eventFilter(QObject *object, QEvent *event);

};
//======================================================================
#endif // ZSPECTRUMTABLEDELEGATE_H
