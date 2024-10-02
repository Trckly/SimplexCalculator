#include "tablebuilder.h"


TableBuilder::TableBuilder(LPMethod* method, QObject *parent)
    : QObject{parent}
{
    currentMethod = method;
}

void TableBuilder::ConstructTable()
{
    // QVector<float> objFuncCoeffVector;
    // QVector<QVector<float>> constrCoeffMatrix;
    // QVector<int> signs;
    // QVector<float> plans;
    // int leadRowIndex;
    // int leadColIndex;
    // QVector<int> baseIndexes;
    // float resultValue;
    // QVector<float> ratio;
    // QVector<float> lastRow;

    LpStructure structure = currentMethod->GetAll();

    int headersCount = headers.count();
    tableWidth = structure.constrCoeffMatrix[0].count() + structure.constrCoeffMatrix.count() + headersCount;
    // '1' is for result (Q) and lastRow
    tableLength = structure.constrCoeffMatrix.count() + 1;

    QTableWidget* table = new QTableWidget(tableLength, tableWidth);

    for (int i = 0; i < tableWidth - headersCount; ++i){
        QString str = "x" + QString::number(i+1);
        headers.insert(headers.count() - 1, str);
    }

    table->setHorizontalHeaderLabels(headers);

    QStringList rowHeaders;
    for (int i = 0; i < table->rowCount(); ++i){
        rowHeaders.append("");
    }

    table->setVerticalHeaderLabels(rowHeaders);

    // Column indexes of table are highly depended on headers QStringList above
    for (int i = 0; i < tableLength; ++i){
        // Base/c_b fill
        QString baseStr, c_bStr, planStr;
        // Without last row
        if(i < structure.baseIndexes.count()){
            baseStr = "x" + QString::number(structure.baseIndexes[i]);
            c_bStr = QString::number(structure.baseIndexes[i] > structure.objFuncCoeffVector.count()
                                         ? 0
                                         : structure.objFuncCoeffVector[structure.baseIndexes[i] - 1]);
            planStr = QString::number(structure.plans[i]);

            for (int j = 0; j < tableWidth - headersCount; ++j){
                QString conCoeffStr;
                if(j < structure.objFuncCoeffVector.count()){
                    conCoeffStr = QString::number(structure.constrCoeffMatrix[i][j]);
                }
                else if(j + 1 == structure.baseIndexes[i]){
                    conCoeffStr = "1";
                    structure.constrCoeffMatrix[i][j] = 1.f;
                }
                else{
                    conCoeffStr = QString::number(structure.constrCoeffMatrix[i][j]);
                }
                table->setItem(i, j + 3, new QTableWidgetItem(conCoeffStr));
            }
        }
        // Last row
        else{
            baseStr = "Q";
            c_bStr = "=";
            planStr = QString::number(structure.resultValue);


            for (int j = 0; j < structure.lastRow.count(); ++j){
                QString lastRowCoeffStr = QString::number(structure.lastRow[j]);
                table->setItem(i, j+3, new QTableWidgetItem(lastRowCoeffStr));
            }
        }
        table->setItem(i, 0, new QTableWidgetItem(baseStr));
        table->setItem(i, 1, new QTableWidgetItem(c_bStr));
        table->setItem(i, 2, new QTableWidgetItem(planStr));
    }
}
