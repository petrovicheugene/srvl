//======================================================
#include "ZSaveSpectraToFilesDialog.h"
#include "ZGeneral.h"
#include "ZSpeSpectrum.h"

#include <QApplication>
#include <QDialogButtonBox>
#include <QHBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QStandardPaths>
#include <QVBoxLayout>

//======================================================
explicit ZSaveSpectraToFilesDialog(const QMap< QPair<quint8, int>, QList<ZSpeSpectrum*> >& spectrumMap,
                                   QWidget *parent )

{
    zh_createComponents();
    zh_createConnections();
}
//======================================================
void ZSaveSpectraToFilesDialog::zh_createComponents()
{
    // main layout
    QVBoxLayout* mainLayout = new QVBoxLayout;
    setLayout(mainLayout);

    // file path
    QLabel* label = new QLabel(this);
    label->setText(tr("Calibration file:"));
    mainLayout->addWidget(label);

//    QHBoxLayout* pathLayout = new QHBoxLayout(this);
//    mainLayout->addLayout(pathLayout);
//    zv_filePathLineEdit = new QLineEdit(this);
//    zv_filePathLineEdit->setReadOnly(true);
//    pathLayout->addWidget(zv_filePathLineEdit);
//    zv_browseButton = new QPushButton(NS_Buttons::glButtonBrowse, this);
//    pathLayout->addWidget(zv_browseButton);

//    // central layout
//    QHBoxLayout* centralLayout = new QHBoxLayout(this);
//    mainLayout->addLayout(centralLayout);
//    // left layout
//    QVBoxLayout* leftLayout = new QVBoxLayout(this);
//    centralLayout->addLayout(leftLayout);
//    centralLayout->addSpacing(20);
//    // right layout
//    QVBoxLayout* rightLayout = new QVBoxLayout(this);
//    centralLayout->addLayout(rightLayout);

//    // name
//    label = new QLabel(this);
//    label->setText(tr("Calibration name:"));
//    leftLayout->addWidget(label);

//    zv_calibrationNameLineEdit = new QLineEdit(this);
//    leftLayout->addWidget(zv_calibrationNameLineEdit);

//    // chem elment
//    label = new QLabel(this);
//    label->setText(tr("Chemical element:"));
//    leftLayout->addWidget(label);

//    zv_chemElementLineEdit = new QLineEdit(this);
//    leftLayout->addWidget(zv_chemElementLineEdit);

//    // range
//    label = new QLabel(this);
//    label->setText(tr("Concentration range:"));
//    rightLayout->addWidget(label);

//    // max
//    QHBoxLayout* rangeMaxLayout = new QHBoxLayout(this);
//    rightLayout->addLayout(rangeMaxLayout);

//    label = new QLabel(this);
//    label->setText(tr("max:"));
//    rangeMaxLayout->addWidget(label);
//    zv_rangeMaxMarginSpinBox = new QDoubleSpinBox(this);
//    zv_rangeMaxMarginSpinBox->setRange(0.0, 1.0e+99);
//    label->setBuddy(zv_rangeMaxMarginSpinBox);
//    rangeMaxLayout->addWidget(zv_rangeMaxMarginSpinBox, 999999);

//    // spacing between min and max
//    rightLayout->addSpacing(label->sizeHint().width());

//    // min
//    QHBoxLayout* rangeMinLayout = new QHBoxLayout(this);
//    rightLayout->addLayout(rangeMinLayout);

//    label = new QLabel(this);
//    label->setText(tr("min:"));
//    rangeMinLayout->addWidget(label);
//    zv_rangeMinMarginSpinBox = new QDoubleSpinBox(this);
//    label->setBuddy(zv_rangeMinMarginSpinBox);
//    zv_rangeMinMarginSpinBox->setRange(0.0, 1.0e+99);
//    rangeMinLayout->addWidget(zv_rangeMinMarginSpinBox, 999999);

    // Basement
    mainLayout->addStretch();

    // sparator line
    QFrame* line = new QFrame(this);
    line->setFrameStyle(QFrame::HLine | QFrame::Sunken);
    line->setLineWidth(1);
    mainLayout->addWidget(line);

    // dialog buttons
    QDialogButtonBox* buttonBox = new QDialogButtonBox(this);
    mainLayout->addWidget(buttonBox);

    zv_okButton = new QPushButton(NS_Buttons::glButtonOk, this);
    buttonBox->addButton(zv_okButton, QDialogButtonBox::ActionRole);

    zv_cancelButton = new QPushButton(NS_Buttons::glButtonCancel, this);
    buttonBox->addButton(zv_cancelButton, QDialogButtonBox::ActionRole);
}
//======================================================
void ZSaveSpectraToFilesDialog::zh_createConnections()
{

}
//======================================================
