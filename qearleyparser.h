#ifndef QEARLEYPARSER_H
#define QEARLEYPARSER_H

#include <QObject>
#include <QStringList>
#include <QMultiHash>
#include <QDebug>

typedef qint32                  EarleySymbol; //datatype of one symbol
typedef QList<EarleySymbol>     EarleyRule;   //a list containing symbols (nonTerminals and terminals)

typedef struct {
    EarleySymbol    A;          //premise
    EarleyRule      alpha;      //recognized part of the rule
    EarleyRule      beta;       //not recognized part of the rule
    int             K;          //backpointer of the item
} EarleyItem;

typedef QList<EarleyItem> EarleyItemList;

typedef struct {
    QString rule;
    int     pos;
} EarleyTreeItem;

class QEarleyParser : public QObject
{
    Q_OBJECT
public:
    explicit QEarleyParser(QObject *parent = 0);

    /* loads the rules from a string list and fills nonTerminals and terminals */
    bool loadRules(QStringList ruleList);
    /* loads one rule */
    bool loadRule(QString rule);
    /* parse the word starting with the given start symbol */
    void parseWord(QString earleyWord, QString earleyStartSymbol);
    /* creates the binary tree*/
    QList<EarleyTreeItem> createTree();

private:

    QVector<QList<EarleyRule> >     rules;
    //QMultiHash<QString, EarleyRule>   rules;     //mapping of all rules, for quick finding rules with the key (premise, member of nonTerminals)
    //QStringList         terminals;              //contains all terminals, not needed, because terminals are represented by unicode value
    QStringList                     nonTerminals;           //contains all nonTerminals
    EarleySymbol                    startSymbol;            //the start symbol

    QList<EarleyItemList>           earleyItemLists;      //holds the item lists
    int                             itemListCount;          //the count of item lists needed for pasing

    EarleyRule word;

    /* initializes variables and lists for the parser*/
    void initialize();
    /* starts to parse from start position, return wheter parsing was successful or not*/
    bool parse(int startPosition = 0);
    /* recursive function to create the binary tree */
    void treeRecursion(int listIndex, int itemIndex, EarleyItemList *tree);

    /* appends an item to the given ItemList (index), checks also for duplicates */
    void appendEarleyItem(int index, EarleySymbol A, EarleyRule alpha, EarleyRule beta, int K);

    /* checks for duplicates and adds a NonTerminal, return NonTerminal-Index*/
    qint32 addNonTerminal(QString nonTerminal);

signals:

public slots:

};

#endif // QEARLEYPARSER_H
