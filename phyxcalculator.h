#ifndef PHYXCALCULATOR_H
#define PHYXCALCULATOR_H

#include <QObject>
#include <QStack>
#include <QDateTime>
#include <QDebug>
#include "cmath"

class PhyxCalculator;

typedef struct {
    void (PhyxCalculator::*valueFunction)();
    void (PhyxCalculator::*unitFunction)();
} PhyxRule;

class PhyxCalculator : public QObject
{
    Q_OBJECT
public:
    explicit PhyxCalculator(QObject *parent = 0);

private:
    typedef long double PhyxValueDataType;      //the base data type for values
    typedef float       PhyxUnitDataType;       //the base data type for units
    QStack<PhyxValueDataType>   valueStack;     //stack for value calculation
    QStack<PhyxUnitDataType>    unitStack;      //stack for unit calculation

    QHash<QString, PhyxRule> phyxRules;         //map of all rules, key is rule

    /* initializes PhyxCalculator */
    void initialize();

    /* raises an exception */
    void raiseException(QString exception);

    /* adds a rule */
    void addRule(QString rule, void (PhyxCalculator::*valueFunction)(), void (PhyxCalculator::*unitFunction)());
    
    /* functions for value calculation */
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

    /* functions for unit calculation */
    void unit0()            {/* ?? */}
    void unitCheckEqual()   {PhyxUnitDataType unit = unitStack.pop();
                            if (unit != unitStack.pop())
                                raiseException("different units");
                            else
                                unitStack.push(unit);
                            }
    void unitCheck0()       {}
    void unitCheck0k()      {}
    void unitMul()          {unitStack.push(unitStack.pop() + unitStack.pop());}
    void unitDiv()          {unitStack.push(unitStack.pop() + unitStack.pop());}
    void unitExponent()     {unitStack.push(unitStack.pop() * valueStack.last());}
    void unitExp2()         {unitStack.push(unitStack.pop() * 2);}
    void unitExp3()         {unitStack.push(unitStack.pop() * 3);}
    void unitSqrt()         {unitStack.push(unitStack.pop() / 2);}
signals:
    
public slots:
    
};

#endif // PHYXCALCULATOR_H
