#pragma once

#include <QObject>
#include "lpmethods.h"

class GomoryClass : public LPMethod
{
    Q_OBJECT
public:
    explicit GomoryClass(const QVector<double>& objFuncCoeffVector, const QVector<QVector<double>>& constrCoeffMatrix,
                         const QVector<int>& signs, const QVector<double>& plans, QObject *parent = nullptr);

private:
    LPMethod* activeLpMethod;

    bool bSimplexSolved;
    bool bDualSolved;

    void InitializeClass();

protected:
    double plansFraction;
    QVector<double> fractions;
    int rowIndex;

public:
    virtual bool SolveOneStep() override;

    LPMethod *GetCurrentMethod();

protected:
    virtual bool IsSolved() override;

    virtual void RatioSetup() override;

    virtual void CalculateLeadingElement() override;

private:
    bool SolveSimplexOneStep();
    void MakeConstraint();
    void InsertConstraint();
    void CorrectRatio();
    void CorrectConstrCoeffMatrix();
    void CorrectLastRow();
    void CorrectBaseIndexes();
};
