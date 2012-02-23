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

bool PhyxVariable::isComplex()
{
    return (m_value.imag() != 0.0L);
}

bool PhyxVariable::isPositive()
{
    return (m_value.real() >= 0.0L);
}

bool PhyxVariable::isInteger()
{
    return (!this->isComplex() && (static_cast<long double>(this->toInt()) == m_value.real()));
}

long int PhyxVariable::toInt()
{
    return static_cast<long int>(m_value.real());
}
