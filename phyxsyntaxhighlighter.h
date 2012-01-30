#ifndef PHYXSYNTAXHIGHLIGHTER_H
#define PHYXSYNTAXHIGHLIGHTER_H

#include <QSyntaxHighlighter>
#include <QTextCharFormat>

class PhyxSyntaxHighlighter : public QSyntaxHighlighter
{
    Q_OBJECT

public:
    PhyxSyntaxHighlighter(QTextDocument *parent = 0);

    void setVariableHighlightingRules(QStringList variableList);
    void setConstantHighlightingRules(QStringList variableList);
    
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

    QRegExp commentStartExpression;
    QRegExp commentEndExpression;

    QTextCharFormat singleLineCommentFormat;
    QTextCharFormat multiLineCommentFormat;
    QTextCharFormat keywordFormat;
    QTextCharFormat quotationFormat;
    QTextCharFormat functionFormat;
    QTextCharFormat variablesFormat;
    QTextCharFormat constantsFormat;
    
};

#endif // PHYXSYNTAXHIGHLIGHTER_H
