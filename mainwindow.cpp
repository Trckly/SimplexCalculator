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

    AppendCoefficients(2);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::keyPressEvent(QKeyEvent *event)
{
    {
        // Left Arrow Key: Switch to the previous table
        if (event->key() == Qt::Key_Left) {
            int currentIndex = ui->stackedWidget->currentIndex();
            int previousIndex = (currentIndex - 1 + ui->stackedWidget->count()) % ui->stackedWidget->count();
            ui->stackedWidget->setCurrentIndex(previousIndex);
        }
        // Right Arrow Key: Switch to the next table
        else if (event->key() == Qt::Key_Right) {
            int currentIndex = ui->stackedWidget->currentIndex();
            int nextIndex = (currentIndex + 1) % ui->stackedWidget->count();
            ui->stackedWidget->setCurrentIndex(nextIndex);
        }
    }
}

void MainWindow::onAddCoefficientButtonClicked()
{
    AppendCoefficients(1);
}

void MainWindow::AppendCoefficients(int numToAdd)
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

int MainWindow::countQLineEdits(const QList<QObject*> &children) {
    int count = 0;
    for (QObject *child : children) {
        if (qobject_cast<QLineEdit*>(child)) {
            ++count;
        }
    }
    return count;
}
