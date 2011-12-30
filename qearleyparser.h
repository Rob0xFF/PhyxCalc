#ifndef QEARLEYPARSER_H
#define QEARLEYPARSER_H

#include <QObject>
#include <QStringList>
#include <QMultiHash>
#include <QDebug>

typedef qint32                  EarleySymbol; /// datatype of one symbol
typedef QVector<EarleySymbol>     EarleyRule;   /// a list containing symbols (nonTerminals and terminals)

typedef struct EarleyItemStruct{
    EarleySymbol    A;          /// premise
    EarleyRule      *rule;      /// a pointer to the rule
    int             dotPos;     /// the position of the dot, left side recognized, right side not recognized
    int             K;          /// backpointer of the item

    bool operator ==(const EarleyItemStruct &item) const
    {
        return (A == item.A) && (rule == item.rule) && (dotPos == item.dotPos) && (K == item.K);
    }

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

    bool loadRules(QStringList ruleList);                               ///< loads the rules from a string list and fills nonTerminals and terminals
    bool loadRule(QString rule);                                        ///< loads one rule
    bool removeRule(QString rule);                                      ///< removes one rule
    void setStartSymbol(QString earleyStartSymbol);                     ///< sets the start symbol
    bool parse(int startPosition = 0);                                  ///< starts to parse from start position, return wheter parsing was successful or not
    bool parseWord(QString earleyWord);                                 ///< parse the given word, returns wheter word can be build with the given grammar or not
    void clearWord();                                                   ///< clears the word
    bool addSymbol(QChar earleySymbol);                                 ///< adds one symbol to word and parses it, return is same as parseWord
    bool removeSymbol();                                                ///< removes one symbol from word and parses it, return is same as parseWord
    QList<EarleyTreeItem> getTree();                                    ///< creates and returns the binary tree
    QString EarleyItemToString(EarleyItem item);                        ///< converts an Earley-item into a string for debugging


private:

    QVector<QList<EarleyRule> >     rules;                  /// vector holding all rules, index is index in nonTerminals
    QVector<bool>                   isNullableVector;       /// vector holding wheter a nonTerminal at index is nullable or not, needed for epsilon rules
    QStringList                     nonTerminals;           /// contains all nonTerminals
    EarleySymbol                    startSymbol;            /// the start symbol


    QList<EarleyItemList>           earleyItemLists;        /// holds the item lists
    EarleyItemList                  earleyItemResultLists;  /// holds the resulting tree of the recursive tree walk
    int                             itemListCount;          /// the count of item lists needed for pasing
    bool                            isRecursionDone;        /// holds wheter a tree recursion was done or not, needed for partial parsing

    EarleyRule word;                                        /// word that should be parsed



    void initialize();                                                                              ///< initializes variables and lists for the parser
    void setWord(QString earleyWord);                                                               ///< sets the word for pasing
    void treeRecursion(int listIndex, int itemIndex, EarleyItemList& tree);                         ///< recursive function to create the binary tree
    void appendEarleyItem(int index, EarleySymbol A, EarleyRule *rule, int dotPos, int K);          ///< appends an item to the given ItemList (index), checks also for duplicates
    bool checkSuccessful();                                                                         ///< checks wheter parsing was successful or not
    EarleySymbol addNonTerminal(QString nonTerminal);                                               ///< checks for duplicates and adds a NonTerminal, return NonTerminal-Index

signals:

public slots:

};

#endif // QEARLEYPARSER_H
