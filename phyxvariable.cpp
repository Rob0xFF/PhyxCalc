#include "phyxvariable.h"

PhyxVariable::PhyxVariable(QObject *parent) :
    QObject(parent)
{
    m_value = 0;
    m_unit = new PhyxUnit();
}

PhyxVariable::~PhyxVariable()
{
    delete m_unit;
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

bool PhyxVariable::multiply(PhyxVariable *variable)
{
    if ((!this->unit()->isDimensionlessUnit()) && (!variable->unit()->isDimensionlessUnit()))
    {
        this->simplifyUnit();
        variable->simplifyUnit();
    }
    else
    {
        if (this->unit()->isDimensionlessUnit())
            this->simplifyUnit();
        if (variable->unit()->isDimensionlessUnit())
            variable->simplifyUnit();
    }

    this->m_value *= variable->value();
    this->unit()->powersMultiply(variable->unit()->powers());

    return true;
}

bool PhyxVariable::devide(PhyxVariable *variable)
{
    if ((!this->unit()->isDimensionlessUnit()) && (!variable->unit()->isDimensionlessUnit()))
    {
        this->simplifyUnit();
        variable->simplifyUnit();
    }
    else
    {
        if (this->unit()->isDimensionlessUnit())
            this->simplifyUnit();
        if (variable->unit()->isDimensionlessUnit())
            variable->simplifyUnit();
    }

    this->m_value /= variable->value();
    this->unit()->powersDevide(variable->unit()->powers());

    return true;
}

bool PhyxVariable::raise(double power)
{
    this->simplifyUnit();

    this->m_value = pow(this->m_value, power);
    this->unit()->powersRaise(power);

    return true;
}

bool PhyxVariable::root(double root)
{
    this->simplifyUnit();

    this->m_value = pow(this->m_value, 1.0/root);
    this->unit()->powersRoot(root);

    return true;
}

void PhyxVariable::simplifyUnit()
{
    //make galilean the transformation y = ax + b
    m_value *= m_unit->scaleFactor();
    m_value -= m_unit->offset();
    m_unit->setOffset(0);
    m_unit->setScaleFactor(1);
}
