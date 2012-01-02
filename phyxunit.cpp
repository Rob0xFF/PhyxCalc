#include "phyxunit.h"

PhyxUnit::PhyxUnit(QObject *parent) :
    QObject(parent)
{
}

void PhyxUnit::addPower(QString base, double power)
{
    powers.insert(base, power);
}
