#pragma once

#include "QObject"
#include "lpmethods.h"

class DualSimplexClass : public LPMethod
{
    Q_OBJECT
public:
    explicit DualSimplexClass(const QVector<cpp_dec_float_100>& objFuncCoeffVector, const QVector<QVector<cpp_dec_float_100>>& constrCoeffMatrix,
                              const QVector<int>& signs, const QVector<cpp_dec_float_100>& plans, QObject *parent = nullptr);
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
