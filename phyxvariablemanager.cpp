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

void PhyxVariableManager::clearVariables()
{
    QMapIterator<QString, PhyxVariable*> i(variableMap);
    while (i.hasNext())
    {
        i.next();
        emit variableRemoved(i.key());
        delete i.value();
    }
}
