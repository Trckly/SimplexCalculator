#include "newsimplexclass.h"

NewSimplexClass::NewSimplexClass(const QVector<float> &objFuncCoeffVector,
                                 const QVector<QVector<float> > &constrCoeffMatrix,
                                 const QVector<int> &signs, const QVector<float> &plans, QObject *parent)
    : LPMethod(objFuncCoeffVector, constrCoeffMatrix, signs, plans, parent)
{
    ApplySignEffect();
}

bool NewSimplexClass::SolveOneStep()
{

    ratio.resize(objFuncCoeffVector.count(), 0.f);

    leadingColIndex = GetMinColumnIndex();

    leadingRowIndex = GetMinRowIndex(leadingColIndex);

    leadingElement = this->constrCoeffMatrix[leadingRowIndex][leadingColIndex];

    return SquareRule();
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

void NewSimplexClass::ApplySignEffect()
{
    for (int i = 0; i < constrCoeffMatrix.count(); ++i){
        if(signs[i] == 1){
            for (int j = 0; j < constrCoeffMatrix[0].count(); ++j){
                constrCoeffMatrix[i][j] *= -1;
            }
            plans[i] *= -1;
        }
    }
}
