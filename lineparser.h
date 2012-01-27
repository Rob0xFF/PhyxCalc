#ifndef LINEPARSER_H
#define LINEPARSER_H

#include <QObject>
#include <QMap>
#include <QList>
#include <QDebug>
#include <QTextBlock>
#include <QTextEdit>
#include <QTableWidgetItem>
#include <QTableWidget>
#include "unitloader.h"
#include "global.h"
#include "phyxcalculator.h"

class LineParser: public QObject
{
    Q_OBJECT
    Q_PROPERTY(QTextEdit *calculationEdit READ calculationEdit WRITE setCalculationEdit)
    Q_PROPERTY(QTableWidget *variableTable READ variableTable WRITE setVariableTable)
    Q_PROPERTY(QTableWidget *constantsTable READ constantsTable WRITE setConstantsTable)
    Q_PROPERTY(AppSettings *appSettings READ appSettings WRITE setAppSettings)
    Q_PROPERTY(UnitLoader *unitLoader READ unitLoader WRITE setUnitLoader)
    Q_PROPERTY(PhyxCalculator *phyxCalculator READ phyxCalculator)

public:
    explicit LineParser(QObject * = 0);
    ~LineParser();

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
    PhyxCalculator * phyxCalculator() const
    {
        return m_phyxCalculator;
    }

private:
    QTextEdit       *m_calculationEdit;
    QTableWidget    *m_variableTable;
    QTableWidget    *m_constantsTable;
    AppSettings     *m_appSettings;
    UnitLoader      *m_unitLoader;
    PhyxCalculator  *m_phyxCalculator;

    /* Replace greek letters written out with the symbols */
    void replaceSymbols();

    /* Text functions */
    void replaceCurrentLine(QString text);
    void insertOutput(QString text);
    QString getCurrentLine();

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
        showConstants();
        showVariables();
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
