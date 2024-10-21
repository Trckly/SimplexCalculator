#include "transportpotentialmethod.h"
#include <boost/sort/spreadsort/spreadsort.hpp>

TransportPotentialMethod::TransportPotentialMethod(QTableWidget *srcTable, QObject *parent)
    : QObject{parent}
{
    ReadTransportationTable(srcTable);
}

bool TransportPotentialMethod::SolveOneStep()
{
    if(totalCost == 0){
        NorthWestCorner();
        return false;
    }

    CalculatePotentials();
    CalculateFictitiousCells();
    if(IsOptimal())
        return true;
    LoopPivoting();

    return false;
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
    u.clear();
    v.clear();

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
    w.clear();

    for (int i = 0; i < supply.count(); ++i){
        for (int j = 0 ; j < demand.count(); ++j){
            if(supplyDemandMatrix[i][j] != 0) continue;

            w.append(Cell{u[i] + v[j] - pathMatrix[i][j], i, j});
        }
    }
}

void TransportPotentialMethod::LoopPivoting()
{
    // Sort w array in ascending order by value
    boost::sort::pdqsort(w.begin(), w.end(), [](const Cell& a, const Cell& b) {
        return a.value < b.value;
    });

    Cell nonBasicCell = w.last();

    qDebug() << "Value: " <<(double)nonBasicCell.value << ";\ti: " << nonBasicCell.i << ";\tj: " << nonBasicCell.j;

    int dir = 0;
    int hit = 0;
    Cell result = nonBasicCell;
    QVector<Cell> markedCells{nonBasicCell};
    // Trace loop
    do{
        result = hit != 3 ? LookInDirection(result, dir) : LookInDirection(result, dir, nonBasicCell);

        if(result.i != -1){
            hit++;
            markedCells.append(result);

            if(hit == 4 && !IsSameCell(result, nonBasicCell)){
                dir < 2 ? dir += 2 : dir -= 2;
                markedCells.removeLast();

                qDebug() << "Value: " <<(double)result.value << ";\ti: " << result.i << ";\tj: " << result.j;
                qDebug() << "Direction: " << dir << ";\tHits: " << hit;

                continue;
            }
        }

        qDebug() << "Value: " <<(double)result.value << ";\ti: " << result.i << ";\tj: " << result.j;
        qDebug() << "Direction: " << dir << ";\tHits: " << hit << "\n";

        dir = dir == 3 ? 0 : dir + 1;
    } while (!IsSameCell(result, nonBasicCell));

    // Replace values
    cpp_dec_float_100 pivotValue = FindPivotValue(markedCells);
    for (int i = 0; i < markedCells.count(); ++i){
        if (i % 2 == 0){
            supplyDemandMatrix[markedCells[i].i][markedCells[i].j] += pivotValue;
        }
        else
            supplyDemandMatrix[markedCells[i].i][markedCells[i].j] -= pivotValue;
    }
}

bool TransportPotentialMethod::IsOptimal()
{
    for (int i = 0; i < w.count(); ++i){
        qDebug() << "Value: " <<(double)w[i].value << ";\ti: " << w[i].i << ";\tj: " << w[i].j;
    }

    for (int i = 0; i < w.count(); ++i){
        if(w[i].value > 0)
            return false;
    }

    return true;
}

Cell TransportPotentialMethod::LookInDirection(const Cell& currentCell, int direction, const Cell initialCell)
{
    int rowCount = supplyDemandMatrix.count();
    int colCount = supplyDemandMatrix.first().count();

    QVector<Cell> cells;
    if(direction == Left){
        for (int i = currentCell.j - 1; i >= 0; --i){
            auto value = supplyDemandMatrix[currentCell.i][i];
            if(value != 0)
                cells.append(Cell{value, currentCell.i, i});

            if(initialCell.i == currentCell.i && initialCell.j == i)
                cells.append(initialCell);
        }

        if(cells.count() > 0){
            return cells.first();
        }
    }
    if(direction == Down){
        for (int i = currentCell.i + 1; i < rowCount; ++i){
            auto value = supplyDemandMatrix[i][currentCell.j];
            if(value != 0)
                cells.append(Cell{value, i, currentCell.j});

            if(initialCell.i == i && initialCell.j == currentCell.j)
                cells.append(initialCell);
        }

        if(cells.count() > 0){
            return cells.first();
        }
    }
    if(direction == Right){
        for (int i = currentCell.j + 1; i < colCount; ++i){
            auto value = supplyDemandMatrix[currentCell.i][i];
            if(value != 0)
                cells.append(Cell{value, currentCell.i, i});

            if(initialCell.i == currentCell.i && initialCell.j == i)
                cells.append(initialCell);
        }

        if(cells.count() > 0){
            return cells.first();
        }
    }
    if(direction == Up){
        for (int i = currentCell.i - 1; i >= 0; --i){
            auto value = supplyDemandMatrix[i][currentCell.j];
            if(value != 0)
                cells.append(Cell{value, i, currentCell.j});

            if(initialCell.i == i && initialCell.j == currentCell.j)
                cells.append(initialCell);
        }

        if(cells.count() > 0){
            return cells.first();
        }
    }
    return Cell{-1, -1, -1};
}

bool TransportPotentialMethod::IsSameCell(const Cell &firstCell, const Cell &otherCell)
{
    return firstCell.i == otherCell.i && firstCell.j == otherCell.j && firstCell.value == otherCell.value;
}

cpp_dec_float_100 TransportPotentialMethod::FindPivotValue(const QVector<Cell> &searchVector)
{
    cpp_dec_float_100 min = std::numeric_limits<cpp_dec_float_100>::max();
    for (int i = 0; i < searchVector.count(); ++i){
        if(i % 2 == 1 && min > searchVector[i].value){
            min = searchVector[i].value;
        }
    }

    qDebug() << "Pivot value is: " << (double)min;

    return min;
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
