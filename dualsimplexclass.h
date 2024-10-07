#pragma once

#include "QObject"
#include "lpmethods.h"

class DualSimplexClass : public LPMethod
{
    Q_OBJECT
public:
    explicit DualSimplexClass(const QVector<float>& objFuncCoeffVector, const QVector<QVector<float>>& constrCoeffMatrix,
                              const QVector<int>& signs, const QVector<float>& plans, QObject *parent = nullptr);

private:
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
