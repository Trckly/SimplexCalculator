#include "mainwindow.h"
#include "tablebuilder.h"
#include "ui_mainwindow.h"

void clearLayout(QHBoxLayout*);

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    ui->methodComboBox->addItems({"Simplex", "DualSimplex", "Gomory"});

    ui->coeffCountSpinBox->setMinimum(2);

    // Find the QStackedWidget (assuming it is named 'stackedWidget' in your .ui file)
    QStackedWidget *stackedWidget = findChild<QStackedWidget*>("tablesStackedWidget");

    if (stackedWidget) {
        // Remove all pages
        while (stackedWidget->count() > 0) {
            QWidget *page = stackedWidget->widget(0);
            stackedWidget->removeWidget(page);
            delete page; // Optional: delete the widget to free memory
        }
    }

    // Connecting the button's clicked() signal to a slot (lambda function in this case)
    connect(ui->addCoefficientButton, &QPushButton::clicked, this, &MainWindow::onAddCoefficientButtonClicked);
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::keyPressEvent(QKeyEvent *event)
{
    {
        // Left Arrow Key: Switch to the previous table
        if (event->key() == Qt::Key_A) {
            int currentIndex = ui->tablesStackedWidget->currentIndex();
            int previousIndex = (currentIndex - 1 + ui->tablesStackedWidget->count()) % ui->tablesStackedWidget->count();
            ui->tablesStackedWidget->setCurrentIndex(previousIndex);

            QString tableStr = "CT-" + QString::number(previousIndex+1);
            ui->tableLabel->setText(tableStr);
        }
        // Right Arrow Key: Switch to the next table
        else if (event->key() == Qt::Key_D) {
            int currentIndex = ui->tablesStackedWidget->currentIndex();
            int nextIndex = (currentIndex + 1) % ui->tablesStackedWidget->count();
            ui->tablesStackedWidget->setCurrentIndex(nextIndex);

            QString tableStr = "CT-" + QString::number(nextIndex+1);
            ui->tableLabel->setText(tableStr);
        }
    }
}

void MainWindow::onAddCoefficientButtonClicked()
{
    AppendConstraint();
}

void MainWindow::SetCoefficientsCount(int num)
{
    objFuncLineEditList.clear();
    clearLayout(ui->objectiveFunctionLayout);

    for (int i = 0; i < num; ++i){
        QLineEdit* LineEdit = new QLineEdit(this);
        LineEdit->setFixedSize(BOX_WIDTH, BOX_HEIGHT);
        objFuncLineEditList.append(LineEdit);

        QLabel* label = new QLabel;
        QString str;
        if(i != num - 1){
            str = "x_" + QString::number(i+1) + " + ";
            label->setFixedWidth(BOX_WIDTH + 8);
        }
        else{
            str = "x_" + QString::number(i+1);
        }
        label->setText(str);
        ui->objectiveFunctionLayout->addWidget(LineEdit);
        ui->objectiveFunctionLayout->addWidget(label);
    }
}

void MainWindow::AppendConstraint()
{
    QRect parentRect = ui->constraintVerticalLayout->geometry();

    QHBoxLayout* HBoxLayout = new QHBoxLayout();
    HBoxLayout->setGeometry(QRect(parentRect.topLeft(), QSize(parentRect.width(), 21)));
    ui->constraintVerticalLayout->addLayout(HBoxLayout);

    QVector<QLineEdit*> currentConstraintLineEditList;
    int count = 1;
    for (int i = 0; i < ui->coeffCountSpinBox->value(); ++i){
        QLineEdit* LineEdit = new QLineEdit(this);
        LineEdit->setFixedSize(BOX_WIDTH, BOX_HEIGHT);
        currentConstraintLineEditList.append(LineEdit);

        QLabel* label = new QLabel;
        QString str;
        if(i != ui->coeffCountSpinBox->value() - 1){
            str = "x_" + QString::number(count++) + " + ";
            label->setFixedWidth(BOX_WIDTH + 8);
        }
        else{
            str = "x_" + QString::number(count);
            label->setFixedWidth(BOX_WIDTH);
        }
        label->setText(str);
        HBoxLayout->addWidget(LineEdit);
        HBoxLayout->addWidget(label);
    }
    QComboBox* inequalitySign = new QComboBox;
    inequalitySign->addItems({"≤", "≥"});
    inequalitySign->setFixedSize(BOX_WIDTH * 2.5, BOX_HEIGHT);
    inequalitySignComboBoxVect.append(inequalitySign);
    HBoxLayout->addWidget(inequalitySign);

    QLineEdit* plan = new QLineEdit();
    plan->setFixedSize(BOX_WIDTH * 2.5, BOX_HEIGHT);
    planLineEditVect.append(plan);
    HBoxLayout->addWidget(plan);

    constraintsLineEditMatrix.append(currentConstraintLineEditList);
}

QVector<int> MainWindow::ConvertSigns()
{
    QVector<int> inequalitySigns;
    for (QComboBox* signComboBox : inequalitySignComboBoxVect){
        inequalitySigns.append(signComboBox->currentIndex());
    }
    return inequalitySigns;
}

// In any line edit that is empty it automatically asumes there is supposed to be zero.
// Returns first found invalid input line edit
QLineEdit* MainWindow::ReadAllInputs()
{
    QVector<float> objFuncCoefficients;
    for (int i = 0; i < objFuncLineEditList.count(); ++i){
        if(objFuncLineEditList[i]->text().isEmpty()){
            objFuncLineEditList[i]->setText("0");
        }

        bool ok;
        float temp = objFuncLineEditList[i]->text().toFloat(&ok);
        if(!ok){
            return objFuncLineEditList[i];
        }
        objFuncCoefficients.append(temp);
    }

    QVector<QVector<float>> constraintsCoefficients;
    QVector<float> plans;
    for(int i = 0; i < constraintsLineEditMatrix.count(); ++i){
        QVector<float> row;

        if(planLineEditVect[i]->text().isEmpty()){
            planLineEditVect[i]->setText("0");
        }

        bool k;
        float t = planLineEditVect[i]->text().toFloat(&k);
        if(!k){
            return planLineEditVect[i];
        }
        plans.append(t);

        for(int j = 0; j < constraintsLineEditMatrix[i].count(); ++j){
            if(constraintsLineEditMatrix[i][j]->text().isEmpty()){
                constraintsLineEditMatrix[i][j]->setText("0");
            }

            bool ok;
            float temp = constraintsLineEditMatrix[i][j]->text().toFloat(&ok);
            if(!ok){
                return constraintsLineEditMatrix[i][j];
            }
            row.append(temp);
        }
        constraintsCoefficients.append(row);
    }

    if(currentMethod == Simplex){
        lpMethod = new SimplexClass(objFuncCoefficients, constraintsCoefficients, ConvertSigns(), plans);
        qDebug() << "Simplex";
    }
    if(currentMethod == DualSimplex){
        qDebug() << "Dual";
    }
    if(currentMethod == Method::Gomory){
        qDebug() << "Gomory";
    }

    return nullptr;
}


void MainWindow::on_calculateButton_clicked()
{
    tables.clear();

    if(prevFalseLineEdit){
        prevFalseLineEdit->setStyleSheet("");
    }

    if(QLineEdit* falseLineEdit = ReadAllInputs()){
        prevFalseLineEdit = falseLineEdit;
        prevFalseLineEdit->setStyleSheet("QLineEdit { background-color: red; }");
        return;
    }

    if (currentMethod == Simplex){
        if(SimplexClass* simplexMethod = dynamic_cast<SimplexClass*>(lpMethod)){
            TableBuilder builder(lpMethod);

            int tableCounter = -1;
            do{
                tables.append(builder.ConstructTable());
                if(tableCounter >= 0)
                    builder.MarkLeadingElement(tables[tableCounter]);
                tableCounter++;
            }
            while (!simplexMethod->SolveOneStep());
            // Last table with solution
            tables.append(builder.ConstructTable());
        }
        else qDebug() << "Failed to cast LPMethod to NewSimplexClass!";
    }

    for(int i = 0; i < tables.count(); ++i){
        ui->tablesStackedWidget->addWidget(tables[i]);
    }

    QString tableStr = "CT-1";
    ui->tableLabel->setText(tableStr);
}


void MainWindow::on_previousTable_clicked()
{
    int currentIndex = ui->tablesStackedWidget->currentIndex();
    int previousIndex = (currentIndex - 1 + ui->tablesStackedWidget->count()) % ui->tablesStackedWidget->count();
    ui->tablesStackedWidget->setCurrentIndex(previousIndex);

    QString tableStr = "CT-" + QString::number(previousIndex+1);
    ui->tableLabel->setText(tableStr);
}


void MainWindow::on_nextTable_clicked()
{
    int currentIndex = ui->tablesStackedWidget->currentIndex();
    int nextIndex = (currentIndex + 1) % ui->tablesStackedWidget->count();
    ui->tablesStackedWidget->setCurrentIndex(nextIndex);

    QString tableStr = "CT-" + QString::number(nextIndex+1);
    ui->tableLabel->setText(tableStr);
}

void MainWindow::Transpose(QVector<QVector<float>> &vectorToTranspose)
{
    const QVector<QVector<float>> temp = vectorToTranspose;
    vectorToTranspose.clear();

    for (int i = 0; i < temp[0].count(); ++i){
        QVector<float> row;
        for (int j = 0; j < temp.count(); ++j){
            row.append(temp[j][i]);
        }
        vectorToTranspose.append(row);
    }
}


void MainWindow::on_coeffCountSpinBox_valueChanged(int arg)
{
    SetCoefficientsCount(arg);
}

void clearLayout(QHBoxLayout* layout) {
    QLayoutItem* item;
    while ((item = layout->takeAt(0)) != nullptr) {
        QWidget* widget = item->widget();
        if (widget) {
            widget->hide();   // Hide the widget (optional)
            widget->deleteLater();  // Schedule widget for deletion
        }
        delete item;  // Delete the layout item
    }
}

void MainWindow::on_methodComboBox_currentIndexChanged(int index)
{
    currentMethod = static_cast<Method>(index);
}

