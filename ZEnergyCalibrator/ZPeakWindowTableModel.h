//======================================================
#ifndef ZPEAKWINDOWTABLEMODEL_H
#define ZPEAKWINDOWTABLEMODEL_H
//======================================================
#include <QAbstractTableModel>
#include <QList>
//======================================================
class ZControlAction;
class ZPeakWindow;
//======================================================
class ZPeakWindowTableModel : public QAbstractTableModel
{
    Q_OBJECT
public:
    explicit ZPeakWindowTableModel(QObject *parent = 0);
    ~ZPeakWindowTableModel();
    // override FUNCS
    virtual Qt::ItemFlags	flags(const QModelIndex & index) const;
    QModelIndex index(int row, int column, const QModelIndex &parent) const;
    virtual int	columnCount(const QModelIndex & parent = QModelIndex()) const;
    virtual int	rowCount(const QModelIndex & parent = QModelIndex()) const;
    virtual QVariant	data(const QModelIndex & index, int role = Qt::DisplayRole) const;
    virtual bool setData(const QModelIndex & index, const QVariant & value, int role = Qt::EditRole);
    virtual QVariant	headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const;

    // custom FUNCS
    QList<ZControlAction*> zp_buttonActions() const;
    QList<ZControlAction*> zp_contextActions() const;

signals:

    void zg_inquiryCurrentVisibleSceneRect(QRectF& sceneRect) const;
    void zg_inquiryCurrentIndex(QModelIndex& index);

public slots:


protected slots:

    void zh_onAddPeakWindowAction();
    void zh_onRemovePeakWindowAction();

protected:

    // VARS
    ZControlAction* zv_addPeakWindowAction;
    ZControlAction* zv_removePeakWindowAction;
    QList<ZPeakWindow*> zv_peakWindowList;

    // FUNCS
    void zh_createActions();
    void zh_createConnections();

};
//======================================================
#endif // ZPEAKWINDOWTABLEMODEL_H
