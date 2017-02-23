//=======================================================
#include "ZWidgetWithSidebar.h"
#include "ZClickableLabel.h"
#include "ZGeneral.h"

#include <QCloseEvent>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QSplitter>
#include <QLabel>
#include <QSettings>
//=======================================================
ZWidgetWithSidebar::ZWidgetWithSidebar(const QString &objectName,
                                       bool sideBarOnLeft,
                                       QWidget *parent) : QFrame(parent)
{
    zv_sideBarOnLeft = sideBarOnLeft;
    zv_sideBarWidget = 0;
    zv_mainWidget = 0;
    hideToolTipString = zv_sideBarOnLeft? tr("Hide left panel") : tr("Hide right panel");
    showToolTipString = zv_sideBarOnLeft? tr("Show left panel") : tr("Show right panel");

    setObjectName(objectName);
    zh_createComponents();
    zh_createConnections();
    // zh_restoreState();

}
//=======================================================
ZWidgetWithSidebar::ZWidgetWithSidebar(const QString &title,
                                       const QString &objectName,
                                       bool sideBarIsOnLeft,
                                       QWidget *parent) : QFrame(parent)
{
    zv_sideBarWidget = 0;
    zv_mainWidget = 0;
    setObjectName(objectName);
    zh_createComponents(title);
    zh_createConnections();
    // zh_restoreState();

}
//=======================================================
bool ZWidgetWithSidebar::zp_setMainWidget(QWidget* mainWidget)
{
    if(mainWidget == 0 || zv_mainWidget != 0 || !(zv_splitter->count() < 2))
    {
        return false;
    }
    zv_mainWidget = mainWidget;
    int widgetIndex = zv_sideBarOnLeft? zv_splitter->count() : 0;

    zv_splitter->insertWidget(widgetIndex, zv_mainWidget);
    zv_splitter->setCollapsible(widgetIndex, false);
    zh_restoreSettings();
    return true;
}
//=======================================================
bool ZWidgetWithSidebar::zp_setSidebarWidget(QWidget* sideBarWidget)
{
    if(sideBarWidget == 0 || zv_sideBarWidget != 0 || !(zv_splitter->count() < 2))
    {
        return false;
    }

    zv_sideBarWidget = sideBarWidget;
    int widgetIndex = zv_sideBarOnLeft? 0 : zv_splitter->count();
    zv_splitter->insertWidget(widgetIndex, zv_sideBarWidget);
    zv_splitter->setCollapsible(widgetIndex, false);

    zv_hideLabel->setPixmap(zv_closePixmap); // QPixmap(":/images/ZWidgets/sidebarRightBlueClose.png"));
    zv_hideLabel->setVisible(true);
    zv_hideLabel->setToolTip(hideToolTipString);
    zh_restoreSettings();
    return true;
}
//=======================================================
void ZWidgetWithSidebar::zp_setMargin(int margin)
{
    zv_mainLayout->setMargin(margin);
}
//=======================================================
void ZWidgetWithSidebar::zp_insertSpacingUpperHideLabel(int spacing)
{
    zv_mainLayout->insertSpacing(1, spacing);
}
//=======================================================
void ZWidgetWithSidebar::zp_setInfoLabelText(bool dirty, const QString& text)
{
    QString infoText = text;
    if(dirty)
    {
        infoText += "*";
        infoText = "<font color=red><b>" + infoText + "</b></font>";
    }
    zv_infoLabel->setText(infoText);
}
//=======================================================
void ZWidgetWithSidebar::zh_createComponents(QString title)
{
    // Main Layout
    zv_mainLayout = new QVBoxLayout(this);
    zv_margin = zv_mainLayout->margin();
    // zv_mainLayout->setMargin(0);
    zv_mainLayout->setSpacing(0);
    setLayout(zv_mainLayout);

    // Title
    if(!title.isEmpty())
    {
        QLabel* titleLabel = new QLabel(this);
        titleLabel->setText(title);
        titleLabel->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);

        QHBoxLayout* titleLayout = new QHBoxLayout(this);
        titleLayout->addSpacing(zv_margin);
        titleLayout->addWidget(titleLabel);
        titleLayout->addStretch();
        zv_mainLayout->addSpacing(zv_margin);
        zv_mainLayout->addLayout(titleLayout);
    }
    // Splitter
    zv_splitter = new QSplitter(this);
    zv_splitter->setFrameShape(QFrame::NoFrame);
    zv_mainLayout->addWidget(zv_splitter);

    // Basement Widget
    QWidget* basement = new QWidget(this);
    // basement->setFrameShape(QFrame::HLine);
    // basement->setFrameStyle(QFrame::HLine | QFrame::Sunken);
    // basement layout
    QHBoxLayout* basementLayout = new QHBoxLayout(this);
    basement->setLayout(basementLayout);
    // int margin = basementLayout->margin();
    basementLayout->setMargin(0);
    basementLayout->setSpacing(zv_margin);
    // basementLayout->addSpacing(margin);

    QSizePolicy sizePolicy = basement->sizePolicy();
    sizePolicy.setVerticalPolicy(QSizePolicy::Fixed);
    basement->setSizePolicy(sizePolicy);

    zv_mainLayout->addWidget(basement);
    zv_mainLayout->addSpacing(zv_margin);

    // labels
    zv_hideLabel = new ZClickableLabel(this);
    zv_hideLabel->setCursor(Qt::PointingHandCursor);
    zv_hideLabel->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    zv_hideLabel->setVisible(false);
    basementLayout->addWidget(zv_hideLabel);

    zv_infoLabel = new QLabel(this);
    basementLayout->addWidget(zv_infoLabel);
    // basementLayout->addSpacing(margin);

    if(zv_sideBarOnLeft)
    {
        zv_closePixmap = QPixmap(NS_Icons::glIconSidebarRightClose);
        zv_openPixmap = QPixmap(NS_Icons::glIconSidebarRightOpen);
        basementLayout->addWidget(zv_hideLabel, 0, Qt::AlignLeft);
        basementLayout->addWidget(zv_infoLabel, 999999, Qt::AlignLeft);
    }
    else
    {
        zv_closePixmap = QPixmap(NS_Icons::glIconSidebarLeftClose);
        zv_openPixmap = QPixmap(NS_Icons::glIconSidebarLeftOpen);
        basementLayout->addWidget(zv_infoLabel, 999999, Qt::AlignRight);
        basementLayout->addWidget(zv_hideLabel, 0, Qt::AlignRight);
    }
}
//=======================================================
void ZWidgetWithSidebar::zh_createConnections()
{
    connect(zv_hideLabel, &ZClickableLabel::clicked,
            this, &ZWidgetWithSidebar::zh_onHideSidePanelButtonClick);
}
//=======================================================
void ZWidgetWithSidebar::zh_restoreSettings()
{
    QSettings settings;
    QVariant vData;
    settings.beginGroup(glAppVersion);
    settings.beginGroup(this->objectName());

    vData = settings.value("splitterGeometry");
    if(vData.isValid() && !vData.isNull() && vData.canConvert<QByteArray>())
    {
        zv_splitter->restoreState(vData.value<QByteArray>());
    }


    settings.endGroup();
    settings.endGroup();
}
//=======================================================
void ZWidgetWithSidebar::zp_saveSettings()
{
    QSettings settings;
    settings.beginGroup(glAppVersion);
    settings.beginGroup(this->objectName());

    settings.setValue("splitterGeometry", QVariant::fromValue<QByteArray>(zv_splitter->saveState()));

    settings.endGroup();
    settings.endGroup();
}
//=======================================================
void ZWidgetWithSidebar::zh_manageHideLabelPix()
{
    if(zv_sideBarWidget->isVisible())
    {
        zv_hideLabel->setPixmap(zv_closePixmap); //  QPixmap(":/images/ZWidgets/sidebarRightBlueClose.png"));
        zv_hideLabel->setToolTip(hideToolTipString);
    }
    else
    {
        zv_hideLabel->setPixmap(zv_openPixmap);// ":/images/ZWidgets/sidebarRightBlueOpen.png"));
        zv_hideLabel->setToolTip(showToolTipString);
    }
}
//=======================================================
void ZWidgetWithSidebar::zh_onHideSidePanelButtonClick()
{
    if(zv_sideBarWidget)
    {
        zv_sideBarWidget->setVisible(!zv_sideBarWidget->isVisible());
    }

    zh_manageHideLabelPix();
}
//=======================================================

