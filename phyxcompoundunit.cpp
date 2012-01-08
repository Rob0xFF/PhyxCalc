#include "phyxcompoundunit.h"

PhyxCompoundUnit::PhyxCompoundUnit(QObject *parent) :
    PhyxUnit(parent)
{
}

PhyxCompoundUnit::PhyxCompoundUnit(PhyxUnit *unit)
{
    m_compounds.clear();
    appendCompound(unit->symbol(),1);
    powersMultiply(unit->powers());
}

void PhyxCompoundUnit::appendCompound(QString base, double power)
{
    m_compounds.insert(base, power);
}

void PhyxCompoundUnit::compoundMultiply(QString base, double factor)
{
    double power;
    if (m_compounds.contains(base))
    {
        power = m_compounds.value(base);
        power += factor;
    }
    else
    {
        power = factor;
    }

    if (power == 0)
        m_compounds.remove(base);
    else
        m_compounds.insert(base, power);
}

void PhyxCompoundUnit::compoundDevide(QString base, double factor)
{
    double power;
    if (m_compounds.contains(base))
    {
        power = m_compounds.value(base);
        power -= factor;
    }
    else
    {
        power = -factor;
    }

    if (power == 0)
        m_compounds.remove(base);
    else
        m_compounds.insert(base, power);
}

void PhyxCompoundUnit::compoundsMultiply(PhyxUnit::PowerMap powers)
{
    QMapIterator<QString, double> i(powers);
     while (i.hasNext()) {
         i.next();
         compoundMultiply(i.key(), i.value());
     }
}

void PhyxCompoundUnit::compoundsDivide(PhyxUnit::PowerMap powers)
{
    QMapIterator<QString, double> i(powers);
     while (i.hasNext()) {
         i.next();
         compoundDevide(i.key(), i.value());
     }
}

void PhyxCompoundUnit::compoundsRaise(double power)
{
    QMapIterator<QString, double> i(m_compounds);
     while (i.hasNext()) {
         i.next();
         m_compounds.insert(i.key(), i.value() * power);
     }
}

void PhyxCompoundUnit::compoundsRoot(double root)
{
    QMapIterator<QString, double> i(m_compounds);
     while (i.hasNext()) {
         i.next();
         m_compounds.insert(i.key(), i.value() / root);
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
        //this->simplifyUnit();
        //variable->simplifyUnit();
        //make units the same

        return true;
    }
}

bool PhyxCompoundUnit::sub(PhyxCompoundUnit *unit)
{
    //same function as add
    add(unit);
}

void PhyxCompoundUnit::multiply(PhyxCompoundUnit *unit)
{
    if ((!this->isDimensionlessUnit()) && (!unit->isDimensionlessUnit()))
    {
        //this->simplifyUnit();
        //variable->simplifyUnit();
    }
    else
    {
        /*if (this->isDimensionlessUnit())
            this->simplifyUnit();
        if (unit->isDimensionlessUnit())
            unit->simplifyUnit();*/
    }

    this->powersMultiply(unit->powers());
    this->compoundsMultiply(unit->compounds());
}

void PhyxCompoundUnit::divide(PhyxCompoundUnit *unit)
{
    if ((!this->isDimensionlessUnit()) && (!unit->isDimensionlessUnit()))
    {
        //this->simplifyUnit();
        //variable->simplifyUnit();
    }
    else
    {
        /*if (this->isDimensionlessUnit())
            this->simplifyUnit();
        if (unit->isDimensionlessUnit())
            unit->simplifyUnit();*/
    }

    this->powersDivide(unit->powers());
    this->compoundsDivide(unit->compounds());
}

void PhyxCompoundUnit::raise(double power)
{
    this->powersRaise(power);
    this->compoundsRaise(power);
}

void PhyxCompoundUnit::root(double root)
{
    this->powersRoot(root);
    this->compoundsRoot(root);
}

const QString PhyxCompoundUnit::symbol()
{
    if (m_unitSystem != NULL)
    {
        /*if (m_unitSystem->verifyUnit(this))
        {
            m_compounds.clear();
            appendCompound(m_unit->symbol(),1);
        }
        else     // if unit is not in unit manager*/
        {
            //build a nice unit symbol
            int positiveCompoundsCount = 0;
            int negativeCompoundsCount = 0;
            QString positiveCompounds;
            QString negativeCompounds;
            QString compoundsString;

            QMapIterator<QString, double> i(m_compounds);
            while (i.hasNext())
            {
                i.next();
                if (i.value() > 0)
                {
                    if (!positiveCompoundsCount == 0)
                        positiveCompounds.append("*");

                    positiveCompounds.append(i.key());

                    if (i.value() != 1)
                        positiveCompounds.append(QString("%1").arg(i.value()));

                    positiveCompoundsCount++;
                }
                else
                {
                    if (!negativeCompoundsCount == 0)
                        negativeCompounds.append("*");

                    negativeCompounds.append(i.key());

                    if (i.value() != -1)
                        negativeCompounds.append(QString("%1").arg(-i.value()));

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
    }
}
