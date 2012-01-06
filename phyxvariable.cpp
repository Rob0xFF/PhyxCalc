#include "phyxvariable.h"

PhyxVariable::PhyxVariable(QObject *parent) :
    QObject(parent)
{
    m_value = 1;
    m_unit = new PhyxUnit();
}

PhyxVariable::~PhyxVariable()
{
    delete m_unit;
}

void PhyxVariable::simplifyUnit()
{
    //make the galilean transformation y = ax - b
    m_value *= m_unit->scaleFactor();
    m_value -= m_unit->offset();
    m_unit->setOffset(0);
    m_unit->setScaleFactor(1);

    updateUnitSymbol();
}

void PhyxVariable::updateUnitSymbol()
{
    if (m_unitManager != NULL)
    {
        if (m_unitManager->verifyUnit(m_unit))
        {
            m_compounds.clear();
            appendCompound(m_unit->symbol(),1);
        }
        else     // if unit is not in unit manager
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

            m_unit->setSymbol(compoundsString);
        }
    }
}

bool PhyxVariable::convertUnit(PhyxUnit *unit)
{
    if (!this->unit()->isConvertible(unit))
    {
        m_error = PhyxVariable::UnitsNotConvertibleError;
        return false;
    }
    else
    {
        this->simplifyUnit();

        // make the inverse galilean transformation x = (y+b)/a
        m_value += unit->offset();
        m_value /= unit->scaleFactor();
        m_unit->setPowers(unit->powers());
        m_unit->setScaleFactor(unit->scaleFactor());
        m_unit->setOffset(unit->offset());
        m_unit->setFlags(unit->flags());

        return true;
    }
}

bool PhyxVariable::mathAdd(PhyxVariable *variable)
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

bool PhyxVariable::mathSub(PhyxVariable *variable)
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

bool PhyxVariable::mathMul(PhyxVariable *variable)
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
    this->compoundsMultiply(variable->compounds());

    this->updateUnitSymbol();

    return true;
}

bool PhyxVariable::mathDiv(PhyxVariable *variable)
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
    this->compoundsDivide(variable->compounds());

    this->updateUnitSymbol();

    return true;
}

void PhyxVariable::mathNeg()
{
    m_value = -m_value;
}

bool PhyxVariable::mathRaise(PhyxVariable *variable)
{
    if (!variable->unit()->isDimensionlessUnit())
    {
        m_error = PhyxVariable::UnitNotDimensionlessError;
        return false;
    }
    this->simplifyUnit();
    variable->simplifyUnit();

    this->m_value = pow(this->m_value, variable->value());
    this->unit()->powersRaise(variable->value());
    this->compoundsRaise(variable->value());

    this->updateUnitSymbol();

    return true;
}

bool PhyxVariable::mathRoot(PhyxVariable *variable)
{
    if (!variable->unit()->isDimensionlessUnit())
    {
        m_error = PhyxVariable::UnitNotDimensionlessError;
        return false;
    }
    this->simplifyUnit();
    variable->simplifyUnit();

    this->m_value = pow(this->m_value, 1.0/variable->value());
    this->unit()->powersRoot(variable->value());
    this->compoundsRoot(variable->value());

    this->updateUnitSymbol();

    return true;
}

void PhyxVariable::mathSqrt()
{
    this->simplifyUnit();

    this->m_value = sqrt(this->m_value);
    this->unit()->powersRoot(2);
}

bool PhyxVariable::mathSin()
{
    if (!this->unit()->isDimensionlessUnit())
    {
        m_error = PhyxVariable::UnitNotDimensionlessError;
        return false;
    }
    this->simplifyUnit();

    m_value = sin(m_value);
    return true;
}

bool PhyxVariable::mathArcsin()
{
    if (!this->unit()->isDimensionlessUnit())
    {
        m_error = PhyxVariable::UnitNotDimensionlessError;
        return false;
    }
    this->simplifyUnit();

    m_value = asin(m_value);
    return true;
}

bool PhyxVariable::mathCos()
{
    if (!this->unit()->isDimensionlessUnit())
    {
        m_error = PhyxVariable::UnitNotDimensionlessError;
        return false;
    }
    this->simplifyUnit();

    m_value = cos(m_value);
    return true;
}

bool PhyxVariable::mathArccos()
{
    if (!this->unit()->isDimensionlessUnit())
    {
        m_error = PhyxVariable::UnitNotDimensionlessError;
        return false;
    }
    this->simplifyUnit();

    m_value = acos(m_value);
    return true;
}

bool PhyxVariable::mathTan()
{
    if (!this->unit()->isDimensionlessUnit())
    {
        m_error = PhyxVariable::UnitNotDimensionlessError;
        return false;
    }
    this->simplifyUnit();

    m_value = tan(m_value);
    return true;
}

bool PhyxVariable::mathArctan()
{
    if (!this->unit()->isDimensionlessUnit())
    {
        m_error = PhyxVariable::UnitNotDimensionlessError;
        return false;
    }
    this->simplifyUnit();

    m_value = atan(m_value);
    return true;
}

bool PhyxVariable::mathSinh()
{
    if (!this->unit()->isDimensionlessUnit())
    {
        m_error = PhyxVariable::UnitNotDimensionlessError;
        return false;
    }
    this->simplifyUnit();

    m_value = sinh(m_value);
    return true;
}

bool PhyxVariable::mathArcsinh()
{
    if (!this->unit()->isDimensionlessUnit())
    {
        m_error = PhyxVariable::UnitNotDimensionlessError;
        return false;
    }
    this->simplifyUnit();

    m_value = asinh(m_value);
    return true;
}

bool PhyxVariable::mathCosh()
{
    if (!this->unit()->isDimensionlessUnit())
    {
        m_error = PhyxVariable::UnitNotDimensionlessError;
        return false;
    }
    this->simplifyUnit();

    m_value = cosh(m_value);
    return true;
}

bool PhyxVariable::mathArccosh()
{
    if (!this->unit()->isDimensionlessUnit())
    {
        m_error = PhyxVariable::UnitNotDimensionlessError;
        return false;
    }
    this->simplifyUnit();

    m_value = acosh(m_value);
    return true;
}

bool PhyxVariable::mathTanh()
{
    if (!this->unit()->isDimensionlessUnit())
    {
        m_error = PhyxVariable::UnitNotDimensionlessError;
        return false;
    }
    this->simplifyUnit();

    m_value = tanh(m_value);
    return true;
}

bool PhyxVariable::mathArctanh()
{
    if (!this->unit()->isDimensionlessUnit())
    {
        m_error = PhyxVariable::UnitNotDimensionlessError;
        return false;
    }
    this->simplifyUnit();

    m_value = atanh(m_value);
    return true;
}

bool PhyxVariable::mathExp()
{
    if (!this->unit()->isDimensionlessUnit())
    {
        m_error = PhyxVariable::UnitNotDimensionlessError;
        return false;
    }
    this->simplifyUnit();

    m_value = exp(m_value);
    return true;
}

bool PhyxVariable::mathLn()
{
    if (!this->unit()->isDimensionlessUnit())
    {
        m_error = PhyxVariable::UnitNotDimensionlessError;
        return false;
    }
    this->simplifyUnit();

    m_value = log(m_value);
    return true;
}

bool PhyxVariable::mathLog10()
{
    if (!this->unit()->isDimensionlessUnit())
    {
        m_error = PhyxVariable::UnitNotDimensionlessError;
        return false;
    }
    this->simplifyUnit();

    m_value = log10(m_value);
    return true;
}

bool PhyxVariable::mathLog2()
{
    if (!this->unit()->isDimensionlessUnit())
    {
        m_error = PhyxVariable::UnitNotDimensionlessError;
        return false;
    }
    this->simplifyUnit();

    m_value = log2(m_value);
    return true;
}

bool PhyxVariable::mathLogn(PhyxVariable *variable)
{
    if (!this->unit()->isDimensionlessUnit() || !variable->unit()->isDimensionlessUnit())
    {
        m_error = PhyxVariable::UnitNotDimensionlessError;
        return false;
    }
    this->simplifyUnit();
    variable->simplifyUnit();

    m_value = log(this->value()) / log(variable->value());
    return true;
}

void PhyxVariable::mathAbs()
{
    m_value = abs(m_value);
}

bool PhyxVariable::mathMax(PhyxVariable *variable)
{
    if (this->unit()->isSame(variable->unit()))
    {
        this->m_value = qMax(this->value(), variable->value());
        return true;
    }
    else if (this->unit()->isConvertible(variable->unit()))
    {
        PhyxUnit *unit1 = new PhyxUnit();
        PhyxUnit *unit2 = new PhyxUnit();
        PhyxUnit::copyUnit(this->unit(), unit1);
        PhyxUnit::copyUnit(variable->unit(), unit2);

        this->simplifyUnit();
        variable->simplifyUnit();

        if (this->value() >= variable->value())
        {
            this->convertUnit(unit1);
        }
        else
        {
            m_value = variable->value();
            this->convertUnit(unit2);
        }

        delete unit1;
        delete unit2;
        return true;
    }
    else
    {
        this->m_error = UnitsNotConvertibleError;
        return false;
    }
}

bool PhyxVariable::mathMin(PhyxVariable *variable)
{
    if (this->unit()->isSame(variable->unit()))
    {
        this->m_value = qMax(this->value(), variable->value());
        return true;
    }
    else if (this->unit()->isConvertible(variable->unit()))
    {
        PhyxUnit *unit1 = new PhyxUnit();
        PhyxUnit *unit2 = new PhyxUnit();
        PhyxUnit::copyUnit(this->unit(), unit1);
        PhyxUnit::copyUnit(variable->unit(), unit2);

        this->simplifyUnit();
        variable->simplifyUnit();

        if (this->value() <= variable->value())
        {
            this->convertUnit(unit1);
        }
        else
        {
            m_value = variable->value();
            this->convertUnit(unit2);
        }

        delete unit1;
        delete unit2;
        return true;
    }
    else
    {
        this->m_error = UnitsNotConvertibleError;
        return false;
    }
}

void PhyxVariable::mathInt()
{
    m_value = (int)m_value;
}

void PhyxVariable::mathTrunc()
{
    m_value = trunc(m_value);
}

void PhyxVariable::mathFloor()
{
    m_value = floor(m_value);
}

void PhyxVariable::mathRound()
{
    m_value = round(m_value);
}

void PhyxVariable::mathCeil()
{
    m_value = ceil(m_value);
}

void PhyxVariable::mathSign()
{
    m_value = copysign(1.0,m_value);
}

bool PhyxVariable::mathHeaviside()
{
    if (!this->unit()->isDimensionlessUnit())
    {
        m_error = PhyxVariable::UnitNotDimensionlessError;
        return false;
    }
    this->simplifyUnit();

    if (m_value >= 0)
        m_value = 1;
    else
        m_value = 0;

    return true;
}

bool PhyxVariable::mathRandg(PhyxVariable *variable)
{
    if (!this->unit()->isDimensionlessUnit() || !variable->unit()->isDimensionlessUnit())
    {
        m_error = PhyxVariable::UnitNotDimensionlessError;
        return false;
    }
    this->simplifyUnit();
    variable->simplifyUnit();

    int min = this->value();
    int max = variable->value();
    m_value = min + qrand()%(max-min+1);

    return true;
}

bool PhyxVariable::mathFaculty()
{
    if (!this->unit()->isDimensionlessUnit())
    {
        m_error = PhyxVariable::UnitNotDimensionlessError;
        return false;
    }
    this->simplifyUnit();

    PhyxValueDataType value = m_value;

    if (value < 0)
    {
        m_error = PhyxVariable::ValueNotPositiveError;
        return false;
    }
    if ((int)value != value)
    {
        m_error = PhyxVariable::ValueNotIntegerError;
        return false;
    }

    int n = (int)round(value);
    value = 1;
    for (int i = 2; i <=n; i++)
        value *= i;

    m_value = value;

    return true;
}

void PhyxVariable::appendCompound(QString base, double power)
{
    m_compounds.insert(base, power);
}

void PhyxVariable::compoundMultiply(QString base, double factor)
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

void PhyxVariable::compoundDevide(QString base, double factor)
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

void PhyxVariable::compoundsMultiply(PhyxUnit::PowerMap powers)
{
    QMapIterator<QString, double> i(powers);
     while (i.hasNext()) {
         i.next();
         compoundMultiply(i.key(), i.value());
     }
}

void PhyxVariable::compoundsDivide(PhyxUnit::PowerMap powers)
{
    QMapIterator<QString, double> i(powers);
     while (i.hasNext()) {
         i.next();
         compoundDevide(i.key(), i.value());
     }
}

void PhyxVariable::compoundsRaise(double power)
{
    QMapIterator<QString, double> i(m_compounds);
     while (i.hasNext()) {
         i.next();
         m_compounds.insert(i.key(), i.value() * power);
     }
}

void PhyxVariable::compoundsRoot(double root)
{
    QMapIterator<QString, double> i(m_compounds);
     while (i.hasNext()) {
         i.next();
         m_compounds.insert(i.key(), i.value() / root);
     }
}
