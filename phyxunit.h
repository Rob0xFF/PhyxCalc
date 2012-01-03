#ifndef PHYXUNIT_H
#define PHYXUNIT_H

#include <QObject>
#include <QMap>

class PhyxUnit : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString symbol READ symbol WRITE setSymbol)
    Q_PROPERTY(QString name READ name WRITE setName)
    Q_PROPERTY(UnitType type READ type WRITE setType)
    Q_PROPERTY(double offset READ offset WRITE setOffset)
    Q_PROPERTY(double scaleFactor READ scaleFactor WRITE setScaleFactor)
    Q_PROPERTY(PowerMap powers READ powers WRITE setPowers)
    Q_PROPERTY(double prefixPower READ prefixPower WRITE setPrefixPower)
    Q_PROPERTY(UnitFlags flags READ flags WRITE setFlags)
    Q_ENUMS(UnitType)
    Q_FLAGS(UnitFlag UnitFlags)

public:
    explicit PhyxUnit(QObject *parent = 0);

    /** Possible unit types */
    enum UnitType {
        BaseUnitType,       /// BaseUnits and dimensionless units
        ProductUnit,        /// ProductUnits represented only by the exponents of their BaseUnits
        OffsetUnitType,     /// OffsetUnits such as °C   (y = x + b)
        GalileanUnit,       /// GalileanUnits such as °F (y = ax + b)
        LogarithmicUnit     /// LogarithmicUnits such as dB (y = a * log(x))
    };

    /** Flags for unit handling*/
    enum UnitFlag {
        SiUnitFlag = 0x01         /// flag for Si-Units, needed to handle Si-Prefixes
    //    TimeUnitFlag = 0x02        /// flag for time units (min, h, d), to avoid Si-Prefixes beeing used with them
    };
    Q_DECLARE_FLAGS(UnitFlags, UnitFlag)

    typedef QMap<QString, double> PowerMap;

    void appendPower(QString base, double power);       /// adds a power to the map
    void powerMultiply(QString base, double factor);    /// multiplies a power with factor
    void powerDevide(QString base, double factor);      /// devides a power with factor
    void powersMultiply(PowerMap powers);               /// multiplies powers of the unit with other powers
    void powersDevide(PowerMap powers);                 /// devides powers of the unit with other powers
    void powersRaise(double power);                     /// raises all powers to power
    void powersRoot(double root);                       /// takes the root of all powers
    bool powersCompare(PowerMap powers);                /// compares powers of the unit with oder powers and returns ==

    void prefixMultiply(double factor);
    void prefixDevide(double factor);
    void prefixRaise(double power);
    void prefixRoot(double root);

    bool isDimensionless();                             /// returns wheter the unit is dimensionless or not

    bool operator ==(PhyxUnit unit);

    QString     symbol() const
    {
        return m_symbol;
    }
    QString     name() const
    {
        return m_name;
    }
    UnitType    type() const
    {
        return m_type;
    }
    double      offset() const
    {
        return m_offset;
    }
    double      scaleFactor() const
    {
        return m_scaleFactor;
    }
    UnitFlags   flags() const
    {
        return m_flags;
    }
    PowerMap powers() const
    {
        return m_powers;
    }
    double prefixPower() const
    {
        return m_prefixPower;
    }

private:
    QString     m_symbol;                  /// the symbol of the unit
    QString     m_name;                    /// the name of the unit
    UnitType    m_type;                    /// the type of the unit
    double      m_offset;                  /// offset for OffsetUnit and GalileanUnit
    double      m_scaleFactor;             /// scale factor for GalileanUnit and LogarithmicUnit
    PowerMap    m_powers;                  /// this map holds the powers of the base units
    double      m_prefixPower;             /// for Si units -> the power of the prefix (10^x)
    UnitFlags   m_flags;                   /// flags of the unit

signals:
    
public slots:

void setSymbol(QString arg)
{
    m_symbol = arg;
}
void setName(QString arg)
{
    m_name = arg;
}
void setType(UnitType arg)
{
    m_type = arg;
}
void setOffset(double arg)
{
    m_offset = arg;
}
void setScaleFactor(double arg)
{
    m_scaleFactor = arg;
}
void setFlags(UnitFlags arg)
{
    m_flags = arg;
}
void setPowers(PowerMap arg)
{
    m_powers = arg;
}
void setPrefixPower(double arg)
{
    m_prefixPower = arg;
}
};

#endif // PHYXUNIT_H
