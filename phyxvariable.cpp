#include "phyxvariable.h"

PhyxVariable::PhyxVariable(QObject *parent) :
    QObject(parent)
{
    m_value = 1;
    setUnit(new PhyxCompoundUnit());
}

PhyxVariable::~PhyxVariable()
{
    delete m_unit;
}

bool PhyxVariable::convertUnit(PhyxCompoundUnit *unit)
{
    if (!this->unit()->convertTo(unit))
    {
        m_error = PhyxVariable::UnitsNotConvertibleError;
        return false;
    }
    else
        return true;
}

QString PhyxVariable::errorString()
{
    switch (m_error)
    {
    case UnitsNotConvertibleError:      return tr("Units are not convertible");
    case UnitNotDimensionlessError:     return tr("Unit is not dimensionless");
    case ValueNotPositiveError:         return tr("value is not positive");
    case ValueNotIntegerError:          return tr("Value is not an integer");
    default:                            return tr("No error");
    }
}

void PhyxVariable::setUnit(PhyxUnit *unit)
{
    m_unit->fromSimpleUnit(unit);
}

void PhyxVariable::setPreferedUnit(userUnitBuffer userInputUnits, outputMode mode)
{
    PhyxUnitSystem* UnitSystem = m_unit->unitSystem();
    PhyxVariable newVar;

    switch (mode)
    {
    case onlyBaseUnits:
        break;

    case minimizeUnit:
        break;

    case forceInputUnits:
        break;
    }
}
