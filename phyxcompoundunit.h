#ifndef PHYXCOMPOUNDUNIT_H
#define PHYXCOMPOUNDUNIT_H

#include "phyxunit.h"
#include "phyxunitsystem.h"
#include "cmath"

class PhyxUnitSystem;

class PhyxCompoundUnit : public PhyxUnit
{
    /// a compound of the compound unit
    typedef struct CompoundStruct{
        PhyxUnit    *unit;  /// the unit of the compound
        double      power;  /// the power of the compound

        bool operator == (CompoundStruct compound){
            return (unit->isSame(compound.unit) && (power == compound.power));
        }
    } PhyxCompound;
    typedef QList<PhyxCompound>     PhyxCompoundList;

    Q_OBJECT
    Q_PROPERTY(PhyxUnitSystem *unitSystem READ unitSystem WRITE setUnitSystem)
    Q_PROPERTY(PhyxCompoundList compounds READ compounds)

public:
    explicit PhyxCompoundUnit(QObject *parent = 0);
    explicit PhyxCompoundUnit(PhyxUnit *unit);

    void compoundSimplify(int index);                   /// simplifies a unit
    void compoundsEqualize(PhyxCompoundUnit *unit);     /// equalizes two units

    bool isSame(PhyxCompoundUnit *unit);
    bool isConvertible(PhyxCompoundUnit *unit);
    bool isOne();
    bool isSimpleUnit();                                /// returns wheter this unit contains only one unit or not

    bool add(PhyxCompoundUnit *unit);
    bool sub(PhyxCompoundUnit *unit);
    void multiply(PhyxCompoundUnit *unit);
    void divide(PhyxCompoundUnit *unit);
    void raise(double power);
    void root(double root);

    bool convertTo(PhyxCompoundUnit *unit);               ///< converts the variable to the given unit, returns successful
    void fromSimpleUnit(PhyxUnit *unit);

    void simplify();                                    ///< simplifies the unit (e.g.: GalileanUnit -> ProductUnit, DimensionlessUnit -> NoUnit)

    QString const symbol();
    PhyxUnitSystem * unitSystem() const
{
    return m_unitSystem;
}
    PhyxCompoundList compounds() const
{
    return m_compounds;
}

private:
    PhyxUnitSystem * m_unitSystem;

    PhyxCompoundList  m_compounds;    /// holds all compounds of a unit: e.g.: m/s -> compuound 1: m^1, compound 2: s^-1
    void compoundAppend(PhyxUnit *unit, double power);       /// adds a power to the map
    void compoundMultiply(PhyxUnit *unit, double factor);    /// multiplies a power with factor
    void compoundDivide(PhyxUnit *unit, double factor);      /// devides a power with factor
    void compoundEqualize(int unitIndex, PhyxCompoundUnit *parentUnit);
    void compoundsMultiply(PhyxCompoundList compounds);              /// multiplies powers of the unit with other powers
    void compoundsDivide(PhyxCompoundList compounds);                /// devides powers of the unit with other powers
    void compoundsRaise(double power);                       /// raises all powers to power
    void compoundsRoot(double root);                         /// takes the root of all powers
    void compoundsClear();

    void verify();                                          /// verifies the unit

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
