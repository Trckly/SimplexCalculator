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

void SimplexClass::DebugOutput()
{
    QString objFuncDebugStr = "";
    for (int i = 0; i < objFuncCoeffVector.count(); ++i){
        objFuncDebugStr += QString::number(objFuncCoeffVector[i]) + " ";
    }
    objFuncDebugStr += "\n";
    qDebug() << objFuncDebugStr;

    QString constraintsDebugStr = "";
    QString signsStr = "";
    QString plansStr = "";
    for (int i = 0; i < constrCoeffMatrix.count(); ++i){
        signsStr += QString::number(signs[i]) + " ";

        plansStr += QString::number(plans[i]) + " ";

        for (int j = 0; j < constrCoeffMatrix[i].count(); ++j){
            constraintsDebugStr += QString::number(constrCoeffMatrix[i][j]) + " ";
        }
        constraintsDebugStr += "\n";
    }
    qDebug() << constraintsDebugStr;

    qDebug() << signsStr;

    qDebug() << plansStr;
}

QVector<QTableWidget *> SimplexClass::BuildTables()
{
    QVector<QTableWidget*> tables;
    QPoint tableDimentions = CalculateTableDimentions();

    bool stop = false;
    for(int i = 0; !stop; ++i){
        if(QTableWidget* table = ConstructTable(tableDimentions)){
            tables.append(table);
            stop = true;
        }
        else {
            qDebug() << "Error with construction of table template!";
        }
    }

    return tables;
}

QPoint SimplexClass::CalculateTableDimentions()
{
    int columns = constrCoeffMatrix[0].count() + 4;
    int rows = constrCoeffMatrix.count() + 1;

    return QPoint(rows, columns);
}

QTableWidget *SimplexClass::ConstructTable(QPoint Dimentions)
{
    QTableWidget* table = new QTableWidget(Dimentions.rx(), Dimentions.ry());

    QStringList headers = {"Base", "c_b", "Plan", "Ratio"};
    int headersCountSnapshot = headers.count();
    for (int i = 0; i < Dimentions.ry() - headersCountSnapshot; ++i){
        QString str = "x" + QString::number(i+1);
        headers.insert(headers.count() - 1, str);
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
            baseStr = "x" + QString::number(baseIndexes[i]);
            c_bStr = QString::number(baseIndexes[i] >= plans.count() ? 0 : plans[baseIndexes[i]]);
            planStr = QString::number(plans[i]);


            for (int j = 0; j < Dimentions.ry() - headersCountSnapshot; ++j){
                QString conCoeffStr;
                if(j < objFuncCoeffVector.count()){
                    conCoeffStr = QString::number(constrCoeffMatrix[i][j]);
                }
                else if(j + 1 == baseIndexes[i]){
                    conCoeffStr = "1";
                    constrCoeffMatrix[i].append(1.f);
                }
                else{
                    conCoeffStr = "0";
                    constrCoeffMatrix[i].append(0.f);
                }
                table->setItem(i, j + 3, new QTableWidgetItem(conCoeffStr));
            }
        }
        // Last row
        else{
            baseStr = "Q";
            c_bStr = "=";
            planStr = QString::number(QValue);


            for (int j = 0; j < lastRow.count(); ++j){
                QString lastRowCoeffStr = QString::number(lastRow[j]);
                    table->setItem(i, j+3, new QTableWidgetItem(lastRowCoeffStr));
            }
        }
        table->setItem(i, 0, new QTableWidgetItem(baseStr));
        table->setItem(i, 1, new QTableWidgetItem(c_bStr));
        table->setItem(i, 2, new QTableWidgetItem(planStr));

    }

    // Adjust the size of the cells
    table->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    // table->verticalHeader()->setSectionResizeMode(QHeaderView::Stretch);

    return table;
}

bool SimplexClass::SimplexAlgorithm()
{
    return 1;
}
