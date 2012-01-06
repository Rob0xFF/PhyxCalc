#ifndef PHYXVARIABLE_H
#define PHYXVARIABLE_H

#include <QObject>
#include <cmath>
#include "phyxunit.h"
#include "phyxunitmanager.h"

class PhyxUnitManager;

typedef long double         PhyxValueDataType;      /// the base data type for values

class PhyxVariable : public QObject
{
    Q_OBJECT
    Q_PROPERTY(PhyxValueDataType value READ value WRITE setValue)
    Q_PROPERTY(PhyxUnit *unit READ unit WRITE setUnit)
    Q_PROPERTY(VariableError error READ error)
    Q_PROPERTY(PhyxUnitManager *unitManager READ unitManager WRITE setUnitManager)
    Q_PROPERTY(PhyxUnit::PowerMap compounds READ compounds)
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
    bool convertUnit(PhyxUnit *unit);               ///< converts the variable to the given unit, returns successful

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
    PhyxUnit * unit() const
    {
        return m_unit;
    }
    VariableError error() const
    {
        return m_error;
    }
    PhyxUnitManager * unitManager() const
    {
        return m_unitManager;
    }
    PhyxUnit::PowerMap compounds() const
    {
        return m_compounds;
    }

private:
    PhyxValueDataType   m_value;
    PhyxUnit            *m_unit;
    VariableError       m_error;
    PhyxUnitManager     *m_unitManager;

    PhyxUnit::PowerMap  m_compounds;    /// holds all compounds of a unit: e.g.: m/s -> compuound 1: m^1, compound 2: s^-1
    void appendCompound(QString base, double power);       /// adds a power to the map
    void compoundMultiply(QString base, double factor);    /// multiplies a power with factor
    void compoundDevide(QString base, double factor);      /// devides a power with factor
    void compoundsMultiply(PhyxUnit::PowerMap powers);               /// multiplies powers of the unit with other powers
    void compoundsDivide(PhyxUnit::PowerMap powers);                 /// devides powers of the unit with other powers
    void compoundsRaise(double power);                     /// raises all powers to power
    void compoundsRoot(double root);                       /// takes the root of all powers


    void updateUnitSymbol();            ///< fills the unit symbol with the correct information

signals:
    
public slots:

void setValue(PhyxValueDataType arg)
{
    m_value = arg;
}
void setUnit(PhyxUnit * arg)
{
    m_unit = arg;
    m_compounds.clear();
    appendCompound(m_unit->symbol(),1);
}
void setUnitManager(PhyxUnitManager * arg)
{
    m_unitManager = arg;
}
};

#endif // PHYXVARIABLE_H
