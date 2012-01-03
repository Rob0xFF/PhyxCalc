#ifndef PHYXCALCULATOR_H
#define PHYXCALCULATOR_H

#include <QObject>
#include <QStack>
#include <QDateTime>
#include <QDebug>
#include <QFile>
#include "cmath"
#include "qearleyparser.h"
#include "phyxunitmanager.h"
#include "phyxvariable.h"
#include "phyxunit.h"

class PhyxCalculator;

/** a si unit is defined as: [Q]=10^n*L^a*M^b*T^c*I^d*Theta^e*N^f*J^g */
typedef struct SiUnitStruct {
    int ten;    /// exponent of 10, si prefix
    int L;      /// dimension of L, length
    int M;      /// dimension of M, mass
    int T;      /// dimension of T, time
    int I;      /// dimension of I, electric current
    int Theta;  /// dimension of Theta, thermodynamic temperature
    int N;      /// dimension of N, amount of substance
    int J;      /// dimension of J, liminous intensity

    SiUnitStruct operator *(const SiUnitStruct &unit) const
    {
        SiUnitStruct result;
        result.ten = ten+ unit.ten;
        result.L   = L  + unit.L;
        result.M   = M  + unit.M;
        result.T   = T  + unit.T;
        result.I   = I  + unit.I;
        result.Theta = Theta + unit.Theta;
        result.N   = N  + unit.N;
        result.J   = J  + unit.J;
        return result;
    }
    SiUnitStruct operator /(const SiUnitStruct &unit) const
    {
        SiUnitStruct result;
        result.ten = ten- unit.ten;
        result.L   = L  - unit.L;
        result.M   = M  - unit.M;
        result.T   = T  - unit.T;
        result.I   = I  - unit.I;
        result.Theta = Theta - unit.Theta;
        result.N   = N  - unit.N;
        result.J   = J  - unit.J;
        return result;
    }
    SiUnitStruct pow(const int &x) const
    {
        SiUnitStruct result;
        result.ten = ten*x;
        result.L   = L  *x;
        result.M   = M  *x;
        result.T   = T  *x;
        result.I   = I  *x;
        result.Theta = Theta*x;
        result.N   = N  *x;
        result.J   = J  *x;
        return result;
    }
    SiUnitStruct root(const int &x) const
    {
        SiUnitStruct result;
        result.ten = ten/x;
        result.L   = L  /x;
        result.M   = M  /x;
        result.T   = T  /x;
        result.I   = I  /x;
        result.Theta = Theta/x;
        result.N   = N  /x;
        result.J   = J  /x;
        return result;
    }
    bool checkEqual(const SiUnitStruct &unit) const      //checks wheter 2 units are the same or not (prefix is not compared)
    {
        return ((L == unit.L)
                && (M == unit.M)
                && (T == unit.T)
                && (I == unit.I)
                && (Theta == unit.Theta)
                && (N == unit.N)
                && (J == unit.J));
    }
    int conversionFactor(const SiUnitStruct &unit) const //conversion factor between 2 units (eg.: kOhm -> Ohm, factor = 3 -> 10^3)
    {
        return ten - unit.ten;
    }
} SiUnit;

typedef long double         PhyxValueDataType;      /// the base data type for values
typedef SiUnit               PhyxUnitDataType;       /// the base data type for units

typedef struct PhysicalVariableStruct {
    PhyxValueDataType  value;
    SiUnit unit;
    /*bool operator ==(const PhysicalVariableStruct &item) const
    {
        return (value == item.value) && (unit == item.unit);
    }*/
    PhysicalVariableStruct operator +(const PhysicalVariableStruct &variable) const
    {
        PhysicalVariableStruct result;
        if (unit.checkEqual(variable.unit))
        {
            int conversionFactor = unit.conversionFactor(variable.unit);
            if (conversionFactor >= 0)
            {
                result.value = value * pow(10, conversionFactor);
                result.value += variable.value;
                result.unit = variable.unit;
            }
            else
            {
                result.value = variable.value * pow(10, -conversionFactor);
                result.value += value;
                result.unit  = unit;
            }
            return result;
        }
        else
            qDebug() << "units not equal";
    }
    PhysicalVariableStruct operator -(const PhysicalVariableStruct &variable) const
    {
        PhysicalVariableStruct result;
        if (unit.checkEqual(variable.unit))
        {
            int conversionFactor = unit.conversionFactor(variable.unit);
            if (conversionFactor >= 0)
            {
                result.value = value * pow(10, conversionFactor);
                result.value -= variable.value;
                result.unit = variable.unit;
            }
            else
            {
                result.value = variable.value * pow(10, -conversionFactor);
                result.value -= value;
                result.unit  = unit;
            }
            return result;
        }
        else
            qDebug() << "units not equal";
    }
    PhysicalVariableStruct operator *(const PhysicalVariableStruct &variable) const
    {
        PhysicalVariableStruct result;
        result.unit = unit * variable.unit;
        result.value = value * variable.value;
        return result;
    }
    PhysicalVariableStruct operator /(const PhysicalVariableStruct &variable) const
    {
        PhysicalVariableStruct result;
        result.unit = unit / variable.unit;
        result.value = value / variable.value;
        return result;
    }
} PhysicalVariable;

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
    QString                     numberBuffer;                                   /// string for buffering numbers
    QString                     parameterBuffer;                                /// string for buffering numbers
    QString                     stringBuffer;                                   /// string for buffering strings

    QHash<QString, PhyxRule>    phyxRules;                                      /// map of all rules, key is rule

    QEarleyParser               earleyParser;                                   /// the earley parser

    QString                     expression;                                     /// currently set expression
    bool                        expressionIsParsable;                           /// holds wheter currently set expression is parsable or not

    QMap<QString, PhysicalVariable> variableMap;                                /// variables mapped with their name
    QMap<QString, PhysicalVariable> constantMap;                                /// constants mapped with their name

    QHash<QString, void (PhyxCalculator::*)()> functionMap;                     /// functions mapped with their names

    void initialize();                                                                                          ///< initializes PhyxCalculator
    void loadGrammar(QString fileName);                                                                         ///< loads the grammar from a file

    void raiseException(QString exception);                                                                     ///< raises an exception
    void addRule(QString rule, QString functions = "");     ///< adds a rule

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
    void unitCheckEqual()   {PhyxUnitDataType unit = unitStack.pop();
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
    void unitSqrt()         {unitStack.push(unitStack.pop().root(2));}

    /** functions for number generation */
    void numberBuf()                        {numberBuffer.prepend(parameterBuffer);}
    void numberPush()                       {valueStack.push(parameterBuffer.toDouble());
                                             numberBuffer.clear();}

    /** functions for variable handling */
    void variableAdd();
    void variableRemove();
    void variablePush();
signals:
    
public slots:
    
};

#endif // PHYXCALCULATOR_H
