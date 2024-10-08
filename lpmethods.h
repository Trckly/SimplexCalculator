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

    LPMethod(const LpStructure& otherStructure);

    LPMethod();

protected:
    LpStructure structure;

    float leadingElement;

public:
    virtual bool SolveOneStep();

    const LpStructure &GetAll();

    virtual void InjectStructure(const LpStructure& otherStructure);

protected:
    bool SquareRule();

    virtual bool IsSolved() = 0;

    void ApplySignEffect();

    // To be called in child constructors because thay can change initial structure
    void GeneralSetup();

    void SafeInjectStructure(const LpStructure& otherStructure);

    virtual void RatioSetup() = 0;

    virtual void CalculateLeadingElement() = 0;

private:   
    void SetupBaseIndexes();
    void SetupLastRow();
    void SetupConstraintsCoefficientMatrix();

signals:
};

#endif // LPMETHODS_H
