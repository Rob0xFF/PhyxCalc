#ifndef PHYXCALCULATOR_H
#define PHYXCALCULATOR_H

#define _USE_MATH_DEFINES

#include <QObject>
#include <QStack>
#include <QDateTime>
#include <QDebug>
#include <QFile>
#include <sstream>
#include "boost/math/special_functions.hpp"
#include "boost/math/complex.hpp"
#include "qearleyparser.h"
#include "phyxunitsystem.h"
#include "phyxvariable.h"
#include "phyxvariablemanager.h"
#include "phyxtesting.h"

typedef std::complex<long double>   PhyxValueDataType;      /// the base data type for values
typedef int                         PhyxUnitDataType;       /// the base data type for units

typedef struct {
    QStringList functions;                          /// a list of functions to call
} PhyxRule;

class PhyxCalculator : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString expression READ expression WRITE setExpression)
    Q_PROPERTY(bool error READ hasError)
    Q_PROPERTY(int errorNumber READ errorNumber)
    Q_PROPERTY(int errorPosition READ errorPosition)
    Q_PROPERTY(PhyxValueDataType resultValue READ resultValue)
    Q_PROPERTY(QString resultUnit READ resultUnit)
    Q_PROPERTY(PhyxVariable * result READ result)

public:
    explicit PhyxCalculator(QObject *parent = 0);

    enum CalculationError {
        SyntaxError,
        UnitsNotConvertibleError,
        UnitNotDimensionlessError,
        ValueNotPositiveError,
        ValueNotIntegerError,
        ValueComplexError,
        StringNotValidError,
        PrefixError
    };

    enum OutputMode {
        OnlyBaseUnitsOutputMode   = 0x03,         /// option to convert output units into base units
        MinimizeUnitOutputMode    = 0x01,         /// option to use shortest possible output unit, but use input units in case of ambiguity
        ForceInputUnitsOutputMode = 0x02          /// option to use only input units as output unit, as far as possible
    };

    enum PrefixMode {
        UsePrefix   = 0x01,
        UseNoPrefix = 0x02
    };

    enum InputOutputFlags {
        InputOnlyFlag   = 0x01                  /// option used for units and prefix which should be used for input only (like da or PS)
    };

    typedef struct {
        QString value;
        QString unit;
    } ResultVariable;   /// this struct is for outputting the formated variable

    bool setExpression (QString m_expression);          ///< sets the expression, checks what must be parsed and returns wheter the expression is parsable or not
    bool evaluate();                                    ///< evaluates the expression
    void loadFile(QString fileName);                    ///< parses a complete txt file

    PhyxVariableManager::PhyxVariableMap * variables() const;
    PhyxVariableManager::PhyxVariableMap * constants() const;
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
    int errorPosition() const
    {
        return m_errorPosition;
    }
    PhyxVariable * result() const
    {
        return m_result;
    }

    static QString complexToString(const PhyxValueDataType number, int precision, char numberFormat);
    static PhyxValueDataType stringToComplex(QString string);

    ResultVariable formatVariable(PhyxVariable *variable, OutputMode outputMode, PrefixMode prefixMode, int precision, char numberFormat) const;

private:
    QStack<PhyxVariable*>       variableStack;                                  /// stack for variable calculation
    QString                     parameterBuffer;                                /// string for buffering numbers
    QString                     stringBuffer;                                   /// string for buffering strings
    PhyxValueDataType           valueBuffer;
    QString                     prefixBuffer;
    QString                     unitBuffer;
    QString                     unitGroupBuffer;
    int                         flagBuffer;                                     /// this buffer holds current set flags (e.g. the inputOnly flag

    QHash<QString, PhyxRule>    phyxRules;                                      /// map of all rules, key is rule

    QEarleyParser               *earleyParser;                                  /// the earley parser
    PhyxUnitSystem              *unitSystem;                                    /// the unit system
    PhyxVariableManager         *variableManager;                               /// the variable manager

    QString                     m_expression;                                   /// currently set expression
    bool                        expressionIsParsable;                           /// holds wheter currently set expression is parsable or not
    PhyxValueDataType           m_resultValue;                                  /// value of the result
    QString                     m_resultUnit;                                   /// unit symbol of the result
    PhyxVariable                *m_result;                                      /// result as variable
    bool                        m_error;                                        /// holds wheter an error occured or not
    int                         m_errorNumber;                                  /// current error number
    int                         m_errorPosition;                                /// position where the error occured


    QHash<QString, void (PhyxCalculator::*)()> functionMap;                     /// functions mapped with their names

    void initialize();                                                                                          ///< initializes PhyxCalculator
    void loadGrammar(QString fileName);                                                                         ///< loads the grammar from a file

    void raiseException(int errorNumber);                                                                     ///< raises an exception
    void addRule(QString rule, QString functions = "");     ///< adds a rule

    PhyxUnitSystem::PhyxPrefix getBestPrefx(PhyxValueDataType value, QString unitGroup, QString preferedPrefix) const;     /// gets the best fitting prefix

    void clearStack();
    void clearResult();

    void clearFlags();

    /** functions for value calculation */
    void valueCheckComplex();
    void valueCheckComplex2();
    void valueCheckPositive();
    void valueCheckInteger();
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
    void valueAvg();
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

    void complexArg();
    void complexNorm();
    void complexConj();
    void complexPolar();

    void logicAnd();
    void logicOr();
    void logicNand();
    void logicNor();
    void logicXand();
    void logicXor();
    void logicNot();
    void logicEqual();
    void logicNotEqual();
    void logicGreater();
    void logicGreaterOrEqual();
    void logicSmaller();
    void logicSmallerOrEqual();

    void bitAnd();
    void bitOr();
    void bitXor();
    void bitInv();

    /** functions for unit calculation */
    void unitCheckDimensionless();
    void unitCheckDimensionless2();
    void unitCheckConvertible();
    void unitConvert();
    void unitMul();
    void unitDiv();
    void unitPow();
    void unitPow2();
    void unitPow3();
    void unitRoot();
    void unitSqrt();
    void unitAdd();
    void unitRemove();

    /** functions for variable handling */
    void variableAdd();
    void variableRemove();
    void variableLoad();
    void constantAdd();
    void constantRemove();
    void constantLoad();

    /** functions for buffering */
    void bufferUnit();
    void bufferValue();
    void bufferPrefix();
    void bufferString();
    void bufferUnitGroup();
    void pushVariable();

    void setInputOnlyFlag();

    void outputVariable();
    void unitGroupAdd();
    void unitGroupRemove();
    void prefixAdd();
    void prefixRemove();

signals:
    void variablesChanged();        ///< is emited when variables have changed
    void constantsChanged();        ///< is emited wehen constants have changed
    void outputResult();            ///< is emited when result should be output
    void outputError();             ///< is emited when an error should be output
    
public slots:
    void clearVariables();

private slots:
    void addUnitRule(QString symbol);
    void removeUnitRule(QString symbol);
    void addVariableRule(QString name);
    void removeVariableRule(QString name);
    void addConstantRule(QString name);
    void removeConstantRule(QString name);
    void addPrefixRule(QString symbol);
    void removePrefixRule(QString symbol);
    void addUnitGroupRule(QString name);
    void removeUnitGroupRule(QString name);
};

#endif // PHYXCALCULATOR_H
