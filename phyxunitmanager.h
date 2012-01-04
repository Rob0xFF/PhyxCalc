#ifndef PHYXUNITMANAGER_H
#define PHYXUNITMANAGER_H

#include <QObject>
#include <QMap>
#include "phyxunit.h"
#include "phyxvariable.h"

class PhyxUnitManager : public QObject
{
    Q_OBJECT
public:
    explicit PhyxUnitManager(QObject *parent = 0);

    void addBaseUnit(QString symbol, PhyxUnit::UnitFlags flags);    ///< adds a base unit
    void addDerivedUnit(QString symbol,
                        PhyxVariable *variable,
                        double offset,
                        PhyxUnit::UnitFlags flags);                 ///< adds a derived unit, based on a variable
    void addDerivedUnit(PhyxUnit *unit);                            ///< adds a derived unit
    bool removeUnit(QString symbol);                                ///< removes a unit, returns successful
    PhyxUnit * copyUnit(QString symbol);                            ///< copys a unit
    PhyxUnit * unit(QString symbol);                                ///< gives back a reference to the unit
    
private:
    QMap<QString, PhyxUnit*>    baseUnitsMap;       ///< contains all base units mapped with their symbol
    QMap<QString, PhyxUnit*>    derivedUnitsMap;     ///< contains all derived units mapped with their symbol

    void recalculateUnits();                        ///< recalculates all units
    void recalculateVariables();                    ///< recalculates all variables
    void recalculate();                             ///< recalculates units and variables
signals:
    
public slots:
    
};

#endif // PHYXUNITMANAGER_H
