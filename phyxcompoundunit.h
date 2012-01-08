#ifndef PHYXCOMPOUNDUNIT_H
#define PHYXCOMPOUNDUNIT_H

#include "phyxunit.h"
#include "phyxunitsystem.h"

class PhyxUnitSystem;

class PhyxCompoundUnit : public PhyxUnit
{
    typedef QMap<PhyxUnit*, double> PhyxUnitMap;

    Q_OBJECT
    Q_PROPERTY(PhyxUnitSystem *unitSystem READ unitSystem WRITE setUnitSystem)
    Q_PROPERTY(PhyxUnitMap compounds READ compounds)

public:
    explicit PhyxCompoundUnit(QObject *parent = 0);
    explicit PhyxCompoundUnit(PhyxUnit *unit);

    void compoundSimplify(PhyxUnit *unit);                   /// simplifies a unit
    void compoundsEqualize(PhyxCompoundUnit *unit);              /// equalizes two units

    bool isSame(PhyxCompoundUnit *unit);
    bool isConvertible(PhyxCompoundUnit *unit);
    bool isDimensionlessUnit();

    bool add(PhyxCompoundUnit *unit);
    bool sub(PhyxCompoundUnit *unit);
    void multiply(PhyxCompoundUnit *unit);
    void divide(PhyxCompoundUnit *unit);
    void raise(double power);
    void root(double root);

    bool convertTo(PhyxCompoundUnit *unit);               ///< converts the variable to the given unit, returns successful

    QString const symbol();
    PhyxUnitSystem * unitSystem() const
{
    return m_unitSystem;
}
    PhyxUnitMap compounds() const
{
    return m_compounds;
}

private:
    PhyxUnitSystem * m_unitSystem;

    PhyxUnitMap  m_compounds;    /// holds all compounds of a unit: e.g.: m/s -> compuound 1: m^1, compound 2: s^-1
    void appendCompound(PhyxUnit *base, double power);       /// adds a power to the map
    void compoundMultiply(PhyxUnit *unit, double factor);    /// multiplies a power with factor
    void compoundDevide(PhyxUnit *base, double factor);      /// devides a power with factor
    void compoundEqualize(PhyxUnit *unit, PhyxCompoundUnit *parentUnit);
    void compoundsMultiply(PhyxUnitMap compounds);              /// multiplies powers of the unit with other powers
    void compoundsDivide(PhyxUnitMap compounds);                /// devides powers of the unit with other powers
    void compoundsRaise(double power);                       /// raises all powers to power
    void compoundsRoot(double root);                         /// takes the root of all powers

signals:
    void offsetValue(double value);
    void scaleValue(double scaleFactor);
    
public slots:

void setUnitSystem(PhyxUnitSystem * arg)
{
    m_unitSystem = arg;
}
};

#endif // PHYXCOMPOUNDUNIT_H
