#include "simplexclass.h"
#include "qheaderview.h"

SimplexClass::SimplexClass(QObject *parent)
    : QObject{parent}
{}

SimplexClass::SimplexClass(QVector<float> objCoeffVector, QVector<QVector<float> > constrCoeffVector, QVector<QComboBox *> signsVector, QVector<float> plansVector)
{
    SetObjectiveCoefficientVector(objCoeffVector);
    SetConstraintsCoefficientMatrix(constrCoeffVector);
    SetSigns(signsVector);
    SetPlans(plansVector);

    for(int i = 1; i <= constrCoeffVector.count(); ++i){
        baseIndexes.append(i + objCoeffVector.count());
    }

    int lastRowSize = constrCoeffMatrix.count() + objFuncCoeffVector.count();
    for(int i = 0; i < lastRowSize; ++i){
        lastRow.append(i < objCoeffVector.count() ? -objCoeffVector[i] : 0);
    }

    ratio.resize(lastRow.count(), 0.f);
}

void SimplexClass::SetObjectiveCoefficientVector(QVector<float> otherVector)
{
    objFuncCoeffVector.clear();
    objFuncCoeffVector = otherVector;
}

void SimplexClass::SetConstraintsCoefficientMatrix(QVector<QVector<float> > otherMatrix)
{
    constrCoeffMatrix.clear();
    constrCoeffMatrix = otherMatrix;

    int newSize = otherMatrix.count() + objFuncCoeffVector.count();
    for (int i = 0; i < otherMatrix.count(); ++i){
        constrCoeffMatrix[i].resize(newSize, 0.f);
    }
}

void SimplexClass::SetSigns(QVector<QComboBox *> signsComboBoxVector)
{
    signs.clear();

    for (int i = 0; i < signsComboBoxVector.count(); ++i){
        signs.append(signsComboBoxVector[i]->currentIndex());
    }
}

void SimplexClass::SetPlans(QVector<float> otherVector)
{
    plans.clear();
    plans = otherVector;
}

QVector<QTableWidget *> SimplexClass::BuildTables()
{
    QVector<QTableWidget*> tables;
    QPoint tableDimentions = CalculateTableDimentions();

    if(QTableWidget* table = ConstructTable(tableDimentions, false)){
        tables.append(table);

        while(true){
            bool quit = SimplexAlgorithm();

            if(QTableWidget* table = ConstructTable(tableDimentions, quit)){
                tables.append(table);
                if(quit){
                    break;
                }
            }
            else {
                qDebug() << "Error with construction of table template!";
            }
        }
        return tables;
    }
}

QPoint SimplexClass::CalculateTableDimentions()
{
    int columns = constrCoeffMatrix[0].count() + 3;
    int rows = constrCoeffMatrix.count() + 2;

    return QPoint(rows, columns);
}

QTableWidget *SimplexClass::ConstructTable(QPoint Dimentions, bool lastTable)
{
    QTableWidget* table = new QTableWidget(Dimentions.rx(), Dimentions.ry());

    QStringList headers = {"Base", "c_b", "Plan"};
    int headersCountSnapshot = headers.count();
    for (int i = 0; i < Dimentions.ry() - headersCountSnapshot; ++i){
        QString str = "y" + QString::number(i+1);
        headers.insert(headers.count(), str);
    }

    table->setHorizontalHeaderLabels(headers);

    QStringList rowHeaders;
    for (int i = 0; i < table->rowCount(); ++i){
        rowHeaders.append("");
    }

    table->setVerticalHeaderLabels(rowHeaders);

    // Column indexes of table are depended of headers QStringList above
    for (int i = 0; i < Dimentions.rx(); ++i){

        // Base/c_b fill
        QString baseStr, c_bStr, planStr;
        // Without last row
        if(i < baseIndexes.count()){
            baseStr = "y" + QString::number(baseIndexes[i]);
            c_bStr = QString::number(baseIndexes[i] > objFuncCoeffVector.count() ? 0 : objFuncCoeffVector[baseIndexes[i] - 1]);
            planStr = QString::number(plans[i]);


            for (int j = 0; j < Dimentions.ry() - headersCountSnapshot; ++j){
                QString conCoeffStr;
                if(j < objFuncCoeffVector.count()){
                    conCoeffStr = QString::number(constrCoeffMatrix[i][j]);
                }
                else if(j + 1 == baseIndexes[i]){
                    conCoeffStr = "1";
                    constrCoeffMatrix[i][j] = 1.f;
                }
                else{
                    conCoeffStr = QString::number(constrCoeffMatrix[i][j]);
                }
                table->setItem(i, j + 3, new QTableWidgetItem(conCoeffStr));
            }
        }
        // Last row
        else if(i == baseIndexes.count()){
            baseStr = "F";
            c_bStr = "=";
            planStr = QString::number(QValue);


            for (int j = 0; j < lastRow.count(); ++j){
                QString lastRowCoeffStr = QString::number(lastRow[j]);
                    table->setItem(i, j+3, new QTableWidgetItem(lastRowCoeffStr));
            }
        }
        else{
            baseStr = "Ratio";
            c_bStr = "";
            planStr = "";
        }
        table->setItem(i, 0, new QTableWidgetItem(baseStr));
        table->setItem(i, 1, new QTableWidgetItem(c_bStr));
        table->setItem(i, 2, new QTableWidgetItem(planStr));
    }

    // Adjust the size of the cells
    table->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    // table->verticalHeader()->setSectionResizeMode(QHeaderView::Stretch);

    if(!lastTable){
        leadingRowIndex = GetMinRowIndex();

        leadingColIndex = GetMinColumnIndex(leadingRowIndex);

        leadingElement = constrCoeffMatrix[leadingRowIndex][leadingColIndex];

        table->item(leadingRowIndex, leadingColIndex + 3)->setBackground(QBrush(QColor(200, 255, 200)));

        QString ratioStr;
        for (int i = 0; i < ratio.count(); ++i){
            ratio[i] < 0 ? ratioStr = "-" : ratioStr = QString::number(ratio[i]);

            table->setItem(table->rowCount() - 1,  i + 3, new QTableWidgetItem(ratioStr));
        }
    }

    return table;
}

bool SimplexClass::SimplexAlgorithm()
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
            QValue -= rowFactor * plans[leadingRowIndex];
        }
    }
    return IsSolved();
}

int SimplexClass::GetMinRowIndex(float *minValue)
{
    float min = 0;
    int minIndex = 0;
    for (int i = 0; i < plans.count(); ++i){
        if(i == 0){
            min = plans[i];
            minIndex = i;
            continue;
        }
        if(plans[i] < min){
            min = plans[i];
            minIndex = i;
        }
    }
    if(minValue){
        *minValue = min;
    }
    return minIndex;
}

int SimplexClass::GetMinColumnIndex(int rowIndex, float *minValue)
{
    float min = std::numeric_limits<float>::max(), tempRatio;
    int minIndex = 0;
    for (int i = 0; i < constrCoeffMatrix[0].count(); ++i){
        tempRatio = -1;
        if(constrCoeffMatrix[rowIndex][i] < 0){
            tempRatio = -lastRow[i] / constrCoeffMatrix[rowIndex][i];
            if(tempRatio < min){
                min = tempRatio;
                minIndex = i;
            }
        }
        ratio[i] = tempRatio;
    }
    if(minValue){
        *minValue = min;
    }
    return minIndex;
}

bool SimplexClass::IsSolved()
{
    bool bSolved = true;
    for (int i = 0; i < plans.count(); ++i){
        plans[i] < 0 ? bSolved = false : bSolved;
    }

    return bSolved;
}
