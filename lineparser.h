#ifndef LINEPARSER_H
#define LINEPARSER_H

#include <QObject>
#include <QMap>
#include <QList>
#include <QDebug>
#include <QKeyEvent>
#include <QTextBlock>
#include <QTextEdit>
#include <QTableWidgetItem>
#include <QTableWidget>
#include <cmath>
#include "unitloader.h"
#include "global.h"
#include "qearleyparser.h"
#include "phyxcalculator.h"
//#include "phexparser.h"

enum LineType {DefinitionLine, ExpressionLine, CommentLine, OutputLine, EmptyLine, UnknownLine};

class LineParser: public QObject
{
    Q_OBJECT

public:
    explicit LineParser(UnitLoader *loader, QTableWidget *tableWidget, QTextEdit *textEdit, AppSettings *settings);
    ~LineParser();

    void setCalculationEdit(QTextEdit *textEdit) { calculationEdit = textEdit;}
    void setVariableTable(QTableWidget *tableWidget) { variableTable = tableWidget;}
    void setUnitLoader(UnitLoader *loader) { unitLoader = loader;}
    void setAppSettings(AppSettings *settings) { appSettings = settings;}

    void showVariables();
    void parseLine();
    void parseAll();
    void parseFromCurrentPosition();

    void insertNewLine(bool force = false);

    QString exportFormelEditor();

private:
    QTextEdit       *calculationEdit;
    QTableWidget    *variableTable;
    UnitLoader      *unitLoader;
    AppSettings     *appSettings;
    QMap<QString, physicalVariable> variableMap;
    //PhexParser      *phexParser;

    /* Check the type of a line */
    LineType checkLineType(QString text);
    /* Reads a definition line */
    //void readDefinition(QString line);
    /* Reads an expression line */
    //void readExpression(QString line, QString varName = "");
    /* Reads an output line and outputs a variable) */
    //void OutputVariable(QString line, QTextEdit *text);

    /* Replace greek letters written out with the symbols */
    void replaceSymbols();

    /* Text functions */
    void replaceCurrentLine(QString text);
    void insertResult(QString text);
    QString getCurrentLine();

    /* format a double with si prefixes */
    QString formatValue(double value, QString siPrefix = QString());
    /* Gets the unit from the symbol in variable name */
    QString getUnitFromSymbol(QString variableName);


    /* si unit paser functions */
    QString removeWhitespace(QString string);
    /* Writes the expression with numbers and units instead of variables */
    QString replaceVariables(QString expression, bool insertValue = true, bool insertUnit = true);


    /* just for testing the parser */
    void testParser();
public slots:
    void clearAllVariables();
};

#endif // LINEPARSER_H
