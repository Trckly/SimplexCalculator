#ifndef TRANSPORTPOTENTIALMETHOD_H
#define TRANSPORTPOTENTIALMETHOD_H

#include <QObject>
#include<QTableWidget>

class TransportPotentialMethod : public QObject
{
    Q_OBJECT
public:
    explicit TransportPotentialMethod(QTableWidget* srcTable, QObject *parent = nullptr);

protected:
    QVector<double> supply;
    QVector<double> demand;

    QVector<QVector<double>> pathMatrix;
    QVector<QVector<double>> supplyDemandMatrix;

    QVector<double> v;
    QVector<double> u;

    double totalCost = 0;

public:
    void ReadTransportationTable(QTableWidget *srcTable);
    void NorthWestCorner();

    QVector<double> GetSupply();
    QVector<double> GetDemand();
    QVector<QVector<double>> GetPathMatrix();
    QVector<QVector<double>> GetSupplyDemandMatrix();
    double GetTotalCost();

protected:
    void CalculateTotalCost();

    void CalculatePotentials();

    void CalculateFictitiousCells();

};

#endif // TRANSPORTPOTENTIALMETHOD_H
