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
    // QVector<float> objFuncCoeffVector;
    // QVector<QVector<float>> constrCoeffMatrix;
    // QVector<int> signs;
    // QVector<float> plans;
    // // Variables calculated inside
    // QVector<float> lastRow;
    // QVector<float> ratio;
    // QVector<int> baseIndexes;
    // int leadingRowIndex;
    // int leadingColIndex;
    // float resultValue;

    LpStructure structure;

    float leadingElement;

public:
    virtual bool SolveOneStep() = 0;

    // void GetAll(QVector<float>& outObjFuncCoeffVector, QVector<QVector<float>>& outConstrCoeffMatrix,
    //             QVector<int>& outSigns, QVector<float>& outPlans, int& outLeadRowIndex, int& outLeadColIndex,
    //             QVector<int>& outBaseIndexes, float& outResultValue, QVector<float>& outRatio,
    //             QVector<float>& outLastRow);

    const LpStructure &GetAll();

protected:
    bool SquareRule();

    bool IsSolved();

    virtual void ApplySignEffect() = 0;

private:
    void SetupConstraintsCoefficientMatrix(const QVector<QVector<float>>& otherMatrix);
    void SetupBaseIndexes();
    void SetupLastRow();

signals:
};

#endif // LPMETHODS_H
