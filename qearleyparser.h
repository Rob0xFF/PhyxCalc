#ifndef QEARLEYPARSER_H
#define QEARLEYPARSER_H

#include <QObject>
#include <QStringList>
#include <QMultiMap>
#include <QDebug>

typedef struct {
    QStringList conclusio;    //containing nonTerminals and terminals
    int type;               //the type of the rule, for calling the right function
} EarleyRule;

typedef struct {
    QString A;              //premise
    QStringList alpha;      //recognized part of the rule
    QStringList beta;       //not recognized part of the rule
    int K;                  //backpointer of the item
} EarleyItem;

typedef QList<EarleyItem> EarleyItemList;

class QEarleyParser : public QObject
{
    Q_OBJECT
public:
    explicit QEarleyParser(QObject *parent = 0);

    /* loads the rules from a string list and fills nonTerminals and terminals */
    bool loadRules(QStringList ruleList);
    /* parse the word starting with the given start symbol */
    void parseWord(QString earleyWord, QString earleyStartSymbol);

private:

    QMultiMap<QString, EarleyRule>   rules;     //mapping of all rules, for quick finding rules with the key (premise, member of nonTerminals)
    QStringList         terminals;              //contains all terminals
    QStringList         nonTerminals;           //contains all nonTerminals
    QString             startSymbol;            //the start symbol

    QList<EarleyItemList> earleyItemLists;      //holds the item lists
    int                 itemListCount;          //the count of item lists needed for pasing

    QStringList word;

    /* initializes variables and lists for the parser*/
    void initialize();
    /* starts to parse */
    void parse();

    /* appends an item to the given ItemList (index), checks also for duplicates */
    void appendEarleyItem(int index, QString A, QStringList alpha, QStringList beta, int K);

signals:

public slots:

};

#endif // QEARLEYPARSER_H
