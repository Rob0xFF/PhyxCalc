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
    variable3->unit()->setUnitSystem(&unitSystem);
    variable3->setValue(60);
    variable3->setUnit(unitSystem.unit("s"));
    newUnit = new PhyxUnit();
    newUnit->setPowers(variable3->unit()->powers());
    newUnit->setScaleFactor(variable3->value());
    newUnit->setSymbol("min");
    unitSystem.addDerivedUnit(newUnit);
    //unitSystem.addDerivedUnit("min", variable3, 0, 0);
    //variable3->setUnit(unitSystem.copyUnit("min"));
    //unitSystem.addDerivedUnit("h", variable3, 0, 0);

    PhyxVariable *variable1 = new PhyxVariable();
    variable1->unit()->setUnitSystem(&unitSystem);
    variable1->setValue(100);
    variable1->setUnit(unitSystem.unit("m"));
    qDebug() << "variable1 powers size:" << variable1->unit()->powers().size();
    qDebug() << "variable1 is ProductUnit:" << variable1->unit()->isProductUnit();
    qDebug() << "variable1 is DimensionlessUnit:" << variable1->unit()->isDimensionlessUnit();

    PhyxVariable *variable2 = new PhyxVariable();
    variable2->unit()->setUnitSystem(&unitSystem);
    variable2->setValue(50);
    variable2->setUnit(unitSystem.unit("m"));
    qDebug() << "variable2 powers size:" << variable2->unit()->powers().size();
    qDebug() << "variable2 is ProductUnit:" << variable2->unit()->isProductUnit();
    qDebug() << "variable2 is DimensionlessUnit:" << variable2->unit()->isDimensionlessUnit();

    variable1->mathDiv(variable2);
    variable1->mathMul(variable3);

    variable2->setValue(2);
    variable2->setUnit(new PhyxUnit());

    qDebug() << (double)variable1->value();
    qDebug() << variable1->unit()->isOne();
    qDebug() << variable1->unit()->isBaseUnit();
    qDebug() << variable1->unit()->symbol();
    qDebug() << variable1->unit()->dimensionString();

    if (!variable1->mathAdd(variable3))
        qDebug() << variable1->errorString();
    else
        qDebug() << (double)variable1->value() << variable1->unit()->symbol();

    if (!variable1->convertUnit(new PhyxCompoundUnit(unitSystem.unit("min"))))
        qDebug() << variable1->errorString();
    else
        qDebug() << (double)variable1->value() << variable1->unit()->symbol();
}
