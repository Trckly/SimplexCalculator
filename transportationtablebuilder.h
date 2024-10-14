#ifndef TRANSPORTATIONTABLEBUILDER_H
#define TRANSPORTATIONTABLEBUILDER_H

#include <QObject>
#include <QTableWidget>

class TransportationTableBuilder : public QObject
{
    Q_OBJECT
public:
    explicit TransportationTableBuilder(QObject *parent = nullptr);

    static QTableWidget* CreateInitialTable(int rows, int columns);
};

#endif // TRANSPORTATIONTABLEBUILDER_H
