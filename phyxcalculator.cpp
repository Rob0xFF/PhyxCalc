/**************************************************************************
**
** This file is part of PhyxCalc.
**
** PhyxCalc is free software: you can redistribute it and/or modify
** it under the terms of the GNU General Public License as published by
** the Free Software Foundation, either version 3 of the License, or
** (at your option) any later version.
**
** PhyxCalc is distributed in the hope that it will be useful,
** but WITHOUT ANY WARRANTY; without even the implied warranty of
** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
** GNU General Public License for more details.
**
** You should have received a copy of the GNU General Public License
** along with PhyxCalc.  If not, see <http://www.gnu.org/licenses/>.
**
***************************************************************************/

#include "phyxcalculator.h"

PhyxCalculator::PhyxCalculator(QObject *parent) :
    QObject(parent)
{
    initialize();
}

void PhyxCalculator::initialize()
{
    //initialize variables
    m_expression = "";
    expressionIsParsable = false;
    valueBuffer = 1;
    prefixBuffer = "";
    unitBuffer = "";
    flagBuffer = 0;
    stackLevel = 0;
    listModeActive = false;
    m_error = false;
    m_errorNumber = 0;
    m_errorStartPosition = 0;
    m_resultValue = 0;
    m_resultUnit = "";
    m_result = NULL;

    //initialize random number generator
    qsrand(QDateTime::currentMSecsSinceEpoch());

    // create earley parser
    earleyParser = new QEarleyParser();

    //map functions
    functionMap.insert("valueCheckComplex",     &PhyxCalculator::valueCheckComplex);
    functionMap.insert("valueCheckComplex2",    &PhyxCalculator::valueCheckComplex2);
    functionMap.insert("valueCheckComplex2th",  &PhyxCalculator::valueCheckComplex2th);
    functionMap.insert("valueCheckComplex3th",  &PhyxCalculator::valueCheckComplex3th);
    functionMap.insert("valueCheckPositive",    &PhyxCalculator::valueCheckPositive);
    functionMap.insert("valueCheckInteger",     &PhyxCalculator::valueCheckInteger);
    functionMap.insert("valueCheckInteger2",    &PhyxCalculator::valueCheckInteger2);
    functionMap.insert("valueCheckInteger2th",  &PhyxCalculator::valueCheckInteger2th);
    functionMap.insert("valueCheckInteger3th",  &PhyxCalculator::valueCheckInteger3th);
    functionMap.insert("valuePush1",    &PhyxCalculator::valuePush1);
    functionMap.insert("valuePush2",    &PhyxCalculator::valuePush2);
    functionMap.insert("valuePush3",    &PhyxCalculator::valuePush3);
    functionMap.insert("valueCopy",     &PhyxCalculator::valueCopy);
    functionMap.insert("valueInvert",   &PhyxCalculator::valueInvert);
    functionMap.insert("valueAdd",      &PhyxCalculator::valueAdd);
    functionMap.insert("valueSub",      &PhyxCalculator::valueSub);
    functionMap.insert("valueMul",      &PhyxCalculator::valueMul);
    functionMap.insert("valueDiv",      &PhyxCalculator::valueDiv);
    functionMap.insert("valueMod",      &PhyxCalculator::valueMod);
    functionMap.insert("valueNeg",      &PhyxCalculator::valueNeg);
    functionMap.insert("valueNoPow",    &PhyxCalculator::valueNoPow);
    functionMap.insert("valuePow",      &PhyxCalculator::valuePow);
    functionMap.insert("valueSin",      &PhyxCalculator::valueSin);
    functionMap.insert("valueArcsin",   &PhyxCalculator::valueArcsin);
    functionMap.insert("valueCos",      &PhyxCalculator::valueCos);
    functionMap.insert("valueArccos",   &PhyxCalculator::valueArccos);
    functionMap.insert("valueTan",      &PhyxCalculator::valueTan);
    functionMap.insert("valueArctan",   &PhyxCalculator::valueArctan);
    functionMap.insert("valueCot",      &PhyxCalculator::valueCot);
    functionMap.insert("valueArccot",   &PhyxCalculator::valueArccot);
    functionMap.insert("valueSec",      &PhyxCalculator::valueSec);
    functionMap.insert("valueArcsec",   &PhyxCalculator::valueArcsec);
    functionMap.insert("valueCsc",      &PhyxCalculator::valueCsc);
    functionMap.insert("valueArccsc",   &PhyxCalculator::valueArccsc);
    functionMap.insert("valueSinh",     &PhyxCalculator::valueSinh);
    functionMap.insert("valueArcsinh",  &PhyxCalculator::valueArcsinh);
    functionMap.insert("valueCosh",     &PhyxCalculator::valueCosh);
    functionMap.insert("valueArccosh",  &PhyxCalculator::valueArccosh);
    functionMap.insert("valueTanh",     &PhyxCalculator::valueTanh);
    functionMap.insert("valueArctanh",  &PhyxCalculator::valueArctanh);
    functionMap.insert("valueCoth",      &PhyxCalculator::valueCoth);
    functionMap.insert("valueArccoth",   &PhyxCalculator::valueArccoth);
    functionMap.insert("valueSech",      &PhyxCalculator::valueSech);
    functionMap.insert("valueArcsech",   &PhyxCalculator::valueArcsech);
    functionMap.insert("valueCsch",      &PhyxCalculator::valueCsch);
    functionMap.insert("valueArccsch",   &PhyxCalculator::valueArccsch);
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
    functionMap.insert("valueAvg",      &PhyxCalculator::valueAvg);
    functionMap.insert("valuePi",       &PhyxCalculator::valuePi);
    functionMap.insert("valueE",        &PhyxCalculator::valueE);
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
    functionMap.insert("valueBcd",      &PhyxCalculator::valueBcd);
    functionMap.insert("valueToBcd",    &PhyxCalculator::valueToBcd);
    functionMap.insert("valueAns",      &PhyxCalculator::valueAns);

    functionMap.insert("complexReal",   &PhyxCalculator::complexReal);
    functionMap.insert("complexImag",   &PhyxCalculator::complexImag);
    functionMap.insert("complexArg",    &PhyxCalculator::complexArg);
    functionMap.insert("complexNorm",   &PhyxCalculator::complexNorm);
    functionMap.insert("complexConj",   &PhyxCalculator::complexConj);
    functionMap.insert("complexPolar",  &PhyxCalculator::complexPolar);

    functionMap.insert("logicAnd",      &PhyxCalculator::logicAnd);
    functionMap.insert("logicOr",       &PhyxCalculator::logicOr);
    functionMap.insert("logicNand",     &PhyxCalculator::logicNand);
    functionMap.insert("logicNor",      &PhyxCalculator::logicNor);
    functionMap.insert("logicXand",     &PhyxCalculator::logicXand);
    functionMap.insert("logicXor",      &PhyxCalculator::logicXor);
    functionMap.insert("logicNot",      &PhyxCalculator::logicNot);
    functionMap.insert("logicEqual",    &PhyxCalculator::logicEqual);
    functionMap.insert("logicNotEqual", &PhyxCalculator::logicNotEqual);
    functionMap.insert("logicGreater",  &PhyxCalculator::logicGreater);
    functionMap.insert("logicGreaterOrEqual",      &PhyxCalculator::logicGreaterOrEqual);
    functionMap.insert("logicSmaller",  &PhyxCalculator::logicSmaller);
    functionMap.insert("logicSmallerOrEqual",      &PhyxCalculator::logicSmallerOrEqual);

    functionMap.insert("bitAnd",        &PhyxCalculator::bitAnd);
    functionMap.insert("bitOr",         &PhyxCalculator::bitOr);
    functionMap.insert("bitXor",        &PhyxCalculator::bitXor);
    functionMap.insert("bitInv",        &PhyxCalculator::bitInv);
    functionMap.insert("bitShiftLeft",      &PhyxCalculator::bitShiftLeft);
    functionMap.insert("bitShiftRight",     &PhyxCalculator::bitShiftRight);

    functionMap.insert("conditionIfElse",   &PhyxCalculator::conditionIfElse);

    functionMap.insert("unitCheckDimensionless",    &PhyxCalculator::unitCheckDimensionless);
    functionMap.insert("unitCheckDimensionless2",   &PhyxCalculator::unitCheckDimensionless2);
    functionMap.insert("unitCheckDimensionless2th", &PhyxCalculator::unitCheckDimensionless2th);
    functionMap.insert("unitCheckDimensionless3th", &PhyxCalculator::unitCheckDimensionless3th);
    functionMap.insert("unitCheckConvertible",      &PhyxCalculator::unitCheckConvertible);
    functionMap.insert("unitConvert",   &PhyxCalculator::unitConvert);
    functionMap.insert("unitMul",       &PhyxCalculator::unitMul);
    functionMap.insert("unitDiv",       &PhyxCalculator::unitDiv);
    functionMap.insert("unitPow",       &PhyxCalculator::unitPow);
    functionMap.insert("unitSqrt",      &PhyxCalculator::unitSqrt);
    functionMap.insert("unitRoot",      &PhyxCalculator::unitRoot);
    functionMap.insert("unitClear",     &PhyxCalculator::unitClear);
    functionMap.insert("unitAdd",       &PhyxCalculator::unitAdd);
    functionMap.insert("unitRemove",    &PhyxCalculator::unitRemove);

    functionMap.insert("listAddStart",  &PhyxCalculator::listAddStart);
    functionMap.insert("listSubStart",  &PhyxCalculator::listSubStart);
    functionMap.insert("listMulStart",  &PhyxCalculator::listMulStart);
    functionMap.insert("listDivStart",  &PhyxCalculator::listDivStart);
    functionMap.insert("listModStart",  &PhyxCalculator::listModStart);
    functionMap.insert("listPowStart",  &PhyxCalculator::listPowStart);
    functionMap.insert("listBAndStart", &PhyxCalculator::listBAndStart);
    functionMap.insert("listBOrStart",  &PhyxCalculator::listBOrStart);
    functionMap.insert("listBXorStart", &PhyxCalculator::listBXorStart);
    functionMap.insert("listXandStart", &PhyxCalculator::listXandStart);
    functionMap.insert("listAndStart",  &PhyxCalculator::listAndStart);
    functionMap.insert("listNandStart", &PhyxCalculator::listNandStart);
    functionMap.insert("listXorStart",  &PhyxCalculator::listXorStart);
    functionMap.insert("listOrStart",   &PhyxCalculator::listOrStart);
    functionMap.insert("listNorStart",  &PhyxCalculator::listNorStart);
    functionMap.insert("listValueSave", &PhyxCalculator::listValueSave);
    functionMap.insert("listValueLoad", &PhyxCalculator::listValueLoad);
    functionMap.insert("listValueSwap", &PhyxCalculator::listValueSwap);
    functionMap.insert("listEnd",       &PhyxCalculator::listEnd);

    functionMap.insert("variableAdd",   &PhyxCalculator::variableAdd);
    functionMap.insert("variableRemove",&PhyxCalculator::variableRemove);
    functionMap.insert("variableLoad",  &PhyxCalculator::variableLoad);
    functionMap.insert("variablePreDec",&PhyxCalculator::variablePreDec);
    functionMap.insert("variablePreInc",&PhyxCalculator::variablePreInc);
    functionMap.insert("variablePostDec",&PhyxCalculator::variablePostDec);
    functionMap.insert("variablePostInc",&PhyxCalculator::variablePostInc);
    functionMap.insert("constantAdd",   &PhyxCalculator::constantAdd);
    functionMap.insert("constantRemove",&PhyxCalculator::constantRemove);
    functionMap.insert("constantLoad",  &PhyxCalculator::constantLoad);

    functionMap.insert("functionAdd",   &PhyxCalculator::functionAdd);
    functionMap.insert("functionRemove",&PhyxCalculator::functionRemove);
    functionMap.insert("functionRun",   &PhyxCalculator::functionRun);

    functionMap.insert("bufferUnit",            &PhyxCalculator::bufferUnit);
    functionMap.insert("bufferValue",           &PhyxCalculator::bufferValue);
    functionMap.insert("bufferHex",             &PhyxCalculator::bufferHex);
    functionMap.insert("bufferOct",             &PhyxCalculator::bufferOct);
    functionMap.insert("bufferBin",             &PhyxCalculator::bufferBin);
    functionMap.insert("bufferHexString",       &PhyxCalculator::bufferHexString);
    functionMap.insert("bufferOctString",       &PhyxCalculator::bufferOctString);
    functionMap.insert("bufferBinString",       &PhyxCalculator::bufferBinString);
    functionMap.insert("bufferPrefix",          &PhyxCalculator::bufferPrefix);
    functionMap.insert("bufferString",          &PhyxCalculator::bufferString);
    functionMap.insert("bufferUnitGroup",       &PhyxCalculator::bufferUnitGroup);
    functionMap.insert("pushVariable",          &PhyxCalculator::pushVariable);
    functionMap.insert("pushFunctionParameter", &PhyxCalculator::pushFunctionParameter);

    functionMap.insert("setInputOnlyFlag",      &PhyxCalculator::setInputOnlyFlag);

    functionMap.insert("outputVariable",        &PhyxCalculator::outputVariable);
    functionMap.insert("outputString",          &PhyxCalculator::outputString);
    functionMap.insert("unitGroupAdd",          &PhyxCalculator::unitGroupAdd);
    functionMap.insert("unitGroupRemove",       &PhyxCalculator::unitGroupRemove);
    functionMap.insert("prefixAdd",             &PhyxCalculator::prefixAdd);
    functionMap.insert("prefixRemove",          &PhyxCalculator::prefixRemove);

    functionMap.insert("lowLevelAdd",                   &PhyxCalculator::lowLevelAdd);
    functionMap.insert("lowLevelRun",                   &PhyxCalculator::lowLevelRun);
    functionMap.insert("lowLevelAssignment",            &PhyxCalculator::lowLevelAssignment);
    functionMap.insert("lowLevelAssignmentRemove",      &PhyxCalculator::lowLevelAssignmentRemove);
    functionMap.insert("lowLevelCombinedAssignmentAdd", &PhyxCalculator::lowLevelCombinedAssignmentAdd);
    functionMap.insert("lowLevelCombinedAssignmentSub", &PhyxCalculator::lowLevelCombinedAssignmentSub);
    functionMap.insert("lowLevelCombinedAssignmentMul", &PhyxCalculator::lowLevelCombinedAssignmentMul);
    functionMap.insert("lowLevelCombinedAssignmentDiv", &PhyxCalculator::lowLevelCombinedAssignmentDiv);
    functionMap.insert("lowLevelCombinedAssignmentMod", &PhyxCalculator::lowLevelCombinedAssignmentMod);
    functionMap.insert("lowLevelCombinedAssignmentAnd", &PhyxCalculator::lowLevelCombinedAssignmentAnd);
    functionMap.insert("lowLevelCombinedAssignmentOr",  &PhyxCalculator::lowLevelCombinedAssignmentOr);
    functionMap.insert("lowLevelCombinedAssignmentXor", &PhyxCalculator::lowLevelCombinedAssignmentXor);
    functionMap.insert("lowLevelCombinedAssignmentShiftLeft",  &PhyxCalculator::lowLevelCombinedAssignmentShiftLeft);
    functionMap.insert("lowLevelCombinedAssignmentShiftRight", &PhyxCalculator::lowLevelCombinedAssignmentShiftRight);
    functionMap.insert("lowLevelOutput",                &PhyxCalculator::lowLevelOutput);

    loadGrammar(":/settings/grammar");
    earleyParser->setStartSymbol("S");

    standardFunctionList.append("sin");
    standardFunctionList.append("arcsin");
    standardFunctionList.append("asin");
    standardFunctionList.append("cos");
    standardFunctionList.append("arccos");
    standardFunctionList.append("acos");
    standardFunctionList.append("tan");
    standardFunctionList.append("arctan");
    standardFunctionList.append("atan");
    standardFunctionList.append("cot");
    standardFunctionList.append("arccot");
    standardFunctionList.append("acot");
    standardFunctionList.append("sec");
    standardFunctionList.append("arcsec");
    standardFunctionList.append("asec");
    standardFunctionList.append("csc");
    standardFunctionList.append("arccsc");
    standardFunctionList.append("acsc");
    standardFunctionList.append("sinh");
    standardFunctionList.append("arcsinh");
    standardFunctionList.append("asinh");
    standardFunctionList.append("cosh");
    standardFunctionList.append("arccosh");
    standardFunctionList.append("acosh");
    standardFunctionList.append("tanh");
    standardFunctionList.append("arctanh");
    standardFunctionList.append("atanh");
    standardFunctionList.append("coth");
    standardFunctionList.append("arccoth");
    standardFunctionList.append("acoth");
    standardFunctionList.append("sech");
    standardFunctionList.append("arcsech");
    standardFunctionList.append("asech");
    standardFunctionList.append("csch");
    standardFunctionList.append("arccsch");
    standardFunctionList.append("acsch");
    standardFunctionList.append("exp");
    standardFunctionList.append("ln");
    standardFunctionList.append("log");
    standardFunctionList.append("root");
    standardFunctionList.append("sqrt");
    standardFunctionList.append("sqr");
    standardFunctionList.append("abs");
    standardFunctionList.append("max");
    standardFunctionList.append("min");
    standardFunctionList.append("avg");
    standardFunctionList.append("int");
    standardFunctionList.append("trunc");
    standardFunctionList.append("floor");
    standardFunctionList.append("round");
    standardFunctionList.append("ceil");
    standardFunctionList.append("sign");
    standardFunctionList.append("heaviside");
    standardFunctionList.append("rand");
    standardFunctionList.append("rnd");
    standardFunctionList.append("randi");
    standardFunctionList.append("rndi");
    standardFunctionList.append("randg");
    standardFunctionList.append("rndg");
    standardFunctionList.append("re");
    standardFunctionList.append("im");
    standardFunctionList.append("arg");
    standardFunctionList.append("norm");
    standardFunctionList.append("conj");
    standardFunctionList.append("polar");
    standardFunctionList.append("bcd");
    standardFunctionList.append("tobcd");
    standardFunctionList.append("ans");

    for (int i = standardFunctionList.size()-1; i >= 0 ; i--)
    {
        QString name = standardFunctionList.at(i);
        name[0] = name.at(0).toUpper();
        standardFunctionList.append(name);
        name = name.toUpper();
        standardFunctionList.append(name);
    }

    //initialize unit system
    unitSystem = new PhyxUnitSystem(this);
    connect(unitSystem, SIGNAL(unitAdded(QString)),
            this, SLOT(addUnitRule(QString)));
    connect(unitSystem, SIGNAL(unitRemoved(QString)),
            this, SLOT(removeUnitRule(QString)));
    connect(unitSystem, SIGNAL(prefixAdded(QString)),
            this, SLOT(addPrefixRule(QString)));
    connect(unitSystem, SIGNAL(prefixRemoved(QString)),
            this, SLOT(removePrefixRule(QString)));
    connect(unitSystem, SIGNAL(unitGroupAdded(QString)),
            this, SLOT(addUnitGroupRule(QString)));
    connect(unitSystem, SIGNAL(unitGroupRemoved(QString)),
            this, SLOT(removeUnitGroupRule(QString)));

    //initialize variable manager
    variableManager = new PhyxVariableManager(this);
    connect(variableManager, SIGNAL(variableAdded(QString)),
            this, SLOT(addVariableRule(QString)));
    connect(variableManager, SIGNAL(variableRemoved(QString)),
            this, SLOT(removeVariableRule(QString)));
    connect(variableManager, SIGNAL(constantAdded(QString)),
            this, SLOT(addConstantRule(QString)));
    connect(variableManager, SIGNAL(constantRemoved(QString)),
            this, SLOT(removeConstantRule(QString)));
    connect(variableManager, SIGNAL(functionAdded(QString,int)),
            this, SLOT(addFunctionRule(QString,int)));
    connect(variableManager, SIGNAL(functionRemoved(QString,int)),
            this, SLOT(removeFunctionRule(QString,int)));

    //initialize special variable #
    PhyxVariable *variable = new PhyxVariable(this);
    variable->setValue(PhyxValueDataType(PHYX_FLOAT_NULL,PHYX_FLOAT_NULL));
    variableManager->addVariable("#", variable);
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

            if (line.contains("//"))
                line.truncate(line.indexOf("//"));

            QStringList ruleData = line.split(';');
            QString rule;
            QString functions;

            for (int i = ruleData.size()-2; i >= 0; i--)    // handle termination of ;
            {
                if (ruleData.at(i).at(ruleData.at(i).size()-1) == '\\')
                {
                    if (ruleData.size() > (i+1))
                    {
                        ruleData[i].chop(1);
                        ruleData[i].append(';');
                        ruleData[i].append(ruleData.at(i+1));
                        ruleData.removeAt(i+1);
                    }
                }
            }

            rule = ruleData.at(0).trimmed();
            if (ruleData.size() > 1)
                functions = ruleData.at(1).trimmed();

            addRule(rule, functions);
        }
    }
    else
        qFatal("Can't open file");
}

QString PhyxCalculator::stripComments(QString text)
{
    //strip singe line comment
    int pos = text.indexOf("//");
    if (pos != -1)
        text = text.left(pos);
    //strip multi line comments
    pos = text.indexOf("/*");
    if (pos != -1)
        text = text.left(pos);
    pos = text.indexOf("*/");
    if (pos != -1)
        text = text.mid(pos+2);
    return text;
}

QString PhyxCalculator::removeWhitespace(QString text, QList<int> *whiteSpaceList)
{
    int whitespaceCount = 0;
    QString output;

    whiteSpaceList->clear();
    for (int i = text.size() - 1; i >= 0; i--)
    {
        if (text.at(i).isSpace())
            whitespaceCount++;
        else
        {
            output.prepend(text.at(i));
            whiteSpaceList->prepend(whitespaceCount);
        }
    }

    for (int i = 0; i < whiteSpaceList->size(); i++)
        (*whiteSpaceList)[i] = whitespaceCount - whiteSpaceList->at(i);

    return output;
}

int PhyxCalculator::restoreErrorPosition(int pos, QList<int> whiteSpaceList)
{
    return pos + whiteSpaceList.at(pos);
}

void PhyxCalculator::raiseException(int errorNumber)
{
    m_error = true;
    m_errorNumber = errorNumber;
    clearStack();
    emit outputError();
#ifdef QT_DEBUG
    qDebug() << "error occured:" << m_errorNumber << errorString();
#endif
}

QString PhyxCalculator::errorString() const
{
    QString output;

    switch (m_errorNumber)
    {
    case SyntaxError:       output.append(tr("Syntax Error!"));
                            break;
    case ValueComplexError: output.append(tr("Value is complex"));
                            break;
    case ValueNotPositiveError: output.append(tr("Value is negative"));
                            break;
    case ValueNotIntegerError: output.append(tr("Only integer values"));
                            break;
    case UnitNotDimensionlessError: output.append(tr("Unit is not dimensionless"));
                            break;
    case UnitsNotConvertibleError: output.append(tr("Units not convertible"));
                            break;
    case PrefixError:       output.append(tr("Prefix does not fit unit"));
                            break;
    case UnknownVariableError:  output.append(tr("Unknown variable"));
                            break;
    case ProgramError:      output.append(tr("Calculation error"));
                            break;
    }

    return tr("error at position %1-%2: %3").arg(m_errorStartPosition).arg(m_errorEndPosition).arg(output);
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
    addRule(QString("unit=%1").arg(symbol), QString("bufferParameter, bufferUnit"));
    emit unitsChanged();
}

void PhyxCalculator::removeUnitRule(QString symbol)
{
    earleyParser->removeRule(QString("unit=%1").arg(symbol));
    emit unitsChanged();
}

void PhyxCalculator::addVariableRule(QString name)
{
    addRule(QString("variable=%1").arg(name), QString("bufferParameter, variableLoad"));
    emit variablesChanged();
}

void PhyxCalculator::removeVariableRule(QString name)
{
    earleyParser->removeRule(QString("variable=%1").arg(name));
    emit variablesChanged();
}

void PhyxCalculator::addConstantRule(QString name)
{
    addRule(QString("constant=%1").arg(name), QString("bufferParameter, constantLoad"));
    emit constantsChanged();
}

void PhyxCalculator::removeConstantRule(QString name)
{
    earleyParser->removeRule(QString("constant=%1").arg(name));
    emit constantsChanged();
}

void PhyxCalculator::addPrefixRule(QString symbol)
{
    addRule(QString("prefix=%1").arg(symbol), QString("bufferParameter, bufferPrefix"));
    emit prefixesChanged();
}

void PhyxCalculator::removePrefixRule(QString symbol)
{
    earleyParser->removeRule(QString("prefix=%1").arg(symbol));
    emit prefixesChanged();
}

void PhyxCalculator::addUnitGroupRule(QString name)
{
    addRule(QString("unitGroup=%1").arg(name), QString("bufferParameter, bufferUnitGroup"));
}

void PhyxCalculator::removeUnitGroupRule(QString name)
{
    earleyParser->removeRule(QString("unitGroup=%1").arg(name));
}

void PhyxCalculator::addFunctionRule(QString name, int parameterCount)
{
    QString ruleString;
    ruleString.append(QString("function=%1").arg(name));
    if (parameterCount == 1)
    {
        ruleString.append("|funcParam|");
    }
    else if (parameterCount > 1)
    {
        ruleString.append("(");
        for (int i = 0; i < parameterCount; i++)
        {
            if (i > 0)
                ruleString.append(",");
            ruleString.append("|p3|");
        }
        ruleString.append(")");
    }
    addRule(ruleString, QString("bufferParameter, functionRun"));
    emit functionsChanged();
}

void PhyxCalculator::removeFunctionRule(QString name, int parameterCount)
{
    QString ruleString;
    ruleString.append(QString("function=%1").arg(name));
    if (parameterCount == 1)
    {
        ruleString.append("|funcParam|");
    }
    else if (parameterCount > 1)
    {
        ruleString.append("(");
        for (int i = 0; i < parameterCount; i++)
        {
            if (i > 0)
                ruleString.append(",");
            ruleString.append("|p3|");
        }
        ruleString.append(")");
    }
    earleyParser->removeRule(ruleString);
    emit functionsChanged();
}

void PhyxCalculator::clearStack()
{
    foreach (PhyxVariable *variable, variableStack)
        variable->deleteLater();
    variableStack.clear();
    lowLevelStack.clear();
    functionParameterStack.clear();
}

void PhyxCalculator::clearResult()
{
    m_resultValue = PhyxValueDataType(PHYX_FLOAT_NULL,PHYX_FLOAT_NULL);
    m_resultUnit = "";
}

void PhyxCalculator::clearFlags()
{
    flagBuffer = 0;
}

bool PhyxCalculator::setExpression(QString expression)
{
    expression = stripComments(expression);
    expression = removeWhitespace(expression, &expressionWhitespaceList);

    if (expression.isEmpty())
    {
        earleyParser->clearWord();
        expressionIsParsable = false;
        if (listModeActive)
        {
            listValueLoad();
            listEnd();
            stackLevel = 1; //enable output
            outputVariable();
            stackLevel = 0;
        }
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
        clearResult();
        m_error = false;
        return evaluate(earleyParser->getTree(), m_expression, expressionWhitespaceList);
    }
    else
    {
        raiseException(SyntaxError);
        return false;
    }
}

bool PhyxCalculator::evaluate(QList<EarleyTreeItem> earleyTree, QString expression, QList<int> whiteSpaceList)
{
    stackLevel++;
#ifdef QT_DEBUG
        qDebug() << "evaluating expression:" << expression;
        qDebug() << "stack level:" << stackLevel;
#endif
    for (int i = (earleyTree.size()-1); i >= 0; i--)
    {
        EarleyTreeItem *earleyTreeItem = &earleyTree[i];
        //PhyxRule phyxRule = phyxRules.value(earleyTreeItem->rule);

        m_errorStartPosition = restoreErrorPosition(earleyTreeItem->startPos, whiteSpaceList);     //just in case
        m_errorEndPosition   = restoreErrorPosition(earleyTreeItem->endPos, whiteSpaceList)+1;
        //if (!phyxRule.functions.isEmpty())
        //if (!earleyTreeItem->rule->functions.isEmpty())
        //{
            foreach (QString function, earleyTreeItem->rule->functions)
            {
                if (this->hasError())
                {
                    stackLevel--;
#ifdef QT_DEBUG
                    qDebug() << "returned to stack level:" << stackLevel;
#endif
                    return false;
                }

                if (function == "bufferParameter")
                    parameterBuffer = expression.mid(earleyTreeItem->startPos, earleyTreeItem->endPos - earleyTreeItem->startPos + 1);
                else
                {
#ifdef QT_DEBUG
                    qDebug() << function;
#endif
                    if (functionMap.value(function, NULL) != NULL)
                        (this->*functionMap.value(function))();
#ifdef QT_DEBUG
                    else
                        qFatal("Function %s not found!", function.toAscii().constData());
#endif
                }
            }
        //}
    }
    stackLevel--;
#ifdef QT_DEBUG
    qDebug() << "returned to stack level:" << stackLevel;
#endif
    return true;
}

void PhyxCalculator::loadFile(QString fileName)
{
    QFile file(fileName);

    if (file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        QStringList lines = QString::fromUtf8(file.readAll()).split('\n');
        foreach (QString line, lines)
        {
            if (line.contains("//"))
                line.truncate(line.indexOf("//"));

            if (line.trimmed().isEmpty() || (line.trimmed().at(0) == '#'))
                continue;

            this->setExpression(line.trimmed());
            if (!this->evaluate())
                qDebug() << line;
        }
    }
}

PhyxVariable *PhyxCalculator::variable(QString name) const
{
    return variableManager->getVariable(name);
}

PhyxVariable *PhyxCalculator::constant(QString name) const
{
    return variableManager->getConstant(name);
}

PhyxUnit *PhyxCalculator::unit(QString symbol) const
{
    return unitSystem->unit(symbol);
}

PhyxUnitSystem::PhyxPrefix PhyxCalculator::prefix(QString symbol, QString unitGroup) const
{
    return unitSystem->prefix(symbol, unitGroup);
}

PhyxVariableManager::PhyxFunction *PhyxCalculator::function(QString name) const
{
    return variableManager->getFunction(name);
}

void PhyxCalculator::clearVariables()
{
    variableManager->clearVariables();
}

PhyxVariableManager::PhyxVariableMap *PhyxCalculator::variables() const
{
    return variableManager->variables();
}

PhyxVariableManager::PhyxVariableMap *PhyxCalculator::constants() const
{
    return variableManager->constants();
}

PhyxUnitSystem::PhyxUnitMap PhyxCalculator::units() const
{
    return unitSystem->units();
}

QList<PhyxUnitSystem::PhyxPrefix> PhyxCalculator::prefixes() const
{
    return unitSystem->prefixes();
}

QStringList PhyxCalculator::functions() const
{
    QStringList functionList;
    functionList.append(standardFunctionList);
    PhyxVariableManager::PhyxFunctionMap *functionMap = variableManager->functions();
    QMapIterator<QString, PhyxVariableManager::PhyxFunction*> mapIterator(*functionMap);
    while (mapIterator.hasNext())
    {
        mapIterator.next();
        functionList.append(mapIterator.key());
    }

    return functionList;
}

QString PhyxCalculator::complexToString(const PhyxValueDataType number, int precision, char numberFormat, QString imaginaryUnit, bool useBraces)
{
    QString string;
    boost::format format(QString("%.%1%2").arg(precision).arg(numberFormat).toStdString());
    int components = 0;
    bool useInteger = false;

    //check wheter number is integer or not
    /*if (number.imag() == PHYX_FLOAT_NULL)
    {
        if (static_cast<PhyxFloatDataType>(static_cast<PhyxIntegerDataType>(number.real())) == number.real())
            useInteger = true;
    }*/

    if (number.real() != PHYX_FLOAT_NULL)
    {
        std::ostringstream ss;
        if (!useInteger)
            ss << format % number.real();
//        else
//            ss << format % static_cast<PhyxIntegerDataType>(number.real());
        string.append(QString::fromStdString(ss.str()));
        components++;
    }

    if (number.imag() != PHYX_FLOAT_NULL)
    {
        if (!string.isEmpty())
            string.append("+");
        if ((number.imag() != 1) && (number.imag() != -1))
        {
            std::ostringstream ss;
            ss << format % number.imag();
            string.append(QString::fromStdString(ss.str()));
        }
        else if (number.imag() == -1)
        {
            string.append("-");
        }
        string.append(imaginaryUnit);
        components++;
    }

    string.replace("+-","-");

    if (string.isEmpty())
        string.append("0");
    else if ((components == 2) && useBraces)
    {
        string.prepend("(");
        string.append(")");
    }

    return string;
}

PhyxValueDataType PhyxCalculator::stringToComplex(QString string)
{
    PhyxValueDataType value;

    if (string.indexOf(QRegExp("[ij]")) != -1)
    {
        string.remove(QRegExp("[ij]"));
        if (string.isEmpty())
            string="1";

        PhyxFloatDataType tmpValue;
        std::istringstream inStream(string.toStdString());
        inStream >> tmpValue;
        value = PhyxValueDataType(PHYX_FLOAT_NULL,tmpValue);
    }
    else
    {
        PhyxFloatDataType tmpValue;
        std::istringstream inStream(string.toStdString());
        inStream >> tmpValue;
        value = PhyxValueDataType(tmpValue,PHYX_FLOAT_NULL);
    }

    return value;
}

PhyxIntegerDataType PhyxCalculator::hexToLongInt(QString string)
{
    PhyxIntegerDataType value = 0;
    PhyxIntegerDataType n = 1;

    string.remove("0x");
    for (int i = string.size()-1; i >= 0; i--)
    {
        if (string.at(i).isNumber())
            value += (string.at(i).toAscii()-48)*n;
        else
            value += (string.at(i).toAscii()-55)*n;
        n *= 16;
    }

    return value;
}

PhyxIntegerDataType PhyxCalculator::octToLongInt(QString string)
{
    PhyxIntegerDataType value = 0;
    PhyxIntegerDataType n = 1;

    string.remove("0o");
    for (int i = string.size()-1; i >= 0; i--)
    {
        value += (string.at(i).toAscii()-48)*n;
        n *= 8;
    }

    return value;
}

long PhyxCalculator::binToLongInt(QString string)
{
    PhyxIntegerDataType value = 0;
    PhyxIntegerDataType n = 1;

    string.remove("0b");
    for (int i = string.size()-1; i >= 0; i--)
    {
        value += (string.at(i).toAscii()-48)*n;
        n *= 2;
    }

    return value;
}

PhyxIntegerDataType PhyxCalculator::bcdToLongInt(PhyxIntegerDataType number)
{
    PhyxIntegerDataType output = 0;
    int i = 0;

    while (number != 0)
    {
        PhyxIntegerDataType digit = number & 0x0F;
        output += pow(10.0,i) * digit;

        number = number >> 4;
        i++;
    }
    return output;
}

QString PhyxCalculator::longIntToHex(PhyxIntegerDataType number)
{
    QString output;
    int i = 0;

    while (number != 0)
    {
        int rest = number % 16;
        if (rest < 10)
            output.prepend(QChar::fromAscii(rest+48));
        else
            output.prepend(QChar::fromAscii(rest+55));
        number /= 16;
        i++;
    }

    if ((i % 2) != 0)
        output.prepend("0");

    output.prepend("0x");
    return output;
}

QString PhyxCalculator::longIntToOct(PhyxIntegerDataType number)
{
    QString output;

    while (number != 0)
    {
        int rest = number % 8;
        output.prepend(QChar::fromAscii(rest+48));
        number /= 8;
    }

    output.prepend("0o");
    return output;
}

QString PhyxCalculator::longIntToBin(PhyxIntegerDataType number)
{
    QString output;
    int i = 0;

    while (number != 0)
    {
        int rest = number % 2;
        output.prepend(QChar::fromAscii(rest+48));
        number /= 2;
        i++;
    }

    while ((i % 8) != 0)
    {
        output.prepend("0");
        i++;
    }

    output.prepend("0b");
    return output;
}

PhyxIntegerDataType PhyxCalculator::longIntToBcd(PhyxIntegerDataType number)
{
    PhyxIntegerDataType output = 0;
    int i = 0;

    while (number != 0)
    {
        PhyxIntegerDataType digit = number % 10;
        output |= digit << (i*4);

        number /= 10;
        i++;
    }
    return output;
}

PhyxUnitSystem::PhyxPrefix PhyxCalculator::getBestPrefx(PhyxFloatDataType value, QString unitGroup, QString preferedPrefix) const
{
    PhyxFloatDataType preferedPrefixValue = PHYX_FLOAT_ONE;
    if (!preferedPrefix.isEmpty())
        preferedPrefixValue = unitSystem->prefix(preferedPrefix, unitGroup).value;


    QList<PhyxUnitSystem::PhyxPrefix> prefixes = unitSystem->prefixes(unitGroup);
    for (int i = prefixes.size()-1; i >= 0; i--)
    {
        if (prefixes.at(i).inputOnly)
            continue;

        prefixes[i].value /= preferedPrefixValue;
        PhyxFloatDataType tmpValue = value / prefixes.at(i).value;

        if (abs(tmpValue) >= PHYX_FLOAT_ONE)
            return prefixes.at(i);
    }

    PhyxUnitSystem::PhyxPrefix prefix;
    prefix.value = 1;
    prefix.symbol = "";
    return prefix;
}

PhyxCalculator::ResultVariable PhyxCalculator::formatVariable(PhyxVariable *variable,
                                                              OutputMode outputMode,
                                                              PrefixMode prefixMode,
                                                              int precision,
                                                              char numberFormat,
                                                              QString imaginaryUnit) const
{
    ResultVariable result;
    PhyxValueDataType value = variable->value();
    PhyxCompoundUnit *unit = variable->unit();

    switch (outputMode)
    {
    case OnlyBaseUnitsOutputMode:
        result.unit = unit->dimensionString();
        break;

    case MinimizeUnitOutputMode:
        result.unit = unit->symbol();
        if (prefixMode == UsePrefix)
        {
            if (unit->isSimpleUnit())
            {
                if (value.imag() == PHYX_FLOAT_NULL)
                {
                    PhyxUnitSystem::PhyxPrefix realPrefix = getBestPrefx(value.real(), unit->unitGroup(), unit->preferedPrefix());     //get best prefix
                    //PhyxUnitSystem::PhyxPrefix imagPrefix = getBestPrefx(value.imag(), unit->unitGroup(), unit->preferedPrefix());     //get best prefix
                    //value = PhyxValueDataType(value.real() / realPrefix.value, value.imag() / imagPrefix.value);
                    value = PhyxValueDataType(value.real() / realPrefix.value, PHYX_FLOAT_NULL);

                    if (!unit->preferedPrefix().isEmpty())      // preferd prefix handling
                        result.unit.remove(0, unit->preferedPrefix().size());

                    result.unit.prepend(realPrefix.symbol);
                } else if (value.real() == PHYX_FLOAT_NULL)
                {
                    PhyxUnitSystem::PhyxPrefix imagPrefix = getBestPrefx(value.imag(), unit->unitGroup(), unit->preferedPrefix());     //get best prefix
                    value = PhyxValueDataType(PHYX_FLOAT_NULL, value.imag() / imagPrefix.value);

                    if (!unit->preferedPrefix().isEmpty())      // preferd prefix handling
                        result.unit.remove(0, unit->preferedPrefix().size());

                    result.unit.prepend(imagPrefix.symbol);
                }
            }
        }
        else if (prefixMode == UseNoPrefix)
        {

        }
        break;

    case ForceInputUnitsOutputMode:
        if (prefixMode == UsePrefix)
        {

        }
        else if (prefixMode == UseNoPrefix)
        {

        }
        break;
    }

    result.value = complexToString(value, precision, numberFormat, imaginaryUnit, !unit->isOne());

    return result;
}

void PhyxCalculator::valueCheckComplex()
{
    if (variableStack.size() < 1)
    {
        raiseException(ProgramError);
        return;
    }
    PhyxVariable *variable1 = variableStack.pop();

    if (variable1->isComplex())
        raiseException(ValueComplexError);

    variableStack.push(variable1);
}

void PhyxCalculator::valueCheckComplex2()
{
    if (variableStack.size() < 2)
    {
        raiseException(ProgramError);
        return;
    }
    PhyxVariable *variable1 = variableStack.pop();
    PhyxVariable *variable2 = variableStack.pop();

    if (variable1->isComplex() || variable2->isComplex())
        raiseException(ValueComplexError);

    variableStack.push(variable2);
    variableStack.push(variable1);
}

void PhyxCalculator::valueCheckComplex2th()
{
    if (variableStack.size() < 2)
    {
        raiseException(ProgramError);
        return;
    }
    PhyxVariable *variable1 = variableStack.pop();
    PhyxVariable *variable2 = variableStack.pop();

    if (variable2->isComplex())
        raiseException(ValueComplexError);

    variableStack.push(variable2);
    variableStack.push(variable1);
}

void PhyxCalculator::valueCheckComplex3th()
{
    if (variableStack.size() < 3)
    {
        raiseException(ProgramError);
        return;
    }
    PhyxVariable *variable1 = variableStack.pop();
    PhyxVariable *variable2 = variableStack.pop();
    PhyxVariable *variable3 = variableStack.pop();

    if (variable3->isComplex())
        raiseException(ValueComplexError);

    variableStack.push(variable3);
    variableStack.push(variable2);
    variableStack.push(variable1);
}

void PhyxCalculator::valueCheckPositive()
{
    if (variableStack.size() < 1)
    {
        raiseException(ProgramError);
        return;
    }
    PhyxVariable *variable1 = variableStack.pop();

    if (!variable1->isPositive())
        raiseException(ValueNotPositiveError);

    variableStack.push(variable1);
}

void PhyxCalculator::valueCheckInteger()
{
    if (variableStack.size() < 1)
    {
        raiseException(ProgramError);
        return;
    }
    PhyxVariable *variable1 = variableStack.pop();

    if (!variable1->isInteger())
        raiseException(ValueNotIntegerError);

    variableStack.push(variable1);
}

void PhyxCalculator::valueCheckInteger2()
{
    if (variableStack.size() < 2)
    {
        raiseException(ProgramError);
        return;
    }
    PhyxVariable *variable1 = variableStack.pop();
    PhyxVariable *variable2 = variableStack.pop();

    if (!variable1->isInteger())
        raiseException(ValueNotIntegerError);
    else if (!variable2->isInteger())
        raiseException(ValueNotIntegerError);

    variableStack.push(variable2);
    variableStack.push(variable1);
}

void PhyxCalculator::valueCheckInteger2th()
{
    if (variableStack.size() < 2)
    {
        raiseException(ProgramError);
        return;
    }
    PhyxVariable *variable1 = variableStack.pop();
    PhyxVariable *variable2 = variableStack.pop();

    if (!variable2->isInteger())
        raiseException(ValueNotIntegerError);

    variableStack.push(variable2);
    variableStack.push(variable1);
}

void PhyxCalculator::valueCheckInteger3th()
{
    if (variableStack.size() < 3)
    {
        raiseException(ProgramError);
        return;
    }
    PhyxVariable *variable1 = variableStack.pop();
    PhyxVariable *variable2 = variableStack.pop();
    PhyxVariable *variable3 = variableStack.pop();

    if (!variable3->isInteger())
        raiseException(ValueNotIntegerError);

    variableStack.push(variable3);
    variableStack.push(variable2);
    variableStack.push(variable1);
}

void PhyxCalculator::valuePush1()
{
    valueBuffer = PhyxValueDataType(PHYX_FLOAT_ONE,PHYX_FLOAT_NULL);
    pushVariable();
}

void PhyxCalculator::valuePush2()
{
    valueBuffer = PhyxValueDataType(PHYX_FLOAT_TWO,PHYX_FLOAT_NULL);
    pushVariable();
}

void PhyxCalculator::valuePush3()
{
    valueBuffer = PhyxValueDataType(PHYX_FLOAT_THREE,PHYX_FLOAT_NULL);
    pushVariable();
}

void PhyxCalculator::valueCopy()
{
    if (variableStack.size() < 1)
    {
        raiseException(ProgramError);
        return;
    }
    PhyxVariable *variable1 = variableStack.pop();

    PhyxVariable *variable2 = new PhyxVariable(this);
    PhyxVariable::copyVariable(variable1, variable2);

    variableStack.push(variable1);
    variableStack.push(variable2);
}

void PhyxCalculator::valueInvert()
{
    if (variableStack.size() < 1)
    {
        raiseException(ProgramError);
        return;
    }
    PhyxVariable *variable1 = variableStack.pop();

    variable1->setValue(-variable1->value());
    variableStack.push(variable1);
}

void PhyxCalculator::valueAdd()
{
    if (variableStack.size() < 2)
    {
        raiseException(ProgramError);
        return;
    }
    PhyxVariable *variable2 = variableStack.pop();
    PhyxVariable *variable1 = variableStack.pop();

    variable1->setValue(variable1->value() + variable2->value());
    variableStack.push(variable1);

    variable2->deleteLater();
}

void PhyxCalculator::valueSub()
{
    if (variableStack.size() < 2)
    {
        raiseException(ProgramError);
        return;
    }
    PhyxVariable *variable2 = variableStack.pop();
    PhyxVariable *variable1 = variableStack.pop();

    variable1->setValue(variable1->value() - variable2->value());
    variableStack.push(variable1);

    variable2->deleteLater();
}

void PhyxCalculator::valueMul()
{
    if (variableStack.size() < 2)
    {
        raiseException(ProgramError);
        return;
    }
    PhyxVariable *variable2 = variableStack.pop();
    PhyxVariable *variable1 = variableStack.pop();

    variable1->setValue(variable1->value() * variable2->value());
    variableStack.push(variable1);

    variable2->deleteLater();
}

void PhyxCalculator::valueDiv()
{
    if (variableStack.size() < 2)
    {
        raiseException(ProgramError);
        return;
    }
    PhyxVariable *variable2 = variableStack.pop();
    PhyxVariable *variable1 = variableStack.pop();

    variable1->setValue(variable1->value() / variable2->value());
    variableStack.push(variable1);

    variable2->deleteLater();
}

void PhyxCalculator::valueMod()
{
    if (variableStack.size() < 2)
    {
        raiseException(ProgramError);
        return;
    }
    PhyxVariable *variable2 = variableStack.pop();
    PhyxVariable *variable1 = variableStack.pop();

    variable1->setValue(PhyxValueDataType(static_cast<PhyxFloatDataType>(variable1->toInt() % variable2->toInt()), PHYX_FLOAT_NULL));
    variableStack.push(variable1);

    variable2->deleteLater();
}

void PhyxCalculator::valueNeg()
{
    if (variableStack.size() < 1)
    {
        raiseException(ProgramError);
        return;
    }
    PhyxVariable *variable1 = variableStack.pop();

    variable1->setValue(-variable1->value());
    variableStack.push(variable1);
}

void PhyxCalculator::valueNoPow()
{
    if (variableStack.size() < 2)
    {
        raiseException(ProgramError);
        return;
    }
    PhyxVariable *variable2 = variableStack.pop();
    PhyxVariable *variable1 = variableStack.pop();

    if (variable1->unit()->isOne())
    {
        variableStack.push(variable1);
        variableStack.push(variable2);
        valuePow();
    }
    else
    {
        variableStack.push(variable1);
        variable2->deleteLater();
    }
}

void PhyxCalculator::valuePow()
{
    if (variableStack.size() < 2)
    {
        raiseException(ProgramError);
        return;
    }
    PhyxVariable *variable2 = variableStack.pop();
    PhyxVariable *variable1 = variableStack.pop();

    if (variable2->value().imag() == PHYX_FLOAT_NULL)     // precision fix
        variable1->setValue(pow(variable1->value(),variable2->value().real()));
    else
        variable1->setValue(pow(variable1->value(),variable2->value()));
    variableStack.push(variable1);

    variable2->deleteLater();
}

void PhyxCalculator::valueSin()
{
    if (variableStack.size() < 1)
    {
        raiseException(ProgramError);
        return;
    }
    PhyxVariable *variable1 = variableStack.pop();

    variable1->setValue(sin(variable1->value()));
    variableStack.push(variable1);
}

void PhyxCalculator::valueArcsin()
{
    if (variableStack.size() < 1)
    {
        raiseException(ProgramError);
        return;
    }
    PhyxVariable *variable1 = variableStack.pop();

    variable1->setValue(boost::math::asin(variable1->value()));
    variableStack.push(variable1);
}

void PhyxCalculator::valueCos()
{
    if (variableStack.size() < 1)
    {
        raiseException(ProgramError);
        return;
    }
    PhyxVariable *variable1 = variableStack.pop();

    variable1->setValue(cos(variable1->value()));
    variableStack.push(variable1);
}

void PhyxCalculator::valueArccos()
{
    if (variableStack.size() < 1)
    {
        raiseException(ProgramError);
        return;
    }
    PhyxVariable *variable1 = variableStack.pop();

    variable1->setValue(boost::math::acos(variable1->value()));
    variableStack.push(variable1);
}

void PhyxCalculator::valueTan()
{
    if (variableStack.size() < 1)
    {
        raiseException(ProgramError);
        return;
    }
    PhyxVariable *variable1 = variableStack.pop();

    variable1->setValue(tan(variable1->value()));
    variableStack.push(variable1);
}

void PhyxCalculator::valueArctan()
{
    if (variableStack.size() < 1)
    {
        raiseException(ProgramError);
        return;
    }
    PhyxVariable *variable1 = variableStack.pop();

    variable1->setValue(boost::math::atan(variable1->value()));
    variableStack.push(variable1);
}

void PhyxCalculator::valueCot()
{
    if (variableStack.size() < 1)
    {
        raiseException(ProgramError);
        return;
    }
    PhyxVariable *variable1 = variableStack.pop();

    variable1->setValue(PhyxValueDataType(PHYX_FLOAT_ONE, PHYX_FLOAT_NULL)/tan(variable1->value()));
    variableStack.push(variable1);
}

void PhyxCalculator::valueArccot()
{
    if (variableStack.size() < 1)
    {
        raiseException(ProgramError);
        return;
    }
    PhyxVariable *variable1 = variableStack.pop();

    variable1->setValue(PhyxValueDataType(PHYX_FLOAT_ONE, PHYX_FLOAT_NULL)/boost::math::atan(variable1->value()));
    variableStack.push(variable1);
}

void PhyxCalculator::valueSec()
{
    if (variableStack.size() < 1)
    {
        raiseException(ProgramError);
        return;
    }
    PhyxVariable *variable1 = variableStack.pop();

    variable1->setValue(PhyxValueDataType(PHYX_FLOAT_ONE, PHYX_FLOAT_NULL)/cos(variable1->value()));
    variableStack.push(variable1);
}

void PhyxCalculator::valueArcsec()
{
    if (variableStack.size() < 1)
    {
        raiseException(ProgramError);
        return;
    }
    PhyxVariable *variable1 = variableStack.pop();

    variable1->setValue(PhyxValueDataType(PHYX_FLOAT_ONE, PHYX_FLOAT_NULL)/boost::math::acos(variable1->value()));
    variableStack.push(variable1);
}

void PhyxCalculator::valueCsc()
{
    if (variableStack.size() < 1)
    {
        raiseException(ProgramError);
        return;
    }
    PhyxVariable *variable1 = variableStack.pop();

    variable1->setValue(PhyxValueDataType(PHYX_FLOAT_ONE, PHYX_FLOAT_NULL)/sin(variable1->value()));
    variableStack.push(variable1);
}

void PhyxCalculator::valueArccsc()
{
    if (variableStack.size() < 1)
    {
        raiseException(ProgramError);
        return;
    }
    PhyxVariable *variable1 = variableStack.pop();

    variable1->setValue(PhyxValueDataType(PHYX_FLOAT_ONE, PHYX_FLOAT_NULL)/boost::math::asin(variable1->value()));
    variableStack.push(variable1);
}

void PhyxCalculator::valueSinh()
{
    if (variableStack.size() < 1)
    {
        raiseException(ProgramError);
        return;
    }
    PhyxVariable *variable1 = variableStack.pop();

    variable1->setValue(sinh(variable1->value()));
    variableStack.push(variable1);
}

void PhyxCalculator::valueArcsinh()
{
    if (variableStack.size() < 1)
    {
        raiseException(ProgramError);
        return;
    }
    PhyxVariable *variable1 = variableStack.pop();

    variable1->setValue(boost::math::asinh(variable1->value()));
    variableStack.push(variable1);
}

void PhyxCalculator::valueCosh()
{
    if (variableStack.size() < 1)
    {
        raiseException(ProgramError);
        return;
    }
    PhyxVariable *variable1 = variableStack.pop();

    variable1->setValue(cosh(variable1->value()));
    variableStack.push(variable1);
}

void PhyxCalculator::valueArccosh()
{
    if (variableStack.size() < 1)
    {
        raiseException(ProgramError);
        return;
    }
    PhyxVariable *variable1 = variableStack.pop();

    variable1->setValue(boost::math::acosh(variable1->value()));
    variableStack.push(variable1);
}

void PhyxCalculator::valueTanh()
{
    if (variableStack.size() < 1)
    {
        raiseException(ProgramError);
        return;
    }
    PhyxVariable *variable1 = variableStack.pop();

    variable1->setValue(tanh(variable1->value()));
    variableStack.push(variable1);
}

void PhyxCalculator::valueArctanh()
{
    if (variableStack.size() < 1)
    {
        raiseException(ProgramError);
        return;
    }
    PhyxVariable *variable1 = variableStack.pop();

    variable1->setValue(boost::math::atanh(variable1->value()));
    variableStack.push(variable1);
}

void PhyxCalculator::valueCoth()
{
    if (variableStack.size() < 1)
    {
        raiseException(ProgramError);
        return;
    }
    PhyxVariable *variable1 = variableStack.pop();

    variable1->setValue(PhyxValueDataType(PHYX_FLOAT_ONE, PHYX_FLOAT_NULL)/tanh(variable1->value()));
    variableStack.push(variable1);
}

void PhyxCalculator::valueArccoth()
{
    if (variableStack.size() < 1)
    {
        raiseException(ProgramError);
        return;
    }
    PhyxVariable *variable1 = variableStack.pop();

    variable1->setValue(PhyxValueDataType(PHYX_FLOAT_ONE, PHYX_FLOAT_NULL)/boost::math::atanh(variable1->value()));
    variableStack.push(variable1);
}

void PhyxCalculator::valueSech()
{
    if (variableStack.size() < 1)
    {
        raiseException(ProgramError);
        return;
    }
    PhyxVariable *variable1 = variableStack.pop();

    variable1->setValue(PhyxValueDataType(PHYX_FLOAT_ONE, PHYX_FLOAT_NULL)/cosh(variable1->value()));
    variableStack.push(variable1);
}

void PhyxCalculator::valueArcsech()
{
    if (variableStack.size() < 1)
    {
        raiseException(ProgramError);
        return;
    }
    PhyxVariable *variable1 = variableStack.pop();

    variable1->setValue(PhyxValueDataType(PHYX_FLOAT_ONE, PHYX_FLOAT_NULL)/boost::math::acosh(variable1->value()));
    variableStack.push(variable1);
}

void PhyxCalculator::valueCsch()
{
    if (variableStack.size() < 1)
    {
        raiseException(ProgramError);
        return;
    }
    PhyxVariable *variable1 = variableStack.pop();

    variable1->setValue(PhyxValueDataType(PHYX_FLOAT_ONE, PHYX_FLOAT_NULL)/sinh(variable1->value()));
    variableStack.push(variable1);
}

void PhyxCalculator::valueArccsch()
{
    if (variableStack.size() < 1)
    {
        raiseException(ProgramError);
        return;
    }
    PhyxVariable *variable1 = variableStack.pop();

    variable1->setValue(PhyxValueDataType(PHYX_FLOAT_ONE, PHYX_FLOAT_NULL)/boost::math::asinh(variable1->value()));
    variableStack.push(variable1);
}

void PhyxCalculator::valueExp()
{
    if (variableStack.size() < 1)
    {
        raiseException(ProgramError);
        return;
    }
    PhyxVariable *variable1 = variableStack.pop();

    variable1->setValue(exp(variable1->value()));
    variableStack.push(variable1);
}

void PhyxCalculator::valueLn()
{
    if (variableStack.size() < 1)
    {
        raiseException(ProgramError);
        return;
    }
    PhyxVariable *variable1 = variableStack.pop();

    variable1->setValue(log(variable1->value()));
    variableStack.push(variable1);
}

void PhyxCalculator::valueLog10()
{
    if (variableStack.size() < 1)
    {
        raiseException(ProgramError);
        return;
    }
    PhyxVariable *variable1 = variableStack.pop();

    variable1->setValue(log10(variable1->value()));
    variableStack.push(variable1);
}

void PhyxCalculator::valueLog2()
{
    if (variableStack.size() < 1)
    {
        raiseException(ProgramError);
        return;
    }
    PhyxVariable *variable1 = variableStack.pop();

    variable1->setValue(log(variable1->value()));
    variableStack.push(variable1);
}

void PhyxCalculator::valueLogn()
{
    if (variableStack.size() < 2)
    {
        raiseException(ProgramError);
        return;
    }
    PhyxVariable *variable2 = variableStack.pop();
    PhyxVariable *variable1 = variableStack.pop();

    variable1->setValue(log(variable2->value()) / log(variable1->value()));
    variableStack.push(variable1);

    variable2->deleteLater();
}

void PhyxCalculator::valueRoot()
{
    if (variableStack.size() < 2)
    {
        raiseException(ProgramError);
        return;
    }
    PhyxVariable *variable2 = variableStack.pop();
    PhyxVariable *variable1 = variableStack.pop();

    variable1->setValue(exp(log(variable2->value()) / variable1->value()));
    variableStack.push(variable1);

    variable2->deleteLater();
}

void PhyxCalculator::valueSqrt()
{
    if (variableStack.size() < 1)
    {
        raiseException(ProgramError);
        return;
    }
    PhyxVariable *variable1 = variableStack.pop();

    variable1->setValue(sqrt(variable1->value()));
    variableStack.push(variable1);
}

void PhyxCalculator::valueAbs()
{
    if (variableStack.size() < 1)
    {
        raiseException(ProgramError);
        return;
    }
    PhyxVariable *variable1 = variableStack.pop();

    variable1->setValue(abs(variable1->value()));
    variableStack.push(variable1);
}

void PhyxCalculator::valueMax()
{
    if (variableStack.size() < 2)
    {
        raiseException(ProgramError);
        return;
    }
    PhyxVariable *variable1 = variableStack.pop();
    PhyxVariable *variable2 = variableStack.pop();

    if (variable1->value().real() >= variable2->value().real())
    {
        variableStack.push(variable1);
        variable2->deleteLater();
    }
    else
    {
        variableStack.push(variable2);
        variable1->deleteLater();
    }
}

void PhyxCalculator::valueMin()
{
    if (variableStack.size() < 2)
    {
        raiseException(ProgramError);
        return;
    }
    PhyxVariable *variable1 = variableStack.pop();
    PhyxVariable *variable2 = variableStack.pop();

    if (variable1->value().real()<= variable2->value().real())
    {
        variableStack.push(variable1);
        variable2->deleteLater();
    }
    else
    {
        variableStack.push(variable2);
        variable1->deleteLater();
    }

    variable2->deleteLater();
}

void PhyxCalculator::valueAvg()
{
    if (variableStack.size() < 2)
    {
        raiseException(ProgramError);
        return;
    }
    PhyxVariable *variable1 = variableStack.pop();
    PhyxVariable *variable2 = variableStack.pop();

    variable1->setValue((variable1->value() + variable2->value())
                        / PhyxValueDataType(PHYX_FLOAT_TWO,PHYX_FLOAT_NULL));
    variableStack.push(variable1);

    variable2->deleteLater();
}

void PhyxCalculator::valuePi()
{
#ifndef Q_WS_S60
    valueBuffer = PhyxValueDataType(boost::math::constants::pi<PhyxFloatDataType>(), PHYX_FLOAT_NULL);
#else
    valueBuffer = PhyxValueDataType(static_cast<PhyxFloatDataType>(M_PI), PHYX_FLOAT_NULL);
#endif
    unitBuffer = "";
    pushVariable();
}

void PhyxCalculator::valueE()
{
#ifndef Q_WS_S60
    valueBuffer = PhyxValueDataType(boost::math::constants::e<PhyxFloatDataType>(), PHYX_FLOAT_NULL);
#else
    valueBuffer = PhyxValueDataType(static_cast<PhyxFloatDataType>(M_E), PHYX_FLOAT_NULL);
#endif
    unitBuffer = "";
    pushVariable();
}

void PhyxCalculator::valueInt()
{
    if (variableStack.size() < 1)
    {
        raiseException(ProgramError);
        return;
    }
    PhyxVariable *variable1 = variableStack.pop();

    PhyxValueDataType value(static_cast<PhyxFloatDataType>(variable1->toInt()),PHYX_FLOAT_NULL);
    variable1->setValue(value);
    variableStack.push(variable1);
}

void PhyxCalculator::valueTrunc()
{
    if (variableStack.size() < 1)
    {
        raiseException(ProgramError);
        return;
    }
    PhyxVariable *variable1 = variableStack.pop();

#ifndef Q_WS_S60
    PhyxValueDataType value(boost::math::trunc<PhyxFloatDataType>(variable1->value().real()),PHYX_FLOAT_NULL);
#else
    PhyxValueDataType value(static_cast<PhyxFloatDataType>(variable1->toInt()),PHYX_FLOAT_NULL);
#endif
    variable1->setValue(value);
    variableStack.push(variable1);
}

void PhyxCalculator::valueFloor()
{
    if (variableStack.size() < 1)
    {
        raiseException(ProgramError);
        return;
    }
    PhyxVariable *variable1 = variableStack.pop();

    PhyxValueDataType value(floor(variable1->value().real()),PHYX_FLOAT_NULL);
    variable1->setValue(value);
    variableStack.push(variable1);
}

void PhyxCalculator::valueRound()
{
    if (variableStack.size() < 1)
    {
        raiseException(ProgramError);
        return;
    }
    PhyxVariable *variable1 = variableStack.pop();

#ifndef Q_WS_S60
    PhyxValueDataType value(boost::math::round<PhyxFloatDataType>(variable1->value().real()),PHYX_FLOAT_NULL);
#else
    PhyxValueDataType value(floor(variable1->value().real()+0.5L),PHYX_FLOAT_NULL);
#endif
    variable1->setValue(value);
    variableStack.push(variable1);
}

void PhyxCalculator::valueCeil()
{
    if (variableStack.size() < 1)
    {
        raiseException(ProgramError);
        return;
    }
    PhyxVariable *variable1 = variableStack.pop();

    PhyxValueDataType value(ceil(variable1->value().real()),PHYX_FLOAT_NULL);
    variable1->setValue(value);
    variableStack.push(variable1);
}

void PhyxCalculator::valueSign()
{
    if (variableStack.size() < 1)
    {
        raiseException(ProgramError);
        return;
    }
    PhyxVariable *variable1 = variableStack.pop();

    //PhyxValueDataType value(boost::math::copysign(PHYX_FLOAT_ONE,variable1->value().real()),PHYX_FLOAT_NULL);
    if (variable1->value().real() > PHYX_FLOAT_NULL)
    {
        PhyxValueDataType value(PHYX_FLOAT_ONE,PHYX_FLOAT_NULL);
        variable1->setValue(value);
    }
    else if (variable1->value().real() == PHYX_FLOAT_NULL)
    {
        PhyxValueDataType value(PHYX_FLOAT_NULL,PHYX_FLOAT_NULL);
        variable1->setValue(value);
    }
    else
    {
        PhyxValueDataType value(-PHYX_FLOAT_ONE,PHYX_FLOAT_NULL);
        variable1->setValue(value);
    }

    variableStack.push(variable1);
}

void PhyxCalculator::valueHeaviside()
{
    if (variableStack.size() < 1)
    {
        raiseException(ProgramError);
        return;
    }
    PhyxVariable *variable1 = variableStack.pop();

    if (variable1->value().real() >= PHYX_FLOAT_NULL)
        variable1->setValue(PhyxValueDataType(PHYX_FLOAT_ONE,PHYX_FLOAT_NULL));
    else
        variable1->setValue(PhyxValueDataType(PHYX_FLOAT_NULL,PHYX_FLOAT_NULL));
    variableStack.push(variable1);
}

void PhyxCalculator::valueRand()
{
    valueBuffer = PhyxValueDataType(static_cast<PhyxFloatDataType>(qrand() / static_cast<PhyxFloatDataType>(RAND_MAX)),PHYX_FLOAT_NULL);
    unitBuffer = "";
    pushVariable();
}

void PhyxCalculator::valueRandint()
{
    if (variableStack.size() < 1)
    {
        raiseException(ProgramError);
        return;
    }
    PhyxVariable *variable1 = variableStack.pop();

    PhyxIntegerDataType max = variable1->toInt();
    valueBuffer = static_cast<PhyxIntegerDataType>(static_cast<PhyxFloatDataType>(max) * static_cast<PhyxFloatDataType>(qrand())/static_cast<PhyxFloatDataType>(RAND_MAX));
    unitBuffer = "";
    pushVariable();

    variable1->deleteLater();
}

void PhyxCalculator::valueRandg()
{
    if (variableStack.size() < 2)
    {
        raiseException(ProgramError);
        return;
    }
    PhyxVariable *variable1 = variableStack.pop();
    PhyxVariable *variable2 = variableStack.pop();

    PhyxFloatDataType mean, standard;
    mean = variable1->value().real();
    standard = variable2->value().real();

    boost::variate_generator<boost::mt19937, boost::normal_distribution<> >
        generator(boost::mt19937(QDateTime::currentMSecsSinceEpoch()),
                  boost::normal_distribution<>(mean,standard));
    variable1->setValue(PhyxValueDataType(static_cast<PhyxFloatDataType>(generator()),PHYX_FLOAT_NULL));

    variableStack.push(variable1);

    variable2->deleteLater();
}

void PhyxCalculator::valueFaculty()
{
    if (variableStack.size() < 1)
    {
        raiseException(ProgramError);
        return;
    }
    PhyxVariable *variable1 = variableStack.pop();

    PhyxFloatDataType value = PHYX_FLOAT_ONE;
    int n = static_cast<int>(variable1->toInt());

    for (int i = 2L; i <=n; i++)
        value *= static_cast<PhyxFloatDataType>(i);

    variable1->setValue(PhyxValueDataType(value,PHYX_FLOAT_NULL));
    variableStack.push(variable1);
}

void PhyxCalculator::valueBcd()
{
    if (variableStack.size() < 1)
    {
        raiseException(ProgramError);
        return;
    }
    PhyxVariable *variable1 = variableStack.pop();

    variable1->setValue(PhyxValueDataType(static_cast<PhyxFloatDataType>(bcdToLongInt(variable1->toInt())),PHYX_FLOAT_NULL));
    variableStack.push(variable1);
}

void PhyxCalculator::valueToBcd()
{
    if (variableStack.size() < 1)
    {
        raiseException(ProgramError);
        return;
    }
    PhyxVariable *variable1 = variableStack.pop();

    variable1->setValue(PhyxValueDataType(static_cast<PhyxFloatDataType>(longIntToBcd(variable1->toInt())),PHYX_FLOAT_NULL));
    variableStack.push(variable1);
}

void PhyxCalculator::valueAns()
{
    parameterBuffer = "#";
    variableLoad();
}

void PhyxCalculator::complexReal()
{
    if (variableStack.size() < 1)
    {
        raiseException(ProgramError);
        return;
    }
    PhyxVariable *variable1 = variableStack.pop();

    variable1->setValue(PhyxValueDataType(real(variable1->value()),PHYX_FLOAT_NULL));
    variableStack.push(variable1);
}

void PhyxCalculator::complexImag()
{
    if (variableStack.size() < 1)
    {
        raiseException(ProgramError);
        return;
    }
    PhyxVariable *variable1 = variableStack.pop();

    variable1->setValue(PhyxValueDataType(imag(variable1->value()),PHYX_FLOAT_NULL));
    variableStack.push(variable1);
}

void PhyxCalculator::complexArg()
{
    if (variableStack.size() < 1)
    {
        raiseException(ProgramError);
        return;
    }
    PhyxVariable *variable1 = variableStack.pop();

    variable1->setValue(PhyxValueDataType(arg(variable1->value()),PHYX_FLOAT_NULL));
    variableStack.push(variable1);
}

void PhyxCalculator::complexNorm()
{
    if (variableStack.size() < 1)
    {
        raiseException(ProgramError);
        return;
    }
    PhyxVariable *variable1 = variableStack.pop();

    variable1->setValue(PhyxValueDataType(norm(variable1->value()),PHYX_FLOAT_NULL));
    variableStack.push(variable1);
}

void PhyxCalculator::complexConj()
{
    if (variableStack.size() < 1)
    {
        raiseException(ProgramError);
        return;
    }
    PhyxVariable *variable1 = variableStack.pop();

    variable1->setValue(conj(variable1->value()));
    variableStack.push(variable1);
}

void PhyxCalculator::complexPolar()
{
    if (variableStack.size() < 2)
    {
        raiseException(ProgramError);
        return;
    }
    PhyxVariable *variable2 = variableStack.pop();
    PhyxVariable *variable1 = variableStack.pop();

    variable1->setValue(std::polar(variable1->value().real(), variable2->value().real()));
    variableStack.push(variable1);

    variable2->deleteLater();
}

void PhyxCalculator::logicAnd()
{
    if (variableStack.size() < 2)
    {
        raiseException(ProgramError);
        return;
    }
    PhyxVariable *variable1 = variableStack.pop();
    PhyxVariable *variable2 = variableStack.pop();

    variable1->setValue(variable1->value().real() && variable2->value().real());
    variableStack.push(variable1);

    variable2->deleteLater();
}

void PhyxCalculator::logicOr()
{
    if (variableStack.size() < 2)
    {
        raiseException(ProgramError);
        return;
    }
    PhyxVariable *variable1 = variableStack.pop();
    PhyxVariable *variable2 = variableStack.pop();

    variable1->setValue(variable1->value().real() || variable2->value().real());
    variableStack.push(variable1);

    variable2->deleteLater();
}

void PhyxCalculator::logicNand()
{
    if (variableStack.size() < 2)
    {
        raiseException(ProgramError);
        return;
    }
    PhyxVariable *variable1 = variableStack.pop();
    PhyxVariable *variable2 = variableStack.pop();

    variable1->setValue(!(variable1->value().real() && variable2->value().real()));
    variableStack.push(variable1);

    variable2->deleteLater();
}

void PhyxCalculator::logicNor()
{
    if (variableStack.size() < 2)
    {
        raiseException(ProgramError);
        return;
    }
    PhyxVariable *variable1 = variableStack.pop();
    PhyxVariable *variable2 = variableStack.pop();

    variable1->setValue(!(variable1->value().real() || variable2->value().real()));
    variableStack.push(variable1);

    variable2->deleteLater();
}

void PhyxCalculator::logicXand()
{
    if (variableStack.size() < 2)
    {
        raiseException(ProgramError);
        return;
    }
    PhyxVariable *variable1 = variableStack.pop();
    PhyxVariable *variable2 = variableStack.pop();

    variable1->setValue((variable1->value().real() && variable2->value().real())
                        || (!variable1->value().real() && !variable2->value().real()));
    variableStack.push(variable1);

    variable2->deleteLater();
}

void PhyxCalculator::logicXor()
{
    if (variableStack.size() < 2)
    {
        raiseException(ProgramError);
        return;
    }
    PhyxVariable *variable1 = variableStack.pop();
    PhyxVariable *variable2 = variableStack.pop();

    variable1->setValue((variable1->value().real() && !variable2->value().real())
                        || (!variable1->value().real() && variable2->value().real()));
    variableStack.push(variable1);

    variable2->deleteLater();
}

void PhyxCalculator::logicNot()
{
    if (variableStack.size() < 1)
    {
        raiseException(ProgramError);
        return;
    }
    PhyxVariable *variable1 = variableStack.pop();

    variable1->setValue(!variable1->value().real());
    variableStack.push(variable1);

}

void PhyxCalculator::logicEqual()
{
    if (variableStack.size() < 2)
    {
        raiseException(ProgramError);
        return;
    }
    PhyxVariable *variable1 = variableStack.pop();
    PhyxVariable *variable2 = variableStack.pop();

    PhyxVariable *variable3 = new PhyxVariable(this);
    variable3->setValue((variable1->value() == variable2->value()) && variable1->unit()->isSame(variable2->unit()));
    variableStack.push(variable3);

    variable1->deleteLater();
    variable2->deleteLater();
}

void PhyxCalculator::logicNotEqual()
{
    if (variableStack.size() < 2)
    {
        raiseException(ProgramError);
        return;
    }
    PhyxVariable *variable1 = variableStack.pop();
    PhyxVariable *variable2 = variableStack.pop();

    PhyxVariable *variable3 = new PhyxVariable(this);
    variable3->setValue((variable1->value() != variable2->value()) || !variable1->unit()->isSame(variable2->unit()));
    variableStack.push(variable3);

    variable1->deleteLater();
    variable2->deleteLater();
}

void PhyxCalculator::logicGreater()
{
    if (variableStack.size() < 2)
    {
        raiseException(ProgramError);
        return;
    }
    PhyxVariable *variable2 = variableStack.pop();
    PhyxVariable *variable1 = variableStack.pop();

    PhyxVariable *variable3 = new PhyxVariable(this);
    variable3->setValue(variable1->value().real() > variable2->value().real());
    variableStack.push(variable3);

    variable1->deleteLater();
    variable2->deleteLater();
}

void PhyxCalculator::logicGreaterOrEqual()
{
    if (variableStack.size() < 2)
    {
        raiseException(ProgramError);
        return;
    }
    PhyxVariable *variable2 = variableStack.pop();
    PhyxVariable *variable1 = variableStack.pop();

    PhyxVariable *variable3 = new PhyxVariable(this);
    variable3->setValue(variable1->value().real() >= variable2->value().real());
    variableStack.push(variable3);

    variable1->deleteLater();
    variable2->deleteLater();
}

void PhyxCalculator::logicSmaller()
{
    if (variableStack.size() < 2)
    {
        raiseException(ProgramError);
        return;
    }
    PhyxVariable *variable2 = variableStack.pop();
    PhyxVariable *variable1 = variableStack.pop();

    PhyxVariable *variable3 = new PhyxVariable(this);
    variable3->setValue(variable1->value().real() < variable2->value().real());
    variableStack.push(variable3);

    variable1->deleteLater();
    variable2->deleteLater();
}

void PhyxCalculator::logicSmallerOrEqual()
{
    if (variableStack.size() < 2)
    {
        raiseException(ProgramError);
        return;
    }
    PhyxVariable *variable2 = variableStack.pop();
    PhyxVariable *variable1 = variableStack.pop();

    PhyxVariable *variable3 = new PhyxVariable(this);
    variable3->setValue(variable1->value().real() <= variable2->value().real());
    variableStack.push(variable3);

    variable1->deleteLater();
    variable2->deleteLater();
}

void PhyxCalculator::bitAnd()
{
    if (variableStack.size() < 2)
    {
        raiseException(ProgramError);
        return;
    }
    PhyxVariable *variable2 = variableStack.pop();
    PhyxVariable *variable1 = variableStack.pop();

    variable1->setValue(variable1->toInt() & variable2->toInt());
    variableStack.push(variable1);

    variable2->deleteLater();
}

void PhyxCalculator::bitOr()
{
    if (variableStack.size() < 2)
    {
        raiseException(ProgramError);
        return;
    }
    PhyxVariable *variable2 = variableStack.pop();
    PhyxVariable *variable1 = variableStack.pop();

    variable1->setValue(variable1->toInt() | variable2->toInt());
    variableStack.push(variable1);

    variable2->deleteLater();
}

void PhyxCalculator::bitXor()
{
    if (variableStack.size() < 2)
    {
        raiseException(ProgramError);
        return;
    }
    PhyxVariable *variable2 = variableStack.pop();
    PhyxVariable *variable1 = variableStack.pop();

    variable1->setValue(variable1->toInt() ^ variable2->toInt());
    variableStack.push(variable1);

    variable2->deleteLater();
}

void PhyxCalculator::bitInv()
{
    if (variableStack.size() < 1)
    {
        raiseException(ProgramError);
        return;
    }
    PhyxVariable *variable1 = variableStack.pop();

    variable1->setValue(~variable1->toInt());
    variableStack.push(variable1);
}

void PhyxCalculator::bitShiftLeft()
{
    if (variableStack.size() < 2)
    {
        raiseException(ProgramError);
        return;
    }
    PhyxVariable *variable2 = variableStack.pop();
    PhyxVariable *variable1 = variableStack.pop();

    variable1->setValue(variable1->toInt() << variable2->toInt());
    variableStack.push(variable1);

    variable2->deleteLater();
}

void PhyxCalculator::bitShiftRight()
{
    if (variableStack.size() < 2)
    {
        raiseException(ProgramError);
        return;
    }
    PhyxVariable *variable2 = variableStack.pop();
    PhyxVariable *variable1 = variableStack.pop();

    variable1->setValue(variable1->toInt() >> variable2->toInt());
    variableStack.push(variable1);

    variable2->deleteLater();
}

void PhyxCalculator::conditionIfElse()
{
    if (variableStack.size() < 3)
    {
        raiseException(ProgramError);
        return;
    }
    PhyxVariable *variable3 = variableStack.pop();
    PhyxVariable *variable2 = variableStack.pop();
    PhyxVariable *variable1 = variableStack.pop();

    if (variable1->toInt())
    {
        variableStack.push(variable2);
        variable3->deleteLater();
    }
    else
    {
        variableStack.push(variable3);
        variable2->deleteLater();
    }

    variable1->deleteLater();
}

void PhyxCalculator::unitCheckDimensionless()
{
    if (variableStack.size() < 1)
    {
        raiseException(ProgramError);
        return;
    }
    PhyxVariable *variable1 = variableStack.pop();

    if (!variable1->unit()->isDimensionlessUnit())
        raiseException(UnitNotDimensionlessError);
    else
        variable1->unit()->simplify();

    variableStack.push(variable1);
}

void PhyxCalculator::unitCheckDimensionless2()
{
    if (variableStack.size() < 2)
    {
        raiseException(ProgramError);
        return;
    }
    PhyxVariable *variable1 = variableStack.pop();
    PhyxVariable *variable2 = variableStack.pop();

    if (!variable1->unit()->isDimensionlessUnit() || !variable2->unit()->isDimensionlessUnit())
        raiseException(UnitNotDimensionlessError);
    else
    {
        variable1->unit()->simplify();
        variable2->unit()->simplify();
    }

    variableStack.push(variable2);
    variableStack.push(variable1);
}

void PhyxCalculator::unitCheckDimensionless2th()
{
    if (variableStack.size() < 2)
    {
        raiseException(ProgramError);
        return;
    }
    PhyxVariable *variable1 = variableStack.pop();
    PhyxVariable *variable2 = variableStack.pop();

    if (!variable2->unit()->isDimensionlessUnit())
        raiseException(UnitNotDimensionlessError);
    else
        variable2->unit()->simplify();

    variableStack.push(variable2);
    variableStack.push(variable1);
}

void PhyxCalculator::unitCheckDimensionless3th()
{
    if (variableStack.size() < 3)
    {
        raiseException(ProgramError);
        return;
    }
    PhyxVariable *variable1 = variableStack.pop();
    PhyxVariable *variable2 = variableStack.pop();
    PhyxVariable *variable3 = variableStack.pop();

    if (!variable3->unit()->isDimensionlessUnit())
        raiseException(UnitNotDimensionlessError);
    else
        variable3->unit()->simplify();

    variableStack.push(variable3);
    variableStack.push(variable2);
    variableStack.push(variable1);
}

void PhyxCalculator::unitCheckConvertible()
{
    if (variableStack.size() < 2)
    {
        raiseException(ProgramError);
        return;
    }
    PhyxVariable *variable2 = variableStack.pop();
    PhyxVariable *variable1 = variableStack.pop();

    if (!variable1->unit()->isConvertible(variable2->unit()))
        raiseException(UnitsNotConvertibleError);
    else
        variable1->unit()->compoundsEqualize(variable2->unit());

    variableStack.push(variable1);
    variableStack.push(variable2);
}

void PhyxCalculator::unitConvert()
{
    // conversion works -> bug with output, bug with unit symbol (not correct)
    if (variableStack.size() < 2)
    {
        raiseException(ProgramError);
        return;
    }
    PhyxVariable *variable2 = variableStack.pop();
    PhyxVariable *variable1 = variableStack.pop();

    variable1->setValue(variable1->value() / variable2->value());
    variable1->convertUnit(variable2->unit());
    variableStack.push(variable1);

    variable2->deleteLater();
}

void PhyxCalculator::unitMul()
{
    if (variableStack.size() < 2)
    {
        raiseException(ProgramError);
        return;
    }
    PhyxVariable *variable2 = variableStack.pop();
    PhyxVariable *variable1 = variableStack.pop();

    variable1->unit()->multiply(variable2->unit());
    variableStack.push(variable1);
    variableStack.push(variable2);
}

void PhyxCalculator::unitDiv()
{
    if (variableStack.size() < 2)
    {
        raiseException(ProgramError);
        return;
    }
    PhyxVariable *variable2 = variableStack.pop();
    PhyxVariable *variable1 = variableStack.pop();

    variable1->unit()->divide(variable2->unit());
    variableStack.push(variable1);
    variableStack.push(variable2);
}

void PhyxCalculator::unitPow()
{
    if (variableStack.size() < 2)
    {
        raiseException(ProgramError);
        return;
    }
    PhyxVariable *variable2 = variableStack.pop();
    PhyxVariable *variable1 = variableStack.pop();

    variable1->unit()->raise(variable2->value().real());
    variableStack.push(variable1);
    variableStack.push(variable2);
}

void PhyxCalculator::unitRoot()
{
    if (variableStack.size() < 2)
    {
        raiseException(ProgramError);
        return;
    }
    PhyxVariable *variable2 = variableStack.pop();
    PhyxVariable *variable1 = variableStack.pop();

    variable1->unit()->root(variable2->value().real());
    variableStack.push(variable1);
    variableStack.push(variable2);
}

void PhyxCalculator::unitSqrt()
{
    if (variableStack.size() < 1)
    {
        raiseException(ProgramError);
        return;
    }
    PhyxVariable *variable1 = variableStack.pop();

    variable1->unit()->root(2);
    variableStack.push(variable1);
}

void PhyxCalculator::unitClear()
{
    if (variableStack.size() < 1)
    {
        raiseException(ProgramError);
        return;
    }
    PhyxVariable *variable1 = variableStack.pop();

    variable1->setUnit(new PhyxCompoundUnit(this));
    variableStack.push(variable1);
}

void PhyxCalculator::unitAdd()
{
    if (variableStack.isEmpty())        // <- base unit
    {
        unitSystem->addBaseUnit(stringBuffer, 0, unitGroupBuffer, prefixBuffer);
    }
    else
    {
        PhyxVariable *variable1 = NULL;
        PhyxVariable *variable2 = NULL;
        if (variableStack.size() == 2)
        {
            variable2 = variableStack.pop();
            variable1 = variableStack.pop();
        }
        else
            variable1 = variableStack.pop();

        variable1->unit()->simplify();

        PhyxUnit *unit = new PhyxUnit(this);
        unit->setPowers(variable1->unit()->powers());
        unit->setUnitGroup(unitGroupBuffer);
        unit->setPreferedPrefix(prefixBuffer);
        unit->setSymbol(stringBuffer);
        unit->setFlags(0);
        unit->setScaleFactor(variable1->value().real());
        if (variable2 != NULL)
            unit->setOffset(variable2->value().real());

        unitSystem->addDerivedUnit(unit);

        variable1->deleteLater();
    }

    stringBuffer.clear();
    unitGroupBuffer.clear();
    prefixBuffer.clear();
    clearFlags();
}

void PhyxCalculator::unitRemove()
{

}

void PhyxCalculator::variableAdd()
{
    if (variableStack.size() < 1)
    {
        raiseException(ProgramError);
        return;
    }
    PhyxVariable *variable1 = variableStack.pop();

    variableManager->addVariable(stringBuffer, variable1);
    stringBuffer.clear();
}

void PhyxCalculator::variableRemove()
{
    variableManager->removeVariable(parameterBuffer);
}

void PhyxCalculator::variableLoad()
{
    variableStack.push(variableManager->getVariable(parameterBuffer));
    nameBuffer = parameterBuffer;
}

void PhyxCalculator::variablePreInc()
{
    valuePush1();
    valueAdd();
    valueCopy();

    stringBuffer = nameBuffer;
    variableAdd();
}

void PhyxCalculator::variablePreDec()
{
    valuePush1();
    valueSub();
    valueCopy();

    stringBuffer = nameBuffer;
    variableAdd();
}

void PhyxCalculator::variablePostInc()
{
    valueCopy();
    valuePush1();
    valueAdd();

    stringBuffer = nameBuffer;
    variableAdd();
}

void PhyxCalculator::variablePostDec()
{
    valueCopy();
    valuePush1();
    valueSub();

    stringBuffer = nameBuffer;
    variableAdd();
}

void PhyxCalculator::constantAdd()
{
    if (variableStack.size() < 1)
    {
        raiseException(ProgramError);
        return;
    }
    PhyxVariable *variable1 = variableStack.pop();

    variableManager->addConstant(stringBuffer, variable1);
    stringBuffer.clear();
}

void PhyxCalculator::constantRemove()
{
    variableManager->removeConstant(parameterBuffer);
}

void PhyxCalculator::constantLoad()
{
    variableStack.push(variableManager->getConstant(parameterBuffer));
    nameBuffer = parameterBuffer;
}

void PhyxCalculator::listAddStart()
{
    listModeActive = true;
    listModeType = ListAddType;
}

void PhyxCalculator::listSubStart()
{
    listModeActive = true;
    listModeType = ListSubType;
}

void PhyxCalculator::listMulStart()
{
    listModeActive = true;
    listModeType = ListMulType;
}

void PhyxCalculator::listDivStart()
{
    listModeActive = true;
    listModeType = ListDivType;
}

void PhyxCalculator::listModStart()
{
    listModeActive = true;
    listModeType = ListModType;
}

void PhyxCalculator::listPowStart()
{
    listModeActive = true;
    listModeType = ListPowType;
}

void PhyxCalculator::listBOrStart()
{
    listModeActive = true;
    listModeType = ListBOrType;
}

void PhyxCalculator::listBAndStart()
{
    listModeActive = true;
    listModeType = ListBAndType;
}

void PhyxCalculator::listBXorStart()
{
    listModeActive = true;
    listModeType = ListBXorType;
}

void PhyxCalculator::listXandStart()
{
    listModeActive = true;
    listModeType = ListXandType;
}

void PhyxCalculator::listAndStart()
{
    listModeActive = true;
    listModeType = ListAndType;
}

void PhyxCalculator::listNandStart()
{
    listModeActive = true;
    listModeType = ListNandType;
}

void PhyxCalculator::listXorStart()
{
    listModeActive = true;
    listModeType = ListXorType;
}

void PhyxCalculator::listOrStart()
{
    listModeActive = true;
    listModeType = ListOrType;
}

void PhyxCalculator::listNorStart()
{
    listModeActive = true;
    listModeType = ListNorType;
}

void PhyxCalculator::listValueSave()
{
    stringBuffer = tr("current result");
    variableAdd();
}

void PhyxCalculator::listValueLoad()
{
    parameterBuffer = tr("current result");
    variableLoad();
}

void PhyxCalculator::listValueSwap()
{
    if (variableStack.size() < 2)
    {
        raiseException(ProgramError);
        return;
    }
    PhyxVariable *variable1 = variableStack.pop();
    PhyxVariable *variable2 = variableStack.pop();
    variableStack.push(variable1);
    variableStack.push(variable2);
}

void PhyxCalculator::listEnd()
{
    listModeActive = false;
    variableManager->removeVariable(tr("current result"));
}

bool PhyxCalculator::executeFunction(QString expression, QStringList parameters, bool verifyOnly = false)
{
    QStringList newVariables;   //lists where all temporary variables are stores
    QList<PhyxVariable*> oldVariables;
    bool success;

    //psuh function parameters to stack
    for (int i = 0; i < parameters.size(); i++)
    {
        QString parameterName = parameters.at(i);

        //if variable with same name is defined, temporarily change value
        if (variableManager->containsVariable(parameterName))
        {
            newVariables.append(parameterName);
            oldVariables.append(variableManager->getVariable(parameterName));
        }
        variableManager->addVariable(parameterName, variableStack.pop());
    }

    //execute function
    QList<int> whiteSpaceList;
    m_expression = removeWhitespace(expression, &whiteSpaceList);
    success = /*this->setExpression(expression);*/ earleyParser->parseWord(expression);
    if (success)
    {
        if (!verifyOnly)
        {
            QList<EarleyTreeItem> earleyTree = earleyParser->getTree();                     //get a earley tree for the function
            success = this->evaluate(earleyTree, m_expression, whiteSpaceList);
        }
    }
    else
        raiseException(SyntaxError);

    //clear temporary variables
    for (int i = 0; i < parameters.size(); i++)
    {
        if (!newVariables.contains(parameters.at(i)))
            variableManager->removeVariable(parameters.at(i));
    }

    //rename temporary renamed variables back
    for (int i = 0; i < newVariables.count(); i++)
        variableManager->addVariable(newVariables.at(i), oldVariables[i]);

    return success;
}

void PhyxCalculator::functionAdd()
{
    int clipIndex = parameterBuffer.indexOf("(");
    int equalIndex = parameterBuffer.indexOf("=");
    QString functionName = parameterBuffer.left(clipIndex);
    QString functionExpression = parameterBuffer.mid(equalIndex+1);
    QStringList functionParameters;
    while (!functionParameterStack.isEmpty())
    {
        functionParameters.append(functionParameterStack.pop());
        valueBuffer = 1;
        pushVariable();     //for each parameter push 1 to stack for verifying the syntax of the expression
    }

    if (executeFunction(functionExpression, functionParameters, true))
        variableManager->addFunction(functionName, functionExpression, functionParameters);
}

void PhyxCalculator::functionRemove()
{
    int clipIndex = parameterBuffer.indexOf("(");
    QString functionName = parameterBuffer.left(clipIndex);
    variableManager->removeFunction(functionName);
}

void PhyxCalculator::functionRun()
{
    //int clipIndex = parameterBuffer.indexOf("(");
    QString functionName;//= parameterBuffer.left(clipIndex);
    QStringList functionList = functions();
    functionList.sort();

    for (int i = functionList.size()-1; i >= 0; i--)
    {
        if (parameterBuffer.indexOf(functionList.at(i)) == 0)
        {
            functionName = functionList.at(i);
            break;
        }
    }

    PhyxVariableManager::PhyxFunction *function = variableManager->getFunction(functionName);
    executeFunction(function->expression, function->parameters);
}

void PhyxCalculator::bufferUnit()
{
    //get the unit
    unitBuffer = parameterBuffer;
}

void PhyxCalculator::bufferValue()
{
    valueBuffer = stringToComplex(parameterBuffer);
}

void PhyxCalculator::bufferHex()
{
    valueBuffer = PhyxValueDataType(static_cast<PhyxFloatDataType>(hexToLongInt(parameterBuffer)),PHYX_FLOAT_NULL);
}

void PhyxCalculator::bufferOct()
{
    valueBuffer = PhyxValueDataType(static_cast<PhyxFloatDataType>(octToLongInt(parameterBuffer)),PHYX_FLOAT_NULL);
}

void PhyxCalculator::bufferBin()
{
    valueBuffer = PhyxValueDataType(static_cast<PhyxFloatDataType>(binToLongInt(parameterBuffer)),PHYX_FLOAT_NULL);
}

void PhyxCalculator::bufferHexString()
{
    if (variableStack.size() < 1)
    {
        raiseException(ProgramError);
        return;
    }
    PhyxVariable *variable1 = variableStack.pop();

    stringBuffer = longIntToHex(static_cast<PhyxIntegerDataType>(variable1->value().real()));
    variable1->deleteLater();
}

void PhyxCalculator::bufferOctString()
{
    if (variableStack.size() < 1)
    {
        raiseException(ProgramError);
        return;
    }
    PhyxVariable *variable1 = variableStack.pop();

    stringBuffer = longIntToOct(static_cast<PhyxIntegerDataType>(variable1->value().real()));
    variable1->deleteLater();
}

void PhyxCalculator::bufferBinString()
{
    if (variableStack.size() < 1)
    {
        raiseException(ProgramError);
        return;
    }
    PhyxVariable *variable1 = variableStack.pop();

    stringBuffer = longIntToBin(static_cast<PhyxIntegerDataType>(variable1->value().real()));
    variable1->deleteLater();
}

void PhyxCalculator::bufferPrefix()
{
    prefixBuffer = parameterBuffer;
}

void PhyxCalculator::bufferString()
{
    stringBuffer = parameterBuffer;
    stringBuffer.remove('"');

    if (stringBuffer.at(0).isNumber())
        raiseException(StringNotValidError);
}

void PhyxCalculator::bufferUnitGroup()
{
    unitGroupBuffer = parameterBuffer;
}

void PhyxCalculator::pushVariable()
{
    //create new variable
    PhyxVariable *variable = new PhyxVariable(this);
    variable->unit()->setUnitSystem(unitSystem);
    if (!unitBuffer.isEmpty())
        variable->setUnit(unitSystem->unit(unitBuffer));

    //handle units with prefered prefix (like kg)
    QString preferedPrefix;
    QString unitGroup;
    PhyxFloatDataType preferedPrefixValue = PHYX_FLOAT_ONE;
    if (!variable->unit()->isDimensionlessUnit())
    {
        preferedPrefix  = variable->unit()->compounds().first().unit->preferedPrefix();
        unitGroup       = variable->unit()->compounds().first().unit->unitGroup();
        if (!preferedPrefix.isEmpty())
            preferedPrefixValue = unitSystem->prefix(preferedPrefix, unitGroup).value;
    }

    if (!prefixBuffer.isEmpty())
    {
        PhyxUnitSystem::PhyxPrefix prefix = unitSystem->prefix(prefixBuffer, variable->unit()->unitGroup());
        if (!prefix.unitGroup.isEmpty())
        {
            PhyxValueDataType value = PhyxValueDataType(prefix.value, 0.0);

            value /= preferedPrefixValue;
            valueBuffer *= value;

            variable->setValue(valueBuffer);
        }
        else
        {
            raiseException(PrefixError);
        }
    }
    else
        variable->setValue(valueBuffer / PhyxValueDataType(preferedPrefixValue,0.0));

    //push it to the stack
    variableStack.push(variable);

    //cleanup
    valueBuffer = 1;
    unitBuffer.clear();
    prefixBuffer.clear();
}

void PhyxCalculator::pushFunctionParameter()
{
    functionParameterStack.push(stringBuffer);
}

void PhyxCalculator::setInputOnlyFlag()
{
    flagBuffer |= InputOnlyFlag;
}

void PhyxCalculator::outputVariable()
{
    if (stackLevel == 1)    //this function has only effect on lowest stack level
    {
        if (!variableStack.isEmpty())
        {
            if (!listModeActive)
            {
                if (variableStack.size() < 1)
                {
                    raiseException(ProgramError);
                    return;
                }
                PhyxVariable *variable1 = variableStack.pop();

                //set the special variable #
                variableManager->addVariable("#", variable1);
                //if (m_result != NULL)   // delete old result
                //    delete m_result;
                m_resultValue = variable1->value();
                m_resultUnit = variable1->unit()->symbol();
                m_result = variable1;

                emit outputResult();
            }
            else
            {
                listValueLoad();
                listValueSwap();
                if (listModeType == ListAddType)
                {
                    unitCheckConvertible();
                    if (m_error)
                        return;
                    valueAdd();
                }
                else if (listModeType == ListSubType)
                {
                    unitCheckConvertible();
                    if (m_error)
                        return;
                    valueSub();
                }
                else if (listModeType == ListMulType)
                {
                    unitMul();
                    valueMul();
                }
                else if (listModeType == ListDivType)
                {
                    unitDiv();
                    valueDiv();
                }
                else if (listModeType == ListModType)
                {
                    unitCheckDimensionless2();
                    if (m_error)
                        return;
                    valueCheckInteger2();
                    if (m_error)
                        return;
                    valueMod();
                }
                else if (listModeType == ListPowType)
                {
                    unitCheckDimensionless();
                    if (m_error)
                        return;
                    unitPow();
                    valuePow();
                }
                else
                {
                    unitCheckDimensionless2();
                    if (m_error)
                        return;
                    valueCheckInteger2();
                    if (m_error)
                        return;
                    if (listModeType == ListBAndType)
                        bitAnd();
                    else if (listModeType == ListBOrType)
                        bitOr();
                    else if (listModeType == ListBXorType)
                        bitXor();
                    else if (listModeType == ListXandType)
                        logicXand();
                    else if (listModeType == ListAndType)
                        logicAnd();
                    else if (listModeType == ListNandType)
                        logicNand();
                    else if (listModeType == ListXorType)
                        logicXor();
                    else if (listModeType == ListOrType)
                        logicOr();
                    else if (listModeType == ListNorType)
                        logicNor();
                }

                listValueSave();
            }
        }
    }
}

void PhyxCalculator::outputString()
{
    emit outputText(stringBuffer);
}

void PhyxCalculator::unitGroupAdd()
{
    unitSystem->addUnitGroup(stringBuffer);
    stringBuffer.clear();
}

void PhyxCalculator::unitGroupRemove()
{
    unitSystem->removeUnitGroup(unitGroupBuffer);
    unitGroupBuffer.clear();
}

void PhyxCalculator::prefixAdd()
{
    if (variableStack.size() < 1)
    {
        raiseException(ProgramError);
        return;
    }
    PhyxVariable *variable1 = variableStack.pop();
    unitSystem->addPrefix(stringBuffer, variable1->value().real(), unitGroupBuffer, (flagBuffer & InputOnlyFlag));

    unitGroupBuffer.clear();
    stringBuffer.clear();
    clearFlags();
    variable1->deleteLater();
}

void PhyxCalculator::prefixRemove()
{
    unitSystem->removePrefix(prefixBuffer);
    prefixBuffer.clear();
}

void PhyxCalculator::appendLowLevelOperation(QString variableName, PhyxCalculator::LowLevelOperationType type, PhyxVariable *variable)
{
    LowLevelOperation   *operation = new LowLevelOperation;
    LowLevelOperationList *operationList = new LowLevelOperationList;

    operation->type = type;
    operation->variableName = variableName;
    operation->variable = variable;
    operationList->append(operation);
    lowLevelStack.push(operationList);
}

void PhyxCalculator::runLowLevelOperation(PhyxCalculator::LowLevelOperation *operation)
{
    if (operation->type == AssignmentOperation)
    {
        variableManager->addVariable(operation->variableName, operation->variable);
    }
    else if (operation->type == AssignmentRemoveOperation)
    {
        variableManager->removeVariable(operation->variableName);
    }
    else if (operation->type == OutputOperation)
    {
        //set stacklevel temporary to 0 to make outputVariable work
        int tmpStackLevel = stackLevel;
        stackLevel = 1;

        variableStack.push(operation->variable);
        outputVariable();

        stackLevel = tmpStackLevel;
    }
    else if (operation->type == CombinedAssignmentOperationAdd)
    {
        variableStack.push(variableManager->getVariable(operation->variableName));
        variableStack.push(operation->variable);
        unitCheckConvertible();
        valueAdd();
        variableManager->addVariable(operation->variableName, variableStack.pop());
    }
    else if (operation->type == CombinedAssignmentOperationSub)
    {
        variableStack.push(variableManager->getVariable(operation->variableName));
        variableStack.push(operation->variable);
        unitCheckConvertible();
        valueSub();
        variableManager->addVariable(operation->variableName, variableStack.pop());
    }
    else if (operation->type == CombinedAssignmentOperationMul)
    {
        variableStack.push(variableManager->getVariable(operation->variableName));
        variableStack.push(operation->variable);
        unitMul();
        valueMul();
        variableManager->addVariable(operation->variableName, variableStack.pop());
    }
    else if (operation->type == CombinedAssignmentOperationDiv)
    {
        variableStack.push(variableManager->getVariable(operation->variableName));
        variableStack.push(operation->variable);
        unitDiv();
        valueDiv();
        variableManager->addVariable(operation->variableName, variableStack.pop());
    }
    else if (operation->type == CombinedAssignmentOperationMod)
    {
        variableStack.push(variableManager->getVariable(operation->variableName));
        variableStack.push(operation->variable);
        unitCheckDimensionless2();
        valueCheckInteger2();
        valueMod();
        variableManager->addVariable(operation->variableName, variableStack.pop());
    }
    else if (operation->type == CombinedAssignmentOperationAnd)
    {
        variableStack.push(variableManager->getVariable(operation->variableName));
        variableStack.push(operation->variable);
        unitCheckDimensionless2();
        valueCheckInteger2();
        bitAnd();
        variableManager->addVariable(operation->variableName, variableStack.pop());
    }
    else if (operation->type == CombinedAssignmentOperationOr)
    {
        variableStack.push(variableManager->getVariable(operation->variableName));
        variableStack.push(operation->variable);
        unitCheckDimensionless2();
        valueCheckInteger2();
        bitOr();
        variableManager->addVariable(operation->variableName, variableStack.pop());
    }
    else if (operation->type == CombinedAssignmentOperationXor)
    {
        variableStack.push(variableManager->getVariable(operation->variableName));
        variableStack.push(operation->variable);
        unitCheckDimensionless2();
        valueCheckInteger2();
        bitXor();
        variableManager->addVariable(operation->variableName, variableStack.pop());
    }
    else if (operation->type == CombinedAssignmentOperationShiftLeft)
    {
        variableStack.push(variableManager->getVariable(operation->variableName));
        variableStack.push(operation->variable);
        unitCheckDimensionless2();
        valueCheckInteger2();
        bitShiftLeft();
        variableManager->addVariable(operation->variableName, variableStack.pop());
    }
    else if (operation->type == CombinedAssignmentOperationShiftRight)
    {
        variableStack.push(variableManager->getVariable(operation->variableName));
        variableStack.push(operation->variable);
        unitCheckDimensionless2();
        valueCheckInteger2();
        bitShiftRight();
        variableManager->addVariable(operation->variableName, variableStack.pop());
    }
}

void PhyxCalculator::lowLevelAdd()
{
    LowLevelOperationList *operationList1 = lowLevelStack.pop();
    LowLevelOperationList *operationList2 = lowLevelStack.pop();

    operationList1->append(*operationList2);
    lowLevelStack.push(operationList1);

    delete operationList2;
}

void PhyxCalculator::lowLevelRun()
{
    if (!lowLevelStack.isEmpty())
    {
        LowLevelOperationList *operationList1 = lowLevelStack.pop();

        for (int i = operationList1->size()-1; i  >= 0; i--)
        {
            runLowLevelOperation(operationList1->at(i));
            delete operationList1->at(i);
        }

        delete operationList1;
    }
}

void PhyxCalculator::lowLevelAssignment()
{
    if (variableStack.size() < 1)
    {
        raiseException(ProgramError);
        return;
    }
    PhyxVariable *variable1 = variableStack.pop();

    appendLowLevelOperation(stringBuffer, AssignmentOperation, variable1);
    stringBuffer.clear();
}

void PhyxCalculator::lowLevelAssignmentRemove()
{
    appendLowLevelOperation(nameBuffer, AssignmentRemoveOperation, NULL);
    nameBuffer.clear();
}

void PhyxCalculator::lowLevelCombinedAssignmentAdd()
{
    if (variableStack.size() < 1)
    {
        raiseException(ProgramError);
        return;
    }
    PhyxVariable *variable1 = variableStack.pop();

    appendLowLevelOperation(nameBuffer, CombinedAssignmentOperationAdd, variable1);
    nameBuffer.clear();
}

void PhyxCalculator::lowLevelCombinedAssignmentSub()
{
    if (variableStack.size() < 1)
    {
        raiseException(ProgramError);
        return;
    }
    PhyxVariable *variable1 = variableStack.pop();

    appendLowLevelOperation(nameBuffer, CombinedAssignmentOperationSub, variable1);
    nameBuffer.clear();
}

void PhyxCalculator::lowLevelCombinedAssignmentMul()
{
    if (variableStack.size() < 1)
    {
        raiseException(ProgramError);
        return;
    }
    PhyxVariable *variable1 = variableStack.pop();

    appendLowLevelOperation(nameBuffer, CombinedAssignmentOperationMul, variable1);
    nameBuffer.clear();
}

void PhyxCalculator::lowLevelCombinedAssignmentDiv()
{
    if (variableStack.size() < 1)
    {
        raiseException(ProgramError);
        return;
    }
    PhyxVariable *variable1 = variableStack.pop();

    appendLowLevelOperation(nameBuffer, CombinedAssignmentOperationDiv, variable1);
    nameBuffer.clear();
}

void PhyxCalculator::lowLevelCombinedAssignmentMod()
{
    if (variableStack.size() < 1)
    {
        raiseException(ProgramError);
        return;
    }
    PhyxVariable *variable1 = variableStack.pop();

    appendLowLevelOperation(nameBuffer, CombinedAssignmentOperationMod, variable1);
    nameBuffer.clear();
}

void PhyxCalculator::lowLevelCombinedAssignmentAnd()
{
    if (variableStack.size() < 1)
    {
        raiseException(ProgramError);
        return;
    }
    PhyxVariable *variable1 = variableStack.pop();

    appendLowLevelOperation(nameBuffer, CombinedAssignmentOperationAnd, variable1);
    nameBuffer.clear();
}

void PhyxCalculator::lowLevelCombinedAssignmentOr()
{
    if (variableStack.size() < 1)
    {
        raiseException(ProgramError);
        return;
    }
    PhyxVariable *variable1 = variableStack.pop();

    appendLowLevelOperation(nameBuffer, CombinedAssignmentOperationOr, variable1);
    nameBuffer.clear();
}

void PhyxCalculator::lowLevelCombinedAssignmentXor()
{
    if (variableStack.size() < 1)
    {
        raiseException(ProgramError);
        return;
    }
    PhyxVariable *variable1 = variableStack.pop();

    appendLowLevelOperation(nameBuffer, CombinedAssignmentOperationXor, variable1);
    nameBuffer.clear();
}

void PhyxCalculator::lowLevelCombinedAssignmentShiftLeft()
{
    if (variableStack.size() < 1)
    {
        raiseException(ProgramError);
        return;
    }
    PhyxVariable *variable1 = variableStack.pop();

    appendLowLevelOperation(nameBuffer, CombinedAssignmentOperationShiftLeft, variable1);
    nameBuffer.clear();
}

void PhyxCalculator::lowLevelCombinedAssignmentShiftRight()
{
    if (variableStack.size() < 1)
    {
        raiseException(ProgramError);
        return;
    }
    PhyxVariable *variable1 = variableStack.pop();

    appendLowLevelOperation(nameBuffer, CombinedAssignmentOperationShiftRight, variable1);
    nameBuffer.clear();
}

void PhyxCalculator::lowLevelOutput()
{
    if (stackLevel <= 1)    //deactivate this function for stack levels > 1
    {
        if (variableStack.size() < 1)
        {
            raiseException(ProgramError);
            return;
        }
        PhyxVariable *variable1 = variableStack.pop();

        appendLowLevelOperation(QString(), OutputOperation, variable1);
    }
}
