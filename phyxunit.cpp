#include "phyxunit.h"

PhyxUnit::PhyxUnit(QObject *parent) :
    QObject(parent)
{
    m_symbol = "";
    m_name   = "";
    m_offset = 0;
    m_scaleFactor = 1;
    m_flags = 0;
}

void PhyxUnit::appendPower(QString base, double power)
{
    m_powers.insert(base, power);
}

void PhyxUnit::powerMultiply(QString base, double factor)
{
    double power;
    if (m_powers.contains(base))
    {
        power = m_powers.value(base);
        power += factor;
    }
    else
    {
        power = factor;
    }

    if (power == 0)
        m_powers.remove(base);
    else
        m_powers.insert(base, power);
}

void PhyxUnit::powerDevide(QString base, double factor)
{
    double power;
    if (m_powers.contains(base))
    {
        power = m_powers.value(base);
        power -= factor;
    }
    else
    {
        power = factor;
    }

    if (power == 0)
        m_powers.remove(base);
    else
        m_powers.insert(base, power);
}

void PhyxUnit::powersMultiply(PhyxUnit::PowerMap powers)
{
    QMapIterator<QString, double> i(powers);
     while (i.hasNext()) {
         i.next();
         powerMultiply(i.key(), i.value());
     }
}

void PhyxUnit::powersDevide(PhyxUnit::PowerMap powers)
{
    QMapIterator<QString, double> i(powers);
     while (i.hasNext()) {
         i.next();
         powerDevide(i.key(), i.value());
     }
}

void PhyxUnit::powersRaise(double power)
{
    QMapIterator<QString, double> i(m_powers);
     while (i.hasNext()) {
         i.next();
         m_powers.insert(i.key(), i.value() * power);
     }
}

void PhyxUnit::powersRoot(double root)
{
    QMapIterator<QString, double> i(m_powers);
     while (i.hasNext()) {
         i.next();
         m_powers.insert(i.key(), i.value() / root);
     }
}

bool PhyxUnit::powersCompare(PhyxUnit::PowerMap powers)
{
    return m_powers == powers;
}

bool PhyxUnit::isOne()
{
    return ((m_scaleFactor == 1) && (m_offset == 0) && (m_powers.isEmpty()));
}

bool PhyxUnit::isBaseUnit()
{
    return ((m_scaleFactor == 1) && (m_offset == 0) && (m_powers.size() == 1));
}

bool PhyxUnit::isDimensionlessUnit()
{
    return ((m_scaleFactor != 1) && (m_offset == 0) && (m_powers.isEmpty()));
}

bool PhyxUnit::isProductUnit()
{
    return ((m_scaleFactor == 1) && (m_offset == 0) && (m_powers.size() >= 1));
}

bool PhyxUnit::isGalileanUnit()
{
    return (((m_scaleFactor != 1) || (m_offset != 0)) && (m_powers.size() >= 1));
}

bool PhyxUnit::isConvertible(PhyxUnit *unit)
{
    return m_powers == unit->powers();
}

bool PhyxUnit::isSame(PhyxUnit *unit)
{
    if (offset() != unit->offset())
        return false;
    if (scaleFactor() != unit->scaleFactor())
        return false;
    return powersCompare(unit->powers());
}

/*void PhyxUnit::prefixMultiply(double factor)
{
    m_prefixPower += factor;
}

void PhyxUnit::prefixDevide(double factor)
{
    m_prefixPower -= factor;
}

void PhyxUnit::prefixRaise(double power)
{
    m_prefixPower *= power;
}

void PhyxUnit::prefixRoot(double root)
{
    m_prefixPower /= root;
}*/
