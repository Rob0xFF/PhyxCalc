/**************************************************************************
**
** This file is part of PhyxCalc.
**
** PhyxCalc is free software: you can redistribute it and/or modify
** it under the terms of the GNU General Public License as published by
** the Free Software Foundation, either version 3 of the License, or
** (at your option) any later version.
**
** PhyxCalc is distributed in the hope that it will be useful,
** but WITHOUT ANY WARRANTY; without even the implied warranty of
** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
** GNU General Public License for more details.
**
** You should have received a copy of the GNU General Public License
** along with PhyxCalc.  If not, see <http://www.gnu.org/licenses/>.
**
***************************************************************************/

#include "phyxsyntaxhighlighter.h"

PhyxSyntaxHighlighter::PhyxSyntaxHighlighter(QTextDocument *parent) :
    QSyntaxHighlighter(parent)
{
    HighlightingRule rule;

    keywordFormat.setForeground(Qt::darkYellow);
    QStringList keywordPatterns;
    keywordPatterns << "\\bif\\b" << "\\bthen\\b" << "\\belse\\b";
    foreach (const QString &pattern, keywordPatterns) {
        rule.pattern = QRegExp(pattern);
        rule.format = keywordFormat;
        highlightingRules.append(rule);
    }

    variablesFormat.setForeground(Qt::darkBlue);
    variablesFormat.setToolTip("variable");
    constantsFormat.setForeground(Qt::darkRed);
    constantsFormat.setToolTip("constant");

    singleLineCommentFormat.setForeground(Qt::darkGreen);
    rule.pattern = QRegExp("//[^\n]*");
    rule.format = singleLineCommentFormat;
    highlightingRules.append(rule);

    multiLineCommentFormat.setForeground(Qt::darkGreen);

    quotationFormat.setForeground(Qt::darkGreen);
    rule.pattern = QRegExp("\".*\"");
    rule.format = quotationFormat;
    highlightingRules.append(rule);

    functionFormat.setFontItalic(true);
    functionFormat.setToolTip("function");
    //functionFormat.setForeground(Qt::darkBlue);
    rule.pattern = QRegExp("\\b[A-Za-z0-9_]+(?=\\()");
    rule.format = functionFormat;
    highlightingRules.append(rule);

    errorFormat.setUnderlineStyle(QTextCharFormat::SpellCheckUnderline);
    errorFormat.setUnderlineColor(Qt::red);

    commentStartExpression = QRegExp("/\\*");
    commentEndExpression = QRegExp("\\*/");
}

void PhyxSyntaxHighlighter::setVariableHighlightingRules(QStringList variableList)
{
    variableHighlightingRules.clear();
    foreach (QString variableName, variableList)
    {
        QString pattern = QString("\\b%1\\b").arg(variableName);
        HighlightingRule rule;
        rule.pattern = QRegExp(pattern);
        rule.format = variablesFormat;
        variableHighlightingRules.append(rule);
    }
    rehighlight();
}

void PhyxSyntaxHighlighter::setConstantHighlightingRules(QStringList variableList)
{
    constantHighlightingRules.clear();
    foreach (QString variableName, variableList)
    {
        QString pattern = QString("\\b%1_\\b").arg(variableName);
        HighlightingRule rule;
        rule.pattern = QRegExp(pattern);
        rule.format = constantsFormat;
        constantHighlightingRules.append(rule);
    }
    rehighlight();
}

void PhyxSyntaxHighlighter::addError(int line, int pos, int length)
{
    Error error;
    error.line = line;
    error.pos = pos;
    error.length = length;

    errorList.append(error);
}

void PhyxSyntaxHighlighter::removeError(int line, int pos)
{
    for (int i = 0; i < errorList.size(); i++)
    {
        if ((errorList.at(i).line == line) && (errorList.at(i).pos == pos))
            errorList.removeAt(i);
    }
}

void PhyxSyntaxHighlighter::highlightBlock(const QString &text)
{
    foreach (const HighlightingRule &rule, highlightingRules) {
        QRegExp expression(rule.pattern);
        int index = expression.indexIn(text);
        while (index >= 0) {
            int length = expression.matchedLength();
            setFormat(index, length, rule.format);
            index = expression.indexIn(text, index + length);
        }
    }
    foreach (const HighlightingRule &rule, variableHighlightingRules) {
        QRegExp expression(rule.pattern);
        int index = expression.indexIn(text);
        while (index >= 0) {
            int length = expression.matchedLength();
            setFormat(index, length, rule.format);
            index = expression.indexIn(text, index + length);
        }
    }
    foreach (const HighlightingRule &rule, constantHighlightingRules) {
        QRegExp expression(rule.pattern);
        int index = expression.indexIn(text);
        while (index >= 0) {
            int length = expression.matchedLength();
            setFormat(index, length, rule.format);
            index = expression.indexIn(text, index + length);
        }
    }

    setCurrentBlockState(0);

    int startIndex = 0;
    if (previousBlockState() != 1)
        startIndex = commentStartExpression.indexIn(text);

    while (startIndex >= 0) {
        int endIndex = commentEndExpression.indexIn(text, startIndex);
        int commentLength;
        if (endIndex == -1) {
            setCurrentBlockState(1);
            commentLength = text.length() - startIndex;
        } else {
            commentLength = endIndex - startIndex
                            + commentEndExpression.matchedLength();
        }
        setFormat(startIndex, commentLength, multiLineCommentFormat);
        startIndex = commentStartExpression.indexIn(text, startIndex + commentLength);
    }

    //underline error
    foreach (Error error, errorList)
    {
        if (currentBlock().firstLineNumber() == error.line)
            setFormat(error.pos, error.length, errorFormat);
    }
}
