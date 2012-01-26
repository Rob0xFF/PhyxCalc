#ifndef UNITLOADER_H
#define UNITLOADER_H

#include <QObject>
#include <QMap>
#include <QFile>
#include <QTextStream>
#include <QStringList>
#include <QDebug>
#include <cmath>
#include "global.h"

/*struct SIUnit {
    QString measure;
    QStringList symbols;
    QString dimension;
    QString unit;
    QString name;
    double identifier;
};*/

//map index is symbol
/*struct physicalUnit {
    QString quantity;
    QString unit;
    QString formula;
};*/

struct symbolStruct
{
    QString name;
    QString symbol;
};

class UnitLoader : public QObject
{
    Q_OBJECT
public:
    explicit UnitLoader(QObject *parent = 0);

    //QMap<QString, QString> *units() {return &unitMap;}
    QList<symbolStruct>            *symbols() {return &symbolList;}

        //QMap<double, SIUnit>        *siUnits() {return &siUnitMap;}
        //QMap<QString, double>       *siPrefixes() {return &siPrefixMap;}

    bool load();

private:
    //QMap<QString, QString>     unitMap;
    QList<symbolStruct>                symbolList;

    /* loads physcial units and symbols from local file*/
    //bool loadUnits();
    /* loads replacement table for symbols from local file*/
    bool loadSymbols();


    //QMap<QString, double>           siPrefixMap;
    //QMap<double, SIUnit>           siBaseUnitMap;
    //QMap<double, SIUnit>           siUnitMap;

    /* initializes si prefixes*/
    //void loadSiPrefixes();
    /* loads SI base units */
    //bool loadSiBaseUnits();
    /* loads SI units */
    //bool loadSiUnits();

signals:

public slots:

};

#endif // UNITLOADER_H
