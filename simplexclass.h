#pragma once

#include <QObject>
#include <QComboBox>
#include <QDebug>
#include <QTableWidget>
#include <QVector2D>

class SimplexClass : public QObject
{
    Q_OBJECT
public:
    explicit SimplexClass(QObject *parent = nullptr);
    SimplexClass(QVector<float> objCoeffVector, QVector<QVector<float>> constrCoeffVector, QVector<QComboBox*> signsVector, QVector<float> plansVector);

protected:
    QVector<float> objFuncCoeffVector;

    QVector<QVector<float>> constrCoeffMatrix;

    QVector<int> signs;

    QVector<float> plans;

    QVector<int> baseIndexes;

public:
    void SetObjectiveCoefficientVector(QVector<float> otherVector);

    void SetConstraintsCoefficientMatrix(QVector<QVector<float>> otherMatrix);

    void SetSigns(QVector<QComboBox*> signsComboBoxVector);

    void SetPlans(QVector<float> otherVector);

    void DebugOutput();

    QVector<QTableWidget*> BuildTables();

    QPoint CalculateTableDimentions();

    QTableWidget* ConstructTable(QPoint Dimentions);

    bool SimplexAlgorithm();

signals:
};
