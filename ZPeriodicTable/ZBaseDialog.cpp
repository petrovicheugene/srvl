//====================================================
#include "ZBaseDialog.h"

#include <QApplication>
#include <QDebug>
#include <QDialogButtonBox>
#include <QFrame>
#include <QKeyEvent>
#include <QLayout>
#include <QPushButton>
#include <QSettings>
#include <QVBoxLayout>
//====================================================
ZBaseDialog::ZBaseDialog(QWidget *parent) : QDialog(parent)
{
    zh_createBaseComponents();
    zh_restoreSettings();
}
//====================================================
ZBaseDialog::ZBaseDialog(const QString& subclassName,
                         QWidget *parent) : QDialog(parent), zv_settingsGroupName(subclassName)
{
    zh_createBaseComponents();
    zh_restoreSettings();
}
//====================================================
ZBaseDialog::~ZBaseDialog()
{
    zh_saveSettings();
}
//====================================================
bool ZBaseDialog::eventFilter(QObject *watched, QEvent *event)
{
    if(!zh_ctrlXHandling(event))
    {
        return QDialog::eventFilter(watched, event);
    }
    return true;
}
//====================================================
bool ZBaseDialog::event(QEvent *event)
{
    if(!zh_ctrlXHandling(event))
    {
        return QDialog::event(event);
    }
    return true;
}
//====================================================
bool ZBaseDialog::zh_ctrlXHandling(QEvent *event)
{
    if(event->type() == QEvent::KeyPress)
    {
        QKeyEvent* keyEvent = static_cast<QKeyEvent*>(event);
        if(!keyEvent)
        {
            return false;
        }

        if(keyEvent->key() == Qt::Key_X && (keyEvent->modifiers() & Qt::ControlModifier))
        {
            reject();
            return false;
        }
    }

    return false;
}
//====================================================
void ZBaseDialog::zh_createBaseComponents()
{
    zv_mainLayout = new QVBoxLayout;
    setLayout(zv_mainLayout);

    // basement sparator line
    QFrame* line = new QFrame(this);
    line->setFrameStyle(QFrame::HLine | QFrame::Sunken);
    line->setLineWidth(1);
    zv_mainLayout->addWidget(line);

    // basement
    zv_buttonBox = new QDialogButtonBox(this);
    zv_mainLayout->addWidget(zv_buttonBox);

    zv_baseComponentCount = zv_mainLayout->count();
}
//====================================================
void ZBaseDialog::zh_saveSettings()
{
    QSettings settings;
    if(!zh_openDialogSettingsGroup(&settings))
    {
        return;
    }

    settings.setValue(zv_geometryName, saveGeometry());

    zh_closeDialogSettingsGroup(&settings);
}
//====================================================
void ZBaseDialog::zh_restoreSettings()
{
    QSettings settings;
    if(!zh_openDialogSettingsGroup(&settings))
    {
        return;
    }

    if(settings.value(zv_geometryName).canConvert<QByteArray>())
    {
        restoreGeometry(settings.value(zv_geometryName).toByteArray());
    }

    zh_closeDialogSettingsGroup(&settings);
}
//====================================================
bool ZBaseDialog::zh_openDialogSettingsGroup(QSettings* settings)
{
    if(settings == nullptr)
    {
        return false;
    }
    // open version group
    QString verString = qApp->property("glAppVersion").toString();
    if(!verString.isEmpty())
    {
        settings->beginGroup(verString);
    }

    // open dialog group
    if(!zv_settingsGroupName.isEmpty())
    {
        settings->beginGroup(zv_settingsGroupName);
    }
    return true;
}
//====================================================
bool ZBaseDialog::zh_closeDialogSettingsGroup(QSettings *settings)
{
    if(settings == nullptr)
    {
        return false;
    }

    // close groups
    while(!settings->group().isEmpty())
    {
        settings->endGroup();
    }

    return true;
}
//====================================================
void ZBaseDialog::zh_insertLayoutToMainLayout(int index, QLayout* layout,
                                              int stretch)
{
    zv_mainLayout->insertLayout(index, layout, stretch);
}
//====================================================
void ZBaseDialog::zh_insertWidgetToMainLayout(int index, QWidget* widget,
                                              int stretch, Qt::Alignment alignment)
{
    zv_mainLayout->insertWidget(index, widget, stretch, alignment);
}
//====================================================
void ZBaseDialog::zh_addLayoutToMainLayout(QLayout* layout,
                                           int stretch)
{
    zh_insertLayoutToMainLayout(zv_mainLayout->count() - zv_baseComponentCount,
                                layout, stretch);
}
//====================================================
void ZBaseDialog::zh_addWidgetToMainLayout(QWidget* widget, int stretch,
                                           Qt::Alignment alignment)
{
    zh_insertWidgetToMainLayout(zv_mainLayout->count() - zv_baseComponentCount,
                                widget,  stretch, alignment);
}
//====================================================
void ZBaseDialog::zh_addStretchToMainLayout(int stretch)
{
    zv_mainLayout->insertStretch(zv_mainLayout->count() - zv_baseComponentCount,
                                 stretch);
}
//====================================================
QPushButton* ZBaseDialog::zh_createBasementButton(QString text,
                                                  QString toolTip, QIcon icon)
{
    QPushButton* button = new QPushButton(this);
    if(!text.isEmpty())
    {
        button->setText(text);
    }

    if(!toolTip.isEmpty())
    {
        button->setToolTip(toolTip);
    }

    if(!icon.isNull())
    {
        button->setIcon(icon);
    }

    zv_buttonBox->addButton(button, QDialogButtonBox::ActionRole);
    button->setFocusPolicy(Qt::NoFocus);
    return button;
}
//====================================================
