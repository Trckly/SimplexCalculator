#ifndef TRANSPORTPOTENTIALMETHOD_H
#define TRANSPORTPOTENTIALMETHOD_H

#include <QObject>
#include<QTableWidget>
#include <boost/multiprecision/cpp_dec_float.hpp>

using namespace boost::multiprecision;

struct Cell {
    cpp_dec_float_100 value;
    int i;
    int j;
};

enum LookDirection{
    Left,
    Down,
    Right,
    Up
};

class TransportPotentialMethod : public QObject
{
    Q_OBJECT
public:
    explicit TransportPotentialMethod(QTableWidget* srcTable, QObject *parent = nullptr);

protected:
    QVector<cpp_dec_float_100> supply;
    QVector<cpp_dec_float_100> demand;

    QVector<QVector<cpp_dec_float_100>> pathMatrix;
    QVector<QVector<cpp_dec_float_100>> supplyDemandMatrix;

    QVector<cpp_dec_float_100> v;
    QVector<cpp_dec_float_100> u;

    QVector<Cell> w;

    cpp_dec_float_100 totalCost = 0;

public:
    bool SolveOneStep();

    void ReadTransportationTable(QTableWidget *srcTable);

    QVector<cpp_dec_float_100> GetSupply();
    QVector<cpp_dec_float_100> GetDemand();
    QVector<QVector<cpp_dec_float_100>> GetPathMatrix();
    QVector<QVector<cpp_dec_float_100>> GetSupplyDemandMatrix();
    cpp_dec_float_100 GetTotalCost();
    QVector<cpp_dec_float_100> GetFictiousCells();

protected:
    void NorthWestCorner();

    void CalculateTotalCost();

    void CalculatePotentials();

    void CalculateFictitiousCells();

    void GetNonBasicCell(int& iIndex, int& yIndex);

    void LoopPivoting();

    bool IsOptimal();

    Cell LookInDirection(const Cell &nonBasicCell, LookDirection direction);
};

#endif // TRANSPORTPOTENTIALMETHOD_H
