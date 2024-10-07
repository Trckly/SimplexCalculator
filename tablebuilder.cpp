#include "tablebuilder.h"
#include "dualsimplexclass.h"
#include "simplexclass.h"

TableBuilder::TableBuilder(LPMethod* method, QObject *parent)
    : QObject{parent}
{
    currentMethod = method;
}

QTableWidget* TableBuilder::ConstructTable()
{
    const LpStructure& structure = currentMethod->GetAll();

    // At this point structure.constrCoeffMatrix[0].count()
    // has width like the functional part of table
    tableWidth = structure.constrCoeffMatrix[0].count() + initHeaders.count();
    // '1' is for result (Q) and lastRow
    tableLength = structure.constrCoeffMatrix.count() + 1;

    QTableWidget* table = new QTableWidget(tableLength, tableWidth);

    currentHeaders = initHeaders;
    for (int i = 0; i < tableWidth - initHeaders.count(); ++i){
        QString str = "x" + QString::number(i+1);
        currentHeaders.append(str);
    }

    table->setHorizontalHeaderLabels(currentHeaders);

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

            for (int j = 0; j < tableWidth - initHeaders.count(); ++j){
                QString conCoeffStr;
                    conCoeffStr = QString::number(structure.constrCoeffMatrix[i][j]);
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

    return table;
}

void TableBuilder::MarkLeadingElement(QTableWidget *tableToMark)
{
    if(!tableToMark){
        qDebug() << "tableToMark is null";
        return;
    }

    const LpStructure structure = currentMethod->GetAll();

    // '3' because 'Base', 'c_b' and 'Plan' are before coefficients
    int offset = 3;
    if(QTableWidgetItem* item = tableToMark->item(structure.leadRowIndex, structure.leadColIndex + offset))
        item->setBackground(QBrush(QColor(0, 125, 0)));
    else qDebug() << "Table item is null" << tableToMark->rowCount() << " " << tableToMark->columnCount()
                 << "\n" << structure.leadRowIndex << " " << structure.leadColIndex;

    AppendRatio(tableToMark, structure);
}

void TableBuilder::AppendRatio(QTableWidget* table, const LpStructure& structure)
{
    if(dynamic_cast<SimplexClass*>(currentMethod)){
        // Append horizontal ratio
        QStringList headersWithRatio = currentHeaders;
        headersWithRatio.append("Ratio");
        int newColumnCount= headersWithRatio.count();

        table->setColumnCount(newColumnCount);
        table->setHorizontalHeaderLabels(headersWithRatio);

        int j = table->columnCount() - 1;
        for (int i = 0; i < structure.ratio.count(); ++i){
            table->setItem(i, j, new QTableWidgetItem(QString::number(structure.ratio[i])));
        }
    }

    if(dynamic_cast<DualSimplexClass*>(currentMethod)){
        // Append vertical ratio
        int newRowCount = table->rowCount() + 1;
        table->setRowCount(newRowCount);

        int offset = initHeaders.count();
        for (int i = 0; i < structure.ratio.count(); ++i){
            table->setItem(newRowCount - 1, i + offset, new QTableWidgetItem(QString::number(structure.ratio[i])));
        }
    }
}
