#pragma once

#include <QObject>
#include "lpmethods.h"

class Gomoryclass : public LPMethod
{
    Q_OBJECT
public:
    explicit Gomoryclass(const QVector<float>& objFuncCoeffVector, const QVector<QVector<float>>& constrCoeffMatrix,
                         const QVector<int>& signs, const QVector<float>& plans, QObject *parent = nullptr);

private:
    LPMethod* activeLpMethod;

protected:
    float plansFraction;
    QVector<float> fractions;
    int rowIndex;

public:
    virtual bool SolveOneStep() override;

protected:
    virtual bool IsSolved() override;

private:
    bool SolveSimplexOneStep();

    void MakeConstraint();
    void InsertConstraint();
};
