//======================================================
#ifndef ZPROPERTYEDITDIALOG_H
#define ZPROPERTYEDITDIALOG_H
//======================================================
#include <ZBaseDialog.h>
//======================================================
class QLineEdit;
class QPushButton;
//======================================================
class ZPropertyEditDialog : public ZBaseDialog
{
    Q_OBJECT
public:
    explicit ZPropertyEditDialog(QWidget *parent = nullptr);

    QString zp_propertyName() const;
    QString zp_propertyValue() const;
    QPair<QString, QString> zp_property() const;

signals:

public slots:

private slots:

    void zh_onOkButtonClick();
    void zh_onCloseButtonClick();

private:

    // VARS

    QLineEdit* zv_propertyNameLineEdit;
    QLineEdit* zv_propertyValueLineEdit;

    QPushButton* zv_okButton;
    QPushButton* zv_cancelButton;


    // FUNCS
    void zh_createComponents();
    void zh_createConnections();

    QWidget* zh_createControlWidget();

};
//======================================================
#endif // ZPROPERTYEDITDIALOG_H
