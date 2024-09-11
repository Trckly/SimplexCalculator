#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QStackedWidget>
#include <QTableWidget>
#include <QVBoxLayout>
#include <QKeyEvent>
#include <QPushButton>
#include <QLayout>
#include <QLineEdit>
#include <QLabel>
#include "simplexclass.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    QLineEdit *ReadAllInputs();

protected:
    void keyPressEvent(QKeyEvent *event) override;

    void onAddCoefficientButtonClicked();

    void AddCoefficients(int numToAdd);

    void AppendConstraint();

private slots:

    void on_calculateButton_clicked();

private:
    Ui::MainWindow *ui;

    const int BOX_WIDTH = 25, BOX_HEIGHT = 21;

    const int COEFFICIENTS_COUNT = 4;

    QPushButton* AddCoefficientButton;

    QVector<QLineEdit*> ObjFuncLineEditList;

    QVector<QVector<QLineEdit*>> ConstraintsLineEditMatrix;

    SimplexClass* SimplexData = nullptr;

    QLineEdit* prevFalseLineEdit = nullptr;
};
#endif // MAINWINDOW_H
