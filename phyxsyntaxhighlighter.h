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

#ifndef PHYXSYNTAXHIGHLIGHTER_H
#define PHYXSYNTAXHIGHLIGHTER_H

#include <QSyntaxHighlighter>
#include <QTextCharFormat>

class PhyxSyntaxHighlighter : public QSyntaxHighlighter
{
    Q_OBJECT

    typedef struct {
        int line;
        int pos;
        int length;
    } Error;

public:
    PhyxSyntaxHighlighter(QTextDocument *parent = 0);

    void setVariableHighlightingRules(QStringList variableList);
    void setConstantHighlightingRules(QStringList variableList);
    void addError(int line, int pos, int length);
    void removeError(int line, int pos);
    
protected:
    void highlightBlock(const QString &text);

private:
    struct HighlightingRule
    {
        QRegExp pattern;
        QTextCharFormat format;
    };
    QVector<HighlightingRule> highlightingRules;
    QVector<HighlightingRule> variableHighlightingRules;
    QVector<HighlightingRule> constantHighlightingRules;
    QList<Error>              errorList;

    QRegExp commentStartExpression;
    QRegExp commentEndExpression;

    QTextCharFormat singleLineCommentFormat;
    QTextCharFormat multiLineCommentFormat;
    QTextCharFormat keywordFormat;
    QTextCharFormat quotationFormat;
    QTextCharFormat functionFormat;
    QTextCharFormat variablesFormat;
    QTextCharFormat constantsFormat;
    QTextCharFormat errorFormat;
    
};

#endif // PHYXSYNTAXHIGHLIGHTER_H
