#include "phyxtesting.h"

PhyxTesting::PhyxTesting(QObject *parent) :
    QObject(parent)
{
}

void PhyxTesting::testUnits()
{
    PhyxUnitSystem unitSystem;

    qDebug() << "Setting SI base units...";
    unitSystem.addBaseUnit("m", PhyxUnit::SiUnitFlag);
    unitSystem.addBaseUnit("kg", PhyxUnit::SiUnitFlag);
    unitSystem.addBaseUnit("A", PhyxUnit::SiUnitFlag);
    unitSystem.addBaseUnit("K", PhyxUnit::SiUnitFlag);
    unitSystem.addBaseUnit("s", PhyxUnit::SiUnitFlag);
    unitSystem.addBaseUnit("mol", PhyxUnit::SiUnitFlag);
    unitSystem.addBaseUnit("cd", PhyxUnit::SiUnitFlag);

    qDebug() << "Adding °C to unit system";
    PhyxUnit *newUnit = new PhyxUnit();
    newUnit->setPowers(unitSystem.unit("K")->powers());
    newUnit->setOffset(-273.15);
    newUnit->setSymbol("°C");
    unitSystem.addDerivedUnit(newUnit);

    qDebug() << "Adding °F to unit system";
    newUnit = new PhyxUnit();
    newUnit->setPowers(unitSystem.unit("K")->powers());
    newUnit->setOffset(-459.67*(5.0/9.0));
    newUnit->setScaleFactor(5.0/9.0);
    newUnit->setSymbol("°F");
    unitSystem.addDerivedUnit(newUnit);

    qDebug() << "Adding in to unit system";
    newUnit = new PhyxUnit();
    newUnit->setPowers(unitSystem.unit("m")->powers());
    newUnit->setScaleFactor(0.0254);
    newUnit->setSymbol("in");
    unitSystem.addDerivedUnit(newUnit);

    qDebug() << "Adding % to unit system";
    newUnit = new PhyxUnit();
    newUnit->setPowers(PhyxUnit::PowerMap());
    newUnit->setScaleFactor(0.01);
    newUnit->setSymbol("%");
    unitSystem.addDerivedUnit(newUnit);

    qDebug() << "Defining variable1: 100m";
    PhyxVariable *variable1 = new PhyxVariable();
    variable1->unit()->setUnitSystem(&unitSystem);
    variable1->setValue(100);
    variable1->setUnit(unitSystem.unit("m"));

    qDebug() << "convert to in:";
    variable1->convertUnit(new PhyxCompoundUnit(unitSystem.unit("in")));
    qDebug() << (double)variable1->value() << variable1->unit()->symbol();


    qDebug() << "Defining variable2: 50°C";
    PhyxVariable *variable2 = new PhyxVariable();
    variable2->unit()->setUnitSystem(&unitSystem);
    variable2->setValue(50);
    variable2->setUnit(unitSystem.unit("°C"));

    qDebug() << "Defining variable3: 23K";
    PhyxVariable *variable3 = new PhyxVariable();
    variable3->unit()->setUnitSystem(&unitSystem);
    variable3->setValue(23);
    variable3->setUnit(unitSystem.unit("K"));

    qDebug() << "variable2 + variable3 =";
    variable2->mathAdd(variable3);
    qDebug() << (double)variable2->value() << variable2->unit()->symbol();

    qDebug() << "Defining variable4: 60s";
    PhyxVariable *variable4 = new PhyxVariable();
    variable4->unit()->setUnitSystem(&unitSystem);
    variable4->setValue(60);
    variable4->setUnit(unitSystem.unit("s"));

    qDebug() << "Define variable4 as unit min";
    newUnit = new PhyxUnit();
    newUnit->setPowers(variable4->unit()->powers());
    newUnit->setScaleFactor(variable4->value());
    newUnit->setSymbol("min");
    unitSystem.addDerivedUnit(newUnit);

    qDebug() << "Defining variable5: 4";
    PhyxVariable *variable5 = new PhyxVariable();
    variable5->unit()->setUnitSystem(&unitSystem);
    variable5->setValue(5);


    qDebug() << "variable4 * variable5 =";
    variable4->mathMul(variable5);
    qDebug() << (double)variable4->value() << variable4->unit()->symbol();

    qDebug() << "convert variable4 to min:";
    if (!variable4->convertUnit(new PhyxCompoundUnit(unitSystem.unit("min"))))
        qDebug() << variable4->errorString();
    else
        qDebug() << (double)variable4->value() << variable4->unit()->symbol();

    qDebug() << "variable1 = 120m/s";
    variable1->setValue(120);
    variable1->setUnit(unitSystem.unit("m"));
    variable2->setValue(1);
    variable2->setUnit(unitSystem.unit("s"));
    variable1->mathDiv(variable2);
    qDebug() << (double)variable1->value() << variable1->unit()->symbol();

    qDebug() << "variable1 * variable4 =";
    variable1->mathMul(variable4);
    qDebug() << (double)variable1->value() << variable1->unit()->symbol();
}
