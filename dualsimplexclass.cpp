#include "dualsimplexclass.h"

DualSimplexClass::DualSimplexClass(const QVector<float> &objFuncCoeffVector, const QVector<QVector<float> > &constrCoeffMatrix,
                                   const QVector<int> &signs, const QVector<float> &plans, QObject *parent)
    : LPMethod(objFuncCoeffVector, constrCoeffMatrix, signs, plans, parent)
{
    ApplySignEffect();

    SwapObjFuncWithPlans();

    TransposeConstrCoefficients();

    BringToCanonical();

    // It is being called in inherited class because matrix need to be transposed
    // before setup
    SetupConstraintsCoefficientMatrix(constrCoeffMatrix);
}

int DualSimplexClass::GetMinColumnIndex(int rowIndex)
{
    float min = std::numeric_limits<float>::max(), tempRatio;
    int minIndex = 0;
    for (int i = 0; i < structure.constrCoeffMatrix[0].count(); ++i){
        tempRatio = -1;
        if(structure.constrCoeffMatrix[rowIndex][i] < 0){
            tempRatio = -structure.lastRow[i] / structure.constrCoeffMatrix[rowIndex][i];
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
    float min = 0;
    int minIndex = 0;
    for (int i = 0; i < structure.plans.count(); ++i){
        if(i == 0){
            min = structure.plans[i];
            minIndex = i;
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
    const QVector<QVector<float>> temp = structure.constrCoeffMatrix;
    structure.constrCoeffMatrix.clear();

    for(int i = 0; i < temp[0].count(); ++i){
        QVector<float> row;
        for (int j = 0; j < temp.count(); ++j){
            row.append(temp[j][i]);
        }
        structure.constrCoeffMatrix.append(row);
    }
}

void DualSimplexClass::SwapObjFuncWithPlans()
{
    QVector<float> newPlans = structure.objFuncCoeffVector;
    QVector<float> newObjFuncCoeff = structure.plans;

    structure.objFuncCoeffVector.clear();
    structure.plans.clear();

    structure.objFuncCoeffVector = newObjFuncCoeff;
    structure.plans = newPlans;
}

void DualSimplexClass::BringToCanonical()
{
    for (float& plan : structure.plans){
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
    leadingElement = structure.constrCoeffMatrix[structure.leadRowIndex][structure.leadColIndex];
}
