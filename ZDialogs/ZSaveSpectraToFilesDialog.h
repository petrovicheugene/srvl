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
class ZSaveSpectraToFilesDialog : public QDialog
{
    Q_OBJECT
public:
    explicit ZSaveSpectraToFilesDialog(const QMap< QPair<quint8, int>, QList<ZSpeSpectrum*> >& spectrumMap,
                                       QWidget *parent = nullptr);

signals:

public slots:


private:

    // VARS
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
