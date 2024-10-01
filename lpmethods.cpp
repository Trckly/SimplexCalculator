#include "lpmethods.h"

LPMethod::LPMethod(const QVector<float>& objFuncCoeffVector, const QVector<QVector<float>>& constrCoeffMatrix,
                     const QVector<int>& signs, const QVector<float>& plans, QObject *parent)
    : QObject{parent}
{
    this->objFuncCoeffVector = objFuncCoeffVector;
    this->constrCoeffMatrix = constrCoeffMatrix;
    this->signs = signs;
    this->plans = plans;
    lastRow.clear();
    ratio.clear();
    baseIndexes.clear();
    leadingElement = 0.f;
    leadingRowIndex = 0;
    leadingColIndex = 0;
    resultValue = 0;
}

bool LPMethod::SquareRule()
{
    for (int j = 0; j < constrCoeffMatrix[0].count(); ++j) {
        constrCoeffMatrix[leadingRowIndex][j] /= leadingElement;
    }

    plans[leadingRowIndex] /= leadingElement;

    for (int i = 0; i <= constrCoeffMatrix.count(); ++i){
        if (i == leadingRowIndex){
            baseIndexes[i] = leadingColIndex + 1;
            continue;
        }

        if(i < constrCoeffMatrix.count()){
            float rowFactor = constrCoeffMatrix[i][leadingColIndex];
            for (int j = 0; j < constrCoeffMatrix[0].count(); ++j){
                constrCoeffMatrix[i][j] -= rowFactor * constrCoeffMatrix[leadingRowIndex][j];
            }
            plans[i] -= rowFactor * plans[leadingRowIndex];
        }
        else{
            float rowFactor = lastRow[leadingColIndex];
            for (int j = 0; j < lastRow.count(); ++j){
                lastRow[j] -= rowFactor * constrCoeffMatrix[leadingRowIndex][j];
            }
            resultValue -= rowFactor * plans[leadingRowIndex];
        }
    }

    return IsSolved();
}

bool LPMethod::IsSolved()
{
    bool bSolved = true;
    for (int i = 0; i < lastRow.count(); ++i){
        lastRow[i] < 0 ? bSolved = false : bSolved;
    }

    return bSolved;
}

void LPMethod::GetAll(QVector<float> &outObjFuncCoeffVector, QVector<QVector<float>> &outConstrCoeffMatrix,
                       QVector<int> &outSigns, QVector<float> &outPlans, int &outLeadRowIndex, int &outLeadColIndex,
                       QVector<int> &outBaseIndexes, float &outResultValue, QVector<float> &outRatio,
                       QVector<float> &outLastRow)
{
    outObjFuncCoeffVector = objFuncCoeffVector;
    outConstrCoeffMatrix = constrCoeffMatrix;
    outSigns = signs;
    outPlans = plans;
    outLeadRowIndex = leadingRowIndex;
    outLeadColIndex = leadingColIndex;
    outBaseIndexes = baseIndexes;
    outResultValue = resultValue;
    outRatio = ratio;
    outLastRow = lastRow;
}

