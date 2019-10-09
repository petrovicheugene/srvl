//===========================================================
#include "ZMeasuringConditionsListDialog.h"
#include "ZGeneral.h"
#include "ZMeasuringConditionsDialog.h"
#include "ZReadOnlyStyledItemDelegate.h"

#include <QApplication>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QDialogButtonBox>

#include <QGroupBox>
#include <QEvent>
#include <QHeaderView>
#include <QLineEdit>
#include <QLabel>
#include <QMessageBox>
#include <QMouseEvent>
#include <QPushButton>
#include <QTableView>
#include <QSettings>
#include <QSpinBox>
#include <QSqlTableModel>
#include <QSqlRelationalTableModel>
#include <QSqlError>
#include <QSqlField>
#include <QSqlQuery>
#include <QSqlRecord>

//===========================================================
ZMeasuringConditionsListDialog::ZMeasuringConditionsListDialog(bool forSelection, QWidget *parent) :
    QDialog(parent)
{
    setWindowTitle(tr("Measurement conditions"));
    setWindowFlags(Qt::Tool);

    zv_quantitySpinBox = 0;
    zh_createComponents(forSelection);
    zh_createConnections();
    zh_restoreSettings();
}
//===========================================================
ZMeasuringConditionsListDialog::~ZMeasuringConditionsListDialog()
{
    zh_saveSettings();
}
//===========================================================
int ZMeasuringConditionsListDialog::zp_id() const
{
    return zh_currentDataFromColumn(0);
}
//===========================================================
int ZMeasuringConditionsListDialog::zp_gainFactor() const
{
    return zh_currentDataFromColumn(1);
}
//===========================================================
int ZMeasuringConditionsListDialog::zp_exposition() const
{
    return zh_currentDataFromColumn(2);
}
//===========================================================
int ZMeasuringConditionsListDialog::zp_quantity() const
{
    if(zv_quantitySpinBox != 0)
    {
        return zv_quantitySpinBox->value();
    }

    return 1;
}
//===========================================================
int ZMeasuringConditionsListDialog::zh_currentDataFromColumn(int column) const
{
    if(column < 0 || column >= zv_measuringConditionsModel->columnCount())
    {
        return -1;
    }

    QModelIndex index = zv_measuringConditionsTable->selectionModel()->currentIndex();
    if(!index.isValid())
    {
        return -1;
    }

    int row = index.row();
    index = zv_measuringConditionsModel->index(row, column);

    if(!index.isValid())
    {
        return -1;
    }

    QVariant vData = index.data(Qt::DisplayRole);
    if(!vData.isValid() || !vData.canConvert<int>())
    {
        return -1;
    }

    return vData.toInt();
}
//===========================================================
void ZMeasuringConditionsListDialog::zh_createComponents(bool forSelection)
{
    // model
    zv_measuringConditionsModel = new QSqlTableModel(this);
    zv_measuringConditionsModel->setTable("measuring_conditions");
    zv_measuringConditionsModel->setEditStrategy(QSqlTableModel::OnManualSubmit);
    zv_measuringConditionsModel->select();
    zv_measuringConditionsModel->setHeaderData(0, Qt::Horizontal, QVariant(tr("Id")));
    zv_measuringConditionsModel->setHeaderData(1, Qt::Horizontal, QVariant(tr("Gain Factor")));
    zv_measuringConditionsModel->setHeaderData(2, Qt::Horizontal, QVariant(tr("Exposition")));


    //zv_measuringConditionsModel->setRelation(2, QSqlRelation("gain_factors", "gain_factor", "gain_factor"));

    zv_gainFactorModel = new QSqlTableModel(this);
    zv_gainFactorModel->setTable("gain_factors");
    zv_gainFactorModel->setEditStrategy(QSqlTableModel::OnManualSubmit);
    zv_gainFactorModel->select();
    zv_gainFactorModel->setHeaderData(0, Qt::Horizontal, QVariant(tr("Gain Factor")));
    zv_gainFactorModel->setHeaderData(1, Qt::Horizontal, QVariant(tr("En K0")));
    zv_gainFactorModel->setHeaderData(2, Qt::Horizontal, QVariant(tr("En K1")));
    zv_gainFactorModel->setHeaderData(3, Qt::Horizontal, QVariant(tr("En K2")));


    QVBoxLayout* mainLayout = new QVBoxLayout;
    setLayout(mainLayout);

    QLabel* label;
    QHBoxLayout* levelLayout;
    // selected conditions
    //    if(forSelection)
    //    {
    //        // group box
    //        QGroupBox* selectedGroupBox = new QGroupBox(this);
    //        selectedGroupBox->setTitle(tr("Selected"));
    //        mainLayout->addWidget(selectedGroupBox);
    //        QVBoxLayout* groupBoxLayout = new QVBoxLayout(this);
    //        selectedGroupBox->setLayout(groupBoxLayout);

    //        // selected gain factor
    //                label = new QLabel(this);
    //                label->setText(glCreateCaption(tr("Gain factor:")));
    //                groupBoxLayout->addWidget(label);


    //                levelLayout = new QHBoxLayout(this);
    //                groupBoxLayout->addLayout(levelLayout);
    //                zv_selectedGainFactorLineEdit = new QLineEdit(this);
    //                zv_selectedGainFactorLineEdit->setReadOnly(true);
    //                zv_selectedGainFactorLineEdit->setText(zv_noSelectedConditionsString);
    //                levelLayout->addWidget(zv_selectedGainFactorLineEdit);
    //                levelLayout->addStretch();

    //                // selected exposition
    //                label = new QLabel(this);
    //                label->setText(glCreateCaption(tr("Exposition (s):")));
    //                groupBoxLayout->addWidget(label);


    //                levelLayout = new QHBoxLayout(this);
    //                groupBoxLayout->addLayout(levelLayout);
    //                zv_selectedExpositionLineEdit = new QLineEdit(this);
    //                zv_selectedExpositionLineEdit->setReadOnly(true);
    //                zv_selectedExpositionLineEdit->setText(zv_noSelectedConditionsString);
    //                levelLayout->addWidget(zv_selectedExpositionLineEdit);
    //                levelLayout->addStretch();


    //        label = new QLabel(this);
    //        label->setText(glCreateCaption(tr("Measuring conditions:")));
    //        groupBoxLayout->addWidget(label);
    //        levelLayout = new QHBoxLayout(this);
    //        groupBoxLayout->addLayout(levelLayout);
    //        zv_selectedConditionsLineEdit = new QLineEdit(this);
    //        zv_selectedConditionsLineEdit->setReadOnly(true);
    //        zv_selectedConditionsLineEdit->setText(zv_noSelectedConditionsString);
    //        levelLayout->addWidget(zv_selectedConditionsLineEdit);
    //        levelLayout->addStretch();
    //    }

    // list table
    label = new QLabel(this);
    label->setText(glCreateCaption(tr("Measuring tasks:")));
    mainLayout->addWidget(label);

    zv_measuringConditionsTable = new QTableView(this);
    zv_measuringConditionsTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    zv_measuringConditionsTable->setSelectionMode(QAbstractItemView::SingleSelection);
    mainLayout->addWidget(zv_measuringConditionsTable);

    // new button
    levelLayout = new QHBoxLayout;
    mainLayout->addLayout(levelLayout);
    zv_newConditionsButton = new QPushButton(this);
    zv_newConditionsButton->setText(tr("New"));
    zv_newConditionsButton->setFlat(true);
    zv_newConditionsButton->setIcon(QIcon(":/images/ZImages/new-8"));
    zv_newConditionsButton->setToolTip(tr("Create new measurement conditions"));


    levelLayout->addStretch();
    levelLayout->addWidget(zv_newConditionsButton, 0, Qt::AlignRight | Qt::AlignVCenter);


    if(forSelection)
    {
        label = new QLabel(this);
        label->setText(glCreateCaption(tr("Quantity:")));
        mainLayout->addWidget(label);
        levelLayout = new QHBoxLayout;
        mainLayout->addLayout(levelLayout);
        zv_quantitySpinBox = new QSpinBox(this);
        zv_quantitySpinBox->setRange(1, zv_maxQuantity);
        levelLayout->addWidget(zv_quantitySpinBox);
        levelLayout->addStretch();
    }

    // message label
    zv_messageLabel = new QLabel(this);
    zv_messageLabel->setWordWrap(true);
    mainLayout->addWidget(zv_messageLabel);

    // basement
    // Separator
    QFrame* line = new QFrame(this);
    line->setFrameStyle(QFrame::HLine | QFrame::Sunken);
    line->setLineWidth(1);
    mainLayout->addWidget(line);

    QDialogButtonBox* buttonBox = new QDialogButtonBox(this);
    mainLayout->addWidget(buttonBox);

    // buttons
    //    zv_newConditionsButton = new QPushButton(NS_Buttons::glButtonNew, this);
    //    buttonBox->addButton(zv_newConditionsButton, QDialogButtonBox::ActionRole);

    zv_okButton = new QPushButton(NS_Buttons::glButtonOk, this);
    buttonBox->addButton(zv_okButton, QDialogButtonBox::ActionRole);

    zv_cancelButton = new QPushButton(NS_Buttons::glButtonCancel, this);
    buttonBox->addButton(zv_cancelButton, QDialogButtonBox::ActionRole);
}
//======================================================
void ZMeasuringConditionsListDialog::zh_createConnections()
{
    zv_measuringConditionsTable->setModel(zv_measuringConditionsModel);
    zv_measuringConditionsTable->setColumnHidden(0, true);

    // set read only delegate
    if(zv_measuringConditionsTable->itemDelegate())
    {
        delete zv_measuringConditionsTable->itemDelegate();
    }

    ZReadOnlyStyledItemDelegate* delegate = new ZReadOnlyStyledItemDelegate(zv_measuringConditionsTable);
    zv_measuringConditionsTable->setItemDelegate(delegate);
    zv_measuringConditionsTable->horizontalHeader()->setStretchLastSection(true);
    zv_measuringConditionsTable->viewport()->installEventFilter(this);

    connect(zv_okButton, &QPushButton::clicked,
            this, &ZMeasuringConditionsListDialog::zh_onOkButtonClick);
    connect(zv_cancelButton, &QPushButton::clicked,
            this, &ZMeasuringConditionsListDialog::reject);
    connect(zv_newConditionsButton, &QPushButton::clicked,
            this, &ZMeasuringConditionsListDialog::zh_newConditions);
    connect(zv_measuringConditionsTable->selectionModel(), &QItemSelectionModel::selectionChanged,
            this, &ZMeasuringConditionsListDialog::zh_onSelectionChange);

}
//======================================================
void ZMeasuringConditionsListDialog::zh_restoreSettings()
{
    QSettings settings;
    QVariant vData;
    settings.beginGroup(qApp->applicationVersion());
    settings.beginGroup("MeasuringConditionsListDialog");

    vData = settings.value("dialogGeometry");
    if(vData.isValid() && !vData.isNull() && vData.canConvert<QByteArray>())
    {
        this->restoreGeometry(vData.value<QByteArray>());
    }

    settings.endGroup();
    settings.endGroup();
}
//======================================================
void ZMeasuringConditionsListDialog::zh_saveSettings() const
{
    QSettings settings;
    settings.beginGroup(qApp->applicationVersion());
    settings.beginGroup("MeasuringConditionsListDialog");

    settings.setValue("dialogGeometry", QVariant::fromValue<QByteArray>(this->saveGeometry()));
    //settings.setValue("splitterGeometry", QVariant::fromValue<QByteArray>(zv_tableSplitter->saveState()));

    settings.endGroup();
    settings.endGroup();
}
//======================================================
bool ZMeasuringConditionsListDialog::eventFilter(QObject* object, QEvent* event)
{
    if(object == zv_measuringConditionsTable->viewport() && event->type() == QEvent::MouseButtonDblClick)
    {
        QMouseEvent* mouseEvent = static_cast<QMouseEvent*>(event);
        if(mouseEvent && mouseEvent->button() == Qt::LeftButton)
        {
            zh_onOkButtonClick();
            return true;
        }
    }

    return QDialog::eventFilter(object, event);
}
//======================================================
void ZMeasuringConditionsListDialog::zh_onSelectionChange(const QItemSelection& current,
                                                          const QItemSelection& previous)
{
    zv_messageLabel->clear();
    zh_updateSelectedMeasuringConditionsString();
}
//======================================================
void ZMeasuringConditionsListDialog::zh_updateSelectedMeasuringConditionsString()
{
    //    int gainFactor = zp_gainFactor();
    //    int exposition = zp_exposition();
    //    if(gainFactor < 0)
    //    {
    //        zv_selectedGainFactorLineEdit->setText(zv_noSelectedConditionsString);
    //    }
    //    else
    //    {
    //        zv_selectedGainFactorLineEdit->setText(QString::number(gainFactor));
    //    }

    //    if(exposition < 0)
    //    {
    //        zv_selectedExpositionLineEdit->setText(zv_noSelectedConditionsString);
    //    }
    //    else
    //    {
    //        zv_selectedExpositionLineEdit->setText(QString::number(exposition));
    //    }


    // zv_selectedConditionsLineEdit->setText(tr("G.F.:%1; Expo:%2").arg(QString::number(gainFactor), QString::number(exposition)));
}
//======================================================
bool ZMeasuringConditionsListDialog::zh_checkData()
{
    if(zp_gainFactor() < 0 || zp_exposition() < 0)
    {
        QString msg = tr("Measuring conditions are not selected.");
        zv_messageLabel->setText(QString("<font color=red>%1</font>").arg(msg));
        return false;
    }

    return true;
}
//======================================================
void ZMeasuringConditionsListDialog::zh_onOkButtonClick()
{
    if(!zh_checkData())
    {
        return;
    }
    accept();
}
//======================================================
void ZMeasuringConditionsListDialog::zh_newConditions()
{
    ZMeasuringConditionsDialog dialog;
    connect(&dialog, &ZMeasuringConditionsDialog::zg_checkMeasuringConditions,
            this, &ZMeasuringConditionsListDialog::zh_checkMeasuringConditions);

    if(!dialog.exec())
    {
        return;
    }

    bool res = false;
    zh_checkMeasuringConditions(dialog.zp_gainFactor(),
                                dialog.zp_exposition(),
                                res);
    if(res)
    {
        zh_setCurrentMeasuringConditions(dialog.zp_gainFactor(),
                                         dialog.zp_exposition());
        return;
    }

    //    QString sampleTaskName = dialog.zp_sampleTaskName();
    //    if(sampleTaskName.isEmpty())
    //    {
    //        return;
    //    }

    // check out if gain factor exists
    QSqlQuery query;
    QString queryString = "SELECT gain_factor FROM gain_factors";
    if(!query.prepare(queryString))
    {
        QString msg = query.lastError().text();
        QMessageBox::critical(this, tr("Error"), tr("Model data record error: %1").arg(msg), QMessageBox::Ok);
        return;
    }

    if(!query.exec())
    {
        QString msg = query.lastError().text();
        QMessageBox::critical(this, tr("Error"), tr("Model data record error: %1").arg(msg), QMessageBox::Ok);
        return;
    }

    bool searchRes = false;
    int currentGainFactor;
    QVariant vData;
    bool ok;

    while(query.next())
    {
        vData = query.value(0);
        if(!vData.isValid() || !vData.canConvert<int>())
        {
            continue;
        }

        currentGainFactor = vData.toInt(&ok);
        if(!ok)
        {
            continue;
        }

        if(dialog.zp_gainFactor() == currentGainFactor)
        {
            searchRes = true;
            break;
        }
    }

    if(!searchRes)
    {
        QSqlRecord record;
        record.append(QSqlField("gain_factor", QVariant::Int));
        record.append(QSqlField("energyFactorK0", QVariant::Int));
        record.append(QSqlField("energyFactorK1", QVariant::Int));
        record.append(QSqlField("energyFactorK2", QVariant::Int));

        record.setValue(0, QVariant(dialog.zp_gainFactor()));
        record.setValue(1, QVariant(0.0));
        record.setValue(2, QVariant(0.0));
        record.setValue(3, QVariant(0.0));

        if(!zv_gainFactorModel->insertRecord(-1, record))
        {
            QString msg = zv_gainFactorModel->lastError().text();
            QMessageBox::critical(this, tr("Error"), tr("Model data record error: %1").arg(msg), QMessageBox::Ok);
            return;
        }

        if(!zv_gainFactorModel->submitAll())
        {
            QString msg = zv_gainFactorModel->lastError().text();
            QMessageBox::critical(this, tr("Error"), tr("Database record error: %1").arg(msg), QMessageBox::Ok);
            zv_gainFactorModel->revertAll();
            return;
        }
    }

    QSqlRecord record;
    record.append(QSqlField("id", QVariant::Int));
    record.append(QSqlField("gain_factor", QVariant::Int));
    record.append(QSqlField("exposition", QVariant::Int));

    int id = zh_findNewMeasurementConditionsId();

    record.setValue(0, QVariant(id));
    record.setValue(1, QVariant(dialog.zp_gainFactor()));
    record.setValue(2, QVariant(dialog.zp_exposition()));

    if(!zv_measuringConditionsModel->insertRecord(-1, record))
    {
        QString msg = zv_measuringConditionsModel->lastError().text();
        QMessageBox::critical(this, tr("Error"), tr("Model data record error: %1").arg(msg), QMessageBox::Ok);
        return;
    }

    if(!zv_measuringConditionsModel->submitAll())
    {
        QString msg = zv_measuringConditionsModel->lastError().text();
        QMessageBox::critical(this, tr("Error"), tr("Database record error: %1").arg(msg), QMessageBox::Ok);
        zv_measuringConditionsModel->revertAll();
    }

    int row = zv_measuringConditionsModel->rowCount() - 1;
    QModelIndex insertedIndex = zv_measuringConditionsModel->index(row, 0);
    zv_measuringConditionsTable->setCurrentIndex(insertedIndex);
    zv_measuringConditionsTable->scrollTo(insertedIndex);
}
//======================================================
void ZMeasuringConditionsListDialog::zh_setCurrentMeasuringConditions(int gainFactor,
                                 int exposition)
{
    QModelIndex index;
    QVariant vData;
    bool ok;
    int currentGF;
    int currentExpo;
    for(int row = 0; row < zv_measuringConditionsModel->rowCount(); row++)
    {
        // G.F.
        index = zv_measuringConditionsModel->index(row, 1);
        if(!index.isValid())
        {
            continue;
        }
        vData = index.data(Qt::DisplayRole);
        if(!vData.isValid() || !vData.canConvert<int>())
        {
            continue;
        }
        currentGF = vData.toInt(&ok);
        if(!ok)
        {
            continue;
        }

        if(currentGF != gainFactor)
        {
            continue;
        }

        // Expo
        index = zv_measuringConditionsModel->index(row, 2);
        if(!index.isValid())
        {
            continue;
        }
        vData = index.data(Qt::DisplayRole);
        if(!vData.isValid() || !vData.canConvert<int>())
        {
            continue;
        }
        currentExpo = vData.toInt(&ok);
        if(!ok)
        {
            continue;
        }

        // compare
        if(currentExpo == exposition)
        {
            zv_measuringConditionsTable->setCurrentIndex(index);
            zv_measuringConditionsTable->scrollTo(index);
            return;
        }
    }
}
//======================================================
int ZMeasuringConditionsListDialog::zh_findNewMeasurementConditionsId()
{
    QString queryString = QString("SELECT id FROM measuring_conditions");
    QSqlQuery query;
    if(!query.prepare(queryString))
    {
        return -1;
    }

    if(!query.exec())
    {
        return -1;
    }

    int id = 0;
    int currentId;
    QVariant vData;
    while(query.next())
    {
        vData = query.value(0);
        if(!vData.isValid() || !vData.canConvert<int>())
        {
            continue;
        }
        currentId = vData.toInt();
        if(id < currentId)
        {
            id = currentId;
        }
    }

    return ++id;
}
//======================================================
void ZMeasuringConditionsListDialog::zh_checkMeasuringConditions(int gf,
                                                                 int expo,
                                                                 bool& res) const
{
    QModelIndex index;
    QVariant vData;
    bool ok;
    int currentGF;
    int currentExpo;
    for(int row = 0; row < zv_measuringConditionsModel->rowCount(); row++)
    {
        // G.F.
        index = zv_measuringConditionsModel->index(row, 1);
        if(!index.isValid())
        {
            continue;
        }
        vData = index.data(Qt::DisplayRole);
        if(!vData.isValid() || !vData.canConvert<int>())
        {
            continue;
        }
        currentGF = vData.toInt(&ok);
        if(!ok)
        {
            continue;
        }

        // Expo
        index = zv_measuringConditionsModel->index(row, 2);
        if(!index.isValid())
        {
            continue;
        }
        vData = index.data(Qt::DisplayRole);
        if(!vData.isValid() || !vData.canConvert<int>())
        {
            continue;
        }
        currentExpo = vData.toInt(&ok);
        if(!ok)
        {
            continue;
        }

        // compare
        if(gf == currentGF && expo == currentExpo)
        {
            res = true;
            return;
        }
    }

    res = false;
}
//======================================================
