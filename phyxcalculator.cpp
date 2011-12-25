#include "phyxcalculator.h"

PhyxCalculator::PhyxCalculator(QObject *parent) :
    QObject(parent)
{
    initialize();
}

void PhyxCalculator::initialize()
{
    //initialize random number generator
    qsrand(QDateTime::currentMSecsSinceEpoch());
}

void PhyxCalculator::raiseException(QString exception)
{
    qDebug() << exception;
}

void PhyxCalculator::addRule(QString rule, void (PhyxCalculator::*valueFunction)(), void (PhyxCalculator::*unitFunction)())
{
    PhyxRule phyxRule;
    phyxRule.valueFunction = valueFunction;
    phyxRule.unitFunction = unitFunction;
    phyxRules.insert(rule, phyxRule);
}

void PhyxCalculator::valueFaculty()
{
    PhyxValueDataType value = valueStack.pop();

    if (value < 0)
        raiseException("Only positive values");
    if ((int)value != value)
        raiseException("Only integer values");

    int n = (int)round(value);
    value = 1;
    for (int i = 2; i <=n; i++)
        value *= i;

    valueStack.push(value);
}
