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
#include "phyxcalculator.h"
//#include "phexparser.h"

//enum LineType {DefinitionLine, ExpressionLine, CommentLine, OutputLine, EmptyLine, UnknownLine};

class LineParser: public QObject
{
    Q_OBJECT
    Q_PROPERTY(QTextEdit *calculationEdit READ calculationEdit WRITE setCalculationEdit)
    Q_PROPERTY(QTableWidget *variableTable READ variableTable WRITE setVariableTable)
    Q_PROPERTY(QTableWidget *constantsTable READ constantsTable WRITE setConstantsTable)
    Q_PROPERTY(AppSettings *appSettings READ appSettings WRITE setAppSettings)
    Q_PROPERTY(UnitLoader *unitLoader READ unitLoader WRITE setUnitLoader)

public:
    explicit LineParser(QObject *parent = 0);
    ~LineParser();

    //void setCalculationEdit(QTextEdit *textEdit) { calculationEdit = textEdit;}
    //void setVariableTable(QTableWidget *tableWidget) { variableTable = tableWidget;}
    //void setUnitLoader(UnitLoader *loader) { unitLoader = loader;}
    //void setAppSettings(AppSettings *settings) { appSettings = settings;}

    void parseLine();
    void parseAll();
    void parseFromCurrentPosition();

    void insertNewLine(bool force = false);
    void deleteLine();
    bool resultLineSelected();


    QString exportFormelEditor();

    QTextEdit * calculationEdit() const
    {
        return m_calculationEdit;
    }
    QTableWidget * variableTable() const
    {
        return m_variableTable;
    }
    AppSettings * appSettings() const
    {
        return m_appSettings;
    }
    QTableWidget * constantsTable() const
    {
        return m_constantsTable;
    }
    UnitLoader * unitLoader() const
    {
        return m_unitLoader;
    }

private:
    //QTextEdit       *calculationEdit;
    //QTableWidget    *variableTable;
    //UnitLoader      *unitLoader;
    //AppSettings     *appSettings;
    //QMap<QString, physicalVariable> variableMap;
    QTextEdit       *m_calculationEdit;
    QTableWidget    *m_variableTable;
    QTableWidget    *m_constantsTable;
    AppSettings     *m_appSettings;
    UnitLoader      *m_unitLoader;
    PhyxCalculator  *phyxCalculator;

    /* Check the type of a line */
    //LineType checkLineType(QString text);
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
    void insertOutput(QString text);
    QString getCurrentLine();

    /* format a double with si prefixes */
    //QString formatValue(double value, QString siPrefix = QString());
    /* Gets the unit from the symbol in variable name */
    //QString getUnitFromSymbol(QString variableName);


    /* si unit paser functions */
    QString removeWhitespace(QString string);
    /* Writes the expression with numbers and units instead of variables */
    QString replaceVariables(QString expression, bool insertValue = true, bool insertUnit = true);

public slots:
    void showVariables();
    void showConstants();
    void clearAllVariables();
    void outputResult();
    void outputError();

    void setCalculationEdit(QTextEdit * arg)
    {
        m_calculationEdit = arg;
    }
    void setVariableTable(QTableWidget * arg)
    {
        m_variableTable = arg;
    }
    void setAppSettings(AppSettings * arg)
    {
        m_appSettings = arg;
    }
    void setConstantsTable(QTableWidget * arg)
    {
        m_constantsTable = arg;
    }
    void setUnitLoader(UnitLoader * arg)
    {
        m_unitLoader = arg;
    }
};

#endif // LINEPARSER_H
