//======================================================
#ifndef ZSAVESPECTRATOFILESDIALOG_H
#define ZSAVESPECTRATOFILESDIALOG_H
//======================================================
#include <QDialog>
#include <QList>
#include <QMap>
#include <QPair>
//======================================================
class QLabel;
class QLineEdit;
class QPushButton;

class ZSpeSpectrum;
//======================================================
class ZSpectrumArraySettingsWidget : public QWidget
{
    Q_OBJECT
public:
    explicit ZSpectrumArraySettingsWidget(QPair<quint8, int>* conditions,
                                          QWidget *parent = nullptr);

    ~ZSpectrumArraySettingsWidget();

    QString zp_folderPath() const;
    QString zp_fileNameTemplate() const;

signals:

public slots:

private slots:

    void zh_onBrowseButtonClick();

private:

    // VARS
    QPair<quint8, int>* zv_conditions;
    QString zv_folderPath;
    QLineEdit* zv_pathLineEdit;
    QLineEdit* zv_spectrumFileNameTemplateLineEdit;
    QPushButton* zv_browseButton;

    // FUNCS
    void zh_createComponents();
    void zh_createConnections();
    void zh_restoreSettings();
    void zh_saveSettings();

};
//======================================================
class ZSaveSpectraToFilesDialog : public QDialog
{
    Q_OBJECT
public:
    explicit ZSaveSpectraToFilesDialog(const QMap< QPair<quint8, int>, QList<ZSpeSpectrum*> >& spectrumMap,
                                       QWidget *parent = nullptr);

signals:

public slots:

private slots:

    void zh_onOkButtonClick();
    void zh_onCancelButtonClick();

private:

    // VARS
    QMap< QPair<quint8, int>, QList<ZSpeSpectrum*> > zv_spectrumMap;

    QMap< QPair<quint8, int>, ZSpectrumArraySettingsWidget*> zv_spectrumArraySettingsWidgetList;
    QLineEdit* zv_pathLineEdit;
    QLineEdit* zv_fileNameTemplate;

    QPushButton* zv_browseButton;
    QPushButton* zv_okButton;
    QPushButton* zv_cancelButton;

    // FUNCS
    void zh_createComponents();
    void zh_createConnections();

};
//======================================================
#endif // ZSAVESPECTRATOFILESDIALOG_H
