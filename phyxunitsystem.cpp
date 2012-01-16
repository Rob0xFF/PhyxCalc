#include "phyxunitsystem.h"

PhyxUnitSystem::PhyxUnitSystem(QObject *parent) :
    QObject(parent)
{
}

PhyxUnitSystem::~PhyxUnitSystem()
{
    // cleanup maps
    QMapIterator<QString, PhyxUnit*> i(baseUnitsMap);
    while (i.hasNext())
    {
        i.next();
        delete i.value();
    }
    QMapIterator<QString, PhyxUnit*> i2(derivedUnitsMap);
    while (i2.hasNext())
    {
        i2.next();
        delete i2.value();
    }
}

void PhyxUnitSystem::addBaseUnit(QString symbol, PhyxUnit::UnitFlags flags, QString unitGroup, QString preferedPrefix)
{
    if (baseUnitsMap.contains(symbol))
        delete baseUnitsMap.take(symbol);

   PhyxUnit *unit = new PhyxUnit();
   unit->setSymbol(symbol);
   unit->powerAppend(symbol,1);
   unit->setFlags(flags);
   unit->setUnitGroup(unitGroup);
   unit->setPreferedPrefix(preferedPrefix);
   baseUnitsMap.insert(symbol, unit);

    if (derivedUnitsMap.contains(symbol))
    {
        delete derivedUnitsMap.take(symbol);
        recalculate();
    }

    emit unitAdded(symbol);
}

/*void PhyxUnitSystem::addDerivedUnit(QString symbol, PhyxVariable *variable, double offset, PhyxUnit::UnitFlags flags)
{
    if (baseUnitsMap.contains(symbol))
        delete baseUnitsMap.take(symbol);

    PhyxUnit *unit;

    if (derivedUnitsMap.contains(symbol))
        unit = derivedUnitsMap.value(symbol);
    else
        unit = new PhyxUnit();

   variable->simplifyUnit();
   unit->setSymbol(symbol);
   unit->setPowers(variable->unit()->powers());
   unit->setScaleFactor(variable->value());
   unit->setOffset(offset);
   unit->setFlags(flags);
   derivedUnitsMap.insert(symbol, unit);

   recalculate();
}*/

void PhyxUnitSystem::addDerivedUnit(PhyxUnit *unit)
{
    if (baseUnitsMap.contains(unit->symbol()))
        delete baseUnitsMap.take(unit->symbol());

    if (derivedUnitsMap.contains(unit->symbol()))
        delete derivedUnitsMap.take(unit->symbol());

   derivedUnitsMap.insert(unit->symbol(), unit);
   recalculate();

   emit unitAdded(unit->symbol());
}

bool PhyxUnitSystem::removeUnit(QString symbol)
{
    if (baseUnitsMap.contains(symbol))
        delete baseUnitsMap.take(symbol);

    if (derivedUnitsMap.contains(symbol))
        delete derivedUnitsMap.take(symbol);

    recalculate();

    emit unitRemoved(symbol);
}

void PhyxUnitSystem::addPrefix(QString symbol, double value, QString unitSystem)
{
    PhyxPrefix prefix;
    prefix.value = value;
    prefix.unitGroup = unitSystem;

    prefixMap.insert(symbol, prefix);
    emit prefixAdded(symbol);
}

bool PhyxUnitSystem::removePrefix(QString symbol)
{
    prefixMap.remove(symbol);
    emit prefixRemoved(symbol);
    return true;
}

void PhyxUnitSystem::addUnitGroup(QString name)
{
    if (!unitGroupsList.contains(name))
        unitGroupsList.append(name);
    emit unitGroupAdded(name);
}

bool PhyxUnitSystem::removeUnitGroup(QString name)
{
    unitGroupsList.removeOne(name);
    emit unitGroupRemoved(name);
    return true;
}

void PhyxUnitSystem::recalculateUnits()
{
}

void PhyxUnitSystem::recalculateVariables()
{
}

void PhyxUnitSystem::recalculate()
{
    recalculateUnits();
    recalculateVariables();
}

PhyxUnit * PhyxUnitSystem::copyUnit(QString symbol)
{
    if (baseUnitsMap.contains(symbol))
    {
        PhyxUnit *unit = new PhyxUnit();
        PhyxUnit::copyUnit(baseUnitsMap.value(symbol), unit);
        return unit;
    }
    else if (derivedUnitsMap.contains(symbol))
    {
        PhyxUnit *unit = new PhyxUnit();
        PhyxUnit::copyUnit(derivedUnitsMap.value(symbol), unit);
        return unit;
    }
    else
        return new PhyxUnit();
}

PhyxUnit *PhyxUnitSystem::unit(QString symbol)
{
    if (baseUnitsMap.contains(symbol))
        return baseUnitsMap.value(symbol);
    else if (derivedUnitsMap.contains(symbol))
        return derivedUnitsMap.value(symbol);
    else
        return new PhyxUnit();
}

PhyxUnitSystem::PhyxPrefix PhyxUnitSystem::prefix(QString symbol)
{
    PhyxPrefix prefix;
    prefix.value = 1;
    prefix.unitGroup = "";

    return prefixMap.value(symbol, prefix);
}

bool PhyxUnitSystem::verifyUnit(PhyxUnit *unit)
{
    if (unit->isBaseUnit())
    {
        QMapIterator<QString, PhyxUnit*> i(baseUnitsMap);
        while (i.hasNext())
        {
            i.next();
            if (i.value()->isSame(unit))
            {
                unit->setSymbol(i.value()->symbol());
                unit->setName(i.value()->name());
                unit->setFlags(i.value()->flags());
                return true;
            }
        }
    }
    else
    {
        QMapIterator<QString, PhyxUnit*> i(derivedUnitsMap);
        while (i.hasNext())
        {
            i.next();
            if (i.value()->isSame(unit))
            {
                unit->setSymbol(i.value()->symbol());
                unit->setName(i.value()->name());
                unit->setFlags(i.value()->flags());
                return true;
            }
        }
    }

    return false;
}
