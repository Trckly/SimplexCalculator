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
    int countQLineEdits(const QList<QObject*> &children);

protected:
    void keyPressEvent(QKeyEvent *event) override;

    void onAddCoefficientButtonClicked();

    void AppendCoefficients(int numToAdd);

private slots:

private:
    Ui::MainWindow *ui;

    const int BOX_WIDTH = 25, BOX_HEIGHT = 21;

    QPushButton* AddCoefficientButton;
};
#endif // MAINWINDOW_H
