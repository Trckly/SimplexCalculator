#include "lpmethods.h"

LPMethods::LPMethods(QObject *parent)
    : QObject{parent}
{}

bool LPMethods::SquareRule(int &outLeadRowIndex, int &outLeadColIndex, QVector<int> &outBaseIndexes, float &outResultValue,
                           QVector<float> &outRatio, QVector<float> &outLastRow)
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

    outLeadRowIndex = leadingRowIndex;
    outLeadColIndex = leadingColIndex;
    outBaseIndexes = baseIndexes;
    outResultValue = resultValue;
    outRatio = ratio;
    outLastRow = lastRow;

    return IsSolved();
}

bool LPMethods::IsSolved()
{
    bool bSolved = true;
    for (int i = 0; i < lastRow.count(); ++i){
        lastRow[i] < 0 ? bSolved = false : bSolved;
    }

    return bSolved;
}

