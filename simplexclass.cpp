#include "simplexclass.h"

SimplexClass::SimplexClass(const QVector<float> &objFuncCoeffVector, const QVector<QVector<float> > &constrCoeffMatrix,
                                 const QVector<int> &signs, const QVector<float> &plans, QObject *parent)
    : LPMethod(objFuncCoeffVector, constrCoeffMatrix, signs, plans, parent)
{
    InitializeClass();
}

SimplexClass::SimplexClass(const LpStructure &otherStructure) : LPMethod(otherStructure)
{}

SimplexClass::SimplexClass() : LPMethod()
{}

void SimplexClass::InitializeClass()
{
    ApplySignEffect();

    GeneralSetup();
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
    structure.leadElement = structure.constrCoeffMatrix[structure.leadRowIndex][structure.leadColIndex];
}

void SimplexClass::RatioSetup()
{
    structure.ratio.resize(structure.constrCoeffMatrix.count(), 0.f);
}

bool SimplexClass::IsSolved()
{
    bool bSolved = true;
    for (int i = 0; i < structure.lastRow.count(); ++i){
        structure.lastRow[i] < 0 ? bSolved = false : bSolved;
    }

    return bSolved;
}
