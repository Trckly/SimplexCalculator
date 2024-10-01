#include "newsimplexclass.h"

NewSimplexClass::NewSimplexClass(QObject *parent)
    : LPMethods{parent}
{}

bool NewSimplexClass::SolveOneStep(QVector<float>& objFuncCoeffVector, QVector<QVector<float>>& constrCoeffMatrix,
                                   QVector<int>& signs, QVector<float>& plans, int& outLeadRowIndex, int& outLeadColIndex,
                                   QVector<int>& outBaseIndexes, float& outResultValue, QVector<float>& outRatio,
                                   QVector<float>& outLastRow)
{
    this->objFuncCoeffVector = objFuncCoeffVector;
    this->constrCoeffMatrix = constrCoeffMatrix;
    this->signs = signs;
    this->plans = plans;
    ratio.resize(objFuncCoeffVector.count(), 0.f);

    leadingColIndex = GetMinColumnIndex();

    leadingRowIndex = GetMinRowIndex(leadingColIndex);

    leadingElement = this->constrCoeffMatrix[leadingRowIndex][leadingColIndex];

    objFuncCoeffVector = this->objFuncCoeffVector;
    constrCoeffMatrix = this->constrCoeffMatrix;
    signs = this->signs;
    plans = this->plans;

    return SquareRule(outLeadRowIndex, outLeadColIndex, outBaseIndexes, outResultValue, outRatio, outLastRow);
}

int NewSimplexClass::GetMinColumnIndex()
{
    float min = 0;
    int minIndex = 0;
    for (int i = 0; i < lastRow.count(); ++i){
        if(i == 0){
            min = lastRow[i];
            minIndex = i;
            continue;
        }
        if(lastRow[i] < min){
            min = lastRow[i];
            minIndex = i;
        }
    }
    return minIndex;
}

int NewSimplexClass::GetMinRowIndex(int colIndex)
{
    float min = std::numeric_limits<float>::max(), tempRatio;
    int minIndex = 0;
    for (int i = 0; i < constrCoeffMatrix.count(); ++i){
        tempRatio = -1;
        if(constrCoeffMatrix[i][colIndex] > 0){
            tempRatio = plans[i] / constrCoeffMatrix[i][colIndex];
            if(tempRatio < min){
                min = tempRatio;
                minIndex = i;
            }
        }
        ratio[i] = tempRatio;
    }
    return minIndex;
}
