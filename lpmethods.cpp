#include "lpmethods.h"

LPMethod::LPMethod(const QVector<cpp_dec_float_100>& objFuncCoeffVector, const QVector<QVector<cpp_dec_float_100>>& constrCoeffMatrix,
                     const QVector<int>& signs, const QVector<cpp_dec_float_100>& plans, QObject *parent)
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
}

LPMethod::LPMethod(const LpStructure &otherStructure)
{
    structure = otherStructure;
}

LPMethod::LPMethod()
{
    structure.leadElement = 0.f;
    structure.leadRowIndex = 0;
    structure.leadColIndex = 0;
    structure.resultValue = 0;
}

bool LPMethod::SolveOneStep()
{
    CalculateLeadingElement();

    return SquareRule();
}



void LPMethod::SetupConstraintsCoefficientMatrix()
{
    int newSize = structure.constrCoeffMatrix.count() + structure.objFuncCoeffVector.count();
    for (int i = 0; i < structure.constrCoeffMatrix.count(); ++i){
        structure.constrCoeffMatrix[i].resize(newSize, 0.f);
        for (int j = structure.objFuncCoeffVector.count(); j < newSize; ++j){
            if(i + structure.objFuncCoeffVector.count() == j)
                structure.constrCoeffMatrix[i][j] = 1;
        }
    }
}

void LPMethod::SafeInjectStructure(const LpStructure &otherStructure)
{
    // structure.objFuncCoeffVector = otherStructure.objFuncCoeffVector;
    // structure.constrCoeffMatrix = otherStructure.constrCoeffMatrix;
    // structure.signs = otherStructure.signs;
    // structure.plans = otherStructure.plans;
    // structure.lastRow = otherStructure.lastRow;
    // structure.baseIndexes = otherStructure.baseIndexes;
    // structure.resultValue = otherStructure.resultValue;
    // structure.ratio = otherStructure.ratio;
    // structure.leadElement = 0.f;
    // structure.leadRowIndex = 0;
    // structure.leadColIndex = 0;

    structure = otherStructure;
}

void LPMethod::InjectStructure(const LpStructure &otherStructure)
{
    SafeInjectStructure(otherStructure);
}

// To be called in child constructors because thay can change initial structure
void LPMethod::GeneralSetup()
{
    SetupConstraintsCoefficientMatrix();

    SetupBaseIndexes();

    SetupLastRow();

    RatioSetup();
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
        cpp_dec_float_100 zero = 0;
        structure.lastRow.append(i < structure.objFuncCoeffVector.count() ? -structure.objFuncCoeffVector[i] : zero);
    }
}

bool LPMethod::SquareRule()
{
    for (int j = 0; j < structure.constrCoeffMatrix[0].count(); ++j) {
        structure.constrCoeffMatrix[structure.leadRowIndex][j] /= structure.leadElement;
    }

    structure.plans[structure.leadRowIndex] /= structure.leadElement;

    for (int i = 0; i <= structure.constrCoeffMatrix.count(); ++i){
        if (i == structure.leadRowIndex){
            structure.baseIndexes[i] = structure.leadColIndex + 1;
            continue;
        }

        if(i < structure.constrCoeffMatrix.count()){
            cpp_dec_float_100 rowFactor = structure.constrCoeffMatrix[i][structure.leadColIndex];
            for (int j = 0; j < structure.constrCoeffMatrix[0].count(); ++j){
                structure.constrCoeffMatrix[i][j] -= rowFactor * structure.constrCoeffMatrix[structure.leadRowIndex][j];
            }
            structure.plans[i] -= rowFactor * structure.plans[structure.leadRowIndex];
        }
        else{
            cpp_dec_float_100 rowFactor = structure.lastRow[structure.leadColIndex];
            for (int j = 0; j < structure.lastRow.count(); ++j){
                structure.lastRow[j] -= rowFactor * structure.constrCoeffMatrix[structure.leadRowIndex][j];
            }
            structure.resultValue -= rowFactor * structure.plans[structure.leadRowIndex];
        }
    }

    return IsSolved();
}

void LPMethod::ApplySignEffect()
{
    for (int i = 0; i < structure.constrCoeffMatrix.count(); ++i){
        if(structure.signs[i] == 1){
            for (int j = 0; j < structure.objFuncCoeffVector.count(); ++j){
                structure.constrCoeffMatrix[i][j] *= -1;
            }
            structure.plans[i] *= -1;
        }
    }
}

const LpStructure& LPMethod::GetAll()
{
    return structure;
}
