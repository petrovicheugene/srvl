//======================================================
#ifndef ZENERGYLINEMANAGER_H
#define ZENERGYLINEMANAGER_H
//======================================================
#include <QObject>


#include <QDoubleSpinBox>
#include <QLineEdit>
#include <QPushButton>
#include <QVBoxLayout>
//======================================================
class ZEnergyLineManager : public QObject
{
    Q_OBJECT
public:
    explicit ZEnergyLineManager(QObject *parent = nullptr);

signals:

    void zg_lineOperation() const;

public slots:


private slots:

    void zt_setLine();

private:

    // VARS


    // FUNCS

    void zt_createTestWidget();
    QDoubleSpinBox* zt_channelSpinBox;
    QLineEdit* zt_symbolLineEdit;
    QPushButton* zt_setLineButton;
};
//======================================================
#endif // ZENERGYLINEMANAGER_H
