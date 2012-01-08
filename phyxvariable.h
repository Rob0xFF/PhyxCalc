#ifndef PHYXVARIABLE_H
#define PHYXVARIABLE_H

#include <QObject>
#include <cmath>
#include "phyxcompoundunit.h"

typedef long double         PhyxValueDataType;      /// the base data type for values

class PhyxVariable : public QObject
{
    Q_OBJECT
    Q_PROPERTY(PhyxValueDataType value READ value WRITE setValue)
    Q_PROPERTY(PhyxCompoundUnit *unit READ unit WRITE setUnit)
    Q_PROPERTY(VariableError error READ error)
    Q_ENUMS(VariableError)

public:
    explicit PhyxVariable(QObject *parent = 0);
    ~PhyxVariable();

    enum VariableError {
        UnitsNotConvertibleError,
        UnitNotDimensionlessError,
        ValueNotPositiveError,
        ValueNotIntegerError
    };

    void simplifyUnit();                            ///< simplifies the unit of the variable (e.g.: GalileanUnit -> ProductUnit, DimensionlessUnit -> NoUnit)
    bool convertUnit(PhyxCompoundUnit *unit);

    /** mathematical functions of the variable */
    bool mathAdd(PhyxVariable *variable);
    bool mathSub(PhyxVariable *variable);
    bool mathMul(PhyxVariable *variable);
    bool mathDiv(PhyxVariable *variable);
    void mathNeg();
    bool mathRaise(PhyxVariable *variable);
    bool mathRoot(PhyxVariable *variable);
    void mathSqrt();
    bool mathSin();
    bool mathArcsin();
    bool mathCos();
    bool mathArccos();
    bool mathTan();
    bool mathArctan();
    bool mathSinh();
    bool mathArcsinh();
    bool mathCosh();
    bool mathArccosh();
    bool mathTanh();
    bool mathArctanh();
    bool mathExp();
    bool mathLn();
    bool mathLog10();
    bool mathLog2();
    bool mathLogn(PhyxVariable *variable);
    void mathAbs();
    bool mathMax(PhyxVariable *variable);
    bool mathMin(PhyxVariable *variable);
    void mathInt();
    void mathTrunc();
    void mathFloor();
    void mathRound();
    void mathCeil();
    void mathSign();
    bool mathHeaviside();
    bool mathRandg(PhyxVariable *variable);
    bool mathFaculty();

    PhyxValueDataType value() const
    {
        return m_value;
    }
    PhyxCompoundUnit * unit() const
    {
        return m_unit;
    }
    VariableError error() const
    {
        return m_error;
    }

private:
    PhyxValueDataType   m_value;
    PhyxCompoundUnit    *m_unit;
    VariableError       m_error;

signals:
    
public slots:

void setValue(PhyxValueDataType arg)
{
    m_value = arg;
}
void setUnit(PhyxCompoundUnit * arg)
{
    m_unit = arg;
}
void offsetValue(double offset)
{
    m_value += offset;
}
void scaleValue(double scaleFactor)
{
    m_value *= scaleFactor;
}
};

#endif // PHYXVARIABLE_H
