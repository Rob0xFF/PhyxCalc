#include "phyxcalculator.h"

PhyxCalculator::PhyxCalculator(QObject *parent) :
    QObject(parent)
{
    initialize();

    setExpression("1.4+4.7*3");
    evaluate();
}

void PhyxCalculator::initialize()
{
    //initialize random number generator
    qsrand(QDateTime::currentMSecsSinceEpoch());

    addRule("S=|E|",        NULL, NULL);
    addRule("E=|E|+|T|",    &PhyxCalculator::valueAdd, NULL);
    addRule("E=|E|-|T|",    &PhyxCalculator::valueSub, NULL);
    addRule("E=|T|",        NULL, NULL);
    addRule("T=|T|*|F|",    &PhyxCalculator::valueMul, NULL);
    addRule("T=|T|/|F|",    &PhyxCalculator::valueDiv, NULL);
    addRule("T=|F|",        NULL, NULL);
    addRule("F=|N|",        NULL, NULL);
    addRule("F=-|F|",       NULL, NULL);
    addRule("F=+|F|",       NULL, NULL);
    addRule("F=(|F|)",      NULL, NULL);
    addRule("T=sin(|F|)",   &PhyxCalculator::valueSin, NULL);
    addRule("N=|Z|",        &PhyxCalculator::numberPush, NULL);
    addRule("N=|Z||Z|",     &PhyxCalculator::numberPush, NULL);
    addRule("N=|Z||D||Z|",  &PhyxCalculator::numberPush, NULL);
    addRule("D=.",          &PhyxCalculator::numberDot, NULL);
    addRule("Z=0",          &PhyxCalculator::number0, NULL);
    addRule("Z=1",          &PhyxCalculator::number1, NULL);
    addRule("Z=2",          &PhyxCalculator::number2, NULL);
    addRule("Z=3",          &PhyxCalculator::number3, NULL);
    addRule("Z=4",          &PhyxCalculator::number4, NULL);
    addRule("Z=5",          &PhyxCalculator::number5, NULL);
    addRule("Z=6",          &PhyxCalculator::number6, NULL);
    addRule("Z=7",          &PhyxCalculator::number7, NULL);
    addRule("Z=8",          &PhyxCalculator::number8, NULL);
    addRule("Z=9",          &PhyxCalculator::number9, NULL);
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

    earleyParser.loadRule(rule);
}

bool PhyxCalculator::evaluate()
{
    earleyParser.setWord(expression);
    earleyParser.setStartSymbol("S");

    if (earleyParser.parse())
    {
        QList<EarleyTreeItem> earleyTree;
        earleyTree = earleyParser.createTree();

        for (int i = (earleyTree.size()-1); i > 0; i--)
        {
            EarleyTreeItem *earleyTreeItem = &earleyTree[i];
            PhyxRule phyxRule = phyxRules.value(earleyTreeItem->rule);

            if (phyxRule.valueFunction != NULL)
                (this->*phyxRule.valueFunction)();
            if (phyxRule.unitFunction != NULL)
                (this->*phyxRule.unitFunction)();
        }
        qDebug() << (double)valueStack.pop();
    }
    else
        qDebug() << "Syntax Error!";
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
