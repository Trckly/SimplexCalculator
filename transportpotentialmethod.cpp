#include "transportpotentialmethod.h"

TransportPotentialMethod::TransportPotentialMethod(QTableWidget *srcTable, QObject *parent)
    : QObject{parent}
{
    ReadTransportationTable(srcTable);
}

void TransportPotentialMethod::ReadTransportationTable(QTableWidget* srcTable)
{
    int rows = srcTable->rowCount();
    int cols = srcTable->columnCount();

    for (int i = 0; i < rows; ++i){
        QVector<double> pathRow;
        for (int j = 0; j < cols; ++j){
            double currentValue = srcTable->item(i, j)->text().toDouble();

            if(i < rows - 1){
                if (j == cols - 1)
                    supply.append(currentValue);
                else
                    pathRow.append(currentValue);
            }
            else
                if(j < cols - 1)
                    demand.append(currentValue);
        }
        if(i < rows - 1)
            pathMatrix.append(pathRow);
    }
}

void TransportPotentialMethod::NorthWestCorner()
{
    int jFillOffset = -1;

    for (int i = 0; i < supply.count(); ++i){
        QVector<double> demandSupplyRow;
        bool bFillRow = false;

        for (int j = 0 ; j < demand.count(); ++j){
            double leftover = supply[i] - demand[j];

            if(j == jFillOffset || bFillRow){
                demandSupplyRow.append(0);
                continue;
            }

            if(leftover >= 0){
                jFillOffset++;
                demandSupplyRow.append(demand[j]);

                supply[i] = leftover;
                demand[j] = 0;
            }
            else {
                bFillRow = true;
                demandSupplyRow.append(supply[i]);

                supply[i] = 0;
                demand[j] = -leftover;
            }
        }
        supplyDemandMatrix.append(demandSupplyRow);
    }
}

QVector<QVector<double> > TransportPotentialMethod::GetPathMatrix()
{
    return pathMatrix;
}

QVector<QVector<double> > TransportPotentialMethod::GetSupplyDemandMatrix()
{
    return supplyDemandMatrix;
}

QVector<double> TransportPotentialMethod::GetSupply()
{
    return supply;
}

QVector<double> TransportPotentialMethod::GetDemand()
{
    return demand;
}
