#ifndef PHYXUNITSYSTEM_H
#define PHYXUNITSYSTEM_H

#include <QObject>
#include "phyxunit.h"

class PhyxUnitSystem : public QObject
{
    Q_OBJECT
public:
    explicit PhyxUnitSystem(QObject *parent = 0);
    ~PhyxUnitSystem();

    void addBaseUnit(QString symbol, PhyxUnit::UnitFlags flags);    ///< adds a base unit
   // void addDerivedUnit(QString symbol,
   //                     PhyxVariable *variable,
   //                     double offset,
   //                     PhyxUnit::UnitFlags flags);                 ///< adds a derived unit, based on a variable
    void addDerivedUnit(PhyxUnit *unit);                            ///< adds a derived unit
    bool removeUnit(QString symbol);                                ///< removes a unit, returns successful
    void addPrefix(QString symbol, double value);                   ///< adds a prefix
    bool removePrefix(QString symbol);                              ///< removes a prefi, returns successful

    PhyxUnit * copyUnit(QString symbol);                            ///< copys a unit
    PhyxUnit * unit(QString symbol);                                ///< gives back a reference to the unit

    double  prefix(QString symbol);                                 ///< returns the value of a prefix

    bool    verifyUnit(PhyxUnit *unit);                             ///< finds unit in the system and sets all the missing information, return wheter unit was found or not
private:
    QMap<QString, PhyxUnit*>    baseUnitsMap;                       ///< contains all base units mapped with their symbol
    QMap<QString, PhyxUnit*>    derivedUnitsMap;                    ///< contains all derived units mapped with their symbol
    QMap<QString, double>       prefixMap;                          ///< contains all unit prefixes

    void recalculateUnits();                                        ///< recalculates all units
    void recalculateVariables();                                    ///< recalculates all variables
    void recalculate();                                             ///< recalculates units and variables
signals:
    void unitAdded(QString symbol);
    void unitRemoved(QString symol);
    void prefixAdded(QString symbol);
    void prefixRemoved(QString symbol);

public slots:

};

#endif // PHYXUNITSYSTEM_H
