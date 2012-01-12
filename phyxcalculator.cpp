#include "phyxcalculator.h"

PhyxCalculator::PhyxCalculator(QObject *parent) :
    QObject(parent)
{
    initialize();

    earleyParser->setStartSymbol("S");

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
    m_expression = "";
    expressionIsParsable = false;
    valueBuffer = 1;
    unitBuffer = "";
    m_error = false;
    m_errorNumber = 0;
    m_resultValue = 0;
    m_resultUnit = "";

    //initialize random number generator
    qsrand(QDateTime::currentMSecsSinceEpoch());

    // create earley parser
    earleyParser = new QEarleyParser();

    //map functions
    functionMap.insert("valueAdd",      &PhyxCalculator::valueAdd);
    functionMap.insert("valueSub",      &PhyxCalculator::valueSub);
    functionMap.insert("valueMul",      &PhyxCalculator::valueMul);
    functionMap.insert("valueDiv",      &PhyxCalculator::valueDiv);
    functionMap.insert("valueNeg",      &PhyxCalculator::valueNeg);
    functionMap.insert("valuePow",      &PhyxCalculator::valuePow);
    functionMap.insert("valuePow2",     &PhyxCalculator::valuePow2);
    functionMap.insert("valuePow3",     &PhyxCalculator::valuePow3);
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
    functionMap.insert("valueRoot",     &PhyxCalculator::valueRoot);
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
    functionMap.insert("valueFaculty",  &PhyxCalculator::valueFaculty);

    functionMap.insert("unitCheckDimensionless",    &PhyxCalculator::unitCheckDimensionless);
    functionMap.insert("unitCheckDimensionless2",    &PhyxCalculator::unitCheckDimensionless2);
    functionMap.insert("unitCheckConvertible",      &PhyxCalculator::unitCheckConvertible);
    functionMap.insert("unitMul",       &PhyxCalculator::unitMul);
    functionMap.insert("unitDiv",       &PhyxCalculator::unitDiv);
    functionMap.insert("unitPow",       &PhyxCalculator::unitPow);
    functionMap.insert("unitPow2",      &PhyxCalculator::unitPow2);
    functionMap.insert("unitPow3",      &PhyxCalculator::unitPow3);
    functionMap.insert("unitSqrt",      &PhyxCalculator::unitSqrt);
    functionMap.insert("unitRoot",      &PhyxCalculator::unitRoot);

    functionMap.insert("variableAdd",   &PhyxCalculator::variableAdd);
    functionMap.insert("variableRemove",&PhyxCalculator::variableRemove);

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
    m_error = true;
    clearStack();
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
    earleyParser->loadRule(rule, ruleFunctions);
}

void PhyxCalculator::addUnitRule(QString symbol)
{
    addRule(QString("unit=%1").arg(symbol), QString());
}

void PhyxCalculator::removeUnitRule(QString symbol)
{
    earleyParser->removeRule(QString("unit=%1").arg(symbol));
}

void PhyxCalculator::clearStack()
{
    foreach (PhyxVariable *variable, variableStack)
        delete variable;
    variableStack.clear();
}

bool PhyxCalculator::setExpression(QString expression)
{
    if (expression.isEmpty())
    {
        earleyParser->clearWord();
        expressionIsParsable = false;
    }
    else if (!m_expression.isEmpty() && expression.indexOf(m_expression) == 0)      //new expression is old expression + string
    {
        QString string = expression.mid(m_expression.size());
        foreach (QChar character, string)
        {
            expressionIsParsable = earleyParser->addSymbol(character);
        }
    }
    else if (m_expression.indexOf(expression) == 0) //new expression is old expression - string
    {
        int  count = m_expression.size() - expression.size();
        for (int i = 0; i < count; i++)
        {
            expressionIsParsable = earleyParser->removeSymbol();
        }
    }
    else
    {
        expressionIsParsable = earleyParser->parseWord(expression);
    }

    m_expression = expression;
    return expressionIsParsable;
}

bool PhyxCalculator::evaluate()
{
    if (expressionIsParsable)
    {
        m_error = false;

        QList<EarleyTreeItem> earleyTree;
        earleyTree = earleyParser->getTree();

        for (int i = (earleyTree.size()-1); i >= 0; i--)
        {
            EarleyTreeItem *earleyTreeItem = &earleyTree[i];
            //PhyxRule phyxRule = phyxRules.value(earleyTreeItem->rule);

            //if (!phyxRule.functions.isEmpty())
            if (!earleyTreeItem->rule->functions.isEmpty())
            {
                foreach (QString function, earleyTreeItem->rule->functions)
                {
                    if (this->hasError())
                        return false;
                    else if (function == "bufferParameter")
                        parameterBuffer = m_expression.mid(earleyTreeItem->startPos, earleyTreeItem->endPos - earleyTreeItem->startPos + 1);
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

QString PhyxCalculator::errorString() const
{
    return QString();
}

void PhyxCalculator::valueAdd()
{
    PhyxVariable *variable1 = variableStack.pop();
    PhyxVariable *variable2 = variableStack.pop();

    variable1->setValue(variable1->value() + variable2->value());
    variableStack.push(variable1);

    delete variable2;
}

void PhyxCalculator::valueSub()
{
    PhyxVariable *variable1 = variableStack.pop();
    PhyxVariable *variable2 = variableStack.pop();

    variable1->setValue(variable1->value() - variable2->value());
    variableStack.push(variable1);

    delete variable2;
}

void PhyxCalculator::valueMul()
{
    PhyxVariable *variable1 = variableStack.pop();
    PhyxVariable *variable2 = variableStack.pop();

    variable1->setValue(variable1->value() * variable2->value());
    variableStack.push(variable1);

    delete variable2;
}

void PhyxCalculator::valueDiv()
{
    PhyxVariable *variable1 = variableStack.pop();
    PhyxVariable *variable2 = variableStack.pop();

    variable1->setValue(variable1->value() / variable2->value());
    variableStack.push(variable1);

    delete variable2;
}

void PhyxCalculator::valueNeg()
{
    PhyxVariable *variable1 = variableStack.pop();

    variable1->setValue(-variable1->value());
    variableStack.push(variable1);
}

void PhyxCalculator::valuePow()
{
    PhyxVariable *variable1 = variableStack.pop();
    PhyxVariable *variable2 = variableStack.pop();

    variable1->setValue(pow(variable1->value(),variable2->value()));
    variableStack.push(variable1);

    delete variable2;
}

void PhyxCalculator::valuePow2()
{
    PhyxVariable *variable1 = variableStack.pop();

    variable1->setValue(pow(variable1->value(),2));
    variableStack.push(variable1);
}

void PhyxCalculator::valuePow3()
{
    PhyxVariable *variable1 = variableStack.pop();

    variable1->setValue(pow(variable1->value(),3));
    variableStack.push(variable1);
}

void PhyxCalculator::valueSin()
{
    PhyxVariable *variable1 = variableStack.pop();

    variable1->setValue(sin(variable1->value()));
    variableStack.push(variable1);
}

void PhyxCalculator::valueArcsin()
{
    PhyxVariable *variable1 = variableStack.pop();

    variable1->setValue(asin(variable1->value()));
    variableStack.push(variable1);
}

void PhyxCalculator::valueCos()
{
    PhyxVariable *variable1 = variableStack.pop();

    variable1->setValue(cos(variable1->value()));
    variableStack.push(variable1);
}

void PhyxCalculator::valueArccos()
{
    PhyxVariable *variable1 = variableStack.pop();

    variable1->setValue(acos(variable1->value()));
    variableStack.push(variable1);
}

void PhyxCalculator::valueTan()
{
    PhyxVariable *variable1 = variableStack.pop();

    variable1->setValue(tan(variable1->value()));
    variableStack.push(variable1);
}

void PhyxCalculator::valueArctan()
{
    PhyxVariable *variable1 = variableStack.pop();

    variable1->setValue(atan(variable1->value()));
    variableStack.push(variable1);
}

void PhyxCalculator::valueSinh()
{
    PhyxVariable *variable1 = variableStack.pop();

    variable1->setValue(sinh(variable1->value()));
    variableStack.push(variable1);
}

void PhyxCalculator::valueArcsinh()
{
    PhyxVariable *variable1 = variableStack.pop();

    variable1->setValue(asinh(variable1->value()));
    variableStack.push(variable1);
}

void PhyxCalculator::valueCosh()
{
    PhyxVariable *variable1 = variableStack.pop();

    variable1->setValue(cosh(variable1->value()));
    variableStack.push(variable1);
}

void PhyxCalculator::valueArccosh()
{
    PhyxVariable *variable1 = variableStack.pop();

    variable1->setValue(acosh(variable1->value()));
    variableStack.push(variable1);
}

void PhyxCalculator::valueTanh()
{
    PhyxVariable *variable1 = variableStack.pop();

    variable1->setValue(tanh(variable1->value()));
    variableStack.push(variable1);
}

void PhyxCalculator::valueArctanh()
{
    PhyxVariable *variable1 = variableStack.pop();

    variable1->setValue(atanh(variable1->value()));
    variableStack.push(variable1);
}

void PhyxCalculator::valueExp()
{
    PhyxVariable *variable1 = variableStack.pop();

    variable1->setValue(exp(variable1->value()));
    variableStack.push(variable1);
}

void PhyxCalculator::valueLn()
{
    PhyxVariable *variable1 = variableStack.pop();

    variable1->setValue(log(variable1->value()));
    variableStack.push(variable1);
}

void PhyxCalculator::valueLog10()
{
    PhyxVariable *variable1 = variableStack.pop();

    variable1->setValue(log10(variable1->value()));
    variableStack.push(variable1);
}

void PhyxCalculator::valueLog2()
{
    PhyxVariable *variable1 = variableStack.pop();

    variable1->setValue(log2(variable1->value()));
    variableStack.push(variable1);
}

void PhyxCalculator::valueLogn()
{
    PhyxVariable *variable1 = variableStack.pop();
    PhyxVariable *variable2 = variableStack.pop();

    variable1->setValue(log(variable2->value()) / log(variable1->value()));
    variableStack.push(variable1);

    delete variable2;
}

void PhyxCalculator::valueRoot()
{
    PhyxVariable *variable1 = variableStack.pop();
    PhyxVariable *variable2 = variableStack.pop();

    variable1->setValue(pow(variable2->value(), 1.0/variable1->value()));
    variableStack.push(variable1);

    delete variable2;
}

void PhyxCalculator::valueSqrt()
{
    PhyxVariable *variable1 = variableStack.pop();

    variable1->setValue(sqrt(variable1->value()));
    variableStack.push(variable1);
}

void PhyxCalculator::valueAbs()
{
    PhyxVariable *variable1 = variableStack.pop();

    variable1->setValue(qAbs(variable1->value()));
    variableStack.push(variable1);
}

void PhyxCalculator::valueMax()
{
    PhyxVariable *variable1 = variableStack.pop();
    PhyxVariable *variable2 = variableStack.pop();

    variable1->setValue(qMax(variable1->value(),variable2->value()));
    variableStack.push(variable1);

    delete variable2;
}

void PhyxCalculator::valueMin()
{
    PhyxVariable *variable1 = variableStack.pop();
    PhyxVariable *variable2 = variableStack.pop();

    variable1->setValue(qMin(variable1->value(),variable2->value()));
    variableStack.push(variable1);

    delete variable2;
}

void PhyxCalculator::valuePi()
{
    valueBuffer = M_PI;
    unitBuffer = "";
    pushVariable();
}

void PhyxCalculator::valueInt()
{
    PhyxVariable *variable1 = variableStack.pop();

    variable1->setValue((int)(variable1->value()));
    variableStack.push(variable1);
}

void PhyxCalculator::valueTrunc()
{
    PhyxVariable *variable1 = variableStack.pop();

    variable1->setValue(trunc(variable1->value()));
    variableStack.push(variable1);
}

void PhyxCalculator::valueFloor()
{
    PhyxVariable *variable1 = variableStack.pop();

    variable1->setValue(floor(variable1->value()));
    variableStack.push(variable1);
}

void PhyxCalculator::valueRound()
{
    PhyxVariable *variable1 = variableStack.pop();

    variable1->setValue(round(variable1->value()));
    variableStack.push(variable1);
}

void PhyxCalculator::valueCeil()
{
    PhyxVariable *variable1 = variableStack.pop();

    variable1->setValue(ceil(variable1->value()));
    variableStack.push(variable1);
}

void PhyxCalculator::valueSign()
{
    PhyxVariable *variable1 = variableStack.pop();

    variable1->setValue(copysign(1.0,variable1->value()));
    variableStack.push(variable1);
}

void PhyxCalculator::valueHeaviside()
{
    PhyxVariable *variable1 = variableStack.pop();

    if (variable1->value() >= 0)
        variable1->setValue(1);
    else
        variable1->setValue(0);
    variableStack.push(variable1);
}

void PhyxCalculator::valueRand()
{
    valueBuffer = qrand();
    unitBuffer = "";
    pushVariable();
}

void PhyxCalculator::valueRandint()
{
    valueBuffer = (int)qrand();
    unitBuffer = "";
    pushVariable();
}

void PhyxCalculator::valueRandg()
{
    PhyxVariable *variable1 = variableStack.pop();
    PhyxVariable *variable2 = variableStack.pop();

    int min, max;
    min = variable1->value();
    max = variable2->value();
    variable1->setValue(min + qrand()%(max-min+1));
    variableStack.push(variable1);

    delete variable2;
}

void PhyxCalculator::valueFaculty()
{
    PhyxVariable *variable1 = variableStack.pop();

    PhyxValueDataType value = variable1->value();

    if (value < 0)
    {
        raiseException(tr("Only positive values"));
        return;
    }
    else if ((int)value != value)
    {
        raiseException(tr("Only integer values"));
        return;
    }

    int n = (int)round(value);
    value = 1;
    for (int i = 2; i <=n; i++)
        value *= i;

    variable1->setValue(value);
    variableStack.push(variable1);
}

void PhyxCalculator::unitCheckDimensionless()
{
    PhyxVariable *variable1 = variableStack.pop();

    if (!variable1->unit()->isDimensionlessUnit())
        raiseException(tr("Unit is not dimensionless"));

    variableStack.push(variable1);
}

void PhyxCalculator::unitCheckDimensionless2()
{
    PhyxVariable *variable1 = variableStack.pop();
    PhyxVariable *variable2 = variableStack.pop();

    if (!variable1->unit()->isDimensionlessUnit() || !variable2->unit()->isDimensionlessUnit())
        raiseException(tr("Unit is not dimensionless"));

    variableStack.push(variable2);
    variableStack.push(variable1);
}

void PhyxCalculator::unitCheckConvertible()
{
    PhyxVariable *variable1 = variableStack.pop();
    PhyxVariable *variable2 = variableStack.pop();

    if (!variable1->unit()->isConvertible(variable2->unit()))
        raiseException(tr("Units not convertible"));

    variableStack.push(variable2);
    variableStack.push(variable1);
}

void PhyxCalculator::unitMul()
{
    PhyxVariable *variable1 = variableStack.pop();
    PhyxVariable *variable2 = variableStack.pop();

    variable1->unit()->multiply(variable2->unit());
    variableStack.push(variable2);
    variableStack.push(variable1);
}

void PhyxCalculator::unitDiv()
{
    PhyxVariable *variable1 = variableStack.pop();
    PhyxVariable *variable2 = variableStack.pop();

    variable1->unit()->divide(variable2->unit());
    variableStack.push(variable2);
    variableStack.push(variable1);
}

void PhyxCalculator::unitPow()
{
    PhyxVariable *variable1 = variableStack.pop();
    PhyxVariable *variable2 = variableStack.pop();

    variable1->unit()->raise(variable2->value());
    variableStack.push(variable2);
    variableStack.push(variable1);
}

void PhyxCalculator::unitPow2()
{
    PhyxVariable *variable1 = variableStack.pop();

    variable1->unit()->raise(2);
    variableStack.push(variable1);
}

void PhyxCalculator::unitPow3()
{
    PhyxVariable *variable1 = variableStack.pop();

    variable1->unit()->raise(3);
    variableStack.push(variable1);
}

void PhyxCalculator::unitRoot()
{
    PhyxVariable *variable1 = variableStack.pop();
    PhyxVariable *variable2 = variableStack.pop();

    variable1->unit()->root(variable2->value());
    variableStack.push(variable2);
    variableStack.push(variable1);
}

void PhyxCalculator::unitSqrt()
{
    PhyxVariable *variable1 = variableStack.pop();

    variable1->unit()->root(2);
    variableStack.push(variable1);
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
        m_resultValue = variable1->value();
        m_resultUnit = variable1->unit()->symbol();
        qDebug() << (double)variable1->value() << variable1->unit()->symbol();

        delete variable1;
    }
}
