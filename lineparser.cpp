#include "lineparser.h"

LineParser::LineParser(QObject *)
{
    m_phyxCalculator = new PhyxCalculator();
    connect(m_phyxCalculator, SIGNAL(outputResult()),
            this, SLOT(outputResult()));
    connect(m_phyxCalculator, SIGNAL(outputError()),
            this, SLOT(outputError()));
    connect(m_phyxCalculator, SIGNAL(variablesChanged()),
            this, SLOT(showVariables()));
    connect(m_phyxCalculator, SIGNAL(constantsChanged()),
            this, SLOT(showConstants()));
}

LineParser::~LineParser()
{

}

void LineParser::parseLine()
{
    //replace greek units
    replaceSymbols();

    //read current line
    QString curLineText = getCurrentLine();

    if (!curLineText.isEmpty() && !(curLineText.at(0) == '='))
    {
        m_phyxCalculator->setExpression(curLineText);
        m_phyxCalculator->evaluate();
    }

    insertNewLine();
}

void LineParser::parseAll()
{
    QTextCursor textCursor = m_calculationEdit->textCursor();
    textCursor.movePosition(QTextCursor::Start, QTextCursor::MoveAnchor);
    m_calculationEdit->setTextCursor(textCursor);

    parseFromCurrentPosition();
}

void LineParser::parseFromCurrentPosition()
{
    while (!m_calculationEdit->textCursor().atEnd())
        parseLine();
}

void LineParser::replaceSymbols()
{
    QString curLineText,
            oldLine;

    curLineText = getCurrentLine();
    oldLine = curLineText;

    for (int i = 0; i < m_unitLoader->symbols()->size(); i++)
    {
        curLineText.replace(m_unitLoader->symbols()->at(i).name, m_unitLoader->symbols()->at(i).symbol, Qt::CaseSensitive);
    }

    //if line not unchanged replace current line
    if (curLineText != oldLine)
    {
        replaceCurrentLine(curLineText);
    }
}

QString LineParser::getCurrentLine()
{
    return m_calculationEdit->textCursor().block().text().trimmed();
}

bool LineParser::resultLineSelected()
{
    QString curLineText = getCurrentLine();

    if (!curLineText.isEmpty() && (curLineText.at(0) == '='))
        return true;
    else
        return false;
}

void LineParser::replaceCurrentLine(QString text)
{
    QTextCursor textCursor = m_calculationEdit->textCursor();
    textCursor.clearSelection();
    textCursor.movePosition(QTextCursor::EndOfBlock, QTextCursor::MoveAnchor);
    textCursor.movePosition(QTextCursor::StartOfBlock, QTextCursor::KeepAnchor);
    textCursor.removeSelectedText();
    textCursor.insertText(text);
    textCursor.movePosition(QTextCursor::EndOfBlock, QTextCursor::MoveAnchor);
    m_calculationEdit->setTextCursor(textCursor);
}

void LineParser::insertOutput(QString text)
{
    QTextCursor textCursor = m_calculationEdit->textCursor();

    textCursor.clearSelection();
    textCursor.movePosition(QTextCursor::EndOfBlock, QTextCursor::MoveAnchor);
    if (textCursor.atEnd())
        textCursor.insertText("\n");
    else
    {
        textCursor.movePosition(QTextCursor::NextBlock, QTextCursor::MoveAnchor);       // move cursor to the next line
        m_calculationEdit->setTextCursor(textCursor);
        QString curLineText = getCurrentLine();

        if (resultLineSelected())   //handle result line
        {
            deleteLine();
            insertNewLine(true);
        }
        else if(!curLineText.isEmpty()) //handle any other line
        {
            textCursor.movePosition(QTextCursor::PreviousBlock, QTextCursor::MoveAnchor);
            m_calculationEdit->setTextCursor(textCursor);
            insertNewLine(true);
        }
        textCursor = m_calculationEdit->textCursor();
    }

    //textCursor.insertText(outputString);
    textCursor.insertHtml(text);
    m_calculationEdit->setTextCursor(textCursor);
}

void LineParser::insertNewLine(bool force)
{
    QTextCursor textCursor = m_calculationEdit->textCursor();

    textCursor.movePosition(QTextCursor::EndOfBlock, QTextCursor::MoveAnchor);

    if (textCursor.atEnd() || force)
        textCursor.insertText("\n");
    else
        textCursor.movePosition(QTextCursor::NextBlock, QTextCursor::MoveAnchor);


    m_calculationEdit->setTextCursor(textCursor);
}

void LineParser::deleteLine()
{
    QTextCursor textCursor = m_calculationEdit->textCursor();
    textCursor.clearSelection();
    textCursor.movePosition(QTextCursor::EndOfBlock, QTextCursor::MoveAnchor);
    textCursor.movePosition(QTextCursor::StartOfBlock, QTextCursor::KeepAnchor);
    textCursor.removeSelectedText();
    textCursor.deletePreviousChar();    //delete newline character
    m_calculationEdit->setTextCursor(textCursor);
}

QString LineParser::replaceVariables(QString expression, bool insertValue, bool insertUnit)
{
    QMapIterator<QString, PhyxVariable*> i(*m_phyxCalculator->variables());

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
                        variableValue.append(PhyxCalculator::complexToString(i.value()->value()));
                    if (insertUnit)
                        variableValue.append(i.value()->unit()->symbol());
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

QString LineParser::exportFormelEditor()
{
    QString text;
    QStringList textLines;
    int pos,
        depth;
    QRegExp regExp;

    text = m_calculationEdit->toPlainText().trimmed();

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


    QMapIterator<QString, PhyxVariable*> mapIterator(*m_phyxCalculator->variables());

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
    for (int k = 0; k < m_unitLoader->symbols()->size(); k++)
    {
        text.replace(m_unitLoader->symbols()->at(k).symbol, "%" + m_unitLoader->symbols()->at(k).name + " ", Qt::CaseSensitive);
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

    m_variableTable->clearContents();
    m_variableTable->setRowCount(0);

    QMapIterator<QString, PhyxVariable*> i(*m_phyxCalculator->variables());
     while (i.hasNext()) {
         i.next();
         PhyxCalculator::ResultVariable variable;
         variable = m_phyxCalculator->formatVariable(i.value(),
                                                     (PhyxCalculator::OutputMode)m_appSettings->output.unitMode,
                                                     (PhyxCalculator::PrefixMode)m_appSettings->output.prefixMode);

         m_variableTable->setRowCount(row+1);
         newItem = new QTableWidgetItem(i.key());   // name
         m_variableTable->setItem(row, 0, newItem);
         newItem = new QTableWidgetItem(variable.value);
         m_variableTable->setItem(row, 1, newItem);
         newItem = new QTableWidgetItem(variable.unit);
         m_variableTable->setItem(row, 2, newItem);

         row++;
     }
}

void LineParser::showConstants()
{
    int row = 0;
    QTableWidgetItem *newItem;

    m_constantsTable->clearContents();
    m_constantsTable->setRowCount(0);


    QMapIterator<QString, PhyxVariable*> i(*m_phyxCalculator->constants());
     while (i.hasNext()) {
         i.next();
         PhyxCalculator::ResultVariable variable;
         variable = m_phyxCalculator->formatVariable(i.value(),
                                                     (PhyxCalculator::OutputMode)m_appSettings->output.unitMode,
                                                     (PhyxCalculator::PrefixMode)m_appSettings->output.prefixMode);

         m_constantsTable->setRowCount(row+1);
         newItem = new QTableWidgetItem(i.key());   // name
         m_constantsTable->setItem(row, 0, newItem);
         newItem = new QTableWidgetItem(variable.value);
         m_constantsTable->setItem(row, 1, newItem);
         newItem = new QTableWidgetItem(variable.unit);
         m_constantsTable->setItem(row, 2, newItem);

         row++;
     }
}

void LineParser::clearAllVariables()
{
    m_phyxCalculator->clearVariables();
}

void LineParser::outputResult()
{
    PhyxCalculator::ResultVariable result = m_phyxCalculator->formatVariable(m_phyxCalculator->result(),
                                                                             (PhyxCalculator::OutputMode)m_appSettings->output.unitMode,
                                                                             (PhyxCalculator::PrefixMode)m_appSettings->output.prefixMode);
    QString output;
    output.append("=");
    output.append(result.value);
    output.append(result.unit);
    insertOutput(output);
}

void LineParser::outputError()
{
    QString output;
    output.append("=");
    output.append("<font color=red>");
    output.append(m_phyxCalculator->errorString());
    output.append("</font> ");
    insertOutput(output);
}
