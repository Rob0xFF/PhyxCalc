#ifndef PHEXPARSER_H
#define PHEXPARSER_H

#include <QObject>
#include <QMap>
#include <QMapIterator>
#include <QRegExp>
#include <QDebug>
#include <udunits2.h>
#include "muParser/muParser.h"
#include "global.h"

typedef struct {
    int     pos;
    QString text;
} queryHit;

class PhexParser : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString expression READ expression WRITE setExpression)
    Q_PROPERTY(QString resultUnit READ resultUnit)
    Q_PROPERTY(QString resultValue READ resultValue)
    //Q_PROPERTY(QMap<QString, physicalVariable> variableMap READ variableMap WRITE setVariableMap)

public:
    explicit PhexParser(QObject *parent = 0);
    ~PhexParser();

    QString expression() const {return m_expression;}
    QString resultUnit() const {return m_resultUnit;}
    double  resultValue() const {return m_resultValue;}
    //QMap<QString, physicalVariable> variableMap() const {return m_variableMap;}

    bool evaluate();

    bool convert(QString inputUnit, double inputValue, QString outputUnit, double *outputValue);

signals:

public slots:
    void setExpression(const QString string)    {m_expression = string;
                                                m_resultValue = 0;
                                                m_resultUnit = "";}
    //void setVariableMap(const QMap<QString, physicalVariable> map) {m_variableMap = map;}

private:
    ut_system  *unitSystem;
    mu::Parser *muParser;

    QString m_expression;
    double  m_resultValue;
    QString m_resultUnit;
    QMap<QString, physicalVariable> m_variableMap;

    QMap<QString, double> siPrefixMap;

    void initializeUdunits2();
    void initializeMuParser();
    void initializeSiPrefixes();
    //QString removeUnits(QString string);
    QString removeValues(QString string);
    QString removeWhitespace(QString string);
    //QString replaceVariables(QString string, bool insertValue, bool insertUnit);
    QString replaceSiPrefixes(QString string);
    QString removeSymbols(QString string);

    QString removeAddSub(QString string);
    QString removeUnneededClips(QString string);
    QString simplifyMultDiv(QString string);
    QString simplifyExpression(QString expression);

    bool evaluateValue(QString expressionString, double *result);
    bool evaluateUnit(QString expressionString, QString *result);

    void preFormatExpression(QString string, QString *unitExpression, QString *valueExpression);

};

#endif // PHEXPARSER_H
