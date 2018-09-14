//====================================================
#ifndef ZBASEDIALOG_H
#define ZBASEDIALOG_H
//====================================================
#include <QDialog>
#include <QIcon>
//====================================================
class QDialogButtonBox;
class QEvent;
class QSettings;
class QPushButton;
class QVBoxLayout;
//====================================================
class ZBaseDialog : public QDialog
{
    Q_OBJECT
public:
    explicit ZBaseDialog(QWidget *parent = nullptr);
    explicit ZBaseDialog(const QString& settingsGroupName,
                         QWidget *parent = nullptr); // zv_settingsGroupName - subclass name
    ~ZBaseDialog();

    bool eventFilter(QObject *watched, QEvent *event);
    bool event(QEvent *event);

signals:

public slots:

protected:

    // VARS
    const QString zv_settingsGroupName;
    const QString zv_geometryName = "geometry";

    QVBoxLayout* zv_mainLayout;
    QDialogButtonBox* zv_buttonBox;
    int zv_baseComponentCount;

    // FUNCS

    void zh_createBaseComponents();
    virtual void zh_saveSettings();
    virtual void zh_restoreSettings();
    bool zh_openDialogSettingsGroup(QSettings* settings);
    bool zh_closeDialogSettingsGroup(QSettings* settings);

    void zh_insertLayoutToMainLayout(int index,
                                     QLayout* layout,
                                     int stretch = 0);
    void zh_insertWidgetToMainLayout(int index,
                                     QWidget* widget ,
                                     int stretch = 0,
                                     Qt::Alignment alignment = Qt::Alignment());
    void zh_addLayoutToMainLayout(QLayout* layout,
                                  int stretch = 0);
    void zh_addWidgetToMainLayout(QWidget* widget,
                                  int stretch = 0,
                                  Qt::Alignment alignment = Qt::Alignment());
    void zh_addStretchToMainLayout(int stretch = 0);

    QPushButton* zh_createBasementButton(QString text = QString(),
                                         QString toolTip = QString(),
                                         QIcon icon = QIcon());

    bool zh_ctrlXHandling(QEvent *event);

};
//====================================================
#endif // ZBASEDIALOG_H
