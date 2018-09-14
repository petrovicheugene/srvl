//======================================================
#ifndef ZPROPERTYSECTIONDIALOG_H
#define ZPROPERTYSECTIONDIALOG_H
//======================================================
#include <ZBaseDialog.h>
#include <QList>
//======================================================
class QLabel;
class QLineEdit;
class QPushButton;
class QRadioButton;
//======================================================
class ZPropertySectionEditDialog : public ZBaseDialog
{
    Q_OBJECT
public:
    enum AdditionTarget {AT_ALL,
                         AT_SELECTED,
                         AT_CURRENT};
    explicit ZPropertySectionEditDialog(const QStringList& branch,
                                    bool currentVisible,
                                    bool selectedVisible,
                                    QWidget *parent = nullptr);

    AdditionTarget zp_additionTarget() const;
    QStringList zp_newSectionBranch() const;

    void zp_setCurrentSectionBranch(const QStringList& branch);

signals:

public slots:

private slots:

    void zh_onOkButtonClick();
    void zh_onCloseButtonClick();

private:

    // VARS
    QStringList zv_parentSectionBranch;

    QLabel* zv_parentSectionBranchLabel;
    QLineEdit* zv_sectionLineEdit;

    QPushButton* zv_okButton;
    QPushButton* zv_cancelButton;

    QRadioButton* zv_toCurrentChemicalElementRadioButton;
    QRadioButton* zv_toSelectedChemicalElementsRadioButton;
    QRadioButton* zv_toAllChemicalElementsRadioButton;


    // FUNCS
    void zh_createComponents();
    void zh_createConnections();
    void zh_setUpAdditionTargetButtons(bool selectedVisible,
                bool currentVisible);
    QWidget* zh_createControlWidget();


};
//======================================================
#endif // ZPROPERTYSECTIONDIALOG_H
