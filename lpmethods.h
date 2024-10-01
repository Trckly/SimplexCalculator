#ifndef LPMETHODS_H
#define LPMETHODS_H

#include <QObject>

class LPMethods : public QObject
{
    Q_OBJECT
public:
    explicit LPMethods(QObject *parent = nullptr);

protected:
    QVector<float> objFuncCoeffVector;

    QVector<QVector<float>> constrCoeffMatrix;

    QVector<int> signs;

    QVector<float> plans;

    // Variables calculated inside
    QVector<float> lastRow;

    QVector<float> ratio;

    float leadingElement;

    int leadingColIndex;

    int leadingRowIndex;

    float resultValue = 0;

    QVector<int> baseIndexes;

protected:
    virtual bool SolveOneStep(QVector<float>& objFuncCoeffVector, QVector<QVector<float>>& constrCoeffMatrix,
                              QVector<int>& signs, QVector<float>& plans, int& outLeadRowIndex, int& outLeadColIndex,
                              QVector<int>& outBaseIndexes, float& outResultValue, QVector<float>& outRatio,
                              QVector<float>& outLastRow) = 0;

    bool SquareRule(int& outLeadRowIndex, int& outLeadColIndex,
                    QVector<int>& outBaseIndexes, float& outResultValue, QVector<float>& outRatio,
                    QVector<float>& outLastRow);

    bool IsSolved();

signals:
};

#endif // LPMETHODS_H
