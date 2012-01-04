#ifndef PHYXVARIABLE_H
#define PHYXVARIABLE_H

#include <QObject>
#include <cmath>
#include "phyxunit.h"

typedef long double         PhyxValueDataType;      /// the base data type for values

class PhyxVariable : public QObject
{
    Q_OBJECT
    Q_PROPERTY(PhyxValueDataType value READ value WRITE setValue)
    Q_PROPERTY(PhyxUnit *unit READ unit WRITE setUnit)
    Q_PROPERTY(VariableError error READ error)
    Q_ENUMS(VariableError)

public:
    explicit PhyxVariable(QObject *parent = 0);
    ~PhyxVariable();

    enum VariableError {
        UnitsNotConvertibleError,
        UnitNotDimensionlessError
    };

    bool add(PhyxVariable *variable);
    bool sub(PhyxVariable *variable);
    bool multiply(PhyxVariable *variable);
    bool devide(PhyxVariable *variable);
    bool raise(double power);
    bool root(double root);

    void simplifyUnit();                            ///< simplifies the unit of the variable (e.g.: GalileanUnit -> ProductUnit, DimensionlessUnit -> NoUnit)

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

private:
    
    PhyxValueDataType m_value;
    PhyxUnit *m_unit;

    VariableError m_error;

signals:
    
public slots:

void setValue(PhyxValueDataType arg)
{
    m_value = arg;
}
void setUnit(PhyxUnit * arg)
{
    m_unit = arg;
}
};

#endif // PHYXVARIABLE_H
