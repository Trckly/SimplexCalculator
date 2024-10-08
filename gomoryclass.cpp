#include <qdebug.h>
#include <QException>
#include <QMessageBox>
#include "gomoryclass.h"
#include "simplexclass.h"
#include "dualsimplexclass.h"

float GetFraction(float num);

GomoryClass::GomoryClass(const QVector<float> &objFuncCoeffVector, const QVector<QVector<float> > &constrCoeffMatrix, const QVector<int> &signs, const QVector<float> &plans, QObject *parent)
    : LPMethod(objFuncCoeffVector, constrCoeffMatrix, signs, plans, parent)
{
    InitializeClass();

    activeLpMethod = new SimplexClass(structure);

    bSimplexSolved = false;
}

void GomoryClass::InitializeClass()
{
    ApplySignEffect();

    GeneralSetup();
}

bool GomoryClass::SolveOneStep()
{
    if(!bSimplexSolved){
        bSimplexSolved = SolveSimplexOneStep();
        structure = activeLpMethod->GetAll();
        return IsSolved();
    }

    if(dynamic_cast<SimplexClass*>(activeLpMethod)){
        activeLpMethod->deleteLater();

        MakeConstraint();
        InsertConstraint();

        activeLpMethod = new DualSimplexClass();
        activeLpMethod->InjectStructure(structure);
        return IsSolved();
    }

    // Solve for DualSimplex
    if(activeLpMethod){
        activeLpMethod->SolveOneStep();
        structure = activeLpMethod->GetAll();
    }

    return IsSolved();
}

LPMethod *GomoryClass::GetCurrentMethod()
{
    return activeLpMethod;
}

bool GomoryClass::IsSolved()
{
    bool bSolved = true;
    for(int i = 0; i < structure.plans.count(); ++i)
        bSolved = structure.plans[i] - (int)structure.plans[i] != 0 ? false : true;

    if(structure.leadElement == 0){
        bSolved = true;
    }

    if(bSolved)
        activeLpMethod->deleteLater();

    return bSolved;
}

void GomoryClass::RatioSetup()
{
    structure.ratio.resize(structure.constrCoeffMatrix[0].count(), 0.f);
}

void GomoryClass::CalculateLeadingElement()
{}

bool GomoryClass::SolveSimplexOneStep()
{
    if(!activeLpMethod || !dynamic_cast<SimplexClass*>(activeLpMethod)){
        return true;
    }

    return activeLpMethod->SolveOneStep();
}

void GomoryClass::MakeConstraint()
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
    plansFraction = -max;

    for (int i = 0; i < structure.constrCoeffMatrix[rowIndex].count(); ++i){
        fractions.append(GetFraction(structure.constrCoeffMatrix[rowIndex][i]));
    }
}

void GomoryClass::InsertConstraint()
{
    structure.plans.append(plansFraction);

    int constrCoeffColCount = structure.constrCoeffMatrix[0].count();
    if(constrCoeffColCount == fractions.count())
        structure.constrCoeffMatrix.append(fractions);
    else
        throw QException();

    CorrectConstrCoeffMatrix();
    CorrectRatio();
    CorrectLastRow();
    CorrectBaseIndexes();
}

void GomoryClass::CorrectRatio()
{
    for (int i = 0; i < structure.constrCoeffMatrix[0].count(); ++i)
        if(i >= structure.ratio.count())
            structure.ratio.append(-1);
}

void GomoryClass::CorrectConstrCoeffMatrix()
{
    int lastRow = structure.constrCoeffMatrix.count() - 1;
    for (int i = 0; i < structure.constrCoeffMatrix.count(); ++i){
        structure.constrCoeffMatrix[i].append(i == lastRow ? 1 : 0);
    }
}

void GomoryClass::CorrectLastRow()
{
    structure.lastRow.append(0);
}

void GomoryClass::CorrectBaseIndexes()
{
    structure.baseIndexes.append(structure.constrCoeffMatrix[0].count() - 1);
}

float GetFraction(float num){
    return num - (int)num;
}
