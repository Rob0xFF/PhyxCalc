#include "phyxunitmanager.h"

PhyxUnitManager::PhyxUnitManager(QObject *parent) :
    QObject(parent)
{
}

void PhyxUnitManager::addBaseUnit(QString symbol, bool isSiUnit)
{
    baseUnitsMap.insert(symbol, isSiUnit);

    if (derivedUnitMap.contains(symbol))
    {
        derivedUnitMap.remove(symbol);
        recalculate();
    }
}

void PhyxUnitManager::addDerivedUnit(QString symbol, PhyxUnit::PowerMap powers, double scaleFactor, double offset)
{
    if (baseUnitsMap.contains(symbol))
        baseUnitsMap.remove(symbol);

    PhyxUnit *unit;

    if (derivedUnitMap.contains(symbol))
        unit = derivedUnitMap.value(symbol);
    else
        unit = new PhyxUnit();

   unit->setSymbol(symbol);
   unit->setPowers(powers);
   unit->setScaleFactor(scaleFactor);
   unit->setOffset(offset);
   derivedUnitMap.insert(symbol, unit);

   recalculate();
}

void PhyxUnitManager::addDerivedUnit(QString symbol, PhyxVariable *variable, double offset)
{
    if (baseUnitsMap.contains(symbol))
        baseUnitsMap.remove(symbol);

    PhyxUnit *unit;

    if (derivedUnitMap.contains(symbol))
        unit = derivedUnitMap.value(symbol);
    else
        unit = new PhyxUnit();

   variable->simplifyUnit();
   unit->setSymbol(symbol);
   unit->setPowers(variable->unit()->powers());
   unit->setScaleFactor(variable->value());
   unit->setOffset(offset);
   derivedUnitMap.insert(symbol, unit);

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

PhyxUnit * PhyxUnitManager::getUnit(QString symbol)
{
    if (baseUnitsMap.contains(symbol))
    {
        PhyxUnit *unit = new PhyxUnit();
        unit->appendPower(symbol, 1);
        unit->setSymbol(symbol);

        return unit;
    }
    else if (derivedUnitMap.contains(symbol))
    {
        PhyxUnit *unit = new PhyxUnit();
        unit->setPowers(derivedUnitMap.value(symbol)->powers());
        unit->setSymbol(symbol);
        unit->setOffset(derivedUnitMap.value(symbol)->offset());
        unit->setScaleFactor(derivedUnitMap.value(symbol)->scaleFactor());
        unit->setFlags(derivedUnitMap.value(symbol)->flags());
        return unit;
    }
    else
    {
        return new PhyxUnit();
    }
}
