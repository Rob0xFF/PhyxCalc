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
    PhyxUnit *unit = new PhyxUnit();
    unit->setSymbol(symbol);
    unit->setPowers(powers);
    unit->setScaleFactor(scaleFactor);
    unit->setOffset(offset);

    if (baseUnitsMap.contains(symbol))
        baseUnitsMap.remove(symbol);

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
        unit->appendPower("symbol", 1);

        return unit;
    }
    else if (derivedUnitMap.contains(symbol))
    {
        return derivedUnitMap.value(symbol);
    }
    else
    {
        return new PhyxUnit();
    }
}
