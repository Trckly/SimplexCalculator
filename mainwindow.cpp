#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // Find the QStackedWidget (assuming it is named 'stackedWidget' in your .ui file)
    QStackedWidget *stackedWidget = findChild<QStackedWidget*>("stackedWidget");

    if (stackedWidget) {
        // Remove all pages
        while (stackedWidget->count() > 0) {
            QWidget *page = stackedWidget->widget(0);
            stackedWidget->removeWidget(page);
            delete page; // Optional: delete the widget to free memory
        }
    }

    AddCoefficientButton = new QPushButton();
    AddCoefficientButton->setText("+");
    AddCoefficientButton->setFixedSize(BOX_WIDTH, BOX_HEIGHT+10);
    ui->objectiveFunctionLayout->addWidget(AddCoefficientButton);

    // Connecting the button's clicked() signal to a slot (lambda function in this case)
    connect(AddCoefficientButton, &QPushButton::clicked, this, &MainWindow::onAddCoefficientButtonClicked);

    AddCoefficients(COEFFICIENTS_COUNT);
}

MainWindow::~MainWindow()
{
    delete ui;
}

// In any line edit that is empty it automatically asumes there is supposed to be zero.
// Returns first found invalid input line edit
QLineEdit* MainWindow::ReadAllInputs()
{
    QVector<float> objFuncCoefficients;
    for (int i = 0; i < ObjFuncLineEditList.count(); ++i){
        if(ObjFuncLineEditList[i]->text().isEmpty()){
            ObjFuncLineEditList[i]->setText("0");
        }

        bool ok;
        float temp = ObjFuncLineEditList[i]->text().toFloat(&ok);
        if(!ok){
            return ObjFuncLineEditList[i];
        }
        objFuncCoefficients.append(temp);

        qDebug() << temp << " ";
    }

    return nullptr;
}

void MainWindow::keyPressEvent(QKeyEvent *event)
{
    {
        // Left Arrow Key: Switch to the previous table
        if (event->key() == Qt::Key_Left) {
            int currentIndex = ui->tablesStackedWidget->currentIndex();
            int previousIndex = (currentIndex - 1 + ui->tablesStackedWidget->count()) % ui->tablesStackedWidget->count();
            ui->tablesStackedWidget->setCurrentIndex(previousIndex);
        }
        // Right Arrow Key: Switch to the next table
        else if (event->key() == Qt::Key_Right) {
            int currentIndex = ui->tablesStackedWidget->currentIndex();
            int nextIndex = (currentIndex + 1) % ui->tablesStackedWidget->count();
            ui->tablesStackedWidget->setCurrentIndex(nextIndex);
        }
    }
}

void MainWindow::onAddCoefficientButtonClicked()
{
    AppendConstraint();
}

void MainWindow::AddCoefficients(int numToAdd)
{
    int count = 1;
    for (int i = 0; i < numToAdd; ++i){
        QLineEdit* LineEdit = new QLineEdit(this);
        LineEdit->setFixedSize(BOX_WIDTH, BOX_HEIGHT);
        ObjFuncLineEditList.append(LineEdit);

        QLabel* label = new QLabel;
        QString str;
        if(i != numToAdd - 1){
            str = "x_" + QString::number(count++) + " + ";
            label->setFixedWidth(BOX_WIDTH + 8);
        }
        else{
            str = "x_" + QString::number(count);
        }
        label->setText(str);
        ui->objectiveFunctionLayout->insertWidget(ui->objectiveFunctionLayout->count() - 1, LineEdit);
        ui->objectiveFunctionLayout->insertWidget(ui->objectiveFunctionLayout->count() - 1, label);
    }
}

void MainWindow::AppendConstraint()
{
    QRect parentRect = ui->constraintVerticalLayout->geometry();

    QHBoxLayout* HBoxLayout = new QHBoxLayout();
    HBoxLayout->setGeometry(QRect(parentRect.topLeft(), QSize(parentRect.width(), 21)));
    ui->constraintVerticalLayout->addLayout(HBoxLayout);

    // ConstraintsLineEditMatrix.append(QVector<QLineEdit*>());

    QVector<QLineEdit*> currentConstraintLineEditList;
    int count = 1;
    for (int i = 0; i < COEFFICIENTS_COUNT; ++i){
        QLineEdit* LineEdit = new QLineEdit(this);
        LineEdit->setFixedSize(BOX_WIDTH, BOX_HEIGHT);
        currentConstraintLineEditList.append(LineEdit);

        QLabel* label = new QLabel;
        QString str;
        if(i != COEFFICIENTS_COUNT - 1){
            str = "x_" + QString::number(count++) + " + ";
            label->setFixedWidth(BOX_WIDTH + 8);
        }
        else{
            str = "x_" + QString::number(count);
        }
        label->setText(str);
        HBoxLayout->addWidget(LineEdit);
        HBoxLayout->addWidget(label);
    }
    ConstraintsLineEditMatrix.append(currentConstraintLineEditList);
}

void MainWindow::on_calculateButton_clicked()
{
    if(prevFalseLineEdit){
        prevFalseLineEdit->setStyleSheet("");
    }

    if(QLineEdit* falseLineEdit = ReadAllInputs()){
        prevFalseLineEdit = falseLineEdit;
        prevFalseLineEdit->setStyleSheet("QLineEdit { background-color: red; }");
        return;
    }
}

