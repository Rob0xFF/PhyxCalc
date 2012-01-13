#include "lineparser.h"

LineParser::LineParser(UnitLoader *loader, QTableWidget *tableWidget, QTextEdit *textEdit, AppSettings *settings)
{
    unitLoader = loader;
    variableTable = tableWidget;
    calculationEdit = textEdit;
    appSettings = settings;

    phyxCalculator = new PhyxCalculator();
}

LineParser::~LineParser()
{

}

LineType LineParser::checkLineType(QString text)
{
    //int equalSignPos;
    //bool isDefinition;

    if (text.trimmed().isEmpty())
        return EmptyLine;

    if (text.trimmed().at(0) == '=')
        return UnknownLine;

    //equalSignPos = text.indexOf('=');

    if (text.indexOf("//") == 0)
        return CommentLine;

    if (text.indexOf("=") != -1)
        return DefinitionLine;

    if (text.indexOf("=") == -1)
        return ExpressionLine;

    return UnknownLine;

    /*if (equalSignPos == 0)
        return UnknownLine;

    //if = is at last position it is clearing a definition
    if (equalSignPos == text.size()-1)
        return DefinitionLine;

    //check for boolean operators
    if ((text.at(equalSignPos-1) == '!') || (text.at(equalSignPos+1) == '='))
        return OutputLine;

    //line is expression or definition
    if (text.indexOf(QRegExp(MATH_OPERATORS), equalSignPos+2) != -1)
        isDefinition = false;
    else
    {
        //check wheter right side is only a variable or not
        if (variableMap.contains(text.mid(equalSignPos+1).trimmed()))
            isDefinition = false;
        else
            isDefinition = true;
    }


    if (isDefinition)
        return DefinitionLine;
    else
        return ExpressionLine;*/
}

/*void LineParser::readExpression(QString line, QString varName)
{
    QString expression,
            outputUnit,
            outputString,
            unit;
    double value;
    int pos;
    bool error = false;

    //check wheter converting is needed or not
    pos = line.indexOf("->");
    if (pos != -1)
    {
        if (line.size() > (pos+2))
            outputUnit=line.mid(pos+2).trimmed();
        expression = line.left(pos).trimmed();
    }
    else
    {
        expression = line.trimmed();
    }

    //check wheter expression is only a variable or not
    if (variableMap.contains(expression))
    {
        value = variableMap.value(expression).value;
        unit = variableMap.value(expression).unit;
    }
    else
    {
        //search for the next character that is not a whitespace, number, dot or minus
        for ( int linePos = 0; linePos < expression.size(); linePos++)
        {
            if (!(expression.at(linePos).isNumber()
                  || expression.at(linePos).isPunct()
                  || (expression.at(linePos) == '-')
                  || expression.at(linePos).isSpace()))
            {
                unit = expression.mid(linePos).trimmed();
                break;
            }
        }
        qDebug() << "Detected unit:" << unit;

        if (unit.isEmpty())
        {
            value = expression.toDouble();
            unit = "";
        }
        else
        {
            phexParser->setExpression(replaceVariables(expression));

            if (phexParser->evaluate())
            {
                value = phexParser->resultValue();

                if (unit != phexParser->resultUnit())
                {
                   if (!phexParser->convert(phexParser->resultUnit(), phexParser->resultValue(), unit, &value))
                       unit = phexParser->resultUnit();
                }
                //unit = phexParser->resultUnit();
            }
            else
            {
                outputString.append(tr("Syntax error"));//tr("<span style='color:red'>Syntax error</span>"));
                error = true;
            }
        }
    }

    if (!error && !outputUnit.isEmpty() && !unit.isEmpty())
    {
        if (!phexParser->convert(unit, value, outputUnit, &value))
        {
            outputString.append(tr("Conversion not possible"));
            error = true;
        }
        else
        {
            unit = outputUnit;

            if (variableMap.contains(expression))
            {
                variableMap[expression].unit = unit;
                variableMap[expression].value = value;
            }
        }
    }

    if (!error)
    {
        if (appSettings->lineParser.expression.outputWithNumbers)
        {
            QString expressionWithNumbers = replaceVariables(expression);
            if (expressionWithNumbers != expression)
                insertResult(expressionWithNumbers);
        }

        if (varName.isEmpty() || appSettings->lineParser.expression.outputResult)
        {
            outputString.append(formatValue(value));
            outputString.append(unit);

            if (outputString != expression)
                insertResult(outputString);
        }

        if (!varName.isEmpty())
        {
            variableMap[varName].value = value;
            variableMap[varName].unit = unit;
        }
    }
    else
        insertResult(outputString);
}

void LineParser::readDefinition(QString line)
{
    int equalSignPos;
    //double value;
    QString variableName,
            expression;

    //get the position of =, left side is variable name
    equalSignPos = line.indexOf("=");
    variableName = line.left(equalSignPos).trimmed();

    //if right side is empty clear variable
    if (equalSignPos == line.size()-1)
    {
        variableMap.remove(variableName);
        return;
    }
    else
    {
        expression = line.mid(equalSignPos+1).trimmed();
    }

    qDebug() << "Variable name:" << variableName << "Expression:" << expression;

    readExpression(expression, variableName);
}*/

void LineParser::parseLine()
{
    //replace greek units
    replaceSymbols();

    //read current line
    QString curLineText = getCurrentLine();

    phyxCalculator->setExpression(curLineText);

    if (phyxCalculator->evaluate())
    {
        QString output;
        output.append(PhyxCalculator::complexToString(phyxCalculator->resultValue()));
        output.append(phyxCalculator->resultUnit());

        insertResult(output);
    }
    else
        insertResult(phyxCalculator->errorString());

    /*LineType lineType = checkLineType(curLineText);

    if (lineType == DefinitionLine)
    {
        readDefinition(curLineText);
        showVariables();
    }
    else if (lineType == ExpressionLine)
    {
        readExpression(curLineText);
        showVariables();
    }*/
    /*else if (lineType == OutputLine)
    {
        OutputVariable(curLineText, calculationEdit);
    }*/

    insertNewLine();
}

void LineParser::parseAll()
{
    QTextCursor textCursor = calculationEdit->textCursor();
    textCursor.movePosition(QTextCursor::Start, QTextCursor::MoveAnchor);
    calculationEdit->setTextCursor(textCursor);

    parseFromCurrentPosition();
}

void LineParser::parseFromCurrentPosition()
{
    while (!calculationEdit->textCursor().atEnd())
        parseLine();
}

void LineParser::replaceSymbols()
{
    QString curLineText,
            oldLine;

    curLineText = getCurrentLine();
    oldLine = curLineText;

    for (int i = 0; i < unitLoader->symbols()->size(); i++)
    {
        curLineText.replace(unitLoader->symbols()->at(i).name, unitLoader->symbols()->at(i).symbol, Qt::CaseSensitive);
    }

    //if line not unchanged replace current line
    if (curLineText != oldLine)
    {
        replaceCurrentLine(curLineText);
    }
}

QString LineParser::replaceVariables(QString expression, bool insertValue, bool insertUnit)
{
    QMapIterator<QString, physicalVariable> i(variableMap);

    //get the size of the biggest item
    int maxSize = 0;
    while (i.hasNext()) {
        i.next();
        if (i.key().size() > maxSize)
            maxSize = i.key().size();
    }
    i.toFront();

    //replace vars, beginning with the biggest
    for (int j = maxSize; j > 0; j--)
    {
        while (i.hasNext()) {
             i.next();
             if (i.key().size() == j) {
                 QString variableName = i.key();
                 int pos = expression.indexOf(variableName);
                 if (pos != -1)
                 {
                    QString variableValue;
                    if (insertValue)
                        variableValue.append(formatValue(i.value().value));
                    if (insertUnit)
                        variableValue.append(i.value().unit);
                    expression.replace(variableName, variableValue);
                 }
             }
         }
        i.toFront();
    }

    return expression;
}

QString LineParser::removeWhitespace(QString string)
{
    string = string.simplified();
    string.replace(" ", "");
    return string;
}

QString LineParser::getCurrentLine()
{
    return calculationEdit->textCursor().block().text().trimmed();
}

void LineParser::replaceCurrentLine(QString text)
{
    QTextCursor textCursor = calculationEdit->textCursor();
    textCursor.clearSelection();
    textCursor.movePosition(QTextCursor::EndOfBlock, QTextCursor::MoveAnchor);
    textCursor.movePosition(QTextCursor::StartOfBlock, QTextCursor::KeepAnchor);
    textCursor.removeSelectedText();
    textCursor.insertText(text);
    textCursor.movePosition(QTextCursor::EndOfBlock, QTextCursor::MoveAnchor);
    calculationEdit->setTextCursor(textCursor);
}

void LineParser::insertResult(QString text)
{
    QTextCursor textCursor = calculationEdit->textCursor();

    text.prepend("=");

    textCursor.clearSelection();
    textCursor.movePosition(QTextCursor::EndOfBlock, QTextCursor::MoveAnchor);
    if (textCursor.atEnd())
        textCursor.insertText("\n");
    else
    {
        textCursor.movePosition(QTextCursor::NextBlock, QTextCursor::MoveAnchor);
        textCursor.movePosition(QTextCursor::Right, QTextCursor::KeepAnchor);
        if (textCursor.selectedText() == "=")
        {
            textCursor.movePosition(QTextCursor::EndOfBlock, QTextCursor::KeepAnchor);
            textCursor.removeSelectedText();
        }
        else if (textCursor.atBlockStart())
        {
            textCursor.movePosition(QTextCursor::Left, QTextCursor::KeepAnchor);
            textCursor.removeSelectedText();
        }
        else
        {
            textCursor.movePosition(QTextCursor::PreviousBlock, QTextCursor::MoveAnchor);
            textCursor.movePosition(QTextCursor::EndOfBlock, QTextCursor::MoveAnchor);
            textCursor.insertText("\n");
        }
    }

    //textCursor.insertText(outputString);
    textCursor.insertHtml(text);
    calculationEdit->setTextCursor(textCursor);
}

void LineParser::insertNewLine(bool force)
{
    QTextCursor textCursor = calculationEdit->textCursor();

    textCursor.movePosition(QTextCursor::EndOfBlock, QTextCursor::MoveAnchor);

    if (textCursor.atEnd() || force)
        textCursor.insertText("\n");
    else
        textCursor.movePosition(QTextCursor::NextBlock, QTextCursor::MoveAnchor);


    calculationEdit->setTextCursor(textCursor);
}

QString LineParser::formatValue(double value, QString siPrefix)
{
    //int     x;
    double  outputValue;
    //        scalingFactor;

    /*if (siPrefix.isEmpty() || (!unitLoader->siPrefixes()->contains(siPrefix)))
    {
        for (x = 24; x > -25; x=x-3)
        {
            outputValue = value / pow(10,x);
            if ((outputValue >= 1) || (outputValue*-1 >= 1))
                break;
        }

        if (x == 0)
            siPrefix = "";
        else
        {
            QMapIterator<QString, double> i(*unitLoader->siPrefixes());
            while (i.hasNext())
            {
                i.next();
                if (pow(10,x) == i.value())
                {
                    siPrefix = i.key();
                    break;
                }
            }
        }
    }
    else
    {
        scalingFactor = unitLoader->siPrefixes()->value(siPrefix, 1);
        outputValue = value / scalingFactor;
    }*/
    outputValue = value;

    return QString::number(outputValue, appSettings->output.numbers.format, appSettings->output.numbers.decimalPrecision) + siPrefix;
}

QString LineParser::getUnitFromSymbol(QString variableName)
{
    int pos;

    //check for DELTA
    if (QString(variableName.at(0)) == QString::fromLocal8Bit("Δ"))
        pos = 1;
    else
        pos = 0;

    return unitLoader->units()->value(variableName.at(pos));
}

QString LineParser::exportFormelEditor()
{
    QString text;
    QStringList textLines;
    int pos,
        depth;
    QRegExp regExp;

    text = calculationEdit->toPlainText().trimmed();

    //append results to the end of line
    textLines = text.split('\n');
    for (int i = 0; i < textLines.size(); i++)
    {
        pos = textLines.at(i).indexOf("->");          //"->" löschen
        if (pos != -1)
            textLines[i] = textLines.at(i).left(pos);
    }
    for (int i = textLines.size()-1; i > -1; i--)
    {
        if (!textLines.at(i).isEmpty() && textLines.at(i).at(0) == '=')
        {
            textLines[i-1].append(textLines.at(i));
            textLines.removeAt(i);
        }
    }
    text = textLines.join("\n");

    //add the _ for physical units +++ must be reworked
    /*QMapIterator<QString, QString> j(*unitLoader->units());
     while (j.hasNext()) {
         j.next();
         //regExp.setPattern(tr("[^A-Z]%1[^ =\n]").arg(j.key()));
         int pos = text.indexOf(j.key());//regExp);
         while (pos != -1)
         {
             //check for all kinds of cases where formating is not wished
             if (pos+j.key().size() < text.size()
                     && (!text.at(pos+j.key().size()).isSpace())
                     && (QString(text.at(pos+j.key().size())).indexOf(QRegExp(MATH_OPERATORS)) == -1))
             {
                 if ((pos == 0) || ((text.at(pos-1) != '_')
                                    && (!unitLoader->units()->contains(text.at(pos-1)))
                                    && (!text.at(pos-1).isNumber())))
                 {
                     text.insert(pos+j.key().size(), '_');
                 }
             }
             pos = text.indexOf(j.key(),pos+1);
         }
     }*/


    QMapIterator<QString, physicalVariable> mapIterator(variableMap);
     //get the size of the biggest item
     int maxSize = 0;
     while (mapIterator.hasNext()) {
         mapIterator.next();
         if (mapIterator.key().size() > maxSize)
             maxSize = mapIterator.key().size();
     }
     mapIterator.toFront();

     //replace vars, beginning with the biggest
     for (int j = maxSize; j > 1; j--)
     {
         while (mapIterator.hasNext()) {
              mapIterator.next();
              if (mapIterator.key().size() == j) {
                  QString variableName = mapIterator.key();
                  variableName.insert(1,"_\"");
                  variableName.append('"');

                  text.replace(mapIterator.key(), variableName);
              }
          }
         mapIterator.toFront();
     }

    //search for / and replace it with regexp
    regExp.setPattern("[)][^)]*[/][^(]*[(]");
    pos = text.indexOf(regExp);
    while (pos != -1)
    {
        depth = 0;
        for (int i = pos; i >= 0; i--)
        {
            if (text.at(i) == ')')
            {
                if (depth == 0)
                    text[i] = '}';

                depth++;
            }
            else if (text.at(i) == '(')
            {
                if (depth == 1)
                {
                    text[i] = '{';
                    break;
                }
                else
                    depth--;
            }
        }
        depth = 0;
        for (int i = pos+1; i < text.size(); i++)
        {
            if (text.at(i) == '(')
            {
                if (depth == 0)
                    text[i] = '{';

                depth++;
            }
            else if (text.at(i) == ')')
            {
                if (depth == 1)
                {
                    text[i] = '}';
                    break;
                }
                else
                    depth--;
            }
        }

        pos = text.indexOf(regExp, pos+1);
    }

    //complete it with replacing /
    text.replace(QRegExp("[ ]*[/][ ]*"), " over ");

    //replace greek letters with the formel edit ones
    for (int k = 0; k < unitLoader->symbols()->size(); k++)
    {
        text.replace(unitLoader->symbols()->at(k).symbol, "%" + unitLoader->symbols()->at(k).name + " ", Qt::CaseSensitive);
    }

    //Append the formel editor newline to every line
    textLines = text.split('\n');
    for (int i = 0; i < textLines.size(); i++)
    {
        if (i != textLines.size()-1)
        {
            textLines[i].append(" newline");
            textLines[i] = textLines.at(i).trimmed();
        }
    }
    text = textLines.join("\n");

    return text;
}

void LineParser::showVariables()
{
    int row = 0;
    QTableWidgetItem *newItem;

    variableTable->clearContents();
    variableTable->setRowCount(0);


    QMapIterator<QString, physicalVariable> i(variableMap);
     while (i.hasNext()) {
         i.next();

         variableTable->setRowCount(row+1);
         newItem = new QTableWidgetItem(i.key());
         variableTable->setItem(row, 0, newItem);
         newItem = new QTableWidgetItem(formatValue(i.value().value));//QString::number(i.value().value, numberFormat, numberPrecision));
         variableTable->setItem(row, 1, newItem);
         newItem = new QTableWidgetItem(i.value().unit);
         variableTable->setItem(row, 2, newItem);

         row++;
     }
}

void LineParser::clearAllVariables()
{
    variableMap.clear();
    showVariables();
}
