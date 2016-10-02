//=======================================================
#ifndef ZSIDEAREDWIDGET_H
#define ZSIDEAREDWIDGET_H
//=======================================================
#include <QFrame>
//=======================================================
class QSplitter;
class QLabel;
class ZClickableLabel;
class QVBoxLayout;
//=======================================================
class ZWidgetWithSidebar : public QFrame
{
    Q_OBJECT
public:
    explicit ZWidgetWithSidebar(const QString& objectName,
                                bool sideBarOnLeft,
                                QWidget *parent);
    explicit ZWidgetWithSidebar(const QString& title,
                                const QString& objectName,
                                bool sideBarIsOnLeft,
                                QWidget *parent);

    bool zp_setMainWidget(QWidget*);
    bool zp_setSidebarWidget(QWidget*);

    void zp_setMargin(int margin);
    void zp_insertSpacingUpperHideLabel(int spacing);

signals:

public slots:

    void zp_setInfoLabelText(bool, const QString&);
    void zp_saveSettings();

private:

    // VARS
    QVBoxLayout* zv_mainLayout;
    int zv_margin;
    bool zv_sideBarOnLeft;

    QSplitter* zv_splitter;
    ZClickableLabel* zv_hideLabel;
    QLabel* zv_infoLabel;
    QWidget* zv_mainWidget;
    QWidget* zv_sideBarWidget;

    QPixmap zv_closePixmap;
    QPixmap zv_openPixmap;
    QString hideToolTipString;
    QString showToolTipString;

    // FUNCS
    void zh_createComponents(QString title = QString());
    void zh_createConnections();
    void zh_restoreSettings();

    void zh_manageHideLabelPix();

private slots:

    void zh_onHideSidePanelButtonClick();

};
//=======================================================
#endif // ZSIDEAREDWIDGET_H
