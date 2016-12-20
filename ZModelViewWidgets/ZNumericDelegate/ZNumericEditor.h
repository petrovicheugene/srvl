//================================================================
#ifndef ZNUMERICEDITOR_H
#define ZNUMERICEDITOR_H
//================================================================
#include <QAbstractSpinBox>
#include <QCloseEvent>
//================================================================

//================================================================
class ZNumericEditor : public QAbstractSpinBox
{
    Q_OBJECT
public:
    explicit ZNumericEditor(QWidget *parent = 0);
    
    virtual void	stepBy ( int steps );
    virtual void keyPressEvent(QKeyEvent *event);

    void setMinMax(double min, double max);

    void setText(QString numericString);
    void setValue(double value);

    QString text();
    double value();

    QSize sizeHint() const;

protected:

    // VARS
    double zv_min;
    double zv_max;

    // FUNCS
    virtual StepEnabled stepEnabled () const;
    bool checkNewNumber(QStringList parts,
                        bool powerTypeRecord,
                        qint64 changedPartNumeric,
                        int changedPartNumber,
                        QString &newNumericString,
                        int &newPartCount);


signals:

    void enterClicked();
    void valueChanged(double);

public slots:
    

protected slots:

    void zp_checkChangedText(const QString&);

};
//================================================================
#endif // ZNUMERICEDITOR_H
