#include "phyxtesting.h"

PhyxTesting::PhyxTesting(QObject *parent) :
    QObject(parent)
{
}

void PhyxTesting::testUnits()
{
    PhyxUnitSystem unitSystem;

    unitSystem.addBaseUnit("m", PhyxUnit::SiUnitFlag);
    unitSystem.addBaseUnit("kg", PhyxUnit::SiUnitFlag);
    unitSystem.addBaseUnit("A", PhyxUnit::SiUnitFlag);
    unitSystem.addBaseUnit("K", PhyxUnit::SiUnitFlag);
    unitSystem.addBaseUnit("s", PhyxUnit::SiUnitFlag);
    unitSystem.addBaseUnit("mol", PhyxUnit::SiUnitFlag);
    unitSystem.addBaseUnit("cd", PhyxUnit::SiUnitFlag);

    PhyxUnit *newUnit = new PhyxUnit();
    newUnit->setPowers(unitSystem.unit("K")->powers());
    newUnit->setOffset(-273.15);
    newUnit->setSymbol("°C");
    unitSystem.addDerivedUnit(newUnit);
    newUnit = new PhyxUnit();
    newUnit->setPowers(unitSystem.unit("K")->powers());
    newUnit->setOffset(-459.67*(5.0/9.0));
    newUnit->setScaleFactor(5.0/9.0);
    newUnit->setSymbol("°F");
    unitSystem.addDerivedUnit(newUnit);
    newUnit = new PhyxUnit();
    newUnit->setPowers(unitSystem.unit("m")->powers());
    newUnit->setScaleFactor(0.0254);
    newUnit->setOffset(0);
    newUnit->setSymbol("in");
    unitSystem.addDerivedUnit(newUnit);
    newUnit = new PhyxUnit();
    newUnit->setPowers(PhyxUnit::PowerMap());
    newUnit->setScaleFactor(0.01);
    newUnit->setSymbol("%");
    unitSystem.addDerivedUnit(newUnit);

    PhyxVariable *variable3 = new PhyxVariable();
    //variable3->setUnitSystem(&unitSystem);
    variable3->setValue(60);
    variable3->setUnit(new PhyxCompoundUnit(unitSystem.unit("s")));
    variable3->unit()->setUnitSystem(&unitSystem);
    //unitSystem.addDerivedUnit("min", variable3, 0, 0);
    //variable3->setUnit(unitSystem.copyUnit("min"));
    //unitSystem.addDerivedUnit("h", variable3, 0, 0);

    PhyxVariable *variable1 = new PhyxVariable();
    //variable1->setUnitSystem(&unitSystem);
    variable1->setValue(5);
    variable1->setUnit(new PhyxCompoundUnit(unitSystem.unit("m")));
    variable1->unit()->setUnitSystem(&unitSystem);
    qDebug() << "variable1 powers size:" << variable1->unit()->powers().size();
    qDebug() << "variable1 is ProductUnit:" << variable1->unit()->isProductUnit();
    qDebug() << "variable1 is DimensionlessUnit:" << variable1->unit()->isDimensionlessUnit();

    PhyxVariable *variable2 = new PhyxVariable();
    //variable2->setUnitSystem(&unitSystem);
    variable2->setValue(2);
    variable2->setUnit(new PhyxCompoundUnit(unitSystem.copyUnit("kg")));
    variable2->unit()->setUnitSystem(&unitSystem);
    qDebug() << "variable2 powers size:" << variable2->unit()->powers().size();
    qDebug() << "variable2 is ProductUnit:" << variable2->unit()->isProductUnit();
    qDebug() << "variable2 is DimensionlessUnit:" << variable2->unit()->isDimensionlessUnit();

    variable1->mathDiv(variable2);
    variable1->mathDiv(variable3);
    //variable1->convertUnit(unitManager.unit("min"));

    qDebug() << (double)variable1->value();
    qDebug() << variable1->unit()->isOne();
    qDebug() << variable1->unit()->isBaseUnit();
    qDebug() << variable1->unit()->symbol();
    qDebug() << variable1->unit()->dimensionString();

    PhyxCompoundUnit *compoundUnit = new PhyxCompoundUnit();

}
