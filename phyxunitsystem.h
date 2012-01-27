#ifndef PHYXUNITSYSTEM_H
#define PHYXUNITSYSTEM_H

#include <QObject>
#include <QStringList>
#include "phyxunit.h"

class PhyxUnitSystem : public QObject
{
    Q_OBJECT
public:
    typedef struct PhyxPrefixStruct{
        long double value;          /// the value of the prefix
        QString     unitGroup;      /// the unit system of the prefix
        QString     symbol;         /// the symbol of the prefix
        bool        inputOnly;      /// input only flag, when set this prefix is not used for output

        bool operator < (PhyxPrefixStruct prefix) const
        {
            return value < prefix.value;
        }
    } PhyxPrefix;

    explicit PhyxUnitSystem(QObject *parent = 0);
    ~PhyxUnitSystem();

    void addBaseUnit(QString symbol, PhyxUnit::UnitFlags flags, QString unitGroup, QString preferedPrefix);    ///< adds a base unit
   // void addDerivedUnit(QString symbol,
   //                     PhyxVariable *variable,
   //                     double offset,
   //                     PhyxUnit::UnitFlags flags);                 ///< adds a derived unit, based on a variable
    void addDerivedUnit(PhyxUnit *unit);                            ///< adds a derived unit
    bool removeUnit(QString symbol);                                ///< removes a unit, returns successful
    void addPrefix(QString symbol, double value, QString unitGroup, bool inputOnly = false);///< adds a prefix
    bool removePrefix(QString symbol);                              ///< removes a prefix, returns successful
    void addUnitGroup(QString name);                                ///< adds a unit group
    bool removeUnitGroup(QString name);                             ///< removes a unit group, returns succesful

    PhyxUnit * copyUnit(QString symbol);                            ///< copys a unit
    PhyxUnit * unit(QString symbol);                                ///< gives back a reference to the unit

    PhyxPrefix  prefix(QString symbol, QString unitGroup);          ///< returns the value of a prefix
    QList<PhyxPrefix> prefixes(QString unitGroup) const;                  ///< returns all prefixes for one unitGroup sorted

    bool    verifyUnit(PhyxUnit *unit);                             ///< finds unit in the system and sets all the missing information, return wheter unit was found or not
private:
    QMap<QString, PhyxUnit*>    baseUnitsMap;                       /// contains all base units mapped with their symbol
    QMap<QString, PhyxUnit*>    derivedUnitsMap;                    /// contains all derived units mapped with their symbol
    QMultiMap<QString, PhyxPrefix>   prefixMap;                          /// contains all unit prefixes
    QStringList                 unitGroupsList;                     /// contains all unit groups

    void recalculateUnits();                                        ///< recalculates all units
    void recalculateVariables();                                    ///< recalculates all variables
    void recalculate();                                             ///< recalculates units and variables
signals:
    void unitAdded(QString symbol);
    void unitRemoved(QString symol);
    void prefixAdded(QString symbol);
    void prefixRemoved(QString symbol);
    void unitGroupAdded(QString name);
    void unitGroupRemoved(QString name);

public slots:

};

#endif // PHYXUNITSYSTEM_H
