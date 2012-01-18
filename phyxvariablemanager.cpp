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
    emit variableAdded(name);
}

PhyxVariable *PhyxVariableManager::getVariable(QString name) const
{
    if (variableMap.contains(name))
    {
        PhyxVariable *variable = new PhyxVariable();
        PhyxVariable::copyVariable(variableMap.value(name, NULL), variable);
        return variable;
    }
    else
        return NULL;
}

void PhyxVariableManager::removeVariable(QString name)
{
    if (variableMap.contains(name))
    {
        delete variableMap.value(name);
        variableMap.remove(name);
        emit variableRemoved(name);
    }
}

PhyxVariableManager::PhyxVariableMap *PhyxVariableManager::variables()
{
    return &variableMap;
}

void PhyxVariableManager::addConstant(QString name, PhyxVariable *variable)
{
    if (constantMap.contains(name))
        delete constantMap.value(name);

    constantMap.insert(name, variable);
    emit constantAdded(name);
}

PhyxVariable *PhyxVariableManager::getConstant(QString name) const
{
    if (constantMap.contains(name))
    {
        PhyxVariable *variable = new PhyxVariable();
        PhyxVariable::copyVariable(constantMap.value(name, NULL), variable);
        return variable;
    }
    else
        return NULL;
}

void PhyxVariableManager::removeConstant(QString name)
{
    if (constantMap.contains(name))
    {
        delete constantMap.value(name);
        constantMap.remove(name);
        emit constantRemoved(name);
    }
}

PhyxVariableManager::PhyxVariableMap *PhyxVariableManager::constants()
{
    return &constantMap;
}

void PhyxVariableManager::clearVariables()
{
    QMapIterator<QString, PhyxVariable*> i(variableMap);
    while (i.hasNext())
    {
        i.next();
        QString name = i.key();
        PhyxVariable *variable = i.value();

        variableMap.remove(name);
        emit variableRemoved(name);
        delete variable;
    }
}
