#ifndef TRANSPORTPOTENTIALMETHOD_H
#define TRANSPORTPOTENTIALMETHOD_H

#include <QObject>
#include<QTableWidget>
#include <boost/multiprecision/cpp_dec_float.hpp>

using namespace boost::multiprecision;

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

    cpp_dec_float_100 totalCost = 0;

public:
    void SolveOneStep();

    void ReadTransportationTable(QTableWidget *srcTable);

    QVector<cpp_dec_float_100> GetSupply();
    QVector<cpp_dec_float_100> GetDemand();
    QVector<QVector<cpp_dec_float_100>> GetPathMatrix();
    QVector<QVector<cpp_dec_float_100>> GetSupplyDemandMatrix();
    cpp_dec_float_100 GetTotalCost();

protected:
    void NorthWestCorner();

    void CalculateTotalCost();

    void CalculatePotentials();

    void CalculateFictitiousCells();

    void LoopPivoting();

    bool IsOptimal(const QVector<cpp_dec_float_100>& w);
};

#endif // TRANSPORTPOTENTIALMETHOD_H
