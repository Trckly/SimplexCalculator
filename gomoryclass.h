#pragma once

#include <QObject>
#include "lpmethods.h"
#include <boost/multiprecision/cpp_dec_float.hpp>

using namespace boost::multiprecision;

class GomoryClass : public LPMethod
{
    Q_OBJECT
public:
    explicit GomoryClass(const QVector<cpp_dec_float_100>& objFuncCoeffVector, const QVector<QVector<cpp_dec_float_100>>& constrCoeffMatrix,
                         const QVector<int>& signs, const QVector<cpp_dec_float_100>& plans, QObject *parent = nullptr);

private:
    LPMethod* activeLpMethod;

    bool bSimplexSolved;
    bool bDualSolved;

    void InitializeClass();

protected:
    cpp_dec_float_100 plansFraction;
    QVector<cpp_dec_float_100> fractions;
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
