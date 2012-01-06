#ifndef PHYXUNIT_H
#define PHYXUNIT_H

#include <QObject>
#include <QMap>

class PhyxUnit : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString symbol READ symbol WRITE setSymbol)
    Q_PROPERTY(QString name READ name WRITE setName)
    //Q_PROPERTY(UnitType type READ type WRITE setType)
    Q_PROPERTY(double offset READ offset WRITE setOffset)
    Q_PROPERTY(double scaleFactor READ scaleFactor WRITE setScaleFactor)
    Q_PROPERTY(PowerMap powers READ powers WRITE setPowers)
    //Q_PROPERTY(double prefixPower READ prefixPower WRITE setPrefixPower)
    Q_PROPERTY(UnitFlags flags READ flags WRITE setFlags)
    //Q_ENUMS(UnitType)
    Q_FLAGS(UnitFlag UnitFlags)

public:
    explicit PhyxUnit(QObject *parent = 0);

    /** Possible unit types */
    /*enum UnitType {
        DimensionlessUnitType,      /// DimensionlessUnits such as %,°,...
        ProductUnitType,            /// ProductUnits represented only by the exponents of their BaseUnits such as V,Ohm,...
        //OffsetUnitType,           /// OffsetUnits such as °C   (y = x + b)
        GalileanUnitType,           /// GalileanUnits such as °F (y = ax + b), also OffsetUnits such as °C (y = x + b) and ScaledUnits such as min ( y = ax)
        LogarithmicUnitType         /// LogarithmicUnits such as dB (y = a * log(x))
    };*/

    /** Flags for unit handling*/
    enum UnitFlag {
        SiUnitFlag = 0x01,              /// flag for Si-Units, needed to handle Si-Prefixes
        LogarithmicUnitFlag = 0x02,     /// flag for logarithmic units (dB)
        SystemUnitFlag = 0x04           /// flag for units that should be prefered over others (e.g. W > PS)
    };
    Q_DECLARE_FLAGS(UnitFlags, UnitFlag)

    typedef QMap<QString, double> PowerMap;

    void appendPower(QString base, double power);       /// adds a power to the map
    void powerMultiply(QString base, double factor);    /// multiplies a power with factor
    void powerDivide(QString base, double factor);      /// devides a power with factor
    void powersMultiply(PowerMap powers);               /// multiplies powers of the unit with other powers
    void powersDevide(PowerMap powers);                 /// devides powers of the unit with other powers
    void powersRaise(double power);                     /// raises all powers to power
    void powersRoot(double root);                       /// takes the root of all powers
    bool powersCompare(PowerMap powers);                /// compares powers of the unit with other powers and returns ==

    /*void prefixMultiply(double factor);
    void prefixDevide(double factor);
    void prefixRaise(double power);
    void prefixRoot(double root);*/

    bool isOne();                                       ///< returns wheter unit is 1 (no unit) or not
    bool isBaseUnit();                                  ///< returns wheter unit is a BaseUnit or not
    bool isDimensionlessUnit();                         ///< returns wheter unit is a DimensionlessUnit or not
    bool isProductUnit();                               ///< returns wheter unit is a ProductUnit or not
    bool isGalileanUnit();                              ///< returns wheter unit is a GalileanUnit or not

    bool isConvertible(PhyxUnit *unit);                 ///< checks wheter unit convertible to the other unit
    bool isSame(PhyxUnit *unit);                        ///< checks wheter unit is the same as the other unit

    static void copyUnit(PhyxUnit *source, PhyxUnit *destination);

    QString dimensionString() const;                    ///< returns a string with the dimensional representation of the unit (e.g. m^2*kg^-1)

    QString     symbol() const
    {
        return m_symbol;
    }
    QString     name() const
    {
        return m_name;
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
    PowerMap    powers() const
    {
        return m_powers;
    }

private:
    QString     m_symbol;                  /// the symbol of the unit
    QString     m_name;                    /// the name of the unit
    double      m_offset;                  /// offset for OffsetUnit and GalileanUnit
    double      m_scaleFactor;             /// scale factor for GalileanUnit and LogarithmicUnit
    PowerMap    m_powers;                  /// this map holds the powers of the base units
    //double      m_prefixPower;             /// for Si units -> the power of the prefix (10^x)
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
};

#endif // PHYXUNIT_H
