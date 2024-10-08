#pragma once

#include "QObject"
#include "lpmethods.h"

class DualSimplexClass : public LPMethod
{
    Q_OBJECT
public:
    explicit DualSimplexClass(const QVector<double>& objFuncCoeffVector, const QVector<QVector<double>>& constrCoeffMatrix,
                              const QVector<int>& signs, const QVector<double>& plans, QObject *parent = nullptr);
    DualSimplexClass(const LpStructure& otherStructure);
    DualSimplexClass();

private:
    void InitializeClass();

    int GetMinColumnIndex(int rowIndex);

    int GetMinRowIndex();

    void TransposeConstrCoefficients();

    void SwapObjFuncWithPlans();

    void BringToCanonical();

protected:
    virtual void CalculateLeadingElement() override;
    virtual void RatioSetup() override;
    virtual bool IsSolved() override;
};
