#include "transportationtablebuilder.h"

TransportationTableBuilder::TransportationTableBuilder(QObject *parent)
    : QObject{parent}
{}

QTableWidget *TransportationTableBuilder::CreateInitialTable(int rows, int columns)
{
    QTableWidget* table = new QTableWidget;

    table->setRowCount(rows);
    table->setColumnCount(columns);

    return table;
}
