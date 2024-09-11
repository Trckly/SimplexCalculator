#pragma once

#include <QObject>

class SimplexClass : public QObject
{
    Q_OBJECT
public:
    explicit SimplexClass(QObject *parent = nullptr);

protected:
    QVector<float> objFuncCoeffVector;

    QVector<QVector<float>> constrCoeffMatrix;

public:
    void SetObjectiveCoefficientVector(QVector<float> otherVector);

    void SetConstraintsCoefficientMatrix(QVector<QVector<float>> otherMatrix);

signals:
};
