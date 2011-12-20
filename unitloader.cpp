#include "unitloader.h"

UnitLoader::UnitLoader(QObject *parent) :
    QObject(parent)
{
}

bool UnitLoader::load()
{
    //loadSiPrefixes();
    return (loadUnits() && loadSymbols());
}

bool UnitLoader::loadUnits()
{
    QFile file("./settings/units.txt");
    if (file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        QTextStream in(&file);
        while (!in.atEnd())
        {
            QString     symbol,
                        unit;
            QStringList list;

            //read one line and split it into pieces
            list = in.readLine().split(";");

            symbol = list.at(0);
            unit = list.at(1);

            //more than one symbol is possible, so split it up
            QStringList symbolList = symbol.split(",");
            for (int i = 0; i < symbolList.size(); i++)
            {
                unitMap[symbolList.at(i)] = unit;
            }
        }

        file.close();
        return true;
    }
    else
        return false;
}

bool UnitLoader::loadSymbols()
{
    QFile file("./settings/symbols.txt");
    if (file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        QTextStream in(&file);
        while (!in.atEnd())
        {
            QStringList         list,
                                nameList;
            struct symbolStruct    tempUnit;

            //read one line and split it into pieces
            list = in.readLine().split(";");
            nameList = list.at(0).split(",");

            for (int i = 0; i < nameList.size(); i++)
            {
                tempUnit.name = nameList.at(i);
                tempUnit.symbol = list.at(1);
                symbolList.append(tempUnit);
            }
        }

        file.close();

        //sort by length of name to avoid problems with replacement
        int maxSize = 0;
        for (int i = 0; i < symbolList.size(); i++)
        {
            if (symbolList.at(i).name.size() > maxSize)
                maxSize = symbolList.at(i).name.size();
        }

        for (int i = 1; i <= maxSize; i++)
        {
            for (int j = 0; j < symbolList.size(); j++)
            {
                if (symbolList.at(j).name.size() == i)
                {
                    symbolList.move(j,0);
                }
            }
        }

        return true;
    }
    else
        return false;
}
