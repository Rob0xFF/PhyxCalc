#ifndef PHYXVARIABLE_H
#define PHYXVARIABLE_H

#include <QObject>
#include <QSet>
#include <complex>
#include "phyxunit.h"
#include "phyxcompoundunit.h"

typedef QSet<QString>               userUnitBuffer;         /// set of input units
typedef std::complex<long double>   PhyxValueDataType;      /// the base data type for values

class PhyxVariable : public QObject
{
    Q_OBJECT
    Q_PROPERTY(PhyxValueDataType value READ value WRITE setValue)
    Q_PROPERTY(PhyxCompoundUnit *unit READ unit WRITE setUnit)
    Q_PROPERTY(VariableError error READ error)
    Q_ENUMS(VariableError)

public:
    explicit PhyxVariable(QObject *parent = 0);
    ~PhyxVariable();

    enum outputMode {
        onlyBaseUnits = 0x01,           /// option to convert output units into base units
        minimizeUnit = 0x02,            /// option to use shortest possible output unit, but use input units in case of ambiguity
        forceInputUnits = 0x03          /// option to use only input units as output unit, as far as possible
    };

    enum VariableError {
        UnitsNotConvertibleError,
        UnitNotDimensionlessError,
        ValueNotPositiveError,
        ValueNotIntegerError
    };

    bool convertUnit(PhyxCompoundUnit *unit);
    static void copyVariable(PhyxVariable *source, PhyxVariable *destination);

    QString errorString();                              ///< returns the error string to current error


    void setPreferedUnit(userUnitBuffer userInputUnits, outputMode mode);

    PhyxValueDataType value() const
    {
        return m_value;
    }
    PhyxCompoundUnit * unit() const
    {
        return m_unit;
    }
    VariableError error() const
    {
        return m_error;
    }

private:
    PhyxValueDataType   m_value;
    PhyxCompoundUnit    *m_unit;
    VariableError       m_error;

signals:
    
public slots:

void setValue(PhyxValueDataType arg)
{
    m_value = arg;
}
void setUnit(PhyxCompoundUnit * arg)
{
    m_unit = arg;
    connect(m_unit, SIGNAL(offsetValue(double)),
            this, SLOT(offsetValue(double)));
    connect(m_unit, SIGNAL(scaleValue(double)),
            this, SLOT(scaleValue(double)));
}
void setUnit(PhyxUnit *unit);
void offsetValue(double offset)
{
    m_value += offset;
}
void scaleValue(double scaleFactor)
{
    m_value *= scaleFactor;
}
};

#endif // PHYXVARIABLE_H
