#include "phyxcalculator.h"

PhyxCalculator::PhyxCalculator(QObject *parent) :
    QObject(parent)
{
    initialize();

    earleyParser.setStartSymbol("S");

    //testing
    PhyxTesting::testUnits();

    unitSystem->addBaseUnit("m", PhyxUnit::SiUnitFlag);
    unitSystem->addBaseUnit("kg", PhyxUnit::SiUnitFlag);
    unitSystem->addBaseUnit("A", PhyxUnit::SiUnitFlag);
    unitSystem->addBaseUnit("K", PhyxUnit::SiUnitFlag);
    unitSystem->addBaseUnit("s", PhyxUnit::SiUnitFlag);
    unitSystem->addBaseUnit("mol", PhyxUnit::SiUnitFlag);
    unitSystem->addBaseUnit("cd", PhyxUnit::SiUnitFlag);
}

void PhyxCalculator::initialize()
{
    //initialize variables
    expression = "";
    expressionIsParsable = false;
    valueBuffer = 1;
    unitBuffer = "";

    //initialize random number generator
    qsrand(QDateTime::currentMSecsSinceEpoch());

    //map functions
    functionMap.insert("mathAdd",      &PhyxCalculator::mathAdd);
    functionMap.insert("mathSub",      &PhyxCalculator::mathSub);
    functionMap.insert("mathMul",      &PhyxCalculator::mathMul);
    functionMap.insert("mathDiv",      &PhyxCalculator::mathDiv);
    functionMap.insert("mathNeg",      &PhyxCalculator::mathNeg);
    functionMap.insert("mathSin",      &PhyxCalculator::mathSin);

    functionMap.insert("valueAdd",      &PhyxCalculator::valueAdd);
    functionMap.insert("valueSub",      &PhyxCalculator::valueSub);
    functionMap.insert("valueMul",      &PhyxCalculator::valueMul);
    functionMap.insert("valueDiv",      &PhyxCalculator::valueDiv);
    functionMap.insert("valueNeg",      &PhyxCalculator::valueNeg);
    functionMap.insert("valueExponent", &PhyxCalculator::valueExponent);
    functionMap.insert("valueExp2",     &PhyxCalculator::valueExp2);
    functionMap.insert("valueExp3",     &PhyxCalculator::valueExp3);
    functionMap.insert("valueSin",      &PhyxCalculator::valueSin);
    functionMap.insert("valueArcsin",   &PhyxCalculator::valueArcsin);
    functionMap.insert("valueCos",      &PhyxCalculator::valueCos);
    functionMap.insert("valueArccos",   &PhyxCalculator::valueArccos);
    functionMap.insert("valueTan",      &PhyxCalculator::valueTan);
    functionMap.insert("valueArctan",   &PhyxCalculator::valueArctan);
    functionMap.insert("valueSinh",     &PhyxCalculator::valueSinh);
    functionMap.insert("valueArcsinh",  &PhyxCalculator::valueArcsinh);
    functionMap.insert("valueCosh",     &PhyxCalculator::valueCosh);
    functionMap.insert("valueArccosh",  &PhyxCalculator::valueArccosh);
    functionMap.insert("valueTanh",     &PhyxCalculator::valueTanh);
    functionMap.insert("valueArctanh",  &PhyxCalculator::valueArctanh);
    functionMap.insert("valueExp",      &PhyxCalculator::valueExp);
    functionMap.insert("valueLn",       &PhyxCalculator::valueLn);
    functionMap.insert("valueLog10",    &PhyxCalculator::valueLog10);
    functionMap.insert("valueLog2",     &PhyxCalculator::valueLog2);
    functionMap.insert("valueLogn",     &PhyxCalculator::valueLogn);
    functionMap.insert("valueSqrt",     &PhyxCalculator::valueSqrt);
    functionMap.insert("valueAbs",      &PhyxCalculator::valueAbs);
    functionMap.insert("valueMax",      &PhyxCalculator::valueMax);
    functionMap.insert("valueMin",      &PhyxCalculator::valueMin);
    functionMap.insert("valuePi",       &PhyxCalculator::valuePi);
    functionMap.insert("valueInt",      &PhyxCalculator::valueInt);
    functionMap.insert("valueTrunc",    &PhyxCalculator::valueTrunc);
    functionMap.insert("valueFloor",    &PhyxCalculator::valueFloor);
    functionMap.insert("valueRound",    &PhyxCalculator::valueRound);
    functionMap.insert("valueCeil",     &PhyxCalculator::valueCeil);
    functionMap.insert("valueSign",     &PhyxCalculator::valueSign);
    functionMap.insert("valueHeaviside",&PhyxCalculator::valueHeaviside);
    functionMap.insert("valueRand",     &PhyxCalculator::valueRand);
    functionMap.insert("valueRandint",  &PhyxCalculator::valueRandint);
    functionMap.insert("valueRandg",    &PhyxCalculator::valueRandg);

    functionMap.insert("unit0",         &PhyxCalculator::unit0);
    /*functionMap.insert("unitCheckEqual",&PhyxCalculator::unitCheckEqual);
    functionMap.insert("unitCheck0",    &PhyxCalculator::unitCheck0);
    functionMap.insert("unitCheck0k",   &PhyxCalculator::unitCheck0k);
    functionMap.insert("unitMul",       &PhyxCalculator::unitMul);
    functionMap.insert("unitDiv",       &PhyxCalculator::unitDiv);
    functionMap.insert("unitExponent",  &PhyxCalculator::unitExponent);
    functionMap.insert("unitExp2",      &PhyxCalculator::unitExp2);
    functionMap.insert("unitExp3",      &PhyxCalculator::unitExp3);
    functionMap.insert("unitSqrt",      &PhyxCalculator::unitSqrt);*/

    functionMap.insert("numberPush",    &PhyxCalculator::numberPush);
    functionMap.insert("variableAdd",   &PhyxCalculator::variableAdd);
    functionMap.insert("variableRemove",&PhyxCalculator::variableRemove);

    functionMap.insert("numberBuf",     &PhyxCalculator::numberBuf);
    functionMap.insert("variablePush",  &PhyxCalculator::variablePush);

    functionMap.insert("bufferUnit",&PhyxCalculator::bufferUnit);
    functionMap.insert("bufferValue",&PhyxCalculator::bufferValue);
    functionMap.insert("pushVariable",&PhyxCalculator::pushVariable);
    functionMap.insert("outputVariable",&PhyxCalculator::outputVariable);

    loadGrammar(":/settings/grammar");

    //initialize unit system
    unitSystem = new PhyxUnitSystem();
    connect(unitSystem, SIGNAL(unitAdded(QString)),
            this, SLOT(addUnitRule(QString)));
    connect(unitSystem, SIGNAL(unitRemoved(QString)),
            this, SLOT(removeUnitRule(QString)));
}

void PhyxCalculator::loadGrammar(QString fileName)
{
    QFile file(fileName);

    if (file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        QStringList lines = QString::fromUtf8(file.readAll()).split('\n');
        foreach (QString line, lines)
        {
            if (line.trimmed().isEmpty() || (line.trimmed().at(0) == '#'))
                continue;

            QStringList ruleData = line.split(';');
            QString rule;
            QString functions;

            rule = ruleData.at(0).trimmed();
            if (ruleData.size() > 1)
                functions = ruleData.at(1).trimmed();

            addRule(rule, functions);
        }
    }
    else
        qFatal("Can't open file");
}

void PhyxCalculator::raiseException(QString exception)
{
    qDebug() << exception;
}

void PhyxCalculator::addRule(QString rule, QString functions)
{
    PhyxRule phyxRule;
    if (!functions.isEmpty())
        phyxRule.functions = functions.split(',');
    phyxRules.insert(rule, phyxRule);

    QStringList ruleFunctions;
    foreach (QString function, phyxRule.functions)
        ruleFunctions.append(function.trimmed());
    earleyParser.loadRule(rule, ruleFunctions);
}

bool PhyxCalculator::setExpression(QString m_expression)
{
    if (m_expression.isEmpty())
    {
        earleyParser.clearWord();
        expressionIsParsable = false;
    }
    else if (!expression.isEmpty() && m_expression.indexOf(expression) == 0)      //new expression is old expression + string
    {
        QString string = m_expression.mid(expression.size());
        foreach (QChar character, string)
        {
            expressionIsParsable = earleyParser.addSymbol(character);
        }
    }
    else if (expression.indexOf(m_expression) == 0) //new expression is old expression - string
    {
        int  count = expression.size() - m_expression.size();
        for (int i = 0; i < count; i++)
        {
            expressionIsParsable = earleyParser.removeSymbol();
        }
    }
    else
    {
        expressionIsParsable = earleyParser.parseWord(m_expression);
    }

    expression = m_expression;
    return expressionIsParsable;
}

bool PhyxCalculator::evaluate()
{
    if (expressionIsParsable)
    {
        QList<EarleyTreeItem> earleyTree;
        earleyTree = earleyParser.getTree();

        for (int i = (earleyTree.size()-1); i >= 0; i--)
        {
            EarleyTreeItem *earleyTreeItem = &earleyTree[i];
            //PhyxRule phyxRule = phyxRules.value(earleyTreeItem->rule);

            //if (!phyxRule.functions.isEmpty())
            if (!earleyTreeItem->rule->functions.isEmpty())
            {
                foreach (QString function, earleyTreeItem->rule->functions)
                {
                    if (function == "bufferParameter")
                        parameterBuffer = expression.mid(earleyTreeItem->startPos, earleyTreeItem->endPos - earleyTreeItem->startPos + 1);
                    else
                        (this->*functionMap.value(function))();
                }
            }
            //if (!phyxRule.paramFunction.isEmpty())
            //    (this->*paramFunctionMap.value(phyxRule.paramFunction))(phyxRule.parameter);
        }
        return true;
    }
    else
    {
        raiseException("Syntax Error!");
        return false;
    }
}

void PhyxCalculator::mathAdd()
{
    PhyxVariable *variable1 = variableStack.pop();
    PhyxVariable *variable2 = variableStack.pop();

    if (variable1->mathAdd(variable2))
        variableStack.push(variable1);
    else
        raiseException(variable1->errorString());

    delete variable2;
}

void PhyxCalculator::mathSub()
{
    PhyxVariable *variable1 = variableStack.pop();
    PhyxVariable *variable2 = variableStack.pop();

    if (variable1->mathSub(variable2))
        variableStack.push(variable1);
    else
        raiseException(variable1->errorString());

    delete variable2;
}

void PhyxCalculator::mathMul()
{
    PhyxVariable *variable1 = variableStack.pop();
    PhyxVariable *variable2 = variableStack.pop();

    if (variable1->mathMul(variable2))
        variableStack.push(variable1);
    else
        raiseException(variable1->errorString());

    delete variable2;
}

void PhyxCalculator::mathDiv()
{
    PhyxVariable *variable1 = variableStack.pop();
    PhyxVariable *variable2 = variableStack.pop();

    if (variable1->mathDiv(variable2))
        variableStack.push(variable1);
    else
        raiseException(variable1->errorString());

    delete variable2;
}

void PhyxCalculator::mathNeg()
{
    PhyxVariable *variable1 = variableStack.pop();

    variable1->setValue(-variable1->value());
}

void PhyxCalculator::mathSin()
{
    PhyxVariable *variable1 = variableStack.pop();

    if (variable1->mathSin())
        variableStack.push(variable1);
    else
        raiseException(variable1->errorString());
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

void PhyxCalculator::variableAdd()
{
    /*PhysicalVariable variable;
    variable.value = valueStack.pop();
    variable.unit = unitStack.pop();
    variableMap.insert(stringBuffer, variable);
    stringBuffer.clear();*/
}

void PhyxCalculator::variableRemove()
{
   /* variableMap.remove(stringBuffer);
    stringBuffer.clear();*/
}

void PhyxCalculator::variablePush()
{
   /* PhysicalVariable variable = variableMap.value(parameterBuffer);
    valueStack.push(variable.value);
    unitStack.push(variable.unit);*/
}

void PhyxCalculator::bufferUnit()
{
    //get the unit
    int pos;
    for (pos = (parameterBuffer.size()-1); pos >= 0; pos--)
    {
        if (parameterBuffer.at(pos).isNumber())
        {
            pos++;
            break;
        }
    }

    unitBuffer = parameterBuffer.mid(pos);
    parameterBuffer.truncate(pos);      //cut off the unit
}

void PhyxCalculator::bufferValue()
{
    valueBuffer = parameterBuffer.toDouble();
}

void PhyxCalculator::pushVariable()
{
    //create new variable
    PhyxVariable *variable = new PhyxVariable();
    variable->unit()->setUnitSystem(unitSystem);
    variable->setValue(valueBuffer);
    if (!unitBuffer.isEmpty())
        variable->setUnit(unitSystem->unit(unitBuffer));

    //push it to the stack
    variableStack.push(variable);

    //cleanup
    valueBuffer = 1;
    unitBuffer = "";
}

void PhyxCalculator::outputVariable()
{
    if (!variableStack.isEmpty())
    {
        PhyxVariable *variable1 = variableStack.pop();
        qDebug() << (double)variable1->value() << variable1->unit()->symbol();
    }
}

void PhyxCalculator::addUnitRule(QString symbol)
{
    addRule(QString("unit=%1").arg(symbol), QString());
}

void PhyxCalculator::removeUnitRule(QString symbol)
{
    earleyParser.removeRule(QString("unit=%1").arg(symbol));
}
