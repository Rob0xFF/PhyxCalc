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

#include "phyxcompoundunit.h"

PhyxCompoundUnit::PhyxCompoundUnit(QObject *parent) :
    PhyxUnit(parent)
{
}

PhyxCompoundUnit::PhyxCompoundUnit(PhyxUnit *unit)
{
    this->fromSimpleUnit(unit);
}

void PhyxCompoundUnit::copyCompoundUnit(PhyxCompoundUnit *source, PhyxCompoundUnit *destination)
{
    destination->setPowers(source->powers());
    destination->setUnitSystem(source->unitSystem());
    destination->setScaleFactor(source->scaleFactor());
    destination->setOffset(source->offset());
    destination->setCompounds(source->compounds());
}

bool PhyxCompoundUnit::isSame(PhyxCompoundUnit *unit)
{
    return (this->powersCompare(unit->powers()) && (m_compounds == unit->compounds()));
}

bool PhyxCompoundUnit::isConvertible(PhyxCompoundUnit *unit)
{
    return this->powersCompare(unit->powers());
}

bool PhyxCompoundUnit::isOne()
{
    return (compoundsNonNullCount() == 0);
}

bool PhyxCompoundUnit::isSimpleUnit()
{
    if (compoundsNonNullCount() == 1)   //unit has only 1 compound
    {
        for (int i = 0; i < m_compounds.size(); i++)
        {
            if (m_compounds.at(i).power < 0)
                return false;
        }
        return true;    //if function has not returned yet, unit has no negative compound and is simplebas

    }
    return false;
}

void PhyxCompoundUnit::compoundAppend(PhyxUnit *unit, double power)
{
    PhyxUnit *newUnit = new PhyxUnit();
    PhyxUnit::copyUnit(unit, newUnit);

    m_compounds.append(PhyxCompound());
    m_compounds.last().unit = newUnit;
    m_compounds.last().power = power;

    if (newUnit->offset() != 0)                 // simplify offset units
        compoundSimplify(m_compounds.size()-1);
}

void PhyxCompoundUnit::compoundSimplify(int index)
{
    PhyxUnit *unit = m_compounds.at(index).unit;
    double power = m_compounds.at(index).power;

    emit scaleValue(pow(unit->scaleFactor(), power));
    emit offsetValue(-unit->offset());
    unit->setOffset(0);
    unit->setScaleFactor(1);

    if (unit->isOne())
    {
        delete m_compounds.at(index).unit;
        m_compounds.removeAt(index);
    }
    else
    {
        m_unitSystem->verifyUnit(unit);
    }
}

void PhyxCompoundUnit::compoundMultiply(PhyxUnit *unit, double factor)
{
    for (int i = 0; i < m_compounds.size(); i++)
    {
        if (m_compounds.at(i).unit->isSame(unit))   //if compounds contain unit
        {
            m_compounds[i].power += factor;      // multiply the with the factor
            /*if (m_compounds.at(i).power == 0)       // if new power = 0
            {
                delete m_compounds.at(i).unit;      //remove the compound
                m_compounds.removeAt(i);
            }*/
            return;
        }
    }

    compoundAppend(unit, factor);                    // if compounds not contain unit, add unit
}

void PhyxCompoundUnit::compoundDivide(PhyxUnit *unit, double factor)
{
    compoundMultiply(unit, -factor);               //divide is same as multiply
}

void PhyxCompoundUnit::compoundEqualize(int unitIndex, PhyxCompoundUnit *parentUnit)
{
    for (int i = 0; i < parentUnit->compounds().size(); i++)
    {
        PhyxUnit *tmpUnit = parentUnit->compounds().at(i).unit;
        PhyxUnit *unit = m_compounds.at(unitIndex).unit;
        if (tmpUnit->isConvertible(unit))
        {
            if (!tmpUnit->isSame(unit))
            {
                this->compoundSimplify(unitIndex);
                parentUnit->compoundSimplify(i);
            }
        }
    }
}

void PhyxCompoundUnit::compoundsMultiply(PhyxCompoundList compounds)
{
    for (int i = 0; i < compounds.size(); i++)
        compoundMultiply(compounds.at(i).unit, compounds.at(i).power);
}

void PhyxCompoundUnit::compoundsDivide(PhyxCompoundList compounds)
{
    for (int i = 0; i < compounds.size(); i++)
        compoundDivide(compounds.at(i).unit, compounds.at(i).power);
}

void PhyxCompoundUnit::compoundsEqualize(PhyxCompoundUnit *unit)
{
    for (int i = 0; i < m_compounds.size(); i++)
        compoundEqualize(i, unit);
}

void PhyxCompoundUnit::compoundsRaise(double power)
{
    for (int i = 0; i < m_compounds.size(); i++)
        m_compounds[i].power *= power;
}

void PhyxCompoundUnit::compoundsRoot(double root)
{
    for (int i = 0; i < m_compounds.size(); i++)
        m_compounds[i].power /= root;
}

void PhyxCompoundUnit::compoundsClear()
{
    for (int i = (m_compounds.size()-1); i >= 0; i--)
    {
        delete m_compounds.at(i).unit;
        m_compounds.removeAt(i);
    }
}

void PhyxCompoundUnit::compoundsSetNull()
{
    for (int i = 0; i < m_compounds.size(); i++)
        m_compounds[i].power = 0;
}

int PhyxCompoundUnit::compoundsNonNullCount()
{
    int count = 0;
    for (int i = 0; i < m_compounds.size(); i++)
    {
        if (m_compounds.at(i).power != 0)
            count++;
    }

    return count;
}


void PhyxCompoundUnit::verify()
{
    if (m_unitSystem != NULL)
    {
        if (!(this->isSimpleUnit() || this->isOne()))
        {
            PhyxUnit *unit = new PhyxUnit();
            unit->setPowers(this->powers());
            if (m_unitSystem->verifyUnit(unit))
            {
                compoundsSetNull();
                compoundAppend(unit, 1);
            }
            else
                delete unit;
        }
    }
}

bool PhyxCompoundUnit::add(PhyxCompoundUnit *unit)
{
    if (this->isSame(unit))
    {
        return true;
    }
    else if (this->isConvertible(unit))
    {
        //make units the same
        this->compoundsEqualize(unit);

        return true;
    }
    return false;
}

bool PhyxCompoundUnit::sub(PhyxCompoundUnit *unit)
{
    //same function as add
    return add(unit);
}

void PhyxCompoundUnit::multiply(PhyxCompoundUnit *unit)
{
    this->compoundsEqualize(unit);

    if (this->isDimensionlessUnit())
        this->simplify();
    if (unit->isDimensionlessUnit())
        unit->simplify();

    this->powersMultiply(unit->powers());
    this->compoundsMultiply(unit->compounds());

    verify();
}

void PhyxCompoundUnit::divide(PhyxCompoundUnit *unit)
{
    this->compoundsEqualize(unit);

    if (this->isDimensionlessUnit())
        this->simplify();
    if (unit->isDimensionlessUnit())
        unit->simplify();

    this->powersDivide(unit->powers());
    this->compoundsDivide(unit->compounds());

    verify();
}

void PhyxCompoundUnit::raise(double power)
{
    this->powersRaise(power);
    this->compoundsRaise(power);

    verify();
}

void PhyxCompoundUnit::root(double root)
{
    this->powersRoot(root);
    this->compoundsRoot(root);

    verify();
}

bool PhyxCompoundUnit::convertTo(PhyxCompoundUnit *unit)
{
    this->simplify();

    for (int i = 0; i < unit->compounds().size(); i++)
    {
        // make the inverse galilean transformation x = (y+b)/a
        offsetValue(unit->compounds().at(i).unit->offset());
        scaleValue(pow(unit->compounds().at(i).unit->scaleFactor(), -unit->compounds().at(i).power));
    }

    this->compoundsClear();
    this->compoundsMultiply(unit->compounds());

    return true;
}

void PhyxCompoundUnit::fromSimpleUnit(PhyxUnit *unit)
{
    compoundsClear();
    compoundAppend(unit,1);
    setPowers(unit->powers());
}

void PhyxCompoundUnit::simplify()
{
    for (int i = (m_compounds.size() - 1); i >= 0; i--)
        compoundSimplify(i);
}

const QString PhyxCompoundUnit::symbol()
{
    if (m_unitSystem != NULL)
    {
        if (this->isOne())
        {
            return "";
        }

        //build a nice unit symbol
        int positiveCompoundsCount = 0;
        int negativeCompoundsCount = 0;
        QString positiveCompounds;
        QString negativeCompounds;
        QString compoundsString;

        for (int i = 0; i < m_compounds.size(); i++)
        {
            PhyxUnit *unit = m_compounds.at(i).unit;
            double  power = m_compounds.at(i).power;

            if (power > 0)
            {
                if (!positiveCompoundsCount == 0)
                    positiveCompounds.append("*");

                positiveCompounds.append(unit->preferedPrefix());
                positiveCompounds.append(unit->symbol());

                if (power != 1)
                    positiveCompounds.append(QString("%1").arg(power));

                positiveCompoundsCount++;
            }
            else if (power < 0)
            {
                if (!negativeCompoundsCount == 0)
                    negativeCompounds.append("*");

                negativeCompounds.append(unit->preferedPrefix());
                negativeCompounds.append(unit->symbol());

                if (power != -1)
                    negativeCompounds.append(QString("%1").arg(-power));

                negativeCompoundsCount++;
            }
        }

        if (positiveCompoundsCount == 0)
            positiveCompounds.append("1");

        if ((positiveCompoundsCount > 1) && (negativeCompoundsCount > 0))
        {
            positiveCompounds.prepend("(");
            positiveCompounds.append(")");
        }

        if (negativeCompoundsCount > 1)
        {
            negativeCompounds.prepend("(");
            negativeCompounds.append(")");
        }

        compoundsString.append(positiveCompounds);

        if (negativeCompoundsCount > 0)
        {
            compoundsString.append("/");
            compoundsString.append(negativeCompounds);
        }

        // unicodify string
        compoundsString.replace(QChar('*'), QChar(0x22C5)); //mathematic dot operator
        compoundsString.replace(QChar('0'), QChar(0x2070));
        compoundsString.replace(QChar('1'), QChar(0x00B9));
        compoundsString.replace(QChar('2'), QChar(0x00B2));
        compoundsString.replace(QChar('3'), QChar(0x00B3));
        compoundsString.replace(QChar('4'), QChar(0x2074));
        compoundsString.replace(QChar('5'), QChar(0x2075));
        compoundsString.replace(QChar('6'), QChar(0x2076));
        compoundsString.replace(QChar('7'), QChar(0x2077));
        compoundsString.replace(QChar('8'), QChar(0x2078));
        compoundsString.replace(QChar('9'), QChar(0x2079));
        compoundsString.replace(QChar('-'), QChar(0x207B));

        return compoundsString;
    }
    return QString();
}

const QString PhyxCompoundUnit::preferedPrefix()
{
    if (this->isSimpleUnit())
    {
        for (int i = 0; i < m_compounds.size(); i++)
        {
            if (m_compounds.at(i).power != 0)
                return m_compounds.at(i).unit->preferedPrefix();
        }
    }

    return QString();
}

const QString PhyxCompoundUnit::unitGroup()
{
    if (this->isSimpleUnit())
    {
        for (int i = 0; i < m_compounds.size(); i++)
        {
            if (m_compounds.at(i).power != 0)
                return m_compounds.at(i).unit->unitGroup();
        }
    }

    return QString();
}
