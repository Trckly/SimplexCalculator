#ifndef LPMETHODS_H
#define LPMETHODS_H

#include <QObject>

struct LpStructure {
    QVector<float> objFuncCoeffVector;
    QVector<QVector<float>> constrCoeffMatrix;
    QVector<int> signs;
    QVector<float> plans;
    int leadRowIndex;
    int leadColIndex;
    QVector<int> baseIndexes;
    float resultValue;
    QVector<float> ratio;
    QVector<float> lastRow;
    float leadElement;
};

class LPMethod : public QObject
{
    Q_OBJECT
public:
    explicit LPMethod(const QVector<float>& objFuncCoeffVector, const QVector<QVector<float>>& constrCoeffMatrix,
                       const QVector<int>& signs, const QVector<float>& plans, QObject *parent = nullptr);

protected:
    LpStructure structure;

    float leadingElement;

public:
    bool SolveOneStep();

    virtual void CalculateLeadingElement() = 0;

    const LpStructure &GetAll();

protected:
    bool SquareRule();

    bool IsSolved();

    void ApplySignEffect();

private:
    void SetupConstraintsCoefficientMatrix(const QVector<QVector<float>>& otherMatrix);
    void SetupBaseIndexes();
    void SetupLastRow();

signals:
};

#endif // LPMETHODS_H
