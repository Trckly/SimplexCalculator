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
    QVector<QVector<double>> demandSupplyMatrix;

public:
    void ReadTransportationTable(QTableWidget *srcTable);

    void NorthWestCorner();

signals:
};

#endif // TRANSPORTPOTENTIALMETHOD_H
