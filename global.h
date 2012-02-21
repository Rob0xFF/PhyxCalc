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

#ifndef GLOBAL_H
#define GLOBAL_H

#define MATH_OPERATORS "[+\\-\\*/\\(\\)<>!\\^=]"

#include <QFont>
#include <QColor>
#include <QList>

/*struct physicalVariable {
    double  value;
    QString unit;
};*/


//structure for colorScheme Items
typedef struct {
    QColor  foregroundColor;
    QColor  backgroundColor;
    bool    bold;
    bool    italic;
} colorSchemeItem;

typedef struct {
    struct {
        struct {
            int decimalPrecision;
            char format;
        } numbers;
        int unitMode;
        int prefixMode;
    } output;
    struct {
        struct {
            bool outputWithNumbers;
            bool outputResult;
        } expression;
    } lineParser;
    struct {
        QFont font;
        bool useSyntaxHighlighter;
        QList<colorSchemeItem> colorScheme;
    } textEditor;
} AppSettings;

#endif // GLOBAL_H
