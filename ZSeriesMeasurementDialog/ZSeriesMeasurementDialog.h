//======================================================
#ifndef ZSERIESMEASUREMENTDIALOG_H
#define ZSERIESMEASUREMENTDIALOG_H
//======================================================
#include "ZBaseDialog.h"

#include <QApplication>
#include <QStyledItemDelegate>
#include <QTime>
//======================================================
class QPushButton;
class QSplitter;
class QSqlTableModel;
class QSqlRelationalTableModel;
class QTableView;

class ZPlotter;
class ZSeriesTableModel;
class ZDependentModelController;
//======================================================
class ZTimeDelegate : public QStyledItemDelegate // displays time w/o msecs
{
    Q_OBJECT
public:

    explicit ZTimeDelegate(QString format = "hh:mm:ss", QObject* parent = nullptr)
        : QStyledItemDelegate(parent)
    {
        zv_format = format;
    }

    void zp_setFormat(const QString& format)
    {
        zv_format = format;
    }

protected:

    void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const override
    {
        QStyleOptionViewItem newOption(option);
        //newOption.state =  newOption.state | QStyle::State_Active;
        initStyleOption(&newOption, index);
        QVariant vData = index.data();
        if(vData.isValid() && vData.canConvert<QTime>())
        {
            newOption.text = vData.toTime().toString(zv_format);
        }

        const QWidget *widget = option.widget;
        QStyle *style = widget ? widget->style() : QApplication::style();

        style->drawControl(QStyle::CE_ItemViewItem, &newOption, painter, widget);
    }

private:
    //VARS
    QString zv_format;

};
//======================================================
class ZSeriesMeasurementDialog : public ZBaseDialog
{
    Q_OBJECT
public:
    explicit ZSeriesMeasurementDialog(QWidget *parent = nullptr);
    ~ZSeriesMeasurementDialog() override;

signals:

public slots:


private slots:

    void zh_onCloseButtonClick();
    void zh_onResetSortButtonClick();
    void zh_onModelReset();
    void zh_onVisibleSpectrumChange(bool visible);
    void zh_onCurrentChange(const QModelIndex &current, const QModelIndex &previous);

protected:

    void zh_saveSettings() override;
    void zh_restoreSettings() override;

private:

    // VARS
    QSqlRelationalTableModel* zv_seriesListTableModel;
    ZSeriesTableModel* zv_seriesResultModel;
    ZDependentModelController* zv_seriesTableModelController;

    QSplitter* zv_mainSplitter;
    QSplitter* zv_tableSplitter;
    const QString zv_mainSplitterStateName = "mainSplitterState";
    const QString zv_tableSplitterStateName = "tableSplitterState";


    double zv_boundingRectTopFactor;
    QTableView* zv_seriesListView;
    QTableView* zv_seriesResultView;
    ZPlotter* zv_plotter;

    QPushButton* zv_filterButton;
    QPushButton* zv_sortButton;

    QPushButton* zv_closeButton;

    // FUNCS
    void zh_createComponents();
    void zh_createConnections();
    QWidget* zh_createSeriesListViewWidget();
    QWidget* zh_createTablesWidget();

};
//======================================================
#endif // ZSERIESMEASUREMENTDIALOG_H
