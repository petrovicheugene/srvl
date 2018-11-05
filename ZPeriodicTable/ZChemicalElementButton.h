//======================================================
#ifndef ZCHEMICALELEMENTBUTTON_H
#define ZCHEMICALELEMENTBUTTON_H
//======================================================
#include <QToolButton>

#include <QMap>
//======================================================
class QFrame;
class QPaintEvent;
class QResizeEvent;
//======================================================
typedef  unsigned int ZNumber;
//======================================================
class ZChemicalElementButton : public QToolButton
{
    Q_OBJECT
public:
    explicit ZChemicalElementButton(ZNumber zNumber, QWidget *parent = nullptr);
    ~ZChemicalElementButton();

    enum PropertyName {PN_SYMBOL,
                       PN_ZNUMBER,
                       PN_NAME};

    static QSize zp_symbolBoundingRectSize();
    static QSize zp_zNumberBoundingRectSize();
    static QSize zp_nameBoundingRectSize();
    static void zp_setSymbolBoundingRectSize(QSize size);
    static void zp_setZNumberBoundingRectSize(QSize size);
    static void zp_setNameBoundingRectSize(QSize size);
    static int zp_smallFontSizeRatio();
    static bool zp_setSmallFontSizeRatio(int);

    ZNumber zp_zNumber() const;

    void zp_setChemicalElementSymbol(const QString& symbol);
    QString zp_chemicalElementSymbol() const;

    void zp_setChemicalElementName(const QString& name);
    QString zp_chemicalElementName() const;

    void zp_updatePropertyCaptions();

    QRect zp_buttonViewPortRect() const;
    QSize zp_symbolWindowRectSize() const;
    QSize zp_zNumberWindowRectSize() const;
    QSize zp_nameWindowRectSize() const;

    void zp_updateMinimumSizeForSymbolRectSize(QSize symbolSize);
    void zp_setPropertyFont(PropertyName proprtyName, QFont font);

signals:

    void zg_symbolChanged(const QString& symbol);

public slots:


protected:

    void resizeEvent(QResizeEvent* event);
    void paintEvent(QPaintEvent *event);

private slots:

    void zh_onAppFontDatabaseChange();

private:

    // VARS
    static QSize zv_symbolBoundingRectSize;
    static QSize zv_zNumberBoundingRectSize;
    static QSize zv_nameBoundingRectSize;
    static double zv_smallFontSizeRatio;

    //QMap<PropertyName, QColor> zv_colorMap;
    QMap<PropertyName, QFont> zv_fontMap;

    QFrame* zv_frame;
    int zv_frameLineWidth;
    int zv_margin;

    ZNumber zv_zNumber;
    QString zv_symbol;
    QString zv_name;

    QRect zv_symbolViewPortRect;
    QRect zv_symbolWindowRect;

    QRect zv_zNumberViewPortRect;
    QRect zv_zNumberWindowRect;

    QRect zv_nameViewPortRect;
    QRect zv_nameWindowRect;


    // FUNCS
    void zh_setDefaultFonts();
    void zh_setDefaultColors();
    void zh_createConnections();

    void zp_updatePropertyViewPortRects();
    void zh_recalcPropertyWindowRects();
    void zh_recalcViewPortRectSize(QSize boundingViewPortSize,
                                   QSize windowSize,
                                   QSize &viewPortSize) const;
    int zh_marginSpacing() const;

    QFont zh_symbolFont() const;
    QFont zh_zNumberFont() const;
    QFont zh_nameFont() const;

    QColor zh_textColor() const;
    QPalette zh_currentPalette() const;

};
//======================================================
#endif // ZCHEMICALELEMENTBUTTON_H
