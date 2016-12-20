//================================================================
#include "ZNumericEditor.h"
#include <QLineEdit>
#include <limits>
#include <QDebug>
#include <math.h>
#include <QMessageBox>
#include <QKeyEvent>
#include <QFontMetrics>
#include <QStyle>
#include <QStyleOptionComplex>
//================================================================
ZNumericEditor::ZNumericEditor(QWidget *parent) :
    QAbstractSpinBox(parent)
{
    zv_min = (double)std::numeric_limits<long long>::min();
    zv_max = (double)std::numeric_limits<long long>::max();

    //int maxPartSymbols = QString::number(m_max).count();
    lineEdit()->setAlignment(Qt::AlignVCenter | Qt::AlignRight);
    lineEdit()->setValidator(new QDoubleValidator());
    lineEdit()->setText("0.0");
    connect(lineEdit(), &QLineEdit::textChanged,
            this, &ZNumericEditor::zp_checkChangedText);

}
//================================================================
ZNumericEditor::StepEnabled	ZNumericEditor::stepEnabled () const
{
    return ZNumericEditor::StepDownEnabled | ZNumericEditor::StepUpEnabled;
}
//================================================================
void	ZNumericEditor::stepBy ( int steps )
{
    QString numericString = lineEdit()->text();
    int cursorPos = lineEdit()->cursorPosition();

    //double doubleValue = numericString.toDouble();

    numericString.replace(QRegExp(","), ".");
    //numericString.replace(QRegExp("e"), "E");

    QStringList parts = numericString.split(QRegExp("[,.Ee]"),QString::KeepEmptyParts);

    for(int i  = 0; i < parts.count(); i++)
    {
        if(parts.value(i) == "")
            parts[i] = "0";
    }

    bool powerTypeRecord = numericString.contains(QRegExp("[eE]"));

    // определяем в какой части курсор
    if(parts.isEmpty() || parts.count() > 3)
    {
        return;
    }

    QString newNumericString;
    int changedPartNumber = 0;
    //int cursorPos;

    int firstSeparatorPos;
    int secondSeparatorPos;

    switch(parts.count())
    {
    case 1:

        firstSeparatorPos = 0;
        secondSeparatorPos = 0;
        changedPartNumber = 0;
        cursorPos = lineEdit()->cursorPosition();

        break;
    case 2:

        secondSeparatorPos = numericString.length() - 1;
        firstSeparatorPos = numericString.indexOf(QRegExp("[,.Ee]"));
        if(cursorPos > firstSeparatorPos)
        {
            changedPartNumber = 1;
            cursorPos = lineEdit()->cursorPosition() - firstSeparatorPos - 1;
        }
        else
        {
            changedPartNumber = 0;
            cursorPos = lineEdit()->cursorPosition();
        }

        break;
    case 3:

        firstSeparatorPos = numericString.indexOf(QRegExp("[,.]"));
        secondSeparatorPos = numericString.indexOf(QRegExp("[Ee]"));

        if(cursorPos <= firstSeparatorPos )
        {
            changedPartNumber = 0;
            cursorPos = lineEdit()->cursorPosition();
        }
        else if((cursorPos > firstSeparatorPos) && (cursorPos <= secondSeparatorPos))
        {
            changedPartNumber = 1;
            cursorPos = lineEdit()->cursorPosition() - firstSeparatorPos - 1;
        }
        else if(cursorPos > secondSeparatorPos)
        {
            changedPartNumber = 2;
            cursorPos = lineEdit()->cursorPosition() - secondSeparatorPos - 1;
        }

        break;
    default:
        return;
    }

    qint64 changedPartNumeric = parts.value(changedPartNumber).toLongLong();
    qint64 fractionlPartMax = -1;

    if((powerTypeRecord == false &&  changedPartNumber == 1) ||
            (changedPartNumber == 1 && powerTypeRecord == true && parts.count() == 3))
    {
        QString fractionalPartString;
        for(int i = 0; i < parts.value(changedPartNumber).count(); i++)
        {
            fractionalPartString += "9";
        }
        fractionlPartMax = fractionalPartString.toLongLong();
    }

    bool partValueEqualLimit = false;
    int newPartCount;
    for(int s = 0; s < qAbs(steps); s++)
    {
        if(fractionlPartMax > 0)
        {
            if((changedPartNumeric <= 0 && steps < 0) || (changedPartNumeric >= fractionlPartMax && steps > 0))
            {
                partValueEqualLimit = true;
            }
            else
            {
                changedPartNumeric = steps > 0 ? changedPartNumeric+1 : changedPartNumeric-1;
            }
        }
        else
        {
            changedPartNumeric = steps > 0 ? changedPartNumeric+1 : changedPartNumeric-1;
        }

        if(!checkNewNumber(parts, powerTypeRecord, changedPartNumeric, changedPartNumber, newNumericString,  newPartCount) )
        {
            //откат
            changedPartNumeric = steps > 0 ? changedPartNumeric-1 : changedPartNumeric+1;
            checkNewNumber(parts, powerTypeRecord, changedPartNumeric, changedPartNumber, newNumericString,  newPartCount);

            break;
        }
        if(partValueEqualLimit)
        {
            break;
        }
    }

    lineEdit()->setText(newNumericString);
    bool ok;
    double newValue = newNumericString.toDouble(&ok);
    if(ok)
    {
        emit valueChanged(newValue);
    }

    int newCursorPos = 0;
    switch (changedPartNumber)
    {
    case 0:
        newCursorPos = cursorPos + (newPartCount - parts.value(changedPartNumber).count());
        break;
    case 1:
        newCursorPos = cursorPos + firstSeparatorPos + 1;
        break;
    case 2:
        newCursorPos = cursorPos + secondSeparatorPos + 1;
        break;
    default:

        break;
    }

    if(newCursorPos < 0)
    {
        newCursorPos = 0;
    }
    else if(newCursorPos > newNumericString.count())
    {
        newCursorPos = newNumericString.count();
    }

    lineEdit()->setCursorPosition(newCursorPos);
}
//================================================================
void ZNumericEditor::keyPressEvent(QKeyEvent *event)
{
    if(event->key() == Qt::Key_Return || event->key() == Qt::Key_Enter)
    {
        // qDebug() << "Key Press";


        // setText(lineEdit()->text());
        emit enterClicked();
        return;
    }

    QAbstractSpinBox::keyPressEvent(event);
}
//================================================================
bool ZNumericEditor::checkNewNumber(QStringList parts,
                                    bool powerTypeRecord,
                                    qint64 changedPartNumeric,
                                    int changedPartNumber,
                                    QString& newNumericString,
                                    int& newPartCount)
{
    newNumericString.clear();
    //    newNumericString = QString::number(changedPartNumeric);

    QString partString = QString::number(changedPartNumeric);

    if( (parts.count() == 2 && changedPartNumber == 1 && powerTypeRecord == false) ||
            (parts.count() == 3 && changedPartNumber == 1 ) )
    {
        int charCountDelta = parts.value(changedPartNumber).count() - partString.count();

        for(int i = 0; i < charCountDelta; i++)
        {
            partString = "0"+partString;
        }
    }

    if(parts.count() - 1 == changedPartNumber && powerTypeRecord && changedPartNumeric >= 0)
    {
        partString = "+"+partString;
    }

    for(int i = 0; i < parts.count(); i++)
    {
        newNumericString = i == changedPartNumber ? newNumericString + partString : newNumericString + parts.value(i);
        if((i == 0 && !powerTypeRecord && parts.count() == 2) ||
                (i == 0 && powerTypeRecord && parts.count() == 3))
        {
            newNumericString = newNumericString + ".";
        }

        if( (i == 0 &&  powerTypeRecord && parts.count() == 2) ||
                (i == 1 && powerTypeRecord && parts.count() == 3) )
        {
            newNumericString = newNumericString + "e";
        }
    }

    double newValue = newNumericString.toDouble();

    newPartCount = partString.count();

    if(newValue < zv_min || newValue > zv_max)
    {
        return false;
    }

    return true;
}
//================================================================
void ZNumericEditor::setMinMax(double min, double max)
{
    if(min < max || min < std::numeric_limits<long>::min() || max > std::numeric_limits<long>::max())
    {
        zv_min = min;
        zv_max = max;
    }
}
//================================================================
void ZNumericEditor::setText(QString numericString)
{

    bool removeLastTree = false;
//    if(val < zv_min )
//    {
//        //QMessageBox::warning(0, tr("Limit exceed"),  tr("The value exceeds minimum limit."), QMessageBox::Ok) ;
//        // numericString = QString::number(zv_min, 'g', 18);
//        zv_min = val;
//    }
//    else if(val > zv_max)
//    {
//        //QMessageBox::warning(0, tr("Limit exceed"),  tr("The value exceeds maximum limit."), QMessageBox::Ok) ;
//        // numericString = QString::number(zv_max, 'g', 18);
//        zv_max = val;
//    }

    if(removeLastTree)
    {
        QStringList parts = numericString.split(QRegExp("[,.Ee]"),QString::KeepEmptyParts);

        QString fractionalPart = parts.value(1);
        fractionalPart.remove(fractionalPart.count() - 3 , 3);
        parts[1] = fractionalPart;

        numericString.clear();
        for(int i = 0; i < parts.count(); i++)
        {
            numericString = numericString + parts.value(i);
            if(i == 0)
            {
                numericString = numericString + ".";
            }

            if(i == 1)
            {
                numericString = numericString + "e";
            }
        }
    }

    bool ok;
    double val = numericString.toDouble(&ok);

    if(val < zv_min )
    {
        zv_min = val;
    }
    else if(val > zv_max)
    {
        zv_max = val;
    }


    lineEdit()->setText(numericString);
    lineEdit()->selectAll();
}
//================================================================
void ZNumericEditor::setValue(double value)
{
    lineEdit()->setText(QString::number(value));
}
//================================================================
QString ZNumericEditor::text()
{
    return lineEdit()->text();
}
//================================================================
double ZNumericEditor::value()
{
    return lineEdit()->text().toDouble();
}
//================================================================
QSize ZNumericEditor::sizeHint() const
{
    QSize size = QAbstractSpinBox::sizeHint();
    QFontMetrics fm(this->font());
    QString maxString = QString::number(zv_max, 'f', 15);
    QString minString = QString::number(zv_min, 'f', 15);
    int stringWidth = qMax(fm.width(maxString), fm.width(minString));
    QStyleOptionComplex opt;
    opt.initFrom(this);
    int upDownButtonWidth = this->style()->subControlRect(QStyle::CC_SpinBox, &opt, QStyle::SC_SpinBoxDown, this).width();
    size.setWidth(stringWidth + (upDownButtonWidth));
    return size;
}
//================================================================
void ZNumericEditor::zp_checkChangedText(const QString& newValueString)
{
    if(newValueString.isEmpty())
    {
        lineEdit()->setText(QString::number(0.0));
        return;
    }

    bool ok;
    qreal newValue = newValueString.toDouble(&ok);
    if(!ok && lineEdit()->isUndoAvailable())
    {
        lineEdit()->undo();
    }

    if(newValue > zv_max)
    {
        lineEdit()->setText(QString::number(zv_max));
    }
    else if(newValue < zv_min)
    {
        lineEdit()->setText(QString::number(zv_min));
    }
}
//================================================================
