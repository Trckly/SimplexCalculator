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

QTableWidget *TransportationTableBuilder::CreateInitialTable_TEST(int rows, int columns)
{
    // Testing function (hardcoded table)
    QTableWidget* table = new QTableWidget;

    QStringList verticalHeaderLabels;
    for (int i = 0; i < 4; ++i){
        verticalHeaderLabels.append(QChar(65+i));
    }
    verticalHeaderLabels.append("Demand");

    QStringList horizontalHeaderLabels;
    for (int i = 0; i < 5; ++i){
        horizontalHeaderLabels.append(QString::number(i + 1));
    }
    horizontalHeaderLabels.append("Supply");

    table->setRowCount(4 + 1);
    table->setColumnCount(5 + 1);

    table->setVerticalHeaderLabels(verticalHeaderLabels);
    table->setHorizontalHeaderLabels(horizontalHeaderLabels);

    QVector<QVector<double>> tableContents = {{2, 8, 4, 6, 3, 95},
                                              {3, 2, 5, 2, 6, 55},
                                              {6, 5, 8, 7, 4, 40},
                                              {3, 4, 4, 2, 1, 60},
                                              {30, 90, 80, 20, 30, 0}};

    for (int i = 0; i < table->rowCount(); ++i){
        for (int j = 0; j < table->columnCount(); ++j){
            table->setItem(i, j, new QTableWidgetItem(QString::number(tableContents[i][j])));
        }
    }

    return table;
}

QTableWidget *TransportationTableBuilder::ConstructTable(TransportPotentialMethod *trpMethod)
{
    auto supply = trpMethod->GetSupply();
    auto demand = trpMethod->GetDemand();
    auto supplyDemandMatrix = trpMethod->GetSupplyDemandMatrix();
    auto pathMatrix = trpMethod->GetPathMatrix();

    int rows = supplyDemandMatrix.count();
    int cols = pathMatrix.count();

    QTableWidget* table = CreateInitialTable(rows, cols);

    for (int i = 0; i < table->rowCount(); ++i){
        for (int j = 0; j < table->columnCount(); ++j){
            //Main matrix fill
            if(i < table->rowCount() - 1 && j < table->columnCount() - 1)
                table->setItem(i, j, new QTableWidgetItem(QString::number(supplyDemandMatrix[i][j]) + "  (" +
                                                            QString::number(pathMatrix[i][j]) + ")"));

            // Supply fill
            else if(i < table->rowCount() - 1 && j == table->columnCount() - 1)
                table->setItem(i, j, new QTableWidgetItem(QString::number(supply[i])));

            // Demand fill
            else if(i == table->rowCount() - 1 && j < table->columnCount() - 1)
                table->setItem(i, j, new QTableWidgetItem(QString::number(demand[j])));

            // Redundant element fill
            else
                table->setItem(i, j, new QTableWidgetItem("0"));
        }
    }
    return table;
}
