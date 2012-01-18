#ifndef PHYXVARIABLEMANAGER_H
#define PHYXVARIABLEMANAGER_H

#include <QObject>
#include "phyxvariable.h"

class PhyxVariableManager : public QObject
{
    Q_OBJECT

public:
    typedef QMap<QString, PhyxVariable*> PhyxVariableMap;

    explicit PhyxVariableManager(QObject *parent = 0);

    void addVariable(QString name, PhyxVariable *variable);
    PhyxVariable * getVariable(QString name) const;
    void removeVariable(QString name);
    PhyxVariableMap * variables();
    void addConstant(QString name, PhyxVariable *variable);
    PhyxVariable * getConstant(QString name) const;
    void removeConstant(QString name);
    PhyxVariableMap * constants();

private:
    PhyxVariableMap variableMap;
    PhyxVariableMap constantMap;
    
signals:
    void variableAdded(QString name);
    void variableRemoved(QString name);
    void constantAdded(QString name);
    void constantRemoved(QString name);
    
public slots:
    void clearVariables();
    
};

#endif // PHYXVARIABLEMANAGER_H
