#include "transportpotentialmethod.h"

TransportPotentialMethod::TransportPotentialMethod(QTableWidget *srcTable, QObject *parent)
    : QObject{parent}
{
    ReadTransportationTable(srcTable);
}

void TransportPotentialMethod::SolveOneStep()
{
    if(totalCost == 0)
        NorthWestCorner();

    CalculatePotentials();
    CalculateFictitiousCells();
}

void TransportPotentialMethod::ReadTransportationTable(QTableWidget* srcTable)
{
    int rows = srcTable->rowCount();
    int cols = srcTable->columnCount();

    for (int i = 0; i < rows; ++i){
        QVector<cpp_dec_float_100> pathRow;
        for (int j = 0; j < cols; ++j){
            cpp_dec_float_100 currentValue = srcTable->item(i, j)->text().toDouble();

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
        QVector<cpp_dec_float_100> demandSupplyRow;
        bool bFillRow = false;

        for (int j = 0 ; j < demand.count(); ++j){
            cpp_dec_float_100 leftover = supply[i] - demand[j];

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

    CalculateTotalCost();
}

void TransportPotentialMethod::CalculatePotentials()
{
    u.resize(1, 0.0);

    for (int i = 0; i < supply.count(); ++i){
        for (int j = 0 ; j < demand.count(); ++j){
            if(supplyDemandMatrix[i][j] == 0) continue;

            if(u.count() == i)
                u.append(pathMatrix[i][j] - v[j]);

            if(v.count() == j)
                v.append(pathMatrix[i][j] - u[i]);
        }
    }
}

void TransportPotentialMethod::CalculateFictitiousCells()
{
    QVector<cpp_dec_float_100> w;
    QVector<int> iIndexes;
    QVector<int> yIndexes;

    for (int i = 0; i < supply.count(); ++i){
        for (int j = 0 ; j < demand.count(); ++j){
            if(supplyDemandMatrix[i][j] != 0) continue;

            w.append(supplyDemandMatrix[i][j] = u[i] + v[j] - pathMatrix[i][j]);
            iIndexes.append(i);
            yIndexes.append(j);
        }
    }
}

void TransportPotentialMethod::LoopPivoting()
{

}

bool TransportPotentialMethod::IsOptimal(const QVector<cpp_dec_float_100>& w)
{
    for (int i = 0; i < w.count(); ++i){
        if(w[i] > 0)
            return false;
    }

    return true;
}

void TransportPotentialMethod::CalculateTotalCost()
{
    totalCost = 0;
    for (int i = 0; i < supplyDemandMatrix.count(); ++i)
        for (int j = 0 ; j < supplyDemandMatrix[0].count(); ++j)
            totalCost += supplyDemandMatrix[i][j] * pathMatrix[i][j];
}

QVector<QVector<cpp_dec_float_100> > TransportPotentialMethod::GetPathMatrix()
{
    return pathMatrix;
}

QVector<QVector<cpp_dec_float_100> > TransportPotentialMethod::GetSupplyDemandMatrix()
{
    return supplyDemandMatrix;
}

cpp_dec_float_100 TransportPotentialMethod::GetTotalCost()
{
    return totalCost;
}

QVector<cpp_dec_float_100> TransportPotentialMethod::GetSupply()
{
    return supply;
}

QVector<cpp_dec_float_100> TransportPotentialMethod::GetDemand()
{
    return demand;
}
