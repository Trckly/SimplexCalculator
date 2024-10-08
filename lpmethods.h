#ifndef LPMETHODS_H
#define LPMETHODS_H

#include <QObject>

struct LpStructure {
    QVector<double> objFuncCoeffVector;
    QVector<QVector<double>> constrCoeffMatrix;
    QVector<int> signs;
    QVector<double> plans;
    int leadRowIndex;
    int leadColIndex;
    QVector<int> baseIndexes;
    double resultValue;
    QVector<double> ratio;
    QVector<double> lastRow;
    double leadElement;
};

class LPMethod : public QObject
{
    Q_OBJECT
public:
    explicit LPMethod(const QVector<double>& objFuncCoeffVector, const QVector<QVector<double>>& constrCoeffMatrix,
                       const QVector<int>& signs, const QVector<double>& plans, QObject *parent = nullptr);

    LPMethod(const LpStructure& otherStructure);

    LPMethod();

protected:
    LpStructure structure;

    double leadingElement;

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
