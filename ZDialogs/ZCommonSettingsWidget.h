//========================================================
#ifndef ZCOMMONSETTINGSWIDGET_H
#define ZCOMMONSETTINGSWIDGET_H
//========================================================
#include <QWidget>
//========================================================
class QListWidget;
class QPushButton;
//========================================================
class ZCommonSettingsWidget : public QWidget
{
    Q_OBJECT
public:
    explicit ZCommonSettingsWidget(QWidget *parent = 0);

signals:

public slots:


private slots:

    void zh_runPasswordChangeDialog();
    void zh_newOperator();
    void zh_editOperator();
    void zh_removeOperator();

    void zh_checkLoginExistance(const QString& login,
                                bool& existance) const;

private:

    // VARS
    QPushButton* zv_passwordSettingsButton;

    QListWidget* zv_operatorsListWidget;
    QPushButton* zv_newOperatorButton;
    QPushButton* zv_editOperatorButton;
    QPushButton* zv_removeOperatorButton;


    // FUNCS
    void zh_createComponents();
    void zh_createConnections();
    void zh_loadOperators();

    bool zh_findNewIdInTable(const QString& tableName, int& newId);
    void zh_addOperatorItem(int id, const QString& login, const QByteArray& password);



};
//========================================================
#endif // ZCOMMONSETTINGSWIDGET_H
