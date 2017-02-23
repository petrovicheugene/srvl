//====================================================
#ifndef ZSERIESLABELWIDGET_H
#define ZSERIESLABELWIDGET_H
//====================================================
#include <QWidget>
#include <ZDashboardSettings.h>

//====================================================
class QLabel;
//====================================================
class ZSeriesLabelWidget : public QWidget
{
    Q_OBJECT
public:
    explicit ZSeriesLabelWidget(Qt::Orientation orientation = Qt::Horizontal,
                                QWidget *parent = 0);

    void zp_applyDashBoardSettings(const ZDashboardSettings& settings);


signals:

public slots:

    void zp_setSeriesName(const QString& seriesName);
    void zp_setSeriesDirty(bool dirty);

private:

    // VARS
    QLabel* zv_seriesLabel;
    QString zv_seriesNameString;

    // FUNCS
    void zh_createComponents();

};
//====================================================
#endif // ZSERIESLABELWIDGET_H
