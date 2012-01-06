#include "phyxunitmanager.h"

PhyxUnitManager::PhyxUnitManager(QObject *parent) :
    QObject(parent)
{
}

void PhyxUnitManager::addBaseUnit(QString symbol, PhyxUnit::UnitFlags flags)
{


    if (baseUnitsMap.contains(symbol))
        delete baseUnitsMap.take(symbol);

   PhyxUnit *unit = new PhyxUnit();
   unit->setSymbol(symbol);
   unit->appendPower(symbol,1);
   unit->setFlags(flags);
   baseUnitsMap.insert(symbol, unit);

    if (derivedUnitsMap.contains(symbol))
    {
        delete derivedUnitsMap.take(symbol);
        recalculate();
    }
}

void PhyxUnitManager::addDerivedUnit(QString symbol, PhyxVariable *variable, double offset, PhyxUnit::UnitFlags flags)
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
}

void PhyxUnitManager::addDerivedUnit(PhyxUnit *unit)
{
    if (baseUnitsMap.contains(unit->symbol()))
        delete baseUnitsMap.take(unit->symbol());

    if (derivedUnitsMap.contains(unit->symbol()))
        delete derivedUnitsMap.take(unit->symbol());

   derivedUnitsMap.insert(unit->symbol(), unit);
   recalculate();
}

void PhyxUnitManager::recalculateUnits()
{
}

void PhyxUnitManager::recalculateVariables()
{
}

void PhyxUnitManager::recalculate()
{
    recalculateUnits();
    recalculateVariables();
}

PhyxUnit * PhyxUnitManager::copyUnit(QString symbol)
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

PhyxUnit *PhyxUnitManager::unit(QString symbol)
{
    if (baseUnitsMap.contains(symbol))
        return baseUnitsMap.value(symbol);
    else if (derivedUnitsMap.contains(symbol))
        return derivedUnitsMap.value(symbol);
    else
        return new PhyxUnit();
}

bool PhyxUnitManager::verifyUnit(PhyxUnit *unit)
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
