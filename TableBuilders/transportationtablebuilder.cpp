#include "transportationtablebuilder.h"

TransportationTableBuilder::TransportationTableBuilder(QObject *parent)
    : QObject{parent}
{}

QTableWidget *TransportationTableBuilder::CreateInitialTable(int rows, int columns)
{
    QTableWidget* table = new QTableWidget;

    QStringList verticalHeaderLabels;
    for (int i = 0; i < rows; ++i){
        verticalHeaderLabels.append(QChar(65+i));
    }
    verticalHeaderLabels.append("Demand");

    QStringList horizontalHeaderLabels;
    for (int i = 0; i < columns; ++i){
        horizontalHeaderLabels.append(QString::number(i + 1));
    }
    horizontalHeaderLabels.append("Supply");

    table->setRowCount(rows + 1);
    table->setColumnCount(columns + 1);

    table->setVerticalHeaderLabels(verticalHeaderLabels);
    table->setHorizontalHeaderLabels(horizontalHeaderLabels);

    return table;
}
