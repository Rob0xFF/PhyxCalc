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

#ifndef PHYXTESTING_H
#define PHYXTESTING_H

#include <QObject>
#include <QDebug>
#include "phyxunit.h"
#include "phyxunitsystem.h"
#include "phyxcompoundunit.h"
#include "phyxvariable.h"

class PhyxTesting : public QObject
{
    Q_OBJECT
public:
    explicit PhyxTesting(QObject *parent = 0);

    static void testUnits();
    
signals:
    
public slots:
    
};

#endif // PHYXTESTING_H
