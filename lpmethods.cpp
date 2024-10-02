#include "lpmethods.h"

LPMethod::LPMethod(const QVector<float>& objFuncCoeffVector, const QVector<QVector<float>>& constrCoeffMatrix,
                     const QVector<int>& signs, const QVector<float>& plans, QObject *parent)
    : QObject{parent}
{
    structure.objFuncCoeffVector = objFuncCoeffVector;
    structure.constrCoeffMatrix = constrCoeffMatrix;
    structure.signs = signs;
    structure.plans = plans;
    structure.lastRow.clear();
    structure.ratio.clear();
    structure.baseIndexes.clear();
    structure.leadElement = 0.f;
    structure.leadRowIndex = 0;
    structure.leadColIndex = 0;
    structure.resultValue = 0;

    SetupBaseIndexes();

    SetupLastRow();

    structure.ratio.resize(structure.constrCoeffMatrix.count(), 0.f);

    SetupConstraintsCoefficientMatrix(constrCoeffMatrix);
}

void LPMethod::SetupConstraintsCoefficientMatrix(const QVector<QVector<float> > &otherMatrix)
{
    structure.constrCoeffMatrix.clear();
    structure.constrCoeffMatrix = otherMatrix;

    int newSize = otherMatrix.count() + structure.objFuncCoeffVector.count();
    for (int i = 0; i < otherMatrix.count(); ++i){
        structure.constrCoeffMatrix[i].resize(newSize, 0.f);
        for (int j = structure.objFuncCoeffVector.count(); j < newSize; ++j){
            if(i + structure.objFuncCoeffVector.count() == j)
                structure.constrCoeffMatrix[i][j] = 1;
        }
    }
}

void LPMethod::SetupBaseIndexes()
{
    for(int i = 1; i <= structure.constrCoeffMatrix.count(); ++i){
        structure.baseIndexes.append(i + structure.constrCoeffMatrix.count());
    }
}

void LPMethod::SetupLastRow()
{
    int lastRowSize = structure.constrCoeffMatrix.count() + structure.objFuncCoeffVector.count();
    for(int i = 0; i < lastRowSize; ++i){
        structure.lastRow.append(i < structure.objFuncCoeffVector.count() ? -structure.objFuncCoeffVector[i] : 0);
    }
}

bool LPMethod::SquareRule()
{
    for (int j = 0; j < structure.constrCoeffMatrix[0].count(); ++j) {
        structure.constrCoeffMatrix[structure.leadRowIndex][j] /= leadingElement;
    }

    structure.plans[structure.leadRowIndex] /= leadingElement;

    for (int i = 0; i <= structure.constrCoeffMatrix.count(); ++i){
        if (i == structure.leadRowIndex){
            structure.baseIndexes[i] = structure.leadColIndex + 1;
            continue;
        }

        if(i < structure.constrCoeffMatrix.count()){
            float rowFactor = structure.constrCoeffMatrix[i][structure.leadColIndex];
            for (int j = 0; j < structure.constrCoeffMatrix[0].count(); ++j){
                structure.constrCoeffMatrix[i][j] -= rowFactor * structure.constrCoeffMatrix[structure.leadRowIndex][j];
            }
            structure.plans[i] -= rowFactor * structure.plans[structure.leadRowIndex];
        }
        else{
            float rowFactor = structure.lastRow[structure.leadColIndex];
            for (int j = 0; j < structure.lastRow.count(); ++j){
                structure.lastRow[j] -= rowFactor * structure.constrCoeffMatrix[structure.leadRowIndex][j];
            }
            structure.resultValue -= rowFactor * structure.plans[structure.leadRowIndex];
        }
    }

    return IsSolved();
}

bool LPMethod::IsSolved()
{
    bool bSolved = true;
    for (int i = 0; i < structure.lastRow.count(); ++i){
        structure.lastRow[i] < 0 ? bSolved = false : bSolved;
    }

    return bSolved;
}

// void LPMethod::GetAll(QVector<float> &outObjFuncCoeffVector, QVector<QVector<float>> &outConstrCoeffMatrix,
//                        QVector<int> &outSigns, QVector<float> &outPlans, int &outLeadRowIndex, int &outLeadColIndex,
//                        QVector<int> &outBaseIndexes, float &outResultValue, QVector<float> &outRatio,
//                        QVector<float> &outLastRow)
// {
//     outObjFuncCoeffVector = objFuncCoeffVector;
//     outConstrCoeffMatrix = constrCoeffMatrix;
//     outSigns = signs;
//     outPlans = plans;
//     outLeadRowIndex = leadingRowIndex;
//     outLeadColIndex = leadingColIndex;
//     outBaseIndexes = baseIndexes;
//     outResultValue = resultValue;
//     outRatio = ratio;
//     outLastRow = lastRow;
// }

const LpStructure& LPMethod::GetAll()
{
    return structure;
}
