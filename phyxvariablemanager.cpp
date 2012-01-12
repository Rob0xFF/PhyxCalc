#include "phyxvariablemanager.h"

PhyxVariableManager::PhyxVariableManager(QObject *parent) :
    QObject(parent)
{
}

void PhyxVariableManager::addVariable(QString name, PhyxVariable *variable)
{
    if (variableMap.contains(name))
        delete variableMap.value(name);

    variableMap.insert(name, variable);
}

PhyxVariable *PhyxVariableManager::getVariable(QString name) const
{
    return variableMap.value(name, NULL);
}

void PhyxVariableManager::removeVariable(QString name)
{
    if (variableMap.contains(name))
    {
        delete variableMap.value(name);
        variableMap.remove(name);
    }
}

PhyxVariableManager::PhyxVariableMap *PhyxVariableManager::variables()
{
    return &variableMap;
}
