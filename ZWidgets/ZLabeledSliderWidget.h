//============================================================
#ifndef ZLABELEDSLIDERWIDGET_H
#define ZLABELEDSLIDERWIDGET_H
//============================================================
#include <QWidget>
#include <QSlider>
//============================================================
class QSlider;
class QLabel;
//============================================================
class ZLabeledSliderWidget : public QWidget
{
    Q_OBJECT
public:
    explicit ZLabeledSliderWidget(Qt::Orientation orientation,
                                  QWidget *parent = 0);

    void zp_setMinimumCaption(const QString& caption);
    void zp_setMaximumCaption(const QString& caption);
    void zp_setTextColor(QColor color);
    void zp_setFontBold(bool bolsFlag);
    void zp_setFontPixelSize(int);
    void zp_setTickPosition(QSlider::TickPosition tickPosition);
    void zp_setOrientation(Qt::Orientation orientation);
    void zp_setMinimum(int value);
    void zp_setMaximum(int value);
    void zp_setTextHTMLColorString(const QString& htmlColorString);
    int zp_value() const;
    void zp_setValue(int value);
    QSlider* zp_slider() const;


signals:

    void zg_valueChanged(int value);

public slots:


private:

    // VARS
    QSlider* zv_slider;
    QLabel* zv_minimumLabel;
    QLabel* zv_maximumLabel;
    QString zv_htmlColorString;
    // FUNCS

    void zh_createComponents();
    void zh_createConnctions();
    void zh_rebuildLayout();
};
//============================================================
#endif // ZLABELEDSLIDERWIDGET_H
