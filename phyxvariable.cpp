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
    return m_unit->convertTo(unit);
}

void PhyxVariable::copyVariable(PhyxVariable *source, PhyxVariable *destination)
{
    PhyxCompoundUnit *unit = new PhyxCompoundUnit();
    PhyxCompoundUnit::copyCompoundUnit(source->unit(), unit);
    destination->setUnit(unit);
    destination->setValue(source->value());
}

void PhyxVariable::setUnit(PhyxUnit *unit)
{
    m_unit->fromSimpleUnit(unit);
}
