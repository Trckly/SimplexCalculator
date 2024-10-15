#ifndef TRANSPORTATIONTABLEBUILDER_H
#define TRANSPORTATIONTABLEBUILDER_H

#include <QObject>
#include <QTableWidget>
#include "transportpotentialmethod.h"

class TransportationTableBuilder : public QObject
{
    Q_OBJECT
public:
    explicit TransportationTableBuilder(QObject *parent = nullptr);

    static QTableWidget* CreateInitialTable(int rows, int columns);

    static QTableWidget* CreateInitialTable_TEST(int rows, int columns);

    static QTableWidget* ConstructTable(TransportPotentialMethod *trpMethod);
};

#endif // TRANSPORTATIONTABLEBUILDER_H
