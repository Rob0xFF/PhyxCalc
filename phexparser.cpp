#include "phexparser.h"

PhexParser::PhexParser(QObject *parent) :
    QObject(parent)
{
    initializeUdunits2();
    initializeMuParser();
    initializeSiPrefixes();
}

PhexParser::~PhexParser()
{
    ut_free_system(unitSystem);
    delete muParser;
}

void PhexParser::initializeUdunits2()
{
    unitSystem = ut_read_xml(NULL);

    if (unitSystem == NULL)
        qWarning("PhexParser: ERROR loading unit system");
    else
        qDebug() << "PhexParser: loading unit system succeded!";
}

void PhexParser::initializeMuParser()
{
    muParser = new mu::Parser;
}

void PhexParser::initializeSiPrefixes()
{
    //initialize SI prefixes
    siPrefixMap["Y"] = 1e24;
    siPrefixMap["Z"] = 1e21;    //zetta
    siPrefixMap["E"] = 1e18;    //exa
    siPrefixMap["P"] = 1e15;    //peta
    siPrefixMap["T"] = 1e12;    //tera
    siPrefixMap["G"] = 1e9;     //giga
    siPrefixMap["M"] = 1e6;     //mega
    siPrefixMap["k"] = 1e3;     //kilo
    siPrefixMap["h"] = 1e2;     //hecto
    //siPrefixMap["da"] = pow(10,1);
    siPrefixMap["d"] = 1e-1;    //deci;
    siPrefixMap["c"] = 1e-2;    //centi
    siPrefixMap["m"] = 1e-3;    //milli
    siPrefixMap[QString::fromLocal8Bit("µ")] = 1e-6;    //micro
    siPrefixMap["n"] = 1e-9;    //nano
    siPrefixMap["p"] = 1e-12;   //piko
    siPrefixMap["f"] = 1e-15;   //femto
    siPrefixMap["a"] = 12-18;   //atto
    siPrefixMap["z"] = 1e-21;   //zepto
    siPrefixMap["y"] = 1e-24;   //yocto
}

QString PhexParser::removeValues(QString string)
{
    //replace variables
    //string = replaceVariables(string, false, true);

    //handle e
    string.replace("e-","");
    string.replace("e","");

    //remove everything that has a unit and replace everything else with 1
    for (int i = 0; i < string.size(); i++)
    {
        if (string.at(i).isNumber())
        {
            int pos;
            pos = string.indexOf(QRegExp("[^0-9.]"), i);
            if ((i == 0) || (string.at(i-1) != '^'))                 //don't remove powers
            {
                if (pos == -1)
                {
                    string.chop(string.size()-i);
                    string.append("1");
                }
                else
                {
                    //if (unitLoader->siPrefixes()->contains(QString(string.at(pos))))
                    //    string.remove(pos,1);
                    if (QString(string.at(pos)).indexOf(QRegExp(MATH_OPERATORS)) != -1)
                    {
                        string.remove(i, pos-i);
                        string.insert(i,"1");
                    }
                    else
                    {
                        string.remove(i, pos-i);

                        //remove -
                        if ((i>0) && (string.at(i-1) == '-'))
                        {
                            if ((i == 1) || (QString(string.at(i-2)).indexOf(QRegExp(MATH_OPERATORS)) != -1))
                            {
                                string.remove(i-1,1);
                                i--;
                            }
                        }
                    }
                }
            }
            else
            {
                if (pos == -1)
                    break;
                else
                    i = pos;
            }
        }
    }

    string = removeAddSub(string);

    //qDebug() << string;

    return string;
}

QString PhexParser::removeWhitespace(QString string)
{
    string = string.simplified();
    string.replace(" ", "");
    return string;
}

bool PhexParser::evaluateValue(QString expressionString, double *result)
{
    mu::string_type expressionBuffer = expressionString.toLocal8Bit().data();
    try
    {
        muParser->SetExpr(expressionBuffer);
        *result = muParser->Eval();

        qDebug() << "PhexParser: parsing value succeded!";
        return true;
    }
    catch(mu::Parser::exception_type &e)
    {
        qDebug() << "PhexParser: parsing value failed!";
        return false;
    }
}

bool PhexParser::evaluateUnit(QString expressionString, QString *result)
{
    QByteArray bytes = expressionString.toUtf8();
    char buffer[128];//(char*)malloc(sizeof(bytes.data()) * sizeof(char));
    //string = bytes.data();
    strcpy(buffer, bytes.data());
    ut_unit*     unit = ut_parse(unitSystem, buffer, UT_UTF8);

    if (unit == NULL)
    {
        qDebug() << "PhexParser: parsing unit failed!";

        ut_free(unit);
        return false;
    }

     unsigned    opts = UT_UTF8;
     int         len = ut_format(unit, buffer, sizeof(buffer), UT_UTF8);

     if (len == -1) {
         // Couldn't get string
     }
     else if (len == sizeof(buffer)) {
         // Entire buffer used: no terminating NUL
     }
     else {
         // Have string with terminating NUL
          *result = QString::fromLocal8Bit(buffer);
         if (*result == "1")
             *result = "";
     }

    qDebug() << "PhexParser: parsing unit succeded!";

    ut_free(unit);
    return true;
}

bool PhexParser::evaluate()
{
    QString unitExpression,
            valueExpression;

    m_expression = removeWhitespace(m_expression);

    preFormatExpression(m_expression, &unitExpression, &valueExpression);
    evaluateValue(valueExpression, &m_resultValue);
    evaluateUnit(simplifyExpression(unitExpression), &m_resultUnit);

    qDebug() << "PhexParser: result:" << m_resultValue << "unit:" << m_resultUnit;

    return true;
}

void PhexParser::preFormatExpression(QString string, QString *unitExpression, QString *valueExpression)
{
    QMap<int, QString> unitHits;
    QMap<int, QString> prefixHits;
    int baseLength = string.size();
    int maxLength;

    if (baseLength < 10)
        maxLength = baseLength;
    else
        maxLength = 10;


    //scan units with udunits
    for (int i = maxLength; i > 0; i--)
    {
        int n = baseLength-i+1;
        for (int j = n-1; j >= 0; j--)
        {
            ut_unit* unit = ut_get_unit_by_symbol(unitSystem, string.mid(j,i).toLocal8Bit());//ut_parse(unitSystem,string.mid(j,i).toLocal8Bit(),UT_UTF8);

            if ((ut_get_status() == UT_SUCCESS) && (unit != NULL))
            {
                char        buf[128];
                int         len = ut_format(unit, buf, sizeof(buf), UT_UTF8);
                if (len != -1)
                {
                    if (!unitHits.contains(j))
                    {
                        QString unitString = QString::fromLocal8Bit(buf);;

                        //fix e number format
                        if (string.at(j) == 'e')
                        {
                            if ((j == 0) || !string.at(j-1).isNumber())
                                unitHits[j] = unitString;
                        }
                        else
                            unitHits[j] = unitString;


                        //qDebug() << unitHits.value(j);
                    }
                }
            }
        }
    }

    //find prefixes
    for (int i = 0; i < string.size(); i++)
    {
        if (QString(string.at(i)).indexOf(QRegExp("[yzafpnµmcdhkMGTPEZY]")) != -1)
        {
            prefixHits[i] = QString(string.at(i));
           // qDebug() << prefixHits.value(i);
        }
    }

    //remove units contained by others
    QMapIterator<int, QString> k(unitHits);
    k.toFront();
    while (k.hasNext())
    {
        k.next();
        for (int i = 1; i < k.value().size(); i++)
        {
            if (unitHits.contains(k.key()+i))
                unitHits.remove(k.key()+i);
        }
    }

    //remove false prefixes and units
    QMapIterator<int, QString> l(prefixHits);
    while (l.hasNext())
    {
        l.next();
        if (!unitHits.contains(l.key()+1))
        {
            prefixHits.remove(l.key());
        }
        else
        {
            if (unitHits.contains(l.key()))
            {
                unitHits.remove(l.key());
            }
        }
    }

    //remove middle dot
    string.replace(QString::fromUtf8("·"), ".");

    //remove superscripts
    string.replace(QString::fromUtf8("⁻¹"),"^-1");
    string.replace(QString::fromUtf8("⁻²"),"^-2");
    string.replace(QString::fromUtf8("⁻³"),"^-3");
    string.replace(QString::fromUtf8("⁻⁴"),"^-4");
    string.replace(QString::fromUtf8("⁻⁵"),"^-5");
    string.replace(QString::fromUtf8("⁻⁶"),"^-6");
    string.replace(QString::fromUtf8("⁻⁷"),"^-7");
    string.replace(QString::fromUtf8("⁻⁸"),"^-8");
    string.replace(QString::fromUtf8("⁻⁹"),"^-9");
    string.replace(QString::fromUtf8("¹"),"^1");
    string.replace(QString::fromUtf8("²"),"^2");
    string.replace(QString::fromUtf8("³"),"^3");
    string.replace(QString::fromUtf8("⁴"),"^4");
    string.replace(QString::fromUtf8("⁵"),"^5");
    string.replace(QString::fromUtf8("⁶"),"^6");
    string.replace(QString::fromUtf8("⁷"),"^7");
    string.replace(QString::fromUtf8("⁸"),"^8");
    string.replace(QString::fromUtf8("⁹"),"^9");
    string.replace(QString::fromUtf8("⁰"),"^0");

    *valueExpression = string;

    //remove dots in units
    //valueExpression->replace(".","*");

    //replace the units with the necessary string
    for (int i = valueExpression->size()-1; i >= 0; i--)
    {
        if (unitHits.contains(i))
        {
            QString replacement;
            valueExpression->remove(i, unitHits.value(i).size());

            if ((i>0) && (QString(valueExpression->at(i-1)).indexOf(QRegExp(MATH_OPERATORS)) == -1))
                replacement.append("*");
            replacement.append("1");
            valueExpression->insert(i, replacement);
        }
    }
    *valueExpression = replaceSiPrefixes(*valueExpression);

    *unitExpression = string;
    //remove siPrefixes

    //replace --
    unitExpression->replace("--", "+");

    for (int i = unitExpression->size()-1; i >= 0; i--)
    {
        if (prefixHits.contains(i))
        {
            unitExpression->remove(i, prefixHits.value(i).size());
        }
    }

    *unitExpression = removeValues(*unitExpression);


    qDebug() << "PhexParser: value expression:" << *valueExpression;
    qDebug() << "PhexParser: unit expression" << *unitExpression;
}

QString PhexParser::replaceSiPrefixes(QString string)
{
    QMapIterator<QString, double> i(siPrefixMap);
    while (i.hasNext())
    {
        i.next();
        string.replace(i.key(), tr("e%1").arg((int)log10(i.value())));   //uses e format, helps for sticking data better together
    }

    return string;
}

bool PhexParser::convert(QString inputUnit, double inputValue, QString outputUnit, double *outputValue)
{
    QByteArray bytes;
    char buffer1[128];
    char buffer2[128];
    bytes = inputUnit.toUtf8();
    strcpy(buffer1, bytes.data());
    bytes = outputUnit.toUtf8();
    strcpy(buffer2, bytes.data());

    ut_unit*     unit1 = ut_parse(unitSystem, buffer1, UT_UTF8);
    ut_unit*     unit2 = ut_parse(unitSystem, buffer2, UT_UTF8);

    if ((unit1 == NULL) || (unit2 == NULL))
    {
        qDebug() << "PhexParser: parsing unit failed!";
        ut_free(unit1);
        ut_free(unit2);
        return false;
    }

    cv_converter* converter = ut_get_converter(unit1, unit2);

    if (ut_get_status() == UT_NOT_SAME_SYSTEM)
    {
        qDebug() << "PhexParser: unit1 and unit2 belong to different unit systems!";
        ut_free(unit1);
        ut_free(unit2);
        return false;
    }
    else if (ut_get_status() == UT_MEANINGLESS)
    {
        qDebug() << "PhexParser: conversion between unit1 and unit2 is meanigless!";
        ut_free(unit1);
        ut_free(unit2);
        return false;
    }
    else if (ut_get_status() == UT_OS)
    {
        qDebug() << "PhexParser: operating system failure";
        ut_free(unit1);
        ut_free(unit2);
        return false;
    }
    else
    {
        *outputValue = cv_convert_double(converter, inputValue);
        cv_free(converter);
        qDebug() << "PhexParser: converting unit1 to unit2 succeded!";
        ut_free(unit1);
        ut_free(unit2);
        return true;
    }

}

QString PhexParser::removeAddSub(QString string)
{
    //remove additions und substractions
    for (int i = 1; i < string.size(); i++)
    {
        if ((string.at(i) == '+') || (string.at(i) == '-'))
        {
            int lastPos = string.lastIndexOf(QRegExp(MATH_OPERATORS),i-1);
            int nextPos = string.indexOf(QRegExp(MATH_OPERATORS),i+1);

            QString left = string.mid(lastPos+1, i-lastPos-1);
            if (nextPos == -1)
                nextPos = string.size()+1;
            QString right = string.mid(i+1, nextPos-i-1);
            if (left == right)
            {
                if ((string.at(lastPos) != '*') &&
                    (string.at(lastPos) != '/') &&
                    (string.at(nextPos) != '*') &&
                    (string.at(nextPos) != '/'))
                {
                    string.remove(i, nextPos-i);
                    i--;
                }
            }
        }
    }

    return string;
}

QString PhexParser::removeUnneededClips(QString string)
{
    int in = -1;
    int out = -1;
    //int lastOperator = -1;

    //lastOperator = string.length();
    for (int i = string.length()-1; i >= 0; i--)
    {
        if (string.at(i) == ')')
        {
            //if (out == -1)
                out = i;
        }
        else if (string.at(i) == '(')
        {
            in = i;
        }
        else if (!string.at(i).isLetterOrNumber())
        {
            in = -1;
            out = -1;
        }

        if ((in != -1) && (out != -1))
        {
            string.remove(out,1);
            string.remove(in,1);
            in = -1;
            out = -1;
        }
    }
    return string;
}

QString PhexParser::simplifyMultDiv(QString string)
{
    int in = -1;
    int out = -1;
    int lastOperator = -1;

    string.replace(".","*");

    lastOperator = string.length();
    for (int i = string.length()-1; i >= 0; i--)
    {
        if ((string.at(i) == '*') || (string.at(i) == '\\'))
        {
            if (out == -1)
                out = i;
        }
        else if (!string.at(i).isLetterOrNumber() || (i==0))
        {
            if (out != -1)
            {
                if (string.at(i).isLetterOrNumber())
                    in = 0;
                else
                    in = i+1;
            }
            else
                lastOperator = i;
        }

        if ((in != -1) && (out != -1))
        {
            QString newUnit;
            QString oldUnit = string.mid(in, lastOperator-in);
            if (evaluateUnit(oldUnit, &newUnit))
            {
                string.remove(in, lastOperator-in);
                string.insert(in, newUnit);
            }
            qDebug() << in << out << string.mid(in, lastOperator-in);
            in = -1;
            out = -1;
            lastOperator = i;
        }
    }

    return string;
}

QString PhexParser::simplifyExpression(QString expression)
{
    QString newExpression;

    while (true)
    {
        newExpression = expression;
        newExpression = removeAddSub(newExpression);
        newExpression = removeUnneededClips(newExpression);
        newExpression = simplifyMultDiv(newExpression);

        qDebug() << newExpression;
        if (newExpression != expression)
            expression = newExpression;
        else
            break;
    }

    return expression;
}
