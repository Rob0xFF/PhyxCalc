#ifndef PHYXUNITMANAGER_H
#define PHYXUNITMANAGER_H

#include <QObject>
#include <QMap>
#include "phyxunit.h"

class PhyxUnitManager : public QObject
{
    Q_OBJECT
public:
    explicit PhyxUnitManager(QObject *parent = 0);

    void addBaseUnit(QString symbol, bool isSiUnit);    /// adds a base unit
    void addDerivedUnit(QString symbol, PhyxUnit::PowerMap powers, double scaleFactor, double offset);
    bool removeUnit(QString symbol);                    /// removes a unit, returns successful
    PhyxUnit * getUnit(QString symbol);                   /// get a unit from its symbol
    
private:
    QMap<QString, bool>     baseUnitsMap;       /// contains all base units -> value: true = SiUnit, key = symbol
    QMap<QString, PhyxUnit*> derivedUnitMap;     /// contains all derived units mapped with their symbol

    void recalculateUnits();
    void recalculateVariables();
    void recalculate();                         /// recalculates Units and Variables
signals:
    
public slots:
    
};

#endif // PHYXUNITMANAGER_H
