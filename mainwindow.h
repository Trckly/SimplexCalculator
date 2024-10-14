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
#include <QComboBox>
#include "simplexclass.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

enum Method{
    Simplex,
    DualSimplex,
    Gomory,
    PotentialsTransportation
};

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

    void SetCoefficientsCount(int numToAdd);

    void AppendConstraint();

    QVector<int> ConvertSigns();

    static void Transpose(QVector<QVector<double>> &vectorToTranspose);

    // General
    void ClearUI();

private slots:

    void on_calculateButton_clicked();

    void on_previousTable_clicked();

    void on_nextTable_clicked();

    void on_coeffCountSpinBox_valueChanged(int arg1);

    void on_methodComboBox_currentIndexChanged(int index);

private:
    Ui::MainWindow *ui;

    const int BOX_WIDTH = 25, BOX_HEIGHT = 21;

    const int COEFFICIENTS_COUNT = 2;

    LPMethod* lpMethod = nullptr;

    QVector<QLineEdit*> objFuncLineEditList;

    QVector<QVector<QLineEdit*>> constraintsLineEditMatrix;

    QLineEdit* prevFalseLineEdit = nullptr;

    QVector<QComboBox*> inequalitySignComboBoxVect;

    QVector<QLineEdit*> planLineEditVect;

    QVector<QTableWidget*> tables;

    Method currentMethod;
};
#endif // MAINWINDOW_H
