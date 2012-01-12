#ifndef PHYXCALCULATOR_H
#define PHYXCALCULATOR_H

#include <QObject>
#include <QStack>
#include <QDateTime>
#include <QDebug>
#include <QFile>
#include "cmath"
#include "qearleyparser.h"
#include "phyxunitsystem.h"
#include "phyxvariable.h"
#include "phyxtesting.h"

typedef long double         PhyxValueDataType;      /// the base data type for values
typedef int               PhyxUnitDataType;       /// the base data type for units

typedef struct {
    QStringList functions;                          /// a list of functions to call
} PhyxRule;

class PhyxCalculator : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString expression READ expression WRITE setExpression)
    Q_PROPERTY(bool error READ hasError)
    Q_PROPERTY(int errorNumber READ errorNumber)
    Q_PROPERTY(PhyxValueDataType resultValue READ resultValue)
    Q_PROPERTY(QString resultUnit READ resultUnit)

public:
    explicit PhyxCalculator(QObject *parent = 0);

    bool setExpression (QString m_expression);                                  ///< sets the expression, checks what must be parsed and returns wheter the expression is parsable or not
    bool evaluate();                                                            ///< evaluates the expression

    QString expression() const
    {
        return m_expression;
    }

    bool hasError() const
    {
        return m_error;
    }

    int errorNumber() const
    {
        return m_errorNumber;
    }

    QString errorString() const;

    PhyxValueDataType resultValue() const
    {
        return m_resultValue;
    }

    QString resultUnit() const
    {
        return m_resultUnit;
    }

private:
    QStack<PhyxValueDataType>   valueStack;                                     /// stack for value calculation
    QStack<PhyxUnitDataType>    unitStack;                                      /// stack for unit calculation
    QStack<PhyxVariable*>       variableStack;
    QString                     numberBuffer;                                   /// string for buffering numbers
    QString                     parameterBuffer;                                /// string for buffering numbers
    QString                     stringBuffer;                                   /// string for buffering strings
    PhyxValueDataType           valueBuffer;
    QString                     unitBuffer;

    QHash<QString, PhyxRule>    phyxRules;                                      /// map of all rules, key is rule

    QEarleyParser               earleyParser;                                   /// the earley parser

    QString                     m_expression;                                     /// currently set expression
    bool                        expressionIsParsable;                           /// holds wheter currently set expression is parsable or not

    PhyxUnitSystem              *unitSystem;

    QHash<QString, void (PhyxCalculator::*)()> functionMap;                     /// functions mapped with their names

    bool m_error;
    int m_errorNumber;
    PhyxValueDataType m_resultValue;
    QString m_resultUnit;

    void initialize();                                                                                          ///< initializes PhyxCalculator
    void loadGrammar(QString fileName);                                                                         ///< loads the grammar from a file

    void raiseException(QString exception);                                                                     ///< raises an exception
    void addRule(QString rule, QString functions = "");     ///< adds a rule

    /** functions for value calculation */
    void valueAdd();
    void valueSub();
    void valueMul();
    void valueDiv();
    void valueNeg();
    void valuePow();
    void valuePow2();
    void valuePow3();
    void valueSin();
    void valueArcsin();
    void valueCos();
    void valueArccos();
    void valueTan();
    void valueArctan();
    void valueSinh();
    void valueArcsinh();
    void valueCosh();
    void valueArccosh();
    void valueTanh();
    void valueArctanh();
    void valueExp();
    void valueLn();
    void valueLog10();
    void valueLog2();
    void valueLogn();
    void valueRoot();
    void valueSqrt();
    void valueAbs();
    void valueMax();
    void valueMin();
    void valuePi();
    void valueInt();
    void valueTrunc();
    void valueFloor();
    void valueRound();
    void valueCeil();
    void valueSign();
    void valueHeaviside();
    void valueRand();
    void valueRandint();
    void valueRandg();
    void valueFaculty();

    /** functions for unit calculation */
    void unitCheckDimensionless();
    void unitCheckConvertible();
    void unitMul();
    void unitDiv();
    void unitPow();
    void unitPow2();
    void unitPow3();
    void unitRoot();
    void unitSqrt();

    /** functions for number generation */
    void numberBuf()                        {numberBuffer.prepend(parameterBuffer);}
    void numberPush()                       {valueStack.push(parameterBuffer.toDouble());
                                             numberBuffer.clear();}

    /** functions for variable handling */
    void variableAdd();
    void variableRemove();
    void variablePush();

    /** functions for buffering */
    void bufferUnit();
    void bufferValue();
    void pushVariable();

    void outputVariable();

signals:
    
public slots:

private slots:
    void addUnitRule(QString symbol);
    void removeUnitRule(QString symbol);
    
};

#endif // PHYXCALCULATOR_H
