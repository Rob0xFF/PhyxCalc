#include "qearleyparser.h"

QEarleyParser::QEarleyParser(QObject *parent) :
    QObject(parent)
{
}

bool QEarleyParser::loadRule(QString rule)
{
    int equalPos = rule.indexOf('=');
    if (equalPos == -1)
    {
        qDebug() << "Not a valid rule";
        return false;
    }

    //split premise and conclusio
    QString premise = rule.left(equalPos);
    QString conclusio = rule.mid(equalPos+1);

    QList<qint32> conclusioConverted;
    qint32 premiseConverted;

    premiseConverted = addNonTerminal(premise);         //convert premise

    //convert conclusio
    if (!conclusio.isEmpty())   //check for epsilon rule
    {
        bool isNonTerminal = false;
        int nonTerminalPos;
        for (int i = 0; i < conclusio.size(); i++)
        {
            if (conclusio.at(i) == '|')
            {
                if (!isNonTerminal)
                    nonTerminalPos = i+1;
                else
                {
                    QString tmpNonTerminal = conclusio.mid(nonTerminalPos, i-nonTerminalPos);
                    conclusioConverted.append(addNonTerminal(tmpNonTerminal));
                }
                isNonTerminal = !isNonTerminal;
            }
            else if (!isNonTerminal)
            {
                conclusioConverted.append(conclusio.at(i).unicode());
            }
        }
    }
    else
    {
        isNullableVector[-premiseConverted] = true;     //if epsilon rule, nonTerminal is nullable
    }

    rules[-premiseConverted].append(conclusioConverted);

    return true;
}

bool QEarleyParser::loadRules(QStringList ruleList)
{
    nonTerminals.clear();
    rules.clear();
    isNullableVector.clear();

    //fixing the nonTerminal 0 problem, 0 is already a terminal
    nonTerminals.append(QString());
    rules.append(QList<EarleyRule>());
    isNullableVector.append(false);

    foreach (QString rule, ruleList)
    {
        if (!loadRule(rule))
            return false;
    }

    qDebug() << "non terminals:" << nonTerminals;

    return true;
}

qint32 QEarleyParser::addNonTerminal(QString nonTerminal)
{
    if (nonTerminals.contains(nonTerminal))
        return -nonTerminals.indexOf(nonTerminal);
    else
    {
        nonTerminals.append(nonTerminal);
        rules.append(QList<EarleyRule>());
        isNullableVector.append(false);
        return -(nonTerminals.size()-1);
    }
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

bool QEarleyParser::parse(int startPosition)
{
    int currentIndex = startPosition;

    //predictor special case
    foreach (EarleyRule rule, rules.at(-startSymbol))
    {
        appendEarleyItem(0, startSymbol, EarleyRule(), rule, 0);
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
                    EarleySymbol firstSymbol = item.beta.at(0);
                    if (firstSymbol < 0)    //if symbol < 0, symbol = nonTerminal
                    {
                        //Predictor
                        foreach (EarleyRule rule, rules.at(-firstSymbol))
                        {
                            appendEarleyItem(currentIndex, firstSymbol, EarleyRule(), rule, currentIndex);
                        }
                        //Aycock and Horspool Epsilon solution
                        if (isNullableVector.at(-firstSymbol))  //if B is nullable
                        {
                            EarleyRule newAlpha = item.alpha;
                            EarleyRule newBeta = item.beta;
                            newAlpha.append(newBeta.takeFirst());   //move point right

                            appendEarleyItem(currentIndex, item.A, newAlpha, newBeta, item.K);
                        }
                    }
                    else if (currentIndex < (itemListCount-1))
                    {
                        //Scanner
                        if ((firstSymbol >= 0) && (word.at(currentIndex) == firstSymbol))
                        {
                            EarleyRule newAlpha = item.alpha;
                            EarleyRule newBeta = item.beta;
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
                            EarleyRule newAlpha = item2.alpha;
                            EarleyRule newBeta = item2.beta;
                            newAlpha.append(newBeta.takeFirst());   //move point right

                            appendEarleyItem(currentIndex, item2.A, newAlpha, newBeta, item2.K);
                        }
                    }
                }
            }
        }
        currentIndex++;
    }

    //check wheter parsing was successful or not
    foreach (EarleyItem item, earleyItemLists.last())
    {
        if ((item.A == startSymbol) && item.beta.isEmpty())
            return true;
    }

    return false;
}

// this function is only for testing purposes
void QEarleyParser::parseWord(QString earleyWord, QString earleyStartSymbol)
{
    word.clear();
    foreach (QChar character, earleyWord)
    {
        word.append(character.unicode());
    }

    startSymbol = -nonTerminals.indexOf(earleyStartSymbol);

    initialize();
    if (parse())
        createTree();
    else
        qDebug() << "Syntax Error!";
}

void QEarleyParser::appendEarleyItem(int index, EarleySymbol A, EarleyRule alpha, EarleyRule beta, int K)
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

    //qDebug() << index << A << alpha << beta << K;
}

void QEarleyParser::treeRecursion(int listIndex, int itemIndex, EarleyItemList *tree)
{
    if (listIndex != 0)
    {
        if (!tree->at(itemIndex).alpha.isEmpty())
        {
            EarleySymbol lastSymbol = tree->at(itemIndex).alpha.last();
            if (lastSymbol < 0)     //if symbol < 0, symbol = nonTerminal
            {
                //backward predictor
                for (int i = 0; i < earleyItemLists.at(listIndex).size(); i++)//foreach (EarleyItem item, earleyItemLists.at(listIndex))
                {
                    if (earleyItemLists.at(listIndex).at(i).A == lastSymbol)  //item.beta.isEmpty() is now unnessecary here
                    {
                        tree->insert(itemIndex+1,earleyItemLists.at(listIndex).at(i));
                        treeRecursion(listIndex, itemIndex+1, tree);
                        earleyItemLists[listIndex].removeAt(i);
                        i--;
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
            if (itemIndex > 0)
            {
                if (!tree->at(itemIndex-1).alpha.isEmpty())
                    (*tree)[itemIndex-1].beta.prepend((*tree)[itemIndex-1].alpha.takeLast());
                treeRecursion(listIndex, itemIndex-1, tree);
            }
        }
    }
}

QList<EarleyTreeItem> QEarleyParser::createTree()
{
    EarleyItemList tree;

    //remove unneeded items
    for (int listIndex = 0; listIndex < itemListCount; listIndex++)
    {
        for (int i = earleyItemLists.at(listIndex).size()-1; i >= 0 ; i--)
        {
            if (!earleyItemLists.at(listIndex).at(i).beta.isEmpty())
                earleyItemLists[listIndex].removeAt(i);
        }
    }

    //for testing purposes only
    for (int i = 0; i < itemListCount; i++)
    {
        foreach (EarleyItem item, earleyItemLists.at(i))
        {
            qDebug() << i << item.A << item.alpha << item.beta << item.K;
        }
    }

    //add the final item
    foreach (EarleyItem item, earleyItemLists.last())
    {
        if ((item.A == startSymbol) && item.beta.isEmpty())
            tree.append(item);
    }

    treeRecursion(itemListCount-1,0,&tree);

    //creating the binary tree for output
    QList<EarleyTreeItem> outputTree;
    foreach (EarleyItem item, tree)
    {
        QString rule;
        rule.append(nonTerminals.at(-item.A));
        rule.append("=");
        item.alpha.append(item.beta);
        foreach (EarleySymbol symbol, item.alpha)
        {
            if (symbol < 0)
                rule.append("|" + nonTerminals.at(-symbol) + "|");
            else
                rule.append(QChar(symbol));
        }

        outputTree.append(EarleyTreeItem());
        outputTree.last().rule = rule;
        outputTree.last().pos = item.K;
    }

    //for testing purposes only
    foreach (EarleyTreeItem item, outputTree)
    {
        qDebug() << item.rule << item.pos;
    }

    return outputTree;
}
