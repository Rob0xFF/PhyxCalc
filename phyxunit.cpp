#include "phyxunit.h"

PhyxUnit::PhyxUnit(QObject *parent) :
    QObject(parent)
{
    m_symbol = "";
    m_name   = "";
    m_unitGroup = "";
    m_preferedPrefix = "";
    m_offset = 0;
    m_scaleFactor = 1;
    m_flags = 0;
}

void PhyxUnit::powerAppend(QString base, double power)
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

void PhyxUnit::powerDivide(QString base, double factor)
{
    double power;
    if (m_powers.contains(base))
    {
        power = m_powers.value(base);
        power -= factor;
    }
    else
    {
        power = -factor;
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

void PhyxUnit::powersDivide(PhyxUnit::PowerMap powers)
{
    QMapIterator<QString, double> i(powers);
     while (i.hasNext()) {
         i.next();
         powerDivide(i.key(), i.value());
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

void PhyxUnit::powersClear()
{
    m_powers.clear();
}

bool PhyxUnit::isOne()
{
    return ((m_scaleFactor == 1) && (m_offset == 0) && (m_powers.isEmpty()));
}

bool PhyxUnit::isBaseUnit()
{
    if (((m_scaleFactor == 1) && (m_offset == 0) && (m_powers.size() == 1)))
    {
        QMapIterator<QString, double> i(m_powers);
        i.next();
        if (i.value() == 1)
            return true;
        else
            return false;
    }
    else
        return false;
}

bool PhyxUnit::isDimensionlessUnit()
{
    return ((m_offset == 0) && (m_powers.isEmpty()));
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
    return powersCompare(unit->powers());
}

bool PhyxUnit::isSame(PhyxUnit *unit)
{
    if (this->offset() != unit->offset())
        return false;
    if (this->scaleFactor() != unit->scaleFactor())
        return false;
    return powersCompare(unit->powers());
}

void PhyxUnit::copyUnit(PhyxUnit *source, PhyxUnit *destination)
{
    destination->setPowers(source->powers());
    destination->setSymbol(source->symbol());
    destination->setOffset(source->offset());
    destination->setScaleFactor(source->scaleFactor());
    destination->setFlags(source->flags());
}

QString PhyxUnit::dimensionString() const
{
    QString outputString;
    QMapIterator<QString, double> i(m_powers);
    while (i.hasNext())
    {
        i.next();
        if (!outputString.isEmpty())
            outputString.append('*');
        outputString.append(QString("%1%2").arg(i.key()).arg(i.value()));
    }

    // unicodify string
    outputString.replace(QChar('*'), QChar(0x22C5)); //mathematic dot operator
    outputString.replace(QChar('0'), QChar(0x2070));
    outputString.replace(QChar('1'), QChar(0x00B9));
    outputString.replace(QChar('2'), QChar(0x00B2));
    outputString.replace(QChar('3'), QChar(0x00B3));
    outputString.replace(QChar('4'), QChar(0x2074));
    outputString.replace(QChar('5'), QChar(0x2075));
    outputString.replace(QChar('6'), QChar(0x2076));
    outputString.replace(QChar('7'), QChar(0x2077));
    outputString.replace(QChar('8'), QChar(0x2078));
    outputString.replace(QChar('9'), QChar(0x2079));
    outputString.replace(QChar('-'), QChar(0x207B));
    return outputString;
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
