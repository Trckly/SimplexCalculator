#include "dualsimplexclass.h"

DualSimplexClass::DualSimplexClass(const QVector<double> &objFuncCoeffVector, const QVector<QVector<double> > &constrCoeffMatrix,
                                   const QVector<int> &signs, const QVector<double> &plans, QObject *parent)
    : LPMethod(objFuncCoeffVector, constrCoeffMatrix, signs, plans, parent)
{
    InitializeClass();
}

DualSimplexClass::DualSimplexClass(const LpStructure &otherStructure) : LPMethod(otherStructure)
{
    InitializeClass();
}

DualSimplexClass::DualSimplexClass() : LPMethod()
{}

void DualSimplexClass::InitializeClass()
{
    ApplySignEffect();

    SwapObjFuncWithPlans();

    TransposeConstrCoefficients();

    BringToCanonical();

    GeneralSetup();
}

int DualSimplexClass::GetMinColumnIndex(int rowIndex)
{
    double min = std::numeric_limits<double>::max(), tempRatio;
    int minIndex = 0;
    for (int i = 0; i < structure.constrCoeffMatrix[0].count(); ++i){
        tempRatio = -1;
        if(structure.constrCoeffMatrix[rowIndex][i] < 0){
            tempRatio = structure.lastRow[i] / structure.constrCoeffMatrix[rowIndex][i];
            if(tempRatio < min){
                min = tempRatio;
                minIndex = i;
            }
        }
        structure.ratio[i] = tempRatio;
    }
    return minIndex;
}

int DualSimplexClass::GetMinRowIndex()
{
    double min = std::numeric_limits<double>::max();
    int minIndex = 0;
    for (int i = 0; i < structure.plans.count(); ++i){
        if(i == 0){
            min = structure.plans[i];
            continue;
        }
        if(structure.plans[i] < min){
            min = structure.plans[i];
            minIndex = i;
        }
    }
    return minIndex;
}

void DualSimplexClass::TransposeConstrCoefficients()
{
    const QVector<QVector<double>> temp = structure.constrCoeffMatrix;
    structure.constrCoeffMatrix.clear();

    for(int i = 0; i < temp[0].count(); ++i){
        QVector<double> row;
        for (int j = 0; j < temp.count(); ++j){
            row.append(temp[j][i]);
        }
        structure.constrCoeffMatrix.append(row);
    }
}

void DualSimplexClass::SwapObjFuncWithPlans()
{
    QVector<double> newPlans = structure.objFuncCoeffVector;
    QVector<double> newObjFuncCoeff = structure.plans;

    structure.objFuncCoeffVector.clear();
    structure.plans.clear();

    structure.objFuncCoeffVector = newObjFuncCoeff;
    structure.plans = newPlans;
}

void DualSimplexClass::BringToCanonical()
{
    for (double& plan : structure.plans){
        plan *= -1;
    }

    for(int i = 0; i < structure.constrCoeffMatrix.count(); ++i){
        for (int j = 0; j < structure.objFuncCoeffVector.count(); ++j){
            structure.constrCoeffMatrix[i][j] *= -1;
        }
    }
}

void DualSimplexClass::CalculateLeadingElement()
{
    structure.leadRowIndex = GetMinRowIndex();
    structure.leadColIndex = GetMinColumnIndex(structure.leadRowIndex);
    structure.leadElement = structure.constrCoeffMatrix[structure.leadRowIndex][structure.leadColIndex];
}

void DualSimplexClass::RatioSetup()
{
    structure.ratio.resize(structure.constrCoeffMatrix[0].count(), 0.f);
}

bool DualSimplexClass::IsSolved()
{
    bool bSolved = true;
    for (int i = 0; i < structure.plans.count(); ++i){
        structure.plans[i] < 0 ? bSolved = false : bSolved;
    }

    return bSolved;
}
