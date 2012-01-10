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
public:
    explicit PhyxCalculator(QObject *parent = 0);

    bool setExpression (QString m_expression);                                  ///< sets the expression, checks what must be parsed and returns wheter the expression is parsable or not
    bool evaluate();                                                            ///< evaluates the expression

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

    QString                     expression;                                     /// currently set expression
    bool                        expressionIsParsable;                           /// holds wheter currently set expression is parsable or not

    PhyxUnitSystem              *unitSystem;

    QHash<QString, void (PhyxCalculator::*)()> functionMap;                     /// functions mapped with their names

    void initialize();                                                                                          ///< initializes PhyxCalculator
    void loadGrammar(QString fileName);                                                                         ///< loads the grammar from a file

    void raiseException(QString exception);                                                                     ///< raises an exception
    void addRule(QString rule, QString functions = "");     ///< adds a rule

    /** math functions */
    void mathAdd();
    void mathSub();
    void mathMul();
    void mathDiv();
    void mathNeg();
    void mathPow();
    void mathPow2();
    void mathPow3();
    void mathSin();
    /** functions for value calculation */
    void valueAdd()         {valueStack.push(valueStack.pop() + valueStack.pop());}
    void valueSub()         {valueStack.push(valueStack.pop() - valueStack.pop());}
    void valueMul()         {valueStack.push(valueStack.pop() * valueStack.pop());}
    void valueDiv()         {valueStack.push(valueStack.pop() / valueStack.pop());}
    void valueNeg()         {valueStack.push(-valueStack.pop());}
    void valueExponent()    {valueStack.push(pow(valueStack.pop(),valueStack.pop()));}
    void valueExp2()        {valueStack.push(pow(valueStack.pop(),2));}
    void valueExp3()        {valueStack.push(pow(valueStack.pop(),3));}
    void valueSin()         {valueStack.push(sin(valueStack.pop()));}
    void valueArcsin()      {valueStack.push(asin(valueStack.pop()));}
    void valueCos()         {valueStack.push(cos(valueStack.pop()));}
    void valueArccos()      {valueStack.push(acos(valueStack.pop()));}
    void valueTan()         {valueStack.push(tan(valueStack.pop()));}
    void valueArctan()      {valueStack.push(atan(valueStack.pop()));}
    void valueSinh()        {valueStack.push(sinh(valueStack.pop()));}
    void valueArcsinh()     {valueStack.push(asinh(valueStack.pop()));}
    void valueCosh()        {valueStack.push(cosh(valueStack.pop()));}
    void valueArccosh()     {valueStack.push(acosh(valueStack.pop()));}
    void valueTanh()        {valueStack.push(tanh(valueStack.pop()));}
    void valueArctanh()     {valueStack.push(atanh(valueStack.pop()));}
    void valueExp()         {valueStack.push(exp(valueStack.pop()));}
    void valueLn()          {valueStack.push(log(valueStack.pop()));}
    void valueLog10()       {valueStack.push(log10(valueStack.pop()));}
    void valueLog2()        {valueStack.push(log2(valueStack.pop()));}
    void valueLogn()        {valueStack.push(log(valueStack.pop()) / log(valueStack.pop()));}
    void valueSqrt()        {valueStack.push(sqrt(valueStack.pop()));}
    void valueAbs()         {valueStack.push(abs(valueStack.pop()));}
    void valueMax()         {valueStack.push(qMax(valueStack.pop(),valueStack.pop()));}
    void valueMin()         {valueStack.push(qMin(valueStack.pop(),valueStack.pop()));}
    void valuePi()          {valueStack.push(M_PI);}
    void valueInt()         {valueStack.push((int)valueStack.pop());}
    void valueTrunc()       {valueStack.push(trunc(valueStack.pop()));}
    void valueFloor()       {valueStack.push(floor(valueStack.pop()));}
    void valueRound()       {valueStack.push(round(valueStack.pop()));}
    void valueCeil()        {valueStack.push(ceil(valueStack.pop()));}
    void valueSign()        {valueStack.push(copysign(1.0,valueStack.pop()));}
    void valueHeaviside()   {if (valueStack.pop() >= 0)  valueStack.push(1); else valueStack.push(0);}
    void valueRand()        {valueStack.push(qrand());}
    void valueRandint()     {valueStack.push((int)qrand());}
    void valueRandg()       {int min, max;
                             min = valueStack.pop();
                             max = valueStack.pop();
                             valueStack.push(min + qrand()%(max-min+1));}
    void valueFaculty();

    /** functions for unit calculation */
    void unit0()            {/* ?? */}
/*    void unitCheckEqual()   {PhyxUnitDataType unit = unitStack.pop();
                            if (!unit.checkEqual(unitStack.pop()))
                                raiseException("different units");
                            else
                                unitStack.push(unit);
                            }
    void unitCheck0()       {}
    void unitCheck0k()      {}
    void unitMul()          {unitStack.push(unitStack.pop() * unitStack.pop());}
    void unitDiv()          {unitStack.push(unitStack.pop() / unitStack.pop());}
    void unitExponent()     {unitStack.push(unitStack.pop().pow(valueStack.last()));}
    void unitExp2()         {unitStack.push(unitStack.pop().pow(2));}
    void unitExp3()         {unitStack.push(unitStack.pop().pow(3));}
    void unitSqrt()         {unitStack.push(unitStack.pop().root(2));}*/

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
