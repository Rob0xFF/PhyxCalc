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
