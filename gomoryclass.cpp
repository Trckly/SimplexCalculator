#include "gomoryclass.h"
#include <qdebug.h>
#include "dualsimplexclass.h"
#include "simplexclass.h"

float GetFraction(float num);

Gomoryclass::Gomoryclass(const QVector<float> &objFuncCoeffVector, const QVector<QVector<float> > &constrCoeffMatrix, const QVector<int> &signs, const QVector<float> &plans, QObject *parent)
    : LPMethod(objFuncCoeffVector, constrCoeffMatrix, signs, plans, parent)
{
    activeLpMethod = new SimplexClass(structure);
}

bool Gomoryclass::SolveOneStep()
{
    bool bSimplexSolved = SolveSimplexOneStep();
    structure = activeLpMethod->GetAll();
    if(bSimplexSolved){
        delete activeLpMethod;

        if(IsSolved()) return true;
    }

    return IsSolved();
}

bool Gomoryclass::IsSolved()
{
    bool bSolved = true;
    for(int i = 0; i < structure.plans.count(); ++i){
        bSolved = structure.plans[i] - (int)structure.plans[i] != 0 ? false : true;
    }
    return bSolved;
}

bool Gomoryclass::SolveSimplexOneStep()
{
    if(!activeLpMethod || !dynamic_cast<SimplexClass*>(activeLpMethod)){
        return true;
    }

    return activeLpMethod->SolveOneStep();
}

void Gomoryclass::MakeConstraint()
{
    float max = std::numeric_limits<float>::min();
    for (int i = 0; i < structure.plans.count(); ++i){
        if(i == 0){
            max = GetFraction(structure.plans[i]);
            rowIndex = i;
        }
        else if(GetFraction(structure.plans[i]) > max){
            max = GetFraction(structure.plans[i]);
            rowIndex = i;
        }
    }
    plansFraction = max;

    for (int i = 0; i < structure.constrCoeffMatrix[rowIndex].count(); ++i){
        fractions.append(GetFraction(structure.constrCoeffMatrix[rowIndex][i]));
    }
}

void Gomoryclass::InsertConstraint()
{
    structure.plans.append(plansFraction);

    int constrCoeffColCount = structure.constrCoeffMatrix[0].count();
    if(constrCoeffColCount == fractions.count())
        structure.constrCoeffMatrix.append(fractions);
    else
        qDebug() << "Fraction count: " + QString::number(fractions.count()) +
                    "; Constraints coefficients column count: " + QString::number(constrCoeffColCount) +
                    " - Are NOT the same!";
}

float GetFraction(float num){
    return num - (int)num;
}
