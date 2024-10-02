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

    structure.ratio.resize(structure.objFuncCoeffVector.count(), 0.f);

    structure.leadColIndex = GetMinColumnIndex();

    structure.leadRowIndex = GetMinRowIndex(structure.leadColIndex);

    leadingElement = this->structure.constrCoeffMatrix[structure.leadRowIndex][structure.leadColIndex];

    return SquareRule();
}

int NewSimplexClass::GetMinColumnIndex()
{
    float min = 0;
    int minIndex = 0;
    for (int i = 0; i < structure.lastRow.count(); ++i){
        if(i == 0){
            min = structure.lastRow[i];
            minIndex = i;
            continue;
        }
        if(structure.lastRow[i] < min){
            min = structure.lastRow[i];
            minIndex = i;
        }
    }
    return minIndex;
}

int NewSimplexClass::GetMinRowIndex(int colIndex)
{
    float min = std::numeric_limits<float>::max(), tempRatio;
    int minIndex = 0;
    for (int i = 0; i < structure.constrCoeffMatrix.count(); ++i){
        tempRatio = -1;
        if(structure.constrCoeffMatrix[i][colIndex] > 0){
            tempRatio = structure.plans[i] / structure.constrCoeffMatrix[i][colIndex];
            if(tempRatio < min){
                min = tempRatio;
                minIndex = i;
            }
        }
        structure.ratio[i] = tempRatio;
    }
    return minIndex;
}

void NewSimplexClass::ApplySignEffect()
{
    for (int i = 0; i < structure.constrCoeffMatrix.count(); ++i){
        if(structure.signs[i] == 1){
            for (int j = 0; j < structure.constrCoeffMatrix[0].count(); ++j){
                structure.constrCoeffMatrix[i][j] *= -1;
            }
            structure.plans[i] *= -1;
        }
    }
}
