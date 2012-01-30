/**************************************************************************
**
** This file is part of PhyxCalc.
**
** PhyxCalc is free software: you can redistribute it and/or modify
** it under the terms of the GNU General Public License as published by
** the Free Software Foundation, either version 3 of the License, or
** (at your option) any later version.
**
** PhyxCalc is distributed in the hope that it will be useful,
** but WITHOUT ANY WARRANTY; without even the implied warranty of
** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
** GNU General Public License for more details.
**
** You should have received a copy of the GNU General Public License
** along with PhyxCalc.  If not, see <http://www.gnu.org/licenses/>.
**
***************************************************************************/

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
