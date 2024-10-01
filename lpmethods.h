#ifndef LPMETHODS_H
#define LPMETHODS_H

#include <QObject>

class LPMethod : public QObject
{
    Q_OBJECT
public:
    explicit LPMethod(const QVector<float>& objFuncCoeffVector, const QVector<QVector<float>>& constrCoeffMatrix,
                       const QVector<int>& signs, const QVector<float>& plans, QObject *parent = nullptr);

protected:
    QVector<float> objFuncCoeffVector;

    QVector<QVector<float>> constrCoeffMatrix;

    QVector<int> signs;

    QVector<float> plans;

    // Variables calculated inside
    QVector<float> lastRow;

    QVector<float> ratio;

    QVector<int> baseIndexes;

    float leadingElement;

    int leadingRowIndex;

    int leadingColIndex;

    float resultValue;

protected:
    virtual bool SolveOneStep() = 0;

    bool SquareRule();

    bool IsSolved();

    void GetAll(QVector<float>& outObjFuncCoeffVector, QVector<QVector<float>>& outConstrCoeffMatrix,
                QVector<int>& outSigns, QVector<float>& outPlans, int& outLeadRowIndex, int& outLeadColIndex,
                QVector<int>& outBaseIndexes, float& outResultValue, QVector<float>& outRatio,
                QVector<float>& outLastRow);

    virtual void ApplySignEffect() = 0;

signals:
};

#endif // LPMETHODS_H
