#include "simplexclass.h"

SimplexClass::SimplexClass(const QVector<float> &objFuncCoeffVector, const QVector<QVector<float> > &constrCoeffMatrix,
                                 const QVector<int> &signs, const QVector<float> &plans, QObject *parent)
    : LPMethod(objFuncCoeffVector, constrCoeffMatrix, signs, plans, parent)
{
    // It is being called in inherited class because other inherited classes
    // need to process this info after e.g. transpose of a matrix
    SetupConstraintsCoefficientMatrix(constrCoeffMatrix);

    ApplySignEffect();
}

int SimplexClass::GetMinColumnIndex()
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

int SimplexClass::GetMinRowIndex(int colIndex)
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

void SimplexClass::CalculateLeadingElement()
{
    structure.leadColIndex = GetMinColumnIndex();
    structure.leadRowIndex = GetMinRowIndex(structure.leadColIndex);
    leadingElement = structure.constrCoeffMatrix[structure.leadRowIndex][structure.leadColIndex];
}
