#ifndef PHYXCOMPOUNDUNIT_H
#define PHYXCOMPOUNDUNIT_H

#include "phyxunit.h"
#include "phyxunitsystem.h"

class PhyxUnitSystem;

class PhyxCompoundUnit : public PhyxUnit
{
    Q_OBJECT
    Q_PROPERTY(PhyxUnitSystem *unitSystem READ unitSystem WRITE setUnitSystem)
    Q_PROPERTY(PhyxUnit::PowerMap compounds READ compounds)

public:
    explicit PhyxCompoundUnit(QObject *parent = 0);
    explicit PhyxCompoundUnit(PhyxUnit *unit);

    bool add(PhyxCompoundUnit *unit);
    bool sub(PhyxCompoundUnit *unit);
    void multiply(PhyxCompoundUnit *unit);
    void divide(PhyxCompoundUnit *unit);
    void raise(double power);
    void root(double root);

    QString const symbol();
    PhyxUnitSystem * unitSystem() const
{
    return m_unitSystem;
}
    PhyxUnit::PowerMap compounds() const
{
    return m_compounds;
}

private:
    PhyxUnitSystem * m_unitSystem;

    PhyxUnit::PowerMap  m_compounds;    /// holds all compounds of a unit: e.g.: m/s -> compuound 1: m^1, compound 2: s^-1
    void appendCompound(QString base, double power);       /// adds a power to the map
    void compoundMultiply(QString base, double factor);    /// multiplies a power with factor
    void compoundDevide(QString base, double factor);      /// devides a power with factor
    void compoundsMultiply(PhyxUnit::PowerMap powers);               /// multiplies powers of the unit with other powers
    void compoundsDivide(PhyxUnit::PowerMap powers);                 /// devides powers of the unit with other powers
    void compoundsRaise(double power);                     /// raises all powers to power
    void compoundsRoot(double root);                       /// takes the root of all powers

signals:
    //double offsetValue(double value) {return value;}
    //double scaleValue(double scaleFactor) {return scaleFactor;}
    
public slots:

void setUnitSystem(PhyxUnitSystem * arg)
{
    m_unitSystem = arg;
}
};

#endif // PHYXCOMPOUNDUNIT_H
