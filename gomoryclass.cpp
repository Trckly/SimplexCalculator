#include <qdebug.h>
#include <QException>
#include <QMessageBox>
#include "gomoryclass.h"
#include "simplexclass.h"
#include "dualsimplexclass.h"
#include <boost/multiprecision/cpp_dec_float.hpp>

using namespace boost::multiprecision;

cpp_dec_float_100 GetFraction(cpp_dec_float_100 num);

GomoryClass::GomoryClass(const QVector<cpp_dec_float_100> &objFuncCoeffVector, const QVector<QVector<cpp_dec_float_100> > &constrCoeffMatrix, const QVector<int> &signs, const QVector<cpp_dec_float_100> &plans, QObject *parent)
    : LPMethod(objFuncCoeffVector, constrCoeffMatrix, signs, plans, parent)
{
    InitializeClass();

    activeLpMethod = new SimplexClass(structure);

    bSimplexSolved = false;
    bDualSolved = false;
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
        return false;
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
        if(bDualSolved){
            MakeConstraint();
            InsertConstraint();
            bDualSolved = !bDualSolved;
        }

        activeLpMethod->InjectStructure(structure);
        bDualSolved = activeLpMethod->SolveOneStep();
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
    qDebug() << "\nGomory IsSolved plans values:";

    bool bSolved = true;

    cpp_dec_float_100 epsilon = 1e-9;  // Small tolerance for floating-point comparison

    for(int i = 0; i < structure.plans.count(); ++i){
        cpp_dec_float_100 result = structure.plans[i] - floor(structure.plans[i]);

        // Check if result is close to 0 or 1 (i.e., it's nearly an integer)
        if (!(boost::multiprecision::abs(result) < epsilon ||
              boost::multiprecision::abs(result - 1.0) < epsilon)) {
            bSolved = false;
        }
    }

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
    cpp_dec_float_100 max = std::numeric_limits<cpp_dec_float_100>::max();
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

    fractions.clear();
    for (int i = 0; i < structure.constrCoeffMatrix[rowIndex].count(); ++i){
        fractions.append(-GetFraction(structure.constrCoeffMatrix[rowIndex][i]));
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
    structure.baseIndexes.append(structure.constrCoeffMatrix[0].count() - 2);
}

cpp_dec_float_100 GetFraction(cpp_dec_float_100 num){
    if(num < 0.0)
        return 1 + (num - (int)num);

    return num - (int)num;
}
