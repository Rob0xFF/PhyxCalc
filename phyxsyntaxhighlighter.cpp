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
        setFormat(startIndex, commentLength, commentFormat);
        startIndex = commentStartExpression.indexIn(text, startIndex + commentLength);
    }

    //underline error
    foreach (Error error, errorList)
    {
        if (currentBlock().firstLineNumber() == error.line)
            setFormat(error.pos, error.length, errorFormat);
    }
}

void PhyxSyntaxHighlighter::updateFormats()
{
    HighlightingRule rule;
    highlightingRules.clear();

    textFormat.setForeground(m_appSettings->textEditor.colorScheme.at(0).foregroundColor);
    textFormat.setBackground(m_appSettings->textEditor.colorScheme.at(0).backgroundColor);
    textFormat.setFontItalic(m_appSettings->textEditor.colorScheme.at(0).italic);
    textFormat.setFontWeight(m_appSettings->textEditor.colorScheme.at(0).bold ? QFont::Bold : QFont::Normal);
    selectionFormat.setForeground(m_appSettings->textEditor.colorScheme.at(1).foregroundColor);
    selectionFormat.setBackground(m_appSettings->textEditor.colorScheme.at(1).backgroundColor);
    selectionFormat.setFontItalic(m_appSettings->textEditor.colorScheme.at(1).italic);
    selectionFormat.setFontWeight(m_appSettings->textEditor.colorScheme.at(1).bold ? QFont::Bold : QFont::Normal);
    numberFormat.setForeground(m_appSettings->textEditor.colorScheme.at(2).foregroundColor);
    numberFormat.setBackground(m_appSettings->textEditor.colorScheme.at(2).backgroundColor);
    numberFormat.setFontItalic(m_appSettings->textEditor.colorScheme.at(2).italic);
    numberFormat.setFontWeight(m_appSettings->textEditor.colorScheme.at(2).bold ? QFont::Bold : QFont::Normal);
    stringFormat.setForeground(m_appSettings->textEditor.colorScheme.at(3).foregroundColor);
    stringFormat.setBackground(m_appSettings->textEditor.colorScheme.at(3).backgroundColor);
    stringFormat.setFontItalic(m_appSettings->textEditor.colorScheme.at(3).italic);
    stringFormat.setFontWeight(m_appSettings->textEditor.colorScheme.at(3).bold ? QFont::Bold : QFont::Normal);
    commentFormat.setForeground(m_appSettings->textEditor.colorScheme.at(4).foregroundColor);
    commentFormat.setBackground(m_appSettings->textEditor.colorScheme.at(4).backgroundColor);
    commentFormat.setFontItalic(m_appSettings->textEditor.colorScheme.at(4).italic);
    commentFormat.setFontWeight(m_appSettings->textEditor.colorScheme.at(4).bold ? QFont::Bold : QFont::Normal);
    keywordFormat.setForeground(m_appSettings->textEditor.colorScheme.at(5).foregroundColor);
    keywordFormat.setBackground(m_appSettings->textEditor.colorScheme.at(5).backgroundColor);
    keywordFormat.setFontItalic(m_appSettings->textEditor.colorScheme.at(5).italic);
    keywordFormat.setFontWeight(m_appSettings->textEditor.colorScheme.at(5).bold ? QFont::Bold : QFont::Normal);
    functionFormat.setForeground(m_appSettings->textEditor.colorScheme.at(6).foregroundColor);
    functionFormat.setBackground(m_appSettings->textEditor.colorScheme.at(6).backgroundColor);
    functionFormat.setFontItalic(m_appSettings->textEditor.colorScheme.at(6).italic);
    functionFormat.setFontWeight(m_appSettings->textEditor.colorScheme.at(6).bold ? QFont::Bold : QFont::Normal);
    variablesFormat.setForeground(m_appSettings->textEditor.colorScheme.at(7).foregroundColor);
    variablesFormat.setBackground(m_appSettings->textEditor.colorScheme.at(7).backgroundColor);
    variablesFormat.setFontItalic(m_appSettings->textEditor.colorScheme.at(7).italic);
    variablesFormat.setFontWeight(m_appSettings->textEditor.colorScheme.at(7).bold ? QFont::Bold : QFont::Normal);
    constantsFormat.setForeground(m_appSettings->textEditor.colorScheme.at(8).foregroundColor);
    constantsFormat.setBackground(m_appSettings->textEditor.colorScheme.at(8).backgroundColor);
    constantsFormat.setFontItalic(m_appSettings->textEditor.colorScheme.at(8).italic);
    constantsFormat.setFontWeight(m_appSettings->textEditor.colorScheme.at(8).bold ? QFont::Bold : QFont::Normal);
    errorFormat.setForeground(m_appSettings->textEditor.colorScheme.at(9).foregroundColor);
    errorFormat.setBackground(m_appSettings->textEditor.colorScheme.at(9).backgroundColor);
    errorFormat.setFontItalic(m_appSettings->textEditor.colorScheme.at(9).italic);
    errorFormat.setFontWeight(m_appSettings->textEditor.colorScheme.at(9).bold ? QFont::Bold : QFont::Normal);

    //tooltips for help
    variablesFormat.setToolTip("variable");
    constantsFormat.setToolTip("constant");
    functionFormat.setToolTip("function");

    //text
    rule.pattern = QRegExp("\\b[^ ]+\\b");
    rule.format = textFormat;
    highlightingRules.append(rule);

    //keywords
    QStringList keywordPatterns;
    keywordPatterns << "\\bif\\b" << "\\bthen\\b" << "\\belse\\b";
    foreach (const QString &pattern, keywordPatterns) {
        rule.pattern = QRegExp(pattern);
        rule.format = keywordFormat;
        highlightingRules.append(rule);
    }

    //single line comments
    rule.pattern = QRegExp("//[^\n]*");
    rule.format = commentFormat;
    highlightingRules.append(rule);

    //strings
    rule.pattern = QRegExp("\".*\"");
    rule.format = stringFormat;
    highlightingRules.append(rule);

    //functions
    rule.pattern = QRegExp("\\b[A-Za-z0-9_]+(?=\\()");
    rule.format = functionFormat;
    highlightingRules.append(rule);

    //errorFormat.setUnderlineStyle(QTextCharFormat::SpellCheckUnderline);

    //numbers
    rule.format = numberFormat;
    rule.pattern = QRegExp("\\b[0-9]+(\\.[0-9]+)?([eE][+-]?[0-9]+)?[ij]?");
    highlightingRules.append(rule);
    rule.pattern = QRegExp("\\b[0][x][0-9A-Fa-f]+\\b");
    highlightingRules.append(rule);
    rule.pattern = QRegExp("\\b[0][b][0-1]+\\b");
    highlightingRules.append(rule);
}
