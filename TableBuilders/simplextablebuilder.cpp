#include "simplextablebuilder.h"
#include "dualsimplexclass.h"
#include "gomoryclass.h"
#include "simplexclass.h"

SimplexTableBuilder::SimplexTableBuilder(LPMethod* method, QObject *parent)
    : QObject{parent}
{
    currentMethod = method;
}

QTableWidget* SimplexTableBuilder::ConstructTable()
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
    table->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

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
                                         : (double)structure.objFuncCoeffVector[structure.baseIndexes[i] - 1]);
            planStr = QString::number((double)structure.plans[i]);

            for (int j = 0; j < tableWidth - initHeaders.count(); ++j){
                QString conCoeffStr;
                    conCoeffStr = QString::number((double)structure.constrCoeffMatrix[i][j]);
                table->setItem(i, j + 3, new QTableWidgetItem(conCoeffStr));
            }
        }
        // Last row
        else{
            baseStr = "Q";
            c_bStr = "=";
            planStr = QString::number((double)structure.resultValue);


            for (int j = 0; j < structure.lastRow.count(); ++j){
                QString lastRowCoeffStr = QString::number((double)structure.lastRow[j]);
                table->setItem(i, j+3, new QTableWidgetItem(lastRowCoeffStr));
            }
        }
        table->setItem(i, 0, new QTableWidgetItem(baseStr));
        table->setItem(i, 1, new QTableWidgetItem(c_bStr));
        table->setItem(i, 2, new QTableWidgetItem(planStr));
    }

    return table;
}

void SimplexTableBuilder::MarkLeadingElement(QTableWidget *tableToMark)
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

void SimplexTableBuilder::AppendRatio(QTableWidget* table, const LpStructure& structure)
{
    LPMethod* method;

    if(auto GomoryMethod = dynamic_cast<GomoryClass*>(currentMethod))
        method = GomoryMethod->GetCurrentMethod();
    else
        method = currentMethod;

    if(dynamic_cast<SimplexClass*>(method)){
        // Append horizontal ratio
        QStringList headersWithRatio = currentHeaders;
        headersWithRatio.append("Ratio");
        int newColumnCount= headersWithRatio.count();

        table->setColumnCount(newColumnCount);
        table->setHorizontalHeaderLabels(headersWithRatio);

        int j = table->columnCount() - 1;
        for (int i = 0; i < structure.ratio.count(); ++i){
            table->setItem(i, j, new QTableWidgetItem(QString::number((double)structure.ratio[i])));
        }
    }

    if(dynamic_cast<DualSimplexClass*>(method)){
        // Append vertical ratio
        int newRowCount = table->rowCount() + 1;
        table->setRowCount(newRowCount);

        int offset = initHeaders.count();
        QString ratioStr;
        for (int i = 0; i < structure.ratio.count(); ++i){
            ratioStr = structure.ratio[i] < 0 ? "-" : QString::number((double)structure.ratio[i]);

            table->setItem(newRowCount - 1, i + offset, new QTableWidgetItem());

            if(i == 0)
                table->setItem(newRowCount - 1, i, new QTableWidgetItem("Ratio"));
        }
    }
}
