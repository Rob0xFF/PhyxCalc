#include "qearleyparser.h"

QEarleyParser::QEarleyParser(QObject *parent) :
    QObject(parent)
{
}

bool QEarleyParser::loadRules(QStringList ruleList)
{
    terminals.clear();
    nonTerminals.clear();
    rules.clear();

    foreach (QString rule, ruleList)
    {
        int equalPos = rule.indexOf('=');
        if (equalPos == -1)
        {
            qDebug() << "Not a valid rule";
            return false;
        }

        QString premise = rule.left(equalPos);
        QString conclusio = rule.mid(equalPos+1);

        //split conclusio, check terminal or not terminal and append to rule
        QStringList conclusioList = conclusio.split('|');

        EarleyRule earleyRule;
        for (int i = 0; i < conclusioList.size(); i++)
        {
            QString currentSymbol = conclusioList.at(i);
            if (!currentSymbol.isEmpty())
            {
                if (((i+1)%2) == 0)
                {
                    if (!nonTerminals.contains(currentSymbol))
                        nonTerminals.append(currentSymbol);
                }
                else
                {
                    if (!terminals.contains(currentSymbol))
                        terminals.append(currentSymbol);
                }

                earleyRule.conclusio.append(currentSymbol);
            }
        }
        rules.insert(premise, earleyRule);
    }

            qDebug() << "non terminals:" << nonTerminals;
            qDebug() << "terminals:" << terminals;
}

void QEarleyParser::initialize()
{
    itemListCount = word.size() + 1;

    earleyItemLists.clear();
    for (int i = 0; i < itemListCount; i++)
    {
        earleyItemLists.append(EarleyItemList());
    }
}

void QEarleyParser::parse(int startPosition)
{
    int currentIndex = startPosition;

    //predictor special case
    foreach (EarleyRule rule, rules.values(startSymbol))
    {
        appendEarleyItem(0, startSymbol, QStringList(), rule.conclusio, 0);
    }

    for (int i = 0; i < itemListCount; i++)
    {
        int oldcount = -1;
        while (earleyItemLists.at(currentIndex).size() != oldcount)
        {
            oldcount = earleyItemLists.at(currentIndex).size();

            foreach (EarleyItem item, earleyItemLists.at(currentIndex))
            {
                if (!item.beta.isEmpty())
                {
                    QString firstSymbol = item.beta.at(0);
                    if (nonTerminals.contains(firstSymbol))
                    {
                        //Predictor
                        foreach (EarleyRule rule, rules.values(firstSymbol))
                        {
                            appendEarleyItem(currentIndex, firstSymbol, QStringList(), rule.conclusio, currentIndex);
                        }
                    }
                    else if (currentIndex < (itemListCount-1))
                    {
                        //Scanner
                        if (terminals.contains(firstSymbol) && (word.at(currentIndex) == firstSymbol))
                        {
                            QStringList newAlpha = item.alpha;
                            QStringList newBeta = item.beta;
                            newAlpha.append(newBeta.takeFirst());   //move point right

                            appendEarleyItem(currentIndex+1, item.A, newAlpha, newBeta, item.K);
                        }
                    }
                }
                else
                {
                    //Completer
                    foreach (EarleyItem item2, earleyItemLists.at(item.K))
                    {
                        if (!item2.beta.isEmpty() && (item2.beta.at(0) == item.A))
                        {
                            QStringList newAlpha = item2.alpha;
                            QStringList newBeta = item2.beta;
                            newAlpha.append(newBeta.takeFirst());   //move point right

                            appendEarleyItem(currentIndex, item2.A, newAlpha, newBeta, item2.K);
                        }
                    }
                }
            }
        }
        currentIndex++;
    }
}

// this function is only for testing purposes
void QEarleyParser::parseWord(QString earleyWord, QString earleyStartSymbol)
{
    word.clear();
    foreach (QChar character, earleyWord)
    {
        word.append(QString(character));
    }

    startSymbol = earleyStartSymbol;

    initialize();
    parse();
    createTree();
}

void QEarleyParser::appendEarleyItem(int index, QString A, QStringList alpha, QStringList beta, int K)
{
    bool match = false;
    foreach (EarleyItem item, earleyItemLists.at(index))
    {
        match |= (item.A == A) && (item.alpha == alpha) && (item.beta == beta) && (item.K == K);
        if (match)
            return;
    }

    EarleyItem earleyItem;
    earleyItem.A = A;
    earleyItem.alpha = alpha;
    earleyItem.beta = beta;
    earleyItem.K = K;

    earleyItemLists[index].append(earleyItem);

    qDebug() << index << A << alpha << beta << K;
}

void QEarleyParser::treeRecursion(int listIndex, int itemIndex, EarleyItemList *tree)
{
    if (listIndex != 0)
    {
        if (!tree->at(itemIndex).alpha.isEmpty())
        {
            QString lastSymbol = tree->at(itemIndex).alpha.last();
            if (nonTerminals.contains(lastSymbol))
            {
                //backward predictor
                foreach (EarleyItem item, earleyItemLists.at(listIndex))
                {
                    if (item.beta.isEmpty() && (item.A == lastSymbol))
                    {
                        tree->append(item);
                        treeRecursion(listIndex, tree->size()-1, tree);
                    }
                }
            }
            else
            {
                //backward scanner
                (*tree)[itemIndex].beta.prepend((*tree)[itemIndex].alpha.takeLast());
                treeRecursion(listIndex-1, itemIndex, tree);
            }
        }
        else
        {
            //backward completer
            if (!tree->at(itemIndex-1).alpha.isEmpty())
                (*tree)[itemIndex-1].beta.prepend((*tree)[itemIndex-1].alpha.takeLast());
            treeRecursion(listIndex, itemIndex-1, tree);
        }
    }
}

void QEarleyParser::createTree()
{
    EarleyItemList tree;
    foreach (EarleyItem item, earleyItemLists.last())
    {
        if ((item.A == startSymbol) && item.beta.isEmpty())
            tree.append(item);
    }

    if (!tree.isEmpty())    //this check should be in parsing function, returns wheter parsing was successful or not
        treeRecursion(itemListCount-1,0,&tree);
    else
    {
        qDebug() << "Syntax Error";
    }

    //for testing purposes only
    foreach (EarleyItem item, tree)
    {
        qDebug() << item.A << item.alpha << item.beta << item.K;
    }
}
