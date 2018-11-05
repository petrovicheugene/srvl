//===============================================
#include "ZQrealToStringConverter.h"
//===============================================
char ZQrealToStringConverter::zv_format = 'g';
int ZQrealToStringConverter::zv_precision = 15;
//===============================================
ZQrealToStringConverter::ZQrealToStringConverter()
{

}
//===============================================
bool ZQrealToStringConverter::zp_setFormat(char format)
{
    if(format != 'g' && format != 'G' && format != 'f' && format != 'F')
    {
        return false;
    }

    zv_format = format;
    return true;
}
//===============================================
bool ZQrealToStringConverter::zp_setPrecision(int precision)
{
    if(precision < 0)
    {
        return false;
    }

    zv_precision = precision;
    return true;
}
//===============================================
QString ZQrealToStringConverter::zp_convert(qreal value)
{
    if(value != value)
    {
        return QString::number(value);
    }

    QString valueString;
    if(qAbs(value) < 1)
    {
        bool started = false;
        valueString = QString::number(value, 'f', zv_precision);
        // first zero count after point
        int firstZeroCount = 0;
        for(int i = 0; i < valueString.count(); i++)
        {
            if(!started && (valueString.at(i) == '.' || valueString.at(i) == ','))
            {
                started = true;
                continue;
            }

            if(valueString.at(i) == '0')
            {
                firstZeroCount++;
                continue;
            }

            break;
        }

        if(firstZeroCount > 9)
        {
            valueString = QString::number(value, zv_format, zv_precision);
            if(zv_format == 'f')
            {
                // chop tail zero if they exist
                for(int i = valueString.count() - 1; i > 0; i--)
                {
                    if(valueString.at(i) != '0' || (valueString.at(i - 1) == '.' || valueString.at(i - 1) == ',' ))
                    {
                        break;
                    }
                    valueString.chop(1);
                }
            }
        }
        else
        {
            // chop tail zero if they exist
            for(int i = valueString.count() - 1; i > 0; i--)
            {
                if(valueString.at(i) != '0' || (valueString.at(i - 1) == '.' || valueString.at(i - 1) == ',' ))
                {
                    break;
                }
                valueString.chop(1);
            }
        }
    }
    else
    {
        valueString = QString::number(value, zv_format, zv_precision);
        if(zv_format == 'f')
        {
            // chop tail zero if they exist
            for(int i = valueString.count() - 1; i > 0; i--)
            {
                if(valueString.at(i) != '0' || (valueString.at(i - 1) == '.' || valueString.at(i - 1) == ',' ))
                {
                    break;
                }
                valueString.chop(1);
            }
        }
    }

    return valueString;
}
//===============================================
void ZQrealToStringConverter::zh_chopTailZeroesFromDoubleString(QString& string)
{
    // check string
    bool ok;
    string.toDouble(&ok);
    if(!ok)
    {
        return;
    }

    // check format
    if(string.contains('e') || string.contains('E'))
    {
        //  format g
        return;
    }

    for(int i = string.count() - 1; i > 0; i--)
    {
        if(string.at(i) != '0' || (string.at(i - 1) == '.' || string.at(i - 1) == ',' ))
        {
            break;
        }
        string.chop(1);
    }
}
//===============================================
