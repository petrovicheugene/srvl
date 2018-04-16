//============================================================
#include "ZDashboardSettingsWidget.h"
#include <ZGeneral.h>
#include "ZDashboardSettings.h"
#include "ZColorLabel.h"
#include "ZLabeledSliderWidget.h"

#include <QGridLayout>
#include <QHBoxLayout>
#include <QVBoxLayout>

#include <QComboBox>
#include <QLabel>
#include <QPushButton>
//============================================================
ZDashboardSettingsWidget::ZDashboardSettingsWidget(QWidget *parent) : QWidget(parent)
{
    zh_createComponents();
    zh_createConnections();
    zh_controlEnableManagement();
}
//============================================================
void ZDashboardSettingsWidget::zp_setAppSettings(const ZDashboardSettings& appSettings)
{
    // style
    int index = zh_findComboBoxIndexForData(zv_dashBoardStyleComboBox, QVariant::fromValue(appSettings.zv_dashboardStyle));
    zv_dashBoardStyleComboBox->setCurrentIndex(index);

    // size
    zv_dashBoardSizeSliderWidget->zp_setValue(appSettings.zv_progressBarSize);

    // location
    index = zh_findComboBoxIndexForData(zv_locationComboBox, QVariant::fromValue(appSettings.zv_dashboardLocation));
    zv_locationComboBox->setCurrentIndex(index);

    // progress bar color
    zv_progressBarColorLabel->zp_setColor(appSettings.zv_progressBarColor);
    // text color
    zv_textColorLabel->zp_setColor(appSettings.zv_textColor);

}
//============================================================
int ZDashboardSettingsWidget::zh_findComboBoxIndexForData(QComboBox* comboBox, QVariant data, int defaultIndex) const
{
    int index = comboBox->findData(data);
    if(index < 0 || index >= comboBox->count())
    {
        index = defaultIndex;
    }

    return index;
}
//============================================================
void ZDashboardSettingsWidget::zp_appSettings(ZDashboardSettings& appSettings)
{
    appSettings.zv_dashboardStyle = zv_dashBoardStyleComboBox->currentData().value<ZDashboardSettings::DashboardStyle>();
    appSettings.zv_progressBarSize = zv_dashBoardSizeSliderWidget->zp_value();
    appSettings.zv_dashboardLocation = zv_locationComboBox->currentData().value<ZDashboardSettings::DashboardLocation>();
    appSettings.zv_progressBarColor = zv_progressBarColorLabel->zp_color();
    appSettings.zv_textColor = zv_textColorLabel->zp_color();
}
//============================================================
void ZDashboardSettingsWidget::zh_createComponents()
{
    QHBoxLayout* baseLayout = new QHBoxLayout(this);
    setLayout(baseLayout);
    QVBoxLayout* mainLayout = new QVBoxLayout(this);
    baseLayout->addLayout(mainLayout);
    baseLayout->addStretch();

    // Main Layout
    // dashboard style
    QLabel* label = new QLabel(this);
    label->setText(QString("<b><font color=darkgreen>%1</font></b>").arg(tr("Progress bar style:")));
    mainLayout->addWidget(label);

    zv_dashBoardStyleComboBox = new QComboBox(this);
    // init  items
    ZDashboardSettings::zp_initDashboardStyleComboBox(*zv_dashBoardStyleComboBox);
    // add to layout
    mainLayout->addWidget(zv_dashBoardStyleComboBox);
    label->setBuddy(zv_dashBoardStyleComboBox);

    // Dashboard size
    label = new QLabel(this);
    label->setText(QString("<b><font color=darkgreen>%1</font></b>").arg(tr("Progress bar size:")));
    mainLayout->addWidget(label);

    zv_dashBoardSizeSliderWidget = new ZLabeledSliderWidget(Qt::Horizontal, this);
    // zv_dashBoardSizeSliderWidget->zp_setTextColor(QColor(Qt::darkGreen));
    zv_dashBoardSizeSliderWidget->zp_setTextHTMLColorString("darkgreen");
    zv_dashBoardSizeSliderWidget->zp_setFontBold(true);
    zv_dashBoardSizeSliderWidget->zp_setMinimumCaption(tr("Small"));
    zv_dashBoardSizeSliderWidget->zp_setMaximumCaption(tr("Large"));

    zv_dashBoardSizeSliderWidget->zp_setTickPosition(QSlider::TicksBelow);
    zv_dashBoardSizeSliderWidget->zp_setMinimum(ZDashboardSettings::zv_progressBarMinimumSize);
    zv_dashBoardSizeSliderWidget->zp_setMaximum(ZDashboardSettings::zv_progressBarMaximumSize);
    // add to layout
    mainLayout->addWidget(zv_dashBoardSizeSliderWidget);
    label->setBuddy(zv_dashBoardSizeSliderWidget);

    // progress bar color
    label = new QLabel(this);
    label->setText(QString("<b><font color=darkgreen>%1</font></b>").arg(tr("Progress bar line color:")));
    mainLayout->addWidget(label);

    QHBoxLayout* progressBarColorLayout = new QHBoxLayout(this);
    mainLayout->addLayout(progressBarColorLayout);

    zv_progressBarColorLabel = new ZColorLabel(this);
    progressBarColorLayout->addWidget(zv_progressBarColorLabel, 0, Qt::AlignLeft | Qt::AlignVCenter);
    zv_progressBarColorLabel->zp_setColor(QColor(Qt::transparent));
    zv_progressBarColorDialogButton = new QPushButton(NS_CommonStrings::glSelectColor, this);
    progressBarColorLayout->addWidget(zv_progressBarColorDialogButton);

    // text color
    label = new QLabel(this);
    label->setText(QString("<b><font color=darkgreen>%1</font></b>").arg(tr("Text color:")));
    mainLayout->addWidget(label);

    QHBoxLayout* textColorLayout = new QHBoxLayout(this);
    mainLayout->addLayout(textColorLayout);

    zv_textColorLabel = new ZColorLabel(this);
    textColorLayout->addWidget(zv_textColorLabel, 0, Qt::AlignLeft | Qt::AlignVCenter);
    zv_textColorLabel->zp_setColor(QColor(Qt::transparent));
    zv_textColorDialogButton = new QPushButton(NS_CommonStrings::glSelectColor, this);
    textColorLayout->addWidget(zv_textColorDialogButton);

    // location
    label = new QLabel(this);
    label->setText(QString("<b><font color=darkgreen>%1</font></b>").arg(tr("Dashboard location:")));
    mainLayout->addWidget(label);

    zv_locationComboBox = new QComboBox(this);
    // init items
    ZDashboardSettings::zp_initDashboardLocationComboBox(*zv_locationComboBox);
    // add to layout
    mainLayout->addWidget(zv_locationComboBox);
    label->setBuddy(zv_locationComboBox);
    mainLayout->addStretch();
}
//============================================================
void ZDashboardSettingsWidget::zh_createConnections()
{
    connect(zv_dashBoardStyleComboBox, SIGNAL(currentIndexChanged(int)),
            this, SLOT(zh_onStyleComboBoxIndexChange(int)));
    connect(zv_dashBoardStyleComboBox, SIGNAL(currentIndexChanged(int)),
            this, SLOT(zh_someValueChange(int)));

    connect(zv_progressBarColorDialogButton, &QPushButton::clicked,
            zv_progressBarColorLabel, &ZColorLabel::zp_runColorSelectionDialog);
    connect(zv_progressBarColorLabel, &ZColorLabel::zg_colorChanged,
            this, &ZDashboardSettingsWidget::zg_settingsChanged);

    connect(zv_textColorDialogButton, &QPushButton::clicked,
            zv_textColorLabel, &ZColorLabel::zp_runColorSelectionDialog);
    connect(zv_textColorLabel, &ZColorLabel::zg_colorChanged,
            this, &ZDashboardSettingsWidget::zg_settingsChanged);


    connect(zv_dashBoardSizeSliderWidget, &ZLabeledSliderWidget::zg_valueChanged,
            this, &ZDashboardSettingsWidget::zh_someValueChange);
    connect(zv_locationComboBox, SIGNAL(currentIndexChanged(int)),
            this, SLOT(zh_someValueChange(int)));

}
//============================================================
void ZDashboardSettingsWidget::zh_onStyleComboBoxIndexChange(int value)
{
    zh_controlEnableManagement();
}
//============================================================
void ZDashboardSettingsWidget::zh_someValueChange(int value)
{
    emit zg_settingsChanged();
}
//============================================================
void ZDashboardSettingsWidget::zh_controlEnableManagement()
{
    bool classicStyle = (zv_dashBoardStyleComboBox->currentData().value<ZDashboardSettings::DashboardStyle>()
                         == ZDashboardSettings::DBS_CLASSIC) ||
            (zv_dashBoardStyleComboBox->currentData().value<ZDashboardSettings::DashboardStyle>()
             == ZDashboardSettings::DBS_CLASSIC_SLIM);

    zv_progressBarColorLabel->setDisabled(classicStyle);
    zv_progressBarColorDialogButton->setDisabled(classicStyle);
}
//============================================================
