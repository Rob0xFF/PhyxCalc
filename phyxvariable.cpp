#include "phyxvariable.h"

PhyxVariable::PhyxVariable(QObject *parent) :
    QObject(parent)
{
    m_value = 0;
    m_unit = new PhyxUnit();
}

bool PhyxVariable::add(PhyxVariable *variable)
{
    if (this->unit()->isSame(variable->unit()))
    {
        this->m_value += variable->value();
        return true;
    }
    else if (this->unit()->isConvertible(variable->unit()))
    {
        this->simplifyUnit();
        variable->simplifyUnit();

        this->m_value += variable->value();
        return true;
    }
    else
    {
        this->m_error = UnitsNotConvertibleError;
        return false;
    }
}

bool PhyxVariable::sub(PhyxVariable *variable)
{
    if (this->unit()->isSame(variable->unit()))
    {
        this->m_value -= variable->value();
        return true;
    }
    else if (this->unit()->isConvertible(variable->unit()))
    {
        this->simplifyUnit();
        variable->simplifyUnit();

        this->m_value -= variable->value();
        return true;
    }
    else
    {
        this->m_error = UnitsNotConvertibleError;
        return false;
    }
}

void PhyxVariable::simplifyUnit()
{
    //make galilean the transformation y = ax + b
    m_value *= m_unit->scaleFactor();
    m_value -= m_unit->offset();
    m_unit->setOffset(0);
    m_unit->setScaleFactor(0);
}
